/*
vgui_clip.cpp - clip in 2D space
Copyright (C) 2011 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "common.h"
#include "vgui_draw.h"
#include "wrect.h"

//-----------------------------------------------------------------------------
// For simulated scissor tests...
//-----------------------------------------------------------------------------
static wrect_t g_ScissorRect;
static qboolean g_bScissor = false;

//-----------------------------------------------------------------------------
// Enable/disable scissoring...
//-----------------------------------------------------------------------------
void EnableScissor( qboolean enable )
{
	g_bScissor = enable;
}

void SetScissorRect( int left, int top, int right, int bottom )
{
	// Check for a valid rectangle...
	Assert( left <= right );
	Assert( top <= bottom );

	g_ScissorRect.left = left;
	g_ScissorRect.top = top;
	g_ScissorRect.right = right;
	g_ScissorRect.bottom = bottom;
}

//-----------------------------------------------------------------------------
// Purpose: Used for clipping, produces an interpolated texture coordinate
//-----------------------------------------------------------------------------
inline float InterpTCoord( float val, float mins, float maxs, float tMin, float tMax )
{
	float	flPercent;

	if( mins != maxs )
		flPercent = (val - mins) / (maxs - mins);
	else flPercent = 0.5f;

	return tMin + (tMax - tMin) * flPercent;
}

//-----------------------------------------------------------------------------
// Purpose: Does a scissor clip of the input rectangle.
// Returns false if it is completely clipped off.
//-----------------------------------------------------------------------------
qboolean ClipRect( const vpoint_t &inUL, const vpoint_t &inLR, vpoint_t *pOutUL, vpoint_t *pOutLR )
{
	if( g_bScissor )
	{
		// pick whichever left side is larger
		if( g_ScissorRect.left > inUL.point[0] )
			pOutUL->point[0] = static_cast<float>(g_ScissorRect.left);
		else
			pOutUL->point[0] = inUL.point[0];

		// pick whichever right side is smaller
		if( g_ScissorRect.right <= inLR.point[0] )
			pOutLR->point[0] = static_cast<float>(g_ScissorRect.right);
		else
			pOutLR->point[0] = inLR.point[0];

		// pick whichever top side is larger
		if( g_ScissorRect.top > inUL.point[1] )
			pOutUL->point[1] = static_cast<float>(g_ScissorRect.top);
		else
			pOutUL->point[1] = inUL.point[1];

		// pick whichever bottom side is smaller
		if( g_ScissorRect.bottom <= inLR.point[1] )
			pOutLR->point[1] = static_cast<float>(g_ScissorRect.bottom);
		else
			pOutLR->point[1] = inLR.point[1];

		// Check for non-intersecting
		if(( pOutUL->point[0] > pOutLR->point[0] ) || ( pOutUL->point[1] > pOutLR->point[1] ))
		{
			return false;
		}

		pOutUL->coord[0] = InterpTCoord(pOutUL->point[0],
				inUL.point[0], inLR.point[0], inUL.coord[0], inLR.coord[0] );
			pOutLR->coord[0] = InterpTCoord(pOutLR->point[0],
				inUL.point[0], inLR.point[0], inUL.coord[0], inLR.coord[0] );

			pOutUL->coord[1] = InterpTCoord(pOutUL->point[1],
				inUL.point[1], inLR.point[1], inUL.coord[1], inLR.coord[1] );
			pOutLR->coord[1] = InterpTCoord(pOutLR->point[1],
				inUL.point[1], inLR.point[1], inUL.coord[1], inLR.coord[1] );
	}
	else
	{
		*pOutUL = inUL;
		*pOutLR = inLR;
	}
	return true;
}
