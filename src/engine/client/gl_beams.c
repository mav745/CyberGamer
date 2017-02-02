/*
gl_beams.c - beams rendering
Copyright (C) 2009 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include <qt/c_gate.h>

#include "client.h"
#include "r_efx.h"
#include "event_flags.h"
#include "entity_types.h"
#include "triangleapi.h"
#include "customentity.h"
#include "cl_tent.h"
#include "pm_local.h"
#include "studio.h"

//#define NOISE_DIVISIONS	64	// don't touch - many tripmines cause the crash when it equal 128

//typedef struct
//{
//	vec3_t	pos;
//	vec3_t	color;
//	float	texcoord;	// Y texture coordinate
//	float	width;
//	float	alpha;
//} beamseg_t;

float	rgNoise[NOISE_DIVISIONS+1];	// global noise array

/*
==============================================================

VIEWBEAMS DRAW METHODS

==============================================================
*/

// freq2 += step * 0.1;
// Fractal noise generator, power of 2 wavelength
void FracNoise( float *noise, int divs, float scale )
{
	int	div2;

	div2 = divs >> 1;
	if( divs < 2 ) return;

	// noise is normalized to +/- scale
	noise[div2] = ( noise[0] + noise[divs] ) * 0.5f + scale * Com_RandomFloat( -1.0f, 1.0f );

	if( div2 > 1 )
	{
		FracNoise( &noise[div2], div2, scale * 0.5f );
		FracNoise( noise, div2, scale * 0.5f );
	}
}

static void SineNoise( float *noise, int divs )
{
	float	freq = 0;
	float	step = M_PI / (float)divs;
	int	i;

	for( i = 0; i < divs; i++ )
	{
		noise[i] = sinf( freq );
		freq += step;
	}
}

cl_entity_t *CL_GetBeamEntityByIndex( int index )
{
	cl_entity_t	*ent;

	if( index > 0 ) index = BEAMENT_ENTITY( index );
	ent = CL_GetEntityByIndex( index );

	return ent;
}

void BeamNormalizeColor( BEAM *pBeam, float r, float g, float b, float brightness )
{
	float	max, scale;

	max = max( max( r, g ), b );

	if( max == 0 )
	{
		pBeam->r = pBeam->g = pBeam->b = 255.0f;
		pBeam->brightness = brightness;
	}

	scale = 255.0f / max;

	pBeam->r = r * scale;
	pBeam->g = g * scale;
	pBeam->b = b * scale;
	pBeam->brightness = ( brightness > 1.0f ) ? brightness : brightness * 255.0f;
}

static qboolean ComputeBeamEntPosition( int beamEnt, vec3_t pt )
{
	cl_entity_t	*pEnt;
	int		nAttachment;

	pEnt = CL_GetBeamEntityByIndex( beamEnt );
	nAttachment = ( beamEnt > 0 ) ? BEAMENT_ATTACHMENT( beamEnt ) : 0;

	if( !pEnt )
	{
		VectorClear( pt );
		return false;
	}

	if(( pEnt->index - 1 ) == cl.playernum && !cl.thirdperson )
	{
		// if we view beam at firstperson use viewmodel instead
		pEnt = &clgame.viewent;
	}

	// get attachment
	if( nAttachment > 0 )
		VectorCopy( pEnt->attachment[nAttachment - 1], pt );
	else VectorCopy( pEnt->origin, pt );

	return true;
}

void ComputeBeamPerpendicular( const vec3_t vecBeamDelta, vec3_t pPerp )
{
	// Direction in worldspace of the center of the beam
	vec3_t	vecBeamCenter;

	VectorNormalize2( vecBeamDelta, vecBeamCenter );
	CrossProduct( RI.vforward, vecBeamCenter, pPerp );
	VectorNormalize( pPerp );
}

void ComputeNormal( const vec3_t vStartPos, const vec3_t vNextPos, vec3_t pNormal )
{
	// vTangentY = line vector for beam
	vec3_t	vTangentY, vDirToBeam;

	VectorSubtract( vStartPos, vNextPos, vTangentY );

	// vDirToBeam = vector from viewer origin to beam
	VectorSubtract( vStartPos, RI.vieworg, vDirToBeam );

	// Get a vector that is perpendicular to us and perpendicular to the beam.
	// This is used to fatten the beam.
	CrossProduct( vTangentY, vDirToBeam, pNormal );
	VectorNormalizeFast( pNormal );
}

/*
==============================================================

VIEWBEAMS MANAGEMENT

==============================================================
*/
#define BEAM_TRAILS		16	// 128 beams * 16 = 2048 particles
#define MAX_SERVERBEAMS	1024	// should be enough

BEAM		*cl_active_beams;
BEAM		*cl_free_beams;
BEAM		*cl_viewbeams = NULL;		// beams pool

particle_t	*cl_free_trails;
particle_t	*cl_beamtrails = NULL;		// trail partilces

cl_entity_t	*cl_custombeams[MAX_SERVERBEAMS];	// to avoid check of all the ents
/*
================
CL_InitViewBeams

================
*/
void CL_InitViewBeams( void )
{
	cl_viewbeams = Mem_Alloc( cls.mempool, sizeof( BEAM ) * GI->max_beams );
	cl_beamtrails = Mem_Alloc( cls.mempool, sizeof( particle_t ) * GI->max_beams * BEAM_TRAILS );
	CL_ClearViewBeams();
}

/*
================
CL_ClearViewBeams

================
*/
void CL_ClearViewBeams( void )
{
	int	i;

	if( !cl_viewbeams ) return;

	// clear beams
	cl_free_beams = cl_viewbeams;
	cl_active_beams = NULL;

	for( i = 0; i < GI->max_beams - 1; i++ )
		cl_viewbeams[i].next = &cl_viewbeams[i+1];
	cl_viewbeams[GI->max_beams - 1].next = NULL;

	// also clear any particles used by beams
	cl_free_trails = cl_beamtrails;

	for( i = 0; i < ( GI->max_beams * BEAM_TRAILS ) - 1; i++ )
		cl_beamtrails[i].next = &cl_beamtrails[i+1];
	cl_beamtrails[( GI->max_beams * BEAM_TRAILS ) - 1].next = NULL;

	cl.num_custombeams = 0;	// clear custom beams
}

/*
================
CL_FreeViewBeams

================
*/
void CL_FreeViewBeams( void )
{
	if( cl_viewbeams ) Mem_Free( cl_viewbeams );
	if( cl_beamtrails ) Mem_Free( cl_beamtrails );

	cl_viewbeams = NULL;
	cl_beamtrails = NULL;
}

/*
================
CL_AddCustomBeam

Add the beam that encoded as custom entity
================
*/
void CL_AddCustomBeam( cl_entity_t *pEnvBeam )
{
	if( cl.num_custombeams >= MAX_SERVERBEAMS )
	{
		MsgDev( D_ERROR, "Too many static beams %d!\n", cl.num_custombeams );
		return;
	}

	if( pEnvBeam )
	{
		cl_custombeams[cl.num_custombeams] = pEnvBeam;
		cl.num_custombeams++;
	}
}

/*
==============
CL_FreeDeadTrails

Free dead trails associated with beam
==============
*/
void CL_FreeDeadTrails( particle_t **trail )
{
	particle_t	*kill;
	particle_t	*p;

	// kill all the ones hanging direcly off the base pointer
	while( 1 )
	{
		kill = *trail;
		if( kill && kill->die < cl.time )
		{
			*trail = kill->next;
			kill->next = cl_free_trails;
			cl_free_trails = kill;
			continue;
		}
		break;
	}

	// kill off all the others
	for( p = *trail; p; p = p->next )
	{
		while( 1 )
		{
			kill = p->next;
			if( kill && kill->die < cl.time )
			{
				p->next = kill->next;
				kill->next = cl_free_trails;
				cl_free_trails = kill;
				continue;
			}
			break;
		}
	}
}

/*
==============
CL_AllocBeam

==============
*/
BEAM *CL_AllocBeam( void )
{
	BEAM	*pBeam;

	if( !cl_free_beams )
		return NULL;

	pBeam = cl_free_beams;
	cl_free_beams = pBeam->next;

	Q_memset( pBeam, 0, sizeof( *pBeam ));

	pBeam->next = cl_active_beams;
	cl_active_beams = pBeam;

	pBeam->die = (float)cl.time;

	return pBeam;
}

/*
==============
CL_FreeBeam

==============
*/
void CL_FreeBeam( BEAM *pBeam )
{
	// free particles that have died off.
	CL_FreeDeadTrails( &pBeam->particles );

	// now link into free list;
	pBeam->next = cl_free_beams;
	cl_free_beams = pBeam;
}

/*
==============
CL_KillDeadBeams

==============
*/
void CL_KillDeadBeams( cl_entity_t *pDeadEntity )
{
	BEAM		*pbeam;
	BEAM		*pnewlist;
	BEAM		*pnext;
	particle_t	*pHead;	// build a new list to replace cl_active_beams.

	pbeam = cl_active_beams;	// old list.
	pnewlist = NULL;		// new list.

	while( pbeam )
	{
		pnext = pbeam->next;

		// link into new list.
		if( CL_GetBeamEntityByIndex( pbeam->startEntity ) != pDeadEntity )
		{
			pbeam->next = pnewlist;
			pnewlist = pbeam;

			pbeam = pnext;
			continue;
		}

		pbeam->flags &= ~(FBEAM_STARTENTITY | FBEAM_ENDENTITY);

		if( pbeam->type != TE_BEAMFOLLOW )
		{
			// remove beam
			pbeam->die = (float)cl.time - 0.1f;

			// kill off particles
			pHead = pbeam->particles;
			while( pHead )
			{
				pHead->die = (float)cl.time - 0.1f;
				pHead = pHead->next;
			}

			// free the beam
			CL_FreeBeam( pbeam );
		}
		else
		{
			// stay active
			pbeam->next = pnewlist;
			pnewlist = pbeam;
		}
		pbeam = pnext;
	}

	// We now have a new list with the bogus stuff released.
	cl_active_beams = pnewlist;
}

/*
==============
CL_CullBeam

Cull the beam by bbox
==============
*/
qboolean CL_CullBeam( const vec3_t start, const vec3_t end, qboolean pvsOnly )
{
	vec3_t	mins, maxs;
	int	i;

	// support for custom mirror management
	if( RI.currentbeam != NULL )
	{
		// don't reflect this entity in mirrors
		if( RI.currentbeam->curstate.effects & EF_NOREFLECT && RI.params & RP_MIRRORVIEW )
			return true;

		// draw only in mirrors
		if( RI.currentbeam->curstate.effects & EF_REFLECTONLY && !( RI.params & RP_MIRRORVIEW ))
			return true;
	}

	for( i = 0; i < 3; i++ )
	{
		if( start[i] < end[i] )
		{
			mins[i] = start[i];
			maxs[i] = end[i];
		}
		else
		{
			mins[i] = end[i];
			maxs[i] = start[i];
		}

		// don't let it be zero sized
		if( mins[i] == maxs[i] )
			maxs[i] += 1.0f;
	}

	// check bbox
	if( Mod_BoxVisible( mins, maxs, Mod_GetCurrentVis( )))
	{
		if( pvsOnly || !R_CullBox( mins, maxs, RI.clipFlags ))
		{
			// beam is visible
			return false;
		}
	}

	// beam is culled
	return true;
}

/*
==============
CL_RecomputeBeamEndpoints

Recomputes beam endpoints..
==============
*/
qboolean CL_RecomputeBeamEndpoints( BEAM *pbeam )
{
	if( pbeam->flags & FBEAM_STARTENTITY )
	{
		if( ComputeBeamEntPosition( pbeam->startEntity, pbeam->source ))
		{
			pbeam->flags |= FBEAM_STARTVISIBLE;
		}
		else if( !( pbeam->flags & FBEAM_FOREVER ))
		{
			pbeam->flags &= ~(FBEAM_STARTENTITY);
		}
		else
		{
			// MsgDev( D_WARN, "can't find start entity\n" );
			// return false;
		}

		// if we've never seen the start entity, don't display
		if( !( pbeam->flags & FBEAM_STARTVISIBLE ))
			return false;
	}

	if( pbeam->flags & FBEAM_ENDENTITY )
	{
		if( ComputeBeamEntPosition( pbeam->endEntity, pbeam->target ))
		{
			pbeam->flags |= FBEAM_ENDVISIBLE;
		}
		else if( !( pbeam->flags & FBEAM_FOREVER ))
		{
			pbeam->flags &= ~(FBEAM_ENDENTITY);
			pbeam->die = (float)cl.time;
			return false;
		}
		else
		{
			return false;
		}

		// if we've never seen the end entity, don't display
		if( !( pbeam->flags & FBEAM_ENDVISIBLE ))
			return false;
	}
	return true;
}

/*
==============
CL_BeamAttemptToDie

Check for expired beams
==============
*/
qboolean CL_BeamAttemptToDie( BEAM *pBeam )
{
	ASSERT( pBeam != NULL );

	// premanent beams never die automatically
	if( pBeam->flags & FBEAM_FOREVER )
		return false;

	if( pBeam->type == TE_BEAMFOLLOW && pBeam->particles )
	{
		// wait for all trails are dead
		return false;
	}

	// other beams
	if( pBeam->die > cl.time )
		return false;

	return true;
}

/*
==============
CL_UpdateBeam

Update beam vars
==============
*/
void CL_UpdateBeam( BEAM *pbeam, float frametime )
{
	pbeam->flags |= FBEAM_ISACTIVE;

	if( Mod_GetType( pbeam->modelIndex ) == mod_bad )
	{
		pbeam->flags &= ~FBEAM_ISACTIVE; // force to ignore
		pbeam->die = (float)cl.time;
		return;
	}

	// update frequency
	pbeam->freq += frametime;

	// Generate fractal noise
	if( CL_IsInGame() && !cl.refdef.paused )
	{
		rgNoise[0] = 0;
		rgNoise[NOISE_DIVISIONS] = 0;
	}

	if( pbeam->amplitude != 0 && CL_IsInGame() && !cl.refdef.paused )
	{
		if( pbeam->flags & FBEAM_SINENOISE )
		{
			SineNoise( rgNoise, NOISE_DIVISIONS );
		}
		else
		{
			FracNoise( rgNoise, NOISE_DIVISIONS, 1.0f );
		}
	}

	// update end points
	if( pbeam->flags & ( FBEAM_STARTENTITY|FBEAM_ENDENTITY ))
	{
		// makes sure attachment[0] + attachment[1] are valid
		if( !CL_RecomputeBeamEndpoints( pbeam ))
		{
			pbeam->flags &= ~FBEAM_ISACTIVE; // force to ignore
			return;
		}

		// compute segments from the new endpoints
		VectorSubtract( pbeam->target, pbeam->source, pbeam->delta );

		if( pbeam->amplitude >= 0.50f )
			pbeam->segments = (int)(VectorLength( pbeam->delta ) * 0.25f + 3.0f); // one per 4 pixels
		else pbeam->segments = (int)(VectorLength( pbeam->delta ) * 0.075f + 3.0f); // one per 16 pixels
	}

	// get position data for spline beam
	switch( pbeam->type )
	{
	case TE_BEAMPOINTS:
		if( CL_CullBeam( pbeam->source, pbeam->target, false ))
		{
			pbeam->flags &= ~FBEAM_ISACTIVE; // force to ignore
			return;
		}
		break;
	}

	if( pbeam->flags & ( FBEAM_FADEIN|FBEAM_FADEOUT ))
	{
		// update life cycle
		pbeam->t = pbeam->freq + ( pbeam->die - (float)cl.time );
		if( pbeam->t != 0.0f ) pbeam->t = 1.0f - pbeam->freq / pbeam->t;
	}
}

/*
==============
CL_DrawBeamFollow

Helper to drawing beam follow
==============
*/
void CL_DrawBeamFollow( int spriteIndex, BEAM *pbeam, int frame, int rendermode, float frametime, float *color )
{
	particle_t	*particles;
	particle_t	*pnew;
	float		div;
	vec3_t		delta;
	vec3_t		screenLast;
	vec3_t		screen;

	CL_FreeDeadTrails( &pbeam->particles );

	particles = pbeam->particles;
	pnew = NULL;

	div = 0;
	if( pbeam->flags & FBEAM_STARTENTITY )
	{
		if( particles )
		{
			VectorSubtract( particles->org, pbeam->source, delta );
			div = VectorLength( delta );

			if( div >= 32 && cl_free_trails )
			{
				pnew = cl_free_trails;
				cl_free_trails = pnew->next;
			}
		}
		else if( cl_free_trails )
		{
			pnew = cl_free_trails;
			cl_free_trails = pnew->next;
			div = 0;
		}
	}

	if( pnew )
	{
		VectorCopy( pbeam->source, pnew->org );
		pnew->die = (float)cl.time + pbeam->amplitude;
		VectorClear( pnew->vel );

		pnew->next = particles;
		pbeam->particles = pnew;
		particles = pnew;
	}

	// nothing to draw
	if( !particles ) return;

	if( !pnew && div != 0 )
	{
		VectorCopy( pbeam->source, delta );
		TriWorldToScreen( pbeam->source, screenLast );
		TriWorldToScreen( particles->org, screen );
	}
	else if( particles && particles->next )
	{
		VectorCopy( particles->org, delta );
		TriWorldToScreen( particles->org, screenLast );
		TriWorldToScreen( particles->next->org, screen );
		particles = particles->next;
	}
	else
	{
		return;
	}

	// draw it
	DrawBeamFollow( spriteIndex, pbeam->particles, frame, rendermode, delta, screen, screenLast,
		pbeam->die, pbeam->source, pbeam->flags, pbeam->width,
		pbeam->amplitude, pbeam->freq, color );

	// drift popcorn trail if there is a velocity
	particles = pbeam->particles;

	while( particles )
	{
		VectorMA( particles->org, frametime, particles->vel, particles->org );
		particles = particles->next;
	}
}

/*
==============
CL_DrawCustomBeam

Draw beam from server
==============
*/
void CL_DrawCustomBeam( cl_entity_t *pbeam )
{
	BEAM	beam;
	int	beamType;
	int	beamFlags;

	ASSERT( pbeam != NULL );

	// bad texture ?
	if( Mod_GetType( pbeam->curstate.modelindex ) != mod_sprite )
		return;

	Q_memset( &beam, 0, sizeof( beam ));

	beamType = ( pbeam->curstate.rendermode & 0x0F );
	beamFlags = ( pbeam->curstate.rendermode & 0xF0 );
	beam.modelIndex = pbeam->curstate.modelindex;
	beam.frame = pbeam->curstate.frame;
	beam.frameRate = pbeam->curstate.framerate;
	beam.speed = pbeam->curstate.animtime;
	Mod_GetFrames( beam.modelIndex, &beam.frameCount );
	VectorCopy( pbeam->origin, beam.source );
	VectorCopy( pbeam->angles, beam.target );
	beam.freq	= (float)cl.time * beam.speed;
	beam.startEntity = pbeam->curstate.sequence;
	beam.endEntity = pbeam->curstate.skin;
	beam.width = pbeam->curstate.scale;
	beam.amplitude = (float)(pbeam->curstate.body * 0.1f);
	beam.brightness = (float)pbeam->curstate.renderamt;
	beam.r = pbeam->curstate.rendercolor.r;
	beam.g = pbeam->curstate.rendercolor.g;
	beam.b = pbeam->curstate.rendercolor.b;
	beam.flags = 0;

	VectorSubtract( beam.target, beam.source, beam.delta );

	if( beam.amplitude >= 0.50f )
		beam.segments = (int)(VectorLength( beam.delta ) * 0.25f) + 3; // one per 4 pixels
	else beam.segments = (int)(VectorLength( beam.delta ) * 0.075f) + 3; // one per 16 pixels

	// handle code from relinking.
	switch( beamType )
	{
	case BEAM_ENTS:
		beam.type	= TE_BEAMPOINTS;
		beam.flags = FBEAM_STARTENTITY|FBEAM_ENDENTITY;
		break;
	case BEAM_HOSE:
		beam.type	= TE_BEAMHOSE;
		beam.flags = FBEAM_STARTENTITY|FBEAM_ENDENTITY;
		break;
	case BEAM_ENTPOINT:
		beam.type	= TE_BEAMPOINTS;
		if( beam.startEntity ) beam.flags |= FBEAM_STARTENTITY;
		if( beam.endEntity ) beam.flags |= FBEAM_ENDENTITY;
		break;
	case BEAM_POINTS:
		// already set up
		break;
	}

	beam.flags |= beamFlags & ( FBEAM_SINENOISE|FBEAM_SOLID|FBEAM_SHADEIN|FBEAM_SHADEOUT );

	// draw it
	CL_UpdateBeam( &beam, (float)(cl.time - cl.oldtime) );
	CL_DrawBeam( &beam );
}

void CL_DrawBeams( int fTrans )
{
	BEAM	*pBeam, *pNext;
	BEAM	*pPrev = NULL;
	int	i;

	// server beams don't allocate beam chains
	// all params are stored in cl_entity_t
	for( i = 0; i < cl.num_custombeams; i++ )
	{
		RI.currentbeam = cl_custombeams[i];

		if( fTrans && ((RI.currentbeam->curstate.rendermode & 0xF0) & FBEAM_SOLID ))
			continue;

		if( !fTrans && !((RI.currentbeam->curstate.rendermode & 0xF0) & FBEAM_SOLID ))
			continue;

		CL_DrawCustomBeam( RI.currentbeam );
		r_stats.c_view_beams_count++;
	}

	RI.currentbeam = NULL;

	if( !cl_active_beams )
		return;

	// draw temporary entity beams
	for( pBeam = cl_active_beams; pBeam; pBeam = pNext )
	{
		// need to store the next one since we may delete this one
		pNext = pBeam->next;

		if( fTrans && pBeam->flags & FBEAM_SOLID )
			continue;
		if( !fTrans && !( pBeam->flags & FBEAM_SOLID ))
			continue;

		// retire old beams
		if( CL_BeamAttemptToDie( pBeam ))
		{
			// reset links
			if( pPrev ) pPrev->next = pNext;
			else cl_active_beams = pNext;

			// free the beam
			CL_FreeBeam( pBeam );

			pBeam = NULL;
			continue;
		}

		// update beam state
		CL_UpdateBeam( pBeam, (float)(cl.time - cl.oldtime) );
		r_stats.c_view_beams_count++;
		CL_DrawBeam( pBeam );

		pPrev = pBeam;
	}
}

/*
==============
CL_BeamKill

Remove beam attached to specified entity
and all particle trails (if this is a beamfollow)
==============
*/
void CL_BeamKill( int deadEntity )
{
	cl_entity_t	*pDeadEntity;

	pDeadEntity = CL_GetBeamEntityByIndex( deadEntity );
	if( !pDeadEntity ) return;

	CL_KillDeadBeams( pDeadEntity );
}

/*
==============
CL_BeamEnts

Create beam between two ents
==============
*/
BEAM *CL_BeamEnts( int startEnt, int endEnt, int modelIndex, float life, float width, float amplitude, float brightness,
	float speed, int startFrame, float framerate, float r, float g, float b )
{
	cl_entity_t	*pStart, *pEnd;
	BEAM		*pBeam;

	// need a valid model.
	if( Mod_GetType( modelIndex ) != mod_sprite )
		return NULL;

	pStart = CL_GetBeamEntityByIndex( startEnt );
	pEnd = CL_GetBeamEntityByIndex( endEnt );

	// don't start temporary beams out of the PVS
	if( life != 0 && ( !pStart || !pStart->curstate.modelindex || !pEnd || !pEnd->curstate.modelindex ))
		return NULL;

	pBeam = CL_AllocBeam();
	if( !pBeam ) return NULL;

	pBeam->type = TE_BEAMPOINTS;
	pBeam->modelIndex = modelIndex;
	pBeam->startEntity = startEnt;
	pBeam->endEntity = endEnt;
	pBeam->frame = (float)startFrame;
	pBeam->frameRate = framerate;
	Mod_GetFrames( modelIndex, &pBeam->frameCount );
	pBeam->freq = (float)cl.time * speed;
	pBeam->flags = FBEAM_STARTENTITY|FBEAM_ENDENTITY;
	if( life == 0.0f ) pBeam->flags |= FBEAM_FOREVER;
	pBeam->die += life;
	pBeam->width = width;
	pBeam->amplitude = amplitude * 10;
	pBeam->speed = speed;
	BeamNormalizeColor( pBeam, r, g, b, brightness );

	CL_UpdateBeam( pBeam, 0.0f );

	return pBeam;
}

/*
==============
CL_BeamPoints

Create beam between two points
==============
*/
BEAM *CL_BeamPoints( const vec3_t start, const vec3_t end, int modelIndex, float life, float width, float amplitude,
	float brightness, float speed, int startFrame, float framerate, float r, float g, float b )
{
	BEAM	*pBeam;

	// need a valid model.
	if( Mod_GetType( modelIndex ) != mod_sprite )
		return NULL;

	// don't start temporary beams out of the PVS
	if( life != 0.0f && CL_CullBeam( start, end, true ))
		return NULL;

	pBeam = CL_AllocBeam();
	if( !pBeam ) return NULL;

	pBeam->type = TE_BEAMPOINTS;
	pBeam->modelIndex = modelIndex;
	VectorCopy( start, pBeam->source );
	VectorCopy( end, pBeam->target );
	pBeam->frame = (float)startFrame;
	pBeam->frameRate = framerate;
	Mod_GetFrames( modelIndex, &pBeam->frameCount );
	pBeam->freq = (float)cl.time * speed;
	if( life == 0.0f ) pBeam->flags |= FBEAM_FOREVER;
	pBeam->die += life;
	pBeam->width = width;
	pBeam->amplitude = amplitude;
	pBeam->speed = speed;
	BeamNormalizeColor( pBeam, r, g, b, brightness );

	VectorSubtract( pBeam->target, pBeam->source, pBeam->delta );

	if( pBeam->amplitude >= 0.50f )
		pBeam->segments = (int)(VectorLength( pBeam->delta ) * 0.25f) + 3; // one per 4 pixels
	else pBeam->segments = (int)(VectorLength( pBeam->delta ) * 0.075f) + 3; // one per 16 pixels

	CL_UpdateBeam( pBeam, 0.0f );

	return pBeam;
}

/*
==============
CL_BeamLighting

Create beam between two points (simple version)
==============
*/
BEAM *CL_BeamLightning( const vec3_t start, const vec3_t end, int modelIndex, float life, float width, float amplitude,
	float brightness, float speed )
{
	return CL_BeamPoints( start, end, modelIndex, life, width, amplitude, brightness, speed,
		0, 1.0f, 255.0f, 255.0f, 255.0f );
}

/*
==============
CL_BeamCirclePoints

Create beam cicrle
==============
*/
BEAM *CL_BeamCirclePoints( int type, const vec3_t start, const vec3_t end, int modelIndex, float life, float width,
	float amplitude, float brightness, float speed, int startFrame, float framerate, float r, float g, float b )
{
	BEAM	*pBeam;

	// need a valid model.
	if( Mod_GetType( modelIndex ) != mod_sprite )
		return NULL;

	pBeam = CL_AllocBeam();
	if( !pBeam ) return NULL;

	pBeam->type = type;
	pBeam->modelIndex = modelIndex;
	VectorCopy( start, pBeam->source );
	VectorCopy( end, pBeam->target );
	pBeam->frame = (float)startFrame;
	pBeam->frameRate = framerate;
	Mod_GetFrames( modelIndex, &pBeam->frameCount );
	pBeam->freq = (float)cl.time * speed;
	if( life == 0.0f ) pBeam->flags |= FBEAM_FOREVER;
	pBeam->die += life;
	pBeam->width = width;
	pBeam->amplitude = amplitude;
	pBeam->speed = speed;
	BeamNormalizeColor( pBeam, r, g, b, brightness );

	VectorSubtract( pBeam->target, pBeam->source, pBeam->delta );

	if( pBeam->amplitude >= 0.50f )
		pBeam->segments = (int)(VectorLength( pBeam->delta ) * 0.25f) + 3; // one per 4 pixels
	else pBeam->segments = (int)(VectorLength( pBeam->delta ) * 0.075f) + 3; // one per 16 pixels

	CL_UpdateBeam( pBeam, 0.0f );

	return pBeam;
}


/*
==============
CL_BeamEntPoint

Create beam between entity and point
==============
*/
BEAM *CL_BeamEntPoint( int startEnt, const vec3_t end, int modelIndex, float life, float width, float amplitude,
	float brightness, float speed, int startFrame, float framerate, float r, float g, float b )
{
	cl_entity_t	*pStart;
	BEAM		*pBeam;

	// need a valid model.
	if( Mod_GetType( modelIndex ) != mod_sprite )
		return NULL;

	pStart = CL_GetBeamEntityByIndex( startEnt );

	// don't start temporary beams out of the PVS
	if( life != 0.0f && ( !pStart || !pStart->curstate.modelindex ))
		return NULL;

	pBeam = CL_AllocBeam();
	if( !pBeam ) return NULL;

	pBeam->type = TE_BEAMPOINTS;
	pBeam->flags = FBEAM_STARTENTITY;
	pBeam->modelIndex = modelIndex;
	pBeam->pFollowModel = Mod_Handle( modelIndex );
	pBeam->startEntity = startEnt;
	VectorCopy( end, pBeam->target );
	pBeam->frame = (float)startFrame;
	pBeam->frameRate = framerate;
	Mod_GetFrames( modelIndex, &pBeam->frameCount );
	pBeam->freq = (float)cl.time * speed;
	if( life == 0.0f ) pBeam->flags |= FBEAM_FOREVER;
	pBeam->die += life;
	pBeam->width = width;
	pBeam->amplitude = amplitude * 10;
	pBeam->speed = speed;
	BeamNormalizeColor( pBeam, r, g, b, brightness );

	VectorSubtract( pBeam->target, pBeam->source, pBeam->delta );

	if( pBeam->amplitude >= 0.50f )
		pBeam->segments = (int)(VectorLength( pBeam->delta ) * 0.25f) + 3; // one per 4 pixels
	else pBeam->segments = (int)(VectorLength( pBeam->delta ) * 0.075f) + 3; // one per 16 pixels

	CL_UpdateBeam( pBeam, 0.0f );

	return pBeam;
}

/*
==============
CL_BeamRing

Create beam between two ents
==============
*/
BEAM *CL_BeamRing( int startEnt, int endEnt, int modelIndex, float life, float width, float amplitude, float brightness,
	float speed, int startFrame, float framerate, float r, float g, float b )
{
	cl_entity_t	*pStart, *pEnd;
	BEAM		*pBeam;

	// need a valid model.
	if( Mod_GetType( modelIndex ) != mod_sprite )
		return NULL;

	pStart = CL_GetBeamEntityByIndex( startEnt );
	pEnd = CL_GetBeamEntityByIndex( endEnt );

	// don't start temporary beams out of the PVS
	if( life != 0 && ( !pStart || !pStart->curstate.modelindex || !pEnd || !pEnd->curstate.modelindex ))
		return NULL;

	pBeam = CL_AllocBeam();
	if( !pBeam ) return NULL;

	pBeam->type = TE_BEAMRING;
	pBeam->modelIndex = modelIndex;
	pBeam->startEntity = startEnt;
	pBeam->endEntity = endEnt;
	pBeam->frame = (float)startFrame;
	pBeam->frameRate = framerate;
	Mod_GetFrames( modelIndex, &pBeam->frameCount );
	pBeam->freq = (float)cl.time * speed;
	pBeam->flags = FBEAM_STARTENTITY|FBEAM_ENDENTITY;
	if( life == 0.0f ) pBeam->flags |= FBEAM_FOREVER;
	pBeam->die += life;
	pBeam->width = width;
	pBeam->amplitude = amplitude;
	pBeam->speed = speed;
	BeamNormalizeColor( pBeam, r, g, b, brightness );

	CL_UpdateBeam( pBeam, 0.0f );

	return pBeam;
}

/*
==============
CL_BeamFollow

Create beam following with entity
==============
*/
BEAM *CL_BeamFollow( int startEnt, int modelIndex, float life, float width, float r, float g, float b, float bright )
{
	cl_entity_t	*pStart;
	BEAM		*pBeam;

	// need a valid model.
	if( Mod_GetType( modelIndex ) != mod_sprite )
		return NULL;

	pStart = CL_GetBeamEntityByIndex( startEnt );

	// don't start temporary beams out of the PVS
	if( life != 0.0f && ( !pStart || !pStart->curstate.modelindex ))
		return NULL;

	pBeam = CL_AllocBeam();
	if( !pBeam ) return NULL;

	pBeam->type = TE_BEAMFOLLOW;
	pBeam->flags = FBEAM_STARTENTITY;
	pBeam->modelIndex = modelIndex;
	pBeam->pFollowModel = Mod_Handle( modelIndex );
	pBeam->startEntity = startEnt;
	pBeam->frame = 0;
	pBeam->frameRate = 1.0f;
	Mod_GetFrames( modelIndex, &pBeam->frameCount );
	if( life == 0.0f ) pBeam->flags |= FBEAM_FOREVER;
	pBeam->freq = (float)cl.time;
	pBeam->die += life;
	pBeam->width = width;
	pBeam->amplitude = life;	// partilces lifetime
	pBeam->speed = 1.0f;
	BeamNormalizeColor( pBeam, r, g, b, bright );

	CL_UpdateBeam( pBeam, 0.0f );

	return pBeam;
}

/*
==============
CL_BeamSprite

Create a beam with sprite at the end
Valve legacy
==============
*/
void CL_BeamSprite( const vec3_t start, const vec3_t end, int beamIndex, int spriteIndex )
{
	CL_BeamLightning( start, end, beamIndex, 0.1f, 1.0f, 0.0f, 255, 1.0f );
	CL_DefaultSprite( end, spriteIndex, 1.0f );
}

/*
==============
CL_ParseViewBeam

handle beam messages
==============
*/
void CL_ParseViewBeam( sizebuf_t *msg, int beamType )
{
	vec3_t	start, end;
	int	modelIndex, startFrame;
	float	frameRate, life, width;
	float	brightness, noise, speed;
	int	startEnt, endEnt;
	float	r, g, b;

	switch( beamType )
	{
	case TE_BEAMPOINTS:
		start[0] = BF_ReadCoord( msg );
		start[1] = BF_ReadCoord( msg );
		start[2] = BF_ReadCoord( msg );
		end[0] = BF_ReadCoord( msg );
		end[1] = BF_ReadCoord( msg );
		end[2] = BF_ReadCoord( msg );
		modelIndex = BF_ReadShort( msg );
		startFrame = BF_ReadByte( msg );
		frameRate = (float)BF_ReadByte( msg );
		life = (float)(BF_ReadByte( msg ) * 0.1f);
		width = (float)(BF_ReadByte( msg ) * 0.1f);
		noise = (float)(BF_ReadByte( msg ) * 0.1f);
		r = (float)BF_ReadByte( msg );
		g = (float)BF_ReadByte( msg );
		b = (float)BF_ReadByte( msg );
		brightness = (float)BF_ReadByte( msg );
		speed = (float)(BF_ReadByte( msg ) * 0.1f);
		CL_BeamPoints( start, end, modelIndex, life, width, noise, brightness, speed, startFrame,
			frameRate, r, g, b );
		break;
	case TE_BEAMENTPOINT:
		startEnt = BF_ReadShort( msg );
		end[0] = BF_ReadCoord( msg );
		end[1] = BF_ReadCoord( msg );
		end[2] = BF_ReadCoord( msg );
		modelIndex = BF_ReadShort( msg );
		startFrame = BF_ReadByte( msg );
		frameRate = (float)BF_ReadByte( msg );
		life = (float)(BF_ReadByte( msg ) * 0.1f);
		width = (float)(BF_ReadByte( msg ) * 0.1f);
		noise = (float)(BF_ReadByte( msg ) * 0.01f);
		r = (float)BF_ReadByte( msg );
		g = (float)BF_ReadByte( msg );
		b = (float)BF_ReadByte( msg );
		brightness = (float)BF_ReadByte( msg );
		speed = (float)(BF_ReadByte( msg ) * 0.1f);
		CL_BeamEntPoint( startEnt, end, modelIndex, life, width, noise, brightness, speed, startFrame,
		frameRate, r, g, b );
		break;
	case TE_LIGHTNING:
		start[0] = BF_ReadCoord( msg );
		start[1] = BF_ReadCoord( msg );
		start[2] = BF_ReadCoord( msg );
		end[0] = BF_ReadCoord( msg );
		end[1] = BF_ReadCoord( msg );
		end[2] = BF_ReadCoord( msg );
		life = (float)(BF_ReadByte( msg ) * 0.1f);
		width = (float)(BF_ReadByte( msg ) * 0.1f);
		noise = (float)(BF_ReadByte( msg ) * 0.1f);
		modelIndex = BF_ReadShort( msg );
		CL_BeamLightning( start, end, modelIndex, life, width, noise, 255.0f, 1.0f );
		break;
	case TE_BEAMENTS:
		startEnt = BF_ReadShort( msg );
		endEnt = BF_ReadShort( msg );
		modelIndex = BF_ReadShort( msg );
		startFrame = BF_ReadByte( msg );
		frameRate = (float)(BF_ReadByte( msg ) * 0.1f);
		life = (float)(BF_ReadByte( msg ) * 0.1f);
		width = (float)(BF_ReadByte( msg ) * 0.1f);
		noise = (float)(BF_ReadByte( msg ) * 0.01f);
		r = (float)BF_ReadByte( msg );
		g = (float)BF_ReadByte( msg );
		b = (float)BF_ReadByte( msg );
		brightness = (float)BF_ReadByte( msg );
		speed = (float)(BF_ReadByte( msg ) * 0.1f);
		CL_BeamEnts( startEnt, endEnt, modelIndex, life, width, noise, brightness, speed, startFrame,
		frameRate, r, g, b );
		break;
	case TE_BEAM:
		MsgDev( D_ERROR, "TE_BEAM is obsolete\n" );
		break;
	case TE_BEAMSPRITE:
		start[0] = BF_ReadCoord( msg );
		start[1] = BF_ReadCoord( msg );
		start[2] = BF_ReadCoord( msg );
		end[0] = BF_ReadCoord( msg );
		end[1] = BF_ReadCoord( msg );
		end[2] = BF_ReadCoord( msg );
		modelIndex = BF_ReadShort( msg );	// beam model
		startFrame = BF_ReadShort( msg );	// sprite model
		CL_BeamSprite( start, end, modelIndex, startFrame );
		break;
	case TE_BEAMTORUS:
	case TE_BEAMDISK:
	case TE_BEAMCYLINDER:
		start[0] = BF_ReadCoord( msg );
		start[1] = BF_ReadCoord( msg );
		start[2] = BF_ReadCoord( msg );
		end[0] = BF_ReadCoord( msg );
		end[1] = BF_ReadCoord( msg );
		end[2] = BF_ReadCoord( msg );
		modelIndex = BF_ReadShort( msg );
		startFrame = BF_ReadByte( msg );
		frameRate = (float)(BF_ReadByte( msg ) * 0.1f);
		life = (float)(BF_ReadByte( msg ) * 0.1f);
		width = (float)BF_ReadByte( msg );
		noise = (float)(BF_ReadByte( msg ) * 0.1f);
		r = (float)BF_ReadByte( msg );
		g = (float)BF_ReadByte( msg );
		b = (float)BF_ReadByte( msg );
		brightness = (float)BF_ReadByte( msg );
		speed = (float)(BF_ReadByte( msg ) * 0.1f);
		CL_BeamCirclePoints( beamType, start, end, modelIndex, life, width, noise, brightness, speed,
		startFrame, frameRate, r, g, b );
		break;
	case TE_BEAMFOLLOW:
		startEnt = BF_ReadShort( msg );
		modelIndex = BF_ReadShort( msg );
		life = (float)(BF_ReadByte( msg ) * 0.1f);
		width = (float)BF_ReadByte( msg );
		r = (float)BF_ReadByte( msg );
		g = (float)BF_ReadByte( msg );
		b = (float)BF_ReadByte( msg );
		brightness = (float)BF_ReadByte( msg );
		CL_BeamFollow( startEnt, modelIndex, life, width, r, g, b, brightness );
		break;
	case TE_BEAMRING:
		startEnt = BF_ReadShort( msg );
		endEnt = BF_ReadShort( msg );
		modelIndex = BF_ReadShort( msg );
		startFrame = BF_ReadByte( msg );
		frameRate = (float)BF_ReadByte( msg );
		life = (float)(BF_ReadByte( msg ) * 0.1f);
		width = (float)(BF_ReadByte( msg ) * 0.1f);
		noise = (float)(BF_ReadByte( msg ) * 0.1f);
		r = (float)BF_ReadByte( msg );
		g = (float)BF_ReadByte( msg );
		b = (float)BF_ReadByte( msg );
		brightness = (float)BF_ReadByte( msg );
		speed = (float)(BF_ReadByte( msg ) * 0.1f);
		CL_BeamRing( startEnt, endEnt, modelIndex, life, width, noise, brightness, speed, startFrame,
		frameRate, r, g, b );
		break;
	case TE_BEAMHOSE:
		MsgDev( D_ERROR, "TE_BEAMHOSE is obsolete\n" );
		break;
	case TE_KILLBEAM:
		startEnt = BF_ReadShort( msg );
		CL_BeamKill( startEnt );
		break;
	}
}

/*
===============
CL_ReadLineFile_f

Optimized version of pointfile - use beams instead of particles
===============
*/
void CL_ReadLineFile_f( void )
{
	char		*afile, *pfile;
	vec3_t		p1, p2;
	int		count, modelIndex;
	char		filename[64];
	string		token;

	Q_snprintf( filename, sizeof( filename ), "maps/%s.lin", clgame.mapname );
	afile = FS_LoadFile( filename, NULL, false );

	if( !afile )
	{
		MsgDev( D_ERROR, "couldn't open %s\n", filename );
		return;
	}

	Msg( "Reading %s...\n", filename );

	count = 0;
	pfile = afile;
	modelIndex = CL_FindModelIndex( "sprites/laserbeam.spr" );

	while( 1 )
	{
		pfile = COM_ParseFile( pfile, token );
		if( !pfile ) break;
		p1[0] = Q_atof( token );

		pfile = COM_ParseFile( pfile, token );
		if( !pfile ) break;
		p1[1] = Q_atof( token );

		pfile = COM_ParseFile( pfile, token );
		if( !pfile ) break;
		p1[2] = Q_atof( token );

		pfile = COM_ParseFile( pfile, token );
		if( !pfile ) break;

		if( token[0] != '-' )
		{
			MsgDev( D_ERROR, "%s is corrupted\n" );
			break;
		}

		pfile = COM_ParseFile( pfile, token );
		if( !pfile ) break;
		p2[0] = Q_atof( token );

		pfile = COM_ParseFile( pfile, token );
		if( !pfile ) break;
		p2[1] = Q_atof( token );

		pfile = COM_ParseFile( pfile, token );
		if( !pfile ) break;
		p2[2] = Q_atof( token );

		count++;

		if( !CL_BeamPoints( p1, p2, modelIndex, 99999, 2, 0, 255, 0, 0, 0, 255.0f, 0.0f, 0.0f ))
		{
			if( Mod_GetType( modelIndex ) != mod_sprite )
				MsgDev( D_ERROR, "CL_ReadLineFile: failed to load sprites/laserbeam.spr!\n" );
			else MsgDev( D_ERROR, "CL_ReadLineFile: not enough free beams!\n" );
			break;
		}
	}

	Mem_Free( afile );

	if( count ) Msg( "%i lines read\n", count );
	else Msg( "map %s has no leaks!\n", clgame.mapname );
}
