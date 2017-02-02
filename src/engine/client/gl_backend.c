/*
gl_backend.c - rendering backend
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

#include <qt/c_gate.h>

#include "client.h"
#include "mathlib.h"


char		r_speeds_msg[MAX_SYSPATH];
ref_speeds_t	r_stats;	// r_speeds counters

/*
===============
R_SpeedsMessage
===============
*/
qboolean R_SpeedsMessage( char *out, size_t size )
{
	if( clgame.drawFuncs.R_SpeedsMessage != NULL )
	{
		if( clgame.drawFuncs.R_SpeedsMessage( out, size ))
			return true;
		// otherwise pass to default handler
	}

	if( r_speeds->integer <= 0 ) return false;
	if( !out || !size ) return false;

	Q_strncpy( out, r_speeds_msg, size );

	return true;
}

/*
==============
GL_BackendStartFrame
==============
*/
void __cdecl GL_BackendStartFrame( void )
{
	r_speeds_msg[0] = '\0';

	if( !RI.drawWorld ) R_Set2DMode( false );
}

/*
==============
GL_BackendEndFrame
==============
*/
void __cdecl GL_BackendEndFrame( void )
{
	// go into 2D mode (in case we draw PlayerSetup between two 2d calls)
	if( !RI.drawWorld ) R_Set2DMode( true );

	if( r_speeds->integer <= 0 || !RI.drawWorld )
		return;

	switch( r_speeds->integer )
	{
	case 1:
		Q_snprintf( r_speeds_msg, sizeof( r_speeds_msg ), "%3i wpoly, %3i bpoly\n%3i epoly, %3i spoly",
		r_stats.c_world_polys, r_stats.c_brush_polys, r_stats.c_studio_polys, r_stats.c_sprite_polys );
		break;
	case 2:
		Q_snprintf( r_speeds_msg, sizeof( r_speeds_msg ), "visible leafs:\n%3i leafs\ncurrent leaf %3i",
		r_stats.c_world_leafs, r_viewleaf - cl.worldmodel->leafs );
		break;
	case 3:
		Q_snprintf( r_speeds_msg, sizeof( r_speeds_msg ), "%3i studio models drawn\n%3i sprites drawn",
		r_stats.c_studio_models_drawn, r_stats.c_sprite_models_drawn );
		break;
	case 4:
		Q_snprintf( r_speeds_msg, sizeof( r_speeds_msg ), "%3i static entities\n%3i normal entities",
		r_numStatics, r_numEntities - r_numStatics );
		break;
	case 5:
		Q_snprintf( r_speeds_msg, sizeof( r_speeds_msg ), "%3i tempents\n%3i viewbeams\n%3i particles",
		r_stats.c_active_tents_count, r_stats.c_view_beams_count, r_stats.c_particle_count );
		break;
	case 6:
		Q_snprintf( r_speeds_msg, sizeof( r_speeds_msg ), "%3i mirrors\n", r_stats.c_mirror_passes );
		break;
	}

	Q_memset( &r_stats, 0, sizeof( r_stats ));
}

/*
==============
GL_DisableAllTexGens
==============
*/
void GL_DisableAllTexGens( void )
{
	GL_TexGen( GL_S, 0 );
	GL_TexGen( GL_T, 0 );
	GL_TexGen( GL_R, 0 );
	GL_TexGen( GL_Q, 0 );
}

/*
==============================================================================

SCREEN SHOTS

==============================================================================
*/
// used for 'env' and 'sky' shots
//typedef struct envmap_s
//{
//	vec3_t	angles;
//	int	flags;
//} envmap_t;

const envmap_t r_skyBoxInfo[6] =
{
{{   0, 270, 180}, IMAGE_FLIP_X },
{{   0,  90, 180}, IMAGE_FLIP_X },
{{ -90,   0, 180}, IMAGE_FLIP_X },
{{  90,   0, 180}, IMAGE_FLIP_X },
{{   0,   0, 180}, IMAGE_FLIP_X },
{{   0, 180, 180}, IMAGE_FLIP_X },
};

const envmap_t r_envMapInfo[6] =
{
{{  0,   0,  90}, 0 },
{{  0, 180, -90}, 0 },
{{  0,  90,   0}, 0 },
{{  0, 270, 180}, 0 },
{{-90, 180, -90}, 0 },
{{ 90,   0,  90}, 0 }
};

/*
================
VID_ImageAdjustGamma
================
*/
void VID_ImageAdjustGamma( byte *in, uint width, uint height )
{
	int	i, c = width * height;
	float	g = 1.0f / bound( 0.5f, vid_gamma->value, 2.3f );
	byte	r_gammaTable[256];	// adjust screenshot gamma
	byte	*p = in;

	if( !gl_compensate_gamma_screenshots->integer )
		return;

	// rebuild the gamma table
	for( i = 0; i < 256; i++ )
	{
		if( g == 1.0f ) r_gammaTable[i] = i;
		else
		{
			byte rgti = (byte)(255.f * powf((i + 0.5f) / 255.5f, g ) + 0.5f);
			r_gammaTable[i] = bound( 0, rgti, 255 );
		}
	}

	// adjust screenshots gamma
	for( i = 0; i < c; i++, p += 3 )
	{
		p[0] = r_gammaTable[p[0]];
		p[1] = r_gammaTable[p[1]];
		p[2] = r_gammaTable[p[2]];
	}
}

