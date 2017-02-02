/*
vgui_draw.h - vgui draw methods
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

#ifndef VGUI_DRAW_H
#define VGUI_DRAW_H

#ifdef __cplusplus
extern "C" {
#endif

#define VGUI_MAX_TEXTURES	2048	// a half of total textures count

extern rgba_t g_color_table[8];	// for colored strings support
extern convar_t *vgui_colorstrings;

// VGUI generic vertex
typedef struct
{
	vec2_t	point;
	vec2_t	coord;
} vpoint_t;

//
// vgui_backend.c
//

void __cdecl VGUI_DrawInit( void );
void __cdecl VGUI_DrawShutdown( void );
void __cdecl VGUI_SetupDrawingText( int *pColor );
void __cdecl VGUI_SetupDrawingRect( int *pColor );
void __cdecl VGUI_SetupDrawingImage( int *pColor );
void __cdecl VGUI_BindTexture( int id );
void __cdecl VGUI_EnableTexture( qboolean enable );
void __cdecl VGUI_CreateTexture( int id, int width, int height );
void __cdecl VGUI_UploadTexture( int id, const char *buffer, int width, int height );
void __cdecl VGUI_UploadTextureBlock( int id, int drawX, int drawY, const byte *rgba, int blockWidth, int blockHeight );
long __cdecl VGUI_SurfaceWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
void __cdecl VGUI_DrawQuad( const vpoint_t *ul, const vpoint_t *lr );
void __cdecl VGUI_GetTextureSizes( int *width, int *height );
int __cdecl VGUI_GenerateTexture( void );
void *__cdecl VGui_GetPanel( void );

#ifdef __cplusplus
void __cdecl EnableScissor( qboolean enable );
void __cdecl SetScissorRect( int left, int top, int right, int bottom );
qboolean __cdecl ClipRect( const vpoint_t &inUL, const vpoint_t &inLR, vpoint_t *pOutUL, vpoint_t *pOutLR );
#endif

//
// gl_vidnt.c
//
qboolean __cdecl R_DescribeVIDMode( int width, int height );

//
// vgui_int.c
//
void __cdecl VGui_Startup( void );
void __cdecl VGui_Shutdown( void );
void *__cdecl VGui_GetPanel( void );
void __cdecl VGui_Paint( void );
void __cdecl VGui_RunFrame( void );
void __cdecl VGui_ViewportPaintBackground( int extents[4] );

#ifdef __cplusplus
}
#endif
#endif//VGUI_DRAW_H
