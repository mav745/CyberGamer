/*
vgui_draw.c - vgui draw methods
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

#include <qt/c_gate.h>

#include "client.h"

convar_t	*vgui_colorstrings;
int	g_textures[VGUI_MAX_TEXTURES];
int	g_textureId = 0;
int	g_iBoundTexture;

/*
================
VGUI_DrawInit

Startup VGUI backend
================
*/
void VGUI_DrawInit( void )
{
	Q_memset( g_textures, 0, sizeof( g_textures ));
	g_textureId = g_iBoundTexture = 0;

	vgui_colorstrings = Cvar_Get( "vgui_colorstrings", "0", CVAR_ARCHIVE, "allow colorstrings in VGUI texts" );
}

/*
================
VGUI_DrawShutdown

Release all textures
================
*/
void VGUI_DrawShutdown( void )
{
	int	i;

	for( i = 1; i < g_textureId; i++ )
	{
		GL_FreeImage( va(  "*vgui%i", i ));
	}
}

/*
================
VGUI_GenerateTexture

generate unique texture number
================
*/
int VGUI_GenerateTexture( void )
{
	if( ++g_textureId >= VGUI_MAX_TEXTURES )
		Sys_Error( "VGUI_GenerateTexture: VGUI_MAX_TEXTURES limit exceeded\n" );
	return g_textureId;
}

/*
================
VGUI_UploadTexture

Upload texture into video memory
================
*/
void VGUI_UploadTexture( int id, const char *buffer, int width, int height )
{
	rgbdata_t	r_image;
	char	texName[32];

	if( id <= 0 || id >= VGUI_MAX_TEXTURES )
	{
		MsgDev( D_ERROR, "VGUI_UploadTexture: bad texture %i. Ignored\n", id );
		return;
	}

	Q_snprintf( texName, sizeof( texName ), "*vgui%i", id );
	r_image.width = width;
	r_image.height = height;
	r_image.type = PF_RGBA_32;
	r_image.size = r_image.width * r_image.height * 4;
	r_image.flags = IMAGE_HAS_COLOR|IMAGE_HAS_ALPHA;
	r_image.buffer = (byte *)buffer;

	g_textures[id] = GL_LoadTextureInternal( texName, &r_image, TF_IMAGE, false );
	GL_SetTextureType( g_textures[id], TEX_VGUI );
	g_iBoundTexture = id;
}

/*
================
VGUI_CreateTexture

Create empty rgba texture and upload them into video memory
================
*/
void VGUI_CreateTexture( int id, int width, int height )
{
	rgbdata_t	r_image;
	char	texName[32];

	if( id <= 0 || id >= VGUI_MAX_TEXTURES )
	{
		MsgDev( D_ERROR, "VGUI_CreateTexture: bad texture %i. Ignored\n", id );
		return;
	}

	Q_snprintf( texName, sizeof( texName ), "*vgui%i", id );
	r_image.width = width;
	r_image.height = height;
	r_image.type = PF_RGBA_32;
	r_image.size = r_image.width * r_image.height * 4;
	r_image.flags = IMAGE_HAS_ALPHA;
	r_image.buffer = NULL;

	g_textures[id] = GL_LoadTextureInternal( texName, &r_image, TF_IMAGE|TF_NEAREST, false );
	GL_SetTextureType( g_textures[id], TEX_VGUI );
	g_iBoundTexture = id;
}

/*
================
VGUI_GetTextureSizes

returns wide and tall for currently binded texture
================
*/
void VGUI_GetTextureSizes( int *width, int *height )
{
	gltexture_t	*glt;
	int		texnum;

	if( g_iBoundTexture )
		texnum = g_textures[g_iBoundTexture];
	else texnum = tr.defaultTexture;

	glt = R_GetTexture( texnum );
	if( width ) *width = glt->srcWidth;
	if( height ) *height = glt->srcHeight;
}

