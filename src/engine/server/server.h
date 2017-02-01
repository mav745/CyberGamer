/*
server.h - primary header for server
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

#ifndef SERVER_H
#define SERVER_H

#include "mathlib.h"
#include "edict.h"
#include "eiface.h"
#include "physint.h"	// physics interface
#include "mod_local.h"
#include "pm_defs.h"
#include "pm_movevars.h"
#include "entity_state.h"
#include "protocol.h"
#include "netchan.h"
#include "custom.h"
#include "world.h"

//=============================================================================

#define SV_UPDATE_MASK	(SV_UPDATE_BACKUP - 1)
extern int SV_UPDATE_BACKUP;

// hostflags
#define SVF_SKIPLOCALHOST	BIT( 0 )
#define SVF_PLAYERSONLY	BIT( 1 )
#define SVF_PORTALPASS	BIT( 2 )			// we are do portal pass

// mapvalid flags
#define MAP_IS_EXIST	BIT( 0 )
#define MAP_HAS_SPAWNPOINT	BIT( 1 )
#define MAP_HAS_LANDMARK	BIT( 2 )
#define MAP_INVALID_VERSION	BIT( 3 )

#define SV_IsValidEdict( e )	( e && !e->free )
#define NUM_FOR_EDICT(e)	((int)((edict_t *)(e) - svgame.edicts))
#define EDICT_NUM( num )	SV_EDICT_NUM( num, __FILE__, __LINE__ )
#define STRING( offset )	SV_GetString( offset )
#define ALLOC_STRING(str)	SV_AllocString( str )
#define MAKE_STRING(str)	SV_MakeString( str )

#define MAX_PUSHED_ENTS	256
#define MAX_CAMERAS		32

#define DVIS_PVS		0
#define DVIS_PHS		1

typedef enum
{
	ss_dead,		// no map loaded
	ss_loading,	// spawning level edicts
	ss_active		// actively running
} sv_state_t;

typedef enum
{
	cs_free = 0,	// can be reused for a new connection
	cs_zombie,	// client has been disconnected, but don't reuse connection for a couple seconds
	cs_connected,	// has been assigned to a sv_client_t, but not in game yet
	cs_spawned	// client is fully in game
} cl_state_t;

// instanced baselines container
typedef struct
{
	int		count;
	string_t		classnames[64];
	entity_state_t	baselines[64];
} sv_baselines_t;

typedef struct
{
	const char	*name;
	FORCE_TYPE	force_state;
	vec3_t		mins, maxs;
} sv_consistency_t;

// like as entity_state_t in Quake
typedef struct
{
	char		model[64];	// name of static-entity model for right precache
	vec3_t		origin;
	vec3_t		angles;
	byte		sequence;
	byte		frame;
	short		colormap;
	byte		skin;		// can't set contents! only real skin!
	byte		rendermode;
	byte		renderamt;
	color24		rendercolor;
	byte		renderfx;
} sv_static_entity_t;

typedef struct server_s
{
	sv_state_t	state;		// precache commands are only valid during load

	qboolean		background;	// this is background map
	qboolean		loadgame;		// client begins should reuse existing entity
	qboolean		changelevel;	// set if changelevel in-action (smooth or classic)
	int		viewentity;	// applied on client restore. this is temporare place
					// until client connected

	double		time;		// sv.time += host.frametime
	int		net_framenum;	// to avoid send edicts twice through portals

	int		hostflags;	// misc server flags: predicting etc

	string		name;		// map name
	string		startspot;	// player_start name on nextmap

	double		lastchecktime;
	int		lastcheck;	// number of last checked client

	char		model_precache[MAX_MODELS][CS_SIZE];
	char		sound_precache[MAX_SOUNDS][CS_SIZE];
	char		files_precache[MAX_CUSTOM][CS_SIZE];
	char		event_precache[MAX_EVENTS][CS_SIZE];

	sv_static_entity_t	static_entities[MAX_STATIC_ENTITIES];
	int		num_static_entities;

	// run local lightstyles to let SV_LightPoint grab the actual information
	lightstyle_t	lightstyles[MAX_LIGHTSTYLES];

	sv_consistency_t	consistency_files[MAX_MODELS];
	resource_t	resources[MAX_MODELS];
	int		num_consistency_resources;	// typically check model bounds on this
	int		num_resources;

	sv_baselines_t	instanced;	// instanced baselines

	// unreliable data to send to clients.
	sizebuf_t		datagram;
	byte		datagram_buf[NET_MAX_PAYLOAD];

	// reliable data to send to clients.
	sizebuf_t		reliable_datagram;	// copied to all clients at end of frame
	byte		reliable_datagram_buf[NET_MAX_PAYLOAD];

	// the multicast buffer is used to send a message to a set of clients
	sizebuf_t		multicast;
	byte		multicast_buf[NET_MAX_PAYLOAD];

	sizebuf_t		signon;
	byte		signon_buf[NET_MAX_PAYLOAD];

	sizebuf_t		spectator_datagram;
	byte		spectator_buf[NET_MAX_PAYLOAD];

	model_t		*worldmodel;	// pointer to world
	uint		checksum;		// for catching cheater maps

	qboolean		write_bad_message;	// just for debug
	qboolean		paused;
} server_t;

typedef struct
{
	double		senttime;
	float		raw_ping;
	float		latency;

	clientdata_t	clientdata;
	weapon_data_t	weapondata[MAX_WEAPONS];
	weapon_data_t	oldweapondata[MAX_WEAPONS];	// g-cont. The fucking Cry Of Fear a does corrupting memory after the weapondata!!!

	int  		num_entities;
	int  		first_entity;		// into the circular sv_packet_entities[]
} client_frame_t;

typedef struct sv_client_s
{
	cl_state_t	state;
	char		name[32];			// extracted from userinfo, color string allowed

	char		userinfo[MAX_INFO_STRING];	// name, etc (received from client)
	char		physinfo[MAX_INFO_STRING];	// set on server (transmit to client)

	qboolean		send_message;
	qboolean		skip_message;

	qboolean		local_weapons;		// enable weapon predicting
	qboolean		lag_compensation;		// enable lag compensation
	qboolean		hltv_proxy;		// this is spectator proxy (hltv)		

	netchan_t		netchan;
	int		chokecount;         	// number of messages rate supressed
	int		delta_sequence;		// -1 = no compression.

	double		next_messagetime;		// time when we should send next world state update  
	double		cl_updaterate;		// default time to wait for next message
	double		next_checkpingtime;		// time to send all players pings to client
	double		timebase;			// client timebase

	customization_t	customization;		// player customization linked list
	resource_t	resource1;
	resource_t	resource2;		// <mapname.res> from client (server downloading)

	qboolean		sendmovevars;
	qboolean		sendinfo;

	qboolean		fakeclient;		// This client is a fake player controlled by the game DLL

	usercmd_t		lastcmd;			// for filling in big drops

	double		last_cmdtime;
	double		last_movetime;
	double		next_movetime;

	int		modelindex;		// custom playermodel index
	int		packet_loss;
	float		latency;
	float		ping;

	int		listeners;		// 32 bits == MAX_CLIENTS (voice listeners)

	edict_t		*edict;			// EDICT_NUM(clientnum+1)
	edict_t		*pViewEntity;		// svc_setview member
	int		messagelevel;		// for filtering printed messages

	edict_t		*cameras[MAX_CAMERAS];	// list of portal cameras in player PVS
	int		num_cameras;		// num of portal cameras that can merge PVS

	// the datagram is written to by sound calls, prints, temp ents, etc.
	// it can be harmlessly overflowed.
	sizebuf_t		datagram;
	byte		datagram_buf[NET_MAX_PAYLOAD];

	client_frame_t	*frames;			// updates can be delta'd from here
	event_state_t	events;

	double		lastmessage;		// time when packet was last received
	double		lastconnect;

	int		challenge;		// challenge of this user, randomly generated
	int		userid;			// identifying number on server
	int		authentication_method;
	uint		WonID;			// WonID
} sv_client_t;

/*
=============================================================================
 a client can leave the server in one of four ways:
 dropping properly by quiting or disconnecting
 timing out if no valid messages are received for timeout.value seconds
 getting kicked off by the server operator
 a program error, like an overflowed reliable buffer
=============================================================================
*/

// MAX_CHALLENGES is made large to prevent a denial
// of service attack that could cycle all of them
// out before legitimate users connected
#define MAX_CHALLENGES	1024

typedef struct
{
	netadr_t		adr;
	int		challenge;
	double		time;
	qboolean		connected;
} challenge_t;

typedef struct
{
	char		name[32];	// in GoldSrc max name length is 12
	int		number;	// svc_ number
	int		size;	// if size == -1, size come from first byte after svcnum
} sv_user_message_t;

typedef struct
{
	edict_t		*ent;
	vec3_t		origin;
	vec3_t		angles;
	int		fixangle;
} sv_pushed_t;

typedef struct
{
	qboolean		active;
	qboolean		moving;
	qboolean		firstframe;
	qboolean		nointerp;

	vec3_t		mins;
	vec3_t		maxs;

	vec3_t		curpos;
	vec3_t		oldpos;
	vec3_t		newpos;
	vec3_t		finalpos;
} sv_interp_t;

typedef struct
{
	// user messages stuff
	const char	*msg_name;		// just for debug
	sv_user_message_t	msg[MAX_USER_MESSAGES];	// user messages array
	int		msg_size_index;		// write message size at this pos in bitbuf
	int		msg_realsize;		// left in bytes
	int		msg_index;		// for debug messages
	int		msg_dest;			// msg destination ( MSG_ONE, MSG_ALL etc )
	qboolean		msg_started;		// to avoid recursive included messages
	edict_t		*msg_ent;			// user message member entity
	vec3_t		msg_org;			// user message member origin

	// catched user messages (nasty hack)
	int		gmsgHudText;		// -1 if not catched (e.g. mod not registered this message)

	void		*hInstance;		// pointer to game.dll

	union
	{
		edict_t	*edicts;			// acess by edict number
		void	*vp;			// acess by offset in bytes
	};
	int		numEntities;		// actual entities count

	movevars_t	movevars;			// curstate
	movevars_t	oldmovevars;		// oldstate
	playermove_t	*pmove;			// pmove state
	sv_interp_t	interp[MAX_CLIENTS];	// interpolate clients

	sv_pushed_t	pushed[MAX_PUSHED_ENTS];	// no reason to keep array for all edicts
						// 256 it should be enough for any game situation
	vec3_t		player_mins[MAX_MAP_HULLS];	// 4 hulls allowed
	vec3_t		player_maxs[MAX_MAP_HULLS];	// 4 hulls allowed

	globalvars_t	*globals;			// server globals
	DLL_FUNCTIONS	dllFuncs;			// dll exported funcs
	NEW_DLL_FUNCTIONS	dllFuncs2;		// new dll exported funcs (may be NULL)
	physics_interface_t	physFuncs;		// physics interface functions (Xash3D extension)
	byte		*mempool;			// server premamnent pool: edicts etc
	byte		*stringspool;		// for engine strings

	SAVERESTOREDATA	SaveData;			// shared struct, used for save data
} svgame_static_t;

typedef struct
{
	qboolean		initialized;		// sv_init has completed
	double		timestart;		// just for profiling

	int		groupmask;
	int		groupop;

	double		changelevel_next_time;	// don't execute multiple changelevels at once time
	int		spawncount;		// incremented each server start
						// used to check late spawns
	sv_client_t	*clients;			// [sv_maxclients->integer]
	sv_client_t	*currentPlayer;		// current client who network message sending on
	int		currentPlayerNum;		// for easy acess to some global arrays
	int		num_client_entities;	// sv_maxclients->integer*UPDATE_BACKUP*MAX_PACKET_ENTITIES
	int		next_client_entities;	// next client_entity to use
	entity_state_t	*packet_entities;		// [num_client_entities]
	entity_state_t	*baselines;		// [GI->max_edicts]

	double		last_heartbeat;
	challenge_t	challenges[MAX_CHALLENGES];	// to prevent invalid IPs from connecting
} server_static_t;

//=============================================================================

extern	server_static_t	svs;			// persistant server info
extern	server_t		sv;			// local server
extern	svgame_static_t	svgame;			// persistant game info
extern	areanode_t	sv_areanodes[];		// AABB dynamic tree

extern	convar_t		*sv_pausable;		// allows pause in multiplayer
extern	convar_t		*sv_newunit;
extern	convar_t		*sv_airaccelerate;
extern	convar_t		*sv_accelerate;
extern	convar_t		*sv_friction;
extern	convar_t		*sv_edgefriction;
extern	convar_t		*sv_maxvelocity;
extern	convar_t		*sv_gravity;
extern	convar_t		*sv_stopspeed;
extern	convar_t		*sv_check_errors;
extern	convar_t		*sv_reconnect_limit;
extern	convar_t		*sv_lighting_modulate;
extern	convar_t		*rcon_password;
extern	convar_t		*hostname;
extern	convar_t		*sv_stepsize;
extern	convar_t		*sv_rollangle;
extern	convar_t		*sv_rollspeed;
extern	convar_t		*sv_maxspeed;
extern	convar_t		*sv_maxclients;
extern	convar_t		*sv_skyname;
extern	convar_t		*serverinfo;
extern	convar_t		*sv_failuretime;
extern	convar_t		*sv_unlag;
extern	convar_t		*sv_novis;
extern	convar_t		*sv_maxunlag;
extern	convar_t		*sv_unlagpush;
extern	convar_t		*sv_unlagsamples;
extern	convar_t		*sv_allow_upload;
extern	convar_t		*sv_allow_download;
extern	convar_t		*sv_allow_studio_attachment_angles;
extern	convar_t		*sv_allow_rotate_pushables;
extern	convar_t		*sv_clienttrace;
extern	convar_t		*sv_send_resources;
extern	convar_t		*sv_send_logos;
extern	convar_t		*sv_sendvelocity;
extern	convar_t		*sv_skyspeed;
extern	convar_t		*sv_skyangle;
extern	convar_t		*sv_quakehulls;
extern	convar_t		*sv_validate_changelevel;
extern	convar_t		*mp_consistency;
extern	convar_t		*public_server;
extern	convar_t		*physinfo;
extern	convar_t		*deathmatch;
extern	convar_t		*teamplay;
extern	convar_t		*skill;
extern	convar_t		*coop;

//===========================================================
//
// sv_main.c
//
void __cdecl SV_FinalMessage( char *message, qboolean reconnect );
void __cdecl SV_DropClient( sv_client_t *drop );
void __cdecl SV_UpdateMovevars( qboolean initialize );
int __cdecl SV_ModelIndex( const char *name );
int __cdecl SV_SoundIndex( const char *name );
int __cdecl SV_EventIndex( const char *name );
int __cdecl SV_GenericIndex( const char *name );
int __cdecl SV_CalcPacketLoss( sv_client_t *cl );
void __cdecl SV_ExecuteUserCommand (char *s);
void __cdecl SV_InitOperatorCommands( void );
void __cdecl SV_KillOperatorCommands( void );
void __cdecl SV_UserinfoChanged( sv_client_t *cl, const char *userinfo );
void __cdecl SV_PrepWorldFrame( void );
void __cdecl SV_ProcessFile( sv_client_t *cl, char *filename );
void __cdecl SV_SendResourceList( sv_client_t *cl );
void __cdecl Master_Add( void );
void __cdecl Master_Heartbeat( void );
void __cdecl Master_Packet( void );

//
// sv_init.c
//
void __cdecl SV_InitGame( void );
void __cdecl SV_ActivateServer( void );
void __cdecl SV_DeactivateServer( void );
void __cdecl SV_LevelInit( const char *pMapName, char const *pOldLevel, char const *pLandmarkName, qboolean loadGame );
qboolean __cdecl SV_SpawnServer( const char *server, const char *startspot );

//
// sv_phys.c
//
void __cdecl SV_Physics( void );
qboolean __cdecl SV_InitPhysicsAPI( void );
void __cdecl SV_CheckVelocity( edict_t *ent );
qboolean __cdecl SV_CheckWater( edict_t *ent );
qboolean __cdecl SV_RunThink( edict_t *ent );
qboolean __cdecl SV_PlayerRunThink( edict_t *ent, float frametime, double time );
qboolean __cdecl SV_TestEntityPosition( edict_t *ent, edict_t *blocker );	// for EntityInSolid checks
void __cdecl SV_Impact( edict_t *e1, edict_t *e2, trace_t *trace );
qboolean __cdecl SV_CanPushed( edict_t *ent );
void __cdecl SV_FreeOldEntities( void );
void __cdecl SV_CheckAllEnts( void );

//
// sv_move.c
//
qboolean __cdecl SV_MoveStep( edict_t *ent, vec3_t move, qboolean relink );
qboolean __cdecl SV_MoveTest( edict_t *ent, vec3_t move, qboolean relink );
void __cdecl SV_MoveToOrigin( edict_t *ed, const vec3_t goal, float dist, int iMode );
qboolean __cdecl SV_CheckBottom( edict_t *ent, int iMode );
float __cdecl SV_VecToYaw( const vec3_t src );
void __cdecl SV_WaterMove( edict_t *ent );

//
// sv_send.c
//
void __cdecl SV_SendClientMessages( void );
void __cdecl SV_ClientPrintf( sv_client_t *cl, int level, char *fmt, ... );
void __cdecl SV_BroadcastPrintf( int level, char *fmt, ... );
void __cdecl SV_BroadcastCommand( char *fmt, ... );

//
// sv_client.c
//
char *__cdecl SV_StatusString( void );
void __cdecl SV_RefreshUserinfo( void );
void __cdecl SV_GetChallenge( netadr_t from );
void __cdecl SV_DirectConnect( netadr_t from );
void __cdecl SV_TogglePause( const char *msg );
void __cdecl SV_PutClientInServer( edict_t *ent );
qboolean __cdecl SV_ShouldUpdatePing( sv_client_t *cl );
const char *__cdecl SV_GetClientIDString( sv_client_t *cl );
void __cdecl SV_FullClientUpdate( sv_client_t *cl, sizebuf_t *msg );
void __cdecl SV_FullUpdateMovevars( sv_client_t *cl, sizebuf_t *msg );
void __cdecl SV_GetPlayerStats( sv_client_t *cl, int *ping, int *packet_loss );
qboolean __cdecl SV_ClientConnect( edict_t *ent, char *userinfo );
void __cdecl SV_ClientThink( sv_client_t *cl, usercmd_t *cmd );
void __cdecl SV_ExecuteClientMessage( sv_client_t *cl, sizebuf_t *msg );
void __cdecl SV_ConnectionlessPacket( netadr_t from, sizebuf_t *msg );
edict_t *__cdecl SV_FakeConnect( const char *netname );
void __cdecl SV_ExecuteClientCommand( sv_client_t *cl, char *s );
void __cdecl SV_RunCmd( sv_client_t *cl, usercmd_t *ucmd, int random_seed );
qboolean __cdecl SV_IsPlayerIndex( int idx );
void __cdecl SV_InitClientMove( void );
void __cdecl SV_UpdateServerInfo( void );

//
// sv_cmds.c
//
void __cdecl SV_Status_f( void );
void __cdecl SV_Newgame_f( void );

//
// sv_custom.c
//
void __cdecl SV_SendResources( sizebuf_t *msg );
int __cdecl SV_TransferConsistencyInfo( void );

//
// sv_frame.c
//
void __cdecl SV_WriteFrameToClient( sv_client_t *client, sizebuf_t *msg );
void __cdecl SV_BuildClientFrame( sv_client_t *client );
void __cdecl SV_InactivateClients( void );
void __cdecl SV_SendMessagesToAll( void );
void __cdecl SV_SkipUpdates( void );

//
// sv_game.c
//
qboolean __cdecl SV_LoadProgs( const char *name );
void __cdecl SV_UnloadProgs( void );
void __cdecl SV_FreeEdicts( void );
edict_t *__cdecl SV_AllocEdict( void );
void __cdecl SV_FreeEdict( edict_t *pEdict );
void __cdecl SV_InitEdict( edict_t *pEdict );
const char *__cdecl SV_ClassName( const edict_t *e );
void __cdecl SV_SetModel( edict_t *ent, const char *name );
void __cdecl SV_CopyTraceToGlobal( trace_t *trace );
void __cdecl SV_SetMinMaxSize( edict_t *e, const float *min, const float *max );
edict_t *__cdecl SV_FindEntityByString( edict_t *pStartEdict, const char *pszField, const char *pszValue );
void __cdecl SV_PlaybackEventFull( int flags, const edict_t *pInvoker, word eventindex, float delay, float *origin,
	float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );
void __cdecl SV_PlaybackReliableEvent( sizebuf_t *msg, word eventindex, float delay, event_args_t *args );
void __cdecl SV_BaselineForEntity( edict_t *pEdict );
void __cdecl SV_WriteEntityPatch( const char *filename );
char *__cdecl SV_ReadEntityScript( const char *filename, int *flags );
float __cdecl SV_AngleMod( float ideal, float current, float speed );
void __cdecl SV_SpawnEntities( const char *mapname, char *entities );
edict_t *__cdecl SV_AllocPrivateData( edict_t *ent, string_t className );
string_t __cdecl SV_AllocString( const char *szValue );
string_t __cdecl SV_MakeString( const char *szValue );
const char *__cdecl SV_GetString( string_t iString );
sv_client_t *__cdecl SV_ClientFromEdict( const edict_t *pEdict, qboolean spawned_only );
void __cdecl SV_SetClientMaxspeed( sv_client_t *cl, float fNewMaxspeed );
int __cdecl SV_MapIsValid( const char *filename, const char *spawn_entity, const char *landmark_name );
void __cdecl SV_StartSound( edict_t *ent, int chan, const char *sample, float vol, float attn, int flags, int pitch );
void __cdecl SV_CreateStaticEntity( struct sizebuf_s *msg, sv_static_entity_t *ent );
edict_t *__cdecl pfnPEntityOfEntIndex( int iEntIndex );
int __cdecl pfnIndexOfEdict( const edict_t *pEdict );
void __cdecl SV_UpdateBaseVelocity( edict_t *ent );
byte *__cdecl pfnSetFatPVS( const float *org );
byte *__cdecl pfnSetFatPAS( const float *org );
int __cdecl pfnPrecacheModel( const char *s );
int __cdecl pfnNumberOfEntities( void );
void __cdecl SV_RestartStaticEnts( void );

_inline edict_t *__cdecl SV_EDICT_NUM( int n, const char * file, const int line )
{
	if((n >= 0) && (n < svgame.globals->maxEntities))
		return svgame.edicts + n;
	Host_Error( "SV_EDICT_NUM: bad number %i (called at %s:%i)\n", n, file, line );
	return NULL;	
}

//
// sv_save.c
//
void __cdecl SV_ClearSaveDir( void );
void __cdecl SV_SaveGame( const char *pName );
qboolean __cdecl SV_LoadGame( const char *pName );
void __cdecl SV_ChangeLevel( qboolean loadfromsavedgame, const char *mapname, const char *start );
int __cdecl SV_LoadGameState( char const *level, qboolean createPlayers );
void __cdecl SV_LoadAdjacentEnts( const char *pOldLevel, const char *pLandmarkName );
const char *__cdecl SV_GetLatestSave( void );
void __cdecl SV_InitSaveRestore( void );

//
// sv_pmove.c
//
void __cdecl SV_GetTrueOrigin( sv_client_t *cl, int edictnum, vec3_t origin );
void __cdecl SV_GetTrueMinMax( sv_client_t *cl, int edictnum, vec3_t mins, vec3_t maxs );

//
// sv_world.c
//
void __cdecl SV_ClearWorld( void );
void __cdecl SV_UnlinkEdict( edict_t *ent );
qboolean __cdecl SV_HeadnodeVisible( mnode_t *node, byte *visbits, int *lastleaf );
void __cdecl SV_ClipMoveToEntity( edict_t *ent, const vec3_t start, vec3_t mins, vec3_t maxs, const vec3_t end, trace_t *trace );
void __cdecl SV_CustomClipMoveToEntity( edict_t *ent, const vec3_t start, vec3_t mins, vec3_t maxs, const vec3_t end, trace_t *trace );
trace_t __cdecl SV_TraceHull( edict_t *ent, int hullNum, const vec3_t start, vec3_t mins, vec3_t maxs, const vec3_t end );
trace_t __cdecl SV_Move( const vec3_t start, vec3_t mins, vec3_t maxs, const vec3_t end, int type, edict_t *e );
trace_t __cdecl SV_MoveNoEnts( const vec3_t start, vec3_t mins, vec3_t maxs, const vec3_t end, int type, edict_t *e );
const char *__cdecl SV_TraceTexture( edict_t *ent, const vec3_t start, const vec3_t end );
trace_t __cdecl SV_MoveToss( edict_t *tossent, edict_t *ignore );
void __cdecl SV_LinkEdict( edict_t *ent, qboolean touch_triggers );
void __cdecl SV_TouchLinks( edict_t *ent, areanode_t *node );
int __cdecl SV_TruePointContents( const vec3_t p );
int __cdecl SV_PointContents( const vec3_t p );
void __cdecl SV_RunLightStyles( void );
void __cdecl SV_SetLightStyle( int style, const char* s, float f );
const char *__cdecl SV_GetLightStyle( int style );
int __cdecl SV_LightForEntity( edict_t *pEdict );
void __cdecl SV_ClearPhysEnts( void );

#endif//SERVER_H
