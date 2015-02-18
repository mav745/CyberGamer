#ifndef C_GATE_HPP
#define C_GATE_HPP

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "studio.h"
#include "vgui_draw.h"
#include "mathlib.h"
#include "particledef.h"
#include "beamdef.h"
#include "sprite.h"
#include "client.h"
#include "cl_tent.h"
#include "gl_local.h"
#include "shake.h"
#include "triangleapi.h"
#include "r_studioint.h"

#define TEXTURES_HASH_SIZE	64

extern int	r_textureMinFilter;
extern int	r_textureMagFilter;
extern gltexture_t	r_textures[MAX_TEXTURES];
extern gltexture_t	*r_texturesHashTable[TEXTURES_HASH_SIZE];
extern int	r_numTextures;
extern byte	*scaledImage;			// pointer to a scaled image
extern byte	data2D[BLOCK_SIZE_MAX*BLOCK_SIZE_MAX*4];	// intermediate texbuffer
extern rgbdata_t	r_image;					// generic pixelbuffer used for internal textures

extern convar_t		*tracerred;
extern convar_t		*tracergreen;
extern convar_t		*tracerblue;
extern convar_t		*traceralpha;
extern convar_t		*tracerspeed;
extern convar_t		*tracerlength;
extern convar_t		*traceroffset;

// particle ramps
#define SPARK_COLORCOUNT 9
#define TRACER_WIDTH 0.5f
#define SIMSHIFT 10
extern int ramp1[8];
extern int ramp2[8];
extern int ramp3[6];
extern int boxpnt[6][4];
extern rgb_t gTracerColors[];
extern int gSparkRamp[SPARK_COLORCOUNT][3];

#define NOISE_DIVISIONS	64 // don't touch - many tripmines cause the crash when it equal 128

qboolean CL_TracerComputeVerts( const vec3_t start, const vec3_t delta, float width, vec3_t *pVerts );

typedef struct
{
	vec3_t	pos;
	vec3_t	color;
	float	texcoord; // Y texture coordinate
	float	width;
	float	alpha;
} beamseg_t;

void ComputeBeamPerpendicular( const vec3_t vecBeamDelta, vec3_t pPerp );
void ComputeNormal( const vec3_t vStartPos, const vec3_t vNextPos, vec3_t pNormal );

extern float rgNoise[NOISE_DIVISIONS+1]; // global noise array
void FracNoise( float *noise, int divs, float scale );

typedef struct
{
	int		allocated[BLOCK_SIZE_MAX];
	int		current_lightmap_texture;
	msurface_t	*dynamic_surfaces;
	msurface_t	*lightmap_surfaces[MAX_LIGHTMAPS];
	byte		lightmap_buffer[BLOCK_SIZE_MAX*BLOCK_SIZE_MAX*4];
} gllightmapstate_t;
extern int		nColinElim; // stats
extern vec2_t		world_orthocenter;
extern vec2_t		world_orthohalf;
extern byte		visbytes[MAX_MAP_LEAFS/8];
extern uint		r_blocklights[BLOCK_SIZE_MAX*BLOCK_SIZE_MAX*3];
extern glpoly_t		*fullbright_polys[MAX_TEXTURES];
extern qboolean		draw_fullbrights;
extern mextrasurf_t		*detail_surfaces[MAX_TEXTURES];
extern qboolean		draw_details;
extern msurface_t		*skychain;
extern gllightmapstate_t gl_lms;

void LM_InitBlock( void );
int LM_AllocBlock( int w, int h, int *x, int *y );
void R_BuildLightMap( msurface_t *surf, byte *dest, int stride, qboolean dynamic );
texture_t *R_TextureAnimation( texture_t *base, int surfacenum );
void R_SetCacheState( msurface_t *surf );
qboolean R_SpriteOccluded( cl_entity_t *e, vec3_t origin, int *alpha, float *pscale );
qboolean R_SpriteHasLightmap( cl_entity_t *e, int texFormat );

extern vec3_t	sprite_mins, sprite_maxs;
extern float	sprite_radius;
extern uint	r_texFlags;

extern qboolean			m_fDoInterp;
extern qboolean			m_fDoRemap;
extern mstudiomodel_t		*m_pSubModel;
extern mstudiobodyparts_t		*m_pBodyPart;
extern player_info_t		*m_pPlayerInfo;
extern studiohdr_t		*m_pStudioHeader;
extern studiohdr_t		*m_pTextureHeader;
extern float			m_flGaitMovement;
//extern pmtrace_t			g_shadowTrace;
extern vec3_t			g_mvShadowVec;
extern int			g_nTopColor, g_nBottomColor;	// remap colors
extern int			g_nFaceFlags, g_nForceFaceFlags;

#define DECAL_OVERLAP_DISTANCE	2
#define DECAL_DISTANCE		4	// too big values produce more clipped polygons
#define MAX_DECALCLIPVERT		32	// produced vertexes of fragmented decal
#define DECAL_CACHEENTRY		256	// MUST BE POWER OF 2 or code below needs to change!
#define DECAL_TRANSPARENT_THRESHOLD	230	// transparent decals draw with GL_MODULATE

// empirically determined constants for minimizing overalpping decals
#define MAX_OVERLAP_DECALS		6
#define DECAL_OVERLAP_DIST		8
#define MIN_DECAL_SCALE		0.01f
#define MAX_DECAL_SCALE		16.0f

// clip edges
#define LEFT_EDGE			0
#define RIGHT_EDGE			1
#define TOP_EDGE			2
#define BOTTOM_EDGE			3

float R_GetSpriteFrameInterpolant( cl_entity_t *ent, mspriteframe_t **oldframe, mspriteframe_t **curframe );
void R_StudioSetupTextureHeader( void );
void R_StudioSetupChrome( float *pchrome, int bone, vec3_t normal );

#define EVENT_CLIENT	5000	// less than this value it's a server-side studio events
#define MAXARRAYVERTS	16384	// used for draw shadows
#define LEGS_BONES_COUNT	8

typedef struct studiolight_s
{
	vec3_t		lightvec;			// light vector
	vec3_t		lightcolor;		// ambient light color
	vec3_t		lightspot;		// potential coords where placed lightsource

	vec3_t		blightvec[MAXSTUDIOBONES];	// ambient lightvectors per bone
	vec3_t		dlightvec[MAX_DLIGHTS][MAXSTUDIOBONES];
	vec3_t		dlightcolor[MAX_DLIGHTS];	// ambient dynamic light colors
	vec3_t		elightvec[MAX_ELIGHTS][MAXSTUDIOBONES];
	vec3_t		elightcolor[MAX_ELIGHTS];	// ambient entity light colors
	int		numdlights;
	int		numelights;
} studiolight_t;

typedef struct sortedmesh_s
{
	mstudiomesh_t	*mesh;
	int		flags;			// face flags
} sortedmesh_t;

//extern r_studio_interface_t	*pStudioDraw;
extern float		aliasXscale, aliasYscale;	// software renderer scale
extern matrix3x4		g_aliastransform;		// software renderer transform
extern matrix3x4		g_rotationmatrix;
extern vec3_t		g_chrome_origin;
extern vec2_t		g_chrome[MAXSTUDIOVERTS];	// texture coords for surface normals
extern sortedmesh_t		g_sortedMeshes[MAXSTUDIOMESHES];
extern matrix3x4		g_bonestransform[MAXSTUDIOBONES];
extern matrix3x4		g_lighttransform[MAXSTUDIOBONES];
extern matrix3x4		g_rgCachedBonesTransform[MAXSTUDIOBONES];
extern matrix3x4		g_rgCachedLightTransform[MAXSTUDIOBONES];
extern vec3_t		g_chromeright[MAXSTUDIOBONES];// chrome vector "right" in bone reference frames
extern vec3_t		g_chromeup[MAXSTUDIOBONES];	// chrome vector "up" in bone reference frames
extern int		g_chromeage[MAXSTUDIOBONES];	// last time chrome vectors were updated
extern vec3_t		g_xformverts[MAXSTUDIOVERTS];
extern vec3_t		g_xformnorms[MAXSTUDIOVERTS];
extern vec3_t		g_xarrayverts[MAXARRAYVERTS];
extern uint		g_xarrayelems[MAXARRAYVERTS*6];
extern uint		g_nNumArrayVerts;
extern uint		g_nNumArrayElems;
extern vec3_t		g_lightvalues[MAXSTUDIOVERTS];
extern studiolight_t	g_studiolight;
extern int			g_nCachedBones;		// number of bones in cache
extern int			g_nStudioCount;		// for chrome update
extern int			g_iRenderMode;		// currentmodel rendermode
extern int			g_iBackFaceCull;
extern convar_t			*r_studio_lerping;
extern convar_t			*r_studio_lambert;
extern convar_t			*r_studio_lighting;
extern convar_t			*r_studio_sort_textures;
extern convar_t			*r_drawviewmodel;
extern convar_t			*r_customdraw_playermodel;
extern convar_t			*cl_himodels;

void R_StudioLighting( float *lv, int bone, int flags, vec3_t normal );
int R_StudioMeshCompare(const void *a, const void *b );


#define MAX_CLIP_VERTS	64 // skybox clip vertices
#define TURBSCALE		( 256.0f / ( M_PI2 ))
extern float		speedscale;
extern const char*		r_skyBoxSuffix[6];
extern const int		r_skyTexOrder[6];
extern float r_turbsin[];

extern convar_t	*vgui_colorstrings;
extern int	g_textures[VGUI_MAX_TEXTURES];
extern int	g_textureId;
extern int	g_iBoundTexture;

extern const vec3_t skyclip[6];
extern const int st_to_vec[6][3];
extern const int vec_to_st[6][3];

void GL_LoadMatrix( const matrix4x4 source );
void GL_SetTextureType( GLenum texnum, GLenum type );
mspriteframe_t *R_GetSpriteFrame( const model_t *pModel, int frame, float yaw );
int GL_LoadTextureInternal( const char *name, rgbdata_t *pic, texFlags_t flags, qboolean update );
qboolean GL_Support( int r_ext );
byte *GL_ResampleTexture( const byte *source, int inWidth, int inHeight, int outWidth, int outHeight, qboolean isNormalMap );
void GL_ResetFogColor( void );
void GL_SetupFogColorForSurfaces( void );
void GL_ResetFogColor( void );
void R_BeginDrawMirror( msurface_t *fa );
void R_EndDrawMirror( void );
void R_LightForPoint( const vec3_t point, color24 *ambientLight, qboolean invLight, qboolean useAmbient, float radius );
void R_LoadIdentity( void );



void GL_BackendStartFrame( void );
void GL_BackendEndFrame( void );
void GL_CleanUpTextureUnits( int last );
//void GL_Bind( GLint tmu, GLenum texnum );
void GL_MultiTexCoord2f( GLenum texture, GLfloat s, GLfloat t );
void GL_SetTexCoordArrayMode( GLenum mode );
void GL_LoadTexMatrix( const matrix4x4 m );
void GL_LoadTexMatrixExt( const float *glmatrix );
//void GL_LoadMatrix( const matrix4x4 source );
void GL_TexGen( GLenum coord, GLenum mode );
//void GL_SelectTexture( GLint texture );
void GL_LoadIdentityTexMatrix( void );
void GL_DisableAllTexGens( void );
//void GL_SetRenderMode( int mode );
//void GL_FrontFace( GLenum front );
void GL_TextureTarget( uint target );
//void GL_Cull( GLenum cull );
//void R_ShowTextures( void );

//
// gl_cull.c
//
int R_CullModel( cl_entity_t *e, vec3_t origin, vec3_t mins, vec3_t maxs, float radius );
//qboolean R_CullBox( const vec3_t mins, const vec3_t maxs, uint clipflags );
qboolean R_CullSphere( const vec3_t centre, const float radius, const uint clipflags );
qboolean R_CullSurface( msurface_t *surf, uint clipflags );

//
// gl_decals.c
//
//void DrawSurfaceDecals( msurface_t *fa );
float *R_DecalSetupVerts( decal_t *pDecal, msurface_t *surf, int texture, int *outCount );
//void DrawSingleDecal( decal_t *pDecal, msurface_t *fa );
void R_EntityRemoveDecals( model_t *mod );
void R_ClearDecals( void );

//
// gl_draw.c
//
//void R_Set2DMode( qboolean enable );
//void R_DrawTileClear( int x, int y, int w, int h );
//void R_UploadStretchRaw( int texture, int cols, int rows, int width, int height, const byte *data );

//
// gl_image.c
//
//void R_SetTextureParameters( void );
gltexture_t *R_GetTexture( GLenum texnum );
//void GL_SetTextureType( GLenum texnum, GLenum type );
//int GL_LoadTexture( const char *name, const byte *buf, size_t size, int flags, imgfilter_t *filter );
//int GL_LoadTextureInternal( const char *name, rgbdata_t *pic, texFlags_t flags, qboolean update );
//byte *GL_ResampleTexture( const byte *source, int in_w, int in_h, int out_w, int out_h, qboolean isNormalMap );
int GL_CreateTexture( const char *name, int width, int height, const void *buffer, texFlags_t flags );
void GL_ProcessTexture( int texnum, float gamma, int topColor, int bottomColor );
//void GL_TexFilter( gltexture_t *tex, qboolean update );
void R_FreeImage( gltexture_t *image );
int GL_FindTexture( const char *name );
void GL_FreeTexture( GLenum texnum );
void GL_FreeImage( const char *name );
const char *GL_Target( GLenum target );
void R_TextureList_f( void );
void R_InitImages( void );
void R_ShutdownImages( void );

//
// gl_mirror.c
//
//void R_BeginDrawMirror( msurface_t *fa );
//void R_EndDrawMirror( void );
void R_DrawMirrors( void );
void R_FindMirrors( const ref_params_t *fd );

//
// gl_refrag.c
//
void R_StoreEfrags( efrag_t **ppefrag, int framecount );

//
// gl_rlight.c
//
void R_PushDlights( void );
void R_AnimateLight( void );
void R_GetLightSpot( vec3_t lightspot );
void R_MarkLights( dlight_t *light, int bit, mnode_t *node );
void R_LightDir( const vec3_t origin, vec3_t lightDir, float radius );
//void R_LightForPoint( const vec3_t point, color24 *ambientLight, qboolean invLight, qboolean useAmbient, float radius );
int R_CountSurfaceDlights( msurface_t *surf );
int R_CountDlights( void );

//
// gl_rmain.c
//
void R_ClearScene( void );
//void R_LoadIdentity( void );
void R_RenderScene( const ref_params_t *fd );
void R_DrawCubemapView( const vec3_t origin, const vec3_t angles, int size );
void R_TranslateForEntity( cl_entity_t *e );
void R_RotateForEntity( cl_entity_t *e );
int R_ComputeFxBlend( cl_entity_t *e );
qboolean R_InitRenderAPI( void );
void R_SetupFrustum( void );
void R_FindViewLeaf( void );
void R_DrawFog( void );

//
// gl_rmath.c
//
float V_CalcFov( float *fov_x, float width, float height );
void V_AdjustFov( float *fov_x, float *fov_y, float width, float height, qboolean lock_x );
void Matrix4x4_ToArrayFloatGL( const matrix4x4 in, float out[16] );
void Matrix4x4_FromArrayFloatGL( matrix4x4 out, const float in[16] );
void Matrix4x4_Concat( matrix4x4 out, const matrix4x4 in1, const matrix4x4 in2 );
void Matrix4x4_ConcatTranslate( matrix4x4 out, float x, float y, float z );
void Matrix4x4_ConcatRotate( matrix4x4 out, float angle, float x, float y, float z );
void Matrix4x4_ConcatScale( matrix4x4 out, float x );
void Matrix4x4_ConcatScale3( matrix4x4 out, float x, float y, float z );
void Matrix4x4_CreateTranslate( matrix4x4 out, float x, float y, float z );
void Matrix4x4_CreateRotate( matrix4x4 out, float angle, float x, float y, float z );
void Matrix4x4_CreateScale( matrix4x4 out, float x );
void Matrix4x4_CreateScale3( matrix4x4 out, float x, float y, float z );
void Matrix4x4_CreateProjection(matrix4x4 out, float xMax, float xMin, float yMax, float yMin, float zNear, float zFar);
void Matrix4x4_CreateOrtho(matrix4x4 m, float xLeft, float xRight, float yBottom, float yTop, float zNear, float zFar);
void Matrix4x4_CreateModelview( matrix4x4 out );

//
// gl_rmisc.
//
void R_ParseTexFilters( const char *filename );
imgfilter_t *R_FindTexFilter( const char *texname );

//
// gl_rsurf.c
//
void R_MarkLeaves( void );
void R_DrawWorld( void );
void R_DrawMirrors( void );
//void R_DrawWaterSurfaces( void );
void R_DrawBrushModel( cl_entity_t *e );
void GL_SubdivideSurface( msurface_t *fa );
void GL_BuildPolygonFromSurface( model_t *mod, msurface_t *fa );
//void GL_SetupFogColorForSurfaces( void );
void GL_RebuildLightmaps( void );
void GL_BuildLightmaps( void );
//void GL_ResetFogColor( void );

//
// gl_sprite.c
//
void R_SpriteInit( void );
void Mod_LoadSpriteModel( model_t *mod, const void *buffer, qboolean *loaded, uint texFlags );
//mspriteframe_t *R_GetSpriteFrame( const model_t *pModel, int frame, float yaw );
//void R_DrawSpriteModel( cl_entity_t *e );

//
// gl_studio.c
//
void R_StudioInit( void );
void Mod_LoadStudioModel( model_t *mod, const void *buffer, qboolean *loaded );
struct mstudiotex_s *R_StudioGetTexture( cl_entity_t *e );
void R_DrawStudioModel( cl_entity_t *e );

//
// gl_warp.c
//
void R_InitSky( struct mip_s *mt, struct texture_s *tx );
void R_AddSkyBoxSurface( msurface_t *fa );
void R_ClearSkyBox( void );
//void R_DrawSkyBox( void );
//void EmitSkyLayers( msurface_t *fa );
//void EmitSkyPolys( msurface_t *fa );
//void EmitWaterPolys( glpoly_t *polys, qboolean noCull );
//void R_DrawSkyChain( msurface_t *s );

//
// gl_vidnt.c
//
#define GL_CheckForErrors() GL_CheckForErrors_( __FILE__, __LINE__ )
void GL_CheckForErrors_( const char *filename, const int fileline );
//void GL_UpdateSwapInterval( void );
//void GL_UpdateGammaRamp( void );
qboolean GL_DeleteContext( void );
//qboolean GL_Support( int r_ext );
void VID_CheckChanges( void );
//int GL_MaxTextureUnits( void );
qboolean R_Init( void );
void R_Shutdown( void );

////
//// renderer exports
////
//qboolean R_Init( void );
//void R_Shutdown( void );
void VID_CheckChanges( void );
int GL_LoadTexture( const char *name, const byte *buf, size_t size, int flags, imgfilter_t *filter );
void GL_FreeImage( const char *name );
qboolean VID_ScreenShot( const char *filename, int shot_type );
qboolean VID_CubemapShot( const char *base, uint size, const float *vieworg, qboolean skyshot );
//void VID_RestoreGamma( void );
void R_BeginFrame( qboolean clearScene );

void R_EndFrame( void );
void R_ClearScene( void );
void R_GetTextureParms( int *w, int *h, int texnum );
void R_GetSpriteParms( int *frameWidth, int *frameHeight, int *numFrames, int curFrame, const struct model_s *pSprite );
//void R_DrawStretchRaw( float x, float y, float w, float h, int cols, int rows, const byte *data, qboolean dirty );
//void R_DrawStretchPic( float x, float y, float w, float h, float s1, float t1, float s2, float t2, int texnum );
qboolean R_SpeedsMessage( char *out, size_t size );
void R_SetupSky( const char *skyboxname );
qboolean R_CullBox( const vec3_t mins, const vec3_t maxs, uint clipflags );
qboolean R_WorldToScreen( const vec3_t point, vec3_t screen );
void R_ScreenToWorld( const vec3_t screen, vec3_t point );
qboolean R_AddEntity( struct cl_entity_s *pRefEntity, int entityType );
void Mod_LoadMapSprite( struct model_s *mod, const void *buffer, size_t size, qboolean *loaded );
void Mod_UnloadSpriteModel( struct model_s *mod );
void Mod_UnloadStudioModel( struct model_s *mod );
void Mod_UnloadBrushModel( struct model_s *mod );
//void GL_SetRenderMode( int mode );
void R_RunViewmodelEvents( void );
void R_DrawViewModel( void );
int R_GetSpriteTexture( const struct model_s *m_pSpriteModel, int frame );
void R_DecalShoot( int textureIndex, int entityIndex, int modelIndex, vec3_t pos, int flags, vec3_t saxis, float scale );
void R_RemoveEfrags( struct cl_entity_s *ent );
void R_AddEfrags( struct cl_entity_s *ent );
void R_DecalRemoveAll( int texture );
byte *Mod_GetCurrentVis( void );
void Mod_SetOrthoBounds( float *mins, float *maxs );
void R_NewMap( void );

typedef struct
{
	string		buffer;
	int		cursor;
	int		scroll;
	int		widthInChars;
} field_t;

qboolean SPR_Scissor( float *x, float *y, float *width, float *height, float *u0, float *v0, float *u1, float *v1 );
void SPR_AdjustSize( float *x, float *y, float *w, float *h );
const model_t *CL_GetSpritePointer( VHSPRITE hSprite );
qboolean PIC_Scissor( float *x, float *y, float *width, float *height, float *u0, float *v0, float *u1, float *v1 );
void VID_ImageAdjustGamma( byte *in, uint width, uint height );
cl_entity_t *CL_GetBeamEntityByIndex( int index );
void GL_RoundImageDimensions( word *width, word *height, texFlags_t flags, qboolean force );
void GL_MakeLuminance( rgbdata_t *in );
int GL_CalcTextureSamples( int flags );
void CL_DrawBeamFollow( int spriteIndex, BEAM *pbeam, int frame, int rendermode, float frametime, float *color );
void GL_BuildMipMap( byte *in, int width, int height, qboolean isNormalMap );
GLenum GL_TextureFormat( gltexture_t *tex, int *samples );
byte *GL_ApplyGamma( const byte *source, int pixels, qboolean isNormalMap );
qboolean R_StaticEntity( cl_entity_t *ent );
int R_SurfaceCompare(const void *a, const void *b );
void R_StudioDeformShadow( void );
void R_SetupModelviewMatrix( const ref_params_t *fd, matrix4x4 m );
void R_SetupProjectionMatrix( const ref_params_t *fd, matrix4x4 m );
qboolean R_StudioComputeBBox( cl_entity_t *e, vec3_t bbox[8] );
void GL_SetExtension( int r_ext, int enable );
int Con_DrawGenericString( int x, int y, const char *string, rgba_t setColor, qboolean forceColor, int hideChar );
void Con_DrawInput( void );
int Con_StringLength( const char *string );

typedef struct envmap_s
{
	vec3_t	angles;
	int	flags;
} envmap_t;


#define CON_TIMES		5	// need for 4 lines
#define COLOR_DEFAULT	'7'
#define CON_HISTORY		64
#define MAX_DBG_NOTIFY	128
#define CON_MAXCMDS		4096	// auto-complete intermediate list
#define CON_NUMFONTS	3	// maxfonts

#define CON_TEXTSIZE	131072	// 128 kb buffer

typedef struct
{
	string		szNotify;
	float		expire;
	rgba_t		color;
	int		key_dest;
} notify_t;
typedef struct
{
	qboolean		initialized;

	short		text[CON_TEXTSIZE];
	int		current;		// line where next message will be printed
	int		display;		// bottom of console displays this line
	int		x;		// offset in current line for next print

	int 		linewidth;	// characters across screen
	int		totallines;	// total lines in console scrollback

	float		displayFrac;	// aproaches finalFrac at scr_conspeed
	float		finalFrac;	// 0.0 to 1.0 lines of console to display

	int		vislines;		// in scanlines
	double		times[CON_TIMES];	// host.realtime the line was generated for transparent notify lines
	rgba_t		color;

	// console images
	int		background;	// console background

	// conchars
	cl_font_t		chars[CON_NUMFONTS];// fonts.wad/font1.fnt
	cl_font_t		*curFont, *lastUsedFont;

	// console input
	field_t		input;

	// chatfiled
	field_t		chat;
	string		chat_cmd;		// can be overrieded by user

	// console history
	field_t		historyLines[CON_HISTORY];
	int		historyLine;	// the line being displayed from history buffer will be <= nextHistoryLine
	int		nextHistoryLine;	// the last line in the history buffer, not masked

	notify_t		notify[MAX_DBG_NOTIFY]; // for Con_NXPrintf
	qboolean		draw_notify;	// true if we have NXPrint message

	// console auto-complete
	string		shortestMatch;
	field_t		*completionField;	// con.input or dedicated server fake field-line
	char		*completionString;
	char		*cmds[CON_MAXCMDS];
	int		matchCount;
} console_t;

extern console_t		con;
extern convar_t	*con_notifytime;
extern convar_t	*scr_conspeed;
extern convar_t	*con_fontsize;

extern const envmap_t r_skyBoxInfo[6];
extern const envmap_t r_envMapInfo[6];
extern r_studio_interface_t	*pStudioDraw;
extern vec3_t hullcolor[8];
extern const char *legs_bones[];

extern convar_t			*r_studio_lerping;
extern convar_t			*r_studio_lambert;
extern convar_t			*r_studio_lighting;
extern convar_t			*r_studio_sort_textures;
extern convar_t			*r_drawviewmodel;
extern convar_t			*r_customdraw_playermodel;
extern convar_t			*cl_himodels;
extern cvar_t			r_shadows;	// dead cvar. especially disabled
extern cvar_t			r_shadowalpha;




// in this module!
void initWindow(int width, int height, int fullscreen);
void killWindow();
GLuint makeTexture();
int GL_MaxTextureUnits();
void GL_SelectTexture( GLint tmu );
void GL_Bind( GLint tmu, GLenum texnum );
void GL_TexFilter( gltexture_t *tex, qboolean update );
void R_SetTextureParameters();
void GL_Cull( GLenum cull );
void GL_FrontFace( GLenum front );
void GL_SetRenderMode( int mode );
void R_DrawStretchPic( float x, float y, float w, float h, float s1, float t1, float s2, float t2, int texnum );
void R_DrawTileClear( int x, int y, int w, int h );
void R_DrawStretchRaw( float x, float y, float w, float h, int cols, int rows, const byte *data, qboolean dirty );
void R_UploadStretchRaw( int texture, int cols, int rows, int width, int height, const byte *data );
void R_Set2DMode( qboolean enable );
int R_AllocateMirrorTexture();
void CL_BulletTracerDraw( particle_t *p, float frametime );
void CL_UpdateParticle( particle_t *p, float ft );
void CL_DrawTracer( vec3_t start, vec3_t delta, float width, rgb_t color, int alpha, float startV, float endV );
void CL_DrawSegs( int modelIndex, float frame, int rendermode, const vec3_t source, const vec3_t delta,
	float width, float scale, float freq, float speed, int segments, int flags, float *color );
void CL_DrawDisk( int modelIndex, float frame, int rendermode, const vec3_t source, const vec3_t delta,
	float width, float scale, float freq, float speed, int segments, float *color );
void CL_DrawCylinder( int modelIndex, float frame, int rendermode, const vec3_t source, const vec3_t delta,
	float width, float scale, float freq, float speed, int segments, float *color );
void CL_DrawRing( int modelIndex, float frame, int rendermode, const vec3_t source, const vec3_t delta, float width,
	float amplitude, float freq, float speed, int segments, float *color );
void CL_DrawLaser( BEAM *pbeam, int frame, int rendermode, float *color, int spriteIndex );
void DrawBeamFollow( int modelIndex, particle_t *pHead, int frame, int rendermode, vec3_t delta,
	vec3_t screen, vec3_t screenLast, float die, const vec3_t source, int flags, float width,
	float amplitude, float freq, float *color );
void DrawGLPoly( glpoly_t *p, float xScale, float yScale );
void DrawGLPolyChain( glpoly_t *p, float soffset, float toffset );
void R_BlendLightmaps();
void R_RenderFullbrights();
void R_RenderDetails();
void R_RenderBrushPoly( msurface_t *fa );
void R_DrawSpriteModel( cl_entity_t *e );
void R_ShowTextures();
int TriSpriteTexture( model_t *pSpriteModel, int frame );
void DrawSingleDecal( decal_t *pDecal, msurface_t *fa );
void DrawSurfaceDecals( msurface_t *fa );
void R_StudioDrawPoints(void);
void R_StudioSetupSkin( mstudiotexture_t *ptexture, int index );
void R_DrawWaterSurfaces();
void LM_UploadBlock( qboolean dynamic );
void R_DrawSkyBox();
void EmitWaterPolys( glpoly_t *polys, qboolean noCull );
void EmitSkyPolys( msurface_t *fa );
void R_DrawSkyChain( msurface_t *s );
void EmitSkyLayers( msurface_t *fa );
void VGUI_EnableTexture( qboolean enable );
void VGUI_DrawQuad( const vpoint_t *ul, const vpoint_t *lr );
void VGUI_UploadTextureBlock( int id, int drawX, int drawY, const byte *rgba, int blockWidth, int blockHeight );
void VGUI_SetupDrawingRect( int *pColor );
void VGUI_SetupDrawingText( int *pColor );
void VGUI_SetupDrawingImage( int *pColor );
void VGUI_BindTexture( int id );
void SetBeamRenderMode( int rendermode );
void R_DrawSpriteQuad( mspriteframe_t *frame, vec3_t org, vec3_t v_right, vec3_t v_up, float scale );
void MakeSkyVec( float s, float t, int axis );

void SPR_DrawGeneric( int frame, float x, float y, float width, float height, const wrect_t *prc );
void CL_DrawScreenFade( void );
void CL_DrawLoading( float percent );
void CL_DrawPause( void );
void pfnSPR_Set( VHSPRITE hPic, int r, int g, int b );
void pfnSPR_Draw( int frame, int x, int y, const wrect_t *prc );
void pfnFillRGBA( int x, int y, int width, int height, int r, int g, int b, int a );
void pfnSPR_DrawGeneric( int frame, int x, int y, const wrect_t *prc, int blendsrc, int blenddst, int width, int height );
void pfnFillRGBABlend( int x, int y, int width, int height, int r, int g, int b, int a );
void TriRenderMode( int mode );
void TriBegin( int mode );
void TriEnd( void );
void TriColor4f( float r, float g, float b, float a );
void TriColor4ub( byte r, byte g, byte b, byte a );
void TriTexCoord2f( float u, float v );
void TriVertex3fv( const float *v );
void TriVertex3f( float x, float y, float z );
void TriBrightness( float brightness );
void TriFog( float flFogColor[3], float flStart, float flEnd, int bOn );
void TriGetMatrix( const int pname, float *matrix );
void TriColor4fRendermode( float r, float g, float b, float a, int rendermode );
void PIC_DrawGeneric( float x, float y, float width, float height, const wrect_t *prc );
void pfnPIC_Set( HIMAGE hPic, int r, int g, int b, int a );
void pfnFillRGBA_menu( int x, int y, int width, int height, int r, int g, int b, int a );
void pfnDrawCharacterQ( int ix, int iy, int iwidth, int iheight, int ch, int ulRGBA, HIMAGE hFont );
void GL_LoadTexMatrix( const matrix4x4 m );
void GL_LoadTexMatrixExt( const float *glmatrix );
void GL_LoadMatrix( const matrix4x4 source );
void GL_LoadIdentityTexMatrix( void );
void GL_CleanUpTextureUnits( int last );
void GL_MultiTexCoord2f( GLenum texture, GLfloat s, GLfloat t );
void GL_TextureTarget( uint target );
void GL_TexGen( GLenum coord, GLenum mode );
void GL_SetTexCoordArrayMode( GLenum mode );
qboolean VID_ScreenShot( const char *filename, int shot_type );
qboolean VID_CubemapShot( const char *base, uint size, const float *vieworg, qboolean skyshot );
void CL_DrawBeam( BEAM *pbeam );
void GL_GenerateMipmaps( byte *buffer, rgbdata_t *pic, gltexture_t *tex, GLenum glTarget, GLenum inFormat, int side, qboolean subImage );
void GL_UploadTexture( rgbdata_t *pic, gltexture_t *tex, qboolean subImage, imgfilter_t *filter );
void R_ShutdownImages( void );
void R_BeginDrawMirror( msurface_t *fa );
void R_EndDrawMirror( void );
void R_Clear( int bitMask );
void R_LoadIdentity( void );
void R_RotateForEntity( cl_entity_t *e );
void R_TranslateForEntity( cl_entity_t *e );
void R_SetupGL( void );
void R_EndGL( void );
void R_DrawFog( void );
void R_DrawEntitiesOnList( void );
void R_BeginFrame( qboolean clearScene );
void R_RenderFrame( const ref_params_t *fd, qboolean drawWorld );
void R_DrawTextureChains( void );
void R_DrawBrushModel( cl_entity_t *e );
void R_DrawTriangleOutlines( void );
void R_StudioRenderShadow( int iSprite, float *p1, float *p2, float *p3, float *p4 );
void R_StudioDrawHulls( void );
void R_StudioDrawAbsBBox( void );
void R_StudioDrawBones( void );
void R_StudioDrawAttachments( void );
void R_StudioSetupRenderer( int rendermode );
void R_StudioRestoreRenderer( void );
void R_StudioDrawPlanarShadow( void );
void GL_StudioDrawShadow( void );
void R_DrawViewModel( void );
void GL_SetDefaults( void );
void GL_InitExtensions( void );
void GL_CheckForErrors_( const char *filename, const int fileline );
int Con_DrawGenericChar( int x, int y, int number, rgba_t color );
void Field_DrawInputLine( int x, int y, field_t *edit );
void Con_DrawNotify( void );
void Con_DrawSolidConsole( float frac );

#ifdef __cplusplus
}
#endif

#endif // C_GATE_HPP
