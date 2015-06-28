/*
host.c - dedicated and normal host
Copyright (C) 2007 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

//#include "netchan.h"
//#include "protocol.h"
//#include "mod_local.h"
//#include "mathlib.h"
//#include "input.h"
//#include "features.h"
//#include "render_api.h"	// decallist_t

#include "xash.hpp"
#include "console2.hpp"
#include "cmd2.hpp"
#include "cvar2.hpp"
#include "fs.hpp"

#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>

// these cvars will be duplicated on each client across network
int CHost::serverState()
{
	return 0;
//	return Cvar_VariableInteger("CHost::serverstate");
}

int CHost::compareFileTime(long ft1, long ft2)
{
	if(ft1 < ft2)
	{
		return -1;
	}
	else if(ft1 > ft2)
	{
		return 1;
	}
	return 0;
}

void CHost::shutdownServer()
{
//	if(!SV_Active()) return;
//	Q_strncpy(host.finalmsg, "Server was killed", MAX_STRING);
//	SV_Shutdown(false);
}

void CHost::printEngineFeatures()
{
//	if(host.features & ENGINE_WRITE_LARGE_COORD)
//		MsgDev(D_AICONSOLE, "^3EXT:^7 big world support enabled\n");

//	if(host.features & ENGINE_BUILD_SURFMESHES)
//		MsgDev(D_AICONSOLE, "^3EXT:^7 surfmeshes enabled\n");

//	if(host.features & ENGINE_LOAD_DELUXEDATA)
//		MsgDev(D_AICONSOLE, "^3EXT:^7 deluxemap support enabled\n");

//	if(host.features & ENGINE_TRANSFORM_TRACE_AABB)
//		MsgDev(D_AICONSOLE, "^3EXT:^7 Transform trace AABB enabled\n");

//	if(host.features & ENGINE_LARGE_LIGHTMAPS)
//		MsgDev(D_AICONSOLE, "^3EXT:^7 Large lightmaps enabled\n");

//	if(host.features & ENGINE_COMPENSATE_QUAKE_BUG)
//		MsgDev(D_AICONSOLE, "^3EXT:^7 Compensate quake bug enabled\n");
}

bool CHost::newGame(const  QString &mapName, bool loadGame)
{
	return false;
	//return SV_NewGame(mapName, loadGame);
}

void CHost::endGame(const  QString &message, ...)
{
//	va_list argptr;
//	static char string[MAX_SYSPATH];

//	va_start(argptr, message);
//	Q_vsprintf(string, message, argptr);
//	va_end(argptr);

//	MsgDev(D_INFO, "CHost::EndGame: %s\n", string);

//	if(SV_Active())
//	{
//		Q_snprintf(host.finalmsg, sizeof(host.finalmsg), "CHost::EndGame: %s", string);
//		SV_Shutdown(false);
//	}

//	if(host.type == HOST_DEDICATED)
//		Sys_Break("CHost::endGame: %s\n", string); // dedicated servers exit

//	SV_Shutdown(false);
//	CL_Disconnect();

//	// recreate world if needs
//	CL_ClearEdicts ();

//	// release all models
//	Mod_ClearAll(true);

//	CHost::abortCurrentFrame ();
}

// aborts the current host frame and goes on with the next one
void CHost::abortCurrentFrame()
{
//	longjmp(host.abortframe, 1);
}

void CHost::setServerState(int state)
{
//	Cvar_FullSet("CHost::serverstate", va("%i", state), CVAR_INIT);
}

void CHost::newInstance(const QString &name, const  QString &finalmsg)
{
//	if(!pChangeGame) return;

//	host.change_game = true;
//	Q_strncpy(host.finalmsg, finalmsg, sizeof(host.finalmsg));
//	pChangeGame(name); // call from hl.exe
}


bool CHost::isLocalGame()
{
//	if(CL_Active() && SV_Active() && CL_GetMaxClients() == 1)
//		return true;
	return false;
}

bool CHost::registerDecal(const  QString &name)
{
//	char shortname[CS_SIZE];
//	int i;

//	if(!name || !name[0])
//		return 0;

//	FS_FileBase(name, shortname);

//	for(i = 1; i < MAX_DECALS && host.draw_decals[i][0]; i++)
//	{
//		if(!Q_stricmp(host.draw_decals[i], shortname))
//			return true;
//	}

//	if(i == MAX_DECALS)
//	{
//		MsgDev(D_ERROR, "CHost::RegisterDecal: MAX_DECALS limit exceeded\n");
//		return false;
//	}

//	// register new decal
//	Q_strncpy(host.draw_decals[i], shortname, sizeof(host.draw_decals[i]));
//	num_decals++;

	return true;
}

void CHost::initDecals()
{
//	search_t *t;
//	int i;

//	Q_memset(host.draw_decals, 0, sizeof(host.draw_decals));
//	num_decals = 0;

//	// lookup all decals in decals.wad
//	t = FS_Search("decals.wad/*.*", true, false);

//	for(i = 0; t && i < t->numfilenames; i++)
//	{
//		if(!CHost::registerDecal(t->filenames[i]))
//			break;
//	}

//	if(t) Mem_Free(t);
//	MsgDev(D_NOTE, "InitDecals: %i decals\n", num_decals);
}

// Write ambient sounds into demo
void CHost::restartAmbientSounds()
{
//	soundlist_t soundInfo[64];
//	string curtrack, looptrack;
//	int i, nSounds;
//	fs_offset_t position;

//	if(!SV_Active())
//	{
//		return;
//	}

//	nSounds = S_GetCurrentStaticSounds(soundInfo, 64);

//	for(i = 0; i < nSounds; i++)
//	{
//		if(!soundInfo[i].looping || soundInfo[i].entnum == -1)
//			continue;

//		MsgDev(D_NOTE, "Restarting sound %s...\n", soundInfo[i].name);
//		S_StopSound(soundInfo[i].entnum, soundInfo[i].channel, soundInfo[i].name);
//		SV_StartSound(pfnPEntityOfEntIndex(soundInfo[i].entnum), CHAN_STATIC, soundInfo[i].name,
//		soundInfo[i].volume, soundInfo[i].attenuation, 0, soundInfo[i].pitch);
//	}

//	// restart soundtrack
//	if(S_StreamGetCurrentState(curtrack, looptrack, reinterpret_cast<int*>(&position)))
//	{
//		SV_StartMusic(curtrack, looptrack, position);
//	}
}

// Write all the decals into demo
void CHost::restartDecals()
{
//	decallist_t *entry;
//	int decalIndex;
//	int modelIndex;
//	sizebuf_t *msg;
//	int i;

//	if(!SV_Active())
//	{
//		return;
//	}

//	// g-cont. add space for studiodecals if present
//	host.decalList = (decallist_t *)Z_Malloc(sizeof(decallist_t) * MAX_RENDER_DECALS * 2);
//	host.numdecals = R_CreateDecalList(host.decalList, false);

//	// remove decals from map
//	R_ClearAllDecals();

//	// write decals into reliable datagram
//	msg = SV_GetReliableDatagram();

//	// restore decals and write them into network message
//	for(i = 0; i < host.numdecals; i++)
//	{
//		entry = &host.decalList[i];
//		modelIndex = pfnPEntityOfEntIndex(entry->entityIndex)->v.modelindex;

//		// game override
//		if(SV_RestoreCustomDecal(entry, pfnPEntityOfEntIndex(entry->entityIndex), false))
//			continue;

//		decalIndex = pfnDecalIndex(entry->name);

//		// BSP and studio decals has different messages
//		if(entry->flags & FDECAL_STUDIO)
//		{
//			// NOTE: studio decal trace start saved into impactPlaneNormal
//			SV_CreateStudioDecal(msg, entry->position, entry->impactPlaneNormal, decalIndex, entry->entityIndex,
//			modelIndex, entry->flags, &entry->studio_state);
//		}
//		else
//		{
//			SV_CreateDecal(msg, entry->position, decalIndex, entry->entityIndex, modelIndex, entry->flags, entry->scale);
//		}
//	}

//	Z_Free(host.decalList);
//	host.decalList = NULL;
//	host.numdecals = 0;
}

// Add them exactly as if they had been typed at the console
void CHost::getConsoleCommands()
{
//	 QString &cmd;

//	if(host.type == HOST_DEDICATED)
//	{
//		cmd = Con_Input();
//		if(cmd) Cbuf_AddText(cmd);
//	}
}

// Returns false if the time is too short to run a frame
bool CHost::filterTime(float time)
{
//	static double oldtime;
//	float fps;

//	host.realtime += time;

//	// dedicated's tic_rate regulates server frame rate.  Don't apply fps filter here.
//	fps = host_maxfps->value;

//	if(fps != 0)
//	{
//		float minframetime;

//		// limit fps to withing tolerable range
//		fps = bound(MIN_FPS, fps, MAX_FPS);

//		minframetime = 1.0f / fps;

//		if((host.realtime - oldtime) < minframetime)
//		{
//			// framerate is too high
//			return false;
//		}
//	}

//	host.frametime = host.realtime - oldtime;
//	host.realframetime = bound(MIN_FRAMETIME, host.frametime, MAX_FRAMETIME);
//	oldtime = host.realtime;

//	if(host_framerate->value > 0 && (CHost::isLocalGame()))
//	{
//		float fps = host_framerate->value;
//		if(fps > 1) fps = 1.0f / fps;
//		host.frametime = fps;
//	}
//	else
//	{	// don't allow really long or short frames
//		host.frametime = bound(MIN_FRAMETIME, host.frametime, MAX_FRAMETIME);
//	}

	return true;
}

void CHost::frame(float time)
{
//	if(setjmp(host.abortframe))
//		return;

//	CHost::inputFrame ();	// input frame

//	// decide the simulation time
//	if(!CHost::filterTime(time))
//		return;

//	CHost::getConsoleCommands ();

//	CHost::serverFrame (); // server frame
//	CHost::clientFrame (time); // client frame

//	host.framecount++;
}

// Called every frame, even if not generating commands
void CHost::inputFrame()
{
//	qboolean shutdownMouse = false;

//	rand (); // keep the random time dependent

//	Sys_SendKeyEvents ();



//	if(host.state == HOST_RESTART)
//		host.state = HOST_FRAME; // restart is finished

//	if(host.type == HOST_DEDICATED)
//	{
//		Cbuf_Execute ();
//		// let the dedicated server some sleep
//		Sys_Sleep(1);
//	}
//	else
//	{
//		if(host.state == HOST_NOFOCUS)
//		{
//			if(Host_ServerState() && CL_IsInGame())
//				Sys_Sleep(1); // listenserver
//			else Sys_Sleep(20); // sleep 20 ms otherwise
//		}
//		else if(host.state == HOST_SLEEP)
//		{
//			// completely sleep in minimized state
//			Sys_Sleep(20);
//		}
//	}

//	if(!in_mouseinitialized)
//		return;

//	if(host.state != HOST_FRAME)
//	{
//		IN_DeactivateMouse();
//		return;
//	}

//	if(cl.refdef.paused && cls.key_dest == key_game)
//		shutdownMouse = true; // release mouse during pause or console typeing

//	if(shutdownMouse && !Cvar_VariableInteger("fullscreen"))
//	{
//		IN_DeactivateMouse();
//		return;
//	}

//	IN_ActivateMouse(false);
//	IN_MouseMove();
}

void CHost::serverFrame()
{
	// if server is not active, do nothing
//	if(!svs.initialized) return;

//	svgame.globals->frametime = (float)host.frametime;

//	// check timeouts
//	SV_CheckTimeouts ();

//	// check clients timewindow
//	SV_CheckCmdTimes ();

//	// read packets from clients
//	SV_ReadPackets ();

//	// update ping based on the last known frame from all clients
//	SV_CalcPings ();

//	// refresh serverinfo on the client side
//	SV_UpdateServerInfo ();

//	// refresh physic movevars on the client side
//	SV_UpdateMovevars (false);

//	// let everything in the world think and move
//	SV_RunGameFrame ();

//	// send messages back to the clients that had packets read this frame
//	SV_SendClientMessages ();

//	// clear edict flags for next frame
//	SV_PrepWorldFrame ();

//	// send a heartbeat to the master if needed
//	Master_Heartbeat ();
}

void clientFrame(double frametime)
{
//	// if client is not active, do nothing
//	if(!cls.initialized) return;

//	// decide the simulation time
//	cl.oldtime = cl.time;
//	//cl.time += host.frametime;
//	cl.time = host.realtime;

//	if(menu.hInstance)
//	{
//		// menu time (not paused, not clamped)
//		menu.globals->time = (float)host.realtime;
//		menu.globals->frametime = (float)host.realframetime;
//		menu.globals->demoplayback = cls.demoplayback;
//		menu.globals->demorecording = cls.demorecording;
//	}

//	// if in the debugger last frame, don't timeout
//	if(host.frametime > 5.0f) cls.netchan.last_received = Sys_DoubleTime();

//	VGui_RunFrame ();

//	clgame.dllFuncs.pfnFrame(host.frametime);

//	// fetch results from server
//	CL_ReadPackets();

//	VID_CheckChanges();

//	// allow sound and video DLL change
//	if(cls.state == ca_active)
//	{
//		if(!cl.video_prepped) CL_PrepVideo();
//		if(!cl.audio_prepped) CL_PrepSound();
//	}

//	// send a new command message to the server
//	CL_SendCommand();

//	// predict all unacknowledged movements
//	CL_PredictMovement();

//	// update the screen
//	SCR_UpdateScreen ();

//	// update audio
//	S_RenderFrame(&cl.refdef);

//	// decay dynamic lights
//	CL_DecayLights ();

//	SCR_RunCinematic();
//	Con_RunConsole();

//	cls.framecount++;
}
/*
================
CHost::print

Handles cursor positioning, line wrapping, etc
All console printing must go through this in order to be logged to disk
If no console is visible, the text will appear at the top of the game window
================
*/
void CHost::print(const  QString &txt)
{
//	if(host.rd.target)
//	{
//		if((Q_strlen(txt) + Q_strlen(host.rd.buffer)) > (host.rd.buffersize - 1))
//		{
//			if(host.rd.flush)
//			{
//				host.rd.flush(host.rd.address, host.rd.target, host.rd.buffer);
//				*host.rd.buffer = 0;
//			}
//		}
//		Q_strcat(host.rd.buffer, txt);
//		return;
//	}
//	Con_Print(txt); // echo to client console
}

void CHost::error(const  QString &error, ...)
{
//	static char hosterror1[MAX_SYSPATH];
//	static char hosterror2[MAX_SYSPATH];
//	static qboolean recursive = false;
//	va_list argptr;

//	if(host.mouse_visible && !CL_IsInMenu())
//	{
//		// hide VGUI mouse
//		while(ShowCursor(false) >= 0);
//		host.mouse_visible = false;
//	}

//	va_start(argptr, error);
//	Q_vsprintf(hosterror1, error, argptr);
//	va_end(argptr);

//	CL_WriteMessageHistory (); // before Q_error call

//	if(host.framecount < 3)
//	{
//		Sys_Error("CHost::InitError: %s", hosterror1);
//	}
//	else if(host.framecount == host.errorframe)
//	{
//		Sys_Error("CHost::MultiError: %s", hosterror2);
//		return;
//	}
//	else
//	{
//		if(host.developer > 0)
//		{
//			UI_SetActiveMenu(false);
//			Key_SetKeyDest(key_console);
//			Msg("CHost::Error: %s", hosterror1);
//		}
//		else MSGBOX2(hosterror1);
//	}

//	// host is shutting down. don't invoke infinite loop
//	if(host.state == HOST_SHUTDOWN) return;

//	if(recursive)
//	{
//		Msg("CHost::RecursiveError: %s", hosterror2);
//		Sys_Error(hosterror1);
//		return; // don't multiple executes
//	}

//	recursive = true;
//	Q_strncpy(hosterror2, hosterror1, MAX_SYSPATH);
//	host.errorframe = host.framecount; // to avoid multply calls per frame
//	Q_sprintf(host.finalmsg, "Server crashed: %s", hosterror1);

//	// clearing cmd buffer to prevent execute any commands
//	Cbuf_Clear();

//	SV_Shutdown(false);
//	CL_Drop(); // drop clients

//	// recreate world if needs
//	CL_ClearEdicts ();

//	// release all models
//	Mod_ClearAll(false);

//	recursive = false;
//	CHost::abortCurrentFrame();
}




void CHost::initCommon(const QString &progname, bool bChangeGame)
{
	host.rootdir = QApplication::applicationDirPath();
	host.argv = QApplication::arguments();
	host.change_game = bChangeGame;
	host.state = HS_Init; // initialzation started
	host.developer = host.old_developer = 0;

	//CRT_Init(); // init some CRT functions

	// some commands may turn engine into infinity loop,
	// e.g. xash.exe +game xash -game xash
	// so we clearing all cmd_args, but leave dbg states as well
	//Sys_ParseCommandLine(GetCommandLine());
	//SetErrorMode(SEM_FAILCRITICALERRORS);	// no abort/retry/fail errors

	//host.mempool = Mem_AllocPool("Zone Engine");

	if(sys_CheckParm("-console") > 0)
		host.developer = 1;
	int devId = sys_CheckParm("-dev");
	if(devId > 0)
	{
		if(host.argv.size() > devId+1)
		{
			QString mayBeDigit = host.argv[devId+1];
			bool ok;
			int digit = mayBeDigit.toInt(&ok);
			if(ok)
				host.developer = qMin(5, qAbs(digit));
			else host.developer++; // -dev == 1, -dev -console == 2
		}
		else host.developer++; // -dev == 1, -dev -console == 2
	}

	host.type = HOST_NORMAL; // predict state
	host.con_showalways = true;

	if(progname.startsWith("#"))
	{
		host.type = HOST_DEDICATED;
		SI.ModuleName = progname.right(progname.size()-1);
	}
	else
		SI.ModuleName = progname;


	if(host.type == HOST_DEDICATED)
	{
		if(host.developer < 3)
			host.developer = 3; // otherwise we see empty console
	}
//	else
//	{
//		// don't show console as default
//		if(host.developer < D_WARN)
//			host.con_showalways = false;
//	}

	host.old_developer = host.developer;

	// first text message into console or log
//	msgDev(D_Note, "Sys_LoadLibrary: Loading xash.dll - ok\n");

	cmd = new CCmd();
	cvar = new CCvar();

	cvar->get("developer", QString::number(host.developer), CVAR_INIT, "current developer level");
	cmd->addCommand("exec", CHost::CExecCmd(), "execute a script file");
	cmd->addCommand("memlist", CHost::CMemStatsCmd(), "prints memory pool information");

	console = new CConsole();

//	FS_Init();
//	Image_Init();
//	Sound_Init();

//	FS_LoadGameInfo(NULL);
//	Q_strncpy(host.gamefolder, GI->gamefolder, sizeof(host.gamefolder));

//	if(GI->secure)
//	{
//		// clear all developer levels when game is protected
//		Cvar_FullSet("developer", "0", CVAR_INIT);
//		host.developer = host.old_developer = 0;
//		host.con_showalways = false;
//	}

//	HPAK_Init();

//	IN_Init();
//	Key_Init();
}

void CHost::freeCommon()
{
//	Image_Shutdown();
//	Sound_Shutdown();
//	Netchan_Shutdown();
//	FS_Shutdown();

//	Mem_FreePool(&host.mempool);
}

void CHost::changeGame(const QString &progname )
{
//	if( !progname || !progname[0] )
//		Sys_Error( "Sys_ChangeGame: NULL gamedir" );

//	shutdown();
//	main( progname, TRUE );
}

int CHost::main(const QString &progname, int bChangeGame)
{
	error_on_exit = false;
	initCommon(progname, bChangeGame);

	// init commands and vars
	if(host.developer >= 3)
	{
		cmd->addCommand ("sys_error" , CHost::CErrorCmd()   ,
						 "just throw a fatal error to test shutdown procedures");
		cmd->addCommand ("host_error", CHost::CErrorCmd()   ,
						 "just throw a host error to test shutdown procedures");
		cmd->addCommand ("net_error" , CHost::CNetErrorCmd(),
						 "send network bad message from random place");
		cmd->addCommand ("crash"     , CHost::CCrashCmd()   ,
						 "a way to force a bus error for development reasons");
	}

	host_cheats = cvar->get("sv_cheats", "0", CVAR_LATCH, "allow cheat variables to enable");
	host_maxfps = cvar->get("fps_max", "72", CVAR_ARCHIVE, "host fps upper limit");
	host_framerate =  cvar->get("host_framerate", "0", 0, "locks frame timing to this value in seconds");
	host_serverstate =  cvar->get("host_serverstate", "0", CVAR_INIT, "displays current server state");
	host_gameloaded =  cvar->get("host_gameloaded", "0", CVAR_INIT, "inidcates a loaded game.dll");
	host_clientloaded =  cvar->get("host_clientloaded", "0", CVAR_INIT, "inidcates a loaded client.dll");
	host_limitlocal =  cvar->get("host_limitlocal", "0", 0, "apply cl_cmdrate and rate to loopback connection");
	con_gamemaps =  cvar->get("con_mapfilter", "1", CVAR_ARCHIVE, "when true show only maps in game folder");
	build =  cvar->get("build", "666", CVAR_INIT, "returns a current build number");
	ver =  cvar->get("ver", QString("{1}/{2} (hw build {3})")
					 .arg(PROTOCOL_VERSION).arg(XASH_VERSION).arg(666), CVAR_INIT, "shows an engine version");

	// content control
	 cvar->get("violence_hgibs", "1", CVAR_ARCHIVE, "show human gib entities");
	 cvar->get("violence_agibs", "1", CVAR_ARCHIVE, "show alien gib entities");
	 cvar->get("violence_hblood", "1", CVAR_ARCHIVE, "draw human blood");
	 cvar->get("violence_ablood", "1", CVAR_ARCHIVE, "draw alien blood");

	if(host.type != HOST_DEDICATED)
	{
		// when we in developer-mode automatically turn cheats on
		if(host.developer > 1) cvar->setFloat("sv_cheats", 1.0f);
		Cbuf_AddText("exec video.cfg\n");
	}

//	Mod_Init();
//	NET_Init();
//	Netchan_Init();

//	// allow to change game from the console
//	if(pChangeGame != NULL)
//	{
//		Cmd_AddCommand("game", &CHost::changeGame_f, "change game");
//		 cvar->get("host_allow_changegame", "1", CVAR_READ_ONLY, "allows to change games");
//	}
//	else
//	{
//		 cvar->get("host_allow_changegame", "0", CVAR_READ_ONLY, "allows to change games");
//	}

//	SV_Init();
//	CL_Init();

//	if(host.type == HOST_DEDICATED)
//	{
//		Con_InitConsoleCommands ();

//		Cmd_AddCommand("quit", Sys_Quit, "quit the game");
//		Cmd_AddCommand("exit", Sys_Quit, "quit the game");

//		// dedicated servers using settings from server.cfg file
//		Cbuf_AddText(va("exec %s\n", Cvar_VariableString("servercfgfile")));
//		Cbuf_Execute();

//		Cbuf_AddText(va("map %s\n", Cvar_VariableString("defaultmap")));
//	}
//	else
//	{
//		Cmd_AddCommand("minimize", CHost::minimize_f, "minimize main window to tray");
//		Cbuf_AddText("exec config.cfg\n");
//	}

//	host.errorframe = 0;
//	Cbuf_Execute();

//	// post initializations
//	switch(host.type)
//	{
//	case HOST_NORMAL:
//		Con_ShowConsole(false); // hide console
//		// execute startup config and cmdline
//		Cbuf_AddText(va("exec %s.rc\n", SI.ModuleName));
//		// intentional fallthrough
//	case HOST_DEDICATED:
//		// if stuffcmds wasn't run, then init.rc is probably missing, use default
//		if(!host.stuffcmdsrun) Cbuf_AddText("stuffcmds\n");

//		Cbuf_Execute();
//		break;
//	}

//	host.change_game = false;	// done
//	Cmd_RemoveCommand("setr");	// remove potentially backdoor for change render settings
//	Cmd_RemoveCommand("setgl");

//	// we need to execute it again here
//	Cmd_ExecuteString("exec config.cfg\n", src_command);
//	oldtime = Sys_DoubleTime();
//	SCR_CheckStartupVids();	// must be last

//	// main window message loop
//	while(!host.crashed)
//	{
//		newtime = Sys_DoubleTime ();
//		CHost::frame((float)(newtime - oldtime));
//		oldtime = newtime;
//	}

	// never reached
	return 0;
}

void CHost::shutdown()
{
	if(host.shutdown_issued) return;
	host.shutdown_issued = true;

	if(host.state != HOST_ERR_FATAL)
		host.state = HOST_SHUTDOWN; // prepare host to normal shutdown
	if(!host.change_game)
		host.finalmsg = "Server shutdown";

	if(host.type == HOST_NORMAL)
		writeConfig();

//	SV_Shutdown(false);
//	CL_Shutdown();

//	Mod_Shutdown();
//	NET_Shutdown();
//	freeCommon();
//	Con_DestroyConsole();

//	// restore filter
//	if(host.oldFilter) SetUnhandledExceptionFilter(host.oldFilter);
}

// main DLL entry point
//BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
//{
//	hCurrent = hinstDLL;
//	return TRUE;
//}

double CHost::sys_DoubleTime()
{
//	static LARGE_INTEGER g_PerformanceFrequency;
//	static LARGE_INTEGER g_ClockStart;
//	LARGE_INTEGER CurrentTime;

//	if( !g_PerformanceFrequency.QuadPart )
//	{
//		QueryPerformanceFrequency( &g_PerformanceFrequency );
//		QueryPerformanceCounter( &g_ClockStart );
//	}
//	QueryPerformanceCounter( &CurrentTime );

	return 0;//(double)( CurrentTime.QuadPart - g_ClockStart.QuadPart ) / (double)( g_PerformanceFrequency.QuadPart );
}

QString CHost::sys_GetClipboardData()
{
	return QApplication::clipboard()->text();
}

void CHost::sys_SetClipboardData(const QString &text )
{
	QApplication::clipboard()->setText(text);
}

QString CHost::currentUser()
{
	QString name = qgetenv("USER");
	if (name.isEmpty())
		name = qgetenv("USERNAME");
	return name;
}



/*
==================
Sys_ParseCommandLine

==================
*/
//void CHost::Sys_ParseCommandLine( LPSTR lpCmdLine )
//{
//	const  QString &blank = "censored";
//	static char commandline[MAX_SYSPATH];
//	int i;

//	host.argc = 1;
//	host.argv[0] = "exe";

//	Q_strncpy( commandline, lpCmdLine, Q_strlen( lpCmdLine ) + 1 );
//	lpCmdLine = commandline; // to prevent modify original commandline

//	while( *lpCmdLine && ( host.argc < MAX_NUM_ARGVS ))
//	{
//		while( *lpCmdLine && *lpCmdLine <= ' ' )
//			lpCmdLine++;
//		if( !*lpCmdLine ) break;

//		if( *lpCmdLine == '\"' )
//		{
//			// quoted string
//			lpCmdLine++;
//			host.argv[host.argc] = lpCmdLine;
//			host.argc++;
//			while( *lpCmdLine && ( *lpCmdLine != '\"' ))
//				lpCmdLine++;
//		}
//		else
//		{
//			// unquoted word
//			host.argv[host.argc] = lpCmdLine;
//			host.argc++;
//			while( *lpCmdLine && *lpCmdLine > ' ')
//				lpCmdLine++;
//		}

//		if( *lpCmdLine )
//		{
//			*lpCmdLine = 0;
//			lpCmdLine++;
//		}
//	}

//	if( !host.change_game ) return;

//	for( i = 0; i < host.argc; i++ )
//	{
//		// we wan't return to first game
//		if( !Q_stricmp( "-game", host.argv[i] )) host.argv[i] = ( QString &)blank;
//		// probably it's timewaster, because engine rejected second change
//		if( !Q_stricmp( "+game", host.argv[i] )) host.argv[i] = ( QString &)blank;
//		// you sure what is map exists in new game?
//		if( !Q_stricmp( "+map", host.argv[i] )) host.argv[i] = ( QString &)blank;
//		// just stupid action
//		if( !Q_stricmp( "+load", host.argv[i] )) host.argv[i] = ( QString &)blank;
//		// changelevel beetwen games? wow it's great idea!
//		if( !Q_stricmp( "+changelevel", host.argv[i] )) host.argv[i] = ( QString &)blank;
//	}
//}

/*
==================
Sys_MergeCommandLine

==================
*/
//void CHost::Sys_MergeCommandLine( LPSTR lpCmdLine )
//{
//	const  QString &blank = "censored";
//	int i;

//	if( !host.change_game ) return;

//	for( i = 0; i < host.argc; i++ )
//	{
//		// second call
//		if( host.type == HOST_DEDICATED && !Q_strnicmp( "+menu_", host.argv[i], 6 ))
//			host.argv[i] = ( QString &)blank;
//	}
//}

/*
================
Sys_CheckParm

Returns the position (1 to argc-1) in the program's argument list
where the given parameter apears, or 0 if not present
================
*/
int CHost::sys_CheckParm( const QString &parm )
{
	return host.argv.indexOf(parm);
}

/*
================
Sys_GetParmFromCmdLine

Returns the argument for specified parm
================
*/
bool CHost::_sys_GetParmFromCmdLine(  QString &parm,  QString &out, size_t size )
{
//	int argc = Sys_CheckParm( parm );

//	if( !argc ) return false;
//	if( !out ) return false;
//	if( !host.argv[argc + 1] ) return false;
//	Q_strncpy( out, host.argv[argc+1], size );

	return true;
}

void CHost::sys_SendKeyEvents()
{
//	MSG msg;

//	while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ))
//	{
//		if( !GetMessage( &msg, NULL, 0, 0 ))
//			Sys_Quit ();

//			TranslateMessage( &msg );
//			DispatchMessage( &msg );
//	}
}

/*
================
Sys_WaitForQuit

wait for 'Esc' key will be hit
================
*/
void CHost::sys_WaitForQuit()
{
//	MSG msg;

//	Con_RegisterHotkeys();

//	msg.message = 0;

//	// wait for the user to quit
//	while( msg.message != WM_QUIT )
//	{
//		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
//		{
//			TranslateMessage( &msg );
//			DispatchMessage( &msg );
//		}
//		else Sys_Sleep( 20 );
//	}
}

/*
================
Sys_Error

NOTE: we must prepare engine to shutdown
before call this
================
*/
void CHost::sys_Error( const  QString &error, ... )
{
//	va_list argptr;
//	char text[MAX_SYSPATH];

//	if( host.state == HOST_ERR_FATAL )
//		return; // don't multiple executes

//	// make sure what console received last message
//	if( host.change_game ) Sys_Sleep( 200 );

//	error_on_exit = true;
//	host.state = HOST_ERR_FATAL;
//	va_start( argptr, error );
//	Q_vsprintf( text, error, argptr );
//	va_end( argptr );

//	SV_SysError( text );

//	if( host.type == HOST_NORMAL )
//	{
//		if( host.hWnd ) ShowWindow( host.hWnd, SW_HIDE );
//		VID_RestoreGamma();
//	}

//	if( host.developer > 0 )
//	{
//		Con_ShowConsole( true );
//		Con_DisableInput();	// disable input line for dedicated server
//		Sys_Print( text );	// print error message
//		Sys_WaitForQuit();
//	}
//	else
//	{
//		Con_ShowConsole( false );
//		MSGBOX( text );
//	}

//	Sys_Quit();
}

/*
================
Sys_Break

same as Error
================
*/
void CHost::sys_Break( const  QString &error, ... )
{
//	va_list argptr;
//	char text[MAX_SYSPATH];

//	if( host.state == HOST_ERR_FATAL )
//		return; // don't multiple executes

//	error_on_exit = true;
//	host.state = HOST_ERR_FATAL;
//	va_start( argptr, error );
//	Q_vsprintf( text, error, argptr );
//	va_end( argptr );

//	if( host.type == HOST_NORMAL )
//	{
//		if( host.hWnd ) ShowWindow( host.hWnd, SW_HIDE );
//		VID_RestoreGamma();
//	}

//	if( host.type != HOST_NORMAL || host.developer > 0 )
//	{
//		Con_ShowConsole( true );
//		Con_DisableInput();	// disable input line for dedicated server
//		Sys_Print( text );
//		Sys_WaitForQuit();
//	}
//	else
//	{
//		Con_ShowConsole( false );
//		MSGBOX( text );
//	}

//	Sys_Quit();
}

/*
================
Sys_Quit
================
*/
void CHost::sys_Quit()
{
//	Host_Shutdown();
//	exit( error_on_exit );
}

/*
================
Sys_Print

print into window console
================
*/
void CHost::sys_Print( const QString pMsg )
{
//	const  QString &msg;
//	char buffer[32768];
//	char logbuf[32768];
//	 QString &b = buffer;
//	 QString &c = logbuf;
//	int i = 0;

//	if( host.type == HOST_NORMAL )
//		Con_Print( pMsg );

//	// if the message is REALLY long, use just the last portion of it
//	if( Q_strlen( pMsg ) > sizeof( buffer ) - 1 )
//		msg = pMsg + Q_strlen( pMsg ) - sizeof( buffer ) + 1;
//	else msg = pMsg;

//	// copy into an intermediate buffer
//	while( msg[i] && (( b - buffer ) < sizeof( buffer ) - 1 ))
//	{
//		if( msg[i] == '\n' && msg[i+1] == '\r' )
//		{
//			b[0] = '\r';
//			b[1] = '\n';
//			c[0] = '\n';
//			b += 2, c++;
//			i++;
//		}
//		else if( msg[i] == '\r' )
//		{
//			b[0] = '\r';
//			b[1] = '\n';
//			b += 2;
//		}
//		else if( msg[i] == '\n' )
//		{
//			b[0] = '\r';
//			b[1] = '\n';
//			c[0] = '\n';
//			b += 2, c++;
//		}
//		else if( msg[i] == '\35' || msg[i] == '\36' || msg[i] == '\37' )
//		{
//			i++; // skip console pseudo graph
//		}
//		else if( IsColorString( &msg[i] ))
//		{
//			i++; // skip color prefix
//		}
//		else
//		{
//			*b = *c = msg[i];
//			b++, c++;
//		}
//		i++;
//	}

//	*b = *c = 0; // cutoff garbage

//	Sys_PrintLog( logbuf );
//	Con_WinPrint( buffer );
}

/*
================
Msg

formatted message
================
*/
void CHost::msg( const  QString &pMsg, ... )
{
//	va_list argptr;
//	char text[8192];

//	va_start( argptr, pMsg );
//	Q_vsnprintf( text, sizeof( text ), pMsg, argptr );
//	va_end( argptr );

//	Sys_Print( text );
}

/*
================
MsgDev

formatted developer message
================
*/
void CHost::msgDev( int level, const  QString &pMsg, ... )
{
//	va_list argptr;
//	char text[8192];

//	if( host.developer < level ) return;

//	va_start( argptr, pMsg );
//	Q_vsnprintf( text, sizeof( text ), pMsg, argptr );
//	va_end( argptr );

//	switch( level )
//	{
//	case D_WARN:
//		Sys_Print( va( "^3Warning:^7 %s", text ));
//		break;
//	case D_ERROR:
//		Sys_Print( va( "^1Error:^7 %s", text ));
//		break;
//	case D_INFO:
//	case D_NOTE:
//	case D_AICONSOLE:
//		Sys_Print( text );
//		break;
//	}
}



void CHost::CChangeGameCmd::exec(const QStringList &args)
{

		int i;

	//	if(Cmd_Argc() != 2)
	//	{
	//		Msg("Usage: game <directory>\n");
	//		return;
	//	}

	//	// validate gamedir
	//	for(i = 0; i < SI.numgames; i++)
	//	{
	//		if(!Q_stricmp(SI.games[i]->gamefolder, Cmd_Argv(1)))
	//			break;
	//	}

	//	if(i == SI.numgames)
	//	{
	//		Msg("%s not exist\n", Cmd_Argv(1));
	//	}
	//	else if(!Q_stricmp(GI->gamefolder, Cmd_Argv(1)))
	//	{
	//		Msg("%s already active\n", Cmd_Argv(1));
	//	}
	//	else
	//	{
	//		const  QString &arg1 = va("%s%s", (host.type == HOST_NORMAL) ? "" : "#", Cmd_Argv(1));
	//		const  QString &arg2 = va("change game to '%s'", SI.games[i]->title);

	//		CHost::newInstance(arg1, arg2);
	//	}


}

void CHost::CExecCmd::exec(const QStringList &args)
{
	//	string cfgpath;
	//	 QString &f, *txt;
	//	size_t len;

	//	if(Cmd_Argc() != 2)
	//	{
	//		Msg("Usage: exec <filename>\n");
	//		return;
	//	}

	//	// HACKHACK: don't execute listenserver.cfg in singleplayer
	//	if(!Q_stricmp(Cvar_VariableString("lservercfgfile"),  Cmd_Argv(1)))
	//	{
	//		if(Cvar_VariableValue("maxplayers") == 1.0f)
	//			return;
	//	}

	//	Q_strncpy(cfgpath, Cmd_Argv(1), sizeof(cfgpath));
	//	FS_DefaultExtension(cfgpath, ".cfg"); // append as default

	//	f = reinterpret_cast<char*>(FS_LoadFile(cfgpath, reinterpret_cast<fs_offset_t*>(&len), false));
	//	if(!f)
	//	{
	//		MsgDev(D_NOTE, "couldn't exec %s\n", Cmd_Argv(1));
	//		return;
	//	}

	//	// adds \n\0 at end of the file
	//	txt = reinterpret_cast<char*>(Z_Malloc(len + 2));
	//	Q_memcpy(txt, f, len);
	//	Q_strncat(txt, "\n", len + 2);
	//	Mem_Free(f);

	//	MsgDev(D_INFO, "execing %s\n", Cmd_Argv(1));
	//	Cbuf_InsertText(txt);
	//	Mem_Free(txt);
}

void CHost::CErrorCmd::exec(const QStringList &args)
{
	//	const  QString &error = Cmd_Argv(1);

	//	if(!*error) error = "Invoked host error";
	//	CHost::error("%s\n", error);
}

void CHost::CNetErrorCmd::exec(const QStringList &args)
{
	//	Q_strncpy(host.finalmsg, Cmd_Argv(1), sizeof(host.finalmsg));
	//	SV_ForceError();
}

void CHost::CCrashCmd::exec(const QStringList &args)
{
	*(int *)0 = 0xffffffff;
}

void CHost::CMemStatsCmd::exec(const QStringList &args)
{
	//	switch(Cmd_Argc())
	//	{
	//	case 1:
	//		Mem_PrintList(1<<30);
	//		Mem_PrintStats();
	//		break;
	//	case 2:
	//		Mem_PrintList(Q_atoi(Cmd_Argv(1)) * 1024);
	//		Mem_PrintStats();
	//		break;
	//	default:
	//		Msg("Usage: memlist <all>\n");
	//		break;
	//	}
}

void CHost::CMinimizeCmd::exec(const QStringList &args)
{
	//	if(host.hWnd)
	//		ShowWindow(host.hWnd, SW_MINIMIZE);
}
