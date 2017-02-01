/*
game.cpp -- executable to run Xash Engine
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

#include "game.hpp"

#include <windows.h>
#include <QDebug>

#define GAME_PATH	"valve"	// default dir to start from

#pragma warning(disable : 4100)
#pragma warning(disable : 4996)

typedef void (__cdecl *pfnChangeGame)( const char *progname );
typedef int (__cdecl *pfnInit)( const char *progname, int bChangeGame, pfnChangeGame func );
typedef void (__cdecl *pfnShutdown)( void );
typedef int (__cdecl *pfnIterate)( void );
pfnInit Host_Main;
pfnIterate Host_Iterate;
pfnShutdown Host_Shutdown = NULL;
char szGameDir[128]; // safe place to keep gamedir
HINSTANCE	hEngine;

void __cdecl Sys_Error( const char *errorstring )
{
	MessageBox( NULL, errorstring, "Xash Error", MB_OK|MB_SETFOREGROUND|MB_ICONSTOP );
	exit( 1 );
}

void __cdecl Sys_LoadEngine( void )
{
	if(( hEngine = LoadLibrary( "xash.dll" )) == NULL )
	{
		Sys_Error( "Unable to load the xash.dll" );
	}

	if(( Host_Main = (pfnInit)GetProcAddress( hEngine, "Host_Main" )) == NULL )
	{
		Sys_Error( "xash.dll missed 'Host_Main' export" );
	}

	if(( Host_Iterate = (pfnIterate)GetProcAddress( hEngine, "Host_Iterate" )) == NULL )
	{
		Sys_Error( "xash.dll missed 'Host_Iterate' export" );
	}

	// this is non-fatal for us but change game will not working
	Host_Shutdown = (pfnShutdown)GetProcAddress( hEngine, "Host_Shutdown" );
}

void __cdecl Sys_UnloadEngine( void )
{
	if( Host_Shutdown ) Host_Shutdown( );
	if( hEngine ) FreeLibrary( hEngine );
}

void __cdecl Sys_ChangeGame( const char *progname )
{
	if( !progname || !progname[0] ) Sys_Error( "Sys_ChangeGame: NULL gamedir" );
	if( Host_Shutdown == NULL ) Sys_Error( "Sys_ChangeGame: missed 'Host_Shutdown' export\n" );
	strncpy( szGameDir, progname, sizeof( szGameDir ) - 1 );

	Sys_UnloadEngine ();
	Sys_LoadEngine ();

	Host_Main( szGameDir, TRUE, ( Host_Shutdown != NULL ) ? Sys_ChangeGame : NULL );
}

//int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
//{
//	Sys_LoadEngine();

//	return Host_Main( GAME_PATH, FALSE, ( Host_Shutdown != NULL ) ? Sys_ChangeGame : NULL );
//}



void Game::timerEvent(QTimerEvent *te)
{
	if(!Host_Iterate())
		theTimer.stop();
}


Game::Game()
{
	Sys_LoadEngine();
	Host_Main( GAME_PATH, FALSE, ( Host_Shutdown != NULL ) ? Sys_ChangeGame : NULL );
	theTimer.start(5, Qt::PreciseTimer, this);
}


Game::~Game()
{

}
