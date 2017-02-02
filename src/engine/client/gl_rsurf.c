/*
gl_rsurf.c - surface-related refresh code
Copyright (C) 2010 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "qt/c_gate.h"

#include "client.h"
#include "mod_local.h"
#include "mathlib.h"

//typedef struct
//{
//	int		allocated[BLOCK_SIZE_MAX];
//	int		current_lightmap_texture;
//	msurface_t	*dynamic_surfaces;
//	msurface_t	*lightmap_surfaces[MAX_LIGHTMAPS];
//	byte		lightmap_buffer[BLOCK_SIZE_MAX*BLOCK_SIZE_MAX*4];
//} gllightmapstate_t;

int		nColinElim; // stats
vec2_t		world_orthocenter;
vec2_t		world_orthohalf;
byte		visbytes[MAX_MAP_LEAFS/8];
uint		r_blocklights[BLOCK_SIZE_MAX*BLOCK_SIZE_MAX*3];
glpoly_t		*fullbright_polys[MAX_TEXTURES];
qboolean		draw_fullbrights = false;
mextrasurf_t		*detail_surfaces[MAX_TEXTURES];
qboolean		draw_details = false;
msurface_t		*skychain = NULL;
gllightmapstate_t	gl_lms;

//static void LM_UploadBlock( int lightmapnum );

byte *__cdecl Mod_GetCurrentVis( void )
{
	return Mod_LeafPVS( r_viewleaf, cl.worldmodel );
}

void __cdecl Mod_SetOrthoBounds( float *mins, float *maxs )
{
	if( clgame.drawFuncs.GL_OrthoBounds )
	{
		clgame.drawFuncs.GL_OrthoBounds( mins, maxs );
	}

	Vector2Average( maxs, mins, world_orthocenter );
	Vector2Subtract( maxs, world_orthocenter, world_orthohalf );
}

static void __cdecl BoundPoly( int numverts, float *verts, vec3_t mins, vec3_t maxs )
{
	int	i, j;
	float	*v;

	mins[0] = mins[1] = mins[2] = 99999.0f;
	maxs[0] = maxs[1] = maxs[2] = -99999.0f;

	for( i = 0, v = verts; i < numverts; i++ )
	{
		for( j = 0; j < 3; j++, v++ )
		{
			if( *v < mins[j] ) mins[j] = *v;
			if( *v > maxs[j] ) maxs[j] = *v;
		}
	}
}

static void SubdividePolygon_r( msurface_t *warpface, int numverts, float *verts )
{
	int	i, j, k, f, b;
	vec3_t	mins, maxs;
	float	m, frac, s, t, *v, vertsDiv;
	vec3_t	front[SUBDIVIDE_SIZE], back[SUBDIVIDE_SIZE], total;
	float	dist[SUBDIVIDE_SIZE], total_s, total_t, total_ls, total_lt;
	glpoly_t	*poly;

	if( numverts > ( SUBDIVIDE_SIZE - 4 ))
		Host_Error( "Mod_SubdividePolygon: too many vertexes on face ( %i )\n", numverts );

	BoundPoly( numverts, verts, mins, maxs );

	for( i = 0; i < 3; i++ )
	{
		m = ( mins[i] + maxs[i] ) * 0.5f;
		m = SUBDIVIDE_SIZE * floorf( m / SUBDIVIDE_SIZE + 0.5f );
		if( maxs[i] - m < 8 ) continue;
		if( m - mins[i] < 8 ) continue;

		// cut it
		v = verts + i;
		for( j = 0; j < numverts; j++, v += 3 )
			dist[j] = *v - m;

		// wrap cases
		dist[j] = dist[0];
		v -= i;
		VectorCopy( verts, v );

		f = b = 0;
		v = verts;
		for( j = 0; j < numverts; j++, v += 3 )
		{
			if( dist[j] >= 0 )
			{
				VectorCopy( v, front[f] );
				f++;
			}

			if( dist[j] <= 0 )
			{
				VectorCopy (v, back[b]);
				b++;
			}

			if( dist[j] == 0 || dist[j+1] == 0 )
				continue;

			if(( dist[j] > 0 ) != ( dist[j+1] > 0 ))
			{
				// clip point
				frac = dist[j] / ( dist[j] - dist[j+1] );
				for( k = 0; k < 3; k++ )
					front[f][k] = back[b][k] = v[k] + frac * (v[3+k] - v[k]);
				f++;
				b++;
			}
		}

		SubdividePolygon_r( warpface, f, front[0] );
		SubdividePolygon_r( warpface, b, back[0] );
		return;
	}

	// add a point in the center to help keep warp valid
	poly = Mem_Alloc( loadmodel->mempool, sizeof( glpoly_t ) + ((numverts-4)+2) * VERTEXSIZE * sizeof( float ));
	poly->next = warpface->polys;
	poly->flags = warpface->flags;
	warpface->polys = poly;
	poly->numverts = numverts + 2;
	VectorClear( total );
	total_s = total_ls = 0.0f;
	total_t = total_lt = 0.0f;

	for( i = 0; i < numverts; i++, verts += 3 )
	{
		VectorCopy( verts, poly->verts[i+1] );
		VectorAdd( total, verts, total );

		if( warpface->flags & SURF_DRAWTURB )
		{
			s = DotProduct( verts, warpface->texinfo->vecs[0] );
			t = DotProduct( verts, warpface->texinfo->vecs[1] );
		}
		else
		{
			s = DotProduct( verts, warpface->texinfo->vecs[0] ) + warpface->texinfo->vecs[0][3];
			t = DotProduct( verts, warpface->texinfo->vecs[1] ) + warpface->texinfo->vecs[1][3];
			s /= warpface->texinfo->texture->width;
			t /= warpface->texinfo->texture->height;
		}

		poly->verts[i+1][3] = s;
		poly->verts[i+1][4] = t;

		total_s += s;
		total_t += t;

		// for speed reasons
		if( !( warpface->flags & SURF_DRAWTURB ))
		{
			// lightmap texture coordinates
			s = DotProduct( verts, warpface->texinfo->vecs[0] ) + warpface->texinfo->vecs[0][3];
			s -= warpface->texturemins[0];
			s += warpface->light_s * LM_SAMPLE_SIZE;
			s += LM_SAMPLE_SIZE >> 1;
			s /= BLOCK_SIZE * LM_SAMPLE_SIZE; //fa->texinfo->texture->width;

			t = DotProduct( verts, warpface->texinfo->vecs[1] ) + warpface->texinfo->vecs[1][3];
			t -= warpface->texturemins[1];
			t += warpface->light_t * LM_SAMPLE_SIZE;
			t += LM_SAMPLE_SIZE >> 1;
			t /= BLOCK_SIZE * LM_SAMPLE_SIZE; //fa->texinfo->texture->height;

			poly->verts[i+1][5] = s;
			poly->verts[i+1][6] = t;

			total_ls += s;
			total_lt += t;
		}
	}

	vertsDiv = ( 1.0f / (float)numverts );

	VectorScale( total, vertsDiv, poly->verts[0] );
	poly->verts[0][3] = total_s * vertsDiv;
	poly->verts[0][4] = total_t * vertsDiv;

	if( !( warpface->flags & SURF_DRAWTURB ))
	{
		poly->verts[0][5] = total_ls * vertsDiv;
		poly->verts[0][6] = total_lt * vertsDiv;
	}

	// copy first vertex to last
	Q_memcpy( poly->verts[i+1], poly->verts[1], sizeof( poly->verts[0] ));
}

/*
================
GL_SubdivideSurface

Breaks a polygon up along axial 64 unit
boundaries so that turbulent and sky warps
can be done reasonably.
================
*/
void GL_SubdivideSurface( msurface_t *fa )
{
	vec3_t	verts[SUBDIVIDE_SIZE];
	int	numverts;
	int	i, lindex;
	float	*vec;

	// convert edges back to a normal polygon
	numverts = 0;
	for( i = 0; i < fa->numedges; i++ )
	{
		lindex = loadmodel->surfedges[fa->firstedge + i];

		if( lindex > 0 ) vec = loadmodel->vertexes[loadmodel->edges[lindex].v[0]].position;
		else vec = loadmodel->vertexes[loadmodel->edges[-lindex].v[1]].position;
		VectorCopy( vec, verts[numverts] );
		numverts++;
	}

	// do subdivide
	SubdividePolygon_r( fa, numverts, verts[0] );
}

/*
================
GL_BuildPolygonFromSurface
================
*/
void GL_BuildPolygonFromSurface( model_t *mod, msurface_t *fa )
{
	int		i, lindex, lnumverts;
	medge_t		*pedges, *r_pedge;
	int		vertpage;
	texture_t		*tex;
	gltexture_t	*glt;
	float		*vec;
	float		s, t;
	glpoly_t		*poly;

	// already created
	if( !mod || fa->polys ) return;

	if( !fa->texinfo || !fa->texinfo->texture )
		return; // bad polygon ?

	if( fa->flags & SURF_CONVEYOR && fa->texinfo->texture->gl_texturenum != 0 )
	{
		glt = R_GetTexture( fa->texinfo->texture->gl_texturenum );
		tex = fa->texinfo->texture;
		ASSERT( glt != NULL && tex != NULL );

		// update conveyor widths for keep properly speed of scrolling
		glt->srcWidth = tex->width;
		glt->srcHeight = tex->height;
	}

	// reconstruct the polygon
	pedges = mod->edges;
	lnumverts = fa->numedges;
	vertpage = 0;

	// draw texture
	poly = Mem_Alloc( mod->mempool, sizeof( glpoly_t ) + ( lnumverts - 4 ) * VERTEXSIZE * sizeof( float ));
	poly->next = fa->polys;
	poly->flags = fa->flags;
	fa->polys = poly;
	poly->numverts = lnumverts;

	for( i = 0; i < lnumverts; i++ )
	{
		lindex = mod->surfedges[fa->firstedge + i];

		if( lindex > 0 )
		{
			r_pedge = &pedges[lindex];
			vec = mod->vertexes[r_pedge->v[0]].position;
		}
		else
		{
			r_pedge = &pedges[-lindex];
			vec = mod->vertexes[r_pedge->v[1]].position;
		}

		s = DotProduct( vec, fa->texinfo->vecs[0] ) + fa->texinfo->vecs[0][3];
		s /= fa->texinfo->texture->width;

		t = DotProduct( vec, fa->texinfo->vecs[1] ) + fa->texinfo->vecs[1][3];
		t /= fa->texinfo->texture->height;

		VectorCopy( vec, poly->verts[i] );
		poly->verts[i][3] = s;
		poly->verts[i][4] = t;

		// lightmap texture coordinates
		s = DotProduct( vec, fa->texinfo->vecs[0] ) + fa->texinfo->vecs[0][3];
		s -= fa->texturemins[0];
		s += fa->light_s * LM_SAMPLE_SIZE;
		s += LM_SAMPLE_SIZE >> 1;
		s /= BLOCK_SIZE * LM_SAMPLE_SIZE; //fa->texinfo->texture->width;

		t = DotProduct( vec, fa->texinfo->vecs[1] ) + fa->texinfo->vecs[1][3];
		t -= fa->texturemins[1];
		t += fa->light_t * LM_SAMPLE_SIZE;
		t += LM_SAMPLE_SIZE >> 1;
		t /= BLOCK_SIZE * LM_SAMPLE_SIZE; //fa->texinfo->texture->height;

		poly->verts[i][5] = s;
		poly->verts[i][6] = t;
	}

	// remove co-linear points - Ed
	if( !gl_keeptjunctions->integer && !( fa->flags & SURF_UNDERWATER ))
	{
		for( i = 0; i < lnumverts; i++ )
		{
			vec3_t	v1, v2;
			float	*prev, *this, *next;

			prev = poly->verts[(i + lnumverts - 1) % lnumverts];
			next = poly->verts[(i + 1) % lnumverts];
			this = poly->verts[i];

			VectorSubtract( this, prev, v1 );
			VectorNormalize( v1 );
			VectorSubtract( next, prev, v2 );
			VectorNormalize( v2 );

			// skip co-linear points
			if(( fabs( v1[0] - v2[0] ) <= 0.001f) && (fabs( v1[1] - v2[1] ) <= 0.001f) && (fabs( v1[2] - v2[2] ) <= 0.001f))
			{
				int	j, k;

				for( j = i + 1; j < lnumverts; j++ )
				{
					for( k = 0; k < VERTEXSIZE; k++ )
						poly->verts[j-1][k] = poly->verts[j][k];
				}

				// retry next vertex next time, which is now current vertex
				lnumverts--;
				nColinElim++;
				i--;
			}
		}
	}

	poly->numverts = lnumverts;
}

/*
===============
R_TextureAnimation

Returns the proper texture for a given time and base texture
===============
*/
texture_t *R_TextureAnimation( texture_t *base, int surfacenum )
{
	int	reletive;
	int	count, speed;

	// random tiling textures
	if( base->anim_total < 0 )
	{
		reletive = abs( surfacenum ) % abs( base->anim_total );
		count = 0;

		while( base->anim_min > reletive || base->anim_max <= reletive )
		{
			base = base->anim_next;
			if( !base ) Host_Error( "R_TextureRandomTiling: broken loop\n" );
			if( ++count > 100 ) Host_Error( "R_TextureRandomTiling: infinite loop\n" );
		}
		return base;
	}

	if( RI.currententity->curstate.frame )
	{
		if( base->alternate_anims )
			base = base->alternate_anims;
	}

	if( !base->anim_total )
		return base;

	// GoldSrc and Quake1 has different animating speed
	if( world.sky_sphere || world.version == Q1BSP_VERSION )
		speed = 10;
	else speed = 20;

	reletive = (int)(cl.time * speed) % base->anim_total;
	count = 0;

	while( base->anim_min > reletive || base->anim_max <= reletive )
	{
		base = base->anim_next;
		if( !base ) Host_Error( "R_TextureAnimation: broken loop\n" );
		if( ++count > 100 ) Host_Error( "R_TextureAnimation: infinite loop\n" );
	}
	return base;
}

/*
===============
R_AddDynamicLights
===============
*/
void R_AddDynamicLights( msurface_t *surf )
{
	float		dist, rad, minlight;
	int		lnum, s, t, sd, td, smax, tmax;
	float		sl, tl, sacc, tacc;
	vec3_t		impact, origin_l;
	mtexinfo_t	*tex;
	dlight_t		*dl;
	uint		*bl;

	// no dlighted surfaces here
	if( !R_CountSurfaceDlights( surf )) return;

	smax = (surf->extents[0] / LM_SAMPLE_SIZE) + 1;
	tmax = (surf->extents[1] / LM_SAMPLE_SIZE) + 1;
	tex = surf->texinfo;

	for( lnum = 0; lnum < MAX_DLIGHTS; lnum++ )
	{
		if(!( surf->dlightbits & BIT( lnum )))
			continue;	// not lit by this light

		dl = &cl_dlights[lnum];

		// transform light origin to local bmodel space
		if( !tr.modelviewIdentity )
			Matrix4x4_VectorITransform( RI.objectMatrix, dl->origin, origin_l );
		else VectorCopy( dl->origin, origin_l );

		rad = dl->radius;
		dist = PlaneDiff( origin_l, surf->plane );
		rad -= fabsf( dist );

		// rad is now the highest intensity on the plane
		minlight = dl->minlight;
		if( rad < minlight )
			continue;

		minlight = rad - minlight;

		if( surf->plane->type < 3 )
		{
			VectorCopy( origin_l, impact );
			impact[surf->plane->type] -= dist;
		}
		else VectorMA( origin_l, -dist, surf->plane->normal, impact );

		sl = DotProduct( impact, tex->vecs[0] ) + tex->vecs[0][3] - surf->texturemins[0];
		tl = DotProduct( impact, tex->vecs[1] ) + tex->vecs[1][3] - surf->texturemins[1];

		bl = r_blocklights;
		for( t = 0, tacc = 0; t < tmax; t++, tacc += LM_SAMPLE_SIZE )
		{
			td = (int)(tl - tacc);
			if( td < 0 ) td = -td;

			for( s = 0, sacc = 0; s < smax; s++, sacc += LM_SAMPLE_SIZE, bl += 3 )
			{
				sd = (int)(sl - sacc);
				if( sd < 0 ) sd = -sd;

				if( sd > td ) dist = (float)(sd + (td >> 1));
				else dist = (float)(td + (sd >> 1));

				if( dist < minlight )
				{
					bl[0] += (uint)(( rad - dist ) * TextureToTexGamma( dl->color.r ));
					bl[1] += (uint)(( rad - dist ) * TextureToTexGamma( dl->color.g ));
					bl[2] += (uint)(( rad - dist ) * TextureToTexGamma( dl->color.b ));
				}
			}
		}
	}
}

/*
================
R_SetCacheState
================
*/
void __cdecl R_SetCacheState( msurface_t *surf )
{
	int	maps;

	for( maps = 0; maps < MAXLIGHTMAPS && surf->styles[maps] != 255; maps++ )
	{
		surf->cached_light[maps] = RI.lightstylevalue[surf->styles[maps]];
	}
}

/*
=============================================================================

  LIGHTMAP ALLOCATION

=============================================================================
*/
void LM_InitBlock( void )
{
	Q_memset( gl_lms.allocated, 0, sizeof( gl_lms.allocated ));
}

int LM_AllocBlock( int w, int h, int *x, int *y )
{
	int	i, j;
	int	best, best2;

	best = BLOCK_SIZE;

	for( i = 0; i < BLOCK_SIZE - w; i++ )
	{
		best2 = 0;

		for( j = 0; j < w; j++ )
		{
			if( gl_lms.allocated[i+j] >= best )
				break;
			if( gl_lms.allocated[i+j] > best2 )
				best2 = gl_lms.allocated[i+j];
		}

		if( j == w )
		{
			// this is a valid spot
			*x = i;
			*y = best = best2;
		}
	}

	if( best + h > BLOCK_SIZE )
		return false;

	for( i = 0; i < w; i++ )
		gl_lms.allocated[*x + i] = best + h;

	return true;
}

/*
=================
R_BuildLightmap

Combine and scale multiple lightmaps into the floating
format in r_blocklights
=================
*/
void __cdecl R_BuildLightMap( msurface_t *surf, byte *dest, int stride, qboolean dynamic )
{
	int	smax, tmax;
	uint	*bl, scale;
	int	i, map, size, s, t;
	color24	*lm;

	smax = ( surf->extents[0] / LM_SAMPLE_SIZE ) + 1;
	tmax = ( surf->extents[1] / LM_SAMPLE_SIZE ) + 1;
	size = smax * tmax;

	lm = surf->samples;

	Q_memset( r_blocklights, 0, sizeof( uint ) * size * 3 );

	// add all the lightmaps
	for( map = 0; map < MAXLIGHTMAPS && surf->styles[map] != 255 && lm; map++ )
	{
		scale = RI.lightstylevalue[surf->styles[map]];

		for( i = 0, bl = r_blocklights; i < size; i++, bl += 3, lm++ )
		{
			bl[0] += TextureToTexGamma( lm->r ) * scale;
			bl[1] += TextureToTexGamma( lm->g ) * scale;
			bl[2] += TextureToTexGamma( lm->b ) * scale;
		}
	}

	// add all the dynamic lights
	if( surf->dlightframe == tr.framecount && dynamic )
		R_AddDynamicLights( surf );

	// Put into texture format
	stride -= (smax << 2);
	bl = r_blocklights;

	for( t = 0; t < tmax; t++, dest += stride )
	{
		for( s = 0; s < smax; s++ )
		{
			dest[0] = min((bl[0] >> 7), 255 );
			dest[1] = min((bl[1] >> 7), 255 );
			dest[2] = min((bl[2] >> 7), 255 );
			dest[3] = 255;

			bl += 3;
			dest += 4;
		}
	}
}

/*
=================
R_SurfaceCompare

compare translucent surfaces
=================
*/
int __cdecl R_SurfaceCompare( const void *a, const void *b )
{
	msurface_t	*surf1, *surf2;
	mextrasurf_t	*info1, *info2;
	vec3_t		org1, org2;
	float		len1, len2;

	surf1 = (msurface_t *)a;
	surf2 = (msurface_t *)b;

	info1 = SURF_INFO( surf1, RI.currentmodel );
	info2 = SURF_INFO( surf2, RI.currentmodel );

	VectorAdd( RI.currententity->origin, info1->origin, org1 );
	VectorAdd( RI.currententity->origin, info2->origin, org2 );

	// compare by plane dists
	len1 = DotProduct( org1, RI.vforward ) - RI.viewplanedist;
	len2 = DotProduct( org2, RI.vforward ) - RI.viewplanedist;

	if( len1 > len2 )
		return -1;
	if( len1 < len2 )
		return 1;

	return 0;
}

/*
=================
R_DrawStaticModel

Merge static model brushes with world surfaces
=================
*/
void __cdecl R_DrawStaticModel( cl_entity_t *e )
{
	int		i, k;
	model_t		*clmodel;
	msurface_t	*psurf;
	dlight_t		*l;

	clmodel = e->model;
	if( R_CullBox( clmodel->mins, clmodel->maxs, RI.clipFlags ))
		return;

	// calculate dynamic lighting for bmodel
	for( k = 0, l = cl_dlights; k < MAX_DLIGHTS; k++, l++ )
	{
		if( l->die < cl.time || !l->radius )
			continue;
		R_MarkLights( l, 1<<k, clmodel->nodes + clmodel->hulls[0].firstclipnode );
	}

	psurf = &clmodel->surfaces[clmodel->firstmodelsurface];
	for( i = 0; i < clmodel->nummodelsurfaces; i++, psurf++ )
	{
		if( R_CullSurface( psurf, RI.clipFlags ))
			continue;

		if( psurf->flags & SURF_DRAWSKY && !world.sky_sphere )
		{
			// make sky chain to right clip the skybox
			psurf->texturechain = skychain;
			skychain = psurf;
		}
		else
		{
			psurf->texturechain = psurf->texinfo->texture->texturechain;
			psurf->texinfo->texture->texturechain = psurf;
		}
	}
}

/*
=================
R_DrawStaticBrushes

Insert static brushes into world texture chains
=================
*/
void __cdecl R_DrawStaticBrushes( void )
{
	int	i;

	// draw static entities
	for( i = 0; i < tr.num_static_entities; i++ )
	{
		RI.currententity = tr.static_entities[i];
		RI.currentmodel = RI.currententity->model;

		ASSERT( RI.currententity != NULL );
		ASSERT( RI.currententity->model != NULL );

		switch( RI.currententity->model->type )
		{
		case mod_brush:
			R_DrawStaticModel( RI.currententity );
			break;
		default:
			Host_Error( "R_DrawStatics: non bsp model in static list!\n" );
			break;
		}
	}
}

/*
=============================================================

	WORLD MODEL

=============================================================
*/
/*
================
R_RecursiveWorldNode
================
*/
void __cdecl R_RecursiveWorldNode( mnode_t *node, uint clipflags )
{
	const mplane_t	*clipplane;
	int		i, clipped;
	msurface_t	*surf, **mark;
	mleaf_t		*pleaf;
	int		c, side;
	float		dot;

	if( node->contents == CONTENTS_SOLID )
		return; // hit a solid leaf

	if( node->visframe != tr.visframecount )
		return;

	if( clipflags )
	{
		for( i = 0, clipplane = RI.frustum; i < 6; i++, clipplane++ )
		{
			if(!( clipflags & ( 1<<i )))
				continue;

			clipped = BoxOnPlaneSide( node->minmaxs, node->minmaxs + 3, clipplane );
			if( clipped == 2 ) return;
			if( clipped == 1 ) clipflags &= ~(1<<i);
		}
	}

	// if a leaf node, draw stuff
	if( node->contents < 0 )
	{
		pleaf = (mleaf_t *)node;

		mark = pleaf->firstmarksurface;
		c = pleaf->nummarksurfaces;

		if( c )
		{
			do
			{
				(*mark)->visframe = tr.framecount;
				mark++;
			} while( --c );
		}

		// deal with model fragments in this leaf
		if( pleaf->efrags )
			R_StoreEfrags( &pleaf->efrags, tr.framecount );

		r_stats.c_world_leafs++;
		return;
	}

	// node is just a decision point, so go down the apropriate sides

	// find which side of the node we are on
	dot = PlaneDiff( tr.modelorg, node->plane );
	side = (dot >= 0.0f) ? 0 : 1;

	// recurse down the children, front side first
	R_RecursiveWorldNode( node->children[side], clipflags );

	// draw stuff
	for( c = node->numsurfaces, surf = cl.worldmodel->surfaces + node->firstsurface; c; c--, surf++ )
	{
		if( R_CullSurface( surf, clipflags ))
			continue;

		if( surf->flags & SURF_DRAWSKY && !world.sky_sphere )
		{
			// make sky chain to right clip the skybox
			surf->texturechain = skychain;
			skychain = surf;
		}
		else
		{
			surf->texturechain = surf->texinfo->texture->texturechain;
			surf->texinfo->texture->texturechain = surf;
		}
	}

	// recurse down the back side
	R_RecursiveWorldNode( node->children[!side], clipflags );
}

/*
================
R_CullNodeTopView

cull node by user rectangle (simple scissor)
================
*/
qboolean R_CullNodeTopView( mnode_t *node )
{
	vec2_t	delta, size;
	vec3_t	center, half;

	// build the node center and half-diagonal
	VectorAverage( node->minmaxs, node->minmaxs + 3, center );
	VectorSubtract( node->minmaxs + 3, center, half );

	// cull against the screen frustum or the appropriate area's frustum.
	Vector2Subtract( center, world_orthocenter, delta );
	Vector2Add( half, world_orthohalf, size );

	return ( fabs( delta[0] ) > size[0] ) || ( fabs( delta[1] ) > size[1] );
}

/*
================
R_DrawTopViewLeaf
================
*/
static void R_DrawTopViewLeaf( mleaf_t *pleaf, uint clipflags )
{
	msurface_t	**mark, *surf;
	int		i;

	for( i = 0, mark = pleaf->firstmarksurface; i < pleaf->nummarksurfaces; i++, mark++ )
	{
		surf = *mark;

		// don't process the same surface twice
		if( surf->visframe == tr.framecount )
			continue;

		surf->visframe = tr.framecount;

		if( R_CullSurface( surf, clipflags ))
			continue;

		if(!( surf->flags & SURF_DRAWSKY ))
		{
			surf->texturechain = surf->texinfo->texture->texturechain;
			surf->texinfo->texture->texturechain = surf;
		}
	}

	// deal with model fragments in this leaf
	if( pleaf->efrags )
		R_StoreEfrags( &pleaf->efrags, tr.framecount );

	r_stats.c_world_leafs++;
}

/*
================
R_DrawWorldTopView
================
*/
void R_DrawWorldTopView( mnode_t *node, uint clipflags )
{
	const mplane_t	*clipplane;
	int		c, clipped;
	msurface_t	*surf;

	do
	{
		if( node->contents == CONTENTS_SOLID )
			return;	// hit a solid leaf

		if( node->visframe != tr.visframecount )
			return;

		if( clipflags )
		{
			for( c = 0, clipplane = RI.frustum; c < 6; c++, clipplane++ )
			{
				if(!( clipflags & ( 1<<c )))
					continue;

				clipped = BoxOnPlaneSide( node->minmaxs, node->minmaxs + 3, clipplane );
				if( clipped == 2 ) return;
				if( clipped == 1 ) clipflags &= ~(1<<c);
			}
		}

		// cull against the screen frustum or the appropriate area's frustum.
		if( R_CullNodeTopView( node ))
			return;

		// if a leaf node, draw stuff
		if( node->contents < 0 )
		{
			R_DrawTopViewLeaf( (mleaf_t *)node, clipflags );
			return;
		}

		// draw stuff
		for( c = node->numsurfaces, surf = cl.worldmodel->surfaces + node->firstsurface; c; c--, surf++ )
		{
			// don't process the same surface twice
			if( surf->visframe == tr.framecount )
				continue;

			surf->visframe = tr.framecount;

			if( R_CullSurface( surf, clipflags ))
				continue;

			if(!( surf->flags & SURF_DRAWSKY ))
			{
				surf->texturechain = surf->texinfo->texture->texturechain;
				surf->texinfo->texture->texturechain = surf;
			}
		}

		// recurse down both children, we don't care the order...
		R_DrawWorldTopView( node->children[0], clipflags );
		node = node->children[1];

	} while( node );
}

/*
=============
R_DrawWorld
=============
*/
void R_DrawWorld( void )
{
	// paranoia issues: when gl_renderer is "0" we need have something valid for currententity
	// to prevent crashing until HeadShield drawing.
	RI.currententity = clgame.entities;
	RI.currentmodel = RI.currententity->model;

	if( !RI.drawWorld || RI.refdef.onlyClientDraw )
		return;

	VectorCopy( RI.cullorigin, tr.modelorg );
	Q_memset( gl_lms.lightmap_surfaces, 0, sizeof( gl_lms.lightmap_surfaces ));
	Q_memset( fullbright_polys, 0, sizeof( fullbright_polys ));
	Q_memset( detail_surfaces, 0, sizeof( detail_surfaces ));

	RI.currentWaveHeight = RI.waveHeight;
	GL_SetRenderMode( kRenderNormal );
	gl_lms.dynamic_surfaces = NULL;

	R_ClearSkyBox ();

	// draw the world fog
	R_DrawFog ();

	if( RI.drawOrtho )
	{
		R_DrawWorldTopView( cl.worldmodel->nodes, RI.clipFlags );
	}
	else
	{
		R_RecursiveWorldNode( cl.worldmodel->nodes, RI.clipFlags );
	}

	R_DrawStaticBrushes();
	R_DrawTextureChains();

	R_BlendLightmaps();
	R_RenderFullbrights();
	R_RenderDetails();

	if( skychain )
		R_DrawSkyBox();
	skychain = NULL;

	R_DrawTriangleOutlines ();
}

/*
===============
R_MarkLeaves

Mark the leaves and nodes that are in the PVS for the current leaf
===============
*/
void R_MarkLeaves( void )
{
	byte	*vis;
	mnode_t	*node;
	int	i;

	if( !RI.drawWorld ) return;

	if( r_novis->modified || tr.fResetVis )
	{
		// force recalc viewleaf
		r_novis->modified = false;
		tr.fResetVis = false;
		r_viewleaf = NULL;
	}

	if( r_viewleaf == r_oldviewleaf && r_viewleaf2 == r_oldviewleaf2 && !r_novis->integer && r_viewleaf != NULL )
		return;

	// development aid to let you run around
	// and see exactly where the pvs ends
	if( r_lockpvs->integer ) return;

	tr.visframecount++;
	r_oldviewleaf = r_viewleaf;
	r_oldviewleaf2 = r_viewleaf2;

	if( r_novis->integer || RI.drawOrtho || !r_viewleaf || !cl.worldmodel->visdata )
	{
		// mark everything
		for( i = 0; i < cl.worldmodel->numleafs; i++ )
			cl.worldmodel->leafs[i+1].visframe = tr.visframecount;
		for( i = 0; i < cl.worldmodel->numnodes; i++ )
			cl.worldmodel->nodes[i].visframe = tr.visframecount;
		return;
	}

	// may have to combine two clusters
	// because of solid water boundaries
	vis = Mod_LeafPVS( r_viewleaf, cl.worldmodel );

	if( r_viewleaf != r_viewleaf2 )
	{
		int	longs = ( cl.worldmodel->numleafs + 31 ) >> 5;

		Q_memcpy( visbytes, vis, longs << 2 );
		vis = Mod_LeafPVS( r_viewleaf2, cl.worldmodel );

		for( i = 0; i < longs; i++ )
			((int *)visbytes)[i] |= ((int *)vis)[i];

		vis = visbytes;
	}

	for( i = 0; i < cl.worldmodel->numleafs; i++ )
	{
		if( vis[i>>3] & ( 1<<( i & 7 )))
		{
			node = (mnode_t *)&cl.worldmodel->leafs[i+1];
			do
			{
				if( node->visframe == tr.visframecount )
					break;
				node->visframe = tr.visframecount;
				node = node->parent;
			} while( node );
		}
	}
}

/*
========================
GL_CreateSurfaceLightmap
========================
*/
void GL_CreateSurfaceLightmap( msurface_t *surf )
{
	int	smax, tmax;
	byte	*base;

	if( !cl.worldmodel->lightdata ) return;
	if( surf->flags & SURF_DRAWTILED )
		return;

	smax = ( surf->extents[0] / LM_SAMPLE_SIZE ) + 1;
	tmax = ( surf->extents[1] / LM_SAMPLE_SIZE ) + 1;

	if( !LM_AllocBlock( smax, tmax, &surf->light_s, &surf->light_t ))
	{
		LM_UploadBlock( false );
		LM_InitBlock();

		if( !LM_AllocBlock( smax, tmax, &surf->light_s, &surf->light_t ))
			Host_Error( "AllocBlock: full\n" );
	}

	surf->lightmaptexturenum = gl_lms.current_lightmap_texture;

	base = gl_lms.lightmap_buffer;
	base += ( surf->light_t * BLOCK_SIZE + surf->light_s ) * 4;

	R_SetCacheState( surf );
	R_BuildLightMap( surf, base, BLOCK_SIZE * 4, false );

	// moved here in case we need valid lightmap coords
	if( host.features & ENGINE_BUILD_SURFMESHES )
		Mod_BuildSurfacePolygons( surf, SURF_INFO( surf, loadmodel ));
}

/*
==================
GL_RebuildLightmaps

Rebuilds the lightmap texture
when gamma is changed
==================
*/
void GL_RebuildLightmaps( void )
{
	int	i, j;
	model_t	*m;

	if( !cl.world ) return;	// wait for worldmodel
	vid_gamma->modified = false;

	// release old lightmaps
	for( i = 0; i < MAX_LIGHTMAPS; i++ )
	{
		if( !tr.lightmapTextures[i] ) break;
		GL_FreeTexture( tr.lightmapTextures[i] );
	}

	Q_memset( tr.lightmapTextures, 0, sizeof( tr.lightmapTextures ));
	gl_lms.current_lightmap_texture = 0;

	// setup all the lightstyles
	R_AnimateLight();

	LM_InitBlock();

	for( i = 1; i < MAX_MODELS; i++ )
	{
		if(( m = Mod_Handle( i )) == NULL )
			continue;

		if( m->name[0] == '*' || m->type != mod_brush )
			continue;

		loadmodel = m;

		for( j = 0; j < m->numsurfaces; j++ )
			GL_CreateSurfaceLightmap( m->surfaces + j );
	}
	LM_UploadBlock( false );

	if( clgame.drawFuncs.GL_BuildLightmaps )
	{
		// build lightmaps on the client-side
		clgame.drawFuncs.GL_BuildLightmaps( );
	}
}

/*
==================
GL_BuildLightmaps

Builds the lightmap texture
with all the surfaces from all brush models
==================
*/
void GL_BuildLightmaps( void )
{
	int	i, j;
	model_t	*m;

	// release old lightmaps
	for( i = 0; i < MAX_LIGHTMAPS; i++ )
	{
		if( !tr.lightmapTextures[i] ) break;
		GL_FreeTexture( tr.lightmapTextures[i] );
	}

	// release old mirror textures
	for( i = 0; i < MAX_MIRRORS; i++ )
	{
		if( !tr.mirrorTextures[i] ) break;
		GL_FreeTexture( tr.mirrorTextures[i] );
	}

	Q_memset( tr.lightmapTextures, 0, sizeof( tr.lightmapTextures ));
	Q_memset( tr.mirror_entities, 0, sizeof( tr.mirror_entities ));
	Q_memset( tr.mirrorTextures, 0, sizeof( tr.mirrorTextures ));
	Q_memset( visbytes, 0x00, sizeof( visbytes ));

	skychain = NULL;

	tr.framecount = tr.visframecount = 1;	// no dlight cache
	gl_lms.current_lightmap_texture = 0;
	tr.num_mirror_entities = 0;
	tr.num_mirrors_used = 0;
	nColinElim = 0;

	// setup all the lightstyles
	R_AnimateLight();

	LM_InitBlock();

	for( i = 1; i < MAX_MODELS; i++ )
	{
		if(( m = Mod_Handle( i )) == NULL )
			continue;

		if( m->name[0] == '*' || m->type != mod_brush )
			continue;

		for( j = 0; j < m->numsurfaces; j++ )
		{
			// clearing all decal chains
			m->surfaces[j].pdecals = NULL;
			m->surfaces[j].visframe = 0;
			loadmodel = m;

			GL_CreateSurfaceLightmap( m->surfaces + j );

			if( m->surfaces[j].flags & SURF_DRAWTURB )
				continue;

			if( m->surfaces[j].flags & SURF_DRAWSKY && world.sky_sphere )
				continue;

			GL_BuildPolygonFromSurface( m, m->surfaces + j );
		}

		// clearing visframe
		for( j = 0; j < m->numleafs; j++ )
			m->leafs[j+1].visframe = 0;
		for( j = 0; j < m->numnodes; j++ )
			m->nodes[j].visframe = 0;
	}

	LM_UploadBlock( false );

	if( clgame.drawFuncs.GL_BuildLightmaps )
	{
		// build lightmaps on the client-side
		clgame.drawFuncs.GL_BuildLightmaps( );
	}

	if( !gl_keeptjunctions->integer )
		MsgDev( D_INFO, "Eliminate %i vertexes\n", nColinElim );
}
