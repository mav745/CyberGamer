/*
cl_menu.c - menu dlls interaction
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
#include "const.h"
#include "library.h"
#include "input.h"

static MENUAPI	GetMenuAPI;
static void __cdecl UI_UpdateUserinfo( void );

menu_static_t	menu;

void __cdecl UI_UpdateMenu( float realtime )
{
	if( !menu.hInstance ) return;
	menu.dllFuncs.pfnRedraw( realtime );
	UI_UpdateUserinfo();
}

void __cdecl UI_KeyEvent( int key, qboolean down )
{
	if( !menu.hInstance ) return;
	menu.dllFuncs.pfnKeyEvent( key, down );
}

void __cdecl UI_MouseMove( int x, int y )
{
	if( !menu.hInstance ) return;
	menu.dllFuncs.pfnMouseMove( x, y );
}

void __cdecl UI_SetActiveMenu( qboolean fActive )
{
	movie_state_t	*cin_state;

	if( !menu.hInstance )
	{
		if( !fActive )
			Key_SetKeyDest( key_game );
		return;
	}

	menu.drawLogo = fActive;
	menu.dllFuncs.pfnSetActiveMenu( fActive );

	if( !fActive )
	{
		// close logo when menu is shutdown
		cin_state = AVI_GetState( CIN_LOGO );
		AVI_CloseVideo( cin_state );
	}
}

void __cdecl UI_AddServerToList( netadr_t adr, const char *info )
{
	if( !menu.hInstance ) return;
	menu.dllFuncs.pfnAddServerToList( adr, info );
}

void __cdecl UI_GetCursorPos( int *pos_x, int *pos_y )
{
	if( !menu.hInstance ) return;
	menu.dllFuncs.pfnGetCursorPos( pos_x, pos_y );
}

void __cdecl UI_SetCursorPos( int pos_x, int pos_y )
{
	if( !menu.hInstance ) return;
	menu.dllFuncs.pfnSetCursorPos( pos_x, pos_y );
}

void __cdecl UI_ShowCursor( qboolean show )
{
	if( !menu.hInstance ) return;
	menu.dllFuncs.pfnShowCursor( show );
}

qboolean __cdecl UI_CreditsActive( void )
{
	if( !menu.hInstance ) return 0;
	return menu.dllFuncs.pfnCreditsActive();
}

void __cdecl UI_CharEvent( int key )
{
	if( !menu.hInstance ) return;
	menu.dllFuncs.pfnCharEvent( key );
}

qboolean __cdecl UI_MouseInRect( void )
{
	if( !menu.hInstance ) return 1;
	return menu.dllFuncs.pfnMouseInRect();
}

qboolean __cdecl UI_IsVisible( void )
{
	if( !menu.hInstance ) return 0;
	return menu.dllFuncs.pfnIsVisible();
}

static void __cdecl UI_DrawLogo( const char *filename, float x, float y, float width, float height )
{
	static float	cin_time;
	static int	last_frame = -1;
	byte		*cin_data = NULL;
	movie_state_t	*cin_state;
	int		cin_frame;
	qboolean		redraw = false;

	if( !menu.drawLogo ) return;
	cin_state = AVI_GetState( CIN_LOGO );

	if( !AVI_IsActive( cin_state ))
	{
		string		path;
		const char	*fullpath;

		// run cinematic if not
		Q_snprintf( path, sizeof( path ), "media/%s", filename );
		FS_DefaultExtension( path, ".avi" );
		fullpath = FS_GetDiskPath( path, false );

		if( FS_FileExists( path, false ) && !fullpath )
		{
			MsgDev( D_ERROR, "Couldn't load %s from packfile. Please extract it\n", path );
			menu.drawLogo = false;
			return;
		}

		AVI_OpenVideo( cin_state, fullpath, false, false, true );
		if( !( AVI_GetVideoInfo( cin_state, &menu.logo_xres, &menu.logo_yres, &menu.logo_length )))
		{
			AVI_CloseVideo( cin_state );
			menu.drawLogo = false;
			return;
		}

		cin_time = 0.0f;
		last_frame = -1;
	}

	if( width <= 0 || height <= 0 )
	{
		// precache call, don't draw
		cin_time = 0.0f;
		last_frame = -1;
		return;
	}

	// advances cinematic time (ignores maxfps and host_framerate settings)
	cin_time += (float)host.realframetime;

	// restarts the cinematic
	if( cin_time > menu.logo_length )
		cin_time = 0.0f;

	// read the next frame
	cin_frame = AVI_GetVideoFrameNumber( cin_state, cin_time );

	if( cin_frame != last_frame )
	{
		cin_data = AVI_GetVideoFrame( cin_state, cin_frame );
		last_frame = cin_frame;
		redraw = true;
	}

	R_DrawStretchRaw( x, y, width, height, menu.logo_xres, menu.logo_yres, cin_data, redraw );
}

static int __cdecl UI_GetLogoWidth( void )
{
	return menu.logo_xres;
}

static int __cdecl UI_GetLogoHeight( void )
{
	return menu.logo_yres;
}

static float __cdecl UI_GetLogoLength( void )
{
	return menu.logo_length;
}

static void __cdecl UI_UpdateUserinfo( void )
{
	player_info_t	*player;

	if( !userinfo->modified ) return;
	player = &menu.playerinfo;

	Q_strncpy( player->userinfo, Cvar_Userinfo(), sizeof( player->userinfo ));
	Q_strncpy( player->name, Info_ValueForKey( player->userinfo, "name" ), sizeof( player->name ));
	Q_strncpy( player->model, Info_ValueForKey( player->userinfo, "model" ), sizeof( player->model ));
	player->topcolor = Q_atoi( Info_ValueForKey( player->userinfo, "topcolor" ));
	player->bottomcolor = Q_atoi( Info_ValueForKey( player->userinfo, "bottomcolor" ));
}

void __cdecl Host_Credits( void )
{
	if( !menu.hInstance ) return;
	menu.dllFuncs.pfnFinalCredits();
}

static void __cdecl UI_ConvertGameInfo( GAMEINFO *out, gameinfo_t *in )
{
	Q_strncpy( out->gamefolder, in->gamefolder, sizeof( out->gamefolder ));
	Q_strncpy( out->startmap, in->startmap, sizeof( out->startmap ));
	Q_strncpy( out->trainmap, in->trainmap, sizeof( out->trainmap ));
	Q_strncpy( out->title, in->title, sizeof( out->title ));
	Q_strncpy( out->version, va( "%g", in->version ), sizeof( out->version ));

	Q_strncpy( out->game_url, in->game_url, sizeof( out->game_url ));
	Q_strncpy( out->update_url, in->update_url, sizeof( out->update_url ));
	Q_strncpy( out->size, Q_pretifymem( (float)in->size, 0 ), sizeof( out->size ));
	Q_strncpy( out->type, in->type, sizeof( out->type ));
	Q_strncpy( out->date, in->date, sizeof( out->date ));

	out->gamemode = in->gamemode;

	if( in->nomodels )
		out->flags |= GFL_NOMODELS;
}

qboolean __cdecl PIC_Scissor( float *x, float *y, float *width, float *height, float *u0, float *v0, float *u1, float *v1 )
{
	float	dudx, dvdy;

	// clip sub rect to sprite
	if(( width == 0 ) || ( height == 0 ))
		return false;

	if( *x + *width <= menu.ds.scissor_x )
		return false;
	if( *x >= menu.ds.scissor_x + menu.ds.scissor_width )
		return false;
	if( *y + *height <= menu.ds.scissor_y )
		return false;
	if( *y >= menu.ds.scissor_y + menu.ds.scissor_height )
		return false;

	dudx = (*u1 - *u0) / *width;
	dvdy = (*v1 - *v0) / *height;

	if( *x < menu.ds.scissor_x )
	{
		*u0 += (menu.ds.scissor_x - *x) * dudx;
		*width -= menu.ds.scissor_x - *x;
		*x = (float)menu.ds.scissor_x;
	}

	if( *x + *width > menu.ds.scissor_x + menu.ds.scissor_width )
	{
		*u1 -= (*x + *width - (menu.ds.scissor_x + menu.ds.scissor_width)) * dudx;
		*width = menu.ds.scissor_x + menu.ds.scissor_width - *x;
	}

	if( *y < menu.ds.scissor_y )
	{
		*v0 += (menu.ds.scissor_y - *y) * dvdy;
		*height -= menu.ds.scissor_y - *y;
		*y = (float)menu.ds.scissor_y;
	}

	if( *y + *height > menu.ds.scissor_y + menu.ds.scissor_height )
	{
		*v1 -= (*y + *height - (menu.ds.scissor_y + menu.ds.scissor_height)) * dvdy;
		*height = menu.ds.scissor_y + menu.ds.scissor_height - *y;
	}
	return true;
}

/*
===============================================================================
	MainUI Builtin Functions

===============================================================================
*/
/*
=========
pfnPIC_Load

=========
*/
static HIMAGE __cdecl pfnPIC_Load( const char *szPicName, const byte *image_buf, long image_size, long flags )
{
	HIMAGE	tx;

	if( !szPicName || !*szPicName )
	{
		MsgDev( D_ERROR, "CL_LoadImage: bad name!\n" );
		return 0;
	}

	// add default parms to image
	flags |= TF_IMAGE;

	host.decal_loading = true; // allow decal images for menu
	tx = GL_LoadTexture( szPicName, image_buf, image_size, flags, NULL );
	host.decal_loading = false;

	return tx;
}

/*
=========
pfnPIC_Width

=========
*/
static int __cdecl pfnPIC_Width( HIMAGE hPic )
{
	int	picWidth;

	R_GetTextureParms( &picWidth, NULL, hPic );

	return picWidth;
}

/*
=========
pfnPIC_Height

=========
*/
static int __cdecl pfnPIC_Height( HIMAGE hPic )
{
	int	picHeight;

	R_GetTextureParms( NULL, &picHeight, hPic );

	return picHeight;
}

/*
=========
pfnPIC_Draw

=========
*/
void __cdecl pfnPIC_Draw( int x, int y, int width, int height, const wrect_t *prc )
{
	GL_SetRenderMode( kRenderNormal );
	PIC_DrawGeneric( (float)x, (float)y, (float)width, (float)height, prc );
}

/*
=========
pfnPIC_DrawTrans

=========
*/
void __cdecl pfnPIC_DrawTrans( int x, int y, int width, int height, const wrect_t *prc )
{
	GL_SetRenderMode( kRenderTransTexture );
	PIC_DrawGeneric( (float)x, (float)y, (float)width, (float)height, prc );
}

/*
=========
pfnPIC_DrawHoles

=========
*/
void __cdecl pfnPIC_DrawHoles( int x, int y, int width, int height, const wrect_t *prc )
{
	GL_SetRenderMode( kRenderTransAlpha );
	PIC_DrawGeneric( (float)x, (float)y, (float)width, (float)height, prc );
}

/*
=========
pfnPIC_DrawAdditive

=========
*/
void __cdecl pfnPIC_DrawAdditive( int x, int y, int width, int height, const wrect_t *prc )
{
	GL_SetRenderMode( kRenderTransAdd );
	PIC_DrawGeneric( (float)x, (float)y, (float)width, (float)height, prc );
}

/*
=========
pfnPIC_EnableScissor

=========
*/
static void __cdecl pfnPIC_EnableScissor( int x, int y, int width, int height )
{
	// check bounds
	x = bound( 0, x, menu.globals->scrWidth );
	y = bound( 0, y, menu.globals->scrHeight );
	width = bound( 0, width, menu.globals->scrWidth - x );
	height = bound( 0, height, menu.globals->scrHeight - y );

	menu.ds.scissor_x = x;
	menu.ds.scissor_width = width;
	menu.ds.scissor_y = y;
	menu.ds.scissor_height = height;
	menu.ds.scissor_test = true;
}

/*
=========
pfnPIC_DisableScissor

=========
*/
static void __cdecl pfnPIC_DisableScissor( void )
{
	menu.ds.scissor_x = 0;
	menu.ds.scissor_width = 0;
	menu.ds.scissor_y = 0;
	menu.ds.scissor_height = 0;
	menu.ds.scissor_test = false;
}

/*
=============
pfnClientCmd

=============
*/
static void __cdecl pfnClientCmd( int exec_now, const char *szCmdString )
{
	if( !szCmdString || !szCmdString[0] )
		return;

	Cbuf_AddText( szCmdString );
	Cbuf_AddText( "\n" );

	// client command executes immediately
	if( exec_now ) Cbuf_Execute();
}

/*
=============
pfnPlaySound

=============
*/
static void __cdecl pfnPlaySound( const char *szSound )
{
	if( !szSound || !*szSound ) return;
	S_StartLocalSound( szSound, VOL_NORM, false );
}

/*
=============
UI_DrawConsoleString

drawing string like a console string
=============
*/
static int __cdecl UI_DrawConsoleString( int x, int y, const char *string )
{
	int	drawLen;

	if( !string || !*string ) return 0; // silent ignore
	drawLen = Con_DrawString( x, y, string, menu.ds.textColor );
	MakeRGBA( menu.ds.textColor, 255, 255, 255, 255 );

	return (x + drawLen); // exclude color prexfixes
}

/*
=============
pfnDrawSetTextColor

set color for anything
=============
*/
static void __cdecl UI_DrawSetTextColor( int r, int g, int b, int alpha )
{
	// bound color and convert to byte
	menu.ds.textColor[0] = r;
	menu.ds.textColor[1] = g;
	menu.ds.textColor[2] = b;
	menu.ds.textColor[3] = alpha;
}

/*
====================
pfnGetPlayerModel

for drawing playermodel previews
====================
*/
static cl_entity_t* __cdecl pfnGetPlayerModel( void )
{
	return &menu.playermodel;
}

/*
====================
pfnSetPlayerModel

for drawing playermodel previews
====================
*/
static void __cdecl pfnSetPlayerModel( cl_entity_t *ent, const char *path )
{
	Mod_RegisterModel( path, MAX_MODELS - 1 );
	ent->curstate.modelindex = MAX_MODELS - 1;
	ent->model = Mod_Handle( MAX_MODELS - 1 );
}

/*
====================
pfnRenderScene

for drawing playermodel previews
====================
*/
static void __cdecl pfnRenderScene( const ref_params_t *fd )
{
	// to avoid division by zero
	if( !fd || fd->fov_x <= 0.0f || fd->fov_y <= 0.0f )
		return;
	R_RenderFrame( fd, false );
}

/*
====================
pfnClientJoin

send client connect
====================
*/
static void __cdecl pfnClientJoin( const netadr_t adr )
{
	Cbuf_AddText( va( "connect %s\n", NET_AdrToString( adr )));
}

/*
====================
pfnKeyGetOverstrikeMode

get global key overstrike state
====================
*/
static int __cdecl pfnKeyGetOverstrikeMode( void )
{
	return host.key_overstrike;
}

/*
====================
pfnKeySetOverstrikeMode

set global key overstrike mode
====================
*/
static void __cdecl pfnKeySetOverstrikeMode( int fActive )
{
	host.key_overstrike = fActive;
}

/*
====================
pfnKeyGetState

returns kbutton struct if found
====================
*/
static void *__cdecl pfnKeyGetState( const char *name )
{
	if( clgame.dllFuncs.KB_Find )
		return clgame.dllFuncs.KB_Find( name );
	return NULL;
}

/*
=========
pfnMemAlloc

=========
*/
static void *__cdecl pfnMemAlloc( size_t cb, const char *filename, const int fileline )
{
	return _Mem_Alloc( menu.mempool, cb, filename, fileline );
}

/*
=========
pfnMemFree

=========
*/
static void pfnMemFree( void *mem, const char *filename, const int fileline )
{
	_Mem_Free( mem, filename, fileline );
}

/*
=========
pfnGetGameInfo

=========
*/
static int pfnGetGameInfo( GAMEINFO *pgameinfo )
{
	if( !pgameinfo ) return 0;

	*pgameinfo = menu.gameInfo;
	return 1;
}

/*
=========
pfnGetGamesList

=========
*/
static GAMEINFO **pfnGetGamesList( int *numGames )
{
	if( numGames ) *numGames = SI.numgames;
	return menu.modsInfo;
}

/*
=========
pfnGetFilesList

release prev search on a next call
=========
*/
static char **pfnGetFilesList( const char *pattern, int *numFiles, int gamedironly )
{
	static search_t	*t = NULL;

	if( t ) Mem_Free( t ); // release prev search

	t = FS_Search( pattern, true, gamedironly );
	if( !t )
	{
		if( numFiles ) *numFiles = 0;
		return NULL;
	}

	if( numFiles ) *numFiles = t->numfilenames;
	return t->filenames;
}

/*
=========
pfnGetClipboardData

pointer must be released in call place
=========
*/
static char *pfnGetClipboardData( void )
{
	return Sys_GetClipboardData();
}

/*
=========
pfnCheckGameDll

=========
*/
int pfnCheckGameDll( void )
{
	void	*hInst;

	if( SV_Active( )) return true;

	if(( hInst = Com_LoadLibrary( GI->game_dll, true )) != NULL )
	{
		Com_FreeLibrary( hInst );
		return true;
	}
	return false;
}

/*
=========
pfnChangeInstance

=========
*/
static void pfnChangeInstance( const char *newInstance, const char *szFinalMessage )
{
	if( !szFinalMessage ) szFinalMessage = "";
	if( !newInstance || !*newInstance ) return;

	Host_NewInstance( newInstance, szFinalMessage );
}

/*
=========
pfnHostNewGame

=========
*/
static void pfnHostNewGame( const char *mapName )
{
	if( !mapName || !*mapName ) return;
	Host_NewGame( mapName, false );
}

/*
=========
pfnHostEndGame

=========
*/
static void pfnHostEndGame( const char *szFinalMessage )
{
	if( !szFinalMessage ) szFinalMessage = "";
	Host_EndGame( szFinalMessage );
}

/*
=========
pfnStartBackgroundTrack

=========
*/
static void pfnStartBackgroundTrack( const char *introTrack, const char *mainTrack )
{
	S_StartBackgroundTrack( introTrack, mainTrack, 0 );

	// HACKHACK to remove glitches from background track while new game is started.
	if( !introTrack && !mainTrack )
	{
		S_Activate( 0, host.hWnd );
		S_Activate( 1, host.hWnd );
	}
}

// engine callbacks
static ui_enginefuncs_t gEngfuncs =
{
	pfnPIC_Load,
	GL_FreeImage,
	pfnPIC_Width,
	pfnPIC_Height,
	pfnPIC_Set,
	pfnPIC_Draw,
	pfnPIC_DrawHoles,
	pfnPIC_DrawTrans,
	pfnPIC_DrawAdditive,
	pfnPIC_EnableScissor,
	pfnPIC_DisableScissor,
	pfnFillRGBA_menu,
	pfnCvar_RegisterVariable,
	Cvar_VariableValue,
	Cvar_VariableString,
	Cvar_Set,
	Cvar_SetFloat,
	pfnAddClientCommand,
	pfnClientCmd,
	Cmd_RemoveCommand,
	Cmd_Argc,
	Cmd_Argv,
	Cmd_Args,
	Con_Printf,
	Con_DPrintf,
	UI_NPrintf,
	UI_NXPrintf,
	pfnPlaySound,
	UI_DrawLogo,
	UI_GetLogoWidth,
	UI_GetLogoHeight,
	UI_GetLogoLength,
	pfnDrawCharacterQ,
	UI_DrawConsoleString,
	UI_DrawSetTextColor,
	Con_DrawStringLen,
	Con_DefaultColor,
	pfnGetPlayerModel,
	pfnSetPlayerModel,
	R_ClearScene,
	pfnRenderScene,
	CL_AddEntity,
	Host_Error,
	FS_FileExists,
	pfnGetGameDir,
	Cmd_CheckMapsList,
	CL_Active,
	pfnClientJoin,
	COM_LoadFileForMe,
	COM_ParseFile,
	COM_FreeFile,
	Key_ClearStates,
	Key_SetKeyDest,
	Key_KeynumToString,
	Key_GetBinding,
	Key_SetBinding,
	Key_IsDown,
	pfnKeyGetOverstrikeMode,
	pfnKeySetOverstrikeMode,
	pfnKeyGetState,
	pfnMemAlloc,
	pfnMemFree,
	pfnGetGameInfo,
	pfnGetGamesList,
	pfnGetFilesList,
	SV_GetComment,
	CL_GetComment,
	pfnCheckGameDll,
	pfnGetClipboardData,
	Sys_ShellExecute,
	Host_WriteServerConfig,
	pfnChangeInstance,
	pfnStartBackgroundTrack,
	pfnHostEndGame,
	Com_RandomFloat,
	Com_RandomLong,
	IN_SetCursor,
	pfnIsMapValid,
	GL_ProcessTexture,
	COM_CompareFileTime,
};

void UI_UnloadProgs( void )
{
	if( !menu.hInstance ) return;

	// deinitialize game
	menu.dllFuncs.pfnShutdown();

	Com_FreeLibrary( menu.hInstance );
	Mem_FreePool( &menu.mempool );
	Q_memset( &menu, 0, sizeof( menu ));
}

qboolean UI_LoadProgs( void )
{
	static ui_enginefuncs_t	gpEngfuncs;
	static ui_globalvars_t	gpGlobals;
	int			i;

	if( menu.hInstance ) UI_UnloadProgs();

	// setup globals
	menu.globals = &gpGlobals;

	if(!( menu.hInstance = Com_LoadLibrary( va( "%s/menu.dll", GI->dll_path ), false )))
	{
		FS_AllowDirectPaths( true );

		if(!( menu.hInstance = Com_LoadLibrary( "../menu.dll", false )))
		{
			FS_AllowDirectPaths( false );
			return false;
		}

		FS_AllowDirectPaths( false );
	}

	if(!( GetMenuAPI = (MENUAPI)Com_GetProcAddress( menu.hInstance, "GetMenuAPI" )))
	{
		Com_FreeLibrary( menu.hInstance );
		MsgDev( D_NOTE, "UI_LoadProgs: can't init menu API\n" );
		menu.hInstance = NULL;
		return false;
	}

	// make local copy of engfuncs to prevent overwrite it with user dll
	Q_memcpy( &gpEngfuncs, &gEngfuncs, sizeof( gpEngfuncs ));

	menu.mempool = Mem_AllocPool( "Menu Pool" );

	if( !GetMenuAPI( &menu.dllFuncs, &gpEngfuncs, menu.globals ))
	{
		Com_FreeLibrary( menu.hInstance );
		MsgDev( D_NOTE, "UI_LoadProgs: can't init menu API\n" );
		Mem_FreePool( &menu.mempool );
		menu.hInstance = NULL;
		return false;
	}

	// setup gameinfo
	for( i = 0; i < SI.numgames; i++ )
	{
		menu.modsInfo[i] = Mem_Alloc( menu.mempool, sizeof( GAMEINFO ));
		UI_ConvertGameInfo( menu.modsInfo[i], SI.games[i] );
	}

	UI_ConvertGameInfo( &menu.gameInfo, SI.GameInfo ); // current gameinfo

	// setup globals
	menu.globals->developer = host.developer;

	// initialize game
	menu.dllFuncs.pfnInit();

	return true;
}
