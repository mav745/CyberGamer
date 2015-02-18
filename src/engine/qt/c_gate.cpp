#include "c_gate.h"
#include "window.hpp"

#include <QDebug>
#include <QLabel>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

Window *xW = nullptr;

void initWindow(int width, int height, int fullscreen)
{
	delete xW;
	xW = new Window;
	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	xW->resize(width, height);
	fullscreen? xW->showFullScreen(): xW->show();

}


void killWindow()
{
	delete xW;
	xW = nullptr;
}

void GL_SelectTexture( GLint tmu )
{
	// don't allow negative texture units
	if( tmu < 0 ) return;
	if( tmu >= GL_MaxTextureUnits( ))
	{
		MsgDev( D_ERROR, "GL_SelectTexture: bad tmu state %i\n", tmu );
		return;
	}
	if( glState.activeTMU == tmu )
		return;
	glState.activeTMU = tmu;
	xW->glActiveTexture( tmu + GL_TEXTURE0 );
}

void GL_Bind( GLint tmu, GLenum texnum )
{
	gltexture_t	*texture;

	// missed texture ?
	if( texnum <= 0 ) texnum = tr.defaultTexture;
	ASSERT( texnum > 0 && texnum < MAX_TEXTURES );

	if( tmu != GL_KEEP_UNIT )
		GL_SelectTexture( tmu );
	else tmu = glState.activeTMU;

	texture = &r_textures[texnum];

	if( glState.currentTextureTargets[tmu] != texture->target )
	{
		if( glState.currentTextureTargets[tmu] != GL_NONE )
			xW->glDisable( glState.currentTextureTargets[tmu] );
		glState.currentTextureTargets[tmu] = texture->target;
		xW->glEnable( glState.currentTextureTargets[tmu] );
	}

	if( glState.currentTextures[tmu] == texture->texnum )
		return;

	xW->glBindTexture( GL_TEXTURE_2D, texture->texid );
	glState.currentTextures[tmu] = texture->texnum;
}
void GL_TexFilter( gltexture_t *tex, qboolean update )
{
	qboolean	allowNearest;
	vec4_t	zeroClampBorder = { 0.0f, 0.0f, 0.0f, 1.0f };
	vec4_t	alphaZeroClampBorder = { 0.0f, 0.0f, 0.0f, 0.0f };

	switch( tex->texType )
	{
	case TEX_NOMIP:
	case TEX_CUBEMAP:
	case TEX_LIGHTMAP:
		allowNearest = false;
		break;
	default:
		allowNearest = true;
		break;
	}

	// set texture filter
	if( tex->flags & TF_DEPTHMAP )
	{
		xW->glTexParameteri( tex->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		xW->glTexParameteri( tex->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		if( !( tex->flags & TF_NOCOMPARE ))
		{
			xW->glTexParameteri( tex->target, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL );
			xW->glTexParameteri( tex->target, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB );
		}

		if( tex->flags & TF_LUMINANCE )
			xW->glTexParameteri( tex->target, GL_DEPTH_TEXTURE_MODE_ARB, GL_LUMINANCE );
		else xW->glTexParameteri( tex->target, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY );

		//if( GL_Support( GL_ANISOTROPY_EXT ))
			xW->glTexParameterf( tex->target, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f );
	}
	else if( tex->flags & TF_NOMIPMAP )
	{
		if( tex->flags & TF_NEAREST )
		{
			xW->glTexParameteri( tex->target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			xW->glTexParameteri( tex->target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		}
		else
		{
			if( r_textureMagFilter == GL_NEAREST && allowNearest )
			{
				xW->glTexParameteri( tex->target, GL_TEXTURE_MIN_FILTER, r_textureMagFilter );
				xW->glTexParameteri( tex->target, GL_TEXTURE_MAG_FILTER, r_textureMagFilter );
			}
			else
			{
				xW->glTexParameteri( tex->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
				xW->glTexParameteri( tex->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			}
		}
	}
	else
	{
		if( tex->flags & TF_NEAREST )
		{
			xW->glTexParameteri( tex->target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
			xW->glTexParameteri( tex->target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		}
		else
		{
			xW->glTexParameteri( tex->target, GL_TEXTURE_MIN_FILTER, r_textureMinFilter );
			xW->glTexParameteri( tex->target, GL_TEXTURE_MAG_FILTER, r_textureMagFilter );
		}

		// set texture anisotropy if available
		if( /*GL_Support( GL_ANISOTROPY_EXT ) &&*/ !( tex->flags & TF_ALPHACONTRAST ))
			xW->glTexParameterf( tex->target, GL_TEXTURE_MAX_ANISOTROPY_EXT, gl_texture_anisotropy->value );

		// set texture LOD bias if available
		//if( GL_Support( GL_TEXTURE_LODBIAS ))
			xW->glTexParameterf( tex->target, GL_TEXTURE_LOD_BIAS_EXT, gl_texture_lodbias->value );
	}

	if( update ) return;

	if( tex->flags & ( TF_BORDER|TF_ALPHA_BORDER ) && !GL_Support( GL_CLAMP_TEXBORDER_EXT ))
	{
		// border is not support, use clamp instead
		tex->flags = (texFlags_t)(tex->flags & (~(TF_BORDER|TF_ALPHA_BORDER)));
		tex->flags = (texFlags_t)(tex->flags | TF_CLAMP);
	}

	// set texture wrap
	if( tex->flags & TF_CLAMP )
	{
//		if( GL_Support( GL_CLAMPTOEDGE_EXT ))
//		{
			xW->glTexParameteri( tex->target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );

			if( tex->target != GL_TEXTURE_1D )
				xW->glTexParameteri( tex->target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

			if( tex->target == GL_TEXTURE_3D || tex->target == GL_TEXTURE_CUBE_MAP_ARB )
				xW->glTexParameteri( tex->target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
//		}
//		else
//		{
//			xW->glTexParameteri( tex->target, GL_TEXTURE_WRAP_S, GL_CLAMP );

//			if( tex->target != GL_TEXTURE_1D )
//				xW->glTexParameteri( tex->target, GL_TEXTURE_WRAP_T, GL_CLAMP );

//			if( tex->target == GL_TEXTURE_3D || tex->target == GL_TEXTURE_CUBE_MAP_ARB )
//				xW->glTexParameteri( tex->target, GL_TEXTURE_WRAP_R, GL_CLAMP );
//		}
	}
	else if( tex->flags & ( TF_BORDER|TF_ALPHA_BORDER ))
	{
		xW->glTexParameteri( tex->target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );

		if( tex->target != GL_TEXTURE_1D )
			xW->glTexParameteri( tex->target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );

		if( tex->target == GL_TEXTURE_3D || tex->target == GL_TEXTURE_CUBE_MAP_ARB )
			xW->glTexParameteri( tex->target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER );

		if( tex->flags & TF_BORDER )
			xW->glTexParameterfv( tex->target, GL_TEXTURE_BORDER_COLOR, zeroClampBorder );
		else if( tex->flags & TF_ALPHA_BORDER )
			xW->glTexParameterfv( tex->target, GL_TEXTURE_BORDER_COLOR, alphaZeroClampBorder );
	}
	else
	{
		xW->glTexParameteri( tex->target, GL_TEXTURE_WRAP_S, GL_REPEAT );

		if( tex->target != GL_TEXTURE_1D )
			xW->glTexParameteri( tex->target, GL_TEXTURE_WRAP_T, GL_REPEAT );

		if( tex->target == GL_TEXTURE_3D || tex->target == GL_TEXTURE_CUBE_MAP_ARB )
			xW->glTexParameteri( tex->target, GL_TEXTURE_WRAP_R, GL_REPEAT );
	}
}

void R_SetTextureParameters()
{
	gltexture_t	*texture;
	int		i;

	if( !Q_stricmp( gl_texturemode->string, "GL_NEAREST" ))
	{
		r_textureMinFilter = GL_NEAREST;
		r_textureMagFilter = GL_NEAREST;
	}
	else if( !Q_stricmp( gl_texturemode->string, "GL_LINEAR" ))
	{
		r_textureMinFilter = GL_LINEAR;
		r_textureMagFilter = GL_LINEAR;
	}
	else if( !Q_stricmp( gl_texturemode->string, "GL_NEAREST_MIPMAP_NEAREST" ))
	{
		r_textureMinFilter = GL_NEAREST_MIPMAP_NEAREST;
		r_textureMagFilter = GL_NEAREST;
	}
	else if( !Q_stricmp( gl_texturemode->string, "GL_LINEAR_MIPMAP_NEAREST" ))
	{
		r_textureMinFilter = GL_LINEAR_MIPMAP_NEAREST;
		r_textureMagFilter = GL_LINEAR;
	}
	else if( !Q_stricmp( gl_texturemode->string, "GL_NEAREST_MIPMAP_LINEAR" ))
	{
		r_textureMinFilter = GL_NEAREST_MIPMAP_LINEAR;
		r_textureMagFilter = GL_NEAREST;
	}
	else if( !Q_stricmp( gl_texturemode->string, "GL_LINEAR_MIPMAP_LINEAR" ))
	{
		r_textureMinFilter = GL_LINEAR_MIPMAP_LINEAR;
		r_textureMagFilter = GL_LINEAR;
	}
	else
	{
		MsgDev( D_ERROR, "gl_texturemode invalid mode %s, defaulting to GL_LINEAR_MIPMAP_LINEAR\n", gl_texturemode->string );
		Cvar_Set( "gl_texturemode", "GL_LINEAR_MIPMAP_LINEAR" );
		r_textureMinFilter = GL_LINEAR_MIPMAP_LINEAR;
		r_textureMagFilter = GL_LINEAR;
	}

	gl_texturemode->modified = false;

//	if( GL_Support( GL_ANISOTROPY_EXT ))
//	{
		if( gl_texture_anisotropy->value > glConfig.max_texture_anisotropy )
			Cvar_SetFloat( "gl_anisotropy", glConfig.max_texture_anisotropy );
		else if( gl_texture_anisotropy->value < 1.0f )
			Cvar_SetFloat( "gl_anisotropy", 1.0f );
//	}

	gl_texture_anisotropy->modified = false;

//	if( GL_Support( GL_TEXTURE_LODBIAS ))
//	{
		if( gl_texture_lodbias->value > glConfig.max_texture_lodbias )
			Cvar_SetFloat( "gl_texture_lodbias", glConfig.max_texture_lodbias );
		else if( gl_texture_lodbias->value < -glConfig.max_texture_lodbias )
			Cvar_SetFloat( "gl_texture_lodbias", -glConfig.max_texture_lodbias );
//	}

	gl_texture_lodbias->modified = false;

	// change all the existing mipmapped texture objects
	for( i = 0, texture = r_textures; i < r_numTextures; i++, texture++ )
	{
		if( !texture->texnum ) continue;	// free slot
		GL_Bind( GL_TEXTURE0, i );
		GL_TexFilter( texture, true );
	}
}
void GL_Cull( GLenum cull )
{
	if( !cull )
	{
		xW->glDisable( GL_CULL_FACE );
		glState.faceCull = 0;
		return;
	}

	xW->glEnable( GL_CULL_FACE );
	xW->glCullFace( cull );
	glState.faceCull = cull;
}

/*
=================
GL_FrontFace
=================
*/
void GL_FrontFace( GLenum front )
{
	xW->glFrontFace( front ? GL_CW : GL_CCW );
	glState.frontFace = front;
}

void GL_SetRenderMode( int mode )
{
	switch( mode )
	{
	case kRenderNormal:
	default:
		xW->glDisable( GL_BLEND );
		xW->glDisable( GL_ALPHA_TEST );
		////////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
		break;
	case kRenderTransColor:
	case kRenderTransTexture:
		xW->glEnable( GL_BLEND );
		xW->glDisable( GL_ALPHA_TEST );
		xW->glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		////////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		break;
	case kRenderTransAlpha:
		xW->glDisable( GL_BLEND );
		xW->glEnable( GL_ALPHA_TEST );
		////////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		break;
	case kRenderGlow:
	case kRenderTransAdd:
		xW->glEnable( GL_BLEND );
		xW->glDisable( GL_ALPHA_TEST );
		xW->glBlendFunc( GL_SRC_ALPHA, GL_ONE );
		////////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////

void R_DrawStretchPic( float x, float y, float w, float h, float s1, float t1, float s2, float t2, int texnum )
{
	GL_Bind( 0, texnum );

	xW->glBegin( GL_QUADS );
		xW->glTexCoord2f( s1, t1 );
		xW->glVertex2f( x, y );

		xW->glTexCoord2f( s2, t1 );
		xW->glVertex2f( x + w, y );

		xW->glTexCoord2f( s2, t2 );
		xW->glVertex2f( x + w, y + h );

		xW->glTexCoord2f( s1, t2 );
		xW->glVertex2f( x, y + h );
	xW->glEnd();
}

void R_DrawTileClear( int x, int y, int w, int h )
{
	float		tw, th;
	gltexture_t	*glt;

	GL_SetRenderMode( kRenderNormal );
	xW->glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	GL_Bind( 0, cls.tileImage );

	glt = R_GetTexture( cls.tileImage );
	tw = glt->srcWidth;
	th = glt->srcHeight;

	xW->glBegin( GL_QUADS );
		xW->glTexCoord2f( (float)x / tw, (float)y / th );
		xW->glVertex2f( (float)x, (float)y );
		xW->glTexCoord2f((float)(x + w) / tw, (float)y / th );
		xW->glVertex2f( (float)(x + w), (float)y );
		xW->glTexCoord2f((float)(x + w) / tw, (float)(y + h) / th );
		xW->glVertex2f( (float)(x + w), (float)(y + h) );
		xW->glTexCoord2f( (float)x / tw, (float)(y + h) / th );
		xW->glVertex2f( (float)x, (float)(y + h) );
	xW->glEnd ();
}

void R_DrawStretchRaw( float x, float y, float w, float h, int cols, int rows, const byte *data, qboolean dirty )
{
	byte		*raw = NULL;
	gltexture_t	*tex;

	int	width = 1, height = 1;

	// check the dimensions
	width = NearestPOW( cols, true );
	height = NearestPOW( rows, false );

	if( cols != width || rows != height )
	{
		raw = GL_ResampleTexture( data, cols, rows, width, height, false );
		cols = width;
		rows = height;
	}
	else
		raw = (byte *)data;

	xW->glDisable( GL_BLEND );
	xW->glDisable( GL_ALPHA_TEST );
	//glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

	tex = R_GetTexture( tr.cinTexture );
	GL_Bind( 0, tr.cinTexture );

	if( cols == tex->width && rows == tex->height )
	{
		if( dirty )
		{
			xW->glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, cols, rows, GL_BGRA, GL_UNSIGNED_BYTE, raw );
		}
	}
	else
	{
		tex->width = cols;
		tex->height = rows;
		if( dirty )
		{
			xW->glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, cols, rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, raw );
		}
	}

	xW->glBegin( GL_QUADS );
	xW->glTexCoord2f( 0, 0 );
	xW->glVertex2f( x, y );
	xW->glTexCoord2f( 1, 0 );
	xW->glVertex2f( x + w, y );
	xW->glTexCoord2f( 1, 1 );
	xW->glVertex2f( x + w, y + h );
	xW->glTexCoord2f( 0, 1 );
	xW->glVertex2f( x, y + h );
	xW->glEnd();
}

//void R_DrawStretchRaw( float x, float y, float w, float h, int cols, int rows, const byte *data, qboolean dirty )
//{
//	byte		*raw = NULL;
//	gltexture_t	*tex;

//	int	width = 1, height = 1;

//	// check the dimensions
//	width = NearestPOW( cols, true );
//	height = NearestPOW( rows, false );

//	if( cols != width || rows != height )
//	{
//		raw = GL_ResampleTexture( data, cols, rows, width, height, false );
//		cols = width;
//		rows = height;
//	}
//	else
//		raw = data;

//	xW->glDisable( GL_BLEND );
//	xW->glDisable( GL_ALPHA_TEST );
//	//xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

//	tex = R_GetTexture( tr.cinTexture );
//	GL_Bind( 0, tr.cinTexture );

//	if( cols == tex->width && rows == tex->height )
//	{
//		if( dirty )
//		{
//			xW->glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, cols, rows, GL_BGRA, GL_UNSIGNED_BYTE, raw );
//		}
//	}
//	else
//	{
//		tex->width = cols;
//		tex->height = rows;
//		if( dirty )
//		{
//			xW->glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, cols, rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, raw );
//		}
//	}

//	xW->glBegin( GL_QUADS );
//	xW->glTexCoord2f( 0, 0 );
//	xW->glVertex2f( x, y );
//	xW->glTexCoord2f( 1, 0 );
//	xW->glVertex2f( x + w, y );
//	xW->glTexCoord2f( 1, 1 );
//	xW->glVertex2f( x + w, y + h );
//	xW->glTexCoord2f( 0, 1 );
//	xW->glVertex2f( x, y + h );
//	xW->glEnd();
//}

void R_UploadStretchRaw( int texture, int cols, int rows, int width, int height, const byte *data )
{
	byte		*raw = NULL;
	gltexture_t	*tex;

	// check the dimensions
	width = NearestPOW( width, true );
	height = NearestPOW( height, false );

	if( cols != width || rows != height )
	{
		raw = GL_ResampleTexture( data, cols, rows, width, height, false );
		cols = width;
		rows = height;
	}
	else
	{
		raw = (byte *)data;
	}

	tex = R_GetTexture( texture );
	GL_Bind( GL_KEEP_UNIT, texture );
	tex->width = cols;
	tex->height = rows;

	xW->glTexImage2D( GL_TEXTURE_2D, 0, tex->format, cols, rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, raw );
	GL_TexFilter( tex, false );
}

void R_Set2DMode( qboolean enable )
{
	if( enable )
	{
		if( glState.in2DMode )
			return;

		// set 2D virtual screen size
		xW->glViewport( 0, 0, glState.width, glState.height );
		xW->glMatrixMode( GL_PROJECTION );
		xW->glLoadIdentity();
		xW->glOrtho( 0, glState.width, glState.height, 0, -99999, 99999 );
		xW->glMatrixMode( GL_MODELVIEW );
		xW->glLoadIdentity();

		GL_Cull( 0 );

		xW->glDepthMask( GL_FALSE );
		xW->glDisable( GL_DEPTH_TEST );
		xW->glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

		glState.in2DMode = true;
		RI.currententity = NULL;
		RI.currentmodel = NULL;
	}
	else
	{
		xW->glDepthMask( GL_TRUE );
		xW->glEnable( GL_DEPTH_TEST );
		glState.in2DMode = false;

		xW->glMatrixMode( GL_PROJECTION );
		GL_LoadMatrix( RI.projectionMatrix );

		xW->glMatrixMode( GL_MODELVIEW );
		GL_LoadMatrix( RI.worldviewMatrix );

	}
}


//////////////////////////////////////////////////////////////////////////////////

int R_AllocateMirrorTexture()
{
	rgbdata_t	r_screen;
	int	i, texture;
	char	txName[16];

	i = tr.num_mirrors_used;
	if( i >= MAX_MIRRORS )
	{
		MsgDev( D_ERROR, "R_AllocateMirrorTexture: mirror textures limit exceeded!\n" );
		return 0;	// disable
	}

	texture = tr.mirrorTextures[i];
	tr.num_mirrors_used++;

	if( !texture )
	{
		// not initialized ?
		Q_memset( &r_screen, 0, sizeof( r_screen ));
		Q_snprintf( txName, sizeof( txName ), "*screen%i", i );

		r_screen.width = RI.viewport[2];
		r_screen.height = RI.viewport[3];
		r_screen.type = PF_RGBA_32;
		r_screen.size = r_screen.width * r_screen.height * 4;
		r_screen.flags = IMAGE_HAS_COLOR;
		r_screen.buffer = NULL; // create empty texture for now
		tr.mirrorTextures[i] = GL_LoadTextureInternal( txName, &r_screen, (texFlags_t)TF_IMAGE, false );
		GL_SetTextureType( tr.mirrorTextures[i], TEX_SCREENCOPY );
		texture = tr.mirrorTextures[i];
	}

	GL_Bind( GL_TEXTURE0, texture );
	xW->glCopyTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, RI.viewport[0], RI.viewport[1], RI.viewport[2], RI.viewport[3], 0 );

	return texture;
}


void CL_BulletTracerDraw( particle_t *p, float frametime )
{
	vec3_t	lineDir, viewDir, cross;
	vec3_t	vecEnd, vecStart, vecDir;
	float	sDistance, eDistance, totalDist;
	float	dDistance, dTotal, fOffset;
	int	alpha = (int)(traceralpha->value * 255);
	float	width = 3.0f, life, frac, length;
	vec3_t	tmp;

	// calculate distance
	VectorCopy( p->vel, vecDir );
	totalDist = VectorNormalizeLength( vecDir );

	length = p->ramp; // ramp used as length

	// calculate fraction
	life = ( totalDist + length ) / ( max( 1.0f, tracerspeed->value ));
	frac = life - ( p->die - (float)cl.time ) + frametime;

	// calculate our distance along our path
	sDistance = tracerspeed->value * frac;
	eDistance = sDistance - length;

	// clip to start
	sDistance = max( 0.0f, sDistance );
	eDistance = max( 0.0f, eDistance );

	if(( sDistance == 0.0f ) && ( eDistance == 0.0f ))
		return;

	// clip it
	if( totalDist != 0.0f )
	{
		sDistance = min( sDistance, totalDist );
		eDistance = min( eDistance, totalDist );
	}

	// get our delta to calculate the tc offset
	dDistance	= fabsf( sDistance - eDistance );
	dTotal = ( length != 0.0f ) ? length : 0.01f;
	fOffset = ( dDistance / dTotal );

	// find our points along our path
	VectorMA( p->org, sDistance, vecDir, vecEnd );
	VectorMA( p->org, eDistance, vecDir, vecStart );

	// setup our info for drawing the line
	VectorSubtract( vecEnd, vecStart, lineDir );
	VectorSubtract( vecEnd, RI.vieworg, viewDir );

	CrossProduct( lineDir, viewDir, cross );
	VectorNormalize( cross );

	GL_SetRenderMode( kRenderTransTexture );

	GL_Bind( GL_TEXTURE0, cls.particleImage );
	glBegin( GL_QUADS );

	glColor4ub( clgame.palette[p->color][0], clgame.palette[p->color][1], clgame.palette[p->color][2], alpha );

	VectorMA( vecStart, -width, cross, tmp );
	glTexCoord2f( 1.0f, 0.0f );
	glVertex3fv( tmp );

	VectorMA( vecStart, width, cross, tmp );
	glTexCoord2f( 0.0f, 0.0f );
	glVertex3fv( tmp );

	VectorMA( vecEnd, width, cross, tmp );
	glTexCoord2f( 0.0f, fOffset );
	glVertex3fv( tmp );

	VectorMA( vecEnd, -width, cross, tmp );
	glTexCoord2f( 1.0f, fOffset );
	glVertex3fv( tmp );

	glEnd();
}

/*
================
CL_UpdateParticle

update particle color, position etc
================
*/
void CL_UpdateParticle( particle_t *p, float ft )
{
	float	time3 = 15.0f * ft;
	float	time2 = 10.0f * ft;
	float	time1 = 5.0f * ft;
	float	dvel = 4 * ft;
	float	grav = ft * clgame.movevars.gravity * 0.05f;
	float	size = 1.5f;
	int	i, iRamp, alpha = 255;
	vec3_t	right, up;
	rgb_t	color;

	r_stats.c_particle_count++;

	switch( p->type )
	{
	case pt_static:
		break;
	case pt_tracer:
	case pt_clientcustom:
		if( p->callback )
		{
			p->callback( p, ft );
		}
		if( p->type == pt_tracer )
			return; // already drawed
		break;
	case pt_fire:
		p->ramp += time1;
		if( p->ramp >= 6 ) p->die = -1;
		else p->color = ramp3[(int)p->ramp];
		p->vel[2] += grav;
		break;
	case pt_explode:
		p->ramp += time2;
		if( p->ramp >= 8 ) p->die = -1;
		else p->color = ramp1[(int)p->ramp];
		for( i = 0; i < 3; i++ )
			p->vel[i] += p->vel[i] * dvel;
		p->vel[2] -= grav;
		break;
	case pt_explode2:
		p->ramp += time3;
		if( p->ramp >= 8 ) p->die = -1;
		else p->color = ramp2[(int)p->ramp];
		for( i = 0; i < 3; i++ )
			p->vel[i] -= p->vel[i] * ft;
		p->vel[2] -= grav;
		break;
	case pt_blob:
	case pt_blob2:
		p->ramp += time2;
		iRamp = (int)p->ramp >> SIMSHIFT;

		if( iRamp >= SPARK_COLORCOUNT )
		{
			p->ramp = 0.0f;
			iRamp = 0;
		}

		p->color = CL_LookupColor( gSparkRamp[iRamp][0], gSparkRamp[iRamp][1], gSparkRamp[iRamp][2] );

		for( i = 0; i < 2; i++ )
			p->vel[i] -= p->vel[i] * 0.5f * ft;
		p->vel[2] -= grav * 5.0f;

		if( Com_RandomLong( 0, 3 ))
		{
			p->type = pt_blob;
			alpha = 0;
		}
		else
		{
			p->type = pt_blob2;
			alpha = 255;
		}
		break;
	case pt_grav:
		p->vel[2] -= grav * 20;
		break;
	case pt_slowgrav:
		p->vel[2] -= grav;
		break;
	case pt_vox_grav:
		p->vel[2] -= grav * 8;
		break;
	case pt_vox_slowgrav:
		p->vel[2] -= grav * 4;
		break;
	}

#if 0
	// HACKHACK a scale up to keep particles from disappearing
	size += (p->org[0] - RI.vieworg[0]) * RI.vforward[0];
	size += (p->org[1] - RI.vieworg[1]) * RI.vforward[1];
	size += (p->org[2] - RI.vieworg[2]) * RI.vforward[2];

	if( size < 20.0f ) size = 1.0f;
	else size = 1.0f + size * 0.004f;
#endif
	// scale the axes by radius
	VectorScale( RI.vright, size, right );
	VectorScale( RI.vup, size, up );

	p->color = bound( 0, p->color, 255 );
	VectorSet( color, clgame.palette[p->color][0], clgame.palette[p->color][1], clgame.palette[p->color][2] );

	GL_SetRenderMode( kRenderTransTexture );
	xW->glColor4ub( color[0], color[1], color[2], alpha );

	GL_Bind( GL_TEXTURE0, cls.particleImage );

	// add the 4 corner vertices.
	xW->glBegin( GL_QUADS );

	xW->glTexCoord2f( 0.0f, 1.0f );
	xW->glVertex3f( p->org[0] - right[0] + up[0], p->org[1] - right[1] + up[1], p->org[2] - right[2] + up[2] );
	xW->glTexCoord2f( 0.0f, 0.0f );
	xW->glVertex3f( p->org[0] + right[0] + up[0], p->org[1] + right[1] + up[1], p->org[2] + right[2] + up[2] );
	xW->glTexCoord2f( 1.0f, 0.0f );
	xW->glVertex3f( p->org[0] + right[0] - up[0], p->org[1] + right[1] - up[1], p->org[2] + right[2] - up[2] );
	xW->glTexCoord2f( 1.0f, 1.0f );
	xW->glVertex3f( p->org[0] - right[0] - up[0], p->org[1] - right[1] - up[1], p->org[2] - right[2] - up[2] );

	xW->glEnd();

	if( p->type != pt_clientcustom )
	{
		// update position.
		VectorMA( p->org, ft, p->vel, p->org );
	}
}

void CL_DrawTracer( vec3_t start, vec3_t delta, float width, rgb_t color, int alpha, float startV, float endV )
{
	// Clip the tracer
	vec3_t	verts[4];

	if ( !CL_TracerComputeVerts( start, delta, width, verts ))
		return;

	// NOTE: Gotta get the winding right so it's not backface culled
	// (we need to turn of backface culling for these bad boys)

	GL_SetRenderMode( kRenderTransTexture );

	xW->glColor4ub( color[0], color[1], color[2], alpha );

	GL_Bind( GL_TEXTURE0, cls.particleImage );
	xW->glBegin( GL_QUADS );

	xW->glTexCoord2f( 0.0f, endV );
	xW->glVertex3fv( verts[2] );

	xW->glTexCoord2f( 1.0f, endV );
	xW->glVertex3fv( verts[3] );

	xW->glTexCoord2f( 1.0f, startV );
	xW->glVertex3fv( verts[1] );

	xW->glTexCoord2f( 0.0f, startV );
	xW->glVertex3fv( verts[0] );

	xW->glEnd();
}

void CL_DrawSegs( int modelIndex, float frame, int rendermode, const vec3_t source, const vec3_t delta,
	float width, float scale, float freq, float speed, int segments, int flags, float *color )
{
	int	noiseIndex, noiseStep;
	int	i, total_segs, segs_drawn;
	float	div, length, fraction, factor;
	float	flMaxWidth, vLast, vStep, brightness;
	vec3_t	perp1, vLastNormal;
	VHSPRITE	m_hSprite;
	beamseg_t	curSeg;

	if( !cl_draw_beams->integer )
		return;

	m_hSprite = R_GetSpriteTexture( Mod_Handle( modelIndex ), (int)frame );

	if( !m_hSprite || segments < 2  )
		return;

	length = VectorLength( delta );
	flMaxWidth = width * 0.5f;
	div = 1.0f / ( segments - 1 );

	if( length * div < flMaxWidth * 1.414f )
	{
		// Here, we have too many segments; we could get overlap... so lets have less segments
		segments = (int)( length / ( flMaxWidth * 1.414f )) + 1;
		if( segments < 2 ) segments = 2;
	}

	if( segments > NOISE_DIVISIONS )
		segments = NOISE_DIVISIONS;

	div = 1.0f / (segments - 1);
	length *= 0.01f;
	vStep = length * div;	// Texture length texels per space pixel

	// Scroll speed 3.5 -- initial texture position, scrolls 3.5/sec (1.0 is entire texture)
	vLast = fmodf( freq * speed, 1.f );

	if( flags & FBEAM_SINENOISE )
	{
		if( segments < 16 )
		{
			segments = 16;
			div = 1.0f / ( segments - 1 );
		}
		scale *= 10;
		length = segments * ( 1.0f / 10 );
	}
	else
	{
		scale *= length;
	}

	// Iterator to resample noise waveform (it needs to be generated in powers of 2)
	noiseStep = (int)((float)( NOISE_DIVISIONS - 1 ) * div * 65536.0f );
	noiseIndex = 0;

	if( flags & FBEAM_SINENOISE )
	{
		noiseIndex = 0;
	}

	brightness = 1.0f;
	if( flags & FBEAM_SHADEIN )
	{
		brightness = 0;
	}

	// Choose two vectors that are perpendicular to the beam
	ComputeBeamPerpendicular( delta, perp1 );

	segs_drawn = 0;
	total_segs = segments;

	SetBeamRenderMode( rendermode );
	GL_Bind( GL_TEXTURE0, m_hSprite );
	xW->glBegin( GL_TRIANGLE_STRIP );

	// specify all the segments.
	for( i = 0; i < segments; i++ )
	{
		beamseg_t	nextSeg;
		vec3_t	vPoint1, vPoint2;

		ASSERT( noiseIndex < ( NOISE_DIVISIONS << 16 ));
		nextSeg.alpha = 1.0f;

		fraction = i * div;

		if(( flags & FBEAM_SHADEIN ) && ( flags & FBEAM_SHADEOUT ))
		{
			if( fraction < 0.5f )
			{
				brightness = 2.0f * fraction;
			}
			else
			{
				brightness = 2.0f * ( 1.0f - fraction );
			}
		}
		else if( flags & FBEAM_SHADEIN )
		{
			brightness = fraction;
		}
		else if( flags & FBEAM_SHADEOUT )
		{
			brightness = 1.0f - fraction;
		}

		// clamps
		brightness = bound( 0.0f, brightness, 1.0f );
		VectorScale( color, brightness, nextSeg.color );

		VectorMA( source, fraction, delta, nextSeg.pos );

		// distort using noise
		if( scale != 0 )
		{
			factor = rgNoise[noiseIndex>>16] * scale;
			if( flags & FBEAM_SINENOISE )
			{
				float	s, c;
				SinCos( fraction * M_PI * length + freq, &s, &c );

				VectorMA( nextSeg.pos, (factor * s), RI.vup, nextSeg.pos );

				// rotate the noise along the perpendicluar axis a bit to keep the bolt
				// from looking diagonal
				VectorMA( nextSeg.pos, (factor * c), RI.vright, nextSeg.pos );
			}
			else
			{
				VectorMA( nextSeg.pos, factor, perp1, nextSeg.pos );
			}
		}

		// specify the next segment.
		nextSeg.width = width * 2.0f;
		nextSeg.texcoord = vLast;

		if( segs_drawn > 0 )
		{
			// Get a vector that is perpendicular to us and perpendicular to the beam.
			// This is used to fatten the beam.
			vec3_t	vNormal, vAveNormal;

			ComputeNormal( curSeg.pos, nextSeg.pos, vNormal );

			if( segs_drawn > 1 )
			{
				// Average this with the previous normal
				VectorAdd( vNormal, vLastNormal, vAveNormal );
				VectorScale( vAveNormal, 0.5f, vAveNormal );
				VectorNormalizeFast( vAveNormal );
			}
			else
			{
				VectorCopy( vNormal, vAveNormal );
			}

			VectorCopy( vNormal, vLastNormal );

			// draw regular segment
			VectorMA( curSeg.pos, ( curSeg.width * 0.5f ), vAveNormal, vPoint1 );
			VectorMA( curSeg.pos, (-curSeg.width * 0.5f ), vAveNormal, vPoint2 );

			xW->glColor4f( curSeg.color[0], curSeg.color[1], curSeg.color[2], curSeg.alpha );
			xW->glTexCoord2f( 0.0f, curSeg.texcoord );
			//xW->glNormal3fv( vAveNormal );
			xW->glVertex3fv( vPoint1 );

			xW->glColor4f( curSeg.color[0], curSeg.color[1], curSeg.color[2], curSeg.alpha );
			xW->glTexCoord2f( 1.0f, curSeg.texcoord );
			//xW->glNormal3fv( vAveNormal );
			xW->glVertex3fv( vPoint2 );
		}

		curSeg = nextSeg;
		segs_drawn++;

		if( segs_drawn == total_segs )
		{
			// draw the last segment
			VectorMA( curSeg.pos, ( curSeg.width * 0.5f ), vLastNormal, vPoint1 );
			VectorMA( curSeg.pos, (-curSeg.width * 0.5f ), vLastNormal, vPoint2 );

			// specify the points.
			xW->glColor4f( curSeg.color[0], curSeg.color[1], curSeg.color[2], curSeg.alpha );
			xW->glTexCoord2f( 0.0f, curSeg.texcoord );
			//xW->glNormal3fv( vLastNormal );
			xW->glVertex3fv( vPoint1 );

			xW->glColor4f( curSeg.color[0], curSeg.color[1], curSeg.color[2], curSeg.alpha );
			xW->glTexCoord2f( 1.0f, curSeg.texcoord );
			//xW->glNormal3fv( vLastNormal );
			xW->glVertex3fv( vPoint2 );
		}

		vLast += vStep; // Advance texture scroll (v axis only)
		noiseIndex += noiseStep;
	}

	xW->glEnd();
}

/*
================
CL_DrawDisk

Draw beamdisk
================
*/
void CL_DrawDisk( int modelIndex, float frame, int rendermode, const vec3_t source, const vec3_t delta,
	float width, float scale, float freq, float speed, int segments, float *color )
{
	float	div, length, fraction;
	float	w, vLast, vStep;
	VHSPRITE	m_hSprite;
	vec3_t	point;
	int	i;

	m_hSprite = R_GetSpriteTexture( Mod_Handle( modelIndex ), (int)frame );

	if( !m_hSprite || segments < 2 )
		return;

	if( segments > NOISE_DIVISIONS )
		segments = NOISE_DIVISIONS;

	length = VectorLength( delta ) * 0.01f;
	if( length < 0.5f ) length = 0.5f;	// don't lose all of the noise/texture on short beams

	div = 1.0f / (segments - 1);
	vStep = length * div;		// Texture length texels per space pixel

	// Scroll speed 3.5 -- initial texture position, scrolls 3.5/sec (1.0 is entire texture)
	vLast = fmodf( freq * speed, 1.f );
	scale = scale * length;

	w = freq * delta[2];

	SetBeamRenderMode( rendermode );
	GL_Bind( GL_TEXTURE0, m_hSprite );

	xW->glBegin( GL_TRIANGLE_STRIP );

	// NOTE: We must force the degenerate triangles to be on the edge
	for( i = 0; i < segments; i++ )
	{
		float	s, c;

		fraction = i * div;
		VectorCopy( source, point );

		xW->glColor4f( color[0], color[1], color[2], 1.0f );
		xW->glTexCoord2f( 1.0f, vLast );
		xW->glVertex3fv( point );

		SinCos( fraction * M_PI2, &s, &c );
		point[0] = s * w + source[0];
		point[1] = c * w + source[1];
		point[2] = source[2];

		xW->glColor4f( color[0], color[1], color[2], 1.0f );
		xW->glTexCoord2f( 0.0f, vLast );
		xW->glVertex3fv( point );

		vLast += vStep;	// Advance texture scroll (v axis only)
	}

	xW->glEnd();
}

/*
================
CL_DrawCylinder

Draw beam cylinder
================
*/
void CL_DrawCylinder( int modelIndex, float frame, int rendermode, const vec3_t source, const vec3_t delta,
	float width, float scale, float freq, float speed, int segments, float *color )
{
	float	length, fraction;
	float	div, vLast, vStep;
	VHSPRITE	m_hSprite;
	vec3_t	point;
	int	i;

	m_hSprite = R_GetSpriteTexture( Mod_Handle( modelIndex ), (int)frame );

	if( !m_hSprite || segments < 2 )
		return;

	if( segments > NOISE_DIVISIONS )
		segments = NOISE_DIVISIONS;

	length = VectorLength( delta ) * 0.01f;
	if( length < 0.5f ) length = 0.5f;	// Don't lose all of the noise/texture on short beams

	div = 1.0f / (segments - 1);
	vStep = length * div;		// Texture length texels per space pixel

	// Scroll speed 3.5 -- initial texture position, scrolls 3.5/sec (1.0 is entire texture)
	vLast = fmodf( freq * speed, 1.0f );
	scale = scale * length;

	GL_Cull( GL_NONE );	// draw both sides
	SetBeamRenderMode( rendermode );
	GL_Bind( GL_TEXTURE0, m_hSprite );

	xW->glBegin( GL_TRIANGLE_STRIP );

	for( i = 0; i < segments; i++ )
	{
		float	s, c;

		fraction = i * div;
		SinCos( fraction * M_PI2, &s, &c );

		point[0] = s * freq * delta[2] + source[0];
		point[1] = c * freq * delta[2] + source[1];
		point[2] = source[2] + width;

		xW->glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );
		xW->glTexCoord2f( 1.0f, vLast );
		xW->glVertex3fv( point );

		point[0] = s * freq * (delta[2] + width) + source[0];
		point[1] = c * freq * (delta[2] + width) + source[1];
		point[2] = source[2] - width;

		xW->glColor4f( color[0], color[1], color[2], 1.0f );
		xW->glTexCoord2f( 0.0f, vLast );
		xW->glVertex3fv( point );

		vLast += vStep;	// Advance texture scroll (v axis only)
	}

	xW->glEnd();
	GL_Cull( GL_FRONT );
}

/*
================
CL_DrawRing

Draw beamring
================
*/
void CL_DrawRing( int modelIndex, float frame, int rendermode, const vec3_t source, const vec3_t delta, float width,
	float amplitude, float freq, float speed, int segments, float *color )
{
	int	i, j, noiseIndex, noiseStep;
	float	div, length, fraction, factor, vLast, vStep;
	vec3_t	last1, last2, point, screen, screenLast;
	vec3_t	center, xaxis, yaxis, zaxis, tmp, normal;
	float	radius, x, y, scale;
	VHSPRITE	m_hSprite;
	vec3_t	d;

	m_hSprite = R_GetSpriteTexture( Mod_Handle( modelIndex ), (int)frame );

	if( !m_hSprite || segments < 2 )
		return;

	VectorCopy( delta, d );

	VectorClear( screenLast );
	segments = (int)((double)segments * M_PI);

	if( segments > NOISE_DIVISIONS * 8 )
		segments = NOISE_DIVISIONS * 8;

	length = VectorLength( d ) * 0.01f * M_PI;
	if( length < 0.5f ) length = 0.5f;		// Don't lose all of the noise/texture on short beams

	div = 1.0f / (float)( segments - 1 );

	vStep = length * div / 8.0f;			// Texture length texels per space pixel

	// Scroll speed 3.5 -- initial texture position, scrolls 3.5/sec (1.0 is entire texture)
	vLast = fmodf( freq * speed, 1.0f );
	scale = amplitude * length / 8.0f;

	// Iterator to resample noise waveform (it needs to be generated in powers of 2)
	noiseStep = (int)(( NOISE_DIVISIONS - 1 ) * div * 65536.0f ) * 8;
	noiseIndex = 0;

	VectorScale( d, 0.5f, d );
	VectorAdd( source, d, center );
	VectorClear( zaxis );

	VectorCopy( d, xaxis );
	radius = VectorLength( xaxis );

	// cull beamring
	// --------------------------------
	// Compute box center +/- radius
	last1[0] = radius;
	last1[1] = radius;
	last1[2] = scale;

	VectorAdd( center, last1, tmp );
	VectorSubtract( center, last1, screen );

	// Is that box in PVS && frustum?
	if( !Mod_BoxVisible( screen, tmp, Mod_GetCurrentVis( )) || R_CullBox( screen, tmp, RI.clipFlags ))
	{
		return;
	}

	yaxis[0] = xaxis[1];
	yaxis[1] = -xaxis[0];
	yaxis[2] = 0;

	VectorNormalize( yaxis );
	VectorScale( yaxis, radius, yaxis );

	j = segments / 8;

	SetBeamRenderMode( rendermode );
	GL_Bind( GL_TEXTURE0, m_hSprite );

	xW->glBegin( GL_TRIANGLE_STRIP );

	for( i = 0; i < segments + 1; i++ )
	{
		fraction = i * div;
		SinCos( fraction * M_PI2, &x, &y );

		VectorMAMAM( x, xaxis, y, yaxis, 1.0f, center, point );

		// Distort using noise
		factor = rgNoise[(noiseIndex >> 16) & (NOISE_DIVISIONS - 1)] * scale;
		VectorMA( point, factor, RI.vup, point );

		// Rotate the noise along the perpendicluar axis a bit to keep the bolt from looking diagonal
		factor = rgNoise[(noiseIndex>>16) & (NOISE_DIVISIONS - 1)] * scale;
		factor *= cosf( fraction * (float)M_PI * 24.f + freq );
		VectorMA( point, factor, RI.vright, point );

		// Transform point into screen space
		TriWorldToScreen( point, screen );

		if( i != 0 )
		{
			// Build world-space normal to screen-space direction vector
			VectorSubtract( screen, screenLast, tmp );

			// We don't need Z, we're in screen space
			tmp[2] = 0;
			VectorNormalize( tmp );

			// Build point along normal line (normal is -y, x)
			VectorScale( RI.vup, tmp[0], normal );
			VectorMA( normal, tmp[1], RI.vright, normal );

			// make a wide line
			VectorMA( point, width, normal, last1 );
			VectorMA( point, -width, normal, last2 );

			vLast += vStep;	// Advance texture scroll (v axis only)
			xW->glColor4f( color[0], color[1], color[2], 1.0f );
			xW->glTexCoord2f( 1.0f, vLast );
			xW->glVertex3fv( last2 );

			xW->glColor4f( color[0], color[1], color[2], 1.0f );
			xW->glTexCoord2f( 0.0f, vLast );
			xW->glVertex3fv( last1 );
		}

		VectorCopy( screen, screenLast );
		noiseIndex += noiseStep;

		j--;
		if( j == 0 && amplitude != 0 )
		{
			j = segments / 8;
			FracNoise( rgNoise, NOISE_DIVISIONS, 1.0f );
		}
	}

	xW->glEnd();
}

/*
==============
CL_DrawLaser

Helper to drawing laser
==============
*/
void CL_DrawLaser( BEAM *pbeam, int frame, int rendermode, float *color, int spriteIndex )
{
	float	color2[3];
	vec3_t	beamDir;
	float	flDot;

	VectorCopy( color, color2 );

	VectorSubtract( pbeam->target, pbeam->source, beamDir );
	VectorNormalize( beamDir );

	flDot = DotProduct( beamDir, RI.vforward );

	// abort if the player's looking along it away from the source
	if( flDot > 0 )
	{
		return;
	}
	else
	{
		// Fade the beam if the player's not looking at the source
		float	flFade = powf( flDot, 10.f );
		vec3_t	localDir, vecProjection, tmp;
		float	flDistance;

		// Fade the beam based on the player's proximity to the beam
		VectorSubtract( RI.vieworg, pbeam->source, localDir );
		flDot = DotProduct( beamDir, localDir );
		VectorScale( beamDir, flDot, vecProjection );
		VectorSubtract( localDir, vecProjection, tmp );
		flDistance = VectorLength( tmp );

		if( flDistance > 30 )
		{
			flDistance = 1.0f - (( flDistance - 30.0f ) / 64.0f );

			if( flDistance <= 0 ) flFade = 0;
			else flFade *= powf( flDistance, 3.f );
		}

		if( flFade < ( 1.0f / 255.0f ))
			return;

		VectorScale( color2, flFade, color2 );
	}

	CL_DrawSegs( spriteIndex, (float)frame, rendermode, pbeam->source, pbeam->delta, pbeam->width, pbeam->amplitude,
		pbeam->freq, pbeam->speed, pbeam->segments, pbeam->flags, color2 );

}

/*
================
CL_DrawBeamFollow

Draw beam trail
================
*/
void DrawBeamFollow( int modelIndex, particle_t *pHead, int frame, int rendermode, vec3_t delta,
	vec3_t screen, vec3_t screenLast, float die, const vec3_t source, int flags, float width,
	float amplitude, float freq, float *color )
{
	float	fraction;
	float	div;
	float	vLast = 0.0;
	float	vStep = 1.0;
	vec3_t	last1, last2, tmp, normal, scaledColor;
	VHSPRITE	m_hSprite;
	rgb_t	nColor;

	m_hSprite = R_GetSpriteTexture( Mod_Handle( modelIndex ), frame );
	if( !m_hSprite ) return;

	// UNDONE: This won't work, screen and screenLast must be extrapolated here to fix the
	// first beam segment for this trail

	// Build world-space normal to screen-space direction vector
	VectorSubtract( screen, screenLast, tmp );
	// We don't need Z, we're in screen space
	tmp[2] = 0;
	VectorNormalize( tmp );

	// Build point along noraml line (normal is -y, x)
	VectorScale( RI.vup, tmp[0], normal );	// Build point along normal line (normal is -y, x)
	VectorMA( normal, tmp[1], RI.vright, normal );

	// make a wide line
	VectorMA( delta, width, normal, last1 );
	VectorMA( delta, -width, normal, last2 );

	div = 1.0f / amplitude;
	fraction = ( die - (float)cl.time ) * div;

	VectorScale( color, fraction, scaledColor );
	nColor[0] = (byte)bound( 0, (int)(scaledColor[0] * 255.0f), 255 );
	nColor[1] = (byte)bound( 0, (int)(scaledColor[1] * 255.0f), 255 );
	nColor[2] = (byte)bound( 0, (int)(scaledColor[2] * 255.0f), 255 );

	SetBeamRenderMode( rendermode );
	GL_Bind( GL_TEXTURE0, m_hSprite );

	xW->glBegin( GL_QUADS );

	while( pHead )
	{
		xW->glColor4ub( nColor[0], nColor[1], nColor[2], 255 );
		xW->glTexCoord2f( 1.0f, 1.0f );
		xW->glVertex3fv( last2 );

		xW->glColor4ub( nColor[0], nColor[1], nColor[2], 255 );
		xW->glTexCoord2f( 0.0f, 1.0f );
		xW->glVertex3fv( last1 );

		// Transform point into screen space
		TriWorldToScreen( pHead->org, screen );

		// Build world-space normal to screen-space direction vector
		VectorSubtract( screen, screenLast, tmp );
		// We don't need Z, we're in screen space
		tmp[2] = 0;
		VectorNormalize( tmp );

		// build point along normal line (normal is -y, x)
		VectorScale( RI.vup, tmp[0], normal );
		VectorMA( normal, tmp[1], RI.vright, normal );

		// Make a wide line
		VectorMA( pHead->org,  width, normal, last1 );
		VectorMA( pHead->org, -width, normal, last2 );

		vLast += vStep;	// Advance texture scroll (v axis only)

		if( pHead->next != NULL )
		{
			fraction = (pHead->die - (float)cl.time ) * div;
			VectorScale( color, fraction, scaledColor );
			nColor[0] = (byte)bound( 0, (int)(scaledColor[0] * 255.0f), 255 );
			nColor[1] = (byte)bound( 0, (int)(scaledColor[1] * 255.0f), 255 );
			nColor[2] = (byte)bound( 0, (int)(scaledColor[2] * 255.0f), 255 );
		}
		else
		{
			VectorClear( nColor );
			fraction = 0.0;
		}

		xW->glColor4ub( nColor[0], nColor[1], nColor[2], 255 );
		xW->glTexCoord2f( 0.0f, 0.0f );
		xW->glVertex3fv( last1 );

		xW->glColor4ub( nColor[0], nColor[1], nColor[2], 255 );
		xW->glTexCoord2f( 1.0f, 0.0f );
		xW->glVertex3fv( last2 );

		VectorCopy( screen, screenLast );

		pHead = pHead->next;
	}

	xW->glEnd();
}
void DrawGLPoly( glpoly_t *p, float xScale, float yScale )
{
	float		*v;
	float		sOffset, sy;
	float		tOffset, cy;
	cl_entity_t	*e = RI.currententity;
	int		i, hasScale = false;

	// special hack for non-lightmapped surfaces
	if( p->flags & SURF_DRAWTILED )
		GL_ResetFogColor();

	if( p->flags & SURF_CONVEYOR )
	{
		gltexture_t	*texture;
		float		flConveyorSpeed;
		float		flRate, flAngle;

		flConveyorSpeed = (e->curstate.rendercolor.g<<8|e->curstate.rendercolor.b) / 16.0f;
		if( e->curstate.rendercolor.r ) flConveyorSpeed = -flConveyorSpeed;
		texture = R_GetTexture( glState.currentTextures[glState.activeTMU] );

		flRate = fabsf( flConveyorSpeed ) / (float)texture->srcWidth;
		flAngle = ( flConveyorSpeed >= 0 ) ? 180.f : 0.f;

		SinCos( flAngle * ( M_PI / 180.0f ), &sy, &cy );
		sOffset = RI.refdef.time * cy * flRate;
		tOffset = RI.refdef.time * sy * flRate;

		// make sure that we are positive
		if( sOffset < 0.0f ) sOffset += 1.0f + -(int)sOffset;
		if( tOffset < 0.0f ) tOffset += 1.0f + -(int)tOffset;

		// make sure that we are in a [0,1] range
		sOffset = sOffset - (int)sOffset;
		tOffset = tOffset - (int)tOffset;
	}
	else
	{
		sOffset = tOffset = 0.0f;
	}

	if( xScale != 0.0f && yScale != 0.0f )
		hasScale = true;

	xW->glBegin( GL_POLYGON );

	for( i = 0, v = p->verts[0]; i < p->numverts; i++, v += VERTEXSIZE )
	{
		if( hasScale )
			xW->glTexCoord2f(( v[3] + sOffset ) * xScale, ( v[4] + tOffset ) * yScale );
		else xW->glTexCoord2f( v[3] + sOffset, v[4] + tOffset );

		xW->glVertex3fv( v );
	}

	xW->glEnd();

	// special hack for non-lightmapped surfaces
	if( p->flags & SURF_DRAWTILED )
		GL_SetupFogColorForSurfaces();
}

/*
================
DrawGLPolyChain

Render lightmaps
================
*/
void DrawGLPolyChain( glpoly_t *p, float soffset, float toffset )
{
	qboolean	dynamic = true;

	if( soffset == 0.0f && toffset == 0.0f )
		dynamic = false;

	for( ; p != NULL; p = p->chain )
	{
		float	*v;
		int	i;

		xW->glBegin( GL_POLYGON );

		v = p->verts[0];
		for( i = 0; i < p->numverts; i++, v += VERTEXSIZE )
		{
			if( !dynamic ) xW->glTexCoord2f( v[5], v[6] );
			else xW->glTexCoord2f( v[5] - soffset, v[6] - toffset );
			xW->glVertex3fv( v );
		}
		xW->glEnd ();
	}
}

/*
================
R_BlendLightmaps
================
*/
void R_BlendLightmaps( void )
{
	msurface_t	*surf, *newsurf = NULL;
	mextrasurf_t	*info;
	int		i;

	if( r_fullbright->integer || !cl.worldmodel->lightdata )
		return;

	GL_SetupFogColorForSurfaces ();

	if( RI.currententity )
	{
		// check for rendermode
		switch( RI.currententity->curstate.rendermode )
		{
		case kRenderTransTexture:
		case kRenderTransColor:
		case kRenderTransAdd:
		case kRenderGlow:
			return; // no lightmaps
		}

		if( RI.currententity->curstate.effects & EF_FULLBRIGHT )
			return;	// disabled by user
	}

	if( !r_lightmap->integer )
	{
		xW->glEnable( GL_BLEND );

		if( !glState.drawTrans )
		{
			// lightmapped solid surfaces
			xW->glDepthMask( GL_FALSE );
			xW->glDepthFunc( GL_EQUAL );
		}

		xW->glDisable( GL_ALPHA_TEST );
		xW->glBlendFunc( GL_ZERO, GL_SRC_COLOR );
//		//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	}

	// render static lightmaps first
	for( i = 0; i < MAX_LIGHTMAPS; i++ )
	{
		if( gl_lms.lightmap_surfaces[i] )
		{
			GL_Bind( GL_TEXTURE0, tr.lightmapTextures[i] );

			for( surf = gl_lms.lightmap_surfaces[i]; surf != NULL; surf = surf->lightmapchain )
			{
				if( surf->polys ) DrawGLPolyChain( surf->polys, 0.0f, 0.0f );
			}
		}
	}

	// render dynamic lightmaps
	if( r_dynamic->integer )
	{
		LM_InitBlock();

		if( host.features & ENGINE_LARGE_LIGHTMAPS )
			GL_Bind( GL_TEXTURE0, tr.dlightTexture2 );
		else GL_Bind( GL_TEXTURE0, tr.dlightTexture );

		newsurf = gl_lms.dynamic_surfaces;

		for( surf = gl_lms.dynamic_surfaces; surf != NULL; surf = surf->lightmapchain )
		{
			int	smax, tmax;
			byte	*base;

			smax = ( surf->extents[0] / LM_SAMPLE_SIZE ) + 1;
			tmax = ( surf->extents[1] / LM_SAMPLE_SIZE ) + 1;
			info = SURF_INFO( surf, RI.currentmodel );

			if( LM_AllocBlock( smax, tmax, &info->dlight_s, &info->dlight_t ))
			{
				base = gl_lms.lightmap_buffer;
				base += ( info->dlight_t * BLOCK_SIZE + info->dlight_s ) * 4;

				R_BuildLightMap( surf, base, BLOCK_SIZE * 4, true );
			}
			else
			{
				msurface_t	*drawsurf;

				// upload what we have so far
				LM_UploadBlock( true );

				// draw all surfaces that use this lightmap
				for( drawsurf = newsurf; drawsurf != surf; drawsurf = drawsurf->lightmapchain )
				{
					if( drawsurf->polys )
					{
						info = SURF_INFO( drawsurf, RI.currentmodel );

						DrawGLPolyChain( drawsurf->polys,
						( drawsurf->light_s - info->dlight_s ) * ( 1.0f / (float)BLOCK_SIZE ),
						( drawsurf->light_t - info->dlight_t ) * ( 1.0f / (float)BLOCK_SIZE ));
					}
				}

				newsurf = drawsurf;

				// clear the block
				LM_InitBlock();

				info = SURF_INFO( surf, RI.currentmodel );

				// try uploading the block now
				if( !LM_AllocBlock( smax, tmax, &info->dlight_s, &info->dlight_t ))
					Host_Error( "AllocBlock: full\n" );

				base = gl_lms.lightmap_buffer;
				base += ( info->dlight_t * BLOCK_SIZE + info->dlight_s ) * 4;

				R_BuildLightMap( surf, base, BLOCK_SIZE * 4, true );
			}
		}

		// draw remainder of dynamic lightmaps that haven't been uploaded yet
		if( newsurf ) LM_UploadBlock( true );

		for( surf = newsurf; surf != NULL; surf = surf->lightmapchain )
		{
			if( surf->polys )
			{
				info = SURF_INFO( surf, RI.currentmodel );

				DrawGLPolyChain( surf->polys,
				( surf->light_s - info->dlight_s ) * ( 1.0f / (float)BLOCK_SIZE ),
				( surf->light_t - info->dlight_t ) * ( 1.0f / (float)BLOCK_SIZE ));
			}
		}
	}

	if( !r_lightmap->integer )
	{
		xW->glDisable( GL_BLEND );

		if( !glState.drawTrans )
		{
			// restore depth state
			xW->glDepthMask( GL_TRUE );
			xW->glDepthFunc( GL_LEQUAL );
		}

		xW->glDisable( GL_ALPHA_TEST );
//		//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	}

	// restore fog here
	GL_ResetFogColor();
}

/*
================
R_RenderFullbrights
================
*/
void R_RenderFullbrights()
{
	glpoly_t	*p;
	int	i;

	if( !draw_fullbrights )
		return;

	if( RI.fogEnabled && !RI.refdef.onlyClientDraw )
		xW->glDisable( GL_FOG );

	xW->glEnable( GL_BLEND );
	xW->glDepthMask( GL_FALSE );
	xW->glDisable( GL_ALPHA_TEST );
	xW->glBlendFunc( GL_ONE, GL_ONE );
//	//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	for( i = 1; i < MAX_TEXTURES; i++ )
	{
		if( !fullbright_polys[i] )
			continue;
		GL_Bind( GL_TEXTURE0, i );

		for( p = fullbright_polys[i]; p; p = p->next )
		{
			if( p->flags & SURF_DRAWTURB )
				EmitWaterPolys( p, ( p->flags & SURF_NOCULL ));
			else DrawGLPoly( p, 0.0f, 0.0f );
		}

		fullbright_polys[i] = NULL;
	}

	xW->glDisable( GL_BLEND );
	xW->glDepthMask( GL_TRUE );
	xW->glDisable( GL_ALPHA_TEST );
	//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

	draw_fullbrights = false;

	// restore for here
	if( RI.fogEnabled && !RI.refdef.onlyClientDraw )
		xW->glEnable( GL_FOG );
}

/*
================
R_RenderDetails
================
*/
void R_RenderDetails()
{
	gltexture_t	*glt;
	mextrasurf_t	*es, *p;
	msurface_t	*fa;
	int		i;

	if( !draw_details )
		return;

	GL_SetupFogColorForSurfaces();

	xW->glEnable( GL_BLEND );
	xW->glBlendFunc( GL_DST_COLOR, GL_SRC_COLOR );
	//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );

	if( RI.currententity->curstate.rendermode == kRenderTransAlpha )
		xW->glDepthFunc( GL_EQUAL );

	for( i = 1; i < MAX_TEXTURES; i++ )
	{
		es = detail_surfaces[i];
		if( !es ) continue;

		GL_Bind( GL_TEXTURE0, i );

		for( p = es; p; p = p->detailchain )
		{
			fa = INFO_SURF( p, RI.currentmodel );
			glt = R_GetTexture( fa->texinfo->texture->gl_texturenum ); // get texture scale
			DrawGLPoly( fa->polys, glt->xscale, glt->yscale );
					}

		detail_surfaces[i] = NULL;
		es->detailchain = NULL;
	}

	xW->glDisable( GL_BLEND );
	//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

	if( RI.currententity->curstate.rendermode == kRenderTransAlpha )
		xW->glDepthFunc( GL_LEQUAL );

	draw_details = false;

	// restore fog here
	GL_ResetFogColor();
}

/*
================
R_RenderBrushPoly
================
*/
void R_RenderBrushPoly( msurface_t *fa )
{
	texture_t	*t;
	int	maps;
	qboolean	is_dynamic = false;
	qboolean	is_mirror = false;

	if( RI.currententity == clgame.entities )
		r_stats.c_world_polys++;
	else r_stats.c_brush_polys++;

	if( fa->flags & SURF_DRAWSKY )
	{
		if( world.sky_sphere )
		{
			// warp texture, no lightmaps
			EmitSkyLayers( fa );
		}
		return;
	}

	t = R_TextureAnimation( fa->texinfo->texture, fa - RI.currententity->model->surfaces );

	if( RP_NORMALPASS() && fa->flags & SURF_REFLECT )
	{
		if( SURF_INFO( fa, RI.currentmodel )->mirrortexturenum )
		{
			GL_Bind( GL_TEXTURE0, SURF_INFO( fa, RI.currentmodel )->mirrortexturenum );
			is_mirror = true;

			// BEGIN WATER STUFF
			if( fa->flags & SURF_DRAWTURB )
			{
				R_BeginDrawMirror( fa );
				GL_Bind( GL_TEXTURE1, t->gl_texturenum );
				//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
			}
		}
		else GL_Bind( GL_TEXTURE0, t->gl_texturenum ); // dummy

		// DEBUG: reset the mirror texture after drawing
		SURF_INFO( fa, RI.currentmodel )->mirrortexturenum = 0;
	}
	else GL_Bind( GL_TEXTURE0, t->gl_texturenum );

	if( fa->flags & SURF_DRAWTURB )
	{
		// warp texture, no lightmaps
		EmitWaterPolys( fa->polys, ( fa->flags & SURF_NOCULL ));
		if( is_mirror ) R_EndDrawMirror();
		// END WATER STUFF
		return;
	}

	if( t->fb_texturenum )
	{
		// HACKHACK: store fullbrights in poly->next (only for non-water surfaces)
		fa->polys->next = fullbright_polys[t->fb_texturenum];
		fullbright_polys[t->fb_texturenum] = fa->polys;
		draw_fullbrights = true;
	}

	if( r_detailtextures->integer )
	{
		mextrasurf_t *es = SURF_INFO( fa, RI.currentmodel );

		if( RI.fogEnabled || RI.fogCustom )
		{
			// don't apply detail textures for windows in the fog
			if( RI.currententity->curstate.rendermode != kRenderTransTexture )
			{
				if( t->dt_texturenum )
				{
					es->detailchain = detail_surfaces[t->dt_texturenum];
					detail_surfaces[t->dt_texturenum] = es;
				}
				else
				{
					// draw stub detail texture for underwater surfaces
					es->detailchain = detail_surfaces[tr.grayTexture];
					detail_surfaces[tr.grayTexture] = es;
				}
				draw_details = true;
			}
		}
		else if( t->dt_texturenum )
		{
			es->detailchain = detail_surfaces[t->dt_texturenum];
			detail_surfaces[t->dt_texturenum] = es;
			draw_details = true;
		}
	}

	if( is_mirror ) R_BeginDrawMirror( fa );
	DrawGLPoly( fa->polys, 0.0f, 0.0f );
	if( is_mirror ) R_EndDrawMirror();
	DrawSurfaceDecals( fa );

	// NOTE: draw mirror through in mirror show dummy lightmapped texture
	if( fa->flags & SURF_REFLECT && RP_NORMALPASS() && r_lighting_extended->integer )
		return; // no lightmaps for mirror

	if( fa->flags & SURF_DRAWTILED )
		return; // no lightmaps anyway

	// check for lightmap modification
	for( maps = 0; maps < MAXLIGHTMAPS && fa->styles[maps] != 255; maps++ )
	{
		if( RI.lightstylevalue[fa->styles[maps]] != fa->cached_light[maps] )
			goto dynamic;
	}

	// dynamic this frame or dynamic previously
	if(( fa->dlightframe == tr.framecount ))
	{
dynamic:
		// NOTE: at this point we have only valid textures
		if( r_dynamic->integer ) is_dynamic = true;
	}

	if( is_dynamic )
	{
		if(( fa->styles[maps] >= 32 || fa->styles[maps] == 0 ) && ( fa->dlightframe != tr.framecount ))
		{
			byte	temp[132*132*4];
			int	smax, tmax;

			smax = ( fa->extents[0] / LM_SAMPLE_SIZE ) + 1;
			tmax = ( fa->extents[1] / LM_SAMPLE_SIZE ) + 1;

			R_BuildLightMap( fa, temp, smax * 4, true );
			R_SetCacheState( fa );

			GL_Bind( GL_TEXTURE0, tr.lightmapTextures[fa->lightmaptexturenum] );

			xW->glTexSubImage2D( GL_TEXTURE_2D, 0, fa->light_s, fa->light_t, smax, tmax,
				GL_RGBA, GL_UNSIGNED_BYTE, temp );

			fa->lightmapchain = gl_lms.lightmap_surfaces[fa->lightmaptexturenum];
			gl_lms.lightmap_surfaces[fa->lightmaptexturenum] = fa;
		}
		else
		{
			fa->lightmapchain = gl_lms.dynamic_surfaces;
			gl_lms.dynamic_surfaces = fa;
		}
	}
	else
	{
		fa->lightmapchain = gl_lms.lightmap_surfaces[fa->lightmaptexturenum];
		gl_lms.lightmap_surfaces[fa->lightmaptexturenum] = fa;
	}
}

void R_DrawSpriteModel( cl_entity_t *e )
{
	mspriteframe_t	*frame, *oldframe;
	msprite_t		*psprite;
	model_t		*model;
	int		i, alpha, type;
	float		angle, dot, sr, cr, flAlpha;
	float		lerp = 1.0f, ilerp, scale;
	vec3_t		v_forward, v_right, v_up;
	vec3_t		origin, color, color2;

	if( RI.params & RP_ENVVIEW )
		return;

	model = e->model;
	psprite = (msprite_t * )model->cache.data;
	VectorCopy( e->origin, origin );	// set render origin

	// do movewith
	if( e->curstate.aiment > 0 && e->curstate.movetype == MOVETYPE_FOLLOW )
	{
		cl_entity_t	*parent;

		parent = CL_GetEntityByIndex( e->curstate.aiment );

		if( parent && parent->model )
		{
			if( parent->model->type == mod_studio && e->curstate.body > 0 )
			{
				int num = bound( 1, e->curstate.body, MAXSTUDIOATTACHMENTS );
				VectorCopy( parent->attachment[num-1], origin );
			}
			else VectorCopy( parent->origin, origin );
		}
	}

	alpha = e->curstate.renderamt;
	scale = e->curstate.scale;

	if( R_SpriteOccluded( e, origin, &alpha, &scale ))
		return; // sprite culled

	r_stats.c_sprite_models_drawn++;

	if( psprite->texFormat == SPR_ALPHTEST && e->curstate.rendermode != kRenderTransAdd )
	{
		xW->glEnable( GL_ALPHA_TEST );
		////xW->glAlphaFunc( GL_GREATER, 0.0f );
	}

	if( e->curstate.rendermode == kRenderGlow || e->curstate.rendermode == kRenderWorldGlow )
		xW->glDisable( GL_DEPTH_TEST );

	// select properly rendermode
	switch( e->curstate.rendermode )
	{
	case kRenderTransAlpha:
	case kRenderTransColor:
	case kRenderTransTexture:
		xW->glEnable( GL_BLEND );
		xW->glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		break;
	case kRenderGlow:
	case kRenderTransAdd:
	case kRenderWorldGlow:
		xW->glDisable( GL_FOG );
		xW->glEnable( GL_BLEND );
		xW->glBlendFunc( GL_SRC_ALPHA, GL_ONE );
		break;
	case kRenderNormal:
	default:
		if( psprite->texFormat == SPR_INDEXALPHA )
		{
			xW->glEnable( GL_BLEND );
			xW->glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		}
		else xW->glDisable( GL_BLEND );
		break;
	}

	// all sprites can have color
	//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	// add basecolor (any rendermode can colored sprite)
	color[0] = (float)e->curstate.rendercolor.r * ( 1.0f / 255.0f );
	color[1] = (float)e->curstate.rendercolor.g * ( 1.0f / 255.0f );
	color[2] = (float)e->curstate.rendercolor.b * ( 1.0f / 255.0f );

	if( R_SpriteHasLightmap( e, psprite->texFormat ))
	{
		color24	lightColor;
		qboolean	invLight;

		invLight = (e->curstate.effects & EF_INVLIGHT) ? true : false;
		R_LightForPoint( origin, &lightColor, invLight, true, sprite_radius );
		color2[0] = (float)lightColor.r * ( 1.0f / 255.0f );
		color2[1] = (float)lightColor.g * ( 1.0f / 255.0f );
		color2[2] = (float)lightColor.b * ( 1.0f / 255.0f );

		if( glState.drawTrans )
			xW->glDepthMask( GL_TRUE );

		// NOTE: sprites with 'lightmap' looks ugly when alpha func is GL_GREATER 0.0
		////xW->glAlphaFunc( GL_GEQUAL, 0.5f );
	}

	if( e->curstate.rendermode == kRenderNormal || e->curstate.rendermode == kRenderTransAlpha )
		frame = oldframe = R_GetSpriteFrame( model, (int)e->curstate.frame, e->angles[YAW] );
	else lerp = R_GetSpriteFrameInterpolant( e, &oldframe, &frame );

	type = psprite->type;

	// automatically roll parallel sprites if requested
	if( e->angles[ROLL] != 0.0f && type == SPR_FWD_PARALLEL )
		type = SPR_FWD_PARALLEL_ORIENTED;

	switch( type )
	{
	case SPR_ORIENTED:
		AngleVectors( e->angles, v_forward, v_right, v_up );
		VectorScale( v_forward, 0.01f, v_forward );	// to avoid z-fighting
		VectorSubtract( origin, v_forward, origin );
		break;
	case SPR_FACING_UPRIGHT:
		VectorSet( v_right, origin[1] - RI.vieworg[1], -(origin[0] - RI.vieworg[0]), 0.0f );
		VectorSet( v_up, 0.0f, 0.0f, 1.0f );
		VectorNormalize( v_right );
		break;
	case SPR_FWD_PARALLEL_UPRIGHT:
		dot = RI.vforward[2];
		if(( dot > 0.999848f ) || ( dot < -0.999848f ))	// cos(1 degree) = 0.999848
			return; // invisible
		VectorSet( v_up, 0.0f, 0.0f, 1.0f );
		VectorSet( v_right, RI.vforward[1], -RI.vforward[0], 0.0f );
		VectorNormalize( v_right );
		break;
	case SPR_FWD_PARALLEL_ORIENTED:
		angle = e->angles[ROLL] * (M_PI2 / 360.0f);
		SinCos( angle, &sr, &cr );
		for( i = 0; i < 3; i++ )
		{
			v_right[i] = (RI.vright[i] * cr + RI.vup[i] * sr);
			v_up[i] = RI.vright[i] * -sr + RI.vup[i] * cr;
		}
		break;
	case SPR_FWD_PARALLEL: // normal sprite
	default:
		VectorCopy( RI.vright, v_right );
		VectorCopy( RI.vup, v_up );
		break;
	}

	flAlpha = (float)alpha * ( 1.0f / 255.0f );

	if( psprite->facecull == SPR_CULL_NONE )
		GL_Cull( GL_NONE );

	if( oldframe == frame )
	{
		// draw the single non-lerped frame
		xW->glColor4f( color[0], color[1], color[2], flAlpha );
		GL_Bind( GL_TEXTURE0, frame->gl_texturenum );
		R_DrawSpriteQuad( frame, origin, v_right, v_up, scale );
	}
	else
	{
		// draw two combined lerped frames
		lerp = bound( 0.0f, lerp, 1.0f );
		ilerp = 1.0f - lerp;

		if( ilerp != 0.0f )
		{
			xW->glColor4f( color[0], color[1], color[2], flAlpha * ilerp );
			GL_Bind( GL_TEXTURE0, oldframe->gl_texturenum );
			R_DrawSpriteQuad( oldframe, origin, v_right, v_up, scale );
		}

		if( lerp != 0.0f )
		{
			xW->glColor4f( color[0], color[1], color[2], flAlpha * lerp );
			GL_Bind( GL_TEXTURE0, frame->gl_texturenum );
			R_DrawSpriteQuad( frame, origin, v_right, v_up, scale );
		}
	}

	// draw the sprite 'lightmap' :-)
	if( R_SpriteHasLightmap( e, psprite->texFormat ))
	{
		xW->glEnable( GL_BLEND );
		xW->glDepthFunc( GL_EQUAL );
		xW->glDisable( GL_ALPHA_TEST );
		xW->glBlendFunc( GL_ZERO, GL_SRC_COLOR );
		//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

		xW->glColor4f( color2[0], color2[1], color2[2], flAlpha );
		GL_Bind( GL_TEXTURE0, tr.whiteTexture );
		R_DrawSpriteQuad( frame, origin, v_right, v_up, scale );

		if( glState.drawTrans )
			xW->glDepthMask( GL_FALSE );
	}

	if( psprite->facecull == SPR_CULL_NONE )
		GL_Cull( GL_FRONT );

	if( e->curstate.rendermode == kRenderGlow || e->curstate.rendermode == kRenderWorldGlow )
		xW->glEnable( GL_DEPTH_TEST );

	if( psprite->texFormat == SPR_ALPHTEST && e->curstate.rendermode != kRenderTransAdd )
		xW->glDisable( GL_ALPHA_TEST );

	xW->glDisable( GL_BLEND );
	xW->glDepthFunc( GL_LEQUAL );
	//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	xW->glColor4ub( 255, 255, 255, 255 );

	if( RI.fogCustom || ( RI.fogEnabled && !glState.drawTrans ))
		xW->glEnable( GL_FOG );
}

void R_ShowTextures()
{
	gltexture_t	*image;
	float		x, y, w, h;
	int		i, j, k, base_w, base_h;
	int		total, start, end;
	rgba_t		color = { 192, 192, 192, 255 };
	int		charHeight, numTries = 0;
	static qboolean	showHelp = true;
	string		shortname;

	if( !gl_showtextures->integer )
		return;

	if( showHelp )
	{
		CL_CenterPrint( "use '<-' and '->' keys for view all the textures", 0.25f );
		showHelp = false;
	}

	xW->glClear( GL_COLOR_BUFFER_BIT );
	////xW->glFinish();

	base_w = 8;
	base_h = 6;

rebuild_page:
	total = base_w * base_h;
	start = total * (gl_showtextures->integer - 1);
	end = total * gl_showtextures->integer;
	if( end > MAX_TEXTURES ) end = MAX_TEXTURES;

	w = (float)glState.width / (float)base_w;
	h = (float)glState.height / (float)base_h;

	Con_DrawStringLen( NULL, NULL, &charHeight );

	for( i = j = 0; i < MAX_TEXTURES; i++ )
	{
		image = R_GetTexture( i );
		if( j == start ) break; // found start
		if( xW->glIsTexture( image->texid )) j++;
	}

	if( i == MAX_TEXTURES && gl_showtextures->integer != 1 )
	{
		// bad case, rewind to one and try again
		Cvar_SetFloat( "r_showtextures", max( 1.f, (float)(gl_showtextures->integer - 1) ));
		if( ++numTries < 2 ) goto rebuild_page;	// to prevent infinite loop
	}

	for( k = 0; i < MAX_TEXTURES; i++ )
	{
		if( j == end ) break; // page is full

		image = R_GetTexture( i );
		if( !xW->glIsTexture( image->texid ))
			continue;

		x = k % base_w * w;
		y = k / base_w * h;

		xW->glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
		GL_Bind( GL_TEXTURE0, image->texid ); // NOTE: don't use image->texnum here, because skybox has a 'wrong' indexes

		if(( image->flags & TF_DEPTHMAP ) && !( image->flags & TF_NOCOMPARE ))
			xW->glTexParameteri( image->target, GL_TEXTURE_COMPARE_MODE_ARB, GL_NONE );

		xW->glBegin( GL_QUADS );
		xW->glTexCoord2f( 0, 0 );
		xW->glVertex2f( x, y );
		if( image->flags & TF_TEXTURE_RECTANGLE )
			xW->glTexCoord2f( image->width, 0 );
		else xW->glTexCoord2f( 1, 0 );
		xW->glVertex2f( x + w, y );
		if( image->flags & TF_TEXTURE_RECTANGLE )
			xW->glTexCoord2f( image->width, image->height );
		else xW->glTexCoord2f( 1, 1 );
		xW->glVertex2f( x + w, y + h );
		if( image->flags & TF_TEXTURE_RECTANGLE )
			xW->glTexCoord2f( 0, image->height );
		else xW->glTexCoord2f( 0, 1 );
		xW->glVertex2f( x, y + h );
		xW->glEnd();

		if(( image->flags & TF_DEPTHMAP ) && !( image->flags & TF_NOCOMPARE ))
			xW->glTexParameteri( image->target, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB );

		FS_FileBase( image->name, shortname );
		if( Q_strlen( shortname ) > 18 )
		{
			// cutoff too long names, it looks ugly
			shortname[16] = '.';
			shortname[17] = '.';
			shortname[18] = '\0';
		}
		Con_DrawString( (int)(x + 1), (int)(y + h - charHeight), shortname, color );
		j++, k++;
	}

	CL_DrawCenterPrint ();
	//xW->glFinish();
}

int TriSpriteTexture( model_t *pSpriteModel, int frame )
{
	int	gl_texturenum;
	msprite_t	*psprite;

	if(( gl_texturenum = R_GetSpriteTexture( pSpriteModel, frame )) == 0 )
		return 0;

	if( gl_texturenum <= 0 || gl_texturenum > MAX_TEXTURES )
	{
		MsgDev( D_ERROR, "TriSpriteTexture: bad index %i\n", gl_texturenum );
		gl_texturenum = tr.defaultTexture;
	}

	psprite = (msprite_t	*)pSpriteModel->cache.data;
	if( psprite->texFormat == SPR_ALPHTEST )
	{
		xW->glEnable( GL_ALPHA_TEST );
		////xW->glAlphaFunc( GL_GREATER, 0.0f );
	}

	GL_Bind( GL_TEXTURE0, gl_texturenum );

	return 1;
}


void DrawSingleDecal( decal_t *pDecal, msurface_t *fa )
{
	float	*v;
	int	i, numVerts;

	v = R_DecalSetupVerts( pDecal, fa, pDecal->texture, &numVerts );
	if( !numVerts ) return;

	GL_Bind( GL_TEXTURE0, pDecal->texture );

	xW->glBegin( GL_POLYGON );

	for( i = 0; i < numVerts; i++, v += VERTEXSIZE )
	{
		xW->glTexCoord2f( v[3], v[4] );
		xW->glVertex3fv( v );
	}

	xW->glEnd();
}

void DrawSurfaceDecals( msurface_t *fa )
{
	decal_t		*p;
	cl_entity_t	*e;

	if( !fa->pdecals ) return;

	e = RI.currententity;
	ASSERT( e != NULL );

	if( e->curstate.rendermode == kRenderNormal || e->curstate.rendermode == kRenderTransAlpha )
	{
		xW->glDepthMask( GL_FALSE );
		xW->glEnable( GL_BLEND );

		if( e->curstate.rendermode == kRenderTransAlpha )
			xW->glDisable( GL_ALPHA_TEST );
	}

	if( e->curstate.rendermode == kRenderTransColor )
		xW->glEnable( GL_TEXTURE_2D );

	if( e->curstate.rendermode == kRenderTransTexture || e->curstate.rendermode == kRenderTransAdd )
		GL_Cull( GL_NONE );

	xW->glEnable( GL_POLYGON_OFFSET_FILL );
	xW->glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	if( fa->flags & SURF_TRANSPARENT && glState.stencilEnabled )
	{
		mtexinfo_t	*tex = fa->texinfo;

		for( p = fa->pdecals; p; p = p->pnext )
		{
			if( p->texture )
			{
				float *o, *v;
				int i, numVerts;
				o = R_DecalSetupVerts( p, fa, p->texture, &numVerts );

				xW->glEnable( GL_STENCIL_TEST );
				xW->glStencilFunc( GL_ALWAYS, 1, 0xFFFFFFFF );
				xW->glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );

				xW->glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
				xW->glBegin( GL_POLYGON );

				for( i = 0, v = o; i < numVerts; i++, v += VERTEXSIZE )
				{
					v[5] = ( DotProduct( v, tex->vecs[0] ) + tex->vecs[0][3] ) / tex->texture->width;
					v[6] = ( DotProduct( v, tex->vecs[1] ) + tex->vecs[1][3] ) / tex->texture->height;

					xW->glTexCoord2f( v[5], v[6] );
					xW->glVertex3fv( v );
				}

				xW->glEnd();
				xW->glStencilOp( GL_KEEP, GL_KEEP, GL_DECR );

				xW->glEnable( GL_ALPHA_TEST );
				xW->glBegin( GL_POLYGON );

				for( i = 0, v = o; i < numVerts; i++, v += VERTEXSIZE )
				{
					xW->glTexCoord2f( v[5], v[6] );
					xW->glVertex3fv( v );
				}

				xW->glEnd();
				xW->glDisable( GL_ALPHA_TEST );

				xW->glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
				xW->glStencilFunc( GL_EQUAL, 0, 0xFFFFFFFF );
				xW->glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
			}
		}
	}

	for( p = fa->pdecals; p; p = p->pnext )
	{
		if( p->texture )
		{
			gltexture_t *glt = R_GetTexture( p->texture );

			// normal HL decal with alpha-channel
			if( glt->flags & TF_HAS_ALPHA )
			{
				// draw transparent decals with GL_MODULATE
				//if( glt->fogParams[3] > DECAL_TRANSPARENT_THRESHOLD )
					//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
				//else //////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
				xW->glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			}
			else
			{
				// color decal like detail texture. Base color is 127 127 127
				//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
				xW->glBlendFunc( GL_DST_COLOR, GL_SRC_COLOR );
			}

			DrawSingleDecal( p, fa );
		}
	}

	if( fa->flags & SURF_TRANSPARENT && glState.stencilEnabled )
		xW->glDisable( GL_STENCIL_TEST );

	if( e->curstate.rendermode == kRenderNormal || e->curstate.rendermode == kRenderTransAlpha )
	{
		xW->glDepthMask( GL_TRUE );
		xW->glDisable( GL_BLEND );

		if( e->curstate.rendermode == kRenderTransAlpha )
			xW->glEnable( GL_ALPHA_TEST );
	}

	xW->glDisable( GL_POLYGON_OFFSET_FILL );

	if( e->curstate.rendermode == kRenderTransTexture || e->curstate.rendermode == kRenderTransAdd )
		GL_Cull( GL_FRONT );

	if( e->curstate.rendermode == kRenderTransColor )
		xW->glDisable( GL_TEXTURE_2D );

	// restore blendfunc here
	if( e->curstate.rendermode == kRenderTransAdd || e->curstate.rendermode == kRenderGlow )
		xW->glBlendFunc( GL_SRC_ALPHA, GL_ONE );
}

void R_StudioDrawPoints(void)
{
	int		i, j, m_skinnum;
	byte		*pvertbone;
	byte		*pnormbone;
	vec3_t		*pstudioverts;
	vec3_t		*pstudionorms;
	mstudiotexture_t	*ptexture;
	mstudiomesh_t	*pmesh;
	short		*pskinref;
	float		*av, *lv, *nv, scale;

	R_StudioSetupTextureHeader ();

	g_nNumArrayVerts = g_nNumArrayElems = 0;

	if( !m_pTextureHeader ) return;
	if( RI.currententity->curstate.renderfx == kRenderFxGlowShell )
		g_nStudioCount++;

	// safety bounding the skinnum
	m_skinnum = bound( 0, RI.currententity->curstate.skin, ( m_pTextureHeader->numskinfamilies - 1 ));
	pvertbone = ((byte *)m_pStudioHeader + m_pSubModel->vertinfoindex);
	pnormbone = ((byte *)m_pStudioHeader + m_pSubModel->norminfoindex);

	// NOTE: user can comment call StudioRemapColors and remap_info will be unavailable
	if( m_fDoRemap ) ptexture = CL_GetRemapInfoForEntity( RI.currententity )->ptexture;
	else ptexture = (mstudiotexture_t *)((byte *)m_pTextureHeader + m_pTextureHeader->textureindex);

	ASSERT( ptexture != NULL );

	pmesh = (mstudiomesh_t *)((byte *)m_pStudioHeader + m_pSubModel->meshindex);
	pstudioverts = (vec3_t *)((byte *)m_pStudioHeader + m_pSubModel->vertindex);
	pstudionorms = (vec3_t *)((byte *)m_pStudioHeader + m_pSubModel->normindex);

	pskinref = (short *)((byte *)m_pTextureHeader + m_pTextureHeader->skinindex);
	if( m_skinnum != 0 && m_skinnum < m_pTextureHeader->numskinfamilies )
		pskinref += (m_skinnum * m_pTextureHeader->numskinref);

	for( i = 0; i < m_pSubModel->numverts; i++ )
		Matrix3x4_VectorTransform( g_bonestransform[pvertbone[i]], pstudioverts[i], g_xformverts[i] );

	if( g_nForceFaceFlags & STUDIO_NF_CHROME )
	{
		scale = RI.currententity->curstate.renderamt * (1.0f / 255.0f);

		for( i = 0; i < m_pSubModel->numnorms; i++ )
			Matrix3x4_VectorRotate( g_bonestransform[pnormbone[i]], pstudionorms[i], g_xformnorms[i] );
	}

	lv = (float *)g_lightvalues;
	for( j = 0; j < m_pSubModel->nummesh; j++ )
	{
		g_nFaceFlags = ptexture[pskinref[pmesh[j].skinref]].flags;

		// fill in sortedmesh info
		g_sortedMeshes[j].mesh = &pmesh[j];
		g_sortedMeshes[j].flags = g_nFaceFlags;

		for( i = 0; i < pmesh[j].numnorms; i++, lv += 3, pstudionorms++, pnormbone++ )
		{
			R_StudioLighting( lv, *pnormbone, g_nFaceFlags, (float *)pstudionorms );

			if(( g_nFaceFlags & STUDIO_NF_CHROME ) || ( g_nForceFaceFlags & STUDIO_NF_CHROME ))
				R_StudioSetupChrome( g_chrome[(float (*)[3])lv - g_lightvalues], *pnormbone, (float *)pstudionorms );
		}
	}

	if( r_studio_sort_textures->integer )
	{
		// sort opaque and translucent for right results
		qsort( g_sortedMeshes, m_pSubModel->nummesh, sizeof( sortedmesh_t ), R_StudioMeshCompare );
	}

	for( j = 0; j < m_pSubModel->nummesh; j++ )
	{
		float	s, t, alpha;
		short	*ptricmds;

		pmesh = g_sortedMeshes[j].mesh;
		ptricmds = (short *)((byte *)m_pStudioHeader + pmesh->triindex);

		g_nFaceFlags = ptexture[pskinref[pmesh->skinref]].flags;
		s = 1.0f / (float)ptexture[pskinref[pmesh->skinref]].width;
		t = 1.0f / (float)ptexture[pskinref[pmesh->skinref]].height;

		if( g_iRenderMode != kRenderTransAdd )
			xW->glDepthMask( GL_TRUE );
		else xW->glDepthMask( GL_FALSE );

		// check bounds
		if( ptexture[pskinref[pmesh->skinref]].index < 0 || ptexture[pskinref[pmesh->skinref]].index > MAX_TEXTURES )
			ptexture[pskinref[pmesh->skinref]].index = tr.defaultTexture;

		if( g_nForceFaceFlags & STUDIO_NF_CHROME )
		{
			color24	*clr;
			clr = &RI.currententity->curstate.rendercolor;
			xW->glColor4ub( clr->r, clr->g, clr->b, 255 );
			alpha = 1.0f;
		}
		else if( g_nFaceFlags & STUDIO_NF_TRANSPARENT && R_StudioOpaque( RI.currententity ))
		{
			GL_SetRenderMode( kRenderTransAlpha );
			//xW->glAlphaFunc( GL_GREATER, 0.0f );
			alpha = 1.0f;
		}
		else if( g_nFaceFlags & STUDIO_NF_ADDITIVE )
		{
			GL_SetRenderMode( kRenderTransAdd );
			alpha = RI.currententity->curstate.renderamt * (1.0f / 255.0f);
			xW->glBlendFunc( GL_SRC_ALPHA, GL_ONE );
			xW->glDepthMask( GL_FALSE );
		}
		else if( g_nFaceFlags & STUDIO_NF_ALPHA )
		{
			GL_SetRenderMode( kRenderTransTexture );
			alpha = RI.currententity->curstate.renderamt * (1.0f / 255.0f);
			xW->glDepthMask( GL_FALSE );
		}
		else
		{
			GL_SetRenderMode( g_iRenderMode );

			if( g_iRenderMode == kRenderNormal )
			{
				//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
				alpha = 1.0f;
			}
			else alpha = RI.currententity->curstate.renderamt * (1.0f / 255.0f);
		}

		if( !( g_nForceFaceFlags & STUDIO_NF_CHROME ))
		{
			GL_Bind( GL_TEXTURE0, ptexture[pskinref[pmesh->skinref]].index );
		}

		while( i = *( ptricmds++ ))
		{
			int	vertexState = 0;
			qboolean	tri_strip;

			if( i < 0 )
			{
				xW->glBegin( GL_TRIANGLE_FAN );
				tri_strip = false;
				i = -i;
			}
			else
			{
				xW->glBegin( GL_TRIANGLE_STRIP );
				tri_strip = true;
			}

			r_stats.c_studio_polys += (i - 2);

			for( ; i > 0; i--, ptricmds += 4 )
			{
				// build in indices
				if( vertexState++ < 3 )
				{
					g_xarrayelems[g_nNumArrayElems++] = g_nNumArrayVerts;
				}
				else if( tri_strip )
				{
					// flip triangles between clockwise and counter clockwise
					if( vertexState & 1 )
					{
						// draw triangle [n-2 n-1 n]
						g_xarrayelems[g_nNumArrayElems++] = g_nNumArrayVerts - 2;
						g_xarrayelems[g_nNumArrayElems++] = g_nNumArrayVerts - 1;
						g_xarrayelems[g_nNumArrayElems++] = g_nNumArrayVerts;
					}
					else
					{
						// draw triangle [n-1 n-2 n]
						g_xarrayelems[g_nNumArrayElems++] = g_nNumArrayVerts - 1;
						g_xarrayelems[g_nNumArrayElems++] = g_nNumArrayVerts - 2;
						g_xarrayelems[g_nNumArrayElems++] = g_nNumArrayVerts;
					}
				}
				else
				{
					// draw triangle fan [0 n-1 n]
					g_xarrayelems[g_nNumArrayElems++] = g_nNumArrayVerts - ( vertexState - 1 );
					g_xarrayelems[g_nNumArrayElems++] = g_nNumArrayVerts - 1;
					g_xarrayelems[g_nNumArrayElems++] = g_nNumArrayVerts;
				}

				if( g_nFaceFlags & STUDIO_NF_CHROME || ( g_nForceFaceFlags & STUDIO_NF_CHROME ))
					xW->glTexCoord2f( g_chrome[ptricmds[1]][0] * s, g_chrome[ptricmds[1]][1] * t );
				else xW->glTexCoord2f( ptricmds[2] * s, ptricmds[3] * t );

				if(!( g_nForceFaceFlags & STUDIO_NF_CHROME ))
										{
					if( g_iRenderMode == kRenderTransAdd )
					{
						xW->glColor4f( 1.0f, 1.0f, 1.0f, alpha );
					}
					else if( g_iRenderMode == kRenderTransColor )
					{
						color24	*clr;
						clr = &RI.currententity->curstate.rendercolor;
						xW->glColor4ub( clr->r, clr->g, clr->b, (byte)(alpha * 255) );
					}
					else if( g_nFaceFlags & STUDIO_NF_FULLBRIGHT )
					{
						xW->glColor4f( 1.0f, 1.0f, 1.0f, alpha );
					}
					else
					{
						lv = (float *)g_lightvalues[ptricmds[1]];
						xW->glColor4f( lv[0], lv[1], lv[2], alpha );
					}
				}

				av = g_xformverts[ptricmds[0]];

				if( g_nForceFaceFlags & STUDIO_NF_CHROME )
				{
					vec3_t	scaled_vertex;
					nv = (float *)g_xformnorms[ptricmds[1]];
					VectorMA( av, scale, nv, scaled_vertex );
					xW->glVertex3fv( scaled_vertex );
				}
				else
				{
					xW->glVertex3f( av[0], av[1], av[2] );
					ASSERT( g_nNumArrayVerts < MAXARRAYVERTS );
					VectorCopy( av, g_xarrayverts[g_nNumArrayVerts] ); // store off vertex
					g_nNumArrayVerts++;
				}
			}
			xW->glEnd();
		}
	}

	// restore depthmask for next call StudioDrawPoints
	if( g_iRenderMode != kRenderTransAdd )
		xW->glDepthMask( GL_TRUE );
}

void R_StudioSetupSkin( mstudiotexture_t *ptexture, int index )
{
	short	*pskinref;
	int	m_skinnum;

	R_StudioSetupTextureHeader ();

	if( !m_pTextureHeader ) return;

	// NOTE: user can comment call StudioRemapColors and remap_info will be unavailable
	if( m_fDoRemap ) ptexture = CL_GetRemapInfoForEntity( RI.currententity )->ptexture;

	// safety bounding the skinnum
	m_skinnum = bound( 0, RI.currententity->curstate.skin, ( m_pTextureHeader->numskinfamilies - 1 ));
	pskinref = (short *)((byte *)m_pTextureHeader + m_pTextureHeader->skinindex);
	if( m_skinnum != 0 && m_skinnum < m_pTextureHeader->numskinfamilies )
		pskinref += (m_skinnum * m_pTextureHeader->numskinref);

	GL_Bind( GL_TEXTURE0, ptexture[pskinref[index]].index );
}

void R_DrawWaterSurfaces()
{
	int		i;
	msurface_t	*s;
	texture_t		*t;

	if( !RI.drawWorld || RI.refdef.onlyClientDraw )
		return;

	// non-transparent water is already drawed
	if( cl.refdef.movevars->wateralpha >= 1.0f )
		return;

	// go back to the world matrix
	R_LoadIdentity();

	xW->glEnable( GL_BLEND );
	xW->glDepthMask( GL_FALSE );
	xW->glDisable( GL_ALPHA_TEST );
	xW->glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	xW->glColor4f( 1.0f, 1.0f, 1.0f, cl.refdef.movevars->wateralpha );

	for( i = 0; i < cl.worldmodel->numtextures; i++ )
	{
		t = cl.worldmodel->textures[i];
		if( !t ) continue;

		s = t->texturechain;
		if( !s ) continue;

		if(!( s->flags & SURF_DRAWTURB ))
			continue;

		// set modulate mode explicitly
		GL_Bind( GL_TEXTURE0, t->gl_texturenum );

		for( ; s; s = s->texturechain )
			EmitWaterPolys( s->polys, ( s->flags & SURF_NOCULL ));

		t->texturechain = NULL;
	}

	xW->glDisable( GL_BLEND );
	xW->glDepthMask( GL_TRUE );
	xW->glDisable( GL_ALPHA_TEST );
	//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	xW->glColor4ub( 255, 255, 255, 255 );
}

void LM_UploadBlock( qboolean dynamic )
{
	int	i;

	if( dynamic )
	{
		int	height = 0;

		for( i = 0; i < BLOCK_SIZE; i++ )
		{
			if( gl_lms.allocated[i] > height )
				height = gl_lms.allocated[i];
		}

		if( host.features & ENGINE_LARGE_LIGHTMAPS )
			GL_Bind( GL_TEXTURE0, tr.dlightTexture2 );
		else GL_Bind( GL_TEXTURE0, tr.dlightTexture );

		xW->glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, BLOCK_SIZE, height, GL_RGBA, GL_UNSIGNED_BYTE, gl_lms.lightmap_buffer );
	}
	else
	{
		rgbdata_t	r_lightmap;
		char	lmName[16];

		i = gl_lms.current_lightmap_texture;

		// upload static lightmaps only during loading
		Q_memset( &r_lightmap, 0, sizeof( r_lightmap ));
		Q_snprintf( lmName, sizeof( lmName ), "*lightmap%i", i );

		r_lightmap.width = BLOCK_SIZE;
		r_lightmap.height = BLOCK_SIZE;
		r_lightmap.type = PF_RGBA_32;
		r_lightmap.size = r_lightmap.width * r_lightmap.height * 4;
		r_lightmap.flags = ( world.version == Q1BSP_VERSION ) ? 0 : IMAGE_HAS_COLOR;
		r_lightmap.buffer = gl_lms.lightmap_buffer;
		tr.lightmapTextures[i] = GL_LoadTextureInternal( lmName, &r_lightmap, (texFlags_t)TF_FONT, false );
		GL_SetTextureType( tr.lightmapTextures[i], TEX_LIGHTMAP );

		if( ++gl_lms.current_lightmap_texture == MAX_LIGHTMAPS )
			Host_Error( "AllocBlock: full\n" );
	}
}

void R_DrawSkyBox()
{
	int	i;

	if( clgame.movevars.skyangle )
	{
		// check for no sky at all
		for( i = 0; i < 6; i++ )
		{
			if( RI.skyMins[0][i] < RI.skyMaxs[0][i] && RI.skyMins[1][i] < RI.skyMaxs[1][i] )
				break;
		}

		if( i == 6 ) return; // nothing visible
	}

	RI.isSkyVisible = true;

	// don't fogging skybox (this fix old Half-Life bug)
	if( !RI.fogCustom )
		xW->glDisable( GL_FOG );
	xW->glDisable( GL_BLEND );
	xW->glDisable( GL_ALPHA_TEST );
	//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

	if( clgame.movevars.skyangle && !VectorIsNull( (float *)&clgame.movevars.skydir_x ))
	{
		matrix4x4	m;
		Matrix4x4_CreateRotate( m, clgame.movevars.skyangle, clgame.movevars.skydir_x, clgame.movevars.skydir_y, clgame.movevars.skydir_z );
		Matrix4x4_ConcatTranslate( m, -RI.vieworg[0], -RI.vieworg[1], -RI.vieworg[2] );
		Matrix4x4_ConcatTransforms( RI.modelviewMatrix, RI.worldviewMatrix, m );
		GL_LoadMatrix( RI.modelviewMatrix );
		tr.modelviewIdentity = false;
	}

	for( i = 0; i < 6; i++ )
	{
		if( RI.skyMins[0][i] >= RI.skyMaxs[0][i] || RI.skyMins[1][i] >= RI.skyMaxs[1][i] )
			continue;

		GL_Bind( GL_TEXTURE0, tr.skyboxTextures[r_skyTexOrder[i]] );

		xW->glBegin( GL_QUADS );
		MakeSkyVec( RI.skyMins[0][i], RI.skyMins[1][i], i );
		MakeSkyVec( RI.skyMins[0][i], RI.skyMaxs[1][i], i );
		MakeSkyVec( RI.skyMaxs[0][i], RI.skyMaxs[1][i], i );
		MakeSkyVec( RI.skyMaxs[0][i], RI.skyMins[1][i], i );
		xW->glEnd();
	}

	R_LoadIdentity();
}

void EmitWaterPolys( glpoly_t *polys, qboolean noCull )
{
	glpoly_t	*p;
	float	*v, nv, waveHeight;
	float	s, t, os, ot;
	int	i;

	if( noCull )
		xW->glDisable( GL_CULL_FACE );

	// set the current waveheight
	waveHeight = RI.currentWaveHeight;

	// reset fog color for nonlightmapped water
	GL_ResetFogColor();

	for( p = polys; p; p = p->next )
	{
		xW->glBegin( GL_POLYGON );

		for( i = 0, v = p->verts[0]; i < p->numverts; i++, v += VERTEXSIZE )
		{
			if( waveHeight )
			{
				nv = v[2] + waveHeight + ( waveHeight * sinf(v[0] * 0.02f + (float)cl.time)
					* sinf(v[1] * 0.02f + (float)cl.time) * sinf(v[2] * 0.02f + (float)cl.time));
				nv -= waveHeight;
			}
			else nv = v[2];

			os = v[3];
			ot = v[4];

			s = os + r_turbsin[(int)((ot * 0.125f + (float)cl.time ) * TURBSCALE) & 255];
			s *= ( 1.0f / SUBDIVIDE_SIZE );

			t = ot + r_turbsin[(int)((os * 0.125f + (float)cl.time ) * TURBSCALE) & 255];
			t *= ( 1.0f / SUBDIVIDE_SIZE );

			if( glState.activeTMU != 0 )
				GL_MultiTexCoord2f( glState.activeTMU, s, t );
			else xW->glTexCoord2f( s, t );
			xW->glVertex3f( v[0], v[1], nv );
		}
		xW->glEnd();
	}

	// restore culling
	if( noCull )
		xW->glEnable( GL_CULL_FACE );

	GL_SetupFogColorForSurfaces();
}

void EmitSkyPolys( msurface_t *fa )
{
	glpoly_t	*p;
	float	*v;
	int	i;
	float	s, t;
	vec3_t	dir;
	float	length;

	for( p = fa->polys; p; p = p->next )
	{
		xW->glBegin( GL_POLYGON );

		for( i = 0, v = p->verts[0]; i < p->numverts; i++, v += VERTEXSIZE )
		{
			VectorSubtract( v, RI.vieworg, dir );
			dir[2] *= 3.0f; // flatten the sphere

			length = VectorLength( dir );
			length = 6.0f * 63.0f / length;

			dir[0] *= length;
			dir[1] *= length;

			s = ( speedscale + dir[0] ) * (1.0f / 128.0f);
			t = ( speedscale + dir[1] ) * (1.0f / 128.0f);

			xW->glTexCoord2f( s, t );
			xW->glVertex3fv( v );
		}
		xW->glEnd ();
	}
}

void R_DrawSkyChain( msurface_t *s )
{
	msurface_t	*fa;

	GL_SetRenderMode( kRenderNormal );
	GL_Bind( GL_TEXTURE0, tr.solidskyTexture );

	speedscale = (float)cl.time * 8.0f;
	speedscale -= (int)speedscale & ~127;

	for( fa = s; fa; fa = fa->texturechain )
		EmitSkyPolys( fa );

	GL_SetRenderMode( kRenderTransTexture );
	GL_Bind( GL_TEXTURE0, tr.alphaskyTexture );

	speedscale = (float)cl.time * 16.0f;
	speedscale -= (int)speedscale & ~127;

	for( fa = s; fa; fa = fa->texturechain )
		EmitSkyPolys( fa );

	xW->glDisable( GL_BLEND );
}

void EmitSkyLayers( msurface_t *fa )
{
	GL_SetRenderMode( kRenderNormal );
	GL_Bind( GL_TEXTURE0, tr.solidskyTexture );

	speedscale = (float)cl.time * 8.0f;
	speedscale -= (int)speedscale & ~127;

	EmitSkyPolys( fa );

	GL_SetRenderMode( kRenderTransTexture );
	GL_Bind( GL_TEXTURE0, tr.alphaskyTexture );

	speedscale = (float)cl.time * 16.0f;
	speedscale -= (int)speedscale & ~127;

	EmitSkyPolys( fa );

	xW->glDisable( GL_BLEND );
}

void VGUI_EnableTexture( qboolean enable )
{
	if( enable ) xW->glEnable( GL_TEXTURE_2D );
	else xW->glDisable( GL_TEXTURE_2D );
}

void VGUI_DrawQuad( const vpoint_t *ul, const vpoint_t *lr )
{
	ASSERT( ul != NULL && lr != NULL );

	xW->glBegin( GL_QUADS );
		xW->glTexCoord2f( ul->coord[0], ul->coord[1] );
		xW->glVertex2f( ul->point[0], ul->point[1] );

		xW->glTexCoord2f( lr->coord[0], ul->coord[1] );
		xW->glVertex2f( lr->point[0], ul->point[1] );

		xW->glTexCoord2f( lr->coord[0], lr->coord[1] );
		xW->glVertex2f( lr->point[0], lr->point[1] );

		xW->glTexCoord2f( ul->coord[0], lr->coord[1] );
		xW->glVertex2f( ul->point[0], lr->point[1] );
	xW->glEnd();
}


void VGUI_UploadTextureBlock( int id, int drawX, int drawY, const byte *rgba, int blockWidth, int blockHeight )
{
	if( id <= 0 || id >= VGUI_MAX_TEXTURES || g_textures[id] == 0 || g_textures[id] == cls.fillImage )
	{
		MsgDev( D_ERROR, "VGUI_UploadTextureBlock: bad texture %i. Ignored\n", id );
		return;
	}

	xW->glTexSubImage2D( GL_TEXTURE_2D, 0, drawX, drawY, blockWidth, blockHeight, GL_RGBA, GL_UNSIGNED_BYTE, rgba );
	g_iBoundTexture = id;
}

void VGUI_SetupDrawingRect( int *pColor )
{
	xW->glEnable( GL_BLEND );
	xW->glDisable( GL_ALPHA_TEST );
	xW->glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	xW->glColor4ub( pColor[0], pColor[1], pColor[2], 255 - pColor[3] );
}

void VGUI_SetupDrawingText( int *pColor )
{
	xW->glEnable( GL_BLEND );
	xW->glEnable( GL_ALPHA_TEST );
	//xW->glAlphaFunc( GL_GREATER, 0.0f );
	xW->glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	xW->glColor4ub( pColor[0], pColor[1], pColor[2], 255 - pColor[3] );
}

void VGUI_SetupDrawingImage( int *pColor )
{
	xW->glEnable( GL_BLEND );
	xW->glEnable( GL_ALPHA_TEST );
	//xW->glAlphaFunc( GL_GREATER, 0.0f );
	xW->glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	xW->glColor4ub( pColor[0], pColor[1], pColor[2], 255 - pColor[3] );
}

void VGUI_BindTexture( int id )
{
	if( id > 0 && id < VGUI_MAX_TEXTURES && g_textures[id] )
	{
		GL_Bind( GL_TEXTURE0, g_textures[id] );
		g_iBoundTexture = id;
	}
	else
	{
		// NOTE: same as bogus index 2700 in GoldSrc
		id = g_iBoundTexture = 1;
		GL_Bind( GL_TEXTURE0, g_textures[id] );
	}
}

void SetBeamRenderMode( int rendermode )
{
	if( rendermode == kRenderTransAdd )
	{
		xW->glEnable( GL_BLEND );
		xW->glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	}
	else
		xW->glDisable( GL_BLEND );	// solid mode

	xW->glDisable( GL_ALPHA_TEST );
	//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
}

void R_DrawSpriteQuad( mspriteframe_t *frame, vec3_t org, vec3_t v_right, vec3_t v_up, float scale )
{
	vec3_t	point;

	r_stats.c_sprite_polys++;

	xW->glBegin( GL_QUADS );
		xW->glTexCoord2f( 0.0f, 1.0f );
		VectorMA( org, frame->down * scale, v_up, point );
		VectorMA( point, frame->left * scale, v_right, point );
		xW->glVertex3fv( point );
		xW->glTexCoord2f( 0.0f, 0.0f );
		VectorMA( org, frame->up * scale, v_up, point );
		VectorMA( point, frame->left * scale, v_right, point );
		xW->glVertex3fv( point );
		xW->glTexCoord2f( 1.0f, 0.0f );
		VectorMA( org, frame->up * scale, v_up, point );
		VectorMA( point, frame->right * scale, v_right, point );
		xW->glVertex3fv( point );
		xW->glTexCoord2f( 1.0f, 1.0f );
		VectorMA( org, frame->down * scale, v_up, point );
		VectorMA( point, frame->right * scale, v_right, point );
		xW->glVertex3fv( point );
	xW->glEnd();
}

void MakeSkyVec( float s, float t, int axis )
{
	int	j, k, farclip;
	vec3_t	v, b;

	farclip = (int)RI.farClip;

	b[0] = s * (farclip >> 1);
	b[1] = t * (farclip >> 1);
	b[2] = (float)(farclip >> 1);

	for( j = 0; j < 3; j++ )
	{
		k = st_to_vec[axis][j];
		v[j] = (k < 0) ? -b[-k-1] : b[k-1];
		v[j] += RI.cullorigin[j];
	}

	// avoid bilerp seam
	s = (s + 1.0f) * 0.5f;
	t = (t + 1.0f) * 0.5f;

	if( s < 1.0f / 512.0f )
		s = 1.0f / 512.0f;
	else if( s > 511.0f / 512.0f )
		s = 511.0f / 512.0f;
	if( t < 1.0f / 512.0f )
		t = 1.0f / 512.0f;
	else if( t > 511.0f / 512.0f )
		t = 511.0f / 512.0f;

	t = 1.0f - t;

	xW->glTexCoord2f( s, t );
	xW->glVertex3fv( v );
}


void SPR_DrawGeneric( int frame, float x, float y, float width, float height, const wrect_t *prc )
{
	float	s1, s2, t1, t2;
	int	texnum;

	if( width == -1 && height == -1 )
	{
		int	w, h;

		// assume we get sizes from image
		R_GetSpriteParms( &w, &h, NULL, frame, clgame.ds.pSprite );

		width = (float)w;
		height = (float)h;
	}

	if( prc )
	{
		wrect_t	rc;

		rc = *prc;

		// Sigh! some stupid modmakers set wrong rectangles in hud.txt
		if( rc.left <= 0 || rc.left >= width ) rc.left = 0;
		if( rc.top <= 0 || rc.top >= height ) rc.top = 0;
		if( rc.right <= 0 || rc.right > width ) rc.right = (int)width;
		if( rc.bottom <= 0 || rc.bottom > height ) rc.bottom = (int)height;

		// calc user-defined rectangle
		s1 = (float)rc.left / width;
		t1 = (float)rc.top / height;
		s2 = (float)rc.right / width;
		t2 = (float)rc.bottom / height;
		width = (float)(rc.right - rc.left);
		height = (float)(rc.bottom - rc.top);
	}
	else
	{
		s1 = t1 = 0.0f;
		s2 = t2 = 1.0f;
	}

	// pass scissor test if supposed
	if( clgame.ds.scissor_test && !SPR_Scissor( &x, &y, &width, &height, &s1, &t1, &s2, &t2 ))
		return;

	// scale for screen sizes
	SPR_AdjustSize( &x, &y, &width, &height );
	texnum = R_GetSpriteTexture( clgame.ds.pSprite, frame );
	xW->glColor4ubv( clgame.ds.spriteColor );
	R_DrawStretchPic( x, y, width, height, s1, t1, s2, t2, texnum );
}

void CL_DrawScreenFade( void )
{
	screenfade_t	*sf = &clgame.fade;
	int		iFadeAlpha, testFlags;

	// keep pushing reset time out indefinitely
	if( sf->fadeFlags & FFADE_STAYOUT )
		sf->fadeReset = (float)cl.time + 0.1f;

	if( sf->fadeReset == 0.0f && sf->fadeEnd == 0.0f )
		return;	// inactive

	// all done?
	if(( cl.time > sf->fadeReset ) && ( cl.time > sf->fadeEnd ))
	{
		Q_memset( &clgame.fade, 0, sizeof( clgame.fade ));
		return;
	}

	testFlags = (sf->fadeFlags & ~FFADE_MODULATE);

	// fading...
	if( testFlags == FFADE_STAYOUT )
	{
		iFadeAlpha = sf->fadealpha;
	}
	else
	{
		iFadeAlpha = (int)(sf->fadeSpeed * ( sf->fadeEnd - (float)cl.time ));
		if( sf->fadeFlags & FFADE_OUT ) iFadeAlpha += sf->fadealpha;
		iFadeAlpha = bound( 0, iFadeAlpha, sf->fadealpha );
	}

	xW->glColor4ub( sf->fader, sf->fadeg, sf->fadeb, iFadeAlpha );

	if( sf->fadeFlags & FFADE_MODULATE )
		GL_SetRenderMode( kRenderTransAdd );
	else GL_SetRenderMode( kRenderTransTexture );
	R_DrawStretchPic( 0.f, 0.f, (float)scr_width->integer, (float)scr_height->integer, 0.f, 0.f, 1.f, 1.f, cls.fillImage );
	xW->glColor4ub( 255, 255, 255, 255 );
}
void CL_DrawLoading( float percent )
{
	int	x, y, width, height, right;
	float	xscale, yscale, step, s2;

	R_GetTextureParms( &width, &height, cls.loadingBar );
	x = ( clgame.scrInfo.iWidth - width ) >> 1;
	y = ( clgame.scrInfo.iHeight - height) >> 1;

	xscale = (float)scr_width->integer / (float)clgame.scrInfo.iWidth;
	yscale = (float)scr_height->integer / (float)clgame.scrInfo.iHeight;

	x = (int)(x*xscale);
	y = (int)(y*yscale);
	width = (int)(width*xscale);
	height = (int)(height*yscale);

	if( cl_allow_levelshots->integer )
		  {
		xW->glColor4ub( 128, 128, 128, 255 );
		GL_SetRenderMode( kRenderTransTexture );
		R_DrawStretchPic( (float)x, (float)y, (float)width, (float)height, 0.f, 0.f, 1.f, 1.f, cls.loadingBar );

		step = (float)width / 100.0f;
		right = (int)ceil( percent * step );
		s2 = (float)right / width;
		width = right;

		xW->glColor4ub( 208, 152, 0, 255 );
		GL_SetRenderMode( kRenderTransTexture );
		R_DrawStretchPic( (float)x, (float)y, (float)width, (float)height, 0.f, 0.f, s2, 1.f, cls.loadingBar );
		xW->glColor4ub( 255, 255, 255, 255 );
	}
	else
	{
		xW->glColor4ub( 255, 255, 255, 255 );
		GL_SetRenderMode( kRenderTransTexture );
		R_DrawStretchPic( (float)x, (float)y, (float)width, (float)height, 0.f, 0.f, 1.f, 1.f, cls.loadingBar );
	}
}
void CL_DrawPause( void )
{
	int	x, y, width, height;
	float	xscale, yscale;

	R_GetTextureParms( &width, &height, cls.pauseIcon );
	x = ( clgame.scrInfo.iWidth - width ) >> 1;
	y = ( clgame.scrInfo.iHeight - height) >> 1;

	xscale = scr_width->integer / (float)clgame.scrInfo.iWidth;
	yscale = scr_height->integer / (float)clgame.scrInfo.iHeight;

	x = (int)(x*xscale);
	y = (int)(y*yscale);
	width = (int)(width*xscale);
	height = (int)(height*yscale);

	xW->glColor4ub( 255, 255, 255, 255 );
	GL_SetRenderMode( kRenderTransTexture );
	R_DrawStretchPic( (float)x, (float)y, (float)width, (float)height, 0.f, 0.f, 1.f, 1.f, cls.pauseIcon );
}
void pfnSPR_Set( VHSPRITE hPic, int r, int g, int b )
{
	clgame.ds.pSprite = CL_GetSpritePointer( hPic );
	clgame.ds.spriteColor[0] = bound( 0, r, 255 );
	clgame.ds.spriteColor[1] = bound( 0, g, 255 );
	clgame.ds.spriteColor[2] = bound( 0, b, 255 );
	clgame.ds.spriteColor[3] = 255;

	// set default state
	xW->glDisable( GL_BLEND );
	xW->glDisable( GL_ALPHA_TEST );
	////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
}

/*
=========
pfnSPR_Draw

=========
*/
void pfnSPR_Draw( int frame, int x, int y, const wrect_t *prc )
{
	xW->glEnable( GL_ALPHA_TEST );
	SPR_DrawGeneric( frame, (float)x, (float)y, -1.f, -1.f, prc );
}
void pfnFillRGBA( int x, int y, int width, int height, int r, int g, int b, int a )
{
	r = bound( 0, r, 255 );
	g = bound( 0, g, 255 );
	b = bound( 0, b, 255 );
	a = bound( 0, a, 255 );
	xW->glColor4ub( r, g, b, a );

	SPR_AdjustSize( (float *)&x, (float *)&y, (float *)&width, (float *)&height );

	GL_SetRenderMode( kRenderTransAdd );
	R_DrawStretchPic( (float)x, (float)y, (float)width, (float)height, 0.f, 0.f, 1.f, 1.f, cls.fillImage );
	xW->glColor4ub( 255, 255, 255, 255 );
}
void pfnSPR_DrawGeneric( int frame, int x, int y, const wrect_t *prc, int blendsrc, int blenddst, int width, int height )
{
	xW->glEnable( GL_BLEND );
	xW->glBlendFunc( blendsrc, blenddst ); // g-cont. are params is valid?
	SPR_DrawGeneric( frame, (float)x, (float)y, (float)width, (float)height, prc );
}
void pfnFillRGBABlend( int x, int y, int width, int height, int r, int g, int b, int a )
{
	r = bound( 0, r, 255 );
	g = bound( 0, g, 255 );
	b = bound( 0, b, 255 );
	a = bound( 0, a, 255 );
	xW->glColor4ub( r, g, b, a );

	SPR_AdjustSize( (float *)&x, (float *)&y, (float *)&width, (float *)&height );

	xW->glEnable( GL_BLEND );
	xW->glDisable( GL_ALPHA_TEST );
	xW->glBlendFunc( GL_ONE_MINUS_SRC_ALPHA, GL_ONE );
	////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	R_DrawStretchPic( (float)x, (float)y, (float)width, (float)height, 0.f, 0.f, 1.f, 1.f, cls.fillImage );
	xW->glColor4ub( 255, 255, 255, 255 );
}
void TriRenderMode( int mode )
{
	switch( mode )
	{
	case kRenderNormal:
	default:
		xW->glDisable( GL_BLEND );
		xW->glDisable( GL_ALPHA_TEST );
		////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		break;
	case kRenderTransColor:
	case kRenderTransAlpha:
	case kRenderTransTexture:
		// NOTE: TriAPI doesn't have 'solid' mode
		xW->glEnable( GL_BLEND );
		xW->glDisable( GL_ALPHA_TEST );
		xW->glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		break;
	case kRenderGlow:
	case kRenderTransAdd:
		xW->glEnable( GL_BLEND );
		xW->glDisable( GL_ALPHA_TEST );
		xW->glBlendFunc( GL_SRC_ALPHA, GL_ONE );
		////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		break;
	}
}

/*
=============
TriBegin

begin triangle sequence
=============
*/
void TriBegin( int mode )
{
	switch( mode )
	{
	case TRI_POINTS:
		mode = GL_POINTS;
		break;
	case TRI_TRIANGLES:
		mode = GL_TRIANGLES;
		break;
	case TRI_TRIANGLE_FAN:
		mode = GL_TRIANGLE_FAN;
		break;
	case TRI_QUADS:
		mode = GL_QUADS;
		break;
	case TRI_LINES:
		mode = GL_LINES;
		break;
	case TRI_TRIANGLE_STRIP:
		mode = GL_TRIANGLE_STRIP;
		break;
	case TRI_QUAD_STRIP:
		mode = GL_QUAD_STRIP;
		break;
	case TRI_POLYGON:
	default:	mode = GL_POLYGON;
		break;
	}

	xW->glBegin( mode );
}

/*
=============
TriEnd

draw triangle sequence
=============
*/
void TriEnd( void )
{
	xW->glEnd();
	xW->glDisable( GL_ALPHA_TEST );
}

/*
=============
TriColor4f

=============
*/
void TriColor4f( float r, float g, float b, float a )
{
	clgame.ds.triColor[0] = (byte)bound( 0, (r * 255.0f), 255 );
	clgame.ds.triColor[1] = (byte)bound( 0, (g * 255.0f), 255 );
	clgame.ds.triColor[2] = (byte)bound( 0, (b * 255.0f), 255 );
	clgame.ds.triColor[3] = (byte)bound( 0, (a * 255.0f), 255 );
	xW->glColor4ub( clgame.ds.triColor[0], clgame.ds.triColor[1], clgame.ds.triColor[2], clgame.ds.triColor[3] );
}

/*
=============
TriColor4ub

=============
*/
void TriColor4ub( byte r, byte g, byte b, byte a )
{
	clgame.ds.triColor[0] = r;
	clgame.ds.triColor[1] = g;
	clgame.ds.triColor[2] = b;
	clgame.ds.triColor[3] = a;
	xW->glColor4ub( r, g, b, a );
}

/*
=============
TriTexCoord2f

=============
*/
void TriTexCoord2f( float u, float v )
{
	xW->glTexCoord2f( u, v );
}

/*
=============
TriVertex3fv

=============
*/
void TriVertex3fv( const float *v )
{
	xW->glVertex3fv( v );
}

/*
=============
TriVertex3f

=============
*/
void TriVertex3f( float x, float y, float z )
{
	xW->glVertex3f( x, y, z );
}

/*
=============
TriBrightness

=============
*/
void TriBrightness( float brightness )
{
	rgba_t	rgba;

	brightness = max( 0.0f, brightness );
	rgba[0] = (byte)(clgame.ds.triColor[0] * brightness);
	rgba[1] = (byte)(clgame.ds.triColor[1] * brightness);
	rgba[2] = (byte)(clgame.ds.triColor[2] * brightness);

	xW->glColor3ub( rgba[0], rgba[1], rgba[2] );
}
void TriFog( float flFogColor[3], float flStart, float flEnd, int bOn )
{
	if( RI.fogEnabled ) return;
	RI.fogCustom = true;

	if( !bOn )
	{
		xW->glDisable( GL_FOG );
		RI.fogCustom = false;
		return;
	}

	// copy fog params
	RI.fogColor[0] = flFogColor[0] / 255.0f;
	RI.fogColor[1] = flFogColor[1] / 255.0f;
	RI.fogColor[2] = flFogColor[2] / 255.0f;
	RI.fogStart = flStart;
	RI.fogDensity = 0.0f;
	RI.fogEnd = flEnd;

	if( VectorIsNull( RI.fogColor ))
	{
		xW->glDisable( GL_FOG );
		return;
	}

	xW->glEnable( GL_FOG );
	////xW->glFogi( GL_FOG_MODE, GL_LINEAR );
	////xW->glFogf( GL_FOG_START, RI.fogStart );
	////xW->glFogf( GL_FOG_END, RI.fogEnd );
	////xW->glFogfv( GL_FOG_COLOR, RI.fogColor );
	xW->glHint( GL_FOG_HINT, GL_NICEST );
}

/*
=============
TriGetMatrix

very strange export
=============
*/
void TriGetMatrix( const int pname, float *matrix )
{
	xW->glGetFloatv( pname, matrix );
}
void TriColor4fRendermode( float r, float g, float b, float a, int rendermode )
{
	if( rendermode == kRenderTransAlpha )
		xW->glColor4f( r, g, b, a );
	else xW->glColor4f( r * a, g * a, b * a, 1.0f );
}
void PIC_DrawGeneric( float x, float y, float width, float height, const wrect_t *prc )
{
	float	s1, s2, t1, t2;
	int	w, h;

	// assume we get sizes from image
	R_GetTextureParms( &w, &h, menu.ds.gl_texturenum );

	if( prc )
	{
		// calc user-defined rectangle
		s1 = (float)prc->left / (float)w;
		t1 = (float)prc->top / (float)h;
		s2 = (float)prc->right / (float)w;
		t2 = (float)prc->bottom / (float)h;

		if( width == -1 && height == -1 )
		{
			width = (float)(prc->right - prc->left);
			height = (float)(prc->bottom - prc->top);
		}
	}
	else
	{
		s1 = t1 = 0.0f;
		s2 = t2 = 1.0f;
	}

	if( width == -1 && height == -1 )
	{
		width = (float)w;
		height = (float)h;
	}

	// pass scissor test if supposed
	if( menu.ds.scissor_test && !PIC_Scissor( &x, &y, &width, &height, &s1, &t1, &s2, &t2 ))
		return;

	PicAdjustSize( &x, &y, &width, &height );
	R_DrawStretchPic( x, y, width, height, s1, t1, s2, t2, menu.ds.gl_texturenum );
	xW->glColor4ub( 255, 255, 255, 255 );
}
void pfnPIC_Set( HIMAGE hPic, int r, int g, int b, int a )
{
	menu.ds.gl_texturenum = hPic;
	r = bound( 0, r, 255 );
	g = bound( 0, g, 255 );
	b = bound( 0, b, 255 );
	a = bound( 0, a, 255 );
	xW->glColor4ub( r, g, b, a );
}
void pfnFillRGBA_menu( int x, int y, int width, int height, int r, int g, int b, int a )
{
	r = bound( 0, r, 255 );
	g = bound( 0, g, 255 );
	b = bound( 0, b, 255 );
	a = bound( 0, a, 255 );
	xW->glColor4ub( r, g, b, a );
	GL_SetRenderMode( kRenderTransTexture );
	R_DrawStretchPic( (float)x, (float)y, (float)width, (float)height, 0.f, 0.f, 1.f, 1.f, cls.fillImage );
	xW->glColor4ub( 255, 255, 255, 255 );
}
void pfnDrawCharacterQ( int ix, int iy, int iwidth, int iheight, int ch, int ulRGBA, HIMAGE hFont )
{
	rgba_t	color;
	float	row, col, size;
	float	s1, t1, s2, t2;
	float	x = (float)ix, y = (float)iy;
	float	width = (float)iwidth;
	float	height = (float)iheight;

	ch &= 255;

	if( ch == ' ' ) return;
	if( y < -height ) return;

	color[3] = (ulRGBA & 0xFF000000) >> 24;
	color[0] = (ulRGBA & 0xFF0000) >> 16;
	color[1] = (ulRGBA & 0xFF00) >> 8;
	color[2] = (ulRGBA & 0xFF) >> 0;
	xW->glColor4ubv( color );

	col = (ch & 15) * 0.0625f + (0.5f / 256.0f);
	row = (ch >> 4) * 0.0625f + (0.5f / 256.0f);
	size = 0.0625f - (1.0f / 256.0f);

	s1 = col;
	t1 = row;
	s2 = s1 + size;
	t2 = t1 + size;

	// pass scissor test if supposed
	if( menu.ds.scissor_test && !PIC_Scissor( &x, &y, &width, &height, &s1, &t1, &s2, &t2 ))
		return;

	GL_SetRenderMode( kRenderTransTexture );
	R_DrawStretchPic( x, y, width, height, s1, t1, s2, t2, hFont );
	xW->glColor4ub( 255, 255, 255, 255 );
}
void GL_LoadTexMatrix( const matrix4x4 m )
{
	xW->glMatrixMode( GL_TEXTURE );
	GL_LoadMatrix( m );
	glState.texIdentityMatrix[glState.activeTMU] = false;
}

/*
=================
GL_LoadTexMatrixExt
=================
*/
void GL_LoadTexMatrixExt( const float *glmatrix )
{
	ASSERT( glmatrix != NULL );
	xW->glMatrixMode( GL_TEXTURE );
	xW->glLoadMatrixf( glmatrix );
	glState.texIdentityMatrix[glState.activeTMU] = false;
}

/*
=================
GL_LoadMatrix
=================
*/
void GL_LoadMatrix( const matrix4x4 source )
{
	GLfloat	dest[16];

	Matrix4x4_ToArrayFloatGL( source, dest );
	xW->glLoadMatrixf( dest );
}

/*
=================
GL_LoadIdentityTexMatrix
=================
*/
void GL_LoadIdentityTexMatrix( void )
{
	if( glState.texIdentityMatrix[glState.activeTMU] )
		return;

	xW->glMatrixMode( GL_TEXTURE );
	xW->glLoadIdentity();
	glState.texIdentityMatrix[glState.activeTMU] = true;
}

void GL_CleanUpTextureUnits( int last )
{
	int	i;

	for( i = glState.activeTMU; i > (last - 1); i-- )
	{
		// disable upper units
		if( glState.currentTextureTargets[i] != GL_NONE )
		{
			xW->glDisable( glState.currentTextureTargets[i] );
			glState.currentTextureTargets[i] = GL_NONE;
			glState.currentTextures[i] = -1; // unbind texture
		}

		GL_SetTexCoordArrayMode( GL_NONE );
		GL_LoadIdentityTexMatrix();
		GL_DisableAllTexGens();
		GL_SelectTexture( i - 1 );
	}
}

/*
=================
GL_MultiTexCoord2f
=================
*/
void GL_MultiTexCoord2f( GLenum texture, GLfloat s, GLfloat t )
{
	////xW->glTexCoord2f( s, t );

//	if( //xW->glMultiTexCoord2f )
//	{
//		//xW->glMultiTexCoord2f( texture + GL_TEXTURE0_ARB, s, t );
//	}
//	else if( //xW->glMTexCoord2fSGIS )
//	{
//		//xW->glMTexCoord2fSGIS( texture + GL_TEXTURE0_SGIS, s, t );
//	}
}

/*
=================
GL_TextureTarget
=================
*/
void GL_TextureTarget( uint target )
{
	if( glState.activeTMU < 0 || glState.activeTMU >= GL_MaxTextureUnits( ))
	{
		MsgDev( D_ERROR, "GL_TextureTarget: bad tmu state %i\n", glState.activeTMU );
		return;
	}

	if( glState.currentTextureTargets[glState.activeTMU] != target )
	{
		if( glState.currentTextureTargets[glState.activeTMU] != GL_NONE )
			xW->glDisable( glState.currentTextureTargets[glState.activeTMU] );
		glState.currentTextureTargets[glState.activeTMU] = target;
		if( target != GL_NONE )
			xW->glEnable( glState.currentTextureTargets[glState.activeTMU] );
	}
}

/*
=================
GL_TexGen
=================
*/
void GL_TexGen( GLenum coord, GLenum mode )
{
	int	tmu = min( glConfig.max_texture_coords, glState.activeTMU );
	int	bit, gen;

	switch( coord )
	{
	case GL_S:
		bit = 1;
		gen = GL_TEXTURE_GEN_S;
		break;
	case GL_T:
		bit = 2;
		gen = GL_TEXTURE_GEN_T;
		break;
	case GL_R:
		bit = 4;
		gen = GL_TEXTURE_GEN_R;
		break;
	case GL_Q:
		bit = 8;
		gen = GL_TEXTURE_GEN_Q;
		break;
	default: return;
	}

	if( mode )
	{
		if( !( glState.genSTEnabled[tmu] & bit ))
		{
			xW->glEnable( gen );
			glState.genSTEnabled[tmu] |= bit;
		}
		//xW->glTexGeni( coord, GL_TEXTURE_GEN_MODE, mode );
	}
	else
	{
		if( glState.genSTEnabled[tmu] & bit )
		{
			xW->glDisable( gen );
			glState.genSTEnabled[tmu] &= ~bit;
		}
	}
}

/*
=================
GL_SetTexCoordArrayMode
=================
*/
void GL_SetTexCoordArrayMode( GLenum mode )
{
	int	tmu = min( glConfig.max_texture_coords, glState.activeTMU );
	int	bit, cmode = glState.texCoordArrayMode[tmu];

	if( mode == GL_TEXTURE_COORD_ARRAY )
		bit = 1;
	else if( mode == GL_TEXTURE_CUBE_MAP_ARB )
		bit = 2;
	else bit = 0;

	if( cmode != bit )
	{
		/*if( cmode == 1 ) xW->glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		else *///if( cmode == 2 ) xW->glDisable( GL_TEXTURE_CUBE_MAP_ARB );

		/*if( bit == 1 ) //xW->glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		else *///if( bit == 2 ) xW->glEnable( GL_TEXTURE_CUBE_MAP_ARB );

		glState.texCoordArrayMode[tmu] = bit;
	}
}

qboolean VID_ScreenShot( const char *filename, int shot_type )
{
	rgbdata_t *r_shot;
	uint	flags = IMAGE_FLIP_Y;
	int	width = 0, height = 0;
	qboolean	result;

	r_shot = (rgbdata_t *)Mem_Alloc( r_temppool, sizeof( rgbdata_t ));
	r_shot->width = (glState.width + 3) & ~3;
	r_shot->height = (glState.height + 3) & ~3;
	r_shot->flags = IMAGE_HAS_COLOR;
	r_shot->type = PF_RGB_24;
	r_shot->size = r_shot->width * r_shot->height * PFDesc[r_shot->type].bpp;
	r_shot->palette = NULL;
	r_shot->buffer = (byte*)Mem_Alloc( r_temppool, r_shot->size );

	// get screen frame
	xW->glReadPixels( 0, 0, r_shot->width, r_shot->height, GL_RGB, GL_UNSIGNED_BYTE, r_shot->buffer );

	switch( shot_type )
	{
	case VID_SCREENSHOT:
		if( !gl_overview->integer )
			VID_ImageAdjustGamma( r_shot->buffer, r_shot->width, r_shot->height ); // scrshot gamma
		break;
	case VID_SNAPSHOT:
		if( !gl_overview->integer )
			VID_ImageAdjustGamma( r_shot->buffer, r_shot->width, r_shot->height ); // scrshot gamma
		FS_AllowDirectPaths( true );
		break;
	case VID_LEVELSHOT:
		flags |= IMAGE_RESAMPLE;
		if( glState.wideScreen )
		{
			height = 480;
			width = 800;
		}
		else
		{
			height = 480;
			width = 640;
		}
		break;
	case VID_MINISHOT:
		flags |= IMAGE_RESAMPLE;
		height = 200;
		width = 320;
		break;
	case VID_MAPSHOT:
		V_WriteOverviewScript();		// store overview script too
		flags |= IMAGE_RESAMPLE|IMAGE_QUANTIZE;	// GoldSrc request overviews in 8-bit format
		height = 768;
		width = 1024;
		break;
	}

	Image_Process( &r_shot, width, height, 0.0f, flags, NULL );

	// write image
	result = FS_SaveImage( filename, r_shot );
	host.write_to_clipboard = false;		// disable write to clipboard
	FS_AllowDirectPaths( false );			// always reset after store screenshot
	FS_FreeImage( r_shot );

	return result;
}

/*
=================
VID_CubemapShot
=================
*/
qboolean VID_CubemapShot( const char *base, uint size, const float *vieworg, qboolean skyshot )
{
	rgbdata_t		*r_shot, *r_side;
	byte		*temp = NULL;
	byte		*buffer = NULL;
	string		basename;
	int		i = 1, flags, result;

	if( !RI.drawWorld || !cl.worldmodel )
		return false;

	// make sure the specified size is valid
	while( i < size ) i<<=1;

	if( i != size ) return false;
	if( size > glState.width || size > glState.height )
		return false;

	// setup refdef
	RI.params |= RP_ENVVIEW;	// do not render non-bmodel entities

	// alloc space
	temp = (byte*)Mem_Alloc( r_temppool, size * size * 3 );
	buffer = (byte*)Mem_Alloc( r_temppool, size * size * 3 * 6 );
	r_shot = (rgbdata_t	*)Mem_Alloc( r_temppool, sizeof( rgbdata_t ));
	r_side = (rgbdata_t	*)Mem_Alloc( r_temppool, sizeof( rgbdata_t ));

	// use client vieworg
	if( !vieworg ) vieworg = cl.refdef.vieworg;

	for( i = 0; i < 6; i++ )
	{
		// go into 3d mode
		R_Set2DMode( false );

		if( skyshot )
		{
			R_DrawCubemapView( vieworg, r_skyBoxInfo[i].angles, size );
			flags = r_skyBoxInfo[i].flags;
		}
		else
		{
			R_DrawCubemapView( vieworg, r_envMapInfo[i].angles, size );
			flags = r_envMapInfo[i].flags;
					}

		xW->glReadPixels( 0, 0, size, size, GL_RGB, GL_UNSIGNED_BYTE, temp );
		r_side->flags = IMAGE_HAS_COLOR;
		r_side->width = r_side->height = size;
		r_side->type = PF_RGB_24;
		r_side->size = r_side->width * r_side->height * 3;
		r_side->buffer = temp;

		if( flags ) Image_Process( &r_side, 0, 0, 0.0f, flags, NULL );
		Q_memcpy( buffer + (size * size * 3 * i), r_side->buffer, size * size * 3 );
	}

	RI.params &= ~RP_ENVVIEW;

	r_shot->flags = IMAGE_HAS_COLOR;
	r_shot->flags |= (skyshot) ? IMAGE_SKYBOX : IMAGE_CUBEMAP;
	r_shot->width = size;
	r_shot->height = size;
	r_shot->type = PF_RGB_24;
	r_shot->size = r_shot->width * r_shot->height * 3 * 6;
	r_shot->palette = NULL;
	r_shot->buffer = buffer;

	// make sure what we have right extension
	Q_strncpy( basename, base, MAX_STRING );
	FS_StripExtension( basename );
	FS_DefaultExtension( basename, ".tga" );

	// write image as 6 sides
	result = FS_SaveImage( basename, r_shot );
	FS_FreeImage( r_shot );
	FS_FreeImage( r_side );

	return result;
}

void CL_DrawBeam( BEAM *pbeam )
{
	int	frame, rendermode;
	vec3_t	color, srcColor;

	// don't draw really short or inactive beams
	if(!( pbeam->flags & FBEAM_ISACTIVE ) || VectorLength( pbeam->delta ) < 0.1f )
	{
		return;
	}

	if( Mod_GetType( pbeam->modelIndex ) == mod_bad )
	{
		// don't draw beams without models
		pbeam->die = (float)cl.time;
		return;
	}

	frame = ((int)( pbeam->frame + cl.time * pbeam->frameRate ) % pbeam->frameCount );
	rendermode = ( pbeam->flags & FBEAM_SOLID ) ? kRenderNormal : kRenderTransAdd;

	// set color
	VectorSet( srcColor, pbeam->r, pbeam->g, pbeam->b );

	if( pbeam->flags & FBEAM_FADEIN )
	{
		VectorScale( srcColor, pbeam->t, color );
	}
	else if ( pbeam->flags & FBEAM_FADEOUT )
	{
		VectorScale( srcColor, ( 1.0f - pbeam->t ), color );
	}
	else
	{
		VectorCopy( srcColor, color );
	}

	if( pbeam->type == TE_BEAMFOLLOW )
	{
		cl_entity_t	*pStart;

		// HACKHACK: get brightness from head entity
		pStart = CL_GetBeamEntityByIndex( pbeam->startEntity );
		if( pStart && pStart->curstate.rendermode != kRenderNormal )
			pbeam->brightness = (float)pStart->curstate.renderamt;
	}

	VectorScale( color, ( pbeam->brightness / 255.0f ), color );
	VectorScale( color, ( 1.0f / 255.0f ), color );

	//xW->glShadeModel( GL_SMOOTH );

	switch( pbeam->type )
	{
	case TE_BEAMDISK:
		CL_DrawDisk( pbeam->modelIndex, (float)frame, rendermode, pbeam->source, pbeam->delta, pbeam->width,
			pbeam->amplitude, pbeam->freq, pbeam->speed, pbeam->segments, color );
		break;
	case TE_BEAMTORUS:
	case TE_BEAMCYLINDER:
		CL_DrawCylinder( pbeam->modelIndex, (float)frame, rendermode, pbeam->source, pbeam->delta, pbeam->width,
			pbeam->amplitude, pbeam->freq, pbeam->speed, pbeam->segments, color );
		break;
	case TE_BEAMPOINTS:
		CL_DrawSegs( pbeam->modelIndex, (float)frame, rendermode, pbeam->source, pbeam->delta, pbeam->width,
			pbeam->amplitude, pbeam->freq, pbeam->speed, pbeam->segments, pbeam->flags, color );
		break;
	case TE_BEAMFOLLOW:
		CL_DrawBeamFollow( pbeam->modelIndex, pbeam, frame, rendermode, (float)(cl.time - cl.oldtime), color );
		break;
	case TE_BEAMRING:
		CL_DrawRing( pbeam->modelIndex, (float)frame, rendermode, pbeam->source, pbeam->delta, pbeam->width,
			pbeam->amplitude, pbeam->freq, pbeam->speed, pbeam->segments, color );
		break;
	case TE_BEAMHOSE:
		CL_DrawLaser( pbeam, frame, rendermode, color, pbeam->modelIndex );
		break;
	default:
		MsgDev( D_ERROR, "CL_DrawBeam:  Unknown beam type %i\n", pbeam->type );
		break;
	}
	////xW->glShadeModel( GL_FLAT );
}

void GL_GenerateMipmaps( byte *buffer, rgbdata_t *pic, gltexture_t *tex, GLenum glTarget, GLenum inFormat, int side, qboolean subImage )
{
	int	mipLevel;
	int	dataType = GL_UNSIGNED_BYTE;
	int	w, h;

	// not needs
	if( tex->flags & TF_NOMIPMAP )
		return;

	if( GL_Support( GL_SGIS_MIPMAPS_EXT ) && !( tex->flags & ( TF_NORMALMAP|TF_ALPHACONTRAST )))
	{
		xW->glHint( GL_GENERATE_MIPMAP_HINT_SGIS, GL_NICEST );
		xW->glTexParameteri( glTarget, GL_GENERATE_MIPMAP_SGIS, GL_TRUE );
		xW->glGetError(); // clear error queue on mips generate
		return;
	}

	// screen texture?
	if( !buffer ) return;

	mipLevel = 0;
	w = tex->width;
	h = tex->height;

	// software mipmap generator
	while( w > 1 || h > 1 )
	{
		// build the mipmap
		if( tex->flags & TF_ALPHACONTRAST ) Q_memset( buffer, pic->width >> mipLevel, w * h * 4 );
		else GL_BuildMipMap( buffer, w, h, ( tex->flags & TF_NORMALMAP ));

		w = (w+1)>>1;
		h = (h+1)>>1;
		mipLevel++;

		if( subImage ) xW->glTexSubImage2D( tex->target + side, mipLevel, 0, 0, w, h, inFormat, dataType, buffer );
		else xW->glTexImage2D( tex->target + side, mipLevel, tex->format, w, h, 0, inFormat, dataType, buffer );
		if( xW->glGetError( )) break; // can't create mip levels
	}
}

GLuint makeTexture()
{
	GLuint n;
	xW->glGenTextures(1, &n);
	return n;
}

void GL_UploadTexture( rgbdata_t *pic, gltexture_t *tex, qboolean subImage, imgfilter_t *filter )
{
	byte		*buf, *data;
	const byte	*bufend;
	GLenum		outFormat, inFormat, glTarget;
	uint		i, s, numSides, offset = 0, err;
	int		texsize = 0, img_flags = 0, samples;
	GLint		dataType = GL_UNSIGNED_BYTE;

	ASSERT( pic != NULL && tex != NULL );

	tex->srcWidth = tex->width = pic->width;
	tex->srcHeight = tex->height = pic->height;
	s = tex->srcWidth * tex->srcHeight;

	tex->fogParams[0] = pic->fogParams[0];
	tex->fogParams[1] = pic->fogParams[1];
	tex->fogParams[2] = pic->fogParams[2];
	tex->fogParams[3] = pic->fogParams[3];

	// NOTE: normalmaps must be power of two or software mip generator will stop working
	GL_RoundImageDimensions( &tex->width, &tex->height, tex->flags, ( tex->flags & TF_NORMALMAP ));

	if( s&3 )
	{
		// will be resample, just tell me for debug targets
		MsgDev( D_NOTE, "GL_Upload: %s s&3 [%d x %d]\n", tex->name, tex->srcWidth, tex->srcHeight );
	}

	// copy flag about luma pixels
	if( pic->flags & IMAGE_HAS_LUMA )
		tex->flags = (texFlags_t)(tex->flags | TF_HAS_LUMA);

	// create luma texture from quake texture
	if( tex->flags & TF_MAKELUMA )
	{
		img_flags |= IMAGE_MAKE_LUMA;
		tex->flags = (texFlags_t)(tex->flags & (~TF_MAKELUMA));
	}

	if( !subImage && tex->flags & TF_KEEP_8BIT )
		tex->original = FS_CopyImage( pic ); // because current pic will be expanded to rgba

	if( !subImage && tex->flags & TF_KEEP_RGBDATA )
		tex->original = pic; // no need to copy

	// we need to expand image into RGBA buffer
	if( pic->type == PF_INDEXED_24 || pic->type == PF_INDEXED_32 )
		img_flags |= IMAGE_FORCE_RGBA;

	// processing image before uploading (force to rgba, make luma etc)
	if( pic->buffer ) Image_Process( &pic, 0, 0, 0.0f, img_flags, filter );

	if( tex->flags & TF_LUMINANCE )
	{
		if( !( tex->flags & TF_DEPTHMAP ))
		{
			GL_MakeLuminance( pic );
			tex->flags = (texFlags_t)(tex->flags & (~TF_LUMINANCE));
		}
		pic->flags &= ~IMAGE_HAS_COLOR;
	}

	samples = GL_CalcTextureSamples( pic->flags );

	if( pic->flags & IMAGE_HAS_ALPHA )
		tex->flags = (texFlags_t)(tex->flags | TF_HAS_ALPHA);

	// determine format
	inFormat = PFDesc[pic->type].glFormat;
	outFormat = GL_TextureFormat( tex, &samples );
	tex->format = outFormat;

	// determine target
	tex->target = glTarget = GL_TEXTURE_2D;
	numSides = 1;

	if( tex->flags & TF_DEPTHMAP )
	{
		inFormat = GL_DEPTH_COMPONENT;
		dataType = GL_FLOAT;
	}

	xW->glBindTexture(GL_TEXTURE_2D, tex->texid);

	buf = pic->buffer;
	bufend = pic->buffer + pic->size;
	offset = pic->width * pic->height * PFDesc[pic->type].bpp;

	// NOTE: probably this code relies when gl_compressed_textures is enabled
	texsize = tex->width * tex->height * samples;

	// determine some texTypes
	if( tex->flags & TF_NOPICMIP )
		tex->texType = TEX_NOMIP;
//	else if( tex->flags & TF_CUBEMAP )
//		tex->texType = TEX_CUBEMAP;
	else if(( tex->flags & TF_DECAL ) == TF_DECAL )
		tex->texType = TEX_DECAL;

	// uploading texture into video memory
	for( i = 0; i < numSides; i++ )
	{
		if( buf != NULL && buf >= bufend )
			Host_Error( "GL_UploadTexture: %s image buffer overflow\n", tex->name );

		// copy or resample the texture
		if(( tex->width == tex->srcWidth && tex->height == tex->srcHeight )/* || ( tex->flags & ( TF_TEXTURE_1D|TF_TEXTURE_3D ))*/)
			data = buf;
		else
			data = GL_ResampleTexture( buf, tex->srcWidth, tex->srcHeight, tex->width, tex->height, ( tex->flags & TF_NORMALMAP ));

		if(!( tex->flags & TF_NOMIPMAP ) && !( tex->flags & TF_SKYSIDE ) && !( tex->flags & TF_TEXTURE_3D ))
			data = GL_ApplyGamma( data, tex->width * tex->height, ( tex->flags & TF_NORMALMAP ));

//		xW->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		xW->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		xW->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		xW->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if( subImage )
			xW->glTexSubImage2D( tex->target, 0, 0, 0, tex->width, tex->height, inFormat, dataType, data );
		else
			xW->glTexImage2D( tex->target, 0, outFormat, tex->width, tex->height, 0, inFormat, dataType, data );

		xW->glGenerateMipmap(GL_TEXTURE_2D);
		xW->glEnable(GL_TEXTURE_2D);

		if( numSides > 1 && buf != NULL )
			buf += offset;
		tex->size += texsize;

		// catch possible errors
		err = xW->glGetError();

		if( err != GL_NO_ERROR )
			MsgDev( D_ERROR, "GL_UploadTexture: error %x while uploading %s [%s]\n", err, tex->name, GL_Target( glTarget ));
	}
}

void R_ShutdownImages( void )
{
	gltexture_t	*image;
	int		i;

	if( !glw_state.initialized ) return;

	for( i = ( MAX_TEXTURE_UNITS - 1); i >= 0; i-- )
	{
		if( i >= GL_MaxTextureUnits( ))
			continue;

		GL_SelectTexture( i );
		xW->glBindTexture( GL_TEXTURE_2D, 0 );

		//if( GL_Support( GL_TEXTURECUBEMAP_EXT ))
			xW->glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, 0 );
	}

	for( i = 0, image = r_textures; i < r_numTextures; i++, image++ )
	{
		if( !image->texnum ) continue;
		GL_FreeTexture( i );
	}

	Q_memset( tr.lightmapTextures, 0, sizeof( tr.lightmapTextures ));
	Q_memset( r_texturesHashTable, 0, sizeof( r_texturesHashTable ));
	Q_memset( r_textures, 0, sizeof( r_textures ));
	r_numTextures = 0;
}

void R_BeginDrawMirror( msurface_t *fa )
{
	matrix4x4		m1, m2, matrix;
	GLfloat		genVector[4][4];
	mextrasurf_t	*es;
	int		i;

	es = SURF_INFO( fa, RI.currentmodel );
	Matrix4x4_Copy( matrix, es->mirrormatrix );

	Matrix4x4_LoadIdentity( m1 );
	Matrix4x4_ConcatScale( m1, 0.5f );
	Matrix4x4_Concat( m2, m1, matrix );

	Matrix4x4_LoadIdentity( m1 );
	Matrix4x4_ConcatTranslate( m1, 0.5f, 0.5f, 0.5f );
	Matrix4x4_Concat( matrix, m1, m2 );

	for( i = 0; i < 4; i++ )
	{
		genVector[0][i] = i == 0 ? 1.f : 0.f;
		genVector[1][i] = i == 1 ? 1.f : 0.f;
		genVector[2][i] = i == 2 ? 1.f : 0.f;
		genVector[3][i] = i == 3 ? 1.f : 0.f;
	}

	GL_TexGen( GL_S, GL_OBJECT_LINEAR );
	GL_TexGen( GL_T, GL_OBJECT_LINEAR );
	GL_TexGen( GL_R, GL_OBJECT_LINEAR );
	GL_TexGen( GL_Q, GL_OBJECT_LINEAR );

	////xW->glTexGenfv( GL_S, GL_OBJECT_PLANE, genVector[0] );
	////xW->glTexGenfv( GL_T, GL_OBJECT_PLANE, genVector[1] );
	////xW->glTexGenfv( GL_R, GL_OBJECT_PLANE, genVector[2] );
	////xW->glTexGenfv( GL_Q, GL_OBJECT_PLANE, genVector[3] );

	GL_LoadTexMatrix( matrix );
}

/*
================
R_EndDrawMirror

Restore identity texmatrix
================
*/
void R_EndDrawMirror( void )
{
	GL_CleanUpTextureUnits( 0 );
	xW->glMatrixMode( GL_MODELVIEW );
}

void R_Clear( int bitMask )
{
	int	bits;

	if( gl_overview->integer )
		xW->glClearColor( 0.0f, 1.0f, 0.0f, 1.0f ); // green background (Valve rules)
	else xW->glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );

	bits = GL_DEPTH_BUFFER_BIT;

	if( RI.drawWorld && r_fastsky->integer )
		bits |= GL_COLOR_BUFFER_BIT;
	if( glState.stencilEnabled )
		bits |= GL_STENCIL_BUFFER_BIT;

	bits &= bitMask;

	xW->glClear( bits );

	// change ordering for overview
	if( RI.drawOrtho )
	{
		gldepthmin = 1.0f;
		gldepthmax = 0.0f;
	}
	else
	{
		gldepthmin = 0.0f;
		gldepthmax = 1.0f;
	}

	xW->glDepthFunc( GL_LEQUAL );
	//xW->glDepthRange( gldepthmin, gldepthmax );
}

void R_LoadIdentity( void )
{
	if( tr.modelviewIdentity ) return;

	Matrix4x4_LoadIdentity( RI.objectMatrix );
	Matrix4x4_Copy( RI.modelviewMatrix, RI.worldviewMatrix );

	xW->glMatrixMode( GL_MODELVIEW );
	GL_LoadMatrix( RI.modelviewMatrix );
	tr.modelviewIdentity = true;
}

/*
=============
R_RotateForEntity
=============
*/
void R_RotateForEntity( cl_entity_t *e )
{
	float	scale = 1.0f;

	if( e == clgame.entities || R_StaticEntity( e ))
	{
		R_LoadIdentity();
		return;
	}

	if( e->model->type != mod_brush && e->curstate.scale > 0.0f )
		scale = e->curstate.scale;

	Matrix4x4_CreateFromEntity( RI.objectMatrix, e->angles, e->origin, scale );
	Matrix4x4_ConcatTransforms( RI.modelviewMatrix, RI.worldviewMatrix, RI.objectMatrix );

	xW->glMatrixMode( GL_MODELVIEW );
	GL_LoadMatrix( RI.modelviewMatrix );
	tr.modelviewIdentity = false;
}

/*
=============
R_TranslateForEntity
=============
*/
void R_TranslateForEntity( cl_entity_t *e )
{
	float	scale = 1.0f;

	if( e == clgame.entities || R_StaticEntity( e ))
	{
		R_LoadIdentity();
		return;
	}

	if( e->model->type != mod_brush && e->curstate.scale > 0.0f )
		scale = e->curstate.scale;

	Matrix4x4_CreateFromEntity( RI.objectMatrix, vec3_origin, e->origin, scale );
	Matrix4x4_ConcatTransforms( RI.modelviewMatrix, RI.worldviewMatrix, RI.objectMatrix );

	xW->glMatrixMode( GL_MODELVIEW );
	GL_LoadMatrix( RI.modelviewMatrix );
	tr.modelviewIdentity = false;
}

void R_SetupGL( void )
{
	if( RI.refdef.waterlevel >= 3 )
	{
		float	f;
		f = sinf( (float)cl.time * 0.4f * ( (float)M_PI * 2.7f ));
		RI.refdef.fov_x += f;
		RI.refdef.fov_y -= f;
	}

	R_SetupModelviewMatrix( &RI.refdef, RI.worldviewMatrix );
	R_SetupProjectionMatrix( &RI.refdef, RI.projectionMatrix );
//	if( RI.params & RP_MIRRORVIEW ) RI.projectionMatrix[0][0] = -RI.projectionMatrix[0][0];

	Matrix4x4_Concat( RI.worldviewProjectionMatrix, RI.projectionMatrix, RI.worldviewMatrix );

	if( RP_NORMALPASS( ))
	{
		int	x, x2, y, y2;

		// set up viewport (main, playersetup)
		x = (int)floorf( RI.viewport[0] * glState.width / glState.width );
		x2 = (int)ceilf(( RI.viewport[0] + RI.viewport[2] ) * glState.width / glState.width );
		y = (int)floorf( glState.height - RI.viewport[1] * glState.height / glState.height );
		y2 = (int)ceilf( glState.height - ( RI.viewport[1] + RI.viewport[3] ) * glState.height / glState.height );

		xW->glViewport( x, y2, x2 - x, y - y2 );
	}
	else
	{
		// envpass, mirrorpass
		xW->glViewport( RI.viewport[0], RI.viewport[1], RI.viewport[2], RI.viewport[3] );
	}

	xW->glMatrixMode( GL_PROJECTION );
	GL_LoadMatrix( RI.projectionMatrix );

	xW->glMatrixMode( GL_MODELVIEW );
	GL_LoadMatrix( RI.worldviewMatrix );

	if( RI.params & RP_CLIPPLANE )
	{
		GLdouble	clip[4];
		mplane_t	*p = &RI.clipPlane;

		clip[0] = p->normal[0];
		clip[1] = p->normal[1];
		clip[2] = p->normal[2];
		clip[3] = -p->dist;

		//xW->glClipPlane( GL_CLIP_PLANE0, clip );
		//xW->glEnable( GL_CLIP_PLANE0 );
	}

	if( RI.params & RP_FLIPFRONTFACE )
		GL_FrontFace( !glState.frontFace );

	GL_Cull( GL_FRONT );

	xW->glDisable( GL_BLEND );
	xW->glDisable( GL_ALPHA_TEST );
	xW->glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
}

/*
=============
R_EndGL
=============
*/
void R_EndGL( void )
{
	if( RI.params & RP_FLIPFRONTFACE )
		GL_FrontFace( !glState.frontFace );

	//if( RI.params & RP_CLIPPLANE )
		//xW->glDisable( GL_CLIP_PLANE0 );
}

void R_DrawFog( void )
{
	if( !RI.fogEnabled || RI.refdef.onlyClientDraw )
		return;

	//xW->glEnable( GL_FOG );
	////xW->glFogi( GL_FOG_MODE, GL_EXP );
	////xW->glFogf( GL_FOG_DENSITY, RI.fogDensity );
	////xW->glFogfv( GL_FOG_COLOR, RI.fogColor );
	//xW->glHint( GL_FOG_HINT, GL_NICEST );
}

/*
=============
R_DrawEntitiesOnList
=============
*/
void R_DrawEntitiesOnList( void )
{
	int	i;

	glState.drawTrans = false;

	// draw the solid submodels fog
	R_DrawFog ();

	// first draw solid entities
	for( i = 0; i < tr.num_solid_entities; i++ )
	{
		if( RI.refdef.onlyClientDraw )
			break;

		RI.currententity = tr.solid_entities[i];
		RI.currentmodel = RI.currententity->model;

		ASSERT( RI.currententity != NULL );
		ASSERT( RI.currententity->model != NULL );

		switch( RI.currentmodel->type )
		{
		case mod_brush:
			R_DrawBrushModel( RI.currententity );
			break;
		case mod_studio:
			R_DrawStudioModel( RI.currententity );
			break;
		case mod_sprite:
			R_DrawSpriteModel( RI.currententity );
			break;
		default:
			break;
		}
	}

	if( !RI.refdef.onlyClientDraw )
		  {
		CL_DrawBeams( false );
	}

	if( RI.drawWorld )
		clgame.dllFuncs.pfnDrawNormalTriangles();

	// NOTE: some mods with custom renderer may generate glErrors
	// so we clear it here
	while( xW->glGetError() != GL_NO_ERROR );

	// don't fogging translucent surfaces
	if( !RI.fogCustom )
		xW->glDisable( GL_FOG );
	xW->glDepthMask( GL_FALSE );
	glState.drawTrans = true;

	// then draw translucent entities
	for( i = 0; i < tr.num_trans_entities; i++ )
	{
		if( RI.refdef.onlyClientDraw )
			break;

		RI.currententity = tr.trans_entities[i];
		RI.currentmodel = RI.currententity->model;

		ASSERT( RI.currententity != NULL );
		ASSERT( RI.currententity->model != NULL );

		switch( RI.currentmodel->type )
		{
		case mod_brush:
			R_DrawBrushModel( RI.currententity );
			break;
		case mod_studio:
			R_DrawStudioModel( RI.currententity );
			break;
		case mod_sprite:
			R_DrawSpriteModel( RI.currententity );
			break;
		default:
			break;
		}
	}

	if( RI.drawWorld )
		clgame.dllFuncs.pfnDrawTransparentTriangles ();

	if( !RI.refdef.onlyClientDraw )
	{
		CL_DrawBeams( true );
		CL_DrawParticles();
	}

	// NOTE: some mods with custom renderer may generate glErrors
	// so we clear it here
	while( xW->glGetError() != GL_NO_ERROR );

	glState.drawTrans = false;
	xW->glDepthMask( GL_TRUE );
	xW->glDisable( GL_BLEND );	// Trinity Render issues

	R_DrawViewModel();

	CL_ExtraUpdate();
}

void R_BeginFrame( qboolean clearScene )
{
	if(( gl_clear->integer || gl_overview->integer ) && clearScene && cls.state != ca_cinematic )
	{
		xW->glClear( GL_COLOR_BUFFER_BIT );
	}

	// update gamma
	if( vid_gamma->modified )
	{
//		if( glConfig.deviceSupportsGamma )
//		{
//			SCR_RebuildGammaTable();
//			//GL_UpdateGammaRamp();
//			vid_gamma->modified = false;
//		}
//		else
//		{
			BuildGammaTable( vid_gamma->value, vid_texgamma->value );
			GL_RebuildLightmaps();
		//}
	}

	R_Set2DMode( true );

	// draw buffer stuff
	//xW->glDrawBuffer( GL_BACK );

	// texturemode stuff
	// update texture parameters
	if( gl_texturemode->modified || gl_texture_anisotropy->modified || gl_texture_lodbias ->modified )
		R_SetTextureParameters();

	// swapinterval stuff
	//GL_UpdateSwapInterval();

	CL_ExtraUpdate ();
}

/*
===============
R_RenderFrame
===============
*/
void R_RenderFrame( const ref_params_t *fd, qboolean drawWorld )
{
	if( r_norefresh->integer )
		return;

	tr.realframecount++;

	if( RI.drawOrtho != gl_overview->integer )
		tr.fResetVis = true;

	// completely override rendering
	if( clgame.drawFuncs.GL_RenderFrame != NULL )
	{
		if( clgame.drawFuncs.GL_RenderFrame( fd, drawWorld ))
		{
			RI.drawWorld = drawWorld;
			tr.fResetVis = true;
			return;
		}
	}

	if( drawWorld ) r_lastRefdef = *fd;

	RI.params = RP_NONE;
	RI.farClip = 0;
	RI.clipFlags = 15;
	RI.drawWorld = drawWorld;
	RI.thirdPerson = cl.thirdperson;
	RI.drawOrtho = (RI.drawWorld) ? gl_overview->integer : 0;

	GL_BackendStartFrame();

	if( !r_lockcull->integer )
		VectorCopy( fd->vieworg, RI.cullorigin );
	VectorCopy( fd->vieworg, RI.pvsorigin );

	// setup viewport
	RI.viewport[0] = fd->viewport[0];
	RI.viewport[1] = fd->viewport[1];
	RI.viewport[2] = fd->viewport[2];
	RI.viewport[3] = fd->viewport[3];

	if( gl_finish->integer && drawWorld )
		//xW->glFinish();

	if( gl_allow_mirrors->integer )
	{
		// render mirrors
		R_FindMirrors( fd );
		R_DrawMirrors ();
	}

	R_RenderScene( fd );

	GL_BackendEndFrame();
}

void R_DrawTextureChains( void )
{
	int		i;
	msurface_t	*s;
	texture_t		*t;

	// make sure what color is reset
	xW->glColor4ub( 255, 255, 255, 255 );
	R_LoadIdentity();	// set identity matrix

	GL_SetupFogColorForSurfaces();

	// restore worldmodel
	RI.currententity = clgame.entities;
	RI.currentmodel = RI.currententity->model;

	// clip skybox surfaces
	for( s = skychain; s != NULL; s = s->texturechain )
		R_AddSkyBoxSurface( s );

	for( i = 0; i < cl.worldmodel->numtextures; i++ )
	{
		t = cl.worldmodel->textures[i];
		if( !t ) continue;

		s = t->texturechain;
		if( !s ) continue;

		if( i == tr.skytexturenum )
		{
			if( world.sky_sphere )
				R_DrawSkyChain( s );
		}
		else
		{
			if(( s->flags & SURF_DRAWTURB ) && cl.refdef.movevars->wateralpha < 1.0f )
				continue;	// draw translucent water later

			for( ; s != NULL; s = s->texturechain )
				R_RenderBrushPoly( s );
		}
		t->texturechain = NULL;
	}

	GL_ResetFogColor();
}

void R_DrawBrushModel( cl_entity_t *e )
{
	int		i, k, num_sorted;
	qboolean		need_sort = false;
	vec3_t		origin_l, oldorigin;
	vec3_t		mins, maxs;
	msurface_t	*psurf;
	model_t		*clmodel;
	qboolean		rotated;
	dlight_t		*l;

	if( !RI.drawWorld ) return;

	clmodel = e->model;
	RI.currentWaveHeight = RI.currententity->curstate.scale * 32.0f;

	// don't reflect this entity in mirrors
	if( e->curstate.effects & EF_NOREFLECT && RI.params & RP_MIRRORVIEW )
		return;

	// draw only in mirrors
	if( e->curstate.effects & EF_REFLECTONLY && !( RI.params & RP_MIRRORVIEW ))
		return;

	if( !VectorIsNull( e->angles ))
	{
		for( i = 0; i < 3; i++ )
		{
			mins[i] = e->origin[i] - clmodel->radius;
			maxs[i] = e->origin[i] + clmodel->radius;
		}
		rotated = true;
	}
	else
	{
		VectorAdd( e->origin, clmodel->mins, mins );
		VectorAdd( e->origin, clmodel->maxs, maxs );
		rotated = false;
	}

	if( R_CullBox( mins, maxs, RI.clipFlags ))
		return;

	Q_memset( gl_lms.lightmap_surfaces, 0, sizeof( gl_lms.lightmap_surfaces ));
	gl_lms.dynamic_surfaces = NULL;

	if( rotated ) R_RotateForEntity( e );
	else R_TranslateForEntity( e );

	e->visframe = tr.framecount; // visible

	if( rotated ) Matrix4x4_VectorITransform( RI.objectMatrix, RI.cullorigin, tr.modelorg );
	else VectorSubtract( RI.cullorigin, e->origin, tr.modelorg );

	// calculate dynamic lighting for bmodel
	for( k = 0, l = cl_dlights; k < MAX_DLIGHTS; k++, l++ )
	{
		if( l->die < cl.time || !l->radius )
			continue;

		VectorCopy( l->origin, oldorigin ); // save lightorigin
		Matrix4x4_VectorITransform( RI.objectMatrix, l->origin, origin_l );
		VectorCopy( origin_l, l->origin ); // move light in bmodel space
		R_MarkLights( l, 1<<k, clmodel->nodes + clmodel->hulls[0].firstclipnode );
		VectorCopy( oldorigin, l->origin ); // restore lightorigin
	}

	// setup the rendermode
	GL_SetRenderMode( e->curstate.rendermode );
	GL_SetupFogColorForSurfaces ();

	if( e->curstate.rendermode == kRenderTransTexture && r_lighting_extended->value >= 2.0f )
		xW->glBlendFunc( GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA );

	// setup the color and alpha
	switch( e->curstate.rendermode )
	{
	case kRenderTransAdd:
		if( RI.fogCustom )
			xW->glDisable( GL_FOG );
	case kRenderTransTexture:
		need_sort = true;
	case kRenderGlow:
		xW->glColor4ub( 255, 255, 255, e->curstate.renderamt );
		break;
	case kRenderTransColor:
		xW->glDisable( GL_TEXTURE_2D );
		xW->glColor4ub( e->curstate.rendercolor.r, e->curstate.rendercolor.g,
			e->curstate.rendercolor.b, e->curstate.renderamt );
		break;
	case kRenderTransAlpha:
		// NOTE: brushes can't change renderamt for 'Solid' mode
		////xW->glAlphaFunc( GL_GEQUAL, 0.5f );
	default:
		xW->glColor4ub( 255, 255, 255, 255 );
		break;
	}

	num_sorted = 0;

	psurf = &clmodel->surfaces[clmodel->firstmodelsurface];
	for( i = 0; i < clmodel->nummodelsurfaces; i++, psurf++ )
	{
		if( R_CullSurface( psurf, 0 ))
			continue;

		if( need_sort && !gl_nosort->integer )
		{
			world.draw_surfaces[num_sorted] = psurf;
			num_sorted++;
			ASSERT( world.max_surfaces >= num_sorted );
		}
		else
		{
			// render unsorted (solid)
			R_RenderBrushPoly( psurf );
		}
	}

	if( need_sort && !gl_nosort->integer )
		qsort( world.draw_surfaces, num_sorted, sizeof( msurface_t* ), R_SurfaceCompare );

	// draw sorted translucent surfaces
	for( i = 0; i < num_sorted; i++ )
		R_RenderBrushPoly( world.draw_surfaces[i] );

	if( e->curstate.rendermode == kRenderTransColor )
		xW->glEnable( GL_TEXTURE_2D );

	GL_ResetFogColor();
	R_BlendLightmaps();
	R_RenderFullbrights();
	R_RenderDetails();

	// restore fog here
	if( e->curstate.rendermode == kRenderTransAdd )
	{
		if( RI.fogCustom )
			xW->glEnable( GL_FOG );
	}

	R_LoadIdentity();	// restore worldmatrix
}

void R_DrawTriangleOutlines( void )
{
	int		i, j;
	msurface_t	*surf;
	glpoly_t		*p;
	float		*v;

	if( !gl_wireframe->integer )
		return;

	xW->glDisable( GL_TEXTURE_2D );
	xW->glDisable( GL_DEPTH_TEST );
	xW->glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	//xW->glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	// render static surfaces first
	for( i = 0; i < MAX_LIGHTMAPS; i++ )
	{
		for( surf = gl_lms.lightmap_surfaces[i]; surf != NULL; surf = surf->lightmapchain )
		{
			p = surf->polys;
			for( ; p != NULL; p = p->chain )
			{
				xW->glBegin( GL_POLYGON );
				v = p->verts[0];
				for( j = 0; j < p->numverts; j++, v += VERTEXSIZE )
					xW->glVertex3fv( v );
				xW->glEnd ();
			}
		}
	}

	// render surfaces with dynamic lightmaps
	for( surf = gl_lms.dynamic_surfaces; surf != NULL; surf = surf->lightmapchain )
	{
		p = surf->polys;

		for( ; p != NULL; p = p->chain )
		{
			xW->glBegin( GL_POLYGON );
			v = p->verts[0];
			for( j = 0; j < p->numverts; j++, v += VERTEXSIZE )
				xW->glVertex3fv( v );
			xW->glEnd ();
		}
	}

	//xW->glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	xW->glEnable( GL_DEPTH_TEST );
	xW->glEnable( GL_TEXTURE_2D );
}

void R_StudioRenderShadow( int iSprite, float *p1, float *p2, float *p3, float *p4 )
{
	if( !p1 || !p2 || !p3 || !p4 )
		return;

	if( TriSpriteTexture( Mod_Handle( iSprite ), 0 ))
	{
		xW->glEnable( GL_BLEND );
		xW->glDisable( GL_ALPHA_TEST );
		xW->glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		xW->glColor4f( 0.0f, 0.0f, 0.0f, 1.0f ); // render only alpha

		xW->glBegin( GL_QUADS );
			xW->glTexCoord2f( 0.0f, 0.0f );
			xW->glVertex3fv( p1 );
			xW->glTexCoord2f( 0.0f, 1.0f );
			xW->glVertex3fv( p2 );
			xW->glTexCoord2f( 1.0f, 1.0f );
			xW->glVertex3fv( p3 );
			xW->glTexCoord2f( 1.0f, 0.0f );
			xW->glVertex3fv( p4 );
		xW->glEnd();

		xW->glDisable( GL_BLEND );
		xW->glDisable( GL_ALPHA_TEST );
	}
}

void R_StudioDrawHulls( void )
{
	int	i, j;
	float	alpha;

	if( r_drawentities->integer == 4 )
		alpha = 0.5f;
	else alpha = 1.0f;

	xW->glDisable( GL_TEXTURE_2D );

	for( i = 0; i < m_pStudioHeader->numhitboxes; i++ )
	{
		mstudiobbox_t	*pbboxes = (mstudiobbox_t *)((byte *)m_pStudioHeader + m_pStudioHeader->hitboxindex);
		vec3_t		v[8], v2[8], bbmin, bbmax;

		VectorCopy( pbboxes[i].bbmin, bbmin );
		VectorCopy( pbboxes[i].bbmax, bbmax );

		v[0][0] = bbmin[0];
		v[0][1] = bbmax[1];
		v[0][2] = bbmin[2];

		v[1][0] = bbmin[0];
		v[1][1] = bbmin[1];
		v[1][2] = bbmin[2];

		v[2][0] = bbmax[0];
		v[2][1] = bbmax[1];
		v[2][2] = bbmin[2];

		v[3][0] = bbmax[0];
		v[3][1] = bbmin[1];
		v[3][2] = bbmin[2];

		v[4][0] = bbmax[0];
		v[4][1] = bbmax[1];
		v[4][2] = bbmax[2];

		v[5][0] = bbmax[0];
		v[5][1] = bbmin[1];
		v[5][2] = bbmax[2];

		v[6][0] = bbmin[0];
		v[6][1] = bbmax[1];
		v[6][2] = bbmax[2];

		v[7][0] = bbmin[0];
		v[7][1] = bbmin[1];
		v[7][2] = bbmax[2];

		Matrix3x4_VectorTransform( g_bonestransform[pbboxes[i].bone], v[0], v2[0] );
		Matrix3x4_VectorTransform( g_bonestransform[pbboxes[i].bone], v[1], v2[1] );
		Matrix3x4_VectorTransform( g_bonestransform[pbboxes[i].bone], v[2], v2[2] );
		Matrix3x4_VectorTransform( g_bonestransform[pbboxes[i].bone], v[3], v2[3] );
		Matrix3x4_VectorTransform( g_bonestransform[pbboxes[i].bone], v[4], v2[4] );
		Matrix3x4_VectorTransform( g_bonestransform[pbboxes[i].bone], v[5], v2[5] );
		Matrix3x4_VectorTransform( g_bonestransform[pbboxes[i].bone], v[6], v2[6] );
		Matrix3x4_VectorTransform( g_bonestransform[pbboxes[i].bone], v[7], v2[7] );

		j = (pbboxes[i].group % 8);

		// set properly color for hull
		xW->glColor4f( hullcolor[j][0], hullcolor[j][1], hullcolor[j][2], alpha );

		xW->glBegin( GL_QUAD_STRIP );
		for( j = 0; j < 10; j++ )
			xW->glVertex3fv( v2[j & 7] );
		xW->glEnd( );

		xW->glBegin( GL_QUAD_STRIP );
		xW->glVertex3fv( v2[6] );
		xW->glVertex3fv( v2[0] );
		xW->glVertex3fv( v2[4] );
		xW->glVertex3fv( v2[2] );
		xW->glEnd( );

		xW->glBegin( GL_QUAD_STRIP );
		xW->glVertex3fv( v2[1] );
		xW->glVertex3fv( v2[7] );
		xW->glVertex3fv( v2[3] );
		xW->glVertex3fv( v2[5] );
		xW->glEnd( );
	}

	xW->glEnable( GL_TEXTURE_2D );
}

/*
===============
R_StudioDrawAbsBBox

===============
*/
void R_StudioDrawAbsBBox( void )
{
	vec3_t	bbox[8];
	int	i;

	// looks u//xW->gly, skip
	if( RI.currententity == &clgame.viewent )
		return;

	if( !R_StudioComputeBBox( RI.currententity, bbox ))
		return;

	xW->glDisable( GL_TEXTURE_2D );
	xW->glDisable( GL_DEPTH_TEST );

	xW->glColor4f( 1.0f, 0.0f, 0.0f, 1.0f );	// red bboxes for studiomodels
	xW->glBegin( GL_LINES );

	for( i = 0; i < 2; i += 1 )
	{
		xW->glVertex3fv( bbox[i+0] );
		xW->glVertex3fv( bbox[i+2] );
		xW->glVertex3fv( bbox[i+4] );
		xW->glVertex3fv( bbox[i+6] );
		xW->glVertex3fv( bbox[i+0] );
		xW->glVertex3fv( bbox[i+4] );
		xW->glVertex3fv( bbox[i+2] );
		xW->glVertex3fv( bbox[i+6] );
		xW->glVertex3fv( bbox[i*2+0] );
		xW->glVertex3fv( bbox[i*2+1] );
		xW->glVertex3fv( bbox[i*2+4] );
		xW->glVertex3fv( bbox[i*2+5] );
	}

	xW->glEnd();
	xW->glEnable( GL_TEXTURE_2D );
	xW->glEnable( GL_DEPTH_TEST );
}

/*
===============
R_StudioDrawBones

===============
*/
void R_StudioDrawBones( void )
{
	mstudiobone_t	*pbones = (mstudiobone_t *) ((byte *)m_pStudioHeader + m_pStudioHeader->boneindex);
	vec3_t		point;
	int		i;

	xW->glDisable( GL_TEXTURE_2D );

	for( i = 0; i < m_pStudioHeader->numbones; i++ )
	{
		if( pbones[i].parent >= 0 )
		{
			//xW->glPointSize( 3.0f );
			xW->glColor3f( 1, 0.7f, 0 );
			xW->glBegin( GL_LINES );

			Matrix3x4_OriginFromMatrix( g_bonestransform[pbones[i].parent], point );
			xW->glVertex3fv( point );
			Matrix3x4_OriginFromMatrix( g_bonestransform[i], point );
			xW->glVertex3fv( point );

			xW->glEnd();

			xW->glColor4f( 0, 0, 0.8f, 1.f );
			xW->glBegin( GL_POINTS );
			if( pbones[pbones[i].parent].parent != -1 )
			{
				Matrix3x4_OriginFromMatrix( g_bonestransform[pbones[i].parent], point );
				xW->glVertex3fv( point );
			}
			Matrix3x4_OriginFromMatrix( g_bonestransform[i], point );
			xW->glVertex3fv( point );
			xW->glEnd();
		}
		else
		{
			// draw parent bone node
			//xW->glPointSize( 5.0f );
			xW->glColor4f( 0.8f, 0, 0, 1.f );
			xW->glBegin( GL_POINTS );
			Matrix3x4_OriginFromMatrix( g_bonestransform[i], point );
			xW->glVertex3fv( point );
			xW->glEnd();
		}
	}

	////xW->glPointSize( 1.0f );
	xW->glEnable( GL_TEXTURE_2D );
}

void R_StudioDrawAttachments( void )
{
	int	i;

	xW->glDisable( GL_TEXTURE_2D );
	xW->glDisable( GL_DEPTH_TEST );

	for( i = 0; i < m_pStudioHeader->numattachments; i++ )
	{
		mstudioattachment_t	*pattachments;
		vec3_t		v[4];

		pattachments = (mstudioattachment_t *)((byte *)m_pStudioHeader + m_pStudioHeader->attachmentindex);
		Matrix3x4_VectorTransform( g_bonestransform[pattachments[i].bone], pattachments[i].org, v[0] );
		Matrix3x4_VectorTransform( g_bonestransform[pattachments[i].bone], pattachments[i].vectors[0], v[1] );
		Matrix3x4_VectorTransform( g_bonestransform[pattachments[i].bone], pattachments[i].vectors[1], v[2] );
		Matrix3x4_VectorTransform( g_bonestransform[pattachments[i].bone], pattachments[i].vectors[2], v[3] );

		xW->glBegin( GL_LINES );
		xW->glColor3f( 1, 0, 0 );
		xW->glVertex3fv( v[0] );
		xW->glColor3f( 1, 1, 1 );
		xW->glVertex3fv (v[1] );
		xW->glColor3f( 1, 0, 0 );
		xW->glVertex3fv (v[0] );
		xW->glColor3f( 1, 1, 1 );
		xW->glVertex3fv (v[2] );
		xW->glColor3f( 1, 0, 0 );
		xW->glVertex3fv (v[0] );
		xW->glColor3f( 1, 1, 1 );
		xW->glVertex3fv( v[3] );
		xW->glEnd();

		//xW->glPointSize( 5.0f );
		xW->glColor3f( 0, 1, 0 );
		xW->glBegin( GL_POINTS );
		xW->glVertex3fv( v[0] );
		xW->glEnd();
		//xW->glPointSize( 1.0f );
	}

	xW->glEnable( GL_TEXTURE_2D );
	xW->glEnable( GL_DEPTH_TEST );
}

void R_StudioSetupRenderer( int rendermode )
{
	g_iRenderMode = bound( 0, rendermode, kRenderTransAdd );
	//xW->glShadeModel( GL_SMOOTH );	// enable gouraud shading
	if( clgame.ds.cullMode != GL_NONE ) GL_Cull( GL_FRONT );

	// enable depthmask on studiomodels
	if( glState.drawTrans && g_iRenderMode != kRenderTransAdd )
		xW->glDepthMask( GL_TRUE );

	////xW->glAlphaFunc( GL_GREATER, 0.0f );

	if( g_iBackFaceCull )
		GL_FrontFace( true );
}

/*
===============
R_StudioRestoreRenderer

===============
*/
void R_StudioRestoreRenderer( void )
{
	////xW->glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	////xW->glShadeModel( GL_FLAT );

	// restore depthmask state for sprites etc
	if( glState.drawTrans && g_iRenderMode != kRenderTransAdd )
		xW->glDepthMask( GL_FALSE );
	else xW->glDepthMask( GL_TRUE );

	if( g_iBackFaceCull )
		GL_FrontFace( false );

	g_iBackFaceCull = false;
	m_fDoRemap = false;
}

void R_StudioDrawPlanarShadow( void )
{
	if( RI.currententity->curstate.effects & EF_NOSHADOW )
		return;

	R_StudioDeformShadow ();

	if( glState.stencilEnabled )
		xW->glEnable( GL_STENCIL_TEST );

	//xW->glEnableClientState( GL_VERTEX_ARRAY );
	////xW->glVertexPointer( 3, GL_FLOAT, 12, g_xarrayverts );

	/*if( GL_Support( GL_DRAW_RANGEELEMENTS_EXT ))
		//xW->glDrawRangeElementsEXT( GL_TRIANGLES, 0, g_nNumArrayVerts, g_nNumArrayElems, GL_UNSIGNED_INT, g_xarrayelems );
	else *///xW->glDrawElements( GL_TRIANGLES, g_nNumArrayElems, GL_UNSIGNED_INT, g_xarrayelems );

	if( glState.stencilEnabled )
		xW->glDisable( GL_STENCIL_TEST );

	////xW->glDisableClientState( GL_VERTEX_ARRAY );
}

/*
===============
GL_StudioDrawShadow

NOTE: this code sucessfully working with ShadowHack only in Release build
===============
*/
void GL_StudioDrawShadow( void )
{
	int	rendermode;
	float	shadow_alpha;
	float	shadow_alpha2;
	GLenum	depthmode;
	GLenum	depthmode2;

	//xW->glDepthMask( GL_TRUE );

	if( r_shadows.value != 0.0f )
	{
		if( RI.currententity->baseline.movetype != MOVETYPE_FLY )
		{
			rendermode = RI.currententity->baseline.rendermode;

			if( rendermode == kRenderNormal && RI.currententity != &clgame.viewent )
			{
				shadow_alpha = 1.0f - r_shadowalpha.value * 0.5f;
				xW->glDisable( GL_TEXTURE_2D );
				xW->glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
				xW->glEnable( GL_BLEND );
				////xW->glShadeModel( GL_FLAT );
				shadow_alpha2 = 1.0f - shadow_alpha;

				xW->glColor4f( 0.0f, 0.0f, 0.0f, shadow_alpha2 );

				depthmode = GL_LESS;
				xW->glDepthFunc( depthmode );

				R_StudioDrawPlanarShadow();

				depthmode2 = GL_LEQUAL;
				xW->glDepthFunc( depthmode2 );

				xW->glEnable( GL_TEXTURE_2D );
				xW->glDisable( GL_BLEND );

				xW->glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
				////xW->glShadeModel( GL_SMOOTH );
			}
		}
	}
}

void R_DrawViewModel( void )
{
	if( RI.refdef.onlyClientDraw || r_drawviewmodel->integer == 0 )
		return;

	// ignore in thirdperson, camera view or client is died
	if( cl.thirdperson || cl.refdef.health <= 0 || cl.refdef.viewentity != ( cl.playernum + 1 ))
		return;

	if( RI.params & RP_NONVIEWERREF )
		return;

	if( !Mod_Extradata( clgame.viewent.model ))
		return;

	RI.currententity = &clgame.viewent;
	RI.currentmodel = RI.currententity->model;
	if( !RI.currentmodel ) return;

	RI.currententity->curstate.renderamt = R_ComputeFxBlend( RI.currententity );

	// hack the depth range to prevent view model from poking into walls
	////xW->glDepthRange( gldepthmin, gldepthmin + 0.3f * ( gldepthmax - gldepthmin ));

	// backface culling for left-handed weapons
	if( r_lefthand->integer == 1 || g_iBackFaceCull )
		GL_FrontFace( !glState.frontFace );

	pStudioDraw->StudioDrawModel( STUDIO_RENDER );

	// restore depth range
	////xW->glDepthRange( gldepthmin, gldepthmax );

	// backface culling for left-handed weapons
	if( r_lefthand->integer == 1 || g_iBackFaceCull )
		GL_FrontFace( !glState.frontFace );

	RI.currententity = NULL;
	RI.currentmodel = NULL;
}

void GL_SetDefaults( void )
{
	int	i;

	////xW->glFinish();

	xW->glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );

	xW->glDisable( GL_DEPTH_TEST );
	xW->glDisable( GL_CULL_FACE );
	xW->glDisable( GL_SCISSOR_TEST );
	xW->glDepthFunc( GL_LEQUAL );
	xW->glDepthMask( GL_FALSE );

	xW->glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

	if( glState.stencilEnabled )
	{
		xW->glDisable( GL_STENCIL_TEST );
		xW->glStencilMask( ( GLuint ) ~0 );
		xW->glStencilFunc( GL_EQUAL, 0, ~0 );
		xW->glStencilOp( GL_KEEP, GL_INCR, GL_INCR );
	}

	//xW->glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	xW->glPolygonOffset( -1.0f, -2.0f );

	// properly disable multitexturing at startup
	for( i = (MAX_TEXTURE_UNITS - 1); i > 0; i-- )
	{
		if( i >= GL_MaxTextureUnits( ))
			continue;

		GL_SelectTexture( i );
		//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		xW->glDisable( GL_BLEND );
		xW->glDisable( GL_TEXTURE_2D );
	}

	GL_SelectTexture( 0 );
	xW->glDisable( GL_BLEND );
	xW->glDisable( GL_ALPHA_TEST );
	xW->glDisable( GL_POLYGON_OFFSET_FILL );
	////xW->glAlphaFunc( GL_GREATER, 0.0f );
	xW->glEnable( GL_TEXTURE_2D );
	////xW->glShadeModel( GL_FLAT );

	//xW->glPointSize( 1.2f );
	xW->glLineWidth( 1.2f );

	GL_Cull( 0 );
	GL_FrontFace( 0 );

	R_SetTextureParameters();

	xW->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	xW->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	xW->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	xW->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
}

void GL_InitExtensions( void )
{
	// initialize gl extensions
	//GL_CheckExtension( "OpenGL 1.1.0", opengl_110funcs, NULL, GL_OPENGL_110 );

	// get our various GL strings
	glConfig.vendor_string = (const char*)xW->glGetString( GL_VENDOR );
	glConfig.renderer_string = (const char*)xW->glGetString( GL_RENDERER );
	glConfig.version_string = (const char*)xW->glGetString( GL_VERSION );
	glConfig.extensions_string = (const char*)xW->glGetString( GL_EXTENSIONS );
	MsgDev( D_INFO, "Video: %s\n", glConfig.renderer_string );

	// initalize until base opengl functions loaded
	//GL_CheckExtension( "WGL_EXT_swap_control", wglswapintervalfuncs, NULL, GL_WGL_SWAPCONTROL );

	//GL_CheckExtension( "glDrawRangeElements", drawrangeelementsfuncs, "gl_drawrangeelments", GL_DRAW_RANGEELEMENTS_EXT );

	//if( !GL_Support( GL_DRAW_RANGEELEMENTS_EXT ))
	//	GL_CheckExtension( "GL_EXT_draw_range_elements", drawrangeelementsextfuncs, "gl_drawrangeelments", GL_DRAW_RANGEELEMENTS_EXT );

	// multitexture
	glConfig.max_texture_units = glConfig.max_texture_coords = glConfig.max_teximage_units = 1;
	//GL_CheckExtension( "GL_ARB_multitexture", multitexturefuncs, "gl_arb_multitexture", GL_ARB_MULTITEXTURE );

	//if( GL_Support( GL_ARB_MULTITEXTURE ))
	//{
		glGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, &glConfig.max_texture_units );
		//GL_CheckExtension( "GL_ARB_texture_env_combine", NULL, "gl_texture_env_combine", GL_ENV_COMBINE_EXT );

		//if( !GL_Support( GL_ENV_COMBINE_EXT ))
		//	GL_CheckExtension( "GL_EXT_texture_env_combine", NULL, "gl_texture_env_combine", GL_ENV_COMBINE_EXT );

		//if( GL_Support( GL_ENV_COMBINE_EXT ))
		//	GL_CheckExtension( "GL_ARB_texture_env_dot3", NULL, "gl_texture_env_dot3", GL_DOT3_ARB_EXT );
	//}
//	else
//	{
//		GL_CheckExtension( "GL_SGIS_multitexture", sgis_multitexturefuncs, "gl_sgis_multitexture", GL_ARB_MULTITEXTURE );
//		if( GL_Support( GL_ARB_MULTITEXTURE )) glConfig.max_texture_units = 2;
//	}

	//if( glConfig.max_texture_units == 1 )
	//	GL_SetExtension( GL_ARB_MULTITEXTURE, false );

	// 3d texture support
	//GL_CheckExtension( "GL_EXT_texture3D", texture3dextfuncs, "gl_texture_3d", GL_TEXTURE_3D_EXT );

	//if( GL_Support( GL_TEXTURE_3D_EXT ))
	//{
		xW->glGetIntegerv( GL_MAX_3D_TEXTURE_SIZE, &glConfig.max_3d_texture_size );

		if( glConfig.max_3d_texture_size < 32 )
		{
			GL_SetExtension( GL_TEXTURE_3D_EXT, false );
			MsgDev( D_ERROR, "GL_EXT_texture3D reported bogus GL_MAX_3D_TEXTURE_SIZE, disabled\n" );
		}
	//}

	//GL_CheckExtension( "GL_SGIS_generate_mipmap", NULL, "gl_sgis_generate_mipmaps", GL_SGIS_MIPMAPS_EXT );

	// hardware cubemaps
	//GL_CheckExtension( "GL_ARB_texture_cube_map", NULL, "gl_texture_cubemap", GL_TEXTURECUBEMAP_EXT );

	//if( GL_Support( GL_TEXTURECUBEMAP_EXT ))
		xW->glGetIntegerv( GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB, &glConfig.max_cubemap_size );

	// point particles extension
	//GL_CheckExtension( "GL_EXT_point_parameters", pointparametersfunc, NULL, GL_EXT_POINTPARAMETERS );

//	GL_CheckExtension( "GL_ARB_texture_non_power_of_two", NULL, "gl_texture_npot", GL_ARB_TEXTURE_NPOT_EXT );
//	GL_CheckExtension( "GL_ARB_texture_compression", texturecompressionfuncs, "gl_dds_hardware_support", GL_TEXTURE_COMPRESSION_EXT );
//	GL_CheckExtension( "GL_EXT_compiled_vertex_array", compiledvertexarrayfuncs, "gl_cva_support", GL_CUSTOM_VERTEX_ARRAY_EXT );

	//if( !GL_Support( GL_CUSTOM_VERTEX_ARRAY_EXT ))
//		GL_CheckExtension( "GL_SGI_compiled_vertex_array", compiledvertexarrayfuncs, "gl_cva_support", GL_CUSTOM_VERTEX_ARRAY_EXT );

	//GL_CheckExtension( "GL_EXT_texture_edge_clamp", NULL, "gl_clamp_to_edge", GL_CLAMPTOEDGE_EXT );

	//if( !GL_Support( GL_CLAMPTOEDGE_EXT ))
	//	GL_CheckExtension("GL_SGIS_texture_edge_clamp", NULL, "gl_clamp_to_edge", GL_CLAMPTOEDGE_EXT );

	glConfig.max_texture_anisotropy = 0.0f;
	//GL_CheckExtension( "GL_EXT_texture_filter_anisotropic", NULL, "gl_ext_anisotropic_filter", GL_ANISOTROPY_EXT );

	//if( GL_Support( GL_ANISOTROPY_EXT ))
		xW->glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &glConfig.max_texture_anisotropy );

	//GL_CheckExtension( "GL_EXT_texture_lod_bias", NULL, "gl_ext_texture_lodbias", GL_TEXTURE_LODBIAS );
	//if( GL_Support( GL_TEXTURE_LODBIAS ))
		xW->glGetFloatv( GL_MAX_TEXTURE_LOD_BIAS_EXT, &glConfig.max_texture_lodbias );

//	GL_CheckExtension( "GL_ARB_texture_border_clamp", NULL, "gl_ext_texborder_clamp", GL_CLAMP_TEXBORDER_EXT );

//	GL_CheckExtension( "GL_EXT_blend_minmax", blendequationfuncs, "gl_ext_customblend", GL_BLEND_MINMAX_EXT );
//	GL_CheckExtension( "GL_EXT_blend_subtract", blendequationfuncs, "gl_ext_customblend", GL_BLEND_SUBTRACT_EXT );

//	GL_CheckExtension( "glStencilOpSeparate", gl2separatestencilfuncs, "gl_separate_stencil", GL_SEPARATESTENCIL_EXT );

//	if( !GL_Support( GL_SEPARATESTENCIL_EXT ))
//		GL_CheckExtension("GL_ATI_separate_stencil", atiseparatestencilfuncs, "gl_separate_stencil", GL_SEPARATESTENCIL_EXT );

//	GL_CheckExtension( "GL_EXT_stencil_two_side", stenciltwosidefuncs, "gl_stenciltwoside", GL_STENCILTWOSIDE_EXT );
//	GL_CheckExtension( "GL_ARB_vertex_buffer_object", vbofuncs, "gl_vertex_buffer_object", GL_ARB_VERTEX_BUFFER_OBJECT_EXT );

	// we don't care if it's an extension or not, they are identical functions, so keep it simple in the rendering code
	//if( pglDrawRangeElementsEXT == NULL ) pglDrawRangeElementsEXT = pglDrawRangeElements;

	//GL_CheckExtension( "GL_ARB_texture_env_add", NULL, "gl_texture_env_add", GL_TEXTURE_ENV_ADD_EXT );

//	// vp and fp shaders
//	GL_CheckExtension( "GL_ARB_shader_objects", shaderobjectsfuncs, "gl_shaderobjects", GL_SHADER_OBJECTS_EXT );
//	GL_CheckExtension( "GL_ARB_shading_language_100", NULL, "gl_glslprogram", GL_SHADER_GLSL100_EXT );
//	GL_CheckExtension( "GL_ARB_vertex_shader", vertexshaderfuncs, "gl_vertexshader", GL_VERTEX_SHADER_EXT );
//	GL_CheckExtension( "GL_ARB_fragment_shader", NULL, "gl_pixelshader", GL_FRAGMENT_SHADER_EXT );

//	GL_CheckExtension( "GL_ARB_depth_texture", NULL, "gl_depthtexture", GL_DEPTH_TEXTURE );
//	GL_CheckExtension( "GL_ARB_shadow", NULL, "gl_arb_shadow", GL_SHADOW_EXT );

//	GL_CheckExtension( "GL_ARB_texture_float", NULL, "gl_arb_texture_float", GL_ARB_TEXTURE_FLOAT_EXT );

//	// occlusion queries
//	GL_CheckExtension( "GL_ARB_occlusion_query", occlusionfunc, "gl_occlusion_queries", GL_OCCLUSION_QUERIES_EXT );

	//if( GL_Support( GL_SHADER_GLSL100_EXT ))
	//{
		xW->glGetIntegerv( GL_MAX_TEXTURE_COORDS_ARB, &glConfig.max_texture_coords );
		xW->glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &glConfig.max_teximage_units );
	//}
//	else
//	{
//		// just get from multitexturing
//		glConfig.max_texture_coords = glConfig.max_teximage_units = glConfig.max_texture_units;
//	}

	// rectangle textures support
//	if( Q_strstr( glConfig.extensions_string, "GL_NV_texture_rectangle" ))
//	{
//		glConfig.texRectangle = GL_TEXTURE_RECTANGLE_NV;
//		pglGetIntegerv( GL_MAX_RECTANGLE_TEXTURE_SIZE_NV, &glConfig.max_2d_rectangle_size );
//	}
//	else if( Q_strstr( glConfig.extensions_string, "GL_EXT_texture_rectangle" ))
//	{
/*		glConfig.texRectangle = GL_TEXTURE_RECTANGLE_EXT;
		pglGetIntegerv( GL_MAX_RECTANGLE_TEXTURE_SIZE_EXT, &glConfig.max_2d_rectangle_size );
	}
	else */glConfig.texRectangle = glConfig.max_2d_rectangle_size = 0; // no rectangle

	glConfig.max_2d_texture_size = 0;
	xW->glGetIntegerv( GL_MAX_TEXTURE_SIZE, &glConfig.max_2d_texture_size );
	if( glConfig.max_2d_texture_size <= 0 ) glConfig.max_2d_texture_size = 256;

	Cvar_Get( "gl_max_texture_size", "0", CVAR_INIT, "opengl texture max dims" );
	Cvar_Set( "gl_max_texture_size", va( "%i", glConfig.max_2d_texture_size ));

	// MCD has buffering issues
	//if(Q_strstr( glConfig.renderer_string, "gdi" ))
	//	Cvar_SetFloat( "gl_finish", 1 );

	Cvar_Set( "gl_anisotropy", va( "%f", bound( 0, gl_texture_anisotropy->value, glConfig.max_texture_anisotropy )));

	// software mipmap generator does wrong result with NPOT textures ...
	//if( !GL_Support( GL_SGIS_MIPMAPS_EXT ))
	//	GL_SetExtension( GL_ARB_TEXTURE_NPOT_EXT, false );

	glw_state.initialized = true;

	tr.framecount = tr.visframecount = 1;
}

void GL_CheckForErrors_( const char *filename, const int fileline )
{
	int	err;
	char	*str;

	if( !gl_check_errors->integer )
		return;

	if(( err = xW->glGetError( )) == GL_NO_ERROR )
		return;

	switch( err )
	{
	case GL_STACK_OVERFLOW:
		str = "GL_STACK_OVERFLOW";
		break;
	case GL_STACK_UNDERFLOW:
		str = "GL_STACK_UNDERFLOW";
		break;
	case GL_INVALID_ENUM:
		str = "GL_INVALID_ENUM";
		break;
	case GL_INVALID_VALUE:
		str = "GL_INVALID_VALUE";
		break;
	case GL_INVALID_OPERATION:
		str = "GL_INVALID_OPERATION";
		break;
	case GL_OUT_OF_MEMORY:
		str = "GL_OUT_OF_MEMORY";
		break;
	default:
		str = "UNKNOWN ERROR";
		break;
	}

	Host_Error( "GL_CheckForErrors: %s (called at %s:%i)\n", str, filename, fileline );
}


int Con_DrawGenericChar( int x, int y, int number, rgba_t color )
{
	int	width, height;
	float	s1, t1, s2, t2;
	wrect_t	*rc;

	number &= 255;

	if( !con.curFont || !con.curFont->valid )
		return 0;

	if( number < 32 ) return 0;
	if( y < -con.curFont->charHeight )
		return 0;

	rc = &con.curFont->fontRc[number];

	xW->glColor4ubv( color );
	R_GetTextureParms( &width, &height, con.curFont->hFontTexture );

	// calc rectangle
	s1 = (float)rc->left / width;
	t1 = (float)rc->top / height;
	s2 = (float)rc->right / width;
	t2 = (float)rc->bottom / height;
	width = rc->right - rc->left;
	height = rc->bottom - rc->top;

	TextAdjustSize( &x, &y, &width, &height );
	R_DrawStretchPic( (float)x, (float)y, (float)width, (float)height, s1, t1, s2, t2, con.curFont->hFontTexture );
	xW->glColor4ub( 255, 255, 255, 255 ); // don't forget reset color

	return con.curFont->charWidths[number];
}

void Field_DrawInputLine( int x, int y, field_t *edit )
{
	int	len, cursorChar;
	int	drawLen, hideChar = -1;
	int	prestep, curPos;
	char	str[MAX_SYSPATH];
	byte	*colorDefault;

	drawLen = edit->widthInChars;
	len = Q_strlen( edit->buffer ) + 1;
	colorDefault = g_color_table[ColorIndex( COLOR_DEFAULT )];

	// guarantee that cursor will be visible
	if( len <= drawLen )
	{
		prestep = 0;
	}
	else
	{
		if( edit->scroll + drawLen > len )
		{
			edit->scroll = len - drawLen;
			if( edit->scroll < 0 ) edit->scroll = 0;
		}

		prestep = edit->scroll;
	}

	if( prestep + drawLen > len )
		drawLen = len - prestep;

	// extract <drawLen> characters from the field at <prestep>
	ASSERT( drawLen < MAX_SYSPATH );

	Q_memcpy( str, edit->buffer + prestep, drawLen );
	str[drawLen] = 0;

	// save char for overstrike
	cursorChar = str[edit->cursor - prestep];

	if( host.key_overstrike && cursorChar && !((int)( host.realtime * 4 ) & 1 ))
		hideChar = edit->cursor - prestep; // skip this char

	// draw it
	Con_DrawGenericString( x, y, str, colorDefault, false, hideChar );

	// draw the cursor
	if((int)( host.realtime * 4 ) & 1 ) return; // off blink

	// calc cursor position
	str[edit->cursor - prestep] = 0;
	Con_DrawStringLen( str, &curPos, NULL );

	if( host.key_overstrike && cursorChar )
	{
		// overstrike cursor
		xW->glEnable( GL_BLEND );
		xW->glDisable( GL_ALPHA_TEST );
		xW->glBlendFunc( GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA );
		//////xW->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		Con_DrawGenericChar( x + curPos, y, cursorChar, colorDefault );
	}
	else Con_DrawCharacter( x + curPos, y, '_', colorDefault );
}

void Con_DrawNotify( void )
{
	int	i, x, v = 0;
	int	start, currentColor;
	short	*text;
	float	time;

	if( !con.curFont ) return;

	if( host.developer && ( !Cvar_VariableInteger( "cl_background" ) && !Cvar_VariableInteger( "sv_background" )))
	{
		currentColor = 7;
		xW->glColor4ubv( g_color_table[currentColor] );

		for( i = con.current - CON_TIMES + 1; i <= con.current; i++ )
		{
			if( i < 0 ) continue;
			time = (float)con.times[i % CON_TIMES];
			if( time == 0 ) continue;
			time = (float)host.realtime - time;

			if( time > con_notifytime->value )
				continue;	// expired

			text = con.text + (i % con.totallines) * con.linewidth;
			start = con.curFont->charWidths[' ']; // offset one space at left screen side

			for( x = 0; x < con.linewidth; x++ )
			{
				if((( text[x] >> 8 ) & 7 ) != currentColor )
					currentColor = ( text[x] >> 8 ) & 7;
				start += Con_DrawCharacter( start, v, text[x] & 0xFF, g_color_table[currentColor] );
			}
			v += con.curFont->charHeight;
		}
	}

	if( cls.key_dest == key_message )
	{
		string	buf;
		int	len;

		currentColor = 7;
		xW->glColor4ubv( g_color_table[currentColor] );

		start = con.curFont->charWidths[' ']; // offset one space at left screen side

		// update chatline position from client.dll
		if( clgame.dllFuncs.pfnChatInputPosition )
			clgame.dllFuncs.pfnChatInputPosition( &start, &v );

		Q_snprintf( buf, sizeof( buf ), "%s: ", con.chat_cmd );

		Con_DrawStringLen( buf, &len, NULL );
		Con_DrawString( start, v, buf, g_color_table[7] );

		Field_DrawInputLine( start + len, v, &con.chat );
	}

	xW->glColor4ub( 255, 255, 255, 255 );
}

/*
================
Con_DrawConsole

Draws the console with the solid background
================
*/
void Con_DrawSolidConsole( float frac )
{
	int	i, x, y;
	int	rows;
	short	*text;
	int	row;
	int	lines, start;
	int	currentColor;
	string	curbuild;

	lines = (int)((float)scr_height->integer * frac);
	if( lines <= 0 ) return;
	if( lines > scr_height->integer )
		lines = scr_height->integer;

	// draw the background
	y = (int)(frac * (float)scr_height->integer);

	if( y >= 1 )
	{
		GL_SetRenderMode( kRenderNormal );
		R_DrawStretchPic( 0, (float)(y - scr_height->integer), (float)scr_width->integer, (float)scr_height->integer, 0, 0, 1, 1, con.background );
	}
	else y = 0;

	if( !con.curFont ) return; // nothing to draw

	if( host.developer )
	{
		// draw current version
		byte	*color = g_color_table[7];
		int	stringLen, width = 0, charH;

		Q_snprintf( curbuild, MAX_STRING, "Xash3D %i/%g (hw build %i)", PROTOCOL_VERSION, XASH_VERSION, Q_buildnum( ));
		Con_DrawStringLen( curbuild, &stringLen, &charH );
		start = scr_width->integer - stringLen;
		stringLen = Con_StringLength( curbuild );

		for( i = 0; i < stringLen; i++ )
			width += Con_DrawCharacter( start + width, 0, curbuild[i], color );
	}

	// draw the text
	con.vislines = lines;
	rows = ( lines - QCHAR_WIDTH ) / QCHAR_WIDTH; // rows of text to draw
	y = lines - ( con.curFont->charHeight * 3 );

	// draw from the bottom up
	if( con.display != con.current )
	{
		start = con.curFont->charWidths[' ']; // offset one space at left screen side

		// draw red arrows to show the buffer is backscrolled
		for( x = 0; x < con.linewidth; x += 4 )
			Con_DrawCharacter(( x + 1 ) * start, y, '^', g_color_table[1] );
		y -= con.curFont->charHeight;
		rows--;
	}

	row = con.display;
	if( con.x == 0 ) row--;

	currentColor = 7;
	xW->glColor4ubv( g_color_table[currentColor] );

	for( i = 0; i < rows; i++, y -= con.curFont->charHeight, row-- )
	{
		if( row < 0 ) break;
		if( con.current - row >= con.totallines )
		{
			// past scrollback wrap point
			continue;
		}

		text = con.text + ( row % con.totallines ) * con.linewidth;
		start = con.curFont->charWidths[' ']; // offset one space at left screen side

		for( x = 0; x < con.linewidth; x++ )
		{
			if((( text[x] >> 8 ) & 7 ) != currentColor )
				currentColor = ( text[x] >> 8 ) & 7;
			start += Con_DrawCharacter( start, y, text[x] & 0xFF, g_color_table[currentColor] );
		}
	}

	// draw the input prompt, user text, and cursor if desired
	Con_DrawInput();
	xW->glColor4ub( 255, 255, 255, 255 );
}






