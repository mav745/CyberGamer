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
#include "server.h"
#include "cl_tent.h"
#include "gl_local.h"
#include "shake.h"
#include "triangleapi.h"
#include "r_studioint.h"
#include "keydefs.h"

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

qboolean __cdecl CL_TracerComputeVerts( const vec3_t start, const vec3_t delta, float width, vec3_t *pVerts );

typedef struct
{
	vec3_t	pos;
	vec3_t	color;
	float	texcoord; // Y texture coordinate
	float	width;
	float	alpha;
} beamseg_t;

void __cdecl ComputeBeamPerpendicular( const vec3_t vecBeamDelta, vec3_t pPerp );
void __cdecl ComputeNormal( const vec3_t vStartPos, const vec3_t vNextPos, vec3_t pNormal );

extern float rgNoise[NOISE_DIVISIONS+1]; // global noise array
void __cdecl FracNoise( float *noise, int divs, float scale );

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

void __cdecl LM_InitBlock( void );
int __cdecl LM_AllocBlock( int w, int h, int *x, int *y );
void __cdecl R_BuildLightMap( msurface_t *surf, byte *dest, int stride, qboolean dynamic );
texture_t *__cdecl R_TextureAnimation( texture_t *base, int surfacenum );
void __cdecl R_SetCacheState( msurface_t *surf );
qboolean __cdecl R_SpriteOccluded( cl_entity_t *e, vec3_t origin, int *alpha, float *pscale );
qboolean __cdecl R_SpriteHasLightmap( cl_entity_t *e, int texFormat );

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

float __cdecl R_GetSpriteFrameInterpolant( cl_entity_t *ent, mspriteframe_t **oldframe, mspriteframe_t **curframe );
void __cdecl R_StudioSetupTextureHeader( void );
void __cdecl R_StudioSetupChrome( float *pchrome, int bone, vec3_t normal );

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

void __cdecl R_StudioLighting( float *lv, int bone, int flags, vec3_t normal );
int __cdecl R_StudioMeshCompare(const void *a, const void *b );


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

void __cdecl GL_LoadMatrix( const matrix4x4 source );
void __cdecl GL_SetTextureType( GLenum texnum, GLenum type );
mspriteframe_t *__cdecl R_GetSpriteFrame( const model_t *pModel, int frame, float yaw );
int __cdecl GL_LoadTextureInternal( const char *name, rgbdata_t *pic, texFlags_t flags, qboolean update );
qboolean __cdecl GL_Support( int r_ext );
byte *__cdecl GL_ResampleTexture( const byte *source, int inWidth, int inHeight, int outWidth, int outHeight, qboolean isNormalMap );
void __cdecl GL_ResetFogColor( void );
void __cdecl GL_SetupFogColorForSurfaces( void );
void __cdecl GL_ResetFogColor( void );
void __cdecl R_BeginDrawMirror( msurface_t *fa );
void __cdecl R_EndDrawMirror( void );
void __cdecl R_LightForPoint( const vec3_t point, color24 *ambientLight, qboolean invLight, qboolean useAmbient, float radius );
void __cdecl R_LoadIdentity( void );



void __cdecl GL_BackendStartFrame( void );
void __cdecl GL_BackendEndFrame( void );
void __cdecl GL_CleanUpTextureUnits( int last );
//void __cdecl GL_Bind( GLint tmu, GLenum texnum );
void __cdecl GL_MultiTexCoord2f( GLenum texture, GLfloat s, GLfloat t );
void __cdecl GL_SetTexCoordArrayMode( GLenum mode );
void __cdecl GL_LoadTexMatrix( const matrix4x4 m );
void __cdecl GL_LoadTexMatrixExt( const float *glmatrix );
//void __cdecl GL_LoadMatrix( const matrix4x4 source );
void __cdecl GL_TexGen( GLenum coord, GLenum mode );
//void __cdecl GL_SelectTexture( GLint texture );
void __cdecl GL_LoadIdentityTexMatrix( void );
void __cdecl GL_DisableAllTexGens( void );
//void __cdecl GL_SetRenderMode( int mode );
//void __cdecl GL_FrontFace( GLenum front );
void __cdecl GL_TextureTarget( uint target );
//void __cdecl GL_Cull( GLenum cull );
//void __cdecl R_ShowTextures( void );

//
// gl_cull.c
//
int __cdecl R_CullModel( cl_entity_t *e, vec3_t origin, vec3_t mins, vec3_t maxs, float radius );
//qboolean __cdecl R_CullBox( const vec3_t mins, const vec3_t maxs, uint clipflags );
qboolean __cdecl R_CullSphere( const vec3_t centre, const float radius, const uint clipflags );
qboolean __cdecl R_CullSurface( msurface_t *surf, uint clipflags );

//
// gl_decals.c
//
//void __cdecl DrawSurfaceDecals( msurface_t *fa );
float *__cdecl R_DecalSetupVerts( decal_t *pDecal, msurface_t *surf, int texture, int *outCount );
//void __cdecl DrawSingleDecal( decal_t *pDecal, msurface_t *fa );
void __cdecl R_EntityRemoveDecals( model_t *mod );
void __cdecl R_ClearDecals( void );

//
// gl_draw.c
//
//void __cdecl R_Set2DMode( qboolean enable );
//void __cdecl R_DrawTileClear( int x, int y, int w, int h );
//void __cdecl R_UploadStretchRaw( int texture, int cols, int rows, int width, int height, const byte *data );

//
// gl_image.c
//
//void __cdecl R_SetTextureParameters( void );
gltexture_t *__cdecl R_GetTexture( GLenum texnum );
//void __cdecl GL_SetTextureType( GLenum texnum, GLenum type );
//int __cdecl GL_LoadTexture( const char *name, const byte *buf, size_t size, int flags, imgfilter_t *filter );
//int __cdecl GL_LoadTextureInternal( const char *name, rgbdata_t *pic, texFlags_t flags, qboolean update );
//byte *__cdecl GL_ResampleTexture( const byte *source, int in_w, int in_h, int out_w, int out_h, qboolean isNormalMap );
int __cdecl GL_CreateTexture( const char *name, int width, int height, const void *buffer, texFlags_t flags );
void __cdecl GL_ProcessTexture( int texnum, float gamma, int topColor, int bottomColor );
//void __cdecl GL_TexFilter( gltexture_t *tex, qboolean update );
void __cdecl R_FreeImage( gltexture_t *image );
int __cdecl GL_FindTexture( const char *name );
void __cdecl GL_FreeTexture( GLenum texnum );
void __cdecl GL_FreeImage( const char *name );
const char *__cdecl GL_Target( GLenum target );
void __cdecl R_TextureList_f( void );
void __cdecl R_InitImages( void );
void __cdecl R_ShutdownImages( void );

//
// gl_mirror.c
//
//void __cdecl R_BeginDrawMirror( msurface_t *fa );
//void __cdecl R_EndDrawMirror( void );
void __cdecl R_DrawMirrors( void );
void __cdecl R_FindMirrors( const ref_params_t *fd );

//
// gl_refrag.c
//
void __cdecl R_StoreEfrags( efrag_t **ppefrag, int framecount );

//
// gl_rlight.c
//
void __cdecl R_PushDlights( void );
void __cdecl R_AnimateLight( void );
void __cdecl R_GetLightSpot( vec3_t lightspot );
void __cdecl R_MarkLights( dlight_t *light, int bit, mnode_t *node );
void __cdecl R_LightDir( const vec3_t origin, vec3_t lightDir, float radius );
//void __cdecl R_LightForPoint( const vec3_t point, color24 *ambientLight, qboolean invLight, qboolean useAmbient, float radius );
int __cdecl R_CountSurfaceDlights( msurface_t *surf );
int __cdecl R_CountDlights( void );

//
// gl_rmain.c
//
void __cdecl R_ClearScene( void );
//void __cdecl R_LoadIdentity( void );
void __cdecl R_RenderScene( const ref_params_t *fd );
void __cdecl R_DrawCubemapView( const vec3_t origin, const vec3_t angles, int size );
void __cdecl R_TranslateForEntity( cl_entity_t *e );
void __cdecl R_RotateForEntity( cl_entity_t *e );
int __cdecl R_ComputeFxBlend( cl_entity_t *e );
qboolean __cdecl R_InitRenderAPI( void );
void __cdecl R_SetupFrustum( void );
void __cdecl R_FindViewLeaf( void );
void __cdecl R_DrawFog( void );

//
// gl_rmath.c
//
float __cdecl V_CalcFov( float *fov_x, float width, float height );
void __cdecl V_AdjustFov( float *fov_x, float *fov_y, float width, float height, qboolean lock_x );
void __cdecl Matrix4x4_ToArrayFloatGL( const matrix4x4 in, float out[16] );
void __cdecl Matrix4x4_FromArrayFloatGL( matrix4x4 out, const float in[16] );
void __cdecl Matrix4x4_Concat( matrix4x4 out, const matrix4x4 in1, const matrix4x4 in2 );
void __cdecl Matrix4x4_ConcatTranslate( matrix4x4 out, float x, float y, float z );
void __cdecl Matrix4x4_ConcatRotate( matrix4x4 out, float angle, float x, float y, float z );
void __cdecl Matrix4x4_ConcatScale( matrix4x4 out, float x );
void __cdecl Matrix4x4_ConcatScale3( matrix4x4 out, float x, float y, float z );
void __cdecl Matrix4x4_CreateTranslate( matrix4x4 out, float x, float y, float z );
void __cdecl Matrix4x4_CreateRotate( matrix4x4 out, float angle, float x, float y, float z );
void __cdecl Matrix4x4_CreateScale( matrix4x4 out, float x );
void __cdecl Matrix4x4_CreateScale3( matrix4x4 out, float x, float y, float z );
void __cdecl Matrix4x4_CreateProjection(matrix4x4 out, float xMax, float xMin, float yMax, float yMin, float zNear, float zFar);
void __cdecl Matrix4x4_CreateOrtho(matrix4x4 m, float xLeft, float xRight, float yBottom, float yTop, float zNear, float zFar);
void __cdecl Matrix4x4_CreateModelview( matrix4x4 out );

//
// gl_rmisc.
//
void __cdecl R_ParseTexFilters( const char *filename );
imgfilter_t *__cdecl R_FindTexFilter( const char *texname );

//
// gl_rsurf.c
//
void __cdecl R_MarkLeaves( void );
void __cdecl R_DrawWorld( void );
void __cdecl R_DrawMirrors( void );
//void __cdecl R_DrawWaterSurfaces( void );
void __cdecl R_DrawBrushModel( cl_entity_t *e );
void __cdecl GL_SubdivideSurface( msurface_t *fa );
void __cdecl GL_BuildPolygonFromSurface( model_t *mod, msurface_t *fa );
//void __cdecl GL_SetupFogColorForSurfaces( void );
void __cdecl GL_RebuildLightmaps( void );
void __cdecl GL_BuildLightmaps( void );
//void __cdecl GL_ResetFogColor( void );

//
// gl_sprite.c
//
void __cdecl R_SpriteInit( void );
void __cdecl Mod_LoadSpriteModel( model_t *mod, const void *buffer, qboolean *loaded, uint texFlags );
//mspriteframe_t *__cdecl R_GetSpriteFrame( const model_t *pModel, int frame, float yaw );
//void __cdecl R_DrawSpriteModel( cl_entity_t *e );

//
// gl_studio.c
//
void __cdecl R_StudioInit( void );
void __cdecl Mod_LoadStudioModel( model_t *mod, const void *buffer, qboolean *loaded );
struct mstudiotex_s *__cdecl R_StudioGetTexture( cl_entity_t *e );
void __cdecl R_DrawStudioModel( cl_entity_t *e );

//
// gl_warp.c
//
void __cdecl R_InitSky( struct mip_s *mt, struct texture_s *tx );
void __cdecl R_AddSkyBoxSurface( msurface_t *fa );
void __cdecl R_ClearSkyBox( void );
//void __cdecl R_DrawSkyBox( void );
//void __cdecl EmitSkyLayers( msurface_t *fa );
//void __cdecl EmitSkyPolys( msurface_t *fa );
//void __cdecl EmitWaterPolys( glpoly_t *polys, qboolean noCull );
//void __cdecl R_DrawSkyChain( msurface_t *s );

//
// gl_vidnt.c
//
#define GL_CheckForErrors() GL_CheckForErrors_( __FILE__, __LINE__ )
void __cdecl GL_CheckForErrors_( const char *filename, const int fileline );
//void __cdecl GL_UpdateSwapInterval( void );
//void __cdecl GL_UpdateGammaRamp( void );
qboolean __cdecl GL_DeleteContext( void );
//qboolean __cdecl GL_Support( int r_ext );
void __cdecl VID_CheckChanges( void );
//int __cdecl GL_MaxTextureUnits( void );
qboolean __cdecl R_Init( void );
void __cdecl R_Shutdown( void );

////
//// renderer exports
////
//qboolean __cdecl R_Init( void );
//void __cdecl R_Shutdown( void );
void __cdecl VID_CheckChanges( void );
int __cdecl GL_LoadTexture( const char *name, const byte *buf, size_t size, int flags, imgfilter_t *filter );
void __cdecl GL_FreeImage( const char *name );
qboolean __cdecl VID_ScreenShot( const char *filename, int shot_type );
qboolean __cdecl VID_CubemapShot( const char *base, uint size, const float *vieworg, qboolean skyshot );
//void __cdecl VID_RestoreGamma( void );
void __cdecl R_BeginFrame( qboolean clearScene );

void __cdecl R_EndFrame( void );
void __cdecl R_ClearScene( void );
void __cdecl R_GetTextureParms( int *w, int *h, int texnum );
void __cdecl R_GetSpriteParms( int *frameWidth, int *frameHeight, int *numFrames, int curFrame, const struct model_s *pSprite );
//void __cdecl R_DrawStretchRaw( float x, float y, float w, float h, int cols, int rows, const byte *data, qboolean dirty );
//void __cdecl R_DrawStretchPic( float x, float y, float w, float h, float s1, float t1, float s2, float t2, int texnum );
qboolean __cdecl R_SpeedsMessage( char *out, size_t size );
void __cdecl R_SetupSky( const char *skyboxname );
qboolean __cdecl R_CullBox( const vec3_t mins, const vec3_t maxs, uint clipflags );
qboolean __cdecl R_WorldToScreen( const vec3_t point, vec3_t screen );
void __cdecl R_ScreenToWorld( const vec3_t screen, vec3_t point );
qboolean __cdecl R_AddEntity( struct cl_entity_s *pRefEntity, int entityType );
void __cdecl Mod_LoadMapSprite( struct model_s *mod, const void *buffer, size_t size, qboolean *loaded );
void __cdecl Mod_UnloadSpriteModel( struct model_s *mod );
void __cdecl Mod_UnloadStudioModel( struct model_s *mod );
void __cdecl Mod_UnloadBrushModel( struct model_s *mod );
//void __cdecl GL_SetRenderMode( int mode );
void __cdecl R_RunViewmodelEvents( void );
void __cdecl R_DrawViewModel( void );
int __cdecl R_GetSpriteTexture( const struct model_s *m_pSpriteModel, int frame );
void __cdecl R_DecalShoot( int textureIndex, int entityIndex, int modelIndex, vec3_t pos, int flags, vec3_t saxis, float scale );
void __cdecl R_RemoveEfrags( struct cl_entity_s *ent );
void __cdecl R_AddEfrags( struct cl_entity_s *ent );
void __cdecl R_DecalRemoveAll( int texture );
byte *__cdecl Mod_GetCurrentVis( void );
void __cdecl Mod_SetOrthoBounds( float *mins, float *maxs );
void __cdecl R_NewMap( void );

typedef struct
{
	string		buffer;
	int		cursor;
	int		scroll;
	int		widthInChars;
} field_t;

qboolean __cdecl SPR_Scissor( float *x, float *y, float *width, float *height, float *u0, float *v0, float *u1, float *v1 );
void __cdecl SPR_AdjustSize( float *x, float *y, float *w, float *h );
const model_t *__cdecl CL_GetSpritePointer( VHSPRITE hSprite );
qboolean __cdecl PIC_Scissor( float *x, float *y, float *width, float *height, float *u0, float *v0, float *u1, float *v1 );
void __cdecl VID_ImageAdjustGamma( byte *in, uint width, uint height );
cl_entity_t *__cdecl CL_GetBeamEntityByIndex( int index );
void __cdecl GL_RoundImageDimensions( word *width, word *height, texFlags_t flags, qboolean force );
void __cdecl GL_MakeLuminance( rgbdata_t *in );
int __cdecl GL_CalcTextureSamples( int flags );
void __cdecl CL_DrawBeamFollow( int spriteIndex, BEAM *pbeam, int frame, int rendermode, float frametime, float *color );
void __cdecl GL_BuildMipMap( byte *in, int width, int height, qboolean isNormalMap );
GLenum __cdecl GL_TextureFormat( gltexture_t *tex, int *samples );
byte *__cdecl GL_ApplyGamma( const byte *source, int pixels, qboolean isNormalMap );
qboolean __cdecl R_StaticEntity( cl_entity_t *ent );
int __cdecl R_SurfaceCompare(const void *a, const void *b );
void __cdecl R_StudioDeformShadow( void );
void __cdecl R_SetupModelviewMatrix( const ref_params_t *fd, matrix4x4 m );
void __cdecl R_SetupProjectionMatrix( const ref_params_t *fd, matrix4x4 m );
qboolean __cdecl R_StudioComputeBBox( cl_entity_t *e, vec3_t bbox[8] );
void __cdecl GL_SetExtension( int r_ext, int enable );
int __cdecl Con_DrawGenericString( int x, int y, const char *string, rgba_t setColor, qboolean forceColor, int hideChar );
void __cdecl Con_DrawInput( void );
int __cdecl Con_StringLength( const char *string );
void __cdecl IN_ActivateMouse( qboolean force );
void __cdecl IN_MouseEvent( int mstate );
int __cdecl Host_MapKey( int key );
void __cdecl IN_ActivateCursor( void );

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
void __cdecl initWindow(int width, int height, int fullscreen);
void __cdecl killWindow();
void __cdecl updateWindow();
void __cdecl raiseWindow();
int __cdecl mouseInWindow();
//void* getWindowHandle();

GLuint __cdecl makeTexture();
int __cdecl GL_MaxTextureUnits();
void __cdecl GL_SelectTexture( GLint tmu );
void __cdecl GL_Bind( GLint tmu, GLenum texnum );
void __cdecl GL_TexFilter( gltexture_t *tex, qboolean update );
void __cdecl R_SetTextureParameters();
void __cdecl GL_Cull( GLenum cull );
void __cdecl GL_FrontFace( GLenum front );
void __cdecl GL_SetRenderMode( int mode );
void __cdecl R_DrawStretchPic( float x, float y, float w, float h, float s1, float t1, float s2, float t2, int texnum );
void __cdecl R_DrawTileClear( int x, int y, int w, int h );
void __cdecl R_DrawStretchRaw( float x, float y, float w, float h, int cols, int rows, const byte *data, qboolean dirty );
void __cdecl R_UploadStretchRaw( int texture, int cols, int rows, int width, int height, const byte *data );
void __cdecl R_Set2DMode( qboolean enable );
int __cdecl R_AllocateMirrorTexture();
void __cdecl CL_BulletTracerDraw( particle_t *p, float frametime );
void __cdecl CL_UpdateParticle( particle_t *p, float ft );
void __cdecl CL_DrawTracer( vec3_t start, vec3_t delta, float width, rgb_t color, int alpha, float startV, float endV );
void __cdecl CL_DrawSegs( int modelIndex, float frame, int rendermode, const vec3_t source, const vec3_t delta,
	float width, float scale, float freq, float speed, int segments, int flags, float *color );
void __cdecl CL_DrawDisk( int modelIndex, float frame, int rendermode, const vec3_t source, const vec3_t delta,
	float width, float scale, float freq, float speed, int segments, float *color );
void __cdecl CL_DrawCylinder( int modelIndex, float frame, int rendermode, const vec3_t source, const vec3_t delta,
	float width, float scale, float freq, float speed, int segments, float *color );
void __cdecl CL_DrawRing( int modelIndex, float frame, int rendermode, const vec3_t source, const vec3_t delta, float width,
	float amplitude, float freq, float speed, int segments, float *color );
void __cdecl CL_DrawLaser( BEAM *pbeam, int frame, int rendermode, float *color, int spriteIndex );
void __cdecl DrawBeamFollow( int modelIndex, particle_t *pHead, int frame, int rendermode, vec3_t delta,
	vec3_t screen, vec3_t screenLast, float die, const vec3_t source, int flags, float width,
	float amplitude, float freq, float *color );
void __cdecl DrawGLPoly( glpoly_t *p, float xScale, float yScale );
void __cdecl DrawGLPolyChain( glpoly_t *p, float soffset, float toffset );
void __cdecl R_BlendLightmaps();
void __cdecl R_RenderFullbrights();
void __cdecl R_RenderDetails();
void __cdecl R_RenderBrushPoly( msurface_t *fa );
void __cdecl R_DrawSpriteModel( cl_entity_t *e );
void __cdecl R_ShowTextures();
int __cdecl TriSpriteTexture( model_t *pSpriteModel, int frame );
void __cdecl DrawSingleDecal( decal_t *pDecal, msurface_t *fa );
void __cdecl DrawSurfaceDecals( msurface_t *fa );
void __cdecl R_StudioDrawPoints(void);
void __cdecl R_StudioSetupSkin( mstudiotexture_t *ptexture, int index );
void __cdecl R_DrawWaterSurfaces();
void __cdecl LM_UploadBlock( qboolean dynamic );
void __cdecl R_DrawSkyBox();
void __cdecl EmitWaterPolys( glpoly_t *polys, qboolean noCull );
void __cdecl EmitSkyPolys( msurface_t *fa );
void __cdecl R_DrawSkyChain( msurface_t *s );
void __cdecl EmitSkyLayers( msurface_t *fa );
void __cdecl VGUI_EnableTexture( qboolean enable );
void __cdecl VGUI_DrawQuad( const vpoint_t *ul, const vpoint_t *lr );
void __cdecl VGUI_UploadTextureBlock( int id, int drawX, int drawY, const byte *rgba, int blockWidth, int blockHeight );
void __cdecl VGUI_SetupDrawingRect( int *pColor );
void __cdecl VGUI_SetupDrawingText( int *pColor );
void __cdecl VGUI_SetupDrawingImage( int *pColor );
void __cdecl VGUI_BindTexture( int id );
void __cdecl SetBeamRenderMode( int rendermode );
void __cdecl R_DrawSpriteQuad( mspriteframe_t *frame, vec3_t org, vec3_t v_right, vec3_t v_up, float scale );
void __cdecl MakeSkyVec( float s, float t, int axis );

void __cdecl SPR_DrawGeneric( int frame, float x, float y, float width, float height, const wrect_t *prc );
void __cdecl CL_DrawScreenFade( void );
void __cdecl CL_DrawLoading( float percent );
void __cdecl CL_DrawPause( void );
void __cdecl pfnSPR_Set( VHSPRITE hPic, int r, int g, int b );
void __cdecl pfnSPR_Draw( int frame, int x, int y, const wrect_t *prc );
void __cdecl pfnFillRGBA( int x, int y, int width, int height, int r, int g, int b, int a );
void __cdecl pfnSPR_DrawGeneric( int frame, int x, int y, const wrect_t *prc, int blendsrc, int blenddst, int width, int height );
void __cdecl pfnFillRGBABlend( int x, int y, int width, int height, int r, int g, int b, int a );
void __cdecl TriRenderMode( int mode );
void __cdecl TriBegin( int mode );
void __cdecl TriEnd( void );
void __cdecl TriColor4f( float r, float g, float b, float a );
void __cdecl TriColor4ub( byte r, byte g, byte b, byte a );
void __cdecl TriTexCoord2f( float u, float v );
void __cdecl TriVertex3fv( const float *v );
void __cdecl TriVertex3f( float x, float y, float z );
void __cdecl TriBrightness( float brightness );
void __cdecl TriFog( float flFogColor[3], float flStart, float flEnd, int bOn );
void __cdecl TriGetMatrix( const int pname, float *matrix );
void __cdecl TriColor4fRendermode( float r, float g, float b, float a, int rendermode );
void __cdecl PIC_DrawGeneric( float x, float y, float width, float height, const wrect_t *prc );
void __cdecl pfnPIC_Set( HIMAGE hPic, int r, int g, int b, int a );
void __cdecl pfnFillRGBA_menu( int x, int y, int width, int height, int r, int g, int b, int a );
void __cdecl pfnDrawCharacterQ( int ix, int iy, int iwidth, int iheight, int ch, int ulRGBA, HIMAGE hFont );
void __cdecl GL_LoadTexMatrix( const matrix4x4 m );
void __cdecl GL_LoadTexMatrixExt( const float *glmatrix );
void __cdecl GL_LoadMatrix( const matrix4x4 source );
void __cdecl GL_LoadIdentityTexMatrix( void );
void __cdecl GL_CleanUpTextureUnits( int last );
void __cdecl GL_MultiTexCoord2f( GLenum texture, GLfloat s, GLfloat t );
void __cdecl GL_TextureTarget( uint target );
void __cdecl GL_TexGen( GLenum coord, GLenum mode );
void __cdecl GL_SetTexCoordArrayMode( GLenum mode );
qboolean __cdecl VID_ScreenShot( const char *filename, int shot_type );
qboolean __cdecl VID_CubemapShot( const char *base, uint size, const float *vieworg, qboolean skyshot );
void __cdecl CL_DrawBeam( BEAM *pbeam );
void __cdecl GL_GenerateMipmaps( byte *buffer, rgbdata_t *pic, gltexture_t *tex, GLenum glTarget, GLenum inFormat, int side, qboolean subImage );
void __cdecl GL_UploadTexture( rgbdata_t *pic, gltexture_t *tex, qboolean subImage, imgfilter_t *filter );
void __cdecl R_ShutdownImages( void );
void __cdecl R_BeginDrawMirror( msurface_t *fa );
void __cdecl R_EndDrawMirror( void );
void __cdecl R_Clear( int bitMask );
void __cdecl R_LoadIdentity( void );
void __cdecl R_RotateForEntity( cl_entity_t *e );
void __cdecl R_TranslateForEntity( cl_entity_t *e );
void __cdecl R_SetupGL( void );
void __cdecl R_EndGL( void );
void __cdecl R_DrawFog( void );
void __cdecl R_DrawEntitiesOnList( void );
void __cdecl R_BeginFrame( qboolean clearScene );
void __cdecl R_RenderFrame( const ref_params_t *fd, qboolean drawWorld );
void __cdecl R_DrawTextureChains( void );
void __cdecl R_DrawBrushModel( cl_entity_t *e );
void __cdecl R_DrawTriangleOutlines( void );
void __cdecl R_StudioRenderShadow( int iSprite, float *p1, float *p2, float *p3, float *p4 );
void __cdecl R_StudioDrawHulls( void );
void __cdecl R_StudioDrawAbsBBox( void );
void __cdecl R_StudioDrawBones( void );
void __cdecl R_StudioDrawAttachments( void );
void __cdecl R_StudioSetupRenderer( int rendermode );
void __cdecl R_StudioRestoreRenderer( void );
void __cdecl R_StudioDrawPlanarShadow( void );
void __cdecl GL_StudioDrawShadow( void );
void __cdecl R_DrawViewModel( void );
void __cdecl GL_SetDefaults( void );
void __cdecl GL_InitExtensions( void );
void __cdecl GL_CheckForErrors_( const char *filename, const int fileline );
int __cdecl Con_DrawGenericChar( int x, int y, int number, rgba_t color );
void __cdecl Field_DrawInputLine( int x, int y, field_t *edit );
void __cdecl Con_DrawNotify( void );
void __cdecl Con_DrawSolidConsole( float frac );

#ifdef __cplusplus
}
#endif

#endif // C_GATE_HPP
