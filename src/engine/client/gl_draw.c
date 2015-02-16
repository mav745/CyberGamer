/*
gl_draw.c - orthogonal drawing stuff
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

/*
=============
R_GetImageParms
=============
*/
void R_GetTextureParms( int *w, int *h, int texnum )
{
	gltexture_t	*glt;

	glt = R_GetTexture( texnum );
	if( w ) *w = glt->srcWidth;
	if( h ) *h = glt->srcHeight;
}

/*
=============
R_GetSpriteParms

same as GetImageParms but used
for sprite models
=============
*/
void R_GetSpriteParms( int *frameWidth, int *frameHeight, int *numFrames, int currentFrame, const model_t *pSprite )
{
	mspriteframe_t	*pFrame;

	if( !pSprite || pSprite->type != mod_sprite ) return; // bad model ?
	pFrame = R_GetSpriteFrame( pSprite, currentFrame, 0.0f );

	if( frameWidth ) *frameWidth = pFrame->width;
	if( frameHeight ) *frameHeight = pFrame->height;
	if( numFrames ) *numFrames = pSprite->numframes;
}

int R_GetSpriteTexture( const model_t *m_pSpriteModel, int frame )
{
	if( !m_pSpriteModel || m_pSpriteModel->type != mod_sprite || !m_pSpriteModel->cache.data )
		return 0;

	return R_GetSpriteFrame( m_pSpriteModel, frame, 0.0f )->gl_texturenum;
}

/*
=============
R_DrawStretchPic
=============
*/
//void R_DrawStretchPic( float x, float y, float w, float h, float s1, float t1, float s2, float t2, int texnum )
//{
//	GL_Bind( GL_TEXTURE0, texnum );

//	glBegin( GL_QUADS );
//		glTexCoord2f( s1, t1 );
//		glVertex2f( x, y );

//		glTexCoord2f( s2, t1 );
//		glVertex2f( x + w, y );

//		glTexCoord2f( s2, t2 );
//		glVertex2f( x + w, y + h );

//		glTexCoord2f( s1, t2 );
//		glVertex2f( x, y + h );
//	glEnd();
//}

///*
//=============
//Draw_TileClear

//This repeats a 64*64 tile graphic to fill the screen around a sized down
//refresh window.
//=============
//*/
//void R_DrawTileClear( int x, int y, int w, int h )
//{
//	float		tw, th;
//	gltexture_t	*glt;

//	GL_SetRenderMode( kRenderNormal );
//	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
//	GL_Bind( GL_TEXTURE0, cls.tileImage );

//	glt = R_GetTexture( cls.tileImage );
//	tw = glt->srcWidth;
//	th = glt->srcHeight;

//	glBegin( GL_QUADS );
//		glTexCoord2f( (float)x / tw, (float)y / th );
//		glVertex2f( (float)x, (float)y );
//		glTexCoord2f((float)(x + w) / tw, (float)y / th );
//		glVertex2f( (float)(x + w), (float)y );
//		glTexCoord2f((float)(x + w) / tw, (float)(y + h) / th );
//		glVertex2f( (float)(x + w), (float)(y + h) );
//		glTexCoord2f( (float)x / tw, (float)(y + h) / th );
//		glVertex2f( (float)x, (float)(y + h) );
//	glEnd ();
//}

///*
//=============
//R_DrawStretchRaw
//=============
//*/
//void R_DrawStretchRaw( float x, float y, float w, float h, int cols, int rows, const byte *data, qboolean dirty )
//{
//	byte		*raw = NULL;
//	gltexture_t	*tex;

//	if( !GL_Support( GL_ARB_TEXTURE_NPOT_EXT ))
//	{
//		int	width = 1, height = 1;

//		// check the dimensions
//		width = NearestPOW( cols, true );
//		height = NearestPOW( rows, false );

//		if( cols != width || rows != height )
//		{
//			raw = GL_ResampleTexture( data, cols, rows, width, height, false );
//			cols = width;
//			rows = height;
//		}
//	}
//	else
//	{
//		raw = (byte *)data;
//	}

//	if( cols > glConfig.max_2d_texture_size )
//		Host_Error( "R_DrawStretchRaw: size %i exceeds hardware limits\n", cols );
//	if( rows > glConfig.max_2d_texture_size )
//		Host_Error( "R_DrawStretchRaw: size %i exceeds hardware limits\n", rows );

//	glDisable( GL_BLEND );
//	glDisable( GL_ALPHA_TEST );
//	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

//	tex = R_GetTexture( tr.cinTexture );
//	GL_Bind( GL_TEXTURE0, tr.cinTexture );

//	if( cols == tex->width && rows == tex->height )
//	{
//		if( dirty )
//		{
//			glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, cols, rows, GL_BGRA, GL_UNSIGNED_BYTE, raw );
//		}
//	}
//	else
//	{
//		tex->width = cols;
//		tex->height = rows;
//		if( dirty )
//		{
//			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, cols, rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, raw );
//		}
//	}

//	glBegin( GL_QUADS );
//	glTexCoord2f( 0, 0 );
//	glVertex2f( x, y );
//	glTexCoord2f( 1, 0 );
//	glVertex2f( x + w, y );
//	glTexCoord2f( 1, 1 );
//	glVertex2f( x + w, y + h );
//	glTexCoord2f( 0, 1 );
//	glVertex2f( x, y + h );
//	glEnd();
//}

///*
//=============
//R_UploadStretchRaw
//=============
//*/
//void R_UploadStretchRaw( int texture, int cols, int rows, int width, int height, const byte *data )
//{
//	byte		*raw = NULL;
//	gltexture_t	*tex;

//	if( !GL_Support( GL_ARB_TEXTURE_NPOT_EXT ))
//	{
//		// check the dimensions
//		width = NearestPOW( width, true );
//		height = NearestPOW( height, false );
//	}
//	else
//	{
//		width = bound( 128, width, glConfig.max_2d_texture_size );
//		height = bound( 128, height, glConfig.max_2d_texture_size );
//	}

//	if( cols != width || rows != height )
//	{
//		raw = GL_ResampleTexture( data, cols, rows, width, height, false );
//		cols = width;
//		rows = height;
//	}
//	else
//	{
//		raw = (byte *)data;
//	}

//	if( cols > glConfig.max_2d_texture_size )
//		Host_Error( "R_UploadStretchRaw: size %i exceeds hardware limits\n", cols );
//	if( rows > glConfig.max_2d_texture_size )
//		Host_Error( "R_UploadStretchRaw: size %i exceeds hardware limits\n", rows );

//	tex = R_GetTexture( texture );
//	GL_Bind( GL_KEEP_UNIT, texture );
//	tex->width = cols;
//	tex->height = rows;

//	glTexImage2D( GL_TEXTURE_2D, 0, tex->format, cols, rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, raw );
//	GL_TexFilter( tex, false );
//}

///*
//===============
//R_Set2DMode
//===============
//*/
//void R_Set2DMode( qboolean enable )
//{
//	if( enable )
//	{
//		if( glState.in2DMode )
//			return;

//		// set 2D virtual screen size
//		glViewport( 0, 0, glState.width, glState.height );
//		glMatrixMode( GL_PROJECTION );
//		glLoadIdentity();
//		glOrtho( 0, glState.width, glState.height, 0, -99999, 99999 );
//		glMatrixMode( GL_MODELVIEW );
//		glLoadIdentity();

//		GL_Cull( 0 );

//		glDepthMask( GL_FALSE );
//		glDisable( GL_DEPTH_TEST );
//		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

//		glState.in2DMode = true;
//		RI.currententity = NULL;
//		RI.currentmodel = NULL;
//	}
//	else
//	{
//		glDepthMask( GL_TRUE );
//		glEnable( GL_DEPTH_TEST );
//		glState.in2DMode = false;

//		glMatrixMode( GL_PROJECTION );
//		GL_LoadMatrix( RI.projectionMatrix );

//		glMatrixMode( GL_MODELVIEW );
//		GL_LoadMatrix( RI.worldviewMatrix );

//	}
//}
