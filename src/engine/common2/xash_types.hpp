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

#include <QtGlobal>
#include <QByteArray>
#include <QString>
#include <QStringList>


// ---------------------------------------- //
// ---------------- DEFINES --------------- //
// ---------------------------------------- //

#define MAX_STRING               256 // generic string
#define MAX_INFO_STRING          256 // infostrings are transmitted across network
#define MAX_SYSPATH              1024 // system filepath
#define MAX_MODS                 512 // environment games that engine can keep visible
#define BIT( n )                 (1<<( n ))
#define IsColorString( p )	     ( p && *( p ) == '^' && *(( p ) + 1) && *(( p ) + 1) >= '0' && *(( p ) + 1 ) <= '9' )
#define ColorIndex( c )	         ((( c ) - '0' ) & 7 )
#define HISTORY_MAX              64 // Must be power of 2
#define HISTORY_MASK             ( HISTORY_MAX - 1 )
#define ENTITY_NORMAL            (1<<0)
#define ENTITY_BEAM              (1<<1)
#define MAX_PHYSINFO_STRING      256
#define FEVENT_ORIGIN            ( 1<<0 )
#define FEVENT_ANGLES            ( 1<<1 )
#define Q1BSP_VERSION            29 // quake1 regular version (beta is 28)
#define HLBSP_VERSION            30 // half-life regular version
#define XTBSP_VERSION            31 // extended lightmaps and expanded clipnodes limit
#define DELUXEMAP_VERSION        1
#define IDDELUXEMAPHEADER        (('T'<<24)+('I'<<16)+('L'<<8)+'Q') // little-endian "QLIT"
#define ANGLE_UP                 -1
#define ANGLE_DOWN               -2
#define MAX_MAP_HULLS            4 // MAX_HULLS
#define SURF_NOCULL              BIT( 0 ) // two-sided polygon (e.g. 'water4b')
#define SURF_PLANEBACK           BIT( 1 ) // plane should be negated
#define SURF_DRAWSKY             BIT( 2 ) // sky surface
#define SURF_WATERCSG            BIT( 3 ) // culled by csg (was SURF_DRAWSPRITE)
#define SURF_DRAWTURB            BIT( 4 ) // warp surface
#define SURF_DRAWTILED           BIT( 5 ) // face without lighmap
#define SURF_CONVEYOR            BIT( 6 ) // scrolled texture (was SURF_DRAWBACKGROUND)
#define SURF_UNDERWATER          BIT( 7 ) // caustics
#define SURF_TRANSPARENT         BIT( 8 ) // it's a transparent texture (was SURF_DONTWARP)
#define SURF_REFLECT             BIT( 31 ) // reflect surface (mirror)
#define LM_STYLES                4 // MAXLIGHTMAPS
#define LS_NORMAL                0x00
#define LS_UNUSED                0xFE
#define LS_NONE                  0xFF
#define MAX_MAP_MODELS           1024 // can be increased up to 2048 if needed
#define MAX_MAP_BRUSHES          32768 // quint16 limit
#define MAX_MAP_ENTITIES         8192 // can be increased up to 32768 if needed
#define MAX_MAP_ENTSTRING        0x80000 // 512 kB should be enough
#define MAX_MAP_PLANES           65536 // can be increased without problems
#define MAX_MAP_NODES            32767 // because negative shorts are leafs
#define MAX_MAP_CLIPNODES        32767 // because negative shorts are contents
#define MAX_MAP_LEAFS            32767 // signed short limit
#define MAX_MAP_VERTS            65535 // quint16 limit
#define MAX_MAP_FACES            65535 // quint16 limit
#define MAX_MAP_MARKSURFACES     65535 // quint16 limit
#define MAX_MAP_TEXINFO          MAX_MAP_FACES // in theory each face may have personal texinfo
#define MAX_MAP_EDGES            0x100000 // can be increased but not needed
#define MAX_MAP_SURFEDGES        0x200000 // can be increased but not needed
#define MAX_MAP_TEXTURES         2048 // can be increased but not needed
#define MAX_MAP_MIPTEX           0x2000000 // 32 Mb internal textures data
#define MAX_MAP_LIGHTING         0x2000000 // 32 Mb lightmap raw data (can contain deluxemaps)
#define MAX_MAP_VISIBILITY       0x800000 // 8 Mb visdata
#define LUMP_ENTITIES            0
#define LUMP_PLANES              1
#define LUMP_TEXTURES            2 // internal textures
#define LUMP_VERTEXES            3
#define LUMP_VISIBILITY          4
#define LUMP_NODES               5
#define LUMP_TEXINFO             6
#define LUMP_FACES               7
#define LUMP_LIGHTING            8
#define LUMP_CLIPNODES           9
#define LUMP_LEAFS               10
#define LUMP_MARKSURFACES        11
#define LUMP_EDGES               12
#define LUMP_SURFEDGES           13
#define LUMP_MODELS              14 // internal submodels
#define HEADER_LUMPS             15
#define LUMP_CLIPNODES2          15 // hull0 goes into LUMP_NODES, hull1 goes into LUMP_CLIPNODES,
#define LUMP_CLIPNODES3          16 // hull2 goes into LUMP_CLIPNODES2, hull3 goes into LUMP_CLIPNODES3
#define TEX_SPECIAL              BIT( 0 ) // sky or slime, no lightmap or 256 subdivision
#define STUDIO_RENDER            1
#define STUDIO_EVENTS            2
#define ZISCALE                  (static_cast<float>(0x8000))
#define MIPLEVELS                4
#define VERTEXSIZE               7
#define MAXLIGHTMAPS             4
//#define NUM_AMBIENTS             4 // automatic ambient sounds
#define MAX_SCOREBOARDNAME       32
#define MAX_INFO_STRING          256
#define XASH_VERSION             0.96f // engine current version
#define MIN_FPS                  15.0f // host minimum fps value for maxfps.
#define MAX_FPS                  500.0f // upper limit for maxfps.
#define MAX_FRAMETIME            0.1f
#define MIN_FRAMETIME            0.001f
#define MAX_CMD_TOKENS           80 // cmd tokens
#define MAX_ENTNUMBER            99999 // for server and client parsing
#define MAX_HEARTBEAT            -99999 // connection time
#define QCHAR_WIDTH              16 // font width
#define CIN_MAIN                 0
#define CIN_LOGO                 1
#define MAX_NUM_ARGVS            128
#define CS_SIZE                  64 // size of one config string
#define CS_TIME                  16 // size of time string
#define MAX_DECALS               512 // touching TE_DECAL messages, etc
#define MAX_STATIC_ENTITIES      512 // static entities that moved on the client when level is spawn
#define FS_STATIC_PATH           1 // FS_ClearSearchPath will be ignore this path
#define FS_NOWRITE_PATH          2 // default behavior - last added gamedir set as writedir. This flag disables it
#define FS_GAMEDIR_PATH          4 // just a marker for gamedir path
#define FL_FLY                   (1<<0)	// Changes the SV_Movestep() behavior to not need to be on ground
#define FL_SWIM                  (1<<1)	// Changes the SV_Movestep() behavior to not need to be on ground (but stay in water)
#define FL_CONVEYOR              (1<<2)
#define FL_CLIENT                (1<<3)
#define FL_INWATER               (1<<4)
#define FL_MONSTER               (1<<5)
#define FL_GODMODE               (1<<6)
#define FL_NOTARGET              (1<<7)
#define FL_SKIPLOCALHOST         (1<<8)	// Don't send entity to local host, it's predicting this entity itself
#define FL_ONGROUND              (1<<9)	// At rest / on the ground
#define FL_PARTIALGROUND         (1<<10)	// not all corners are valid
#define FL_WATERJUMP             (1<<11)	// player jumping out of water
#define FL_FROZEN                (1<<12)	// Player is frozen for 3rd person camera
#define FL_FAKECLIENT            (1<<13)	// JAC: fake client, simulated server side; don't send network messages to them
#define FL_DUCKING               (1<<14)	// Player flag -- Player is fully crouched
#define FL_FLOAT                 (1<<15)	// Apply floating force to this entity when in water
#define FL_GRAPHED               (1<<16)	// worldgraph has this ent listed as something that blocks a connection
#define FL_IMMUNE_WATER          (1<<17)
#define FL_IMMUNE_SLIME          (1<<18)
#define FL_IMMUNE_LAVA           (1<<19)
#define FL_PROXY                 (1<<20)	// This is a spectator proxy
#define FL_ALWAYSTHINK           (1<<21)	// Brush model flag -- call think every frame regardless of nextthink - ltime (for constantly changing velocity/path)
#define FL_BASEVELOCITY          (1<<22)	// Base velocity has been applied this frame (used to convert base velocity into momentum)
#define FL_MONSTERCLIP           (1<<23)	// Only collide in with monsters who have FL_MONSTERCLIP set
#define FL_ONTRAIN               (1<<24)	// Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_WORLDBRUSH            (1<<25)	// Not moveable/removeable brush entity (really part of the world, but represented as an entity for transparency or something)
#define FL_SPECTATOR             (1<<26)	// This client is a spectator, don't run touch functions, etc.
#define FL_CUSTOMENTITY          (1<<29)	// This is a custom entity
#define FL_KILLME                (1<<30)	// This entity is marked for death -- This allows the engine to kill ents at the appropriate time
#define FL_DORMANT               (1<<31)	// Entity is dormant, no updates to client
#define FTRACE_SIMPLEBOX         (1<<0)	// Traceline with a simple box
#define FTRACE_IGNORE_GLASS      (1<<1)	// traceline will be ignored entities with rendermode != kRenderNormal
#define WALKMOVE_NORMAL          0 // normal walkmove
#define WALKMOVE_WORLDONLY       1 // doesn't hit ANY entities, no matter what the solid type
#define WALKMOVE_CHECKONLY       2 // move, but don't touch triggers
#define MOVETYPE_NONE            0 // never moves
#define MOVETYPE_ANGLENOCLIP     1
#define MOVETYPE_ANGLECLIP       2
#define MOVETYPE_WALK            3 // Player only - moving on the ground
#define MOVETYPE_STEP            4 // gravity, special edge handling -- monsters use this
#define MOVETYPE_FLY             5 // No gravity, but still collides with stuff
#define MOVETYPE_TOSS            6 // gravity/collisions
#define MOVETYPE_PUSH            7 // no clip to world, push and crush
#define MOVETYPE_NOCLIP          8 // No gravity, no collisions, still do velocity/avelocity
#define MOVETYPE_FLYMISSILE      9 // extra size to monsters
#define MOVETYPE_BOUNCE          10 // Just like Toss, but reflect velocity when contacting surfaces
#define MOVETYPE_BOUNCEMISSIL    E 11 // bounce w/o gravity
#define MOVETYPE_FOLLOW          12 // track movement of aiment
#define MOVETYPE_PUSHSTEP        13 // BSP model that needs physics/world collisions (uses nearest hull for world collision)
#define MOVETYPE_COMPOUND        14 // glue two entities together (simple movewith)
#define SOLID_NOT                0 // no interaction with other objects
#define SOLID_TRIGGER            1 // touch on edge, but not blocking
#define SOLID_BBOX               2 // touch on edge, block
#define SOLID_SLIDEBOX           3 // touch on edge, but not an onground
#define SOLID_BSP                4 // bsp clip, touch on edge, block
#define SOLID_CUSTOM             5 // call external callbacks for tracing
#define DEAD_NO                  0 // alive
#define DEAD_DYING               1 // playing death animation or still falling off of a ledge waiting to hit ground
#define DEAD_DEAD                2 // dead. lying still.
#define DEAD_RESPAWNABLE         3
#define DEAD_DISCARDBODY         4
#define DAMAGE_NO                0
#define DAMAGE_YES               1
#define DAMAGE_AIM               2
#define EF_BRIGHTFIELD           1 // swirling cloud of particles
#define EF_MUZZLEFLASH           2 // single frame ELIGHT on entity attachment 0
#define EF_BRIGHTLIGHT           4 // DLIGHT centered at entity origin
#define EF_DIMLIGHT              8 // player flashlight
#define EF_INVLIGHT              16 // get lighting from ceiling
#define EF_NOINTERP              32 // don't interpolate the next frame
#define EF_LIGHT                 64 // rocket flare glow sprite
#define EF_NODRAW                128 // don't draw entity
#define EF_NOREFLECT             (1<<24)	// Entity won't reflecting in mirrors
#define EF_REFLECTONLY           (1<<25)	// Entity will be drawing only in mirrors
#define EF_NOWATERCSG            (1<<26)	// Do not remove sides for func_water entity
#define EF_FULLBRIGHT            (1<<27)	// Just get fullbright
#define EF_NOSHADOW              (1<<28)	// ignore shadow for this entity
#define EF_MERGE_VISIBILITY      (1<<29)	// this entity allowed to merge vis (e.g. env_sky or portal camera)
#define EF_REQUEST_PHS           (1<<30)	// This entity requested phs bitvector instead of pvsbitvector in AddToFullPack calls
#define EFLAG_SLERP              1 // do studio interpolation of this entity
#define TE_BEAMPOINTS            0 // beam effect between two points
#define TE_BEAMENTPOINT          1 // beam effect between point and entity
#define TE_GUNSHOT               2 // particle effect plus ricochet sound
#define TE_EXPLOSION             3 // additive sprite, 2 dynamic lights, flickering particles, explosion sound, move vertically 8 pps
#define TE_EXPLFLAG_NONE         0 // all flags clear makes default Half-Life explosion
#define TE_EXPLFLAG_NOADDITIVE   1 // sprite will be drawn opaque (ensure that the sprite you send is a non-additive sprite)
#define TE_EXPLFLAG_NODLIGHTS    2 // do not render dynamic lights
#define TE_EXPLFLAG_NOSOUND      4 // do not play client explosion sound
#define TE_EXPLFLAG_NOPARTICLES  8 // do not draw particles
#define TE_EXPLFLAG_DRAWALPHA    16 // sprite will be drawn alpha
#define TE_EXPLFLAG_ROTATE       32 // rotate the sprite randomly
#define TE_TAREXPLOSION          4 // Quake1 "tarbaby" explosion with sound
#define TE_SMOKE                 5 // alphablend sprite, move vertically 30 pps
#define TE_TRACER                6 // tracer effect from point to point
#define TE_LIGHTNING             7 // TE_BEAMPOINTS with simplified parameters
#define TE_BEAMENTS              8
#define TE_SPARKS                9 // 8 random tracers with gravity, ricochet sprite
#define TE_LAVASPLASH            10 // Quake1 lava splash
#define TE_TELEPORT              11 // Quake1 teleport splash
#define TE_EXPLOSION2            12 // Quake1 colormaped (base palette) particle explosion with sound
#define TE_BSPDECAL              13 // Decal from the .BSP file
#define TE_IMPLOSION             14 // tracers moving toward a point
#define TE_SPRITETRAIL           15 // line of moving glow sprites with gravity, fadeout, and collisions
#define TE_BEAM                  16 // obsolete
#define TE_SPRITE                17 // additive sprite, plays 1 cycle
#define TE_BEAMSPRITE            18 // A beam with a sprite at the end
#define TE_BEAMTORUS             19 // screen aligned beam ring, expands to max radius over lifetime
#define TE_BEAMDISK              20 // disk that expands to max radius over lifetime
#define TE_BEAMCYLINDER          21 // cylinder that expands to max radius over lifetime
#define TE_BEAMFOLLOW            22 // create a line of decaying beam segments until entity stops moving
#define TE_GLOWSPRITE            23
#define TE_BEAMRING              24 // connect a beam ring to two entities
#define TE_STREAK_SPLASH         25 // oriented shower of tracers
#define TE_BEAMHOSE              26 // obsolete
#define TE_DLIGHT                27 // dynamic light, effect world, minor entity effect
#define TE_ELIGHT                28 // point entity light, no world effect
#define TE_TEXTMESSAGE           29
#define TE_LINE                  30
#define TE_BOX                   31
#define TE_KILLBEAM              99 // kill all beams attached to entity
#define TE_LARGEFUNNEL           100
#define TE_BLOODSTREAM           101 // particle spray
#define TE_SHOWLINE              102 // line of particles every 5 units, dies in 30 seconds
#define TE_BLOOD                 103 // particle spray
#define TE_DECAL                 104 // Decal applied to a brush entity (not the world)
#define TE_FIZZ                  105 // create alpha sprites inside of entity, float upwards
#define TE_MODEL                 106 // create a moving model that bounces and makes a sound when it hits
#define TE_EXPLODEMODEL          107 // spherical shower of models, picks from set
#define TE_BREAKMODEL            108 // box of models or sprites
#define TE_GUNSHOTDECAL          109 // decal and ricochet sound
#define TE_SPRITE_SPRAY          110 // spay of alpha sprites
#define TE_ARMOR_RICOCHET        111 // quick spark sprite, client ricochet sound.
#define TE_PLAYERDECAL           112 // ???
#define TE_BUBBLES               113 // create alpha sprites inside of box, float upwards
#define TE_BUBBLETRAIL           114 // create alpha sprites along a line, float upwards
#define TE_BLOODSPRITE           115 // spray of opaque sprite1's that fall, single sprite2 for 1..2 secs (this is a high-priority tent)
#define TE_WORLDDECAL            116 // Decal applied to the world brush
#define TE_WORLDDECALHIGH        117 // Decal (with texture index > 256) applied to world brush
#define TE_DECALHIGH             118 // Same as TE_DECAL, but the texture index was greater than 256
#define TE_PROJECTILE            119 // Makes a projectile (like a nail) (this is a high-priority tent)
#define TE_SPRAY                 120 // Throws a shower of sprites or models
#define TE_PLAYERSPRITES         121 // sprites emit from a player's bounding box (ONLY use for players!)
#define TE_PARTICLEBURST         122 // very similar to lavasplash.
#define TE_FIREFIELD             123 // makes a field of fire.
#define TEFIRE_FLAG_ALLFLOAT     1 // all sprites will drift upwards as they animate
#define TEFIRE_FLAG_SOMEFLOAT    2 // some of the sprites will drift upwards. (50% chance)
#define TEFIRE_FLAG_LOOP         4 // if set, sprite plays at 15 fps, otherwise plays at whatever rate stretches the animation over the sprite's duration.
#define TEFIRE_FLAG_ALPHA        8 // if set, sprite is rendered alpha blended at 50% else, opaque
#define TEFIRE_FLAG_PLANAR       16 // if set, all fire sprites have same initial Z instead of randomly filling a cube.
#define TE_PLAYERATTACHMENT      124 // attaches a TENT to a player (this is a high-priority tent)
#define TE_KILLPLAYERATTACHMENTS 125 // will expire all TENTS attached to a player.
#define TE_MULTIGUNSHOT          126 // much more compact shotgun message
#define TE_USERTRACER            127 // larger message than the standard tracer, but allows some customization.
#define MSG_BROADCAST            0 // unreliable to all
#define MSG_ONE                  1 // reliable to one (msg_entity)
#define MSG_ALL                  2 // reliable to all
#define MSG_INIT                 3 // write to the init string
#define MSG_PVS                  4 // Ents in PVS of org
#define MSG_PAS                  5 // Ents in PAS of org
#define MSG_PVS_R                6 // Reliable to PVS
#define MSG_PAS_R                7 // Reliable to PAS
#define MSG_ONE_UNRELIABLE       8 // Send to one client, but don't put in reliable stream, put in unreliable datagram ( could be dropped )
#define MSG_SPEC                 9 // Sends to all spectator proxies
#define CONTENTS_EMPTY           -1
#define CONTENTS_SOLID           -2
#define CONTENTS_WATER           -3
#define CONTENTS_SLIME           -4
#define CONTENTS_LAVA            -5
#define CONTENTS_SKY             -6
#define CONTENTS_ORIGIN          -7 // removed at csg time
#define CONTENTS_CLIP            -8 // changed to contents_solid
#define CONTENTS_CURRENT_0       -9
#define CONTENTS_CURRENT_90      -10
#define CONTENTS_CURRENT_180     -11
#define CONTENTS_CURRENT_270     -12
#define CONTENTS_CURRENT_UP      -13
#define CONTENTS_CURRENT_DOWN    -14
#define CONTENTS_TRANSLUCENT     -15
#define CONTENTS_LADDER          -16
#define CONTENT_FLYFIELD         -17
#define CONTENT_GRAVITY_FLYFIELD -18
#define CONTENT_FOG              -19
#define CONTENT_EMPTY            -1
#define CONTENT_SOLID            -2
#define CONTENT_WATER            -3
#define CONTENT_SLIME            -4
#define CONTENT_LAVA             -5
#define CONTENT_SKY              -6
#define CHAN_AUTO                0
#define CHAN_WEAPON              1
#define CHAN_VOICE               2
#define CHAN_ITEM                3
#define CHAN_BODY                4
#define CHAN_STREAM              5 // allocate stream channel from the static or dynamic area
#define CHAN_STATIC              6 // allocate channel from the static area
#define CHAN_NETWORKVOICE_BASE   7 // voice data coming across the network
#define CHAN_NETWORKVOICE_END    500 // network voice data reserves slots (CHAN_NETWORKVOICE_BASE through CHAN_NETWORKVOICE_END).
#define ATTN_NONE                0
#define ATTN_NORM                0.8f
#define ATTN_IDLE                2.f
#define ATTN_STATIC              1.25f
#define PITCH_NORM               100 // non-pitch shifted
#define PITCH_LOW                95 // other values are possible - 0-255, where 255 is very high
#define PITCH_HIGH               120
#define VOL_NORM                 1.0f
#define PLAT_LOW_TRIGGER         1
#define SF_TRAIN_WAIT_RETRIGGER  1
#define SF_TRAIN_START_ON        4 // Train is initially moving
#define SF_TRAIN_PASSABLE        8 // Train is not solid -- used to make water trains
#define IN_ATTACK                (1<<0)
#define IN_JUMP                  (1<<1)
#define IN_DUCK                  (1<<2)
#define IN_FORWARD               (1<<3)
#define IN_BACK                  (1<<4)
#define IN_USE                   (1<<5)
#define IN_CANCEL                (1<<6)
#define IN_LEFT                  (1<<7)
#define IN_RIGHT                 (1<<8)
#define IN_MOVELEFT              (1<<9)
#define IN_MOVERIGHT             (1<<10)
#define IN_ATTACK2               (1<<11)
#define IN_RUN                   (1<<12)
#define IN_RELOAD                (1<<13)
#define IN_ALT1                  (1<<14)
#define IN_SCORE                 (1<<15)   // Used by client.dll for when scoreboard is held down
#define BREAK_TYPEMASK           0x4F
#define BREAK_GLASS              0x01
#define BREAK_METAL              0x02
#define BREAK_FLESH              0x04
#define BREAK_WOOD               0x08
#define BREAK_SMOKE              0x10
#define BREAK_TRANS              0x20
#define BREAK_CONCRETE           0x40
#define BREAK_2                  0x80
#define BOUNCE_GLASS             BREAK_GLASS
#define BOUNCE_METAL             BREAK_METAL
#define BOUNCE_FLESH             BREAK_FLESH
#define BOUNCE_WOOD              BREAK_WOOD
#define BOUNCE_SHRAP             0x10
#define BOUNCE_SHELL             0x20
#define BOUNCE_CONCRETE          BREAK_CONCRETE
#define BOUNCE_SHOTSHELL         0x80
#define TE_BOUNCE_NULL           0
#define TE_BOUNCE_SHELL          1
#define TE_BOUNCE_SHOTSHELL      2
#define MAX_ENT_LEAFS            48
#define RES_FATALIFMISSING       (1<<0)	// Disconnect if we can't get this file.
#define RES_WASMISSING           (1<<1)	// Do we have the file locally, did we get it ok?
#define RES_CUSTOM               (1<<2)	// Is this resource one that corresponds to another player's customization
#define RES_REQUESTED            (1<<3)	// Already requested a download of this one
#define RES_PRECACHED            (1<<4)	// Already precached
#define FCUST_FROMHPAK           ( 1<<0 )
#define FCUST_WIPEDATA           ( 1<<1 )
#define FCUST_IGNOREINIT         ( 1<<2 )
#define IDPACKV1HEADER           (('K'<<24)+('C'<<16)+('A'<<8)+'P')	// little-endian "PACK"
#define MAX_FILES_IN_PACK        65536 // pak
#define IDWAD2HEADER             (('2'<<24)+('D'<<16)+('A'<<8)+'W')	// little-endian "WAD2" quake1 gfx.wad
#define IDWAD3HEADER             (('3'<<24)+('D'<<16)+('A'<<8)+'W')	// little-endian "WAD3" half-life wads
#define WAD3_NAMELEN             16
#define MAX_FILES_IN_WAD         8192
#define TYP_ANY                  -1 // any type can be accepted
#define TYP_NONE                 0 // unknown lump type
#define IDCUSTOMHEADER           (('K'<<24)+('A'<<16)+('P'<<8)+'H') // little-endian "HPAK"
#define IDCUSTOM_VERSION         1
#define IDWAD3HEADER             (('3'<<24)+('D'<<16)+('A'<<8)+'W')
#define CMP_NONE                 0 // compression none
#define CMP_LZSS                 1 // LZSS compression
#define TYP_QPAL                 64 // quake palette
#define TYP_QTEX                 65 // probably was never used
#define TYP_QPIC                 66 // quake1 and hl pic (lmp_t)
#define TYP_MIPTEX               67 // half-life (mip_t) previous was TYP_SOUND but never used in quake1
#define TYP_QMIP                 68 // quake1 (mip_t) (replaced with TYPE_MIPTEX while loading)
#define TYP_RAW                  69 // raw data
#define TYP_QFONT                70 // half-life font (qfont_t)
#define FILE_BUFF_SIZE           2048
#define PAK_LOAD_OK              0
#define PAK_LOAD_COULDNT_OPEN    1
#define PAK_LOAD_BAD_HEADER      2
#define PAK_LOAD_BAD_FOLDERS     3
#define PAK_LOAD_TOO_MANY_FILES  4
#define PAK_LOAD_NO_FILES        5
#define PAK_LOAD_CORRUPTED       6
#define DOS_SIGNATURE            0x5A4D // MZ
#define NT_SIGNATURE             0x00004550 // PE00
#define NUMBER_OF_DIRECTORY_ENTRIES 16
#define MAX_LIBRARY_EXPORTS      4096

typedef void (*setpair_t)( const char *key, const char *value, void *buffer, void *numpairs );
typedef void (*xcommand_t)( void );


// ---------------------------------------- //
// ---------------- FORWARD --------------- //
// ---------------------------------------- //

struct SEfrag;
struct SMouth;
struct SLatchedVars;
struct SPositionHistory;
struct SEntityState;
struct SClientData;
struct SWeaponData;
struct SLocalState;
struct SEventArgs;
struct SClEntity;
struct SSearch;
struct SMoveVars;
struct SUserCmd;
struct SRefParams;
struct SRefOverview;
struct SDlump;
struct SDheader;
struct SDmodel;
struct SDmiptexLump;
struct SDvertex;
struct SDplane;
struct SDnode;
struct SDleaf;
struct SDclipNode;
struct SDtexInfo;
struct SDedge;
struct SDface;
struct SMplane;
struct SMvertex;
struct SMedge;
struct STexture;
struct SMtexInfo;
struct SGlvert;
struct SGlpoly;
struct SMnode;
struct SMsurface;
struct SDecal;
struct SMleaf;
struct SMsurfMesh;
struct SMextraSurf;
struct SHull;
struct SCacheUser;
struct SMmodel;
struct SAlight;
struct SAuxvert;
struct SPlayerInfo;
struct SMspriteFrame;
struct SMspriteGroup;
struct SMspriteFrameDesc;
struct SMsprite;
struct SGameInfo;
struct SSysInfo;
struct SMd5Context;
struct SNetadr;
struct SHostRedirect;
struct SSoundItem;
struct SHostParams;
struct SBpcDesc;
struct SRgbData;
struct SImgFilter_s;
struct SWavData;
struct SConVar;
struct SModelState;
struct SDecalItem;
struct SNetadr;
struct SHostRedirect;
struct SDllFunc;
struct SDllInfo;
struct SField;
struct SNotify;
struct SConsole;
struct SConNprint;
struct SCmd;
struct SCmdBuf;
struct SCmdAlias;
struct SCvar;
struct SColor24;
struct SColorVec;
struct SLink;
struct SPlane;
struct entvars_s;
struct SEdict;
struct STrace;
struct SResourceInfo;
struct SResource;
struct SCustomization;
struct SPackHeader;
struct SPackFile;
struct SDwadInfo;
struct SDlumpInfo;
struct SHpak;
struct SHpakHeader;
struct SHpakDir;
struct SHpakContainer;
struct SLmp;
struct SMip;
struct SWadType;
struct SFile;
struct SWfile;
struct SPackfile;
struct SPack;
struct SSearchPath;


// ------------ ENUMS ------------ //
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

enum ENetadrType
{
	NA_UNUSED,
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
	NA_IPX,
	NA_BROADCAST_IPX
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
	HS_Init = 0,	// initalize operations
	HS_Frame,	// host running
	HS_Shutdown,	// shutdown operations
	HS_Fatal,	// sys error
	HS_Sleep,	// sleeped by different reason, e.g. minimize window
	HS_Nofocus,	// same as HOST_FRAME, but disable mouse
	HS_Restart,	// during the changes video mode
	HS_Crashed // an exception handler called
};

enum EInstanceType
{
	HT_Normal,	// listen server, singleplayer
	HT_Dedicated,
};
enum ECvarFlags
{
	CF_Archive = BIT(0), // set to cause it to be saved to vars.rc
	CF_UserInfo = BIT(1), // changes the client's info string
	CF_Server = BIT(2), // notifies players when changed
	CF_ExtDll = BIT(3), // defined by external DLL
	CF_ClientDll = BIT(4), // defined by the client dll
	CF_Protected = BIT(5), // It's a server cvar, but we don't send the data since it's a passquint16, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
	CF_SpOnly = BIT(6), // This cvar cannot be changed by clients connected to a multiplayer server.
	CF_PrintableOnly = BIT(7), // This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
	CF_Unlogged = BIT(8), // If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
	CF_ServerInfo = BIT(9),	// added to serverinfo when changed
	CF_PhysicInfo = BIT(10),// added to physinfo when changed
	CF_RenderInfo = BIT(11),// save to a seperate config called opengl.cfg
	CF_Cheat = BIT(12),// can not be changed if cheats are disabled
	CF_Init = BIT(13),// don't allow change from console at all, but can be set from the command line
	CF_Latch = BIT(14),// save changes until server restart
	CF_ReadOnly = BIT(15),// display only, cannot be set by user at all
	CF_LatchVideo = BIT(16),// save changes until render restart
	CF_UserCreated = BIT(17),// created by a set command (dll's used)
	CF_GlConfig = BIT(18),// set to cause it to be saved to opengl.cfg
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


// ---------------------------------------- //
// ------------- BASE STRUCTS ------------- //
// ---------------------------------------- //

struct SDosHeader
{
	// dos .exe header
	quint16 e_magic; // magic number
	quint16 e_cblp; // quint8s on last page of file
	quint16 e_cp; // pages in file
	quint16 e_crlc; // relocations
	quint16 e_cparhdr;	// size of header in paragraphs
	quint16 e_minalloc;	// minimum extra paragraphs needed
	quint16 e_maxalloc;	// maximum extra paragraphs needed
	quint16 e_ss; // initial (relative) SS value
	quint16 e_sp; // initial SP value
	quint16 e_csum; // checksum
	quint16 e_ip; // initial IP value
	quint16 e_cs; // initial (relative) CS value
	quint16 e_lfarlc; // file address of relocation table
	quint16 e_ovno; // overlay number
	quint16 e_res[4]; // reserved quint16s
	quint16 e_oemid; // OEM identifier (for e_oeminfo)
	quint16 e_oeminfo;	// OEM information; e_oemid specific
	quint16 e_res2[10];	// reserved quint16s
	long e_lfanew; // file address of new exe header
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
	quint8 Name[8];	// dos name length

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
	quint32 AddressOfNameOrdinals;	// RVA from base of image
};

struct SDllUser
{
	void *hInstance; // to avoid possible hacks
	bool custom_loader; // a bit who indicated loader type
	bool encrypted; // dll is crypted (some client.dll in HL, CS etc)
	char dllName[32]; // for debug messages
	QString fullPath, shortPath;	// actual dll paths

	// ordinals stuff
	quint16 *ordinals;
	quint32 *funcs;
	char *names[MAX_LIBRARY_EXPORTS];	// max 4096 exports supported
	int num_ordinals; // actual exports count
	quint32 funcBase; 	// base offset
};

struct SMouth
{
	quint8 mouthopen;	// 0 = mouth closed, 255 = mouth agape
	quint8 sndcount; // counter for running average
	int sndavg; // running average
};

struct SLatchedVars
{
	float prevanimtime;
	float sequencetime;
	quint8 prevseqblending[2];
	float prevorigin[3];
	float prevangles[3];

	int prevsequence;
	float prevframe;

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
	int m_iId;
	int m_iClip;

	float m_flNextPrimaryAttack;
	float m_flNextSecondaryAttack;
	float m_flTimeWeaponIdle;

	int m_fInReload;
	int m_fInSpecialReload;
	float m_flNextReload;
	float m_flPumpTime;
	float m_fReloadTime;

	float m_fAimedDamage;
	float m_fNextAimBonus;
	int m_fInZoom;
	int m_iWeaponState;

	int iuser1;
	int iuser2;
	int iuser3;
	int iuser4;
	float fuser1;
	float fuser2;
	float fuser3;
	float fuser4;
};

struct SEventArgs
{
	int flags;

	// Transmitted
	int entindex;

	float origin[3];
	float angles[3];
	float velocity[3];

	int ducking;

	float fparam1;
	float fparam2;

	int iparam1;
	int iparam2;

	int bparam1;
	int bparam2;
};


struct SSearch
{
	int numfilenames;
	char **filenames;
	char *filenamesbuffer;
};

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
	bool footsteps;        // Play footstep sounds
	char skyName[32];	   // Name of the sky map
	float rollangle;
	float rollspeed;
	float skycolor_r; // Sky color
	float skycolor_g; //
	float skycolor_b; //
	float skyvec_x; // Sky vector
	float skyvec_y; //
	float skyvec_z; //
	int features; // engine features that shared across network
	int fog_settings;	// Global fog settings (packed color+density)
	float wateralpha;	// World water alpha 1.0 - solid 0.0 - transparent
	float skydir_x; // skybox rotate direction
	float skydir_y; //
	float skydir_z; //
	float skyangle; // skybox rotate angle
};

struct SUserCmd
{
	short lerp_msec;	// Interpolation time on client
	quint8 msec; // Duration in ms of command
	float viewangles[3];	// Command view angles

	// intended velocities
	float forwardmove;	// Forward velocity
	float sidemove; // Sideways velocity
	float upmove; // Upward velocity
	quint8 lightlevel;	// Light level at spot where we are standing.
	quint16 buttons; // Attack and move buttons
	quint8 impulse; // Impulse command issued
	quint8 weaponselect;	// Current weapon id

	// Experimental player impact stuff.
	int impact_index;
	float impact_position[3];
};

// same as ref_params but for overview mode
struct SRefOverview
{
	float origin[3];
	bool rotated;

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
	int headnode[MAX_MAP_HULLS];
	int visleafs; // not including the solid leaf 0
	int firstface;
	int numfaces;
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
	int type; // PLANE_X - PLANE_ANYZ ?
};

struct SDnode
{
	int planenum;
	short children[2]; // negative numbers are -(leafs + 1), not nodes
	short mins[3]; // for sphere culling
	short maxs[3];
	quint16 firstface;
	quint16 numfaces; // counting both sides
};

// leaf 0 is the generic CONTENTS_SOLID leaf, used for all solid areas
// all other leafs need visibility info
struct SDleaf
{
	int contents;
	int visofs; // -1 = no visibility info

	short mins[3]; // for frustum culling
	short maxs[3];
	quint16 firstmarksurface;
	quint16 nummarksurfaces;

	// automatic ambient sounds
	quint8 ambient_level[NUM_AMBIENTS];	// ambient sound level (0 - 255)
};

struct SDclipNode
{
	int planenum;
	short children[2]; // negative numbers are contents
};

struct SDtexInfo
{
	float vecs[2][4]; // texmatrix [s/t][xyz offset]
	int miptex;
	int flags;
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
	short side;

	int firstedge; // we must support > 64k edges
	short numedges;
	short texinfo;

	// lighting info
	quint8 styles[LM_STYLES];
	int lightofs; // start of [numstyles*surfsize] samples
};


struct SMplane
{
	float normal[3];
	float dist;
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
	float vertex[3]; // position
	float normal[3]; // normal
	float stcoord[2]; // ST texture coords
	float lmcoord[2]; // ST lightmap coords
	float sccoord[2]; // ST scissor coords (decals only) - for normalmap coords migration
	float tangent[3]; // tangent
	float binormal[3]; // binormal
	quint8 color[4]; // colors per vertex
};

struct SCacheUser
{
	void *data; // extradata
};


struct SAlight
{
	int ambientlight;	// clip at 128
	int shadelight;	// clip at 192 - ambientlight
	float color[3];
	float *plightvec;
};

struct SAuxvert
{
	float fv[3]; // viewspace x, y
};


struct SMspriteFrame
{
	int width;
	int height;
	float up, down, left, right;
	int gl_texturenum;
};

// MD5 Hash
struct SMd5Context
{
	quint32 buf[4];
	quint32 bits[2];
	quint8 in[64];
};

struct SSoundItem
{
	QString name;
	short entnum;
	float origin[3];
	float volume;
	float attenuation;
	bool looping;
	quint8 channel;
	quint8 pitch;
	quint8 quint16Index;	// current playing quint16 in sentence
	double samplePos;
	double forcedEnd;
};

struct SBpcDesc
{
	int format;	// pixelformat
	char name[16];	// used for debug
	quint32 glFormat;	// RGBA format
	int bpp;	// channels (e.g. rgb = 3, rgba = 4)
};

struct SWavData
{
	quint16 rate; // num samples per second (e.g. 11025 - 11 khz)
	quint8 width; // resolution - bum bits divided by 8 (8 bit is 1, 16 bit is 2)
	quint8 channels; // num channels (1 - mono, 2 - stereo)
	int loopStart;	// offset at this point sound will be looping while playing more than only once
	int samples; // total samplecount in wav
	uint type; // compression type
	uint flags; // misc sound flags
	quint8 *buffer; // sound buffer
	size_t size; // for bounds checking
};

struct SModelState
{
	short sequence;
	short frame; // 10 bits multiple by 4, should be enough
	quint8 blending[2];
	quint8 controller[4];
	quint8 body;
	quint8 skin;
};

struct SDllFunc
{
	QString *name;
	void **func;
};

struct SField
{
	QString buffer;
	int cursor;
	int scroll;
	int widthInChars;
};

struct SNotify
{
	QString szNotify;
	float expire;
	quint8 color[4];
	int key_dest;
};

struct SConNprint
{
	int index; // Row #
	float time_to_live;	// # of seconds before it dissappears
	float color[3]; // RGB colors ( 0.0 -> 1.0 scale )
};

struct SClientData
{
	float origin[3];
	float velocity[3];

	int viewmodel;
	float punchangle[3];
	int flags;
	int waterlevel;
	int watertype;
	float view_ofs[3];
	float health;

	int bInDuck;
	int weapons; // remove?

	int flTimeStepSound;
	int flDuckTime;
	int flSwimTime;
	int waterjumptime;

	float maxspeed;

	float fov;
	int weaponanim;

	int m_iId;
	int ammo_shells;
	int ammo_nails;
	int ammo_cells;
	int ammo_rockets;
	float m_flNextAttack;

	int tfstate;
	int pushmsec;
	int deadflag;
	char physinfo[MAX_PHYSINFO_STRING];

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
};


// internal shared gameinfo structure (readonly for engine parts)
struct SGameInfo
{
	// filesystem info
	QString gamefolder;	// used for change game '-game x'
	QString basedir;	// base game directory (like 'id1' for Quake or 'valve' for Half-Life)
	QString gamedir;	// game directory (can be match with basedir, used as game dir and as write path)
	QString falldir;	// used as second basedir
	QString startmap;	// map to start singleplayer game
	QString trainmap;	// map to start hazard course (if specified)
	QString title;	// Game Main Title
	QString version; // game version (optional)

	// .dll pathes
	QString dll_path;	// e.g. "bin" or "cl_dlls"
	QString game_dll;	// custom path for game.dll

	// .ico path
	QString iconpath;	// "game.ico" by default

	// about mod info
	QString game_url; // link to a developer's site
	QString update_url;	// link to updates page
	QString type; // single, toolkit, multiplayer etc
	QString date;
	int size;

	int gamemode;
	bool secure; // prevent to console acess
	bool nomodels; // don't let player to choose model (use player.mdl always)

	QString sp_entity;	// e.g. info_player_start
	QString mp_entity;	// e.g. info_player_deathmatch

	float client_mins[4][3];	// 4 hulls allowed
	float client_maxs[4][3];	// 4 hulls allowed

	QString ambientsound[4];	// quake ambient sounds
	int soundclip_dist; // custom distance to clip sound

	int max_edicts;	// min edicts is 600, max edicts is 4096
	int max_tents;	// min temp ents is 300, max is 2048
	int max_beams;	// min beams is 64, max beams is 512
	int max_particles;	// min particles is 4096, max particles is 32768
};

struct SCmdBuf
{
	QString data;
	int cursize;
	int maxsize;
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
	int filepos;
	int filelen;
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
	int filepos;
	int disksize;
	int size; // uncompressed
	char type;
	char compression;	// probably not used
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
	char name[16];
	quint32 width;
	quint32 height;
	quint32 offsets[4];	// four mip maps stored
};

struct SWadType
{
	char *ext;
	char type;
};

struct SFile
{
	int handle; // file descriptor
	int real_length; // uncompressed file size (for files opened in "read" mode)
	int position; // current position in the file
	int offset; // offset into the package (0 if external file)
	int ungetc; // single stored character from ungetc, cleared to EOF when read
	time_t filetime; // pak, wad or real filetime
   // Contents buffer
	int buff_ind, buff_len; // buffer current index and length
	quint8 buff[FILE_BUFF_SIZE];	// intermediate buffer
};

struct SPackfile
{
	char name[56];
	int offset;
	int realsize;	// real file size (uncompressed)
};


// ---------------------------------------- //
// ------------ NESTED STRUCTS ------------ //
// ---------------------------------------- //

struct SOptionalHeader
{
	quint16 Magic;
	quint8 MajorLinkerVersion;
	quint8 MinorLinkerVersion;
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
	char szFileName[64];	// File name to download/precache.
	EResourceType type; // t_sound, t_skin, t_model, t_decal.
	int nIndex; // For t_decals
	int nDownloadSize;	// Size in Bytes if this must be downloaded.
	quint8 ucFlags;

	// for handling client to client resource propagation
	quint8 rgucMD5_hash[16];	// To determine if we already have it.
	quint8 playernum;	// Which player index this resource is associated with,
   // if it's a custom resource.

	quint8 rguc_reserved[32];	// For future expansion
	SResource *pNext; // Next in chain.
	SResource *pPrev;
};

struct SCustomization
{
	bool bInUse; // Is this customization in use;
	SResource resource; // The resource_t for this customization
	bool bTranslated;	// Has the raw data been translated into a useable format?
   // (e.g., raw decal .wad make into STexture *)
	int nUserData1;	// Customization specific data
	int nUserData2;	// Customization specific data
	void *pInfo; // Buffer that holds the data structure that references
   // the data (e.g., the cachewad_t)
	void *pBuffer; // Buffer that holds the data for the customization
   // (the raw .wad data)
	SCustomization *pNext; // Next in chain
};

struct SEfrag
{
	SMleaf *leaf;
	SEfrag *leafnext;
	SClEntity *entity;
	SEfrag *entnext;
};

// Entity state is used for the baseline and for delta compression of a packet of
//  entities that is sent to a client.
struct SEntityState
{
// Fields which are filled in by routines outside of delta compression
	int entityType;
	// Index into cl_entities array for this entity.
	int number;
	float msg_time;

	// Message number last time the player/entity state was updated.
	int messagenum;

	// Fields which can be transitted and reconstructed over the network stream
	float origin[3];
	float angles[3];

	int modelindex;
	int sequence;
	float frame;
	int colormap;
	short skin;
	short solid;
	int effects;
	float scale;
	quint8 eflags;

	// Render information
	int rendermode;
	int renderamt;
	SColor24 rendercolor;
	int renderfx;

	int movetype;
	float animtime;
	float framerate;
	int body;
	quint8 controller[4];
	quint8 blending[4];
	float velocity[3];

	// Send bbox down to client for use during prediction.
	float mins[3];
	float maxs[3];

	int aiment;
	// If owned by a player, the index of that player ( for projectiles ).
	int owner;

	// Friction, for prediction.
	float friction;
	// Gravity multiplier
	float gravity;

	// PLAYER SPECIFIC
	int team;
	int playerclass;
	int health;
	bool spectator;
	int weaponmodel;
	int gaitsequence;
	// If standing on conveyor, e.g.
	float basevelocity[3];
	// Use the crouched hull, or the regular player hull.
	int usehull;
	// Latched buttons last time state updated.
	int oldbuttons;
	// -1 = in air, else pmove entity number
	int onground;
	int iStepLeft;
	// How fast we are falling
	float flFallVelocity;

	float fov;
	int weaponanim;

	// Parametric movement overrides
	float startpos[3];
	float endpos[3];
	float impacttime;
	float starttime;

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
};

struct SLocalState
{
	double timestamp;
	SEntityState playerstate;
	SClientData client;
	SWeaponData weapondata[64];
};

struct SClEntity
{
	int index;      // Index into cl_entities ( should match actual slot, but not necessarily )
	bool player;     // True if this entity is a "player"

	SEntityState baseline;   // The original state from which to delta during an uncompressed message
	SEntityState prevstate;  // The state information from the penultimate message received from the server
	SEntityState curstate;   // The state information from the last message received from server

	int current_position;	// Last received history update index
	SPositionHistory ph[HISTORY_MAX];	// History of position and angle updates for this player

	SMouth mouth; // For synchronizing mouth movements.

	SLatchedVars latched; // Variables used by studio model rendering routines

	// Information based on interplocation, extrapolation, prediction, or just copied from last msg received.
	//
	float lastmove;

	// Actual render position and angles
	float origin[3];
	float angles[3];

	// Attachment points
	float attachment[4][3];

	// Other entity local information
	int trivial_accept;

	struct SModel *model;	// cl.model_precache[ curstate.modelindes ];  all visible entities have a model
	struct SEfrag *efrag;	// linked list of efrags
	struct SMnode *topnode;	// for bmodels, first world node that splits bmodel, or NULL if not split

	float syncbase;	// for client-side animations -- used by obsolete alias animation system, remove?
	int visframe;	// last frame this entity was found in an active leaf
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
	int intermission;
	int paused;
	int spectator;
	int onground;
	int waterlevel;

	float simvel[3];
	float simorg[3];

	float viewheight[3];
	float idealpitch;

	float cl_viewangles[3];
	int health;
	float crosshairangle[3];
	float viewsize;

	float punchangle[3];
	int maxclients;
	int viewentity;
	int playernum;
	int max_entities;
	int demoplayback;
	int hardware;
	int smoothing;

	// Last issued usercmd
	SUserCmd *cmd;

	// Movevars
	SMoveVars *movevars;

	int viewport[4];	// the viewport coordinates x, y, width, height
	int nextView; // the renderer calls ClientDLL_CalcRefdef() and Renderview
	// so long in cycles until this value is 0 (multiple views)
	int onlyClientDraw;	// if !=0 nothing is drawn by the engine except clientDraw functions
// Xash3D extension
	float fov_x, fov_y;	// actual fov can be overrided on nextView
};

struct SDheader
{
	int version;
	SDlump lumps[HEADER_LUMPS];
};

struct STexture
{
	char name[16];
	quint32 width, height;
	int gl_texturenum;
	STexture *texturechain;	// for gl_texsort drawing
	int anim_total;	// total tenths in sequence ( 0 = no)
	int anim_min, anim_max;	// time for this frame min <=time< max
	STexture *anim_next;	// in the animation sequence
	STexture *alternate_anims;	// bmodels in frame 1 use these
	quint16 fb_texturenum;	// auto-luma texturenum
	quint16 dt_texturenum;	// detail-texture binding
	quint32 unused[3];	// reserved
};

struct SMtexInfo
{
	float vecs[2][4];	// [s/t] unit vectors in world space.
	// [i][3] is the s/t offset relative to the origin.
	// s or t = dot( 3Dpoint, vecs[i] ) + vecs[i][3]
	float mipadjust;	// mipmap limits for very small surfaces
	STexture *texture;
	int flags; // sky or slime, no lightmap or 256 subdivision
};

struct SGlpoly
{
	SGlpoly *next;
	SGlpoly *chain;
	int numverts;
	int flags;           // for SURF_UNDERWATER
	float verts[4][VERTEXSIZE];	// variable sized (xyz s1t1 s2t2)
};

struct SMnode
{
	// common with leaf
	int contents; // 0, to differentiate from leafs
	int visframe; // node needs to be traversed if current

	float minmaxs[6];	// for bounding box culling
	SMnode *parent;

	// node specific
	SMplane *plane;
	SMnode *children[2];

	quint16 firstsurface;
	quint16 numsurfaces;
};

struct SMsurface
{
	int visframe; // should be drawn when node is crossed

	SMplane *plane; // pointer to shared plane
	int flags; // see SURF_ #defines

	int firstedge;	// look up in model->surfedges[], negative numbers
	int numedges; // are backwards edges

	short texturemins[2];
	short extents[2];

	int light_s, light_t;	// gl lightmap coordinates

	SGlpoly *polys; // multiple if warped
	SMsurface *texturechain;

	SMtexInfo *texinfo;

	// lighting info
	int dlightframe;	// last frame the surface was checked by an animated light
	int dlightbits;	// dynamically generated. Indicates if the surface illumination
	// is modified by an animated light.

	int lightmaptexturenum;
	quint8 styles[MAXLIGHTMAPS];
	int cached_light[MAXLIGHTMAPS];	// values currently used in lightmap
	SMsurface *lightmapchain; // for new dlights rendering (was cached_dlight)

	SColor24 *samples; // note: this is the actual lightmap data for this surface
	SDecal *pdecals;
};

// JAY: Compress this as much as possible
struct SDecal
{
	SDecal *pnext; // linked list for each surface
	SMsurface *psurface;	// Surface id for persistence / unlinking
	float dx; // local texture coordinates
	float dy; //
	float scale; // Pixel scale
	short texture; // Decal texture
	quint8 flags; // Decal flags  FDECAL_*
	short entityIndex;	// Entity this is attached to
// Xash3D added
	float position[3]; // location of the decal center in world space.
	float saxis[3]; // direction of the s axis in world space
	SMsurfMesh *mesh; // decal mesh in local space
	int reserved[4];	// for future expansions
};

struct SMleaf
{
// common with node
	int contents;
	int visframe; // node needs to be traversed if current

	float minmaxs[6];	// for bounding box culling

	SMnode *parent;
// leaf specific
	quint8 *compressed_vis;
	SEfrag *efrags;

	SMsurface **firstmarksurface;
	int nummarksurfaces;
	quint8 *compressed_pas;
	quint8 ambient_sound_level[NUM_AMBIENTS];

};

struct SMsurfMesh
{
	quint16 numVerts;
	quint16 numElems; // ~ 20 000 vertex per one surface. Should be enough
	quint32 startVert;	// user-variable. may be used for construct world single-VBO
	quint32 startElem;	// user-variable. may be used for construct world single-VBO

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

	int dlight_s, dlight_t;	// gl lightmap coordinates for dynamic lightmaps

	int mirrortexturenum;	// gl texnum
	float mirrormatrix[4][4];
	SMextraSurf *mirrorchain;	// for gl_texsort drawing
	SMextraSurf *detailchain;	// for detail textures drawing
	SColor24 *deluxemap;	// note: this is the actual deluxemap data for this surface

	int reserved[32];	// just for future expansions or mod-makers
};

struct SHull
{
	SDclipNode *clipnodes;
	SMplane *planes;
	int firstclipnode;
	int lastclipnode;
	float clip_mins[3];
	float clip_maxs[3];
};

struct SMmodel
{
	char name[64]; // model name
	bool needload; // bmodels and sprites don't cache normally

	// shared modelinfo
	EModelType type; // model type
	int numframes;	// sprite's framecount
	quint8 *mempool; // private mempool (was synctype)
	int flags; // hl compatibility

//
// volume occupied by the model
//
	float mins[3], maxs[3];	// bounding box at angles '0 0 0'
	float radius;

	// brush model
	int firstmodelsurface;
	int nummodelsurfaces;

	int numsubmodels;
	SDmodel *submodels;	// or studio animations

	int numplanes;
	SMplane *planes;

	int numleafs; // number of visible leafs, not counting 0
	SMleaf *leafs;

	int numvertexes;
	SMvertex *vertexes;

	int numedges;
	SMedge *edges;

	int numnodes;
	SMnode *nodes;

	int numtexinfo;
	SMtexInfo *texinfo;

	int numsurfaces;
	SMsurface *surfaces;

	int numsurfedges;
	int *surfedges;

	int numclipnodes;
	SDclipNode *clipnodes;

	int nummarksurfaces;
	SMsurface **marksurfaces;

	SHull hulls[MAX_MAP_HULLS];

	int numtextures;
	STexture **textures;

	quint8 *visdata;

	SColor24 *lightdata;
	char *entities;
//
// additional model data
//
	SCacheUser cache; // only access through Mod_Extradata
};

struct SPlayerInfo
{
	int userid; // User id on server
	char userinfo[MAX_INFO_STRING];	// User info string
	char name[MAX_SCOREBOARDNAME];	// Name (extracted from userinfo)
	int spectator; // Spectator or not, unused

	int ping;
	int packet_loss;

	// skin information
	char model[64];
	int topcolor;
	int bottomcolor;

	// last frame rendered
	int renderframe;

	// Gait frame estimation
	int gaitsequence;
	float gaitframe;
	float gaityaw;
	float prevgaitorigin[3];

	SCustomization customdata;
};

struct SMspriteGroup
{
	int numframes;
	float *intervals;
	SMspriteFrame *frames[1];
};

struct SMspriteFrameDesc
{
	ESpriteFrameType type;
	SMspriteFrame *frameptr;
};

struct SMsprite
{
	short type;
	short texFormat;
	int maxwidth;
	int maxheight;
	int numframes;
	int radius;
	int facecull;
	int synctype;
	SMspriteFrameDesc frames[1];
};

struct SSysInfo
{
	QString ModuleName;	// exe.filename
	SGameInfo *GameInfo;	// current GameInfo
	SGameInfo *games;	// environment games (founded at each engine start)
	int numgames;
};


struct SNetadr
{
	ENetadrType type;
	quint8 ip[4];
	quint8 ipx[10];
	quint16 port;
};

struct SHostRedirect
{
	ERdType target;
	QByteArray buffer;
	int buffersize;
	SNetadr address;
	void (*flush)( SNetadr adr, ERdType target, char *buffer );
};

struct SHostParams
{
	QStringList argv;

	EInstanceType type; // running at
	EHostState state; // global host state
	QString finalmsg; // server shutdown final message
	SHostRedirect rd; // remote console

	quint32 framecount;	// global framecount
	qint32 errorframe;	// to prevent multiple host error
	double realtime; // host.curtime
	double frametime;	// time between engine frames
	double realframetime;	// for some system events, e.g. console animations

	int developer;	// show all developer's message
	int old_developer;	// keep real dev state (we need enable dev-mode in multiplayer)
	bool key_overstrike;	// key overstrike mode
	bool stuffcmdsrun;	// execute stuff commands
	bool con_showalways;	// show console always (developer and dedicated)
	bool change_game;	// initialize when game is changed
	bool mouse_visible;	// vgui override cursor control
	bool input_enabled;	// vgui override mouse & keyboard input
	bool shutdown_issued;	// engine is shutting down
	bool decal_loading;	// nasty hack to tell imagelib about decal
	bool overview_loading;	// another nasty hack to tell imagelib about ovierview
	bool force_draw_version;	// used when fraps is loaded
	bool write_to_clipboard;	// put image to clipboard instead of disk
	bool crashed; // set to true if crashed

	QString rootdir;	// member root directory
	QString gamefolder;	// it's a default gamefolder

	QByteArray mempool; // static mempool for misc allocations
	QByteArray imagepool;	// imagelib mempool
	QByteArray soundpool;	// soundlib mempool

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
	uint type; // compression type
	uint flags; // misc image flags
	quint8 *palette; // palette if present
	quint8 *buffer; // image buffer
	quint8 fogParams[4];	// some water textures in hl1 has info about fog color and alpha
	size_t size; // for bounds checking
};


struct SImgFilter_s
{
	int filter; // pixfilter_t
	float factor; // filter factor value
	float bias; // filter bias value
	EFlFlags flags; // filter additional flags
	uint blendFunc;	// blending mode
};

struct SConVar
{
	// this part shared with cvar_t
	QString name;
	QString string;
	int flags;
	float value;
	SConVar *next;

	// this part unique for convar_t
	int integer; // atoi( string )
	bool modified; // set each time the cvar is changed
	QString reset_string;	// cvar_restart will reset to this value
	QString latched_string;	// for CVAR_LATCH vars
	QString description;	// variable descrition info
};


struct SDecalItem
{
	float *position[3];
	QString name;
	short entityIndex;
	float *depth;
	float *flags;
	float scale;

	// this is the surface plane that we hit so that
	// we can move certain decals across
	// transitions if they hit similar geometry
	float *impactPlaneNormal;

	SModelState studio_state;	// studio decals only
};



struct SDllInfo
{
	QString name; // name of library
	const SDllFunc *fcts; // list of dll exports
	bool crash; // crash if dll not found
	void *link; // hinstance of loading library
};

struct SConsole
{
	bool initialized;

	QString text;
	int current; // line where next message will be printed
	int display; // bottom of console displays this line
	int x; // offset in current line for next print

	int linewidth;	// characters across screen
	int totallines;	// total lines in console scrollback

	float displayFrac;	// aproaches finalFrac at scr_conspeed
	float finalFrac;	// 0.0 to 1.0 lines of console to display

	int vislines; // in scanlines
	QList<double> times;	// host.realtime the line was generated for transparent notify lines
	quint8 color[4];

	// console images
	int background;	// console background

	// conchars
//	cl_font_t chars[CON_NUMFONTS];// fonts.wad/font1.fnt
//	cl_font_t *curFont, *lastUsedFont;

	// console input
	SField input;

	// chatfiled
	SField chat;
	QString chat_cmd; // can be overrieded by user

	// console history
	QList<SField> historyLines;
	int historyLine;	// the line being displayed from history buffer will be <= nextHistoryLine
	int nextHistoryLine;	// the last line in the history buffer, not masked

	QList<SNotify> notify; // for Con_NXPrintf
	bool draw_notify;	// true if we have NXPrint message

	// console auto-complete
	QString shortestMatch;
	SField *completionField;	// con.input or dedicated server fake field-line
	QString completionString;
	QStringList cmds;
	int matchCount;
};

struct SCmd
{
	SCmd *next;
	QString name;
	xcommand_t function;
	QString desc;
	int flags;
};

struct SCmdAlias
{
	SCmdAlias *next;
	QString name;
	QString value;
};

struct SCvar
{
	QString name;
	QString string;
	int flags;
	float value;
	SCvar *next;
};

struct SGlobalVars
{
	float time;
	float frametime;
	float force_retouch;
	QString mapname;
	QString startspot;
	float deathmatch;
	float coop;
	float teamplay;
	float serverflags;
	float found_secrets;
	float v_forward[3];
	float v_up[3];
	float v_right[3];
	float trace_allsolid;
	float trace_startsolid;
	float trace_fraction;
	float trace_endpos[3];
	float trace_plane_normal[3];
	float trace_plane_dist;
	SEdict *trace_ent;
	float trace_inopen;
	float trace_inwater;
	int trace_hitgroup;
	int trace_flags;
	int changelevel;	// transition in progress when true (was msg_entity)
	int cdAudioTrack;
	int maxClients;
	int maxEntities;
	const char *pStringBase;

	void *pSaveData;	// (SAVERESTOREDATA *) pointer
	float vecLandmarkOffset[3];
};

struct SEntVars
{
	QString classname;
	QString globalname;

	float origin[3];
	float oldorigin[3];
	float velocity[3];
	float basevelocity[3];
	float clbasevelocity[3];	// Base velocity that was passed in to server physics so
	// client can predict conveyors correctly. Server zeroes it, so we need to store here, too.
	float movedir[3];

	float angles[3]; // Model angles
	float avelocity[3];	// angle velocity (degrees per second)
	float punchangle[3];	// auto-decaying view angle adjustment
	float v_angle[3]; // Viewing angle (player only)

	// For parametric entities
	float endpos[3];
	float startpos[3];
	float impacttime;
	float starttime;

	int fixangle; // 0:nothing, 1:force view angles, 2:add avelocity
	float idealpitch;
	float pitch_speed;
	float ideal_yaw;
	float yaw_speed;

	int modelindex;

	QString model;
	int viewmodel;	// player's viewmodel
	int weaponmodel;	// what other players see

	float absmin[3]; // BB max translated to world coord
	float absmax[3]; // BB max translated to world coord
	float mins[3]; // local BB min
	float maxs[3]; // local BB max
	float size[3]; // maxs - mins

	float ltime;
	float nextthink;

	int movetype;
	int solid;

	int skin;
	int body; // sub-model selection for studiomodels
	int 	effects;
	float gravity; // % of "normal" gravity
	float friction; // inverse elasticity of MOVETYPE_BOUNCE

	int light_level;

	int sequence; // animation sequence
	int gaitsequence;	// movement animation sequence for player (0 for none)
	float frame; // % playback position in animation sequences (0..255)
	float animtime; // world time when frame was set
	float framerate;	// animation playback rate (-8x to 8x)
	quint8 controller[4];	// bone controller setting (0..255)
	quint8 blending[2];	// blending amount between sub-sequences (0..255)

	float scale; // sprites and models rendering scale (0..255)
	int rendermode;
	float renderamt;
	float rendercolor[3];
	int renderfx;

	float health;
	float frags;
	int weapons; // bit mask for available weapons
	float takedamage;

	int deadflag;
	float view_ofs[3]; // eye position

	int button;
	int impulse;

	SEdict *chain; // Entity pointer when linked into a linked list
	SEdict *dmg_inflictor;
	SEdict *enemy;
	SEdict *aiment; // entity pointer when MOVETYPE_FOLLOW
	SEdict *owner;
	SEdict *groundentity;

	int spawnflags;
	int flags;

	int colormap; // lowquint8 topcolor, highquint8 bottomcolor
	int team;

	float max_health;
	float teleport_time;
	float armortype;
	float armorvalue;
	int waterlevel;
	int watertype;

	QString target;
	QString targetname;
	QString netname;
	QString message;

	float dmg_take;
	float dmg_save;
	float dmg;
	float dmgtime;

	QString noise;
	QString noise1;
	QString noise2;
	QString noise3;

	float speed;
	float air_finished;
	float pain_finished;
	float radsuit_finished;

	SEdict *pContainingEntity;

	int playerclass;
	float maxspeed;

	float fov;
	int weaponanim;

	int pushmsec;

	int bInDuck;
	int flTimeStepSound;
	int flSwimTime;
	int flDuckTime;
	int iStepLeft;
	float flFallVelocity;

	int gamestate;

	int oldbuttons;

	int groupinfo;

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
	SEdict *euser1;
	SEdict *euser2;
	SEdict *euser3;
	SEdict *euser4;
};

struct SEdict
{
	bool free;
	int serialnumber;

	SLink area; // linked to a division node or leaf
	int headnode; // -1 to use normal leaf check

	int num_leafs;
	short leafnums[MAX_ENT_LEAFS];

	float freetime; // sv.time when the object was freed

	void* pvPrivateData;	// Alloced and freed by engine, used by DLLs
	SEntVars v; // C exported fields from progs

	// other fields from progs come immediately after
};

struct STrace
{
	bool allsolid; // if true, plane is not valid
	bool startsolid;	// if true, the initial point was in a solid area
	bool inopen, inwater;
	float fraction; // time completed, 1.0 = didn't hit anything
	float endpos[3]; // final position
	SPlane plane; // surface normal at impact
	SEdict *ent; // entity the surface is on
	int hitgroup; // 0 == generic, non zero is specific body part
};

struct SHpak
{
	char *name;
	SResource HpakResource;
	size_t size;
	void *data;
	struct hpak_s *next;
};

struct SHpakDir
{
	SResource DirectoryResource;
	int seek; // filepos ?
	int size;
};

struct SHpakContainer
{
	int count;
	SHpakDir *dirs; // variable sized.
};

struct SWfile
{
	char filename[MAX_SYSPATH];
	int infotableofs;
	quint8 *mempool;	// W_ReadLump temp buffers
	int numlumps;
	int mode;
	int handle;
	SDlumpInfo *lumps;
	time_t filetime;
};

struct SPack
{
	char filename[MAX_SYSPATH];
	int handle;
	int numfiles;
	time_t filetime;	// common for all packed files
	SPackFile *files;
};

struct SSearchPath
{
	char filename[MAX_SYSPATH];
	SPack *pack;
	SWfile *wad;
	int flags;
	struct searchpath_s *next;
};

#endif // XASH_TYPES_HPP

