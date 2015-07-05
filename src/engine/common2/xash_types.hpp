#ifndef XASH_TYPES_HPP
#define XASH_TYPES_HPP


// ---------------------------------------- //
// ---------------- EXPORT ---------------- //
// ---------------------------------------- //

#ifdef XASH_LIB
	#define XASH_EXP __declspec(dllexport)
#else
	#define XASH_EXP __declspec(dllimport)
#endif


// ---------------------------------------- //
// --------------- HEADERS ---------------- //
// ---------------------------------------- //

#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QtGlobal>


// ---------------------------------------- //
// ---------------- DEFINES --------------- //
// ---------------------------------------- //

#define ANGLE_DOWN               -2
#define ANGLE_UP                 -1
#define ANIM_CYCLE               2
#define AREA_DEPTH               4
#define AREA_NODES               32
#define ATTN_IDLE                2.f
#define ATTN_NONE                0
#define ATTN_NORM                0.8f
#define ATTN_STATIC              1.25f
#define BACKFACE_EPSILON         0.01f
#define BIT( n )                 (1<<( n ))
#define BOUNCE_CONCRETE          BREAK_CONCRETE
#define BOUNCE_FLESH             BREAK_FLESH
#define BOUNCE_GLASS             BREAK_GLASS
#define BOUNCE_METAL             BREAK_METAL
#define BOUNCE_SHELL             0x20
#define BOUNCE_SHOTSHELL         0x80
#define BOUNCE_SHRAP             0x10
#define BOUNCE_WOOD              BREAK_WOOD
#define BREAK_2                  0x80
#define BREAK_CONCRETE           0x40
#define BREAK_FLESH              0x04
#define BREAK_GLASS              0x01
#define BREAK_METAL              0x02
#define BREAK_SMOKE              0x10
#define BREAK_TRANS              0x20
#define BREAK_TYPEMASK           0x4F
#define BREAK_WOOD               0x08
#define CHAN_AUTO                0
#define CHAN_BODY                4
#define CHAN_ITEM                3
#define CHAN_NETWORKVOICE_BASE   7 // voice data coming across the network
#define CHAN_NETWORKVOICE_END    500 // network voice data reserves slots (CHAN_NETWORKVOICE_BASE through CHAN_NETWORKVOICE_END).
#define CHAN_STATIC              6 // allocate channel from the static area
#define CHAN_STREAM              5 // allocate stream channel from the static or dynamic area
#define CHAN_VOICE               2
#define CHAN_WEAPON              1
#define check_angles( x )	     ( (int)x == 90 || (int)x == 180 || (int)x == 270 || (int)x == -90 || (int)x == -180 || (int)x == -270 )
#define CIN_LOGO                 1
#define CIN_MAIN                 0
#define CL_RENDER_INTERFACE_VERSION 35
#define CL_UPDATE_MASK           (CL_UPDATE_BACKUP - 1)
#define clc_bad                  0 // immediately drop client when received
#define clc_delta                4 // [quint8] sequence number, requests delta compression of message
#define clc_fileconsistency      7
#define clc_move                 2 // [[SUserCmd]
#define clc_nop                  1
#define clc_requestcvarvalue     9
#define clc_requestcvarvalue2    10
#define clc_resourcelist         5
#define clc_stringcmd            3 // [string] message
#define clc_userinfo             6 // [[userinfo string]
#define clc_voicedata            8
#define CLDLL_INTERFACE_VERSION  7
#define CMD_BACKUP               MULTIPLAYER_BACKUP // allow a lot of command backups for very fast systems
#define CMD_MASK                 (CMD_BACKUP - 1)
#define CMP_LZSS                 1 // LZSS compression
#define CMP_NONE                 0 // compression none
#define ColorIndex( c )	         ((( c ) - '0' ) & 7 )
#define CONTENT_EMPTY            -1
#define CONTENT_FLYFIELD         -17
#define CONTENT_FOG              -19
#define CONTENT_GRAVITY_FLYFIELD -18
#define CONTENT_LAVA             -5
#define CONTENT_SKY              -6
#define CONTENT_SLIME            -4
#define CONTENT_SOLID            -2
#define CONTENT_WATER            -3
#define CONTENTS_CLIP            -8 // changed to contents_solid
#define CONTENTS_CURRENT_0       -9
#define CONTENTS_CURRENT_180     -11
#define CONTENTS_CURRENT_270     -12
#define CONTENTS_CURRENT_90      -10
#define CONTENTS_CURRENT_DOWN    -14
#define CONTENTS_CURRENT_UP      -13
#define CONTENTS_EMPTY           -1
#define CONTENTS_LADDER          -16
#define CONTENTS_LAVA            -5
#define CONTENTS_NONE            0 // no custom contents specified
#define CONTENTS_ORIGIN          -7 // removed at csg time
#define CONTENTS_SKY             -6
#define CONTENTS_SLIME           -4
#define CONTENTS_SOLID           -2
#define CONTENTS_TRANSLUCENT     -15
#define CONTENTS_WATER           -3
#define CS_SIZE                  64 // size of one config string
#define CS_TIME                  16 // size of time string
#define DAMAGE_AIM               2
#define DAMAGE_NO                0
#define DAMAGE_YES               1
#define DEAD_DEAD                2 // dead. lying still.
#define DEAD_DISCARDBODY         4
#define DEAD_DYING               1 // playing death animation or still falling off of a ledge waiting to hit ground
#define DEAD_NO                  0 // alive
#define DEAD_RESPAWNABLE         3
#define DEFAULT_RATE             (9999.0f)
#define DELUXEMAP_VERSION        1
#define DIST_EPSILON             (1.0f / 32.0f)
#define DOS_SIGNATURE            0x5A4D // MZ
#define DVIS_PHS                 1
#define DVIS_PVS                 0
#define EF_BRIGHTFIELD           1 // swirling cloud of particles
#define EF_BRIGHTLIGHT           4 // DLIGHT centered at entity origin
#define EF_DIMLIGHT              8 // player flashlight
#define EF_FULLBRIGHT            (1<<27)	// Just get fullbright
#define EF_INVLIGHT              16 // get lighting from ceiling
#define EF_LIGHT                 64 // rocket flare glow sprite
#define EF_MERGE_VISIBILITY      (1<<29)	// this entity allowed to merge vis (e.g. env_sky or portal camera)
#define EF_MUZZLEFLASH           2 // single frame ELIGHT on entity attachment 0
#define EF_NODRAW                128 // don't draw entity
#define EF_NOINTERP              32 // don't interpolate the next frame
#define EF_NOREFLECT             (1<<24)	// Entity won't reflecting in mirrors
#define EF_NOSHADOW              (1<<28)	// ignore shadow for this entity
#define EF_NOWATERCSG            (1<<26)	// Do not remove sides for func_water entity
#define EF_REFLECTONLY           (1<<25)	// Entity will be drawing only in mirrors
#define EF_REQUEST_PHS           (1<<30)	// This entity requested phs bitvector instead of pvsbitvector in AddToFullPack calls
#define EFLAG_SLERP              1 // do studio interpolation of this entity
#define ENGINE_BUILD_SURFMESHES  (1<<1)	// bulid surface meshes that goes into mextrasurf->mesh. For mod makers and custom renderers
#define ENGINE_COMPENSATE_QUAKE_BUG (1<<5)	// compensate stupid quake bug (inverse pitch) for mods where this bug is fixed
#define ENGINE_COMPUTE_STUDIO_LERP (1<<7)	// enable MOVETYPE_STEP lerping back in engine
#define ENGINE_DISABLE_HDTEXTURES (1<<6)	// disable support of HD-textures in case custom renderer have separate way to load them
#define ENGINE_LARGE_LIGHTMAPS   (1<<4)	// change lightmap sizes from 128x128 to 256x256
#define ENGINE_LOAD_DELUXEDATA   (1<<2)	// loading deluxemap for map (if present)
#define ENGINE_TRANSFORM_TRACE_AABB (1<<3)	// transform trace bbox into local space of rotating bmodels
#define ENGINE_WRITE_LARGE_COORD (1<<0)	// replace standard message WRITE_COORD with big message for support more than 8192 units in world
#define ENTITY_SBeam              (1<<1)
#define ENTITY_NORMAL            (1<<0)
#define EVENT_API_VERSION        1
#define FCUST_FROMHPAK           ( 1<<0 )
#define FCUST_IGNOREINIT         ( 1<<2 )
#define FCUST_WIPEDATA           ( 1<<1 )
#define FDECAL_DONTSAVE          0x04 // Decal was loaded from adjacent level, don't save it for this level
#define FDECAL_LOCAL_SPACE       0x80 // decal is in local space (any decal after serialization)
#define FDECAL_PERMANENT         0x01 // This decal should not be removed in favor of any new decals
#define FDECAL_STUDIO            0x40 // Indicates a studio decal
#define FDECAL_USE_LANDMARK      0x02 // This is a decal applied on a bmodel without origin-brush so we done in absoulute pos
#define FDECAL_USESAXIS          0x20 // Uses the s axis field to determine orientation (footprints)
#define FEVENT_ANGLES            ( 1<<1 )
#define FEVENT_ORIGIN            ( 1<<0 )
#define FILE_BUFF_SIZE           2048
#define FL_ALWAYSTHINK           (1<<21)	// Brush model flag -- call think every frame regardless of nextthink - ltime (for constantly changing velocity/path)
#define FL_BASEVELOCITY          (1<<22)	// Base velocity has been applied this frame (used to convert base velocity into momentum)
#define FL_CLIENT                (1<<3)
#define FL_CONVEYOR              (1<<2)
#define FL_CUSTOMENTITY          (1<<29)	// This is a custom entity
#define FL_DORMANT               (1<<31)	// Entity is dormant, no updates to client
#define FL_DUCKING               (1<<14)	// Player flag -- Player is fully crouched
#define FL_FAKECLIENT            (1<<13)	// JAC: fake client, simulated server side; don't send network messages to them
#define FL_FLOAT                 (1<<15)	// Apply floating force to this entity when in water
#define FL_FLY                   (1<<0)	// Changes the SV_Movestep() behavior to not need to be on ground
#define FL_FROZEN                (1<<12)	// Player is frozen for 3rd person camera
#define FL_GODMODE               (1<<6)
#define FL_GRAPHED               (1<<16)	// worldgraph has this ent listed as something that blocks a connection
#define FL_IMMUNE_LAVA           (1<<19)
#define FL_IMMUNE_SLIME          (1<<18)
#define FL_IMMUNE_WATER          (1<<17)
#define FL_INWATER               (1<<4)
#define FL_KILLME                (1<<30)	// This entity is marked for death -- This allows the engine to kill ents at the appropriate time
#define FL_MONSTER               (1<<5)
#define FL_MONSTERCLIP           (1<<23)	// Only collide in with monsters who have FL_MONSTERCLIP set
#define FL_NOTARGET              (1<<7)
#define FL_ONGROUND              (1<<9)	// At rest / on the ground
#define FL_ONTRAIN               (1<<24)	// Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_PARTIALGROUND         (1<<10)	// not all corners are valid
#define FL_PROXY                 (1<<20)	// This is a spectator proxy
#define FL_SKIPLOCALHOST         (1<<8)	// Don't send entity to local host, it's predicting this entity itself
#define FL_SPECTATOR             (1<<26)	// This client is a spectator, don't run touch functions, etc.
#define FL_SWIM                  (1<<1)	// Changes the SV_Movestep() behavior to not need to be on ground (but stay in water)
#define FL_WATERJUMP             (1<<11)	// player jumping out of water
#define FL_WORLDBRUSH            (1<<25)	// Not moveable/removeable brush entity (really part of the world, but represented as an entity for transparency or something)
#define FLOW_INCOMING            1
#define FLOW_OUTGOING            0
#define FMOVE_IGNORE_GLASS       0x100
#define FMOVE_SIMPLEBOX          0x200
#define FNETAPI_MULTIPLE_RESPONSE ( 1<<0 )
#define FRAC_EPSILON             (1.0f / 1024.0f)
#define FRAG_FILE_STREAM         1
#define FRAG_NORMAL_STREAM       0
#define FRAGMENT_SIZE 		     1400
#define FS_GAMEDIR_PATH          4 // just a marker for gamedir path
#define FS_NOWRITE_PATH          2 // default behavior - last added gamedir set as writedir. This flag disables it
#define FS_STATIC_PATH           1 // FS_ClearSearchPath will be ignore this path
#define FTRACE_IGNORE_GLASS      (1<<1)	// traceline will be ignored entities with rendermode != kRenderNormal
#define FTRACE_SIMPLEBOX         (1<<0)	// Traceline with a simple box
#define HEADER_LUMPS             15
#define HEADER_quint8S           ( 8 + MAX_STREAMS * 9 )
#define HISTORY_MASK             ( HISTORY_MAX - 1 )
#define HISTORY_MAX              64 // Must be power of 2
#define HLBSP_VERSION            30 // half-life regular version
#define IDCUSTOM_VERSION         1
#define IDCUSTOMHEADER           (('K'<<24)+('A'<<16)+('P'<<8)+'H') // little-endian "HPAK"
#define IDDELUXEMAPHEADER        (('T'<<24)+('I'<<16)+('L'<<8)+'Q') // little-endian "QLIT"
#define IDPACKV1HEADER           (('K'<<24)+('C'<<16)+('A'<<8)+'P')	// little-endian "PACK"
#define IDWAD2HEADER             (('2'<<24)+('D'<<16)+('A'<<8)+'W')	// little-endian "WAD2" quake1 gfx.wad
#define IDWAD3HEADER             (('3'<<24)+('D'<<16)+('A'<<8)+'W')
#define IDWAD3HEADER             (('3'<<24)+('D'<<16)+('A'<<8)+'W')	// little-endian "WAD3" half-life wads
#define IN_ALT1                  (1<<14)
#define IN_ATTACK                (1<<0)
#define IN_ATTACK2               (1<<11)
#define IN_BACK                  (1<<4)
#define IN_CANCEL                (1<<6)
#define IN_DUCK                  (1<<2)
#define IN_FORWARD               (1<<3)
#define IN_JUMP                  (1<<1)
#define IN_LEFT                  (1<<7)
#define IN_MOVELEFT              (1<<9)
#define IN_MOVERIGHT             (1<<10)
#define IN_RELOAD                (1<<13)
#define IN_RIGHT                 (1<<8)
#define IN_RUN                   (1<<12)
#define IN_SCORE                 (1<<15)   // Used by client.dll for when scoreboard is held down
#define IN_USE                   (1<<5)
#define INFO_SURF( surf, mod )	 (mod->surfaces + (surf - (mextrasurf_t *)mod->cache.data))
#define INFO_SURF( surf, mod )	 (mod->surfaces + (surf - (mextrasurf_t *)mod->cache.data))
#define INVALID_HANDLE           0xFFFF // for XashXT cache system
#define IsColorString( p )	     ( p && *( p ) == '^' && *(( p ) + 1) && *(( p ) + 1) >= '0' && *(( p ) + 1 ) <= '9' )
#define LM_SAMPLE_SIZE           world.lm_sample_size // lightmap resoultion
#define LM_STYLES                4 // MAXLIGHTMAPS
#define LS_NONE                  0xFF
#define LS_NORMAL                0x00
#define LS_UNUSED                0xFE
#define LUMP_CLIPNODES           9
#define LUMP_CLIPNODES2          15 // hull0 goes into LUMP_NODES, hull1 goes into LUMP_CLIPNODES,
#define LUMP_CLIPNODES3          16 // hull2 goes into LUMP_CLIPNODES2, hull3 goes into LUMP_CLIPNODES3
#define LUMP_EDGES               12
#define LUMP_ENTITIES            0
#define LUMP_FACES               7
#define LUMP_LEAFS               10
#define LUMP_LIGHTING            8
#define LUMP_MARKSURFACES        11
#define LUMP_MODELS              14 // internal submodels
#define LUMP_NODES               5
#define LUMP_PLANES              1
#define LUMP_SURFEDGES           13
#define LUMP_TEXINFO             6
#define LUMP_TEXTURES            2 // internal textures
#define LUMP_VERTEXES            3
#define LUMP_VISIBILITY          4
#define MASTERSERVER_ADR         "hl1master.steampowered.com:27010"
#define MAX_ALIAS_NAME           32
#define MAX_BACKUP_COMMANDS      (1 << NUM_BACKUP_COMMAND_BITS)
#define MAX_BOX_LEAFS            256
#define MAX_CDTRACKS             32
#define MAX_CLIENT_BITS          5
#define MAX_CLIENTS              (1<<MAX_CLIENT_BITS)// 5 bits == 32 clients ( int32 limit )
#define MAX_CLIP_PLANES          5
#define MAX_CMD_TOKENS           80 // cmd tokens
#define MAX_CUSTOM               1024 // max custom resources per level
#define MAX_DECALS               512 // touching TE_DECAL messages, etc
#define MAX_DEMOS                32
#define MAX_DLIGHTS              32 // dynamic lights (rendered per one frame)
#define MAX_EDICTS               (1<<MAX_ENTITY_BITS)// 12 bits = 4096 edicts
#define MAX_EFRAGS               1024
#define MAX_ELIGHTS              64 // entity only point lights
#define MAX_ENT_LEAFS            48
#define MAX_ENTITY_BITS          12
#define MAX_ENTNUMBER            99999 // for server and client parsing
#define MAX_EVENT_BITS           10
#define MAX_EVENT_QUEUE          64 // 16 simultaneous events, max
#define MAX_EVENTS               (1<<MAX_EVENT_BITS)	// 10 bits == 1024 events (the original Half-Life limit)
#define MAX_FILES_IN_PACK        65536 // pak
#define MAX_FILES_IN_WAD         8192
#define MAX_FLOWS                2
#define MAX_FPS                  500.0f // upper limit for maxfps.
#define MAX_FRAMETIME            0.1f
#define MAX_HEARTBEAT            -99999 // connection time
#define MAX_IMAGES               256 // SpriteTextures
#define MAX_INFO_STRING          256
#define MAX_INFO_STRING          256 // infostrings are transmitted across network
#define MAX_LATENT               32
#define MAX_LIBRARY_EXPORTS      4096
#define MAX_LIGHTSTYLES          256 // a quint8 limit, don't modify
#define MAX_MAP_BRUSHES          32768 // quint16 limit
#define MAX_MAP_CLIPNODES        32767 // because negative shorts are contents
#define MAX_MAP_EDGES            0x100000 // can be increased but not needed
#define MAX_MAP_ENTITIES         8192 // can be increased up to 32768 if needed
#define MAX_MAP_ENTSTRING        0x80000 // 512 kB should be enough
#define MAX_MAP_FACES            65535 // quint16 limit
#define MAX_MAP_HULLS            4 // MAX_HULLS
#define MAX_MAP_LEAFS            32767 // signed short limit
#define MAX_MAP_LIGHTING         0x2000000 // 32 Mb lightmap raw data (can contain deluxemaps)
#define MAX_MAP_MARKSURFACES     65535 // quint16 limit
#define MAX_MAP_MIPTEX           0x2000000 // 32 Mb internal textures data
#define MAX_MAP_MODELS           1024 // can be increased up to 2048 if needed
#define MAX_MAP_NODES            32767 // because negative shorts are leafs
#define MAX_MAP_PLANES           65536 // can be increased without problems
#define MAX_MAP_SURFEDGES        0x200000 // can be increased but not needed
#define MAX_MAP_TEXINFO          MAX_MAP_FACES // in theory each face may have personal texinfo
#define MAX_MAP_TEXTURES         2048 // can be increased but not needed
#define MAX_MAP_VERTS            65535 // quint16 limit
#define MAX_MAP_VISIBILITY       0x800000 // 8 Mb visdata
#define MAX_MODEL_BITS           11
#define MAX_MODELS               (1<<MAX_MODEL_BITS)	// 11 bits == 2048 models
#define MAX_MODS                 512 // environment games that engine can keep visible
#define MAX_MOVEENTS             64
#define MAX_MOVIES               8
#define MAX_NUM_ARGVS            128
#define MAX_PHYSENTS             600 // Must have room for all entities in the world.
#define MAX_PHYSINFO_STRING      256
#define MAX_RATE                 20000
#define MAX_RENDER_DECALS        4096 // max rendering decals per a level
#define MAX_REQUESTS             32
#define MAX_RESOURCES            (MAX_MODELS+MAX_SOUNDS+MAX_CUSTOM+MAX_EVENTS)
#define MAX_SCOREBOARDNAME       32
#define MAX_SOUND_BITS           11
#define MAX_SOUNDS               (1<<MAX_SOUND_BITS)	// 11 bits == 2048 sounds
#define MAX_STATIC_ENTITIES      512 // static entities that moved on the client when level is spawn
#define MAX_STREAMS              2
#define MAX_STRING               256 // generic string
#define MAX_STUDIO_DECALS        4096 // + unused space of BSP decals
#define MAX_SYSPATH              1024 // system filepath
#define MAX_TOTAL_ENT_LEAFS      128
#define MAX_USER_MESSAGES        191 // another 63 messages reserved for engine routines
#define MAX_VISIBLE_PACKET       512 // 512 visible entities per frame (hl1 has 256)
#define MAX_WEAPON_BITS          5
#define MAX_WEAPONS              (1<<MAX_WEAPON_BITS)// 5 bits == 32 weapons ( int32 limit )
#define MAXLIGHTMAPS             4
#define MIN_FPS                  15.0f // host minimum fps value for maxfps.
#define MIN_FRAMETIME            0.001f
#define MIN_RATE                 1000
#define MIPLEVELS                4
#define MODEL_CONVEYOR BIT       ( 0 )
#define MODEL_HAS_ORIGIN BIT     ( 1 )
#define MODEL_LIQUID BIT         ( 2 )	// model has only point hull
#define MOVE_MISSILE             2 // extra size for monsters
#define MOVE_NOMONSTERS          1 // ignore monsters (edicts with flags (FL_MONSTER|FL_FAKECLIENT|FL_CLIENT) set)
#define MOVE_NORMAL              0 // normal trace
#define MOVETYPE_ANGLECLIP       2
#define MOVETYPE_ANGLENOCLIP     1
#define MOVETYPE_BOUNCE          10 // Just like Toss, but reflect velocity when contacting surfaces
#define MOVETYPE_BOUNCEMISSIL    E 11 // bounce w/o gravity
#define MOVETYPE_COMPOUND        14 // glue two entities together (simple movewith)
#define MOVETYPE_FLY             5 // No gravity, but still collides with stuff
#define MOVETYPE_FLYMISSILE      9 // extra size to monsters
#define MOVETYPE_FOLLOW          12 // track movement of aiment
#define MOVETYPE_NOCLIP          8 // No gravity, no collisions, still do velocity/avelocity
#define MOVETYPE_NONE            0 // never moves
#define MOVETYPE_PUSH            7 // no clip to world, push and crush
#define MOVETYPE_PUSHSTEP        13 // BSP model that needs physics/world collisions (uses nearest hull for world collision)
#define MOVETYPE_STEP            4 // gravity, special edge handling -- monsters use this
#define MOVETYPE_TOSS            6 // gravity/collisions
#define MOVETYPE_WALK            3 // Player only - moving on the ground
#define MSG_ALL                  2 // reliable to all
#define MSG_BROADCAST            0 // unreliable to all
#define MSG_INIT                 3 // write to the init string
#define MSG_ONE                  1 // reliable to one (msg_entity)
#define MSG_ONE_UNRELIABLE       8 // Send to one client, but don't put in reliable stream, put in unreliable datagram ( could be dropped )
#define MSG_PAS                  5 // Ents in PAS of org
#define MSG_PAS_R                7 // Reliable to PAS
#define MSG_PVS                  4 // Ents in PVS of org
#define MSG_PVS_R                6 // Reliable to PVS
#define MSG_SPEC                 9 // Sends to all spectator proxies
#define MULTIPLAYER_BACKUP       64 // how many data slots to use when in multiplayer (must be power of 2)
#define NET_ERROR_PROTO_UNSUPPORTED ( 1<<1 )
#define NET_ERROR_TIMEOUT        ( 1<<0 )
#define NET_ERROR_UNDEFINED      ( 1<<2 )
#define NET_MAX_MESSAGE PAD_NUMBER(( NET_MAX_PAYLOAD + HEADER_quint8S ), 16 )
#define NET_MAX_PAYLOAD          80000
#define NET_SUCCESS              ( 0 )
#define NETAPI_REQUEST_DETAILS   ( 4 )
#define NETAPI_REQUEST_PING      ( 1 )
#define NETAPI_REQUEST_PLAYERS   ( 3 )
#define NETAPI_REQUEST_RULES     ( 2 )
#define NETAPI_REQUEST_SERVERLIST ( 0 )  // Doesn't need a remote address
#define NT_SIGNATURE             0x00004550 // PE00
#define NUM_BACKUP_COMMAND_BITS  4
#define NUMBER_OF_DIRECTORY_ENTRIES 16
#define OBS_CHASE_FREE           2
#define OBS_CHASE_LOCKED         1
#define OBS_IN_EYE               4
#define OBS_MAP_CHASE            6
#define OBS_MAP_FREE             5
#define OBS_NONE                 0
#define OBS_ROAMING              3
#define PAD_NUMBER( num, boundary )	((( num ) + (( boundary ) - 1 )) / ( boundary )) * ( boundary )
#define PAK_LOAD_BAD_FOLDERS     3
#define PAK_LOAD_BAD_HEADER      2
#define PAK_LOAD_CORRUPTED       6
#define PAK_LOAD_COULDNT_OPEN    1
#define PAK_LOAD_NO_FILES        5
#define PAK_LOAD_OK              0
#define PAK_LOAD_TOO_MANY_FILES  4
#define PARM_ACTIVE_TMU          27 // for debug
#define PARM_CACHEFRAME          28
#define PARM_CLIENT_ACTIVE       30
#define PARM_CLIENT_INGAME       25
#define PARM_FEATURES            26 // same as movevars->features
#define PARM_FULLSCREEN          22
#define PARM_MAP_HAS_DELUXE      19 // map has deluxedata
#define PARM_MAP_HAS_MIRRORS     18 // current map has mirorrs
#define PARM_MAX_ENTITIES        20
#define PARM_MAX_IMAGE_UNITS     29
#define PARM_SCREEN_HEIGHT       24
#define PARM_SCREEN_WIDTH        23
#define PARM_SKY_SPHERE          17 // sky is quake sphere ?
#define PARM_TEX_CACHEFRAME      12 // compare with worldmodel->needload
#define PARM_TEX_FLAGS           10
#define PARM_TEX_HEIGHT          2 // otherwise it's not used
#define PARM_TEX_LIGHTMAP        7 // second arg as number 0 - 128
#define PARM_TEX_SKYBOX          5 // second arg as skybox ordering num
#define PARM_TEX_SKYTEXNUM       6 // skytexturenum for quake sky
#define PARM_TEX_SRC_HEIGHT      4
#define PARM_TEX_SRC_WIDTH       3
#define PARM_TEX_TARGET          8
#define PARM_TEX_TEXNUM          9
#define PARM_TEX_TYPE            11
#define PARM_TEX_WIDTH           1 // all parms with prefix 'TEX_' receive arg as texnum
#define PARM_WIDESCREEN          21
#define PARM_WORLD_VERSION       16 // return the version of bsp
#define PIC_KEEP_8BIT            (1<<3)		// keep original 8-bit image (if present)
#define PIC_KEEP_RGBDATA         (1<<1)		// some images keep source
#define PIC_NEAREST              (1<<0)		// disable texfilter
#define PIC_NOFLIP_TGA           (1<<2)		// Steam background completely ignore tga attribute 0x20
#define PITCH_HIGH               120
#define PITCH_LOW                95 // other values are possible - 0-255, where 255 is very high
#define PITCH_NORM               100 // non-pitch shifted
#define PLAT_LOW_TRIGGER         1
#define PLATE_HUE_END            191
#define PLATE_HUE_START          160
#define PM_CUSTOM_IGNORE         0x00000010 // Ignore entities with SOLID_CUSTOM mode
#define PM_GLASS_IGNORE          0x00000004 // Ignore entities with non-normal rendermode
#define PM_NORMAL                0x00000000
#define PM_STUDIO_BOX            0x00000002 // Use boxes for non-complex studio models (even in traceline)
#define PM_STUDIO_IGNORE         0x00000001 // Skip studio models
#define PM_TRACELINE_ANYVISIBLE  1
#define PM_TRACELINE_PHYSENTSONLY 0
#define PM_WORLD_ONLY            0x00000008 // Only trace against the world
#define PORT_CLIENT              27005
#define PORT_MASTER              27010
#define PORT_SERVER              27015
#define PROTOCOL_VERSION         48
#define Q1BSP_VERSION            29 // quake1 regular version (beta is 28)
#define QCHAR_WIDTH              16 // font width
#define RES_CUSTOM               (1<<2)	// Is this resource one that corresponds to another player's customization
#define RES_FATALIFMISSING       (1<<0)	// Disconnect if we can't get this file.
#define RES_PRECACHED            (1<<4)	// Already precached
#define RES_REQUESTED            (1<<3)	// Already requested a download of this one
#define RES_WASMISSING           (1<<1)	// Do we have the file locally, did we get it ok?
#define SCRINFO_SCREENFLASH      1
#define SCRINFO_STRETCHED        2
#define SF_TRAIN_PASSABLE        8 // Train is not solid -- used to make water trains
#define SF_TRAIN_START_ON        4 // Train is initially moving
#define SF_TRAIN_WAIT_RETRIGGER  1
#define SINGLEPLAYER_BACKUP      16 // same for single player
#define SND_ATTENUATION          (1<<1)	// a quint8
#define SND_CHANGE_PITCH         (1<<7)	// change sound pitch
#define SND_CHANGE_VOL           (1<<6)	// change sound vol
#define SND_LARGE_INDEX          (1<<2)	// a send sound as short
#define SND_PITCH                (1<<3)	// a quint8
#define SND_PMOVESTEP            (1<<9)	//MAV: pmove stepsound on server (ignored for local client)
#define SND_SENTENCE             (1<<4)	// set if sound num is actually a sentence num
#define SND_SPAWNING             (1<<8)	// we're spawning, used in some cases for ambients (not sent across network)
#define SND_STOP                 (1<<5)	// stop the sound
#define SND_VOLUME               (1<<0)	// a scaled quint8
#define SOLID_BBOX               2 // touch on edge, block
#define SOLID_BSP                4 // bsp clip, touch on edge, block
#define SOLID_CUSTOM             5 // call external callbacks for tracing
#define SOLID_NOT                0 // no interaction with other objects
#define SOLID_SLIDEBOX           3 // touch on edge, but not an onground
#define SOLID_TRIGGER            1 // touch on edge, but not blocking
#define STUDIO_EVENTS            2
#define STUDIO_RENDER            1
#define SUIT_HUE_END             223
#define SUIT_HUE_START           192
#define SURF_CONVEYOR            BIT( 6 ) // scrolled texture (was SURF_DRAWBACKGROUND)
#define SURF_DRAWSKY             BIT( 2 ) // sky surface
#define SURF_DRAWTILED           BIT( 5 ) // face without lighmap
#define SURF_DRAWTURB            BIT( 4 ) // warp surface
#define SURF_INFO( surf, mod )	 ((mextrasurf_t *)mod->cache.data + (surf - mod->surfaces))
#define SURF_INFO( surf, mod )	 ((mextrasurf_t *)mod->cache.data + (surf - mod->surfaces))
#define SURF_NOCULL              BIT( 0 ) // two-sided polygon (e.g. 'water4b')
#define SURF_PLANEBACK           BIT( 1 ) // plane should be negated
#define SURF_REFLECT             BIT( 31 ) // reflect surface (mirror)
#define SURF_TRANSPARENT         BIT( 8 ) // it's a transparent texture (was SURF_DONTWARP)
#define SURF_UNDERWATER          BIT( 7 ) // caustics
#define SURF_WATERCSG            BIT( 3 ) // culled by csg (was SURF_DRAWSPRITE)
#define svc_addangle             38 // [angle] add angles when client turn on mover
#define svc_ambientsound         29 // <see code>
#define svc_bad                  0 // immediately crash client when received
#define svc_bspdecal             36 // [float*3][short][short][short]
#define svc_cdtrack              32 // [string] trackname
#define svc_centerprint          26 // [string] to put in center of the screen
#define svc_changing             3 // changelevel by server request
#define svc_chokecount           42 // [quint8]
#define svc_clientdata           15 // [...]
#define svc_crosshairangle       47 // [quint8][quint8]
#define svc_customization        45 // <see code>
#define svc_deltamovevars        44 // [SMoveVars]
#define svc_deltapacketentities  41 // [short][quint8][...]
#define svc_deltatable           14 // [table header][...]
#define svc_director             51 // <variable sized>
#define svc_disconnect           2 // kick client from server
#define svc_event                27 // playback event queue
#define svc_event_reliable       21 // playback event directly from message, not queue
#define svc_eventindex           34 // [index][eventname]
#define svc_intermission         30 // empty message (event)
#define svc_lastmsg              64 // start user messages at this point
#define svc_lightstyle           12 // [index][pattern][float]
#define svc_modelindex           31 // [index][modelpath]
#define svc_nop                  1 // does nothing
#define svc_packetentities       40 // [short][...]
#define svc_particle             18 // [float*3][char*3][quint8][quint8]
#define svc_print                8 // [quint8] id [string] null terminated string
#define svc_querycvarvalue       57 // [string]
#define svc_querycvarvalue2      58 // [string][long] (context)
#define svc_resourcelist         43 // [short][...]
#define svc_restoresound         19 // <see code>
#define svc_roomtype             37 // [short] room type
#define svc_serverdata           11 // [long] protocol ...
#define svc_serverinfo           33 // [string] key [string] value
#define svc_setangle             10 // [angle angle angle] set the view angle to this absolute value
#define svc_setpause             24 // [quint8] 0 = unpaused, 1 = paused
#define svc_setview              5 // [short] entity number
#define svc_signonnum            25 // [quint8] used for the signon sequence
#define svc_sound                6 // <see code>
#define svc_soundfade            48 // [float*4] sound fade parms
#define svc_soundindex           28 // [index][soundpath]
#define svc_spawnbaseline        22 // <see code>
#define svc_spawnstatic          20 // creates a static client entity
#define svc_stopsound            16 // <see code>
#define svc_studiodecal          52 // [float*3][float*3][short][short][quint8]
#define svc_stufftext            9 // [string] stuffed into client's console buffer
#define svc_temp_entity          23 // <variable sized>
#define svc_time                 7 // [float] server time
#define svc_updatepings          17 // [bit][idx][ping][packet_loss]
#define svc_updateuserinfo       13 // [quint8] playernum, [string] userinfo
#define svc_usermessage          39 // [quint8][quint8][string] REG_USER_MSG stuff
#define svc_version              4 // [long] server version
#define svc_weaponanim           35 // [quint8]iAnim [quint8]body
#define TE_ARMOR_RICOCHET        111 // quick spark sprite, client ricochet sound.
#define TE_SBeam                  16 // obsolete
#define TE_SBeamCYLINDER          21 // cylinder that expands to max radius over lifetime
#define TE_SBeamDISK              20 // disk that expands to max radius over lifetime
#define TE_SBeamENTPOINT          1 // beam effect between point and entity
#define TE_SBeamENTS              8
#define TE_SBeamFOLLOW            22 // create a line of decaying beam segments until entity stops moving
#define TE_SBeamHOSE              26 // obsolete
#define TE_SBeamPOINTS            0 // beam effect between two points
#define TE_SBeamRING              24 // connect a beam ring to two entities
#define TE_SBeamSPRITE            18 // A beam with a sprite at the end
#define TE_SBeamTORUS             19 // screen aligned beam ring, expands to max radius over lifetime
#define TE_BLOOD                 103 // particle spray
#define TE_BLOODSPRITE           115 // spray of opaque sprite1's that fall, single sprite2 for 1..2 secs (this is a high-priority tent)
#define TE_BLOODSTREAM           101 // particle spray
#define TE_BOUNCE_NULL           0
#define TE_BOUNCE_SHELL          1
#define TE_BOUNCE_SHOTSHELL      2
#define TE_BOX                   31
#define TE_BREAKMODEL            108 // box of models or sprites
#define TE_BSPDECAL              13 // Decal from the .BSP file
#define TE_BUBBLES               113 // create alpha sprites inside of box, float upwards
#define TE_BUBBLETRAIL           114 // create alpha sprites along a line, float upwards
#define TE_DECAL                 104 // Decal applied to a brush entity (not the world)
#define TE_DECALHIGH             118 // Same as TE_DECAL, but the texture index was greater than 256
#define TE_DLIGHT                27 // dynamic light, effect world, minor entity effect
#define TE_ELIGHT                28 // point entity light, no world effect
#define TE_EXPLFLAG_DRAWALPHA    16 // sprite will be drawn alpha
#define TE_EXPLFLAG_NOADDITIVE   1 // sprite will be drawn opaque (ensure that the sprite you send is a non-additive sprite)
#define TE_EXPLFLAG_NODLIGHTS    2 // do not render dynamic lights
#define TE_EXPLFLAG_NONE         0 // all flags clear makes default Half-Life explosion
#define TE_EXPLFLAG_NOPARTICLES  8 // do not draw particles
#define TE_EXPLFLAG_NOSOUND      4 // do not play client explosion sound
#define TE_EXPLFLAG_ROTATE       32 // rotate the sprite randomly
#define TE_EXPLODEMODEL          107 // spherical shower of models, picks from set
#define TE_EXPLOSION             3 // additive sprite, 2 dynamic lights, flickering particles, explosion sound, move vertically 8 pps
#define TE_EXPLOSION2            12 // Quake1 colormaped (base palette) particle explosion with sound
#define TE_FIREFIELD             123 // makes a field of fire.
#define TE_FIZZ                  105 // create alpha sprites inside of entity, float upwards
#define TE_GLOWSPRITE            23
#define TE_GUNSHOT               2 // particle effect plus ricochet sound
#define TE_GUNSHOTDECAL          109 // decal and ricochet sound
#define TE_IMPLOSION             14 // tracers moving toward a point
#define TE_KILLSBeam              99 // kill all beams attached to entity
#define TE_KILLPLAYERATTACHMENTS 125 // will expire all TENTS attached to a player.
#define TE_LARGEFUNNEL           100
#define TE_LAVASPLASH            10 // Quake1 lava splash
#define TE_LIGHTNING             7 // TE_SBeamPOINTS with simplified parameters
#define TE_LINE                  30
#define TE_MODEL                 106 // create a moving model that bounces and makes a sound when it hits
#define TE_MULTIGUNSHOT          126 // much more compact shotgun message
#define TE_PARTICLEBURST         122 // very similar to lavasplash.
#define TE_PLAYERATTACHMENT      124 // attaches a TENT to a player (this is a high-priority tent)
#define TE_PLAYERDECAL           112 // ???
#define TE_PLAYERSPRITES         121 // sprites emit from a player's bounding box (ONLY use for players!)
#define TE_PROJECTILE            119 // Makes a projectile (like a nail) (this is a high-priority tent)
#define TE_SHOWLINE              102 // line of particles every 5 units, dies in 30 seconds
#define TE_SMOKE                 5 // alphablend sprite, move vertically 30 pps
#define TE_SPARKS                9 // 8 random tracers with gravity, ricochet sprite
#define TE_SPRAY                 120 // Throws a shower of sprites or models
#define TE_SPRITE                17 // additive sprite, plays 1 cycle
#define TE_SPRITE_SPRAY          110 // spay of alpha sprites
#define TE_SPRITETRAIL           15 // line of moving glow sprites with gravity, fadeout, and collisions
#define TE_STREAK_SPLASH         25 // oriented shower of tracers
#define TE_TAREXPLOSION          4 // Quake1 "tarbaby" explosion with sound
#define TE_TELEPORT              11 // Quake1 teleport splash
#define TE_TEXTMESSAGE           29
#define TE_TRACER                6 // tracer effect from point to point
#define TE_USERTRACER            127 // larger message than the standard tracer, but allows some customization.
#define TE_WORLDDECAL            116 // Decal applied to the world brush
#define TE_WORLDDECALHIGH        117 // Decal (with texture index > 256) applied to world brush
#define TEFIRE_FLAG_ALLFLOAT     1 // all sprites will drift upwards as they animate
#define TEFIRE_FLAG_ALPHA        8 // if set, sprite is rendered alpha blended at 50% else, opaque
#define TEFIRE_FLAG_LOOP         4 // if set, sprite plays at 15 fps, otherwise plays at whatever rate stretches the animation over the sprite's duration.
#define TEFIRE_FLAG_PLANAR       16 // if set, all fire sprites have same initial Z instead of randomly filling a cube.
#define TEFIRE_FLAG_SOMEFLOAT    2 // some of the sprites will drift upwards. (50% chance)
#define TEX_SPECIAL              BIT( 0 ) // sky or slime, no lightmap or 256 subdivision
#define TYP_ANY                  -1 // any type can be accepted
#define TYP_MIPTEX               67 // half-life (mip_t) previous was TYP_SOUND but never used in quake1
#define TYP_NONE                 0 // unknown lump type
#define TYP_QFONT                70 // half-life font (qfont_t)
#define TYP_QMIP                 68 // quake1 (mip_t) (replaced with TYPE_MIPTEX while loading)
#define TYP_QPAL                 64 // quake palette
#define TYP_QPIC                 66 // quake1 and hl pic (lmp_t)
#define TYP_QTEX                 65 // probably was never used
#define TYP_RAW                  69 // raw data
#define VERTEXSIZE               7
#define VID_LEVELSHOT            1
#define VID_MAPSHOT              3 // special case for overview layer
#define VID_MINISHOT             2
#define VID_SCREENSHOT           0
#define VID_SNAPSHOT             4 // save screenshot into root dir and no gamma correction
#define VOL_NORM                 1.0f
#define WAD3_NAMELEN             16
#define WALKMOVE_CHECKONLY       2 // move, but don't touch triggers
#define WALKMOVE_NORMAL          0 // normal walkmove
#define WALKMOVE_WORLDONLY       1 // doesn't hit ANY entities, no matter what the solid type
#define XASH_VERSION             0.96f // engine current version
#define XTBSP_VERSION            31 // extended lightmaps and expanded clipnodes limit
#define ZISCALE                  (static_cast<float>(0x8000))
#define FSBeam_STARTENTITY		0x00000001
#define FSBeam_ENDENTITY		0x00000002
#define FSBeam_FADEIN		0x00000004
#define FSBeam_FADEOUT		0x00000008
#define FSBeam_SINENOISE		0x00000010
#define FSBeam_SOLID			0x00000020
#define FSBeam_SHADEIN		0x00000040
#define FSBeam_SHADEOUT		0x00000080
#define FSBeam_STARTVISIBLE		0x10000000 // Has this client actually seen this beam's start entity yet?
#define FSBeam_ENDVISIBLE		0x20000000 // Has this client actually seen this beam's end entity yet?
#define FSBeam_ISACTIVE		0x40000000
#define FSBeam_FOREVER		0x80000000
#define SV_BLENDING_INTERFACE_VERSION 1
#define STUDIO_INTERFACE_VERSION 1

// ---------------------------------------- //
// ---------------- FORWARD --------------- //
// ---------------------------------------- //

struct ISvBlendingInterface;
struct IStudioInterface;
struct IServerStudioApi;
struct IEngineStudioApi;
struct ICldllFunc;
struct IClEngineFuncs;
struct IDemoApi;
struct IEfxApi;
struct IEngineStudioApi;
struct IEventApi;
struct INetApi;
struct IRenderApi;
struct IRenderInterface;
struct ITriangleApi;
struct IUiEngineFuncs;
struct IUiFunctions;
struct IVoiceTweak_s;
struct SAlight;
struct SAuxVert;
struct SBpcDesc;
struct SCacheUser;
struct SCenterPrint;
struct SClEntity;
struct SClFont;
struct SClgameStatic;
struct SClient;
struct SClientData;
struct SClientDataShort;
struct SClientDraw;
struct SClientSprite;
struct SClientStatic;
struct SClientTextMessage;
struct SClUserEvent;
struct SClUserMessage;
struct SCmd;
struct SCmdAlias;
struct SCmdBuf;
struct SColor24;
struct SColorVec;
struct SConNprint;
struct SConsole;
struct SConVar;
struct SCustomization;
struct SCvar;
struct SCvar;
struct SCvar;
struct SDclipNode;
struct SDecal;
struct SDecal;
struct SDecalItem;
struct SDecalList;
struct SDedge;
struct SDface;
struct SDheader;
struct SDleaf;
struct SDlight;
struct SDllFunc;
struct SDllInfo;
struct SDlump;
struct SDlumpInfo;
struct SDmiptexLump;
struct SDmodel;
struct SDnode;
struct SDplane;
struct SDtexInfo;
struct SDvertex;
struct SDwadInfo;
struct SEdict;
struct SEfrag;
struct SEntity;
struct SEntityState;
struct SEntVars;
struct SEventArgs;
struct SEventInfo;
struct SEventState;
struct SField;
struct SFile;
struct SFlow;
struct SFlowStats;
struct SFragBuf;
struct SFragBufWaiting;
struct SFrame;
struct SGameInfo;
struct SGameuiDraw;
struct SGlpoly;
struct SGlvert;
struct SHostParams;
struct SHostRedirect;
struct SHpak;
struct SHpakContainer;
struct SHpakDir;
struct SHpakHeader;
struct SHudPlayerInfo;
struct SHull;
struct SHull;
struct SImgFilter_s;
struct SLatchedVars;
struct SLeafList;
struct SLightStyle;
struct SLink;
struct SLmp;
struct SLocalState;
struct SMd5Context;
struct SMedge;
struct SMenuStatic;
struct SMextraSurf;
struct SMip;
struct SMleaf;
struct SMnode;
struct SModel;
struct SModelState;
struct SModInfo;
struct SMouth;
struct SMoveVars;
struct SMplane;
struct SMsprite;
struct SMspriteFrame;
struct SMspriteFrameDesc;
struct SMspriteGroup;
struct SMstudioEvent;
struct SMstudioTex;
struct SMsurface;
struct SMsurfMesh;
struct SMtexInfo;
struct SMvertex;
struct SNetadr;
struct SNetAdrList;
struct SNetChan;
struct SNetRequest;
struct SNetResponce;
struct SNetStatus;
struct SNotify;
struct SPack;
struct SPackfile;
struct SPackHeader;
struct SPhysEnt;
struct SPlane;
struct SPlayerInfo;
struct SPlayerMove;
struct SPmplane;
struct SPmtrace;
struct SPositionHistory;
struct SRefOverview;
struct SRefParams;
struct SRemapInfo;
struct SResource;
struct SResourceInfo;
struct SRgbData;
struct SScreenFade;
struct SScreenInfo;
struct SScreenShake;
struct SSearch;
struct SSearchPath;
struct SSizeBuf;
struct SSoundItem;
struct SSysInfo;
struct STagPoint;
struct STempEnt;
struct STexture;
struct STrace;
struct SUiGlobalVars;
struct SUserCmd;
struct SWadList;
struct SWadType;
struct SWavData;
struct SWeaponData;
struct SWfile;
struct SWorldStatic;
struct SWrect;


// ---------------------------------------- //
// ---------------- ENUMS ----------------- //
// ---------------------------------------- //

enum ENetAdrType
{
	NA_UNUSED,
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
	NA_IPX,
	NA_BROADCAST_IPX
};

enum EConnState
{
	ca_uninitialized = 0,
	ca_disconnected, 	// not talking to a server
	ca_connecting,	// sending request packets to the server
	ca_connected,	// SNetChan established, waiting for svc_serverdata
	ca_active,	// game views should be displayed
	ca_cinematic,	// playing a cinematic, not connected to a server
};

enum EScrShot
{
	scrshot_inactive,
	scrshot_normal,	// in-game screenshot
	scrshot_snapshot,	// in-game snapshot
	scrshot_plaque,  	// levelshot
	scrshot_savegame,	// saveshot
	scrshot_demoshot,	// for demos preview
	scrshot_envshot,	// cubemap view
	scrshot_skyshot,	// skybox view
	scrshot_mapshot // overview layer
};

// client screen state
enum EScrState
{
	CL_LOADING = 1,	// draw loading progress-bar
	CL_ACTIVE,	// draw normal hud
	CL_PAUSED,	// pause when active
	CL_CHANGELEVEL,	// draw 'loading' during changelevel
};


enum ESkyboxSide
{
	// skybox ordering
	SKYBOX_RIGHT	= 0,
	SKYBOX_BACK,
	SKYBOX_LEFT,
	SKYBOX_FORWARD,
	SKYBOX_UP,
	SKYBOX_DOWN,
};

enum ETexType
{
	TEX_INVALID = 0,	// free slot
	TEX_SYSTEM,	// generated by engine
	TEX_NOMIP,	// hud pics, menu etc
	TEX_BRUSH,	// a map texture
	TEX_SPRITE,	// sprite frames
	TEX_STUDIO,	// studio skins
	TEX_LIGHTMAP,	// lightmap textures
	TEX_DECAL,	// decals
	TEX_VGUI,		// vgui fonts or images
	TEX_CUBEMAP,	// cubemap textures (sky)
	TEX_DETAIL,	// detail textures
	TEX_REMAP,	// local copy of remap texture
	TEX_SCREENCOPY,	// keep screen copy e.g. for mirror
	TEX_CUSTOM,	// user created texture
	TEX_DEPTHMAP // shadowmap texture
};

enum ETexFlags
{
	TF_NEAREST	= (1<<0),		// disable texfilter
	TF_KEEP_RGBDATA	= (1<<1),		// some images keep source
	TF_NOFLIP_TGA	= (1<<2),		// Steam background completely ignore tga attribute 0x20
	TF_KEEP_8BIT	= (1<<3),		// keep original 8-bit image (if present)
	TF_NOPICMIP	= (1<<4),		// ignore r_picmip resample rules
	TF_UNCOMPRESSED	= (1<<5),		// don't compress texture in video memory
	TF_CUBEMAP	= (1<<6),		// it's cubemap texture
	TF_DEPTHMAP	= (1<<7),		// custom texture filter used
	TF_INTENSITY	= (1<<8),		// monochrome intensity image
	TF_LUMINANCE	= (1<<9),		// force image to grayscale
	TF_SKYSIDE	= (1<<10),	// this is a part of skybox
	TF_CLAMP		= (1<<11),	// clamp texcoords to [0..1] range
	TF_NOMIPMAP	= (1<<12),	// don't build mips for this image
	TF_HAS_LUMA	= (1<<13),	// sets by GL_UploadTexture
	TF_MAKELUMA	= (1<<14),	// create luma from quake texture (only q1 textures contain luma-pixels)
	TF_NORMALMAP	= (1<<15),	// is a normalmap
	TF_HAS_ALPHA	= (1<<16),	// image has alpha (used only for GL_CreateTexture)
	TF_FORCE_COLOR	= (1<<17),	// force upload monochrome textures as RGB (detail textures)
	TF_TEXTURE_1D	= (1<<18),	// this is GL_TEXTURE_1D
	TF_BORDER		= (1<<19),	// zero clamp for projected textures
	TF_TEXTURE_3D	= (1<<20),	// this is GL_TEXTURE_3D
	TF_STATIC		= (1<<21),	// a marker for purge mechanism (not used by engine)
	TF_TEXTURE_RECTANGLE= (1<<22),	// this is GL_TEXTURE_RECTANGLE
	TF_ALPHA_BORDER	= (1<<23),	// clamp to (0,0,0,255) (probably no difference)
	TF_IMAGE_PROGRAM	= (1<<24),	// enable image program support like in Doom3
	TF_ALPHACONTRAST	= (1<<25),	// special texture flags for internal usage
	TF_FLOAT		= (1<<26),	// float textures
	TF_NOCOMPARE	= (1<<27),	// disable comparing for depth textures
};

enum EDevLevel
{
	D_INFO = 1,	// "-dev 1", shows various system messages
	D_WARN, // "-dev 2", shows not critical system warnings
	D_ERROR, // "-dev 3", shows critical warnings
	D_AICONSOLE,	// "-dev 4", special case for game aiconsole
	D_NOTE // "-dev 5", show system notifications for engine developers
};

// ambient sound types
enum EAmbientType
{
	AMBIENT_WATER = 0, // waterfall
	AMBIENT_SKY, // wind
	AMBIENT_SLIME, // never used in quake
	AMBIENT_LAVA, // never used in quake
	NUM_AMBIENTS, // automatic ambient sounds
};

// model types
enum EModelType
{
	mod_bad = -1,
	mod_brush,
	mod_sprite,
	mod_alias,
	mod_studio
};

// sprite representation in memory
enum ESpriteFrameType
{
	SPR_SINGLE = 0,
	SPR_GROUP,
	SPR_ANGLED
};

enum EKeyDest
{
	key_console = 0,
	key_game,
	key_menu,
	key_message
};


enum ERdType
{
	RD_NONE = 0,
	RD_CLIENT,
	RD_PACKET
};

// game print level
enum EMessageLevel_t
{
	PRINT_LOW,	// pickup messages
	PRINT_MEDIUM,	// death messages
	PRINT_HIGH,	// critical messages
	PRINT_CHAT,	// chat messages
};

enum ENetSrc
{
	NS_CLIENT,
	NS_SERVER
};

enum EPixFormat
{
	PF_UNKNOWN = 0,
	PF_INDEXED_24,	// inflated palette (768 quint8s)
	PF_INDEXED_32,	// deflated palette (1024 quint8s)
	PF_RGBA_32,	// normal rgba buffer
	PF_BGRA_32,	// big endian RGBA (MacOS)
	PF_RGB_24,	// uncompressed dds or another 24-bit image
	PF_BGR_24,	// big-endian RGB (MacOS)
	PF_TOTALCOUNT,	// must be last
};
// imagelib global settings
enum EIlFlags
{
	IL_USE_LERPING = BIT(0),	// lerping images during resample
	IL_KEEP_8BIT = BIT(1),	// don't expand paletted images
	IL_ALLOW_OVERWRITE = BIT(2),	// allow to overwrite stored images
	IL_DONTFLIP_TGA = BIT(3),	// Steam background completely ignore tga attribute 0x20 (stupid lammers!)
};

// rgbdata output flags
enum EImgFlags
{
	// rgbdata->flags
	IMAGE_CUBEMAP = BIT(0), // it's 6-sides cubemap buffer
	IMAGE_HAS_ALPHA = BIT(1), // image contain alpha-channel
	IMAGE_HAS_COLOR = BIT(2), // image contain RGB-channel
	IMAGE_COLORINDEX = BIT(3), // all colors in palette is gradients of last color (decals)
	IMAGE_HAS_LUMA = BIT(4), // image has luma pixels (q1-style maps)
	IMAGE_SKYBOX = BIT(5), // only used by FS_SaveImage - for write right suffixes
	IMAGE_QUAKESKY = BIT(6), // it's a quake sky double layered clouds (so keep it as 8 bit)

	// Image_Process manipulation flags
	IMAGE_FLIP_X = BIT(16),	// flip the image by width
	IMAGE_FLIP_Y = BIT(17),	// flip the image by height
	IMAGE_ROT_90 = BIT(18),	// flip from upper left corner to down right corner
	IMAGE_ROT180 = IMAGE_FLIP_X|IMAGE_FLIP_Y,
	IMAGE_ROT270 = IMAGE_FLIP_X|IMAGE_FLIP_Y|IMAGE_ROT_90,
	IMAGE_ROUND = BIT(19),	// round image to nearest Pow2
	IMAGE_RESAMPLE = BIT(20),	// resample image to specified dims
	IMAGE_PALTO24 = BIT(21),	// turn 32-bit palette into 24-bit mode (only for indexed images)
	IMAGE_ROUNDFILLER = BIT(22),	// round image to Pow2 and fill unused entries with single color
	IMAGE_FORCE_RGBA = BIT(23),	// force image to RGBA buffer
	IMAGE_MAKE_LUMA = BIT(24),	// create luma texture from indexed
	IMAGE_QUANTIZE = BIT(25),	// make indexed image from 24 or 32- bit image
	IMAGE_LIGHTGAMMA = BIT(26),	// apply gamma for image
	IMAGE_REMAP = BIT(27),	// interpret width and height as top and bottom color
};

// ordering is important!
enum EPixFilter
{
	BLUR_FILTER = 0,
	BLUR_FILTER2,
	EDGE_FILTER,
	EMBOSS_FILTER,
	NUM_FILTERS,
};

// imgfilter processing flags
enum EFlFlags
{
	FILTER_GRAYSCALE = BIT(0),
};
enum ESndFormat
{
	WF_UNKNOWN = 0,
	WF_PCMDATA,
	WF_MPGDATA,
	WF_TOTALCOUNT,	// must be last
};

// soundlib global settings
enum ESlFlags
{
	SL_USE_LERPING = BIT(0), // lerping sounds during resample
	SL_KEEP_8BIT = BIT(1), // don't expand 8bit sounds automatically up to 16 bit
	SL_ALLOW_OVERWRITE = BIT(2), // allow to overwrite stored sounds
};

// wavdata output flags
enum ESndFlags
{
	// wavdata->flags
	SOUND_LOOPED = BIT( 0 ),	// this is looped sound (contain cue markers)
	SOUND_STREAM = BIT( 1 ),	// this is a streaminfo, not a real sound

	// Sound_Process manipulation flags
	SOUND_RESAMPLE = BIT(12),	// resample sound to specified rate
	SOUND_CONVERT16BIT = BIT(13),	// change sound resolution from 8 bit to 16
};

enum EHostState
{
	HOST_INIT = 0,	// initalize operations
	HOST_FRAME,	// host running
	HOST_SHUTDOWN,	// shutdown operations
	HOST_ERR_FATAL,	// sys error
	HOST_SLEEP,	// sleeped by different reason, e.g. minimize window
	HOST_NOFOCUS,	// same as HOST_FRAME, but disable mouse
	HOST_RESTART,	// during the changes video mode
	HOST_CRASHED	// an exception handler called
};

enum EInstance
{
	HOST_NORMAL,	// listen server, singleplayer
	HOST_DEDICATED,
};

enum ECmdSource
{
	src_client, // came in over a net connection as a clc_stringcmd
	// host_client will be valid during this state.
	src_command // from the command buffer
};

// Rendering constants
enum ERenderType
{
	kRenderNormal, // src
	kRenderTransColor, // c*a+dest*(1-a)
	kRenderTransTexture,	// src*a+dest*(1-a)
	kRenderGlow, // src*a+dest -- No Z buffer checks
	kRenderTransAlpha, // src*srca+dest*(1-srca)
	kRenderTransAdd, // src*a+dest
	kRenderWorldGlow, // Same as kRenderGlow but not fixed size in screen space
};

enum ERenderFx
{
	kRenderFxNone = 0,
	kRenderFxPulseSlow,
	kRenderFxPulseFast,
	kRenderFxPulseSlowWide,
	kRenderFxPulseFastWide,
	kRenderFxFadeSlow,
	kRenderFxFadeFast,
	kRenderFxSolidSlow,
	kRenderFxSolidFast,
	kRenderFxStrobeSlow,
	kRenderFxStrobeFast,
	kRenderFxStrobeFaster,
	kRenderFxFlickerSlow,
	kRenderFxFlickerFast,
	kRenderFxNoDissipation,
	kRenderFxDistort, // Distort/scale/translate flicker
	kRenderFxHologram, // kRenderFxDistort + distance fade
	kRenderFxDeadPlayer, // kRenderAmt is the player index
	kRenderFxExplode, // Scale up really big!
	kRenderFxGlowShell, // Glowing Shell
	kRenderFxClampMinScale, // Keep this sprite from getting very small (SPRITES only!)
};

enum EResourceType
{
	t_sound = 0,
	t_skin,
	t_model,
	t_decal,
	t_generic,
	t_eventscript,
	t_world, // Fake type for world, is really t_model
};

// cvar flags
enum ECvarFlags
{
	CVAR_ARCHIVE      = BIT(0),	// set to cause it to be saved to config.cfg
	CVAR_USERINFO     = BIT(1),	// added to userinfo  when changed
	CVAR_SERVERNOTIFY = BIT(2),	// notifies players when changed
	CVAR_EXTDLL       = BIT(3),	// defined by external DLL
	CVAR_CLIENTDLL    = BIT(4),	// defined by the client dll
	CVAR_PROTECTED    = BIT(5),	// it's a server cvar, but we don't send the data since it's a passquint16, etc.
	CVAR_SPONLY       = BIT(6),	// this cvar cannot be changed by clients connected to a multiplayer server.
	CVAR_PRINTABLEONLY= BIT(7),	// this cvar's string cannot contain unprintable characters ( player name )
	CVAR_UNLOGGED     = BIT(8),	// if this is a FCVAR_SERVER, don't log changes to the log file / console
	CVAR_SERVERINFO   = BIT(9),	// added to serverinfo when changed
	CVAR_PHYSICINFO   = BIT(10),// added to physinfo when changed
	CVAR_RENDERINFO   = BIT(11),// save to a seperate config called opengl.cfg
	CVAR_CHEAT        = BIT(12),// can not be changed if cheats are disabled
	CVAR_INIT 	      = BIT(13),// don't allow change from console at all, but can be set from the command line
	CVAR_LATCH        = BIT(14),// save changes until server restart
	CVAR_READ_ONLY    = BIT(15),// display only, cannot be set by user at all
	CVAR_LATCH_VIDEO  = BIT(16),// save changes until render restart
	CVAR_USER_CREATED = BIT(17),// created by a set command (dll's used)
	CVAR_GLCONFIG     = BIT(18),// set to cause it to be saved to opengl.cfg
};


// ---------------------------------------- //
// --------------- TYPEDEFS --------------- //
// ---------------------------------------- //

typedef void (*setpair_t)( const char *key, const char *value, void *buffer, void *numpairs );
typedef void (*CL_EXPORT_FUNCS)( void *pv );

typedef void (*pfnEventHook)( SEventArgs *args );

typedef void (*net_api_response_func_t) (SNetResponce *response );

typedef int (*MENUAPI)( IUiFunctions *pFunctionTable, IUiEngineFuncs* engfuncs, SUiGlobalVars *pGlobals );

typedef int (*pfnUserMsgHook)( const char *pszName, int iSize, void *pbuf );


// ---------------------------------------- //
// ------------- BASE STRUCTS ------------- //
// ---------------------------------------- //

struct SCenterPrint
{
	// centerprint stuff
	float time;
	int y, lines;
	char message[2048];
	int totalWidth;
	int totalHeight;
};

struct SScreenShake
{
	float time;
	float duration;
	float amplitude;
	float frequency;
	float next_shake;
	float offset[3];
	float angle;
	float applied_offset[3];
	float applied_angle;
};

struct SGameuiDraw
{
	int gl_texturenum; // this is a real texnum

	// scissor test
	int scissor_x;
	int scissor_y;
	int scissor_width;
	int scissor_height;
	bool scissor_test;

	// holds text color
	quint8 textColor[4];
};

struct SFlowStats
{
	int size; // size of message sent/received
	double time; // time that message was sent/received
};

struct SSizeBuf
{
	bool bOverflow; // overflow reading or writing
	const char *pDebugName; // buffer name (pointer to const name)

	quint8 *pData;
	int iCurBit;
	int nDataBits;
};

struct SScreenFade
{
	float fadeSpeed; // How fast to fade (tics / second) (+ fade in, - fade out)
	float fadeEnd; // When the fading hits maximum
	float fadeTotalEnd; // Total End Time of the fade (used for FFADE_OUT)
	float fadeReset; // When to reset to not fading (for fadeout and hold)
	quint8 fader, fadeg, fadeb, fadealpha; // Fade color
	int fadeFlags; // Fading flags
};

struct SNetAdr
{
	ENetAdrType	type;
	unsigned char	ip[4];
	unsigned char	ipx[10];
	unsigned short	port;
};

struct SLightStyle
{
	char pattern[256];
	float map[256];
	int length;
	float value;
	bool interp; // allow to interpolate this lightstyle
	float time; // local time is gurantee what new style begins from the start, not mid or end of the sequence
};

struct SPmplane
{
	float normal[3];
	float dist;
};

struct SWadList
{
	char wadnames[256][32];
	int count;
};

struct SLeafList
{
	int count;
	int maxcount;
	bool overflowed;
	short *list;
	float mins[3], maxs[3];
	int topnode; // for overflows where each leaf can't be stored individually
};


struct SUiGlobalVars
{
	float time; // unclamped host.realtime
	float frametime;

	int scrWidth; // actual values
	int scrHeight;

	int maxClients;
	int developer;
	int demoplayback;
	int demorecording;
	char demoname[64]; // name of currently playing demo
	char maptitle[64]; // title of active map
};

struct SModInfo
{
	// filesystem info
	char gamefolder[64]; // used for change game '-game x'
	char startmap[64]; // map to start singleplayer game
	char trainmap[64]; // map to start hazard course (if specified)
	char title[64]; // Game Main Title
	char version[14]; // game version (optional)
	short flags; // game flags

	// about mod info
	char game_url[256]; // link to a developer's site
	char update_url[256]; // link to updates page
	char type[64]; // single, toolkit, multiplayer etc
	char date[64];
	char size[64]; // displayed mod size

	int gamemode;
};

struct SWrect
{
	int left, right, top, bottom;
};


struct SClientTextMessage
{
	int effect;
	quint8 r1, g1, b1, a1; // 2 colors for effects
	quint8 r2, g2, b2, a2;
	float x;
	float y;
	float fadein;
	float fadeout;
	float holdtime;
	float fxtime;
	const char *pName;
	const char *pMessage;
};

struct SHudPlayerInfo
{
	char *name;
	short ping;
	quint8 thisplayer; // TRUE if this is the calling player

	// stuff that's unused at the moment,  but should be done
	quint8 spectator;
	quint8 packetloss;
	char *model;
	short topcolor;
	short bottomcolor;

	unsigned __int64 m_nSteamID;
};

struct SScreenInfo
{
	int iSize;
	int iWidth;
	int iHeight;
	int iFlags;
	int iCharHeight;
	short charWidths[256];
};

struct SClientDataShort
{
	// fields that cannot be modified  (ie. have no effect if changed)
	float origin[3];

	// fields that can be changed by the cldll
	float viewangles[3];
	int iWeaponBits;
	float fov; // field of view
};

struct SDosHeader
{
	// dos .exe header
	quint16 e_magic; // magic number
	quint16 e_cblp; // quint8s on last page of file
	quint16 e_cp; // pages in file
	quint16 e_crlc; // relocations
	quint16 e_cparhdr; // size of header in paragraphs
	quint16 e_minalloc; // minimum extra paragraphs needed
	quint16 e_maxalloc; // maximum extra paragraphs needed
	quint16 e_ss; // initial (relative) SS value
	quint16 e_sp; // initial SP value
	quint16 e_csum; // checksum
	quint16 e_ip; // initial IP value
	quint16 e_cs; // initial (relative) CS value
	quint16 e_lfarlc; // file address of relocation table
	quint16 e_ovno; // overlay number
	quint16 e_res[4]; // reserved quint16s
	quint16 e_oemid; // OEM identifier (for e_oeminfo)
	quint16 e_oeminfo; // OEM information; e_oemid specific
	quint16 e_res2[10]; // reserved quint16s
	int     e_lfanew; // file address of new exe header
};

struct SPeHeader
{
	// win .exe header
	quint16 Machine;
	quint16 NumberOfSections;
	quint32 TimeDateStamp;
	quint32 PointerToSymbolTable;
	quint32 NumberOfSymbols;
	quint16 SizeOfOptionalHeader;
	quint16 Characteristics;
};

struct SSectionHeader
{
	quint8 Name[8]; // dos name length

	union
	{
		quint32 PhysicalAddress;
		quint32 VirtualSize;
	} Misc;

	quint32 VirtualAddress;
	quint32 SizeOfRawData;
	quint32 PointerToRawData;
	quint32 PointerToRelocations;
	quint32 PointerToLinenumbers;
	quint16 NumberOfRelocations;
	quint16 NumberOfLinenumbers;
	quint32 Characteristics;
};

struct SDataDirectory
{
	quint32 VirtualAddress;
	quint32 Size;
};

struct SExportDirectory
{
	quint32 Characteristics;
	quint32 TimeDateStamp;
	quint16 MajorVersion;
	quint16 MinorVersion;
	quint32 Name;
	quint32 Base;
	quint32 NumberOfFunctions;
	quint32 NumberOfNames;
	quint32 AddressOfFunctions; // RVA from base of image
	quint32 AddressOfNames; // RVA from base of image
	quint32 AddressOfNameOrdinals; // RVA from base of image
};

struct SDllUser
{
	void    *hInstance; // to avoid possible hacks
	bool    custom_loader; // a bit who indicated loader type
	bool    encrypted; // dll is crypted (some client.dll in HL, CS etc)
	QString dllName;
	QString fullPath;
	QString shortPath; // actual dll paths

	// ordinals stuff
	quint16 *ordinals;
	quint32 *funcs;
	QStringList names;
	int     num_ordinals; // actual exports count
	quint32 funcBase; 	// base offset
};

struct SMouth
{
	quint8 mouthopen; // 0 = mouth closed, 255 = mouth agape
	quint8 sndcount; // counter for running average
	int    sndavg; // running average
};

struct SLatchedVars
{
	float  prevanimtime;
	float  sequencetime;
	quint8 prevseqblending[2];
	float  prevorigin[3];
	float  prevangles[3];

	int    prevsequence;
	float  prevframe;

	quint8 prevcontroller[4];
	quint8 prevblending[2];
};

struct SPositionHistory
{
	// Time stamp for this movement
	float animtime;

	float origin[3];
	float angles[3];
};

// Info about weapons player might have in his/her possession
struct SWeaponData
{
	int   m_iId;
	int   m_iClip;

	float m_flNextPrimaryAttack;
	float m_flNextSecondaryAttack;
	float m_flTimeWeaponIdle;

	int   m_fInReload;
	int   m_fInSpecialReload;
	float m_flNextReload;
	float m_flPumpTime;
	float m_fReloadTime;

	float m_fAimedDamage;
	float m_fNextAimBonus;
	int   m_fInZoom;
	int   m_iWeaponState;

	int   iuser1;
	int   iuser2;
	int   iuser3;
	int   iuser4;
	float fuser1;
	float fuser2;
	float fuser3;
	float fuser4;
};

struct SEventArgs
{
	int   flags;

	// Transmitted
	int   entindex;

	float origin  [3];
	float angles  [3];
	float velocity[3];

	int   ducking;

	float fparam1;
	float fparam2;

	int   iparam1;
	int   iparam2;

	int   bparam1;
	int   bparam2;
};


//SSearch   USE QStringList
// {
// 	int  numfilenames;
// 	char **filenames;
// 	char *filenamesbuffer;
// };

struct SMoveVars
{
	float gravity;           // Gravity for map
	float stopspeed;         // Deceleration when not moving
	float maxspeed;          // Max allowed speed
	float spectatormaxspeed;
	float accelerate;        // Acceleration factor
	float airaccelerate;     // Same for when in open air
	float wateraccelerate;   // Same for when in water
	float friction;
	float edgefriction;	   // Extra friction near dropofs
	float waterfriction;     // Less in water
	float entgravity;        // 1.0
	float bounce;            // Wall bounce value. 1.0
	float stepsize;          // sv_stepsize;
	float maxvelocity;       // maximum server velocity.
	float zmax;    // Max z-buffer range (for GL)
	float waveHeight;    // Water wave height (for GL)
	bool  footsteps;        // Play footstep sounds
	QString skyName;	   // Name of the sky map
	float rollangle;
	float rollspeed;
	float skycolor_r; // Sky color
	float skycolor_g; //
	float skycolor_b; //
	float skyvec_x; // Sky vector
	float skyvec_y; //
	float skyvec_z; //
	int   features; // engine features that shared across network
	int   fog_settings; // Global fog settings (packed color+density)
	float wateralpha; // World water alpha 1.0 - solid 0.0 - transparent
	float skydir_x; // skybox rotate direction
	float skydir_y; //
	float skydir_z; //
	float skyangle; // skybox rotate angle
};

struct SUserCmd
{
	short  lerp_msec; // Interpolation time on client
	quint8 msec; // Duration in ms of command
	float  viewangles[3]; // Command view angles

	// intended velocities
	float   forwardmove; // Forward velocity
	float   sidemove; // Sideways velocity
	float   upmove; // Upward velocity
	quint8  lightlevel; // Light level at spot where we are standing.
	quint16 buttons; // Attack and move buttons
	quint8  impulse; // Impulse command issued
	quint8  weaponselect; // Current weapon id

	// Experimental player impact stuff.
	int     impact_index;
	float   impact_position[3];
};

class CCmdCode
{
public:
	virtual void exec(const QStringList &) = 0;
};

// same as ref_params but for overview mode
struct SRefOverview
{
	float origin[3];
	bool  rotated;

	float xLeft;
	float xRight;
	float xTop;
	float xBottom;
	float zFar;
	float zNear;
	float flZoom;
};

struct SDlump
{
	int fileofs;
	int filelen;
};


struct SDmodel
{
	float mins[3];
	float maxs[3];
	float origin[3]; // for sounds or lights
	int   headnode[MAX_MAP_HULLS];
	int   visleafs; // not including the solid leaf 0
	int   firstface;
	int   numfaces;
};

struct SDmiptexLump
{
	int nummiptex;
	int dataofs[4]; // [nummiptex]
};

struct SDvertex
{
	float point[3];
};

struct SDplane
{
	float normal[3];
	float dist;
	int   type; // PLANE_X - PLANE_ANYZ ?
};

struct SDnode
{
	int     planenum;
	short   children[2]; // negative numbers are -(leafs + 1), not nodes
	short   mins[3]; // for sphere culling
	short   maxs[3];
	quint16 firstface;
	quint16 numfaces; // counting both sides
};

// leaf 0 is the generic CONTENTS_SOLID leaf, used for all solid areas
// all other leafs need visibility info
struct SDleaf
{
	int     contents;
	int     visofs; // -1 = no visibility info

	short   mins[3]; // for frustum culling
	short   maxs[3];
	quint16 firstmarksurface;
	quint16 nummarksurfaces;

	// automatic ambient sounds
	quint8  ambient_level[NUM_AMBIENTS]; // ambient sound level (0 - 255)
};

struct SDclipNode
{
	int   planenum;
	short children[2]; // negative numbers are contents
};

struct SDtexInfo
{
	float vecs[2][4]; // texmatrix [s/t][xyz offset]
	int   miptex;
	int   flags;
};

// NOTE: that edge 0 is never used, because negative edge nums
// are used for counterclockwise use of the edge in a face
struct SDedge
{
	quint16 v[2]; // vertex numbers
};

struct SDface
{
	quint16 planenum;
	short   side;

	int     firstedge; // we must support > 64k edges
	short   numedges;
	short   texinfo;

	// lighting info
	quint8  styles[LM_STYLES];
	int     lightofs; // start of [numstyles*surfsize] samples
};


struct SMplane
{
	float  normal[3];
	float  dist;
	quint8 type; // for fast side tests
	quint8 signbits; // signx + (signy<<1) + (signz<<1)
	quint8 pad[2];
};

struct SMvertex
{
	float position[3];
};

struct SMedge
{
	quint16 v[2];
	quint32 cachededgeoffset;
};

// 73 quint8s per VBO vertex
// FIXME: align to 32 quint8s
struct SGlvert
{
	float  vertex[3]; // position
	float  normal[3]; // normal
	float  stcoord[2]; // ST texture coords
	float  lmcoord[2]; // ST lightmap coords
	float  sccoord[2]; // ST scissor coords (decals only) - for normalmap coords migration
	float  tangent[3]; // tangent
	float  binormal[3]; // binormal
	quint8 color[4]; // colors per vertex
};

struct SCacheUser
{
	void *data; // extradata
};


struct SAlight
{
	int   ambientlight; // clip at 128
	int   shadelight; // clip at 192 - ambientlight
	float color[3];
	float *plightvec;
};

struct SAuxVert
{
	float fv[3]; // viewspace x, y
};


struct SMspriteFrame
{
	int   width;
	int   height;
	float up;
	float down;
	float left;
	float right;
	int   gl_texturenum;
};

// MD5 Hash
struct SMd5Context
{
	quint32 buf[4];
	quint32 bits[2];
	quint8  in[64];
};

struct SSoundItem
{
	QString name;
	short   entnum;
	float   origin[3];
	float   volume;
	float   attenuation;
	bool    looping;
	quint8  channel;
	quint8  pitch;
	quint8  quint16Index; // current playing quint16 in sentence
	double  samplePos;
	double  forcedEnd;
};

struct SBpcDesc
{
	int     format; // pixelformat
	QString name[16]; // used for debug
	quint32 glFormat; // RGBA format
	int     bpp; // channels (e.g. rgb = 3, rgba = 4)
};

struct SWavData
{
	quint16 rate; // num samples per second (e.g. 11025 - 11 khz)
	quint8  width; // resolution - bum bits divided by 8 (8 bit is 1, 16 bit is 2)
	quint8  channels; // num channels (1 - mono, 2 - stereo)
	int     loopStart; // offset at this point sound will be looping while playing more than only once
	int     samples; // total samplecount in wav
	uint    type; // compression type
	uint    flags; // misc sound flags
	quint8  *buffer; // sound buffer
	size_t  size; // for bounds checking
};

struct SModelState
{
	short  sequence;
	short  frame; // 10 bits multiple by 4, should be enough
	quint8 blending[2];
	quint8 controller[4];
	quint8 body;
	quint8 skin;
};

struct SDllFunc
{
	QString name;
	void    **func;
};

struct SField
{
	QString buffer;
	int     cursor;
	int     scroll;
	int     widthInChars;
};

struct SNotify
{
	QString szNotify;
	float   expire;
	quint8  color[4];
	int     key_dest;
};

struct SConNprint
{
	int   index; // Row #
	float time_to_live; // # of seconds before it dissappears
	float color[3]; // RGB colors ( 0.0 -> 1.0 scale )
};

struct SClientData
{
	float origin[3];
	float velocity[3];

	int   viewmodel;
	float punchangle[3];
	int   flags;
	int   waterlevel;
	int   watertype;
	float view_ofs[3];
	float health;

	int   bInDuck;
	int   weapons; // remove?

	int   flTimeStepSound;
	int   flDuckTime;
	int   flSwimTime;
	int   waterjumptime;

	float maxspeed;

	float fov;
	int   weaponanim;

	int   m_iId;
	int   ammo_shells;
	int   ammo_nails;
	int   ammo_cells;
	int   ammo_rockets;
	float m_flNextAttack;

	int   tfstate;
	int   pushmsec;
	int   deadflag;
	char  physinfo[MAX_PHYSINFO_STRING];

	// For mods
	int   iuser1;
	int   iuser2;
	int   iuser3;
	int   iuser4;
	float fuser1;
	float fuser2;
	float fuser3;
	float fuser4;
	float vuser1[3];
	float vuser2[3];
	float vuser3[3];
	float vuser4[3];
};


// internal shared gameinfo structure (readonly for engine parts)
struct SGameInfo
{
	// filesystem info
	QString gamefolder; // used for change game '-game x'
	QString basedir; // base game directory (like 'id1' for Quake or 'valve' for Half-Life)
	QString gamedir; // game directory (can be match with basedir, used as game dir and as write path)
	QString falldir; // used as second basedir
	QString startmap; // map to start singleplayer game
	QString trainmap; // map to start hazard course (if specified)
	QString title; // Game Main Title
	QString version; // game version (optional)

	// .dll pathes
	QString dll_path; // e.g. "bin" or "cl_dlls"
	QString game_dll; // custom path for game.dll

	// .ico path
	QString iconpath; // "game.ico" by default

	// about mod info
	QString game_url; // link to a developer's site
	QString update_url; // link to updates page
	QString type; // single, toolkit, multiplayer etc
	QString date;
	int     size;

	int     gamemode;
	bool    secure; // prevent to console acess
	bool    nomodels; // don't let player to choose model (use player.mdl always)

	QString sp_entity; // e.g. info_player_start
	QString mp_entity; // e.g. info_player_deathmatch

	float   client_mins[4][3]; // 4 hulls allowed
	float   client_maxs[4][3]; // 4 hulls allowed

	QString ambientsound[4]; // quake ambient sounds
	int     soundclip_dist; // custom distance to clip sound

	int     max_edicts; // min edicts is 600, max edicts is 4096
	int     max_tents; // min temp ents is 300, max is 2048
	int     max_beams; // min beams is 64, max beams is 512
	int     max_particles; // min particles is 4096, max particles is 32768
};

struct SCmdBuf
{
	QString data;
	int     cursize;
	int     maxsize;
};


struct SColor24
{
	quint8 r, g, b;
};

struct SColorVec
{
	quint32 r, g, b, a;
};

struct SLink
{
	SLink *prev, *next;
};

struct SPlane
{
	float normal[3];
	float dist;
};

struct SResourceInfo
{
	int info[8];
};

struct SPackHeader
{
	int ident;
	int dirofs;
	int dirlen;
};

struct SPackFile
{
	QString name;
	int     filepos;
	int     filelen;
};

/*
========================================================================
.WAD archive format (WhereAllData - WAD)

List of compressed files, that can be identify only by TYPE_*

<format>
header:	dwadinfo_t[dwadinfo_t]
file_1:	quint8[dwadinfo_t[num]->disksize]
file_2:	quint8[dwadinfo_t[num]->disksize]
file_3:	quint8[dwadinfo_t[num]->disksize]
...
file_n:	quint8[dwadinfo_t[num]->disksize]
infotable dlumpinfo_t[dwadinfo_t->numlumps]
========================================================================
*/

struct SDwadInfo
{
	int ident; // should be IWAD, WAD2 or WAD3
	int numlumps; // num files
	int infotableofs;
};

struct SDlumpInfo
{
	int  filepos;
	int  disksize;
	int  size; // uncompressed
	char type;
	char compression; // probably not used
	char pad1;
	char pad2;
	char name[16]; // must be null terminated
};

struct SHpakHeader
{
	int ident; // should be equal HPAK
	int version;
	int seek; // infotableofs ?
};


/*
========================================================================

.LMP image format (Half-Life gfx.wad lumps)

========================================================================
*/
struct SLmp
{
	quint32 width;
	quint32 height;
};

// .MIP image format (half-Life textures)
struct SMip
{
	char    name[16];
	quint32 width;
	quint32 height;
	quint32 offsets[4]; // four mip maps stored
};

struct SWadType
{
	char *ext;
	char type;
};

struct SFile
{
	int    handle; // file descriptor
	int    real_length; // uncompressed file size (for files opened in "read" mode)
	int    position; // current position in the file
	int    offset; // offset into the package (0 if external file)
	int    ungetc; // single stored character from ungetc, cleared to EOF when read
	time_t filetime; // pak, wad or real filetime
   // Contents buffer
	int    buff_ind, buff_len; // buffer current index and length
	quint8 buff[FILE_BUFF_SIZE]; // intermediate buffer
};

struct SPackfile
{
	char name[56];
	int  offset;
	int  realsize; // real file size (uncompressed)
};


// ---------------------------------------- //
// ------------ NESTED STRUCTS ------------ //
// ---------------------------------------- //

struct SClFont
{
	int hFontTexture; // handle to texture
	SWrect fontRc[256]; // rectangles
	quint8 charWidths[256];
	int charHeight;
	bool valid; // all rectangles are valid
};

struct SOptionalHeader
{
	quint16 Magic;
	quint8  MajorLinkerVersion;
	quint8  MinorLinkerVersion;
	quint32 SizeOfCode;
	quint32 SizeOfInitializedData;
	quint32 SizeOfUninitializedData;
	quint32 AddressOfEntryPoint;
	quint32 BaseOfCode;
	quint32 BaseOfData;
	quint32 ImageBase;
	quint32 SectionAlignment;
	quint32 FileAlignment;
	quint16 MajorOperatingSystemVersion;
	quint16 MinorOperatingSystemVersion;
	quint16 MajorImageVersion;
	quint16 MinorImageVersion;
	quint16 MajorSubsystemVersion;
	quint16 MinorSubsystemVersion;
	quint32 Win32VersionValue;
	quint32 SizeOfImage;
	quint32 SizeOfHeaders;
	quint32 CheckSum;
	quint16 Subsystem;
	quint16 DllCharacteristics;
	quint32 SizeOfStackReserve;
	quint32 SizeOfStackCommit;
	quint32 SizeOfHeapReserve;
	quint32 SizeOfHeapCommit;
	quint32 LoaderFlags;
	quint32 NumberOfRvaAndSizes;

	SDataDirectory DataDirectory[NUMBER_OF_DIRECTORY_ENTRIES];
};

struct SResource
{
	char      szFileName[64]; // File name to download/precache.
	EResourceType type; // t_sound, t_skin, t_model, t_decal.
	int       nIndex; // For t_decals
	int       nDownloadSize; // Size in quint8s if this must be downloaded.
	quint8    ucFlags;

	// for handling client to client resource propagation
	quint8    rgucMD5_hash[16]; // To determine if we already have it.
	quint8    playernum; // Which player index this resource is associated with,
   // if it's a custom resource.

	quint8    rguc_reserved[32]; // For future expansion
	SResource *pNext; // Next in chain.
	SResource *pPrev;
};

//struct SResourceList USE QMap<QString, int>
//{
//	int rescount;
//	int restype[MAX_RESOURCES];
//	QStringList resnames;
//};

struct SCustomization
{
	bool      bInUse; // Is this customization in use;
	SResource resource; // The resource_t for this customization
	bool      bTranslated; // Has the raw data been translated into a useable format?
   // (e.g., raw decal .wad make into STexture *)
	int       nUserData1; // Customization specific data
	int       nUserData2; // Customization specific data
	void      *pInfo; // Buffer that holds the data structure that references
   // the data (e.g., the cachewad_t)
	void      *pBuffer; // Buffer that holds the data for the customization
   // (the raw .wad data)
	SCustomization *pNext; // Next in chain
};

struct SEfrag
{
	SMleaf    *leaf;
	SEfrag    *leafnext;
	SClEntity *entity;
	SEfrag    *entnext;
};

// Entity state is used for the baseline and for delta compression of a packet of
//  entities that is sent to a client.
struct SEntityState
{
// Fields which are filled in by routines outside of delta compression
	int    entityType;
	// Index into cl_entities array for this entity.
	int    number;
	float  msg_time;

	// Message number last time the player/entity state was updated.
	int    messagenum;

	// Fields which can be transitted and reconstructed over the network stream
	float  origin[3];
	float  angles[3];

	int    modelindex;
	int    sequence;
	float  frame;
	int    colormap;
	short  skin;
	short  solid;
	int    effects;
	float  scale;
	quint8 eflags;

	// Render information
	int    rendermode;
	int    renderamt;
	SColor24 rendercolor;
	int    renderfx;

	int    movetype;
	float  animtime;
	float  framerate;
	int    body;
	quint8 controller[4];
	quint8 blending[4];
	float  velocity[3];

	// Send bbox down to client for use during prediction.
	float  mins[3];
	float  maxs[3];

	int    aiment;
	// If owned by a player, the index of that player ( for projectiles ).
	int    owner;

	// Friction, for prediction.
	float  friction;
	// Gravity multiplier
	float  gravity;

	// PLAYER SPECIFIC
	int    team;
	int    playerclass;
	int    health;
	bool   spectator;
	int    weaponmodel;
	int    gaitsequence;
	// If standing on conveyor, e.g.
	float  basevelocity[3];
	// Use the crouched hull, or the regular player hull.
	int    usehull;
	// Latched buttons last time state updated.
	int    oldbuttons;
	// -1 = in air, else pmove entity number
	int    onground;
	int    iStepLeft;
	// How fast we are falling
	float  flFallVelocity;

	float  fov;
	int    weaponanim;

	// Parametric movement overrides
	float  startpos[3];
	float  endpos[3];
	float  impacttime;
	float  starttime;

	// For mods
	int    iuser1;
	int    iuser2;
	int    iuser3;
	int    iuser4;
	float  fuser1;
	float  fuser2;
	float  fuser3;
	float  fuser4;
	float  vuser1[3];
	float  vuser2[3];
	float  vuser3[3];
	float  vuser4[3];
};

struct SLocalState
{
	double       timestamp;
	SEntityState playerstate;
	SClientDataShort  client;
	SWeaponData  weapondata[64];
};

struct SClEntity
{
	int      index;      // Index into cl_entities ( should match actual slot, but not necessarily )
	bool     player;     // True if this entity is a "player"

	SEntityState baseline;   // The original state from which to delta during an uncompressed message
	SEntityState prevstate;  // The state information from the penultimate message received from the server
	SEntityState curstate;   // The state information from the last message received from server

	int      current_position; // Last received history update index
	SPositionHistory ph[HISTORY_MAX]; // History of position and angle updates for this player

	SMouth   mouth; // For synchronizing mouth movements.

	SLatchedVars latched; // Variables used by studio model rendering routines

	// Information based on interplocation, extrapolation, prediction, or just copied from last msg received.
	//
	float   lastmove;

	// Actual render position and angles
	float   origin[3];
	float   angles[3];

	// Attachment points
	float   attachment[4][3];

	// Other entity local information
	int     trivial_accept;

	SModel *model; // cl.model_precache[ curstate.modelindes ];  all visible entities have a model
	SEfrag  *efrag; // linked list of efrags
	SMnode  *topnode; // for bmodels, first world node that splits bmodel, or NULL if not split

	float   syncbase; // for client-side animations -- used by obsolete alias animation system, remove?
	int     visframe; // last frame this entity was found in an active leaf
	SColorVec cvFloorColor;
};

struct SRefParams
{
	// output
	float vieworg[3];
	float viewangles[3];

	float forward[3];
	float right[3];
	float up[3];

	// Client frametime;
	float frametime;
	// Client time
	float time;

	// Misc
	int   intermission;
	int   paused;
	int   spectator;
	int   onground;
	int   waterlevel;

	float simvel[3];
	float simorg[3];

	float viewheight[3];
	float idealpitch;

	float cl_viewangles[3];
	int   health;
	float crosshairangle[3];
	float viewsize;

	float punchangle[3];
	int   maxclients;
	int   viewentity;
	int   playernum;
	int   max_entities;
	int   demoplayback;
	int   hardware;
	int   smoothing;

	// Last issued usercmd
	SUserCmd *cmd;

	// Movevars
	SMoveVars *movevars;

	int   viewport[4]; // the viewport coordinates x, y, width, height
	int   nextView; // the renderer calls ClientDLL_CalcRefdef() and Renderview
	// so long in cycles until this value is 0 (multiple views)
	int   onlyClientDraw; // if !=0 nothing is drawn by the engine except clientDraw functions
// Xash3D extension
	float fov_x, fov_y; // actual fov can be overrided on nextView
};

struct SDheader
{
	int    version;
	SDlump lumps[HEADER_LUMPS];
};

struct STexture
{
	char     name[16];
	quint32  width, height;
	int      gl_texturenum;
	STexture *texturechain; // for gl_texsort drawing
	int      anim_total; // total tenths in sequence ( 0 = no)
	int      anim_min, anim_max; // time for this frame min <=time< max
	STexture *anim_next; // in the animation sequence
	STexture *alternate_anims; // bmodels in frame 1 use these
	quint16  fb_texturenum; // auto-luma texturenum
	quint16  dt_texturenum; // detail-texture binding
	quint32  unused[3]; // reserved
};

struct SMtexInfo
{
	float    vecs[2][4]; // [s/t] unit vectors in world space.
	// [i][3] is the s/t offset relative to the origin.
	// s or t = dot( 3Dpoint, vecs[i] ) + vecs[i][3]
	float    mipadjust; // mipmap limits for very small surfaces
	STexture *texture;
	int      flags; // sky or slime, no lightmap or 256 subdivision
};

struct SGlpoly
{
	SGlpoly *next;
	SGlpoly *chain;
	int     numverts;
	int     flags;           // for SURF_UNDERWATER
	float   verts[4][VERTEXSIZE]; // variable sized (xyz s1t1 s2t2)
};

struct SMnode
{
	// common with leaf
	int     contents; // 0, to differentiate from leafs
	int     visframe; // node needs to be traversed if current

	float   minmaxs[6]; // for bounding box culling
	SMnode  *parent;

	// node specific
	SMplane *plane;
	SMnode  *children[2];

	quint16 firstsurface;
	quint16 numsurfaces;
};

struct SMsurface
{
	int       visframe; // should be drawn when node is crossed

	SMplane   *plane; // pointer to shared plane
	int       flags; // see SURF_ #defines

	int       firstedge; // look up in model->surfedges[], negative numbers
	int       numedges; // are backwards edges

	short     texturemins[2];
	short     extents[2];

	int       light_s, light_t; // gl lightmap coordinates

	SGlpoly   *polys; // multiple if warped
	SMsurface *texturechain;

	SMtexInfo *texinfo;

	// lighting info
	int       dlightframe; // last frame the surface was checked by an animated light
	int       dlightbits; // dynamically generated. Indicates if the surface illumination
	// is modified by an animated light.

	int       lightmaptexturenum;
	quint8    styles[MAXLIGHTMAPS];
	int       cached_light[MAXLIGHTMAPS]; // values currently used in lightmap
	SMsurface *lightmapchain; // for new dlights rendering (was cached_dlight)

	SColor24  *samples; // note: this is the actual lightmap data for this surface
	SDecal    *pdecals;
};

// JAY: Compress this as much as possible
struct SDecal
{
	SDecal *pnext; // linked list for each surface
	SMsurface *psurface; // Surface id for persistence / unlinking
	float  dx; // local texture coordinates
	float  dy; //
	float  scale; // Pixel scale
	short  texture; // Decal texture
	quint8 flags; // Decal flags  FDECAL_*
	short  entityIndex; // Entity this is attached to
// Xash3D added
	float  position[3]; // location of the decal center in world space.
	float  saxis[3]; // direction of the s axis in world space
	SMsurfMesh *mesh; // decal mesh in local space
	int    reserved[4]; // for future expansions
};

struct SMleaf
{
// common with node
	int    contents;
	int    visframe; // node needs to be traversed if current

	float  minmaxs[6]; // for bounding box culling

	SMnode *parent;
// leaf specific
	quint8 *compressed_vis;
	SEfrag *efrags;

	SMsurface **firstmarksurface;
	int    nummarksurfaces;
	quint8 *compressed_pas;
	quint8 ambient_sound_level[NUM_AMBIENTS];

};

struct SMsurfMesh
{
	quint16 numVerts;
	quint16 numElems; // ~ 20 000 vertex per one surface. Should be enough
	quint32 startVert; // user-variable. may be used for construct world single-VBO
	quint32 startElem; // user-variable. may be used for construct world single-VBO

	SGlvert *verts; // vertexes array
	quint16 *elems; // indices

	SMsurface *surf; // pointer to parent surface. Just for consistency
	SMsurfMesh *next; // temporary chain of subdivided surfaces
};

// surface extradata stored in cache.data for all brushmodels
struct SMextraSurf
{
	float mins, maxs[3];
	float origin[3]; // surface origin
	SMsurfMesh *mesh; // VBO\VA ready surface mesh. Not used by engine but can be used by mod-makers

	int   dlight_t, dlight_s; // gl lightmap coordinates for dynamic lightmaps

	int   mirrortexturenum; // gl texnum
	float mirrormatrix[4][4];
	SMextraSurf *mirrorchain; // for gl_texsort drawing
	SMextraSurf *detailchain; // for detail textures drawing
	SColor24 *deluxemap; // note: this is the actual deluxemap data for this surface

	int   reserved[32]; // just for future expansions or mod-makers
};

struct SHull
{
	SDclipNode *clipnodes;
	SMplane *planes;
	int     firstclipnode;
	int     lastclipnode;
	float   clip_mins[3];
	float   clip_maxs[3];
};

struct SModel
{
	char    name[64]; // model name
	bool    needload; // bmodels and sprites don't cache normally

	// shared modelinfo
	EModelType type; // model type
	int     numframes; // sprite's framecount
	quint8  *mempool; // private mempool (was synctype)
	int     flags; // hl compatibility

//
// volume occupied by the model
//
	float   mins[3], maxs[3]; // bounding box at angles '0 0 0'
	float   radius;

	// brush model
	int     firstmodelsurface;
	int     nummodelsurfaces;

	int     numsubmodels;
	SDmodel *submodels; // or studio animations

	int     numplanes;
	SMplane *planes;

	int     numleafs; // number of visible leafs, not counting 0
	SMleaf  *leafs;

	int     numvertexes;
	SMvertex *vertexes;

	int     numedges;
	SMedge  *edges;

	int     numnodes;
	SMnode  *nodes;

	int     numtexinfo;
	SMtexInfo *texinfo;

	int     numsurfaces;
	SMsurface *surfaces;

	int     numsurfedges;
	int *surfedges;

	int     numclipnodes;
	SDclipNode *clipnodes;

	int     nummarksurfaces;
	SMsurface **marksurfaces;

	SHull hulls[MAX_MAP_HULLS];

	int     numtextures;
	STexture **textures;

	quint8  *visdata;

	SColor24 *lightdata;
	char    *entities;
//
// additional model data
//
	SCacheUser cache; // only access through Mod_Extradata
};

struct SPlayerInfo
{
	int   userid; // User id on server
	char  userinfo[MAX_INFO_STRING]; // User info string
	char  name[MAX_SCOREBOARDNAME]; // Name (extracted from userinfo)
	int   spectator; // Spectator or not, unused

	int   ping;
	int   packet_loss;

	// skin information
	char  model[64];
	int   topcolor;
	int   bottomcolor;

	// last frame rendered
	int   renderframe;

	// Gait frame estimation
	int   gaitsequence;
	float gaitframe;
	float gaityaw;
	float prevgaitorigin[3];

	SCustomization customdata;
};

struct SMspriteGroup
{
	int   numframes;
	float *intervals;
	SMspriteFrame *frames[1];
};

struct SMspriteFrameDesc
{
	ESpriteFrameType type;
	SMspriteFrame    *frameptr;
};

struct SMsprite
{
	short type;
	short texFormat;
	int   maxwidth;
	int   maxheight;
	int   numframes;
	int   radius;
	int   facecull;
	int   synctype;
	SMspriteFrameDesc frames[1];
};

struct SSysInfo
{
	QString   ModuleName; // exe.filename
	SGameInfo *GameInfo; // current GameInfo
	SGameInfo *games; // environment games (founded at each engine start)
	int       numgames;
};


struct SNetadr
{
	ENetAdrType type;
	quint8      ip[4];
	quint8      ipx[10];
	quint16     port;
};

struct SHostRedirect
{
	ERdType    target;
	QByteArray buffer;
	int        buffersize;
	SNetadr    address;
	void (*flush)( SNetadr adr, ERdType target, char *buffer );
};




enum EPtype
{
	pt_static,
	pt_grav,
	pt_slowgrav,
	pt_fire,
	pt_explode,
	pt_explode2,
	pt_blob,
	pt_blob2,
	pt_vox_slowgrav,
	pt_vox_grav,
	pt_clientcustom,	// Must have callback function specified
	pt_tracer		// Always have callback
};

struct SParticle
{
	float		org[3];
	short		color;
	short		packedColor;
	SParticle	*next;
	float		vel[3];
	float		ramp;
	float		die;
	EPtype		type;
	void		(*deathfunc)( SParticle *particle );

	// for pt_clientcusttom, we'll call this function each frame
	void		(*callback)( SParticle *particle, float frametime );

	// For deathfunc, etc.
	unsigned char	context;
};

struct SBeam
{
	SBeam		*next;
	int		type;
	int		flags;
	float		source[3];
	float		target[3];
	float		delta[3];
	float		t;		// 0 .. 1 over lifetime of beam
	float		freq;
	float		die;
	float		width;
	float		amplitude;
	float		r, g, b;
	float		brightness;
	float		speed;
	float		frameRate;
	float		frame;
	int		segments;
	int		startEntity;
	int		endEntity;
	int		modelIndex;
	int		frameCount;
	SModel	*pFollowModel;
	SParticle	*particles;
};

struct SHostParams
{
	QStringList   argv;

	EInstance     type; // running at
	EHostState    state; // global host state
	QString       finalmsg; // server shutdown final message
	SHostRedirect rd; // remote console

	quint32 framecount; // global framecount
	int     errorframe; // to prevent multiple host error
	double  realtime; // host.curtime
	double  frametime; // time between engine frames
	double  realframetime; // for some system events, e.g. console animations

	int     developer; // show all developer's message
	int     old_developer; // keep real dev state (we need enable dev-mode in multiplayer)
	bool    key_overstrike; // key overstrike mode
	bool    stuffcmdsrun; // execute stuff commands
	bool    con_showalways; // show console always (developer and dedicated)
	bool    change_game; // initialize when game is changed
	bool    mouse_visible; // vgui override cursor control
	bool    input_enabled; // vgui override mouse & keyboard input
	bool    shutdown_issued; // engine is shutting down
	bool    decal_loading; // nasty hack to tell imagelib about decal
	bool    overview_loading; // another nasty hack to tell imagelib about ovierview
	bool    force_draw_version; // used when fraps is loaded
	bool    write_to_clipboard; // put image to clipboard instead of disk
	bool    crashed; // set to true if crashed

	QString rootdir; // member root directory
	QString gamefolder; // it's a default gamefolder

	QByteArray mempool; // static mempool for misc allocations
	QByteArray imagepool; // imagelib mempool
	QByteArray soundpool; // soundlib mempool

	quint32 features; // custom features that enables by mod-maker request

	QList<QByteArray> draw_decals;
	QList<SDecalItem> decalList;
	QList<SSoundItem> soundList;
};

struct SRgbData
{
	quint16 width; // image width
	quint16 height; // image height
	quint16 depth; // image depth
	quint32 type; // compression type
	quint32 flags; // misc image flags
	quint8  *palette; // palette if present
	quint8  *buffer; // image buffer
	quint8  fogParams[4]; // some water textures in hl1 has info about fog color and alpha
	size_t  size; // for bounds checking
};


struct SImgFilter_s
{
	int      filter; // pixfilter_t
	float    factor; // filter factor value
	float    bias; // filter bias value
	EFlFlags flags; // filter additional flags
	quint32  blendFunc; // blending mode
};

struct SConVar
{
	// this part shared with SCvar
	QString name;
	QString string;
	int     flags;
	float   value;
	SConVar *next;

	// this part unique for convar_t
	int     integer; // atoi( string )
	bool    modified; // set each time the cvar is changed
	QString reset_string; // cvar_restart will reset to this value
	QString latched_string; // for CVAR_LATCH vars
	QString description; // variable descrition info
};


struct SDecalItem
{
	float   *position[3];
	QString name;
	short   entityIndex;
	float   *depth;
	float   *flags;
	float   scale;

	// this is the surface plane that we hit so that
	// we can move certain decals across
	// transitions if they hit similar geometry
	float   *impactPlaneNormal;

	SModelState studio_state; // studio decals only
};



struct SDllInfo
{
	QString  name; // name of library
	SDllFunc *fcts; // list of dll exports
	bool     crash; // crash if dll not found
	void     *link; // hinstance of loading library
};

struct SConsole
{
	bool    initialized;

	QString text;
	int     current; // line where next message will be printed
	int     display; // bottom of console displays this line
	int     x; // offset in current line for next print

	int     linewidth; // characters across screen
	int     totallines; // total lines in console scrollback

	float   displayFrac; // aproaches finalFrac at scr_conspeed
	float   finalFrac; // 0.0 to 1.0 lines of console to display

	int     vislines; // in scanlines
	QList<double> times; // host.realtime the line was generated for transparent notify lines
	quint8  color[4];

	// console images
	int     background; // console background

	// conchars
//	SClFont chars[CON_NUMFONTS];// fonts.wad/font1.fnt
//	SClFont *curFont, *lastUsedFont;

	// console input
	SField  input;

	// chatfiled
	SField  chat;
	QString chat_cmd; // can be overrieded by user

	// console history
	QList<SField> historyLines;
	int     historyLine; // the line being displayed from history buffer will be <= nextHistoryLine
	int     nextHistoryLine; // the last line in the history buffer, not masked

	QList<SNotify> notify; // for Con_NXPrintf
	bool    draw_notify; // true if we have NXPrint message

	// console auto-complete
	QString shortestMatch;
	SField  *completionField; // con.input or dedicated server fake field-line
	QString completionString;
	QStringList cmds;
	int     matchCount;
};

struct SCmd
{
	SCmd     *next;
	QString  name;
	CCmdCode *code;
	QString  desc;
	int      flags;
};

struct SCmdAlias
{
	SCmdAlias *next;
	QString   name;
	QString   value;
};

struct SCvar
{
	QString name;
	QString string;
	int     flags;
	float   value;
	SCvar   *next;
};

struct SGlobalVars
{
	float   time;
	float   frametime;
	float   force_retouch;
	QString mapname;
	QString startspot;
	float   deathmatch;
	float   coop;
	float   teamplay;
	float   serverflags;
	float   found_secrets;
	float   v_forward[3];
	float   v_up[3];
	float   v_right[3];
	float   trace_allsolid;
	float   trace_startsolid;
	float   trace_fraction;
	float   trace_endpos[3];
	float   trace_plane_normal[3];
	float   trace_plane_dist;
	SEdict  *trace_ent;
	float   trace_inopen;
	float   trace_inwater;
	int     trace_hitgroup;
	int     trace_flags;
	int     changelevel; // transition in progress when true (was msg_entity)
	int     cdAudioTrack;
	int     maxClients;
	int     maxEntities;
	char    *pStringBase;

	void    *pSaveData; // (SAVERESTOREDATA *) pointer
	float   vecLandmarkOffset[3];
};

struct SEntVars
{
	QString classname;
	QString globalname;

	float   origin[3];
	float   oldorigin[3];
	float   velocity[3];
	float   basevelocity[3];
	float   clbasevelocity[3]; // Base velocity that was passed in to server physics so
	// client can predict conveyors correctly. Server zeroes it, so we need to store here, too.
	float   movedir[3];

	float   angles[3]; // Model angles
	float   avelocity[3]; // angle velocity (degrees per second)
	float   punchangle[3]; // auto-decaying view angle adjustment
	float   v_angle[3]; // Viewing angle (player only)

	// For parametric entities
	float   endpos[3];
	float   startpos[3];
	float   impacttime;
	float   starttime;

	int     fixangle; // 0:nothing, 1:force view angles, 2:add avelocity
	float   idealpitch;
	float   pitch_speed;
	float   ideal_yaw;
	float   yaw_speed;

	int     modelindex;

	QString model;
	int     viewmodel; // player's viewmodel
	int     weaponmodel; // what other players see

	float   absmin[3]; // BB max translated to world coord
	float   absmax[3]; // BB max translated to world coord
	float   mins[3]; // local BB min
	float   maxs[3]; // local BB max
	float   size[3]; // maxs - mins

	float   ltime;
	float   nextthink;

	int     movetype;
	int     solid;

	int     skin;
	int     body; // sub-model selection for studiomodels
	int     effects;
	float   gravity; // % of "normal" gravity
	float   friction; // inverse elasticity of MOVETYPE_BOUNCE

	int     light_level;

	int     sequence; // animation sequence
	int     gaitsequence; // movement animation sequence for player (0 for none)
	float   frame; // % playback position in animation sequences (0..255)
	float   animtime; // world time when frame was set
	float   framerate; // animation playback rate (-8x to 8x)
	quint8  controller[4]; // bone controller setting (0..255)
	quint8  blending[2]; // blending amount between sub-sequences (0..255)

	float   scale; // sprites and models rendering scale (0..255)
	int     rendermode;
	float   renderamt;
	float   rendercolor[3];
	int     renderfx;

	float   health;
	float   frags;
	int     weapons; // bit mask for available weapons
	float   takedamage;

	int     deadflag;
	float   view_ofs[3]; // eye position

	int     button;
	int     impulse;

	SEdict  *chain; // Entity pointer when linked into a linked list
	SEdict  *dmg_inflictor;
	SEdict  *enemy;
	SEdict  *aiment; // entity pointer when MOVETYPE_FOLLOW
	SEdict  *owner;
	SEdict  *groundentity;

	int     spawnflags;
	int     flags;

	int     colormap; // lowquint8 topcolor, highquint8 bottomcolor
	int     team;

	float   max_health;
	float   teleport_time;
	float   armortype;
	float   armorvalue;
	int     waterlevel;
	int     watertype;

	QString target;
	QString targetname;
	QString netname;
	QString message;

	float   dmg_take;
	float   dmg_save;
	float   dmg;
	float   dmgtime;

	QString noise;
	QString noise1;
	QString noise2;
	QString noise3;

	float   speed;
	float   air_finished;
	float   pain_finished;
	float   radsuit_finished;

	SEdict  *pContainingEntity;

	int     playerclass;
	float   maxspeed;

	float   fov;
	int     weaponanim;

	int     pushmsec;

	int     bInDuck;
	int     flTimeStepSound;
	int     flSwimTime;
	int     flDuckTime;
	int     iStepLeft;
	float   flFallVelocity;

	int     gamestate;

	int     oldbuttons;

	int     groupinfo;

	// For mods
	int     iuser1;
	int     iuser2;
	int     iuser3;
	int     iuser4;
	float   fuser1;
	float   fuser2;
	float   fuser3;
	float   fuser4;
	float   vuser1[3];
	float   vuser2[3];
	float   vuser3[3];
	float   vuser4[3];
	SEdict  *euser1;
	SEdict  *euser2;
	SEdict  *euser3;
	SEdict  *euser4;
};

struct SEdict
{
	bool  free;
	int   serialnumber;

	SLink area; // linked to a division node or leaf
	int   headnode; // -1 to use normal leaf check

	int   num_leafs;
	short leafnums[MAX_ENT_LEAFS];

	float freetime; // sv.time when the object was freed

	void* pvPrivateData; // Alloced and freed by engine, used by DLLs
	SEntVars v; // C exported fields from progs

	// other fields from progs come immediately after
};

struct STrace
{
	bool   allsolid; // if true, plane is not valid
	bool   startsolid; // if true, the initial point was in a solid area
	bool   inopen, inwater;
	float  fraction; // time completed, 1.0 = didn't hit anything
	float  endpos[3]; // final position
	SPlane plane; // surface normal at impact
	SEdict *ent; // entity the surface is on
	int hitgroup; // 0 == generic, non zero is specific body part
};

struct SHpak
{
	char   *name;
	SResource HpakResource;
	size_t size;
	void   *data;
	SHpak  *next;
};

struct SHpakDir
{
	SResource DirectoryResource;
	int seek; // filepos ?
	int size;
};

struct SHpakContainer
{
	int      count;
	SHpakDir *dirs; // variable sized.
};

struct SWfile
{
	char   filename[MAX_SYSPATH];
	int    infotableofs;
	quint8 *mempool; // W_ReadLump temp buffers
	int    numlumps;
	int    mode;
	int    handle;
	SDlumpInfo *lumps;
	time_t filetime;
};

struct SPack
{
	char   filename[MAX_SYSPATH];
	int    handle;
	int    numfiles;
	time_t filetime; // common for all packed files
	SPackFile *files;
};

struct SSearchPath
{
	char   filename[MAX_SYSPATH];
	SPack  *pack;
	SWfile *wad;
	int    flags;
	SSearchPath *next;
};

struct SClientSprite
{
	char szName[64];
	char szSprite[64];
	int hspr;
	int iRes;
	SWrect rc;
};





struct SWorldStatic
{
	int version; // bsp version
	int mapversion; // map version (an key-value in worldspawn settings)
	uint checksum; // current map checksum
	int load_sequence; // increace each map change
	float SHullizes[MAX_MAP_HULLS][3]; // actual hull sizes
	SMsurface **draw_surfaces; // used for sorting translucent surfaces
	int max_surfaces; // max surfaces per submodel (for all models)
	size_t visdatasize; // actual size of the visdata
	size_t litdatasize; // actual size of the lightdata
	size_t vecdatasize; // actual size of the deluxdata
	size_t entdatasize; // actual size of the entity string
	size_t texdatasize; // actual size of the textures lump
	bool loading; // true if worldmodel is loading
	bool sky_sphere; // true when quake sky-sphere is used
	bool has_mirrors; // one or more brush models contain reflective textures
	int lm_sample_size; // defaulting to 16 (BSP31 uses 8)
	int block_size; // lightmap blocksize
	SColor24 *deluxedata; // deluxemap data pointer
	char message[2048]; // just for debug

	float mins[3]; // real accuracy world bounds
	float maxs[3];
	float size[3];
};




struct SPmtrace
{
	bool allsolid; // if true, plane is not valid
	bool startsolid; // if true, the initial point was in a solid area
	bool inopen, inwater;  // End point is in empty space or in water
	float fraction; // time completed, 1.0 = didn't hit anything
	float endpos[3]; // final position
	SPmplane plane; // surface normal at impact
	int ent; // entity at impact
	float deltavelocity[3]; // Change in player's velocity caused by impact.
				// Only run on server.
	int hitgroup;
};


// SPhysEnt
struct SPhysEnt
{
	char name[32]; // Name of model, or "player" or "world".
	int player;
	float origin[3]; // Model's origin in world coordinates.
	SModel *model; // only for bsp models
	SModel *studiomodel; // SOLID_BBOX, but studio clip intersections.
	float mins[3], maxs[3]; // only for non-bsp models
	int info; // For client or server to use to identify (index into edicts or cl_entities)
	float angles[3]; // rotated entities need this info for hull testing to work.

	int solid; // Triggers and func_door type WATER brushes are SOLID_NOT
	int skin; // BSP Contents for such things like fun_door water brushes.
	int rendermode; // So we can ignore glass

	// Complex collision detection.
	float frame;
	int sequence;
	quint8 controller[4];
	quint8 blending[2];

	int movetype;
	int takedamage;
	int blooddecal;
	int team;
	int classnumber;

	// For mods
	int iuser1;
	int iuser2;
	int iuser3;
	int iuser4;
	float fuser1; // also contains pev->scale when "sv_allow_studio_scaling" is "1"
	float fuser2;
	float fuser3;
	float fuser4;
	float vuser1[3];
	float vuser2[3];
	float vuser3[3];
	float vuser4[3];
};

struct SPlayerMove
{
	int player_index; // So we don't try to run the PM_CheckStuck nudging too quickly.
	bool server; // For debugging, are we running physics code on server side?

	bool multiplayer; // 1 == multiplayer server
	float time; // realtime on host, for reckoning duck timing
	float frametime; // Duration of this frame

	float forward[3], right[3], up[3]; // Vectors for angles

	// player state
	float origin[3]; // Movement origin.
	float angles[3]; // Movement view angles.
	float oldangles[3]; // Angles before movement view angles were looked at.
	float velocity[3]; // Current movement direction.
	float movedir[3]; // For waterjumping, a forced forward velocity so we can fly over lip of ledge.
	float basevelocity[3]; // Velocity of the conveyor we are standing, e.g.

	// For ducking/dead
	float view_ofs[3]; // Our eye position.
	float flDuckTime; // Time we started duck
	bool bInDuck; // In process of ducking or ducked already?

	// For walking/falling
	int flTimeStepSound; // Next time we can play a step sound
	int iStepLeft;

	float flFallVelocity;
	float punchangle[3];

	float flSwimTime;
	float flNextPrimaryAttack;

	int effects; // MUZZLE FLASH, e.g.

	int flags; // FL_ONGROUND, FL_DUCKING, etc.
	int usehull; // 0 = regular player hull, 1 = ducked player hull, 2 = point hull
	float gravity; // Our current gravity and friction.
	float friction;
	int oldbuttons; // Buttons last usercmd
	float waterjumptime; // Amount of time left in jumping out of water cycle.
	bool dead; // Are we a dead player?
	int deadflag;
	int spectator; // Should we use spectator physics model?
	int movetype; // Our movement type, NOCLIP, WALK, FLY

	int onground;
	int waterlevel;
	int watertype;
	int oldwaterlevel;

	char sztexturename[256];
	char chtexturetype;

	float maxspeed;
	float clientmaxspeed; // Player specific maxspeed

	// For mods
	int iuser1;
	int iuser2;
	int iuser3;
	int iuser4;
	float fuser1;
	float fuser2;
	float fuser3;
	float fuser4;
	float vuser1[3];
	float vuser2[3];
	float vuser3[3];
	float vuser4[3];

	// world state

	// Number of entities to clip against.
	int numphysent;
	SPhysEnt physents[MAX_PHYSENTS];

	// Number of momvement entities (ladders)
	int nummoveent;
	// just a list of ladders
	SPhysEnt moveents[MAX_MOVEENTS];

	// All things being rendered, for tracing against things you don't actually collide with
	int numvisent;
	SPhysEnt visents[MAX_PHYSENTS];

	// input to run through physics.
	SUserCmd cmd;

	// Trace results for objects we collided with.
	int numtouch;
	SPmtrace touchindex[MAX_PHYSENTS];

	char physinfo[MAX_PHYSINFO_STRING]; // Physics info string

SMoveVars *movevars;
	float player_mins[4][3];
	float player_maxs[4][3];

	// Common functions
	const char *(*PM_Info_ValueForKey) ( const char *s, const char *key );
	void (*PM_Particle)( float *origin, int color, float life, int zpos, int zvel );
	int (*PM_TestPlayerPosition)( float *pos, SPmtrace *ptrace );
	void (*Con_NPrintf)( int idx, char *fmt, ... );
	void (*Con_DPrintf)( char *fmt, ... );
	void (*Con_Printf)( char *fmt, ... );
	double (*Sys_FloatTime)( void );
	void (*PM_StuckTouch)( int hitent, SPmtrace *ptraceresult );
	int (*PM_PointContents)( float *p, int *truecontents /*filled in if this is non-null*/ );
	int (*PM_TruePointContents)( float *p );
	int (*PM_HullPointContents)(SHull *hull, int num, float *p );
	SPmtrace (*PM_PlayerTrace)( float *start, float *end, int traceFlags, int ignore_pe );
SPmtrace *(*PM_TraceLine)( float *start, float *end, int flags, int usehulll, int ignore_pe );
	long (*RandomLong)( long lLow, long lHigh );
	float (*RandomFloat)( float flLow, float flHigh );
	int (*PM_GetModelType)(SModel *mod );
	void (*PM_GetModelBounds)(SModel *mod, float *mins, float *maxs );
	void *(*PM_HullForBsp)( SPhysEnt *pe, float *offset );
	float (*PM_TraceModel)( SPhysEnt *pEnt, float *start, float *end, STrace *trace );
	int (*COM_FileSize)( char *filename );
	quint8 *(*COM_LoadFile)( char *path, int usehunk, int *pLength );
	void (*COM_FreeFile)( void *buffer );
	char *(*memfgets)( quint8 *pMemFile, int fileSize, int *pFilePos, char *pBuffer, int bufferSize );

	// Functions
	// Run functions for this frame?
	bool runfuncs;
	void (*PM_PlaySound)( int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch );
	const char *(*PM_TraceTexture)( int ground, float *vstart, float *vend );
	void (*PM_PlaybackEventFull)( int flags, int clientindex, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );
	SPmtrace (*PM_PlayerTraceEx) (float *start, float *end, int traceFlags, int (*pfnIgnore)( SPhysEnt *pe ));
	int (*PM_TestPlayerPositionEx) (float *pos, SPmtrace *ptrace, int (*pfnIgnore)( SPhysEnt *pe ));
	SPmtrace *(*PM_TraceLineEx)( float *start, float *end, int flags, int usehulll, int (*pfnIgnore)( SPhysEnt *pe ));
};



struct SDlight
{
	float origin[3];
	float radius;
	SColor24 color;
	float die; // stop lighting after this time
	float decay; // drop this each second
	float minlight; // don't add when contributing less
	int key;
	bool dark; // subtracts light instead of adding
};



struct SDecalList
{
	float position[3];
	char name[64];
	short entityIndex;
	quint8 depth;
	quint8 flags;
	float scale;

	// this is the surface plane that we hit so that
	// we can move certain decals across
	// transitions if they hit similar geometry
	float impactPlaneNormal[3];

	SModelState studio_state; // studio decals only
};


struct SFlow
{
	SFlowStats stats[MAX_LATENT]; // data for last MAX_LATENT messages
	int current; // current message position
	double nextcompute; 	// time when we should recompute k/sec data
	float kquint8spersec; // average data
	float avgkquint8spersec;
	int totalquint8s;
};

// generic fragment structure
struct SFragBuf
{
	SFragBuf *next; // next buffer in chain
	int bufferid; // id of this buffer
	SSizeBuf frag_message; // message buffer where raw data is stored
	quint8 frag_message_buf[FRAGMENT_SIZE]; // the actual data sits here
	bool isfile; // is this a file buffer?
	bool isbuffer; // is this file buffer from memory ( custom decal, etc. ).
	char filename[CS_SIZE]; // name of the file to save out on remote host
	int foffset; // offset in file from which to read data
	int size; // size of data to read at that offset
};

// Waiting list of fragbuf chains
struct SFragBufWaiting
{
SFragBufWaiting *next; // next chain in waiting list
	int fragbufcount; // number of buffers in this chain
	SFragBuf *fragbufs; // the actual buffers
};

// Network Connection Channel
struct SNetChan
{
	ENetSrc sock; // NS_SERVER or NS_CLIENT, depending on channel.
	SNetAdr remote_address; // address this channel is talking to.
	int qport; // qport value to write when transmitting

	bool compress; // enable huffman compression

	double last_received; // for timeouts
	double last_sent; // for retransmits

	double rate; // bandwidth choke. quint8s per second
	double cleartime; // if realtime > cleartime, free to send next packet
	double connect_time; // Usage: host.realtime - netchan.connect_time

	int drop_count; // dropped packets, cleared each level
	int good_count; // cleared each level

	// Sequencing variables
	int incoming_sequence; // increasing count of sequence numbers
	int incoming_acknowledged; // # of last outgoing message that has been ack'd.

	int incoming_reliable_acknowledged; // toggles T/F as reliable messages are received.
	int incoming_reliable_sequence; // single bit, maintained local
	int outgoing_sequence; // message we are sending to remote
	int reliable_sequence; // whether the message contains reliable payload, single bit
	int last_reliable_sequence; // outgoing sequence number of last send that had reliable data

	// staging and holding areas
	SSizeBuf message;
	quint8 message_buf[NET_MAX_PAYLOAD];

	// reliable message buffer.
	// we keep adding to it until reliable is acknowledged.  Then we clear it.
	int reliable_length;
	quint8 reliable_buf[NET_MAX_PAYLOAD]; // unacked reliable message

	// Waiting list of buffered fragments to go onto queue.
	// Multiple outgoing buffers can be queued in succession
	SFragBufWaiting *waitlist[MAX_STREAMS];

	int reliable_fragment[MAX_STREAMS]; // is reliable waiting buf a fragment?
	uint reliable_fragid[MAX_STREAMS]; // buffer id for each waiting fragment

	SFragBuf *fragbufs[MAX_STREAMS]; // the current fragment being set
	int fragbufcount[MAX_STREAMS]; // the total number of fragments in this stream

	short frag_startpos[MAX_STREAMS]; // position in outgoing buffer where frag data starts
	short frag_length[MAX_STREAMS]; // length of frag data in the buffer

	SFragBuf *incomingbufs[MAX_STREAMS]; // incoming fragments are stored here
	bool incomingready[MAX_STREAMS]; // set to true when incoming data is ready

	// Only referenced by the FRAG_FILE_STREAM component
	char incomingfilename[CS_SIZE]; // Name of file being downloaded

	// incoming and outgoing flow metrics
	SFlow flow[MAX_FLOWS];

	// added for net_speeds
	size_t total_sended;
	size_t total_sended_uncompressed;

	size_t total_received;
	size_t total_received_uncompressed;
};





struct SNetAdrList
{
	SNetAdrList *next;
	SNetAdr remote_address;
};

struct SNetResponce
{
	// NET_SUCCESS or an error code
	int error;
	// Context ID
	int context;
	// Type
	int type;
	// Server that is responding to the request
	SNetAdr remote_address;
	// Response RTT ping time
	double ping;
	// Key/Value pair string ( separated by backlash \ characters )
	// WARNING:  You must copy this buffer in the callback function, because it is freed
	// by the engine right after the call!!!!
	// ALSO: For NETAPI_REQUEST_SERVERLIST requests, this will be a pointer to a linked list of SNetAdrList's
	void *response;
};

struct SNetStatus
{
	// Connected to remote server?  1 == yes, 0 otherwise
	int connected;
	// Client's IP address
	SNetAdr local_address;
	// Address of remote server
	SNetAdr remote_address;
	// Packet Loss ( as a percentage )
	int packet_loss;
	// Latency, in seconds ( multiply by 1000.0 to get milliseconds )
	double latency;
	// Connection time, in seconds
	double connection_time;
	// Rate setting ( for incoming data )
	double rate;
};

struct SEventInfo
{
	quint16 index; // 0 implies not in use
	short packet_index; // Use data from state info for entity in delta_packet .
					// -1 implies separate info based on event
					// parameter signature
	short entity_index; // The edict this event is associated with
	float fire_time; // if non-zero, the time when the event should be fired
					// ( fixed up on the client )
	SEventArgs args;
	int flags; // reliable or not, etc. ( CLIENT ONLY )
};

struct SEventState
{
	SEventInfo ei[MAX_EVENT_QUEUE];
};

//=============================================================================
struct SFrame
{
	// received from server
	double receivedtime; // time message was received, or -1
	double latency;
	double time; // server timestamp

	SLocalState local; // local client state
	SEntityState playerstate[MAX_CLIENTS];
	int num_entities;
	int first_entity; // into the circular cl_packet_entities[]

	bool valid; // cleared if delta parsing was invalid
};

// the SClient structure is wiped completely at every
// server map change
struct SClient
{
	int timeoutcount;

	int servercount; // server identification for prespawns
	int validsequence; // this is the sequence number of the last good
						// world snapshot/update we got.  If this is 0, we can't
						// render a frame yet
	int parsecount; // server message counter
	int parsecountmod; // modulo with network window

	bool video_prepped; // false if on new level or new ref dll
	bool audio_prepped; // false if on new level or new snd dll
	bool force_refdef; // vid has changed, so we can't use a paused refdef

	int delta_sequence; // acknowledged sequence number

	double mtime[2]; // the timestamp of the last two messages


	int last_incoming_sequence;

	bool force_send_usercmd;
	bool thirdperson;
	bool background; // not real game, just a background

	uint checksum; // for catching cheater maps

	SClientDataShort data; // some clientdata holds

	SFrame frame; // received from server
	int predictcount; // advances with next clientdata
	SFrame frames[MULTIPLAYER_BACKUP]; // alloced on svc_serverdata
	SUserCmd cmds[MULTIPLAYER_BACKUP]; // each mesage will send several old cmds
	SLocalState predict[MULTIPLAYER_BACKUP]; // local client state

	double time; // this is the time value that the client
						// is rendering at.  always <= cls.realtime
						// a lerp point for other data
	double oldtime; // previous cl.time, time-oldtime is used
						// to decay light values and smooth step ups

	float lerpFrac; // interpolation value
	SRefParams refdef; // shared refdef

	char serverinfo[MAX_INFO_STRING];
	SPlayerInfo players[MAX_CLIENTS];
	SEventState events;

	// predicting stuff
	float predicted_origin[3]; // generated by CL_PredictMovement
	float predicted_viewofs[3];
	float predicted_velocity[3];
	float predicted_punchangle[3];

	// server state information
	int playernum;
	int maxclients;
	int movemessages;
	int num_custombeams; // server beams count

	char model_precache[MAX_MODELS][CS_SIZE];
	char sound_precache[MAX_SOUNDS][CS_SIZE];
	char event_precache[MAX_EVENTS][CS_SIZE];
	SLightStyle lightstyles[MAX_LIGHTSTYLES];

	int sound_index[MAX_SOUNDS];
	int decal_index[MAX_DECALS];

	SClEntity *world;
	SModel *worldmodel; // pointer to world
};

struct SClUserMessage
{
	char name[32];
	int number; // svc_ number
	int size; // if size == -1, size come from first quint8 after svcnum
	pfnUserMsgHook func; // user-defined function
};



struct SClUserEvent
{
	char name[CS_SIZE];
	quint16 index; // event index
	pfnEventHook func; // user-defined function
};



struct SClientDraw
{
	// temp handle
	const SModel *pSprite; // pointer to current SpriteTexture

	// scissor test
	int scissor_x;
	int scissor_y;
	int scissor_width;
	int scissor_height;
	bool scissor_test;
	bool adjust_size; // allow to adjust scale for fonts

	int cullMode; // override CULL FACE from TriAPI

	// holds text color
	quint8 textColor[4];
	quint8 spriteColor[4];
	quint8 triColor[4];

	// crosshair members
	const SModel *pCrosshair;
	SWrect rcCrosshair;
	quint8 rgbaCrosshair[4];
	quint8 gammaTable[256];
};





struct SRemapInfo
{
	SMstudioTex *ptexture; // array of textures with local copy of remapped textures
	short numtextures; // textures count
	short topcolor; // cached value
	short bottomcolor; // cached value
	SModel *model; // for catch model changes
};

struct SNetRequest
{
	SNetResponce resp;
	net_api_response_func_t pfnFunc;
	double timeout;
	double timesend; // time when request was sended
	int flags; // FNETAPI_MULTIPLE_RESPONSE etc
};

struct SClgameStatic
{
	void *hInstance; // pointer to client.dll
	ICldllFunc *dllFuncs; // dll exported funcs
	IRenderInterface *drawFuncs; // custom renderer support
	quint8 *mempool; // client edicts pool
	QString mapname; // map name
	QString maptitle; // display map title
	QString itemspath; // path to items description for auto-complete func

	SClEntity *entities; // dynamically allocated entity array
	SClEntity *static_entities; // dynamically allocated static entity array
	SRemapInfo **remap_info; // store local copy of all remap textures for each entity

	int maxEntities;
	int maxRemapInfos; // maxEntities + cl.viewEnt; also used for catch entcount
	int numStatics; // actual static entity count

	// movement values from server
	SMoveVars movevars;
	SMoveVars oldmovevars;
	SPlayerMove *pmove; // pmove state

	int old_trace_hull; // used by PM_Push\Pop state
	int oldcount; // used by PM_Push\Pop state

	float player_mins[MAX_MAP_HULLS][3]; // 4 hulls allowed
	float player_maxs[MAX_MAP_HULLS][3]; // 4 hulls allowed

	SClUserMessage msg[MAX_USER_MESSAGES]; // keep static to avoid fragment memory
	SClUserEvent *events[MAX_EVENTS];

	QString cdtracks[MAX_CDTRACKS]; // 32 cd-tracks read from cdaudio.txt

	SModel sprites[MAX_IMAGES]; // client spritetextures
	int load_sequence; // for unloading unneeded sprites

	SClientDraw ds; // draw2d stuff (hud, weaponmenu etc)
	SScreenFade fade; // screen fade
	SScreenShake shake; // screen shake
	SCenterPrint centerPrint; // centerprint variables
	SScreenInfo scrInfo; // actual screen info
	SRefOverview overView; // overView params
	quint8 palette[256][3]; // palette used for particle colors

	SClientTextMessage *titles; // title messages, not network messages
	int numTitles;

	SNetRequest net_requests[MAX_REQUESTS]; // no reason to keep more

	SEfrag *free_efrags; // linked efrags
	SClEntity viewent; // viewmodel
};

struct SMenuStatic
{
	void *hInstance; // pointer to client.dll
	IUiFunctions *dllFuncs; // dll exported funcs
	quint8 *mempool; // client edicts pool

	SClEntity playermodel; // uiPlayerSetup drawing model
	SPlayerInfo playerinfo; // local playerinfo

	SGameuiDraw ds; // draw2d stuff (menu images)
	SModInfo gameInfo; // current gameInfo
	SModInfo *modsInfo[MAX_MODS]; // simplified gameInfo for MainUI

	SUiGlobalVars *globals;

	bool drawLogo; // set to TRUE if logo.avi missed or corrupted
	long logo_xres;
	long logo_yres;
	float logo_length;
};

struct SClientStatic
{
	EConnState state;
	bool initialized;
	bool changelevel; // during changelevel
	bool changedemo; // during changedemo

	// screen rendering information
	float disable_screen; // showing loading plaque between levels
						// or changing rendering dlls
						// if time gets > 30 seconds ahead, break it
	int disable_servercount; // when we receive a frame and cl.servercount
						// > cls.disable_servercount, clear disable_screen

	bool draw_changelevel; // draw changelevel image 'Loading...'

	EKeyDest key_dest;

	quint8 *mempool; // client premamnent pool: edicts etc

	int framecount;
	int quakePort; // a 16 bit value that allows quake servers
						// to work around address translating routers
						// g-cont. this port allow many copies of engine in multiplayer game
	// connection information
	QString servername; // name of server from original connect
	double connect_time; // for connection retransmits


	SSizeBuf datagram; // unreliable stuff. gets sent in CL_Move about cl_cmdrate times per second.
	quint8 datagram_buf[NET_MAX_PAYLOAD];

	SNetChan netchan;
	int serverProtocol; // in case we are doing some kind of version hack
	int challenge; // from the server to use for connecting

	float packet_loss;
	double packet_loss_recalc_time;

	float nextcmdtime; // when can we send the next command packet?
	int lastoutgoingcommand; // sequence number of last outgoing command

	// internal images
	int fillImage; // used for emulate FillRGBA to avoid wrong draw-sort
	int particleImage; // built-in particle and sparks image
	int pauseIcon; // draw 'paused' when game in-pause
	int loadingBar; // 'loading' progress bar
	int glowShell; // for renderFxGlowShell
	int tileImage; // for draw any areas not covered by the refresh
	int hChromeSprite; // this is a really HudSprite handle, not texnum!
	SClFont creditsFont; // shared creditsfont

	int num_client_entities; // cl.maxclients * CL_UPDATE_BACKUP * MAX_PACKET_ENTITIES
	int next_client_entities; // next client_entity to use
	SEntityState *packet_entities; // [num_client_entities]

	EScrShot scrshot_request; // request for screen shot
	EScrShot scrshot_action; // in-action
	const float *envshot_vieworg; // envshot position
	int envshot_viewsize; // override cvar
	bool envshot_disable_vis; // disable VIS on server while makes an envshots
	QString shotname;

	// download info
	int downloadcount;
	int downloadfileid;

	// demo loop control
	int demonum; // -1 = don't play demos
	int olddemonum; // restore playing
	QString demos[MAX_DEMOS]; // when not playing

	// movie playlist
	int movienum;
	QString movies[MAX_MOVIES];

	// demo recording info must be here, so it isn't clearing on level change
	bool demorecording;
	bool demoplayback;
	bool demowaiting; // don't record until a non-delta message is received
	bool timedemo;
	QString demoname; // for demo looping
	double demotime; // recording time

	SFile *demofile;
	SFile *demoheader; // contain demo startup info in case we record a demo on this level
};


// ---------------------------------------- //
// ---------------- ENUMS ----------------- //
// ---------------------------------------- //

struct INetApi
{
	// APIs
	void (*InitNetworking)( void );
	void (*Status )( SNetStatus *status );
	void (*SendRequest)( int context, int request, int flags, double timeout, SNetAdr *remote_address, net_api_response_func_t response );
	void (*CancelRequest)( int context );
	void (*CancelAllRequests)( void );
	char *(*AdrToString)( SNetAdr *a );
	int ( *CompareAdr)( SNetAdr *a, SNetAdr *b );
	int ( *StringToAdr)( char *s, SNetAdr *a );
	const char *(*ValueForKey)( const char *s, const char *key );
	void (*RemoveKey)( char *s, const char *key );
	void (*SetValueForKey)( char *s, const char *key, const char *value, int maxsize );
};

struct IEventApi
{
	int version;
	void ( *EV_PlaySound )( int ent, float *origin, int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch );
	void ( *EV_StopSound )( int ent, int channel, const char *sample );
	int ( *EV_FindModelIndex )( const char *pmodel );
	int ( *EV_IsLocal )( int playernum );
	int ( *EV_LocalPlayerDucking )( void );
	void ( *EV_LocalPlayerViewheight )( float * );
	void ( *EV_LocalPlayerBounds )( int hull, float *mins, float *maxs );
	int ( *EV_IndexFromTrace)(SPmtrace *pTrace );
SPhysEnt *( *EV_GetPhysent )( int idx );
	void ( *EV_SetUpPlayerPrediction )( int dopred, int bIncludeLocalClient );
	void ( *EV_PushPMStates )( void );
	void ( *EV_PopPMStates )( void );
	void ( *EV_SetSolidPlayers )( int playernum );
	void ( *EV_SetTraceHull )( int hull );
	void ( *EV_PlayerTrace )( float *start, float *end, int traceFlags, int ignore_pe,SPmtrace *tr );
	void ( *EV_WeaponAnimation )( int sequence, int body );
	unsigned short ( *EV_PrecacheEvent )( int type, const char* psz );
	void ( *EV_PlaybackEvent )( int flags, const SEdict *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );
	const char *( *EV_TraceTexture )( int ground, float *vstart, float *vend );
	void ( *EV_StopAllSounds )( int entnum, int entchannel );
	void ( *EV_KillEvents )( int entnum, const char *eventname );

	// Xash3D extension
	unsigned short (*EV_IndexForEvent)( const char *name );
	const char *(*EV_EventForIndex)( unsigned short index );
	void ( *EV_PlayerTraceExt )( float *start, float *end, int traceFlags, int (*pfnIgnore)(SPhysEnt *pe ),SPmtrace *tr );
	const char *(*EV_SoundForIndex)( int index );
};

struct IRenderApi
{
	// Get renderer info (doesn't changes engine state at all)
	int (*RenderGetParm)( int parm, int arg ); // generic
	void (*GetDetailScaleForTexture)( int texture, float *xScale, float *yScale );
	void (*GetExtraParmsForTexture)( int texture, quint8 *red, quint8 *green, quint8 *blue, quint8 *alpha );
	SLightStyle * (*GetLightStyle)( int number );
	SDlight * (*GetDynamicLight)( int number );
	SDlight * (*GetEntityLight)( int number );
	quint8 (*TextureToTexGamma)( quint8 color ); // software gamma support
	void (*GetBeamChains)( SBeam ***active_beams, SBeam ***free_beams, SParticle ***free_trails );

	// Set renderer info (tell engine about changes)
	void (*R_SetCurrentEntity)(SEntity *ent ); // tell engine about both currententity and currentmodel
	void (*R_SetCurrentModel)(SModel *mod ); // change currentmodel but leave currententity unchanged
	void (*GL_SetWorldviewProjectionMatrix)( const float *glmatrix ); // update viewprojection matrix (tracers uses it)
	void (*R_StoreEfrags)(SEfrag **ppefrag, int framecount );// store efrags for static entities

	// Texture tools
	int (*GL_FindTexture)( const char *name );
	const char * (*GL_TextureName)( unsigned int texnum );
	const quint8 * (*GL_TextureData)( unsigned int texnum ); // may be NULL
	int (*GL_LoadTexture)( const char *name, const quint8 *buf, size_t size, int flags );
	int (*GL_CreateTexture)( const char *name, int width, int height, const void *buffer, int flags );
	void (*GL_SetTextureType)( unsigned int texnum, unsigned int type );
	void (*GL_TextureCacheFrame)( unsigned int texnum );
	void (*GL_FreeTexture)( unsigned int texnum );

	// Decals manipulating (draw & remove)
	void (*DrawSingleDecal)(SDecal *pDecal,SMsurface *fa );
	float *(*R_DecalSetupVerts)(SDecal *pDecal,SMsurface *surf, int texture, int *outCount );
	void (*R_EntityRemoveDecals)(SModel *mod ); // remove all the decals from specified entity (BSP only)

	// AVIkit support
	void *(*AVI_LoadVideo)( const char *filename, int ignore_hwgamma );
	int (*AVI_GetVideoInfo)( void *Avi, long *xres, long *yres, float *duration );
	long (*AVI_GetVideoFrameNumber)( void *Avi, float time );
	quint8 *(*AVI_GetVideoFrame)( void *Avi, long frame );
	void (*AVI_UploadRawFrame)( int texture, int cols, int rows, int width, int height, const quint8 *data );
	void (*AVI_FreeVideo)( void *Avi );
	int (*AVI_IsActive)( void *Avi );

	// glState related calls (must use this instead of normal gl-calls to prevent de-synchornize local states between engine and the client)
	void (*GL_Bind)( int tmu, unsigned int texnum );
	void (*GL_SelectTexture)( int tmu );
	void (*GL_LoadTextureMatrix)( const float *glmatrix );
	void (*GL_TexMatrixIdentity)( void );
	void (*GL_CleanUpTextureUnits)( int last ); // pass 0 for clear all the texture units
	void (*GL_TexGen)( unsigned int coord, unsigned int mode );
	void (*GL_TextureTarget)( unsigned int target ); // change texture unit mode without bind texture
	void (*GL_TexCoordArrayMode)( unsigned int texmode );
	void (*GL_Reserved0)( void ); // for potential interface expansion without broken compatibility
	void (*GL_Reserved1)( void );
	void (*GL_Reserved2)( void );
	void (*GL_Reserved3)( void );

	// Misc renderer functions
	void (*GL_DrawParticles)( const float *vieworg, const float *fwd, const float *rt, const float *up, unsigned int clipFlags );
	void (*EnvShot)( const float *vieworg, const char *name, bool skyshot, int shotsize ); // creates a cubemap or skybox into gfx\env folder
	int (*COM_CompareFileTime)( const char *filename1, const char *filename2, int *iCompare );
	void (*Host_Error)( const char *error, ... ); // cause Host Error
	int (*SPR_LoadExt)( const char *szPicName, unsigned int texFlags ); // extended version of SPR_Load
	void (*TessPolygon)(SMsurface *surf,SModel *mod, float tessSize );
	SMstudioTex *( *StudioGetTexture )(SEntity *e );
	const SRefOverview *( *GetOverviewParms )( void );
	void (*S_FadeMusicVolume)( float fadePercent ); // fade background track (0-100 percents)
	void (*SetRandomSeed)( long lSeed ); // set custom seed for RANDOM_FLOAT\RANDOM_LONG for predictable random
	// static allocations
	void *(*pfnMemAlloc)( size_t cb, const char *filename, const int fileline );
	void (*pfnMemFree)( void *mem, const char *filename, const int fileline );
	// find in files
	char **(*pfnGetFilesList)( const char *pattern, int *numFiles, int gamedironly );
	// ONLY ADD NEW FUNCTIONS TO THE END OF THIS STRUCT.  INTERFACE VERSION IS FROZEN AT 35
};

// render callbacks
struct IRenderInterface
{
	int version;
	// passed through R_RenderFrame (0 - use engine renderer, 1 - use custom client renderer)
	int (*GL_RenderFrame)( const SRefParams *pparams, bool drawWorld );
	// build all the lightmaps on new level or when gamma is changed
	void (*GL_BuildLightmaps)( void );
	// setup map bounds for ortho-projection when we in dev_overview mode
	void (*GL_OrthoBounds)( const float *mins, const float *maxs );
	// handle decals which hit mod_studio or mod_sprite
	void (*R_StudioDecalShoot)( int decalTexture,SEntity *ent, const float *start, const float *pos, int flags, SModelState *state );
	// prepare studio decals for save
	int (*R_CreateStudioDecalList)( SDecalList *pList, int count, bool changelevel );
	// clear decals by engine request (e.g. for demo recording or vid_restart)
	void (*R_ClearStudioDecals)( void );
	// grab r_speeds message
	bool (*R_SpeedsMessage)( char *out, size_t size );
	// replace with built-in R_DrawCubemapView for make skyshots or envshots
	bool (*R_DrawCubemapView)( const float *origin, const float *angles, int size );
	// alloc or destroy studiomodel custom data
	void (*Mod_ProcessUserData)(SModel *mod, bool create, const quint8 *buffer );
};



// NOTE: ordering is important!
struct ICldllFunc
{
	int (*pfnInitialize)( IClEngineFuncs *pEnginefuncs, int iVersion );
	void (*pfnInit)( void );
	int (*pfnVidInit)( void );
	int (*pfnRedraw)( float flTime, int intermission );
	int (*pfnUpdateClientData)( SClientDataShort *cdata, float flTime );
	void (*pfnReset)( void );
	void (*pfnPlayerMove)(SPlayerMove *ppmove, int server );
	void (*pfnPlayerMoveInit)(SPlayerMove *ppmove );
	char (*pfnPlayerMoveTexture)( char *name );
	void (*IN_ActivateMouse)( void );
	void (*IN_DeactivateMouse)( void );
	void (*IN_MouseEvent)( int mstate );
	void (*IN_ClearStates)( void );
	void (*IN_Accumulate)( void );
	void (*CL_CreateMove)( float frametime,SUserCmd *cmd, int active );
	int (*CL_IsThirdPerson)( void );
	void (*CL_CameraOffset)( float *ofs );
	void *(*KB_Find)( const char *name );
	void (*CAM_Think)( void ); // camera stuff
	void (*pfnCalcRefdef)( SRefParams *pparams );
	int (*pfnAddEntity)( int type, SClEntity *ent, const char *modelname );
	void (*pfnCreateEntities)( void );
	void (*pfnDrawNormalTriangles)( void );
	void (*pfnDrawTransparentTriangles)( void );
	void (*pfnStudioEvent)( const SMstudioEvent *event, const SClEntity *entity );
	void (*pfnPostRunCmd)(SLocalState *from,SLocalState *to, SUserCmd *cmd, int runfuncs, double time, unsigned int random_seed );
	void (*pfnShutdown)( void );
	void (*pfnTxferLocalOverrides)( SEntityState *state, const SClientDataShort *client );
	void (*pfnProcessPlayerState)( SEntityState *dst, const SEntityState *src );
	void (*pfnTxferPredictionData)( SEntityState *ps, const SEntityState *pps, SClientDataShort *pcd, const SClientDataShort *ppcd, SWeaponData *wd, const SWeaponData *pwd );
	void (*pfnDemo_ReadBuffer)( int size, quint8 *buffer );
	int (*pfnConnectionlessPacket)( const SNetAdr *net_from, const char *args, char *buffer, int *size );
	int (*pfnGetHullBounds)( int hullnumber, float *mins, float *maxs );
	void (*pfnFrame)( double time );
	int (*pfnKey_Event)( int eventcode, int keynum, const char *pszCurrentBinding );
	void (*pfnTempEntUpdate)( double frametime, double SClientime, double cl_gravity,STempEnt **ppTempEntFree,STempEnt **ppTempEntActive, int ( *Callback_AddVisibleEntity )( SClEntity *pEntity ), void ( *Callback_TempEntPlaySound )(STempEnt *pTemp, float damp ));
	SClEntity *(*pfnGetUserEntity)( int index );
	void (*pfnVoiceStatus)( int entindex, bool bTalking );
	void (*pfnDirectorMessage)( int iSize, void *pbuf );
	int (*pfnGetStudioModelInterface)( int version, IStudioInterface **ppinterface, IEngineStudioApi *pstudio );
	void (*pfnChatInputPosition)( int *x, int *y );
	int (*pfnGetPlayerTeam)( int playerIndex );
	void *(*pfnGetClientFactory)( void );
	// Xash3D extension
	int (*pfnGetRenderInterface)( int version, IRenderApi *renderfuncs, IRenderInterface *callback );
	void (*pfnClipMoveToEntity)(SPhysEnt *pe, const float start[3], float mins[3], float maxs[3], const float end[3],SPmtrace *tr );
};

struct IClEngineFuncs
{
	// sprite handlers
	int (*pfnSPR_Load)( const char *szPicName );
	int (*pfnSPR_Frames)( int hPic );
	int (*pfnSPR_Height)( int hPic, int frame );
	int (*pfnSPR_Width)( int hPic, int frame );
	void (*pfnSPR_Set)( int hPic, int r, int g, int b );
	void (*pfnSPR_Draw)( int frame, int x, int y, const SWrect *prc );
	void (*pfnSPR_DrawHoles)( int frame, int x, int y, const SWrect *prc );
	void (*pfnSPR_DrawAdditive)( int frame, int x, int y, const SWrect *prc );
	void (*pfnSPR_EnableScissor)( int x, int y, int width, int height );
	void (*pfnSPR_DisableScissor)( void );
	SClientSprite *(*pfnSPR_GetList)( char *psz, int *piCount );

	// screen handlers
	void (*pfnFillRGBA)( int x, int y, int width, int height, int r, int g, int b, int a );
	int (*pfnGetScreenInfo)( SScreenInfo *pscrinfo );
	void (*pfnSetCrosshair)( int hspr, SWrect rc, int r, int g, int b );

	// cvar handlers
	SCvar *(*pfnRegisterVariable)( char *szName, char *szValue, int flags );
	float (*pfnGetCvarFloat)( char *szName );
	char * (*pfnGetCvarString)( char *szName );

	// command handlers
	int (*pfnAddCommand)( char *cmd_name, void (*function)(void) );
	int (*pfnHookUserMsg)( char *szMsgName, pfnUserMsgHook pfn );
	int (*pfnServerCmd)( char *szCmdString );
	int (*pfnClientCmd)( char *szCmdString );

	void (*pfnGetPlayerInfo)( int ent_num, SHudPlayerInfo *pinfo );

	// sound handlers
	void (*pfnPlaySoundByName)( char *szSound, float volume );
	void (*pfnPlaySoundByIndex)( int iSound, float volume );

	// vector helpers
	void (*pfnAngleVectors)( const float *vecAngles, float *forward, float *right, float *up );

	// text message system
	SClientTextMessage *(*pfnTextMessageGet)( const char *pName );
	int (*pfnDrawCharacter)( int x, int y, int number, int r, int g, int b );
	int (*pfnDrawConsoleString)( int x, int y, char *string );
	void (*pfnDrawSetTextColor)( float r, float g, float b );
	void (*pfnDrawConsoleStringLen)(  const char *string, int *length, int *height );

	void (*pfnConsolePrint)( const char *string );
	void (*pfnCenterPrint)( const char *string );

	// Added for user input processing
	int (*GetWindowCenterX)( void );
	int (*GetWindowCenterY)( void );
	void (*GetViewAngles)( float * );
	void (*SetViewAngles)( float * );
	int (*GetMaxClients)( void );
	void (*Cvar_SetValue)( char *cvar, float value );

	int       (*Cmd_Argc)( void );
	char *(*Cmd_Argv)( int arg );
	void (*Con_Printf)( char *fmt, ... );
	void (*Con_DPrintf)( char *fmt, ... );
	void (*Con_NPrintf)( int pos, char *fmt, ... );
	void (*Con_NXPrintf)(SConNprint *info, char *fmt, ... );

	const char* (*PhysInfo_ValueForKey)( const char *key );
	const char* (*ServerInfo_ValueForKey)( const char *key );
	float (*GetClientMaxspeed)( void );
	int (*CheckParm)( char *parm, char **ppnext );

	void (*Key_Event)( int key, int down );
	void (*GetMousePosition)( int *mx, int *my );
	int (*IsNoClipping)( void );

	SEntity *(*GetLocalPlayer)( void );
	SEntity *(*GetViewModel)( void );
	SEntity *(*GetEntityByIndex)( int idx );

	float (*GetClientTime)( void );
	void (*V_CalcShake)( void );
	void (*V_ApplyShake)( float *origin, float *angles, float factor );

	int (*PM_PointContents)( float *point, int *truecontents );
	int (*PM_WaterEntity)( float *p );
	SPmtrace *(*PM_TraceLine)( float *start, float *end, int flags, int usehull, int ignore_pe );

	SModel *(*CL_LoadModel)( const char *modelname, int *index );
	int (*CL_CreateVisibleEntity)( int type, SEntity *ent );

	const SModel* (*GetSpritePointer)( int hSprite );
	void (*pfnPlaySoundByNameAtLocation)( char *szSound, float volume, float *origin );

	unsigned short (*pfnPrecacheEvent)( int type, const char* psz );
	void (*pfnPlaybackEvent)( int flags, const SEdict *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );
	void (*pfnWeaponAnim)( int iAnim, int body );
	float (*pfnRandomFloat)( float flLow, float flHigh );
	long (*pfnRandomLong)( long lLow, long lHigh );
	void (*pfnHookEvent)( char *name, void ( *pfnEvent )(SEventArgs *args ));
	int (*Con_IsVisible) ();
	const char *(*pfnGetGameDirectory)( void );
	SCvar *(*pfnGetCvarPointer)( const char *szName );
	const char *(*Key_LookupBinding)( const char *pBinding );
	const char *(*pfnGetLevelName)( void );
	void (*pfnGetScreenFade)( SScreenFade *fade );
	void (*pfnSetScreenFade)( SScreenFade *fade );
	void * (*VGui_GetPanel)( );
	void (*VGui_ViewportPaintBackground)( int extents[4] );

	quint8 * (*COM_LoadFile)( char *path, int usehunk, int *pLength );
	char * (*COM_ParseFile)( char *data, char *token );
	void (*COM_FreeFile)( void *buffer );

	ITriangleApi *pTriAPI;
	IEfxApi *pEfxAPI;
	IEventApi *pEventAPI;
	IDemoApi *pDemoAPI;
	INetApi *pNetAPI;
	IVoiceTweak_s *pVoiceTweak;

	// returns 1 if the client is a spectator only (connected to a proxy), 0 otherwise or 2 if in dev_overview mode
	int (*IsSpectateOnly)( void );
	SModel *(*LoadMapSprite)( const char *filename );

	// file search functions
	void	 (*COM_AddAppDirectoryToSearchPath)( const char *pszBaseDir, const char *appName );
	int	 (*COM_ExpandFilename)( const char *fileName, char *nameOutBuffer, int nameOutBufferSize );

	// User info
	// playerNum is in the range (1, MaxClients)
	// returns NULL if player doesn't exit
	// returns "" if no value is set
	const char *( *PlayerInfo_ValueForKey )( int playerNum, const char *key );
	void (*PlayerInfo_SetValueForKey )( const char *key, const char *value );

	// Gets a unique ID for the specified player. This is the same even if you see the player on a different server.
	// iPlayer is an entity index, so client 0 would use iPlayer=1.
	// Returns false if there is no player on the server in the specified slot.
	bool (*GetPlayerUniqueID)(int iPlayer, char playerID[16]);

	// TrackerID access
	int (*GetTrackerIDForPlayer)(int playerSlot);
	int (*GetPlayerForTrackerID)(int trackerID);

	// Same as pfnServerCmd, but the message goes in the unreliable stream so it can't clog the net stream
	// (but it might not get there).
	int ( *pfnServerCmdUnreliable )( char *szCmdString );

	void (*pfnGetMousePos)( STagPoint *ppt );
	void (*pfnSetMousePos)( int x, int y );
	void (*pfnSetMouseEnable)( bool fEnable );

	// undocumented interface starts here
	SCvar * (*pfnGetFirstCvarPtr)( void );
	void * (*pfnGetFirstCmdFunctionHandle)( void );
	void * (*pfnGetNextCmdFunctionHandle)( void *cmdhandle );
	const char * (*pfnGetCmdFunctionName)( void *cmdhandle );
	float (*pfnGetClientOldTime)( void );
	float (*pfnGetGravity)( void );
	SModel * (*pfnGetModelByIndex)( int index );
	void (*pfnSetFilterMode)( int mode ); // same as gl_texsort in original Quake
	void (*pfnSetFilterColor)( float red, float green, float blue );
	void (*pfnSetFilterBrightness)( float brightness );
	void *(*pfnSequenceGet)( const char *fileName, const char *entryName );
	void (*pfnSPR_DrawGeneric)( int frame, int x, int y, const SWrect *prc, int blendsrc, int blenddst, int width, int height );
	void *(*pfnSequencePickSentence)( const char *groupName, int pickMethod, int *entryPicked );
	int (*pfnDrawString)( int x, int y, const char *str, int r, int g, int b );
	int (*pfnDrawStringReverse)( int x, int y, const char *str, int r, int g, int b );
	const char *(*LocalPlayerInfo_ValueForKey)( const char* key );
	int (*pfnVGUI2DrawCharacter)( int x, int y, int ch, unsigned int font );
	int (*pfnVGUI2DrawCharacterAdditive)( int x, int y, int ch, int r, int g, int b, unsigned int font );
	unsigned int (*pfnGetApproxWavePlayLen)( char *filename );
	void * (*GetCareerGameUI)( void ); // g-cont. !!!! potential crash-point!
	void (*Cvar_Set)( char *name, char *value );
	int (*pfnIsPlayingCareerMatch)( void );
	void (*pfnPlaySoundVoiceByName)( char *szSound, float volume, int pitch );
	void (*pfnPrimeMusicStream)( char *filename, int looping );
	double (*pfnSys_FloatTime)( void );

	// decay funcs
	void (*pfnProcessTutorMessageDecayBuffer)( int *buffer, int buflen );
	void (*pfnConstructTutorMessageDecayBuffer)( int *buffer, int buflen );
	void (*pfnResetTutorMessageDecayData)( void );

	void (*pfnPlaySoundByNameAtPitch)( char *szSound, float volume, int pitch );
	void (*pfnFillRGBABlend)( int x, int y, int width, int height, int r, int g, int b, int a );
	int (*pfnGetAppID)( void );
	SCmdAlias *(*pfnGetAliases)( void );
	void (*pfnVguiWrap2_GetMouseDelta)( int *x, int *y );
};

struct IUiEngineFuncs
{
	// image handlers
	int (*pfnPIC_Load)( const char *szPicName, const quint8 *ucRawImage, long ulRawImageSize, long flags );
	void (*pfnPIC_Free)( const char *szPicName );
	int (*pfnPIC_Width)( int hPic );
	int (*pfnPIC_Height)( int hPic );
	void (*pfnPIC_Set)( int hPic, int r, int g, int b, int a );
	void (*pfnPIC_Draw)( int x, int y, int width, int height, const SWrect *prc );
	void (*pfnPIC_DrawHoles)( int x, int y, int width, int height, const SWrect *prc );
	void (*pfnPIC_DrawTrans)( int x, int y, int width, int height, const SWrect *prc );
	void (*pfnPIC_DrawAdditive)( int x, int y, int width, int height, const SWrect *prc );
	void (*pfnPIC_EnableScissor)( int x, int y, int width, int height );
	void (*pfnPIC_DisableScissor)( void );

	// screen handlers
	void (*pfnFillRGBA)( int x, int y, int width, int height, int r, int g, int b, int a );

	// cvar handlers
	SCvar * (*pfnRegisterVariable)( const char *szName, const char *szValue, int flags );
	float (*pfnGetCvarFloat)( const char *szName );
	char * (*pfnGetCvarString)( const char *szName );
	void (*pfnCvarSetString)( const char *szName, const char *szValue );
	void (*pfnCvarSetValue)( const char *szName, float flValue );

	// command handlers
	int (*pfnAddCommand)( const char *cmd_name, void (*function)(void) );
	void (*pfnClientCmd)( int execute_now, const char *szCmdString );
	void (*pfnDelCommand)( const char *cmd_name );
	int       (*pfnCmdArgc)( void );
	char * (*pfnCmdArgv)( int argc );
	char * (*pfnCmd_Args)( void );

	// debug messages (in-menu shows only notify)
	void (*Con_Printf)( char *fmt, ... );
	void (*Con_DPrintf)( char *fmt, ... );
	void (*Con_NPrintf)( int pos, char *fmt, ... );
	void (*Con_NXPrintf)(SConNprint *info, char *fmt, ... );

	// sound handlers
	void (*pfnPlayLocalSound)( const char *szSound );

	// cinematic handlers
	void (*pfnDrawLogo)( const char *filename, float x, float y, float width, float height );
	int (*pfnGetLogoWidth)( void );
	int (*pfnGetLogoHeight)( void );
	float (*pfnGetLogoLength)( void ); // cinematic duration in seconds

	// text message system
	void (*pfnDrawCharacter)( int x, int y, int width, int height, int ch, int ulRGBA, int hFont );
	int (*pfnDrawConsoleString)( int x, int y, const char *string );
	void (*pfnDrawSetTextColor)( int r, int g, int b, int alpha );
	void (*pfnDrawConsoleStringLen)(  const char *string, int *length, int *height );
	void (*pfnSetConsoleDefaultColor)( int r, int g, int b ); // color must came from colors.lst

	// custom rendering (for playermodel preview)
	SEntity* (*pfnGetPlayerModel)( void ); // for drawing playermodel previews
	void (*pfnSetModel)(SEntity *ed, const char *path );
	void (*pfnClearScene)( void );
	void (*pfnRenderScene)( const SRefParams *fd );
	int (*CL_CreateVisibleEntity)( int type,SEntity *ent );

	// misc handlers
	void (*pfnHostError)( const char *szFmt, ... );
	int (*pfnFileExists)( const char *filename, int gamedironly );
	void (*pfnGetGameDir)( char *szGetGameDir );

	// gameinfo handlers
	int (*pfnCreateMapsList)( int fRefresh );
	int (*pfnClientInGame)( void );
	void (*pfnClientJoin)( const SNetAdr adr );

	// parse txt files
	quint8 * (*COM_LoadFile)( const char *filename, int *pLength );
	char * (*COM_ParseFile)( char *data, char *token );
	void (*COM_FreeFile)( void *buffer );

	// keyfuncs
	void (*pfnKeyClearStates)( void ); // call when menu open or close
	void (*pfnSetKeyDest)( int dest );
	const char *(*pfnKeynumToString)( int keynum );
	const char *(*pfnKeyGetBinding)( int keynum );
	void (*pfnKeySetBinding)( int keynum, const char *binding );
	int (*pfnKeyIsDown)( int keynum );
	int (*pfnKeyGetOverstrikeMode)( void );
	void (*pfnKeySetOverstrikeMode)( int fActive );
	void *(*pfnKeyGetState)( const char *name ); // for mlook, klook etc

	// engine memory manager
	void * (*pfnMemAlloc)( size_t cb, const char *filename, const int fileline );
	void (*pfnMemFree)( void *mem, const char *filename, const int fileline );

	// collect info from engine
	int (*pfnGetGameInfo)( SModInfo *pgameinfo );
	SModInfo **(*pfnGetGamesList)( int *numGames ); // collect info about all mods
	char 	**(*pfnGetFilesList)( const char *pattern, int *numFiles, int gamedironly ); // find in files
	int 	(*pfnGetSaveComment)( const char *savename, char *comment );
	int (*pfnGetDemoComment)( const char *demoname, char *comment );
	int (*pfnCheckGameDll)( void ); // returns false if hl.dll is missed or invalid
	char *(*pfnGetClipboardData)( void );

	// engine launcher
	void (*pfnShellExecute)( const char *name, const char *args, int closeEngine );
	void (*pfnWriteServerConfig)( const char *name );
	void (*pfnChangeInstance)( const char *newInstance, const char *szFinalMessage );
	void (*pfnPlayBackgroundTrack)( const char *introName, const char *loopName );
	void (*pfnHostEndGame)( const char *szFinalMessage );

	// menu interface is freezed at version 0.75
	// new functions starts here
	float (*pfnRandomFloat)( float flLow, float flHigh );
	long (*pfnRandomLong)( long lLow, long lHigh );

	void (*pfnSetCursor)( void *hCursor ); // change cursor
	int (*pfnIsMapValid)( char *filename );
	void (*pfnProcessImage)( int texnum, float gamma, int topColor, int bottomColor );
	int (*pfnCompareFileTime)( char *filename1, char *filename2, int *iCompare );
};

struct IUiFunctions
{
	int (*pfnVidInit)( void );
	void (*pfnInit)( void );
	void (*pfnShutdown)( void );
	void (*pfnRedraw)( float flTime );
	void (*pfnKeyEvent)( int key, int down );
	void (*pfnMouseMove)( int x, int y );
	void (*pfnSetActiveMenu)( int active );
	void (*pfnAddServerToList)( SNetAdr adr, const char *info );
	void (*pfnGetCursorPos)( int *pos_x, int *pos_y );
	void (*pfnSetCursorPos)( int pos_x, int pos_y );
	void (*pfnShowCursor)( int show );
	void (*pfnCharEvent)( int key );
	int (*pfnMouseInRect)( void ); // mouse entering\leave game window
	int (*pfnIsVisible)( void );
	int (*pfnCreditsActive)( void ); // unused
	void (*pfnFinalCredits)( void ); // show credits + game end
};




struct IEngineStudioApi
{
	// Allocate number*size quint8s and zero it
	void		*( *Mem_Calloc )( int number, size_t size );
	// Check to see if pointer is in the cache
	void		*( *Cache_Check )( SCacheUser *c );
	// Load file into cache ( can be swapped out on demand )
	void		( *LoadCacheFile )( char *path, SCacheUser *cu );
	// Retrieve model pointer for the named model
	SModel	*( *Mod_ForName )( const char *name, int crash_if_missing );
	// Retrieve pointer to studio model data block from a model
	void		*( *Mod_Extradata )( SModel *mod );
	// Retrieve indexed model from client side model precache list
	SModel	*( *GetModelByIndex )( int index );
	// Get entity that is set for rendering
	SClEntity * ( *GetCurrentEntity )( void );
	// Get referenced player_info_t
	SPlayerInfo *( *PlayerInfo )( int index );
	// Get most recently received player state data from network system
	SEntityState *( *GetPlayerState )( int index );
	// Get viewentity
	SClEntity * ( *GetViewEntity )( void );
	// Get current frame count, and last two timestampes on client
	void		( *GetTimes )( int *framecount, double *current, double *old );
	// Get a pointer to a cvar by name
	SCvar	*( *GetCvar )( const char *name );
	// Get current render origin and view vectors ( up, right and vpn )
	void		( *GetViewInfo )( float *origin, float *upv, float *rightv, float *vpnv );
	// Get sprite model used for applying chrome effect
	SModel	*( *GetChromeSprite )( void );
	// Get model counters so we can incement instrumentation
	void		( *GetModelCounters )( int **s, int **a );
	// Get software scaling coefficients
	void		( *GetAliasScale )( float *x, float *y );

	// Get bone, light, alias, and rotation matrices
	float		****( *StudioGetBoneTransform )( void );
	float		****( *StudioGetLightTransform )( void );
	float		***( *StudioGetAliasTransform )( void );
	float		***( *StudioGetRotationMatrix )( void );

	// Set up body part, and get submodel pointers
	void		( *StudioSetupModel )( int bodypart, void **ppbodypart, void **ppsubmodel );
	// Check if entity's bbox is in the view frustum
	int		( *StudioCheckBBox )( void );
	// Apply lighting effects to model
	void		( *StudioDynamicLight )( SClEntity *ent, SAlight *plight );
	void		( *StudioEntityLight )( SAlight *plight );
	void		( *StudioSetupLighting )( SAlight *plighting );

	// Draw mesh vertices
	void		( *StudioDrawPoints )( void );

	// Draw hulls around bones
	void		( *StudioDrawHulls )( void );
	// Draw bbox around studio models
	void		( *StudioDrawAbsBBox )( void );
	// Draws bones
	void		( *StudioDrawBones )( void );
	// Loads in appropriate texture for model
	void		( *StudioSetupSkin )( void *ptexturehdr, int index );
	// Sets up for remapped colors
	void		( *StudioSetRemapColors )( int top, int bottom );
	// Set's player model and returns model pointer
	SModel	*( *SetupPlayerModel )( int index );
	// Fires any events embedded in animation
	void		( *StudioClientEvents )( void );
	// Retrieve/set forced render effects flags
	int		( *GetForceFaceFlags )( void );
	void		( *SetForceFaceFlags )( int flags );
	// Tell engine the value of the studio model header
	void		( *StudioSetHeader )( void *header );
	// Tell engine which model_t * is being renderered
	void		( *SetRenderModel )( SModel *model );

	// Final state setup and restore for rendering
	void		( *SetupRenderer )( int rendermode );
	void		( *RestoreRenderer )( void );

	// Set render origin for applying chrome effect
	void		( *SetChromeOrigin )( void );

	// True if using D3D/OpenGL
	int		( *IsHardware )( void );

	// Only called by hardware interface
	void		( *GL_StudioDrawShadow )( void );
	void		( *GL_SetRenderMode )( int mode );

	void		( *StudioSetRenderamt )( int iRenderamt );
	void		( *StudioSetCullState )( int iCull );
	void		( *StudioRenderShadow )( int iSprite, float *p1, float *p2, float *p3, float *p4 );
};

struct IServerStudioApi
{
	// Allocate number*size quint8s and zero it
	void		*( *Mem_Calloc )( int number, size_t size );
	// Check to see if pointer is in the cache
	void		*( *Cache_Check )( SCacheUser *c );
	// Load file into cache ( can be swapped out on demand )
	void		( *LoadCacheFile )( char *path, SCacheUser *cu );
	// Retrieve pointer to studio model data block from a model
	void		*( *Mod_Extradata )( SModel *mod );
};

// client blending
struct IStudioInterface
{
	int		version;
	int		( *StudioDrawModel	)( int flags );
	int		( *StudioDrawPlayer	)( int flags, SEntityState *pplayer );
};

struct ISvBlendingInterface
{
	int	version;

	void	( *SV_StudioSetupBones )( SModel *pModel,
					float frame,
					int sequence,
					const float angles[3],
					const float origin[3],
					const quint8 *pcontroller,
					const quint8 *pblending,
					int iBone,
					const SEdict *pEdict );
};

#endif // XASH_TYPES_HPP

