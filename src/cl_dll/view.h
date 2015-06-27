//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

#ifndef VIEW_H
#define VIEW_H

#include "util_vector.h"

void V_StartPitchDrift( void );
void V_StopPitchDrift( void );

void V_GetInEyePos(int entity, float * origin, float * angles );
void V_ResetChaseCam();
void V_GetChasePos(int target, float * cl_angles, float * origin, float * angles);
void VectorAngles( const float *forward, float *angles );
//void NormalizeAngles( float *angles );
float * GetClientColor( int clientIndex );

extern vec3_t v_origin;		// last view origin
extern vec3_t v_angles;		// last view angle
extern vec3_t v_cl_angles;	// last client/mouse angle
extern vec3_t v_sim_org;	// last sim origin

#endif // VIEW_H
