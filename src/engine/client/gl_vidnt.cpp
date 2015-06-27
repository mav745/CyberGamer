/*
gl_vidnt.c - NT GL vid component
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

#include "common.h"
#include "client.h"
#include "gl_local.h"
#include "mod_local.h"
#include "input.h"


// helper opengl functions
GLenum ( APIENTRY *pglGetError )(void);
const GLubyte * ( APIENTRY *pglGetString )(GLenum name);

// base gl functions
void ( APIENTRY *pglAccum )(GLenum op, GLfloat value);
void ( APIENTRY *pglAlphaFunc )(GLenum func, GLclampf ref);
void ( APIENTRY *pglArrayElement )(GLint i);
void ( APIENTRY *pglBegin )(GLenum mode);
void ( APIENTRY *pglBindTexture )(GLenum target, GLuint texture);
void ( APIENTRY *pglBitmap )(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
void ( APIENTRY *pglBlendFunc )(GLenum sfactor, GLenum dfactor);
void ( APIENTRY *pglCallList )(GLuint list);
void ( APIENTRY *pglCallLists )(GLsizei n, GLenum type, const GLvoid *lists);
void ( APIENTRY *pglClear )(GLbitfield mask);
void ( APIENTRY *pglClearAccum )(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void ( APIENTRY *pglClearColor )(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void ( APIENTRY *pglClearDepth )(GLclampd depth);
void ( APIENTRY *pglClearIndex )(GLfloat c);
void ( APIENTRY *pglClearStencil )(GLint s);
GLboolean ( APIENTRY *pglIsEnabled )( GLenum cap );
GLboolean ( APIENTRY *pglIsList )( GLuint list );
GLboolean ( APIENTRY *pglIsTexture )( GLuint texture );
void ( APIENTRY *pglClipPlane )(GLenum plane, const GLdouble *equation);
void ( APIENTRY *pglColor3b )(GLbyte red, GLbyte green, GLbyte blue);
void ( APIENTRY *pglColor3bv )(const GLbyte *v);
void ( APIENTRY *pglColor3d )(GLdouble red, GLdouble green, GLdouble blue);
void ( APIENTRY *pglColor3dv )(const GLdouble *v);
void ( APIENTRY *pglColor3f )(GLfloat red, GLfloat green, GLfloat blue);
void ( APIENTRY *pglColor3fv )(const GLfloat *v);
void ( APIENTRY *pglColor3i )(GLint red, GLint green, GLint blue);
void ( APIENTRY *pglColor3iv )(const GLint *v);
void ( APIENTRY *pglColor3s )(GLshort red, GLshort green, GLshort blue);
void ( APIENTRY *pglColor3sv )(const GLshort *v);
void ( APIENTRY *pglColor3ub )(GLubyte red, GLubyte green, GLubyte blue);
void ( APIENTRY *pglColor3ubv )(const GLubyte *v);
void ( APIENTRY *pglColor3ui )(GLuint red, GLuint green, GLuint blue);
void ( APIENTRY *pglColor3uiv )(const GLuint *v);
void ( APIENTRY *pglColor3us )(GLushort red, GLushort green, GLushort blue);
void ( APIENTRY *pglColor3usv )(const GLushort *v);
void ( APIENTRY *pglColor4b )(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
void ( APIENTRY *pglColor4bv )(const GLbyte *v);
void ( APIENTRY *pglColor4d )(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
void ( APIENTRY *pglColor4dv )(const GLdouble *v);
void ( APIENTRY *pglColor4f )(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void ( APIENTRY *pglColor4fv )(const GLfloat *v);
void ( APIENTRY *pglColor4i )(GLint red, GLint green, GLint blue, GLint alpha);
void ( APIENTRY *pglColor4iv )(const GLint *v);
void ( APIENTRY *pglColor4s )(GLshort red, GLshort green, GLshort blue, GLshort alpha);
void ( APIENTRY *pglColor4sv )(const GLshort *v);
void ( APIENTRY *pglColor4ub )(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
void ( APIENTRY *pglColor4ubv )(const GLubyte *v);
void ( APIENTRY *pglColor4ui )(GLuint red, GLuint green, GLuint blue, GLuint alpha);
void ( APIENTRY *pglColor4uiv )(const GLuint *v);
void ( APIENTRY *pglColor4us )(GLushort red, GLushort green, GLushort blue, GLushort alpha);
void ( APIENTRY *pglColor4usv )(const GLushort *v);
void ( APIENTRY *pglColorMask )(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void ( APIENTRY *pglColorMaterial )(GLenum face, GLenum mode);
void ( APIENTRY *pglColorPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void ( APIENTRY *pglCopyPixels )(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
void ( APIENTRY *pglCopyTexImage1D )(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
void ( APIENTRY *pglCopyTexImage2D )(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void ( APIENTRY *pglCopyTexSubImage1D )(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void ( APIENTRY *pglCopyTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void ( APIENTRY *pglCullFace )(GLenum mode);
void ( APIENTRY *pglDeleteLists )(GLuint list, GLsizei range);
void ( APIENTRY *pglDeleteTextures )(GLsizei n, const GLuint *textures);
void ( APIENTRY *pglDepthFunc )(GLenum func);
void ( APIENTRY *pglDepthMask )(GLboolean flag);
void ( APIENTRY *pglDepthRange )(GLclampd zNear, GLclampd zFar);
void ( APIENTRY *pglDisable )(GLenum cap);
void ( APIENTRY *pglDisableClientState )(GLenum array);
void ( APIENTRY *pglDrawArrays )(GLenum mode, GLint first, GLsizei count);
void ( APIENTRY *pglDrawBuffer )(GLenum mode);
void ( APIENTRY *pglDrawElements )(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void ( APIENTRY *pglDrawPixels )(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void ( APIENTRY *pglEdgeFlag )(GLboolean flag);
void ( APIENTRY *pglEdgeFlagPointer )(GLsizei stride, const GLvoid *pointer);
void ( APIENTRY *pglEdgeFlagv )(const GLboolean *flag);
void ( APIENTRY *pglEnable )(GLenum cap);
void ( APIENTRY *pglEnableClientState )(GLenum array);
void ( APIENTRY *pglEnd )(void);
void ( APIENTRY *pglEndList )(void);
void ( APIENTRY *pglEvalCoord1d )(GLdouble u);
void ( APIENTRY *pglEvalCoord1dv )(const GLdouble *u);
void ( APIENTRY *pglEvalCoord1f )(GLfloat u);
void ( APIENTRY *pglEvalCoord1fv )(const GLfloat *u);
void ( APIENTRY *pglEvalCoord2d )(GLdouble u, GLdouble v);
void ( APIENTRY *pglEvalCoord2dv )(const GLdouble *u);
void ( APIENTRY *pglEvalCoord2f )(GLfloat u, GLfloat v);
void ( APIENTRY *pglEvalCoord2fv )(const GLfloat *u);
void ( APIENTRY *pglEvalMesh1 )(GLenum mode, GLint i1, GLint i2);
void ( APIENTRY *pglEvalMesh2 )(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
void ( APIENTRY *pglEvalPoint1 )(GLint i);
void ( APIENTRY *pglEvalPoint2 )(GLint i, GLint j);
void ( APIENTRY *pglFeedbackBuffer )(GLsizei size, GLenum type, GLfloat *buffer);
void ( APIENTRY *pglFinish )(void);
void ( APIENTRY *pglFlush )(void);
void ( APIENTRY *pglFogf )(GLenum pname, GLfloat param);
void ( APIENTRY *pglFogfv )(GLenum pname, const GLfloat *params);
void ( APIENTRY *pglFogi )(GLenum pname, GLint param);
void ( APIENTRY *pglFogiv )(GLenum pname, const GLint *params);
void ( APIENTRY *pglFrontFace )(GLenum mode);
void ( APIENTRY *pglFrustum )(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void ( APIENTRY *pglGenTextures )(GLsizei n, GLuint *textures);
void ( APIENTRY *pglGetBooleanv )(GLenum pname, GLboolean *params);
void ( APIENTRY *pglGetClipPlane )(GLenum plane, GLdouble *equation);
void ( APIENTRY *pglGetDoublev )(GLenum pname, GLdouble *params);
void ( APIENTRY *pglGetFloatv )(GLenum pname, GLfloat *params);
void ( APIENTRY *pglGetIntegerv )(GLenum pname, GLint *params);
void ( APIENTRY *pglGetLightfv )(GLenum light, GLenum pname, GLfloat *params);
void ( APIENTRY *pglGetLightiv )(GLenum light, GLenum pname, GLint *params);
void ( APIENTRY *pglGetMapdv )(GLenum target, GLenum query, GLdouble *v);
void ( APIENTRY *pglGetMapfv )(GLenum target, GLenum query, GLfloat *v);
void ( APIENTRY *pglGetMapiv )(GLenum target, GLenum query, GLint *v);
void ( APIENTRY *pglGetMaterialfv )(GLenum face, GLenum pname, GLfloat *params);
void ( APIENTRY *pglGetMaterialiv )(GLenum face, GLenum pname, GLint *params);
void ( APIENTRY *pglGetPixelMapfv )(GLenum map, GLfloat *values);
void ( APIENTRY *pglGetPixelMapuiv )(GLenum map, GLuint *values);
void ( APIENTRY *pglGetPixelMapusv )(GLenum map, GLushort *values);
void ( APIENTRY *pglGetPointerv )(GLenum pname, GLvoid* *params);
void ( APIENTRY *pglGetPolygonStipple )(GLubyte *mask);
void ( APIENTRY *pglGetTexEnvfv )(GLenum target, GLenum pname, GLfloat *params);
void ( APIENTRY *pglGetTexEnviv )(GLenum target, GLenum pname, GLint *params);
void ( APIENTRY *pglGetTexGendv )(GLenum coord, GLenum pname, GLdouble *params);
void ( APIENTRY *pglGetTexGenfv )(GLenum coord, GLenum pname, GLfloat *params);
void ( APIENTRY *pglGetTexGeniv )(GLenum coord, GLenum pname, GLint *params);
void ( APIENTRY *pglGetTexImage )(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
void ( APIENTRY *pglGetTexLevelParameterfv )(GLenum target, GLint level, GLenum pname, GLfloat *params);
void ( APIENTRY *pglGetTexLevelParameteriv )(GLenum target, GLint level, GLenum pname, GLint *params);
void ( APIENTRY *pglGetTexParameterfv )(GLenum target, GLenum pname, GLfloat *params);
void ( APIENTRY *pglGetTexParameteriv )(GLenum target, GLenum pname, GLint *params);
void ( APIENTRY *pglHint )(GLenum target, GLenum mode);
void ( APIENTRY *pglIndexMask )(GLuint mask);
void ( APIENTRY *pglIndexPointer )(GLenum type, GLsizei stride, const GLvoid *pointer);
void ( APIENTRY *pglIndexd )(GLdouble c);
void ( APIENTRY *pglIndexdv )(const GLdouble *c);
void ( APIENTRY *pglIndexf )(GLfloat c);
void ( APIENTRY *pglIndexfv )(const GLfloat *c);
void ( APIENTRY *pglIndexi )(GLint c);
void ( APIENTRY *pglIndexiv )(const GLint *c);
void ( APIENTRY *pglIndexs )(GLshort c);
void ( APIENTRY *pglIndexsv )(const GLshort *c);
void ( APIENTRY *pglIndexub )(GLubyte c);
void ( APIENTRY *pglIndexubv )(const GLubyte *c);
void ( APIENTRY *pglInitNames )(void);
void ( APIENTRY *pglInterleavedArrays )(GLenum format, GLsizei stride, const GLvoid *pointer);
void ( APIENTRY *pglLightModelf )(GLenum pname, GLfloat param);
void ( APIENTRY *pglLightModelfv )(GLenum pname, const GLfloat *params);
void ( APIENTRY *pglLightModeli )(GLenum pname, GLint param);
void ( APIENTRY *pglLightModeliv )(GLenum pname, const GLint *params);
void ( APIENTRY *pglLightf )(GLenum light, GLenum pname, GLfloat param);
void ( APIENTRY *pglLightfv )(GLenum light, GLenum pname, const GLfloat *params);
void ( APIENTRY *pglLighti )(GLenum light, GLenum pname, GLint param);
void ( APIENTRY *pglLightiv )(GLenum light, GLenum pname, const GLint *params);
void ( APIENTRY *pglLineStipple )(GLint factor, GLushort pattern);
void ( APIENTRY *pglLineWidth )(GLfloat width);
void ( APIENTRY *pglListBase )(GLuint base);
void ( APIENTRY *pglLoadIdentity )(void);
void ( APIENTRY *pglLoadMatrixd )(const GLdouble *m);
void ( APIENTRY *pglLoadMatrixf )(const GLfloat *m);
void ( APIENTRY *pglLoadName )(GLuint name);
void ( APIENTRY *pglLogicOp )(GLenum opcode);
void ( APIENTRY *pglMap1d )(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
void ( APIENTRY *pglMap1f )(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
void ( APIENTRY *pglMap2d )(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
void ( APIENTRY *pglMap2f )(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
void ( APIENTRY *pglMapGrid1d )(GLint un, GLdouble u1, GLdouble u2);
void ( APIENTRY *pglMapGrid1f )(GLint un, GLfloat u1, GLfloat u2);
void ( APIENTRY *pglMapGrid2d )(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
void ( APIENTRY *pglMapGrid2f )(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
void ( APIENTRY *pglMaterialf )(GLenum face, GLenum pname, GLfloat param);
void ( APIENTRY *pglMaterialfv )(GLenum face, GLenum pname, const GLfloat *params);
void ( APIENTRY *pglMateriali )(GLenum face, GLenum pname, GLint param);
void ( APIENTRY *pglMaterialiv )(GLenum face, GLenum pname, const GLint *params);
void ( APIENTRY *pglMatrixMode )(GLenum mode);
void ( APIENTRY *pglMultMatrixd )(const GLdouble *m);
void ( APIENTRY *pglMultMatrixf )(const GLfloat *m);
void ( APIENTRY *pglNewList )(GLuint list, GLenum mode);
void ( APIENTRY *pglNormal3b )(GLbyte nx, GLbyte ny, GLbyte nz);
void ( APIENTRY *pglNormal3bv )(const GLbyte *v);
void ( APIENTRY *pglNormal3d )(GLdouble nx, GLdouble ny, GLdouble nz);
void ( APIENTRY *pglNormal3dv )(const GLdouble *v);
void ( APIENTRY *pglNormal3f )(GLfloat nx, GLfloat ny, GLfloat nz);
void ( APIENTRY *pglNormal3fv )(const GLfloat *v);
void ( APIENTRY *pglNormal3i )(GLint nx, GLint ny, GLint nz);
void ( APIENTRY *pglNormal3iv )(const GLint *v);
void ( APIENTRY *pglNormal3s )(GLshort nx, GLshort ny, GLshort nz);
void ( APIENTRY *pglNormal3sv )(const GLshort *v);
void ( APIENTRY *pglNormalPointer )(GLenum type, GLsizei stride, const GLvoid *pointer);
void ( APIENTRY *pglOrtho )(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void ( APIENTRY *pglPassThrough )(GLfloat token);
void ( APIENTRY *pglPixelMapfv )(GLenum map, GLsizei mapsize, const GLfloat *values);
void ( APIENTRY *pglPixelMapuiv )(GLenum map, GLsizei mapsize, const GLuint *values);
void ( APIENTRY *pglPixelMapusv )(GLenum map, GLsizei mapsize, const GLushort *values);
void ( APIENTRY *pglPixelStoref )(GLenum pname, GLfloat param);
void ( APIENTRY *pglPixelStorei )(GLenum pname, GLint param);
void ( APIENTRY *pglPixelTransferf )(GLenum pname, GLfloat param);
void ( APIENTRY *pglPixelTransferi )(GLenum pname, GLint param);
void ( APIENTRY *pglPixelZoom )(GLfloat xfactor, GLfloat yfactor);
void ( APIENTRY *pglPointSize )(GLfloat size);
void ( APIENTRY *pglPolygonMode )(GLenum face, GLenum mode);
void ( APIENTRY *pglPolygonOffset )(GLfloat factor, GLfloat units);
void ( APIENTRY *pglPolygonStipple )(const GLubyte *mask);
void ( APIENTRY *pglPopAttrib )(void);
void ( APIENTRY *pglPopClientAttrib )(void);
void ( APIENTRY *pglPopMatrix )(void);
void ( APIENTRY *pglPopName )(void);
void ( APIENTRY *pglPushAttrib )(GLbitfield mask);
void ( APIENTRY *pglPushClientAttrib )(GLbitfield mask);
void ( APIENTRY *pglPushMatrix )(void);
void ( APIENTRY *pglPushName )(GLuint name);
void ( APIENTRY *pglRasterPos2d )(GLdouble x, GLdouble y);
void ( APIENTRY *pglRasterPos2dv )(const GLdouble *v);
void ( APIENTRY *pglRasterPos2f )(GLfloat x, GLfloat y);
void ( APIENTRY *pglRasterPos2fv )(const GLfloat *v);
void ( APIENTRY *pglRasterPos2i )(GLint x, GLint y);
void ( APIENTRY *pglRasterPos2iv )(const GLint *v);
void ( APIENTRY *pglRasterPos2s )(GLshort x, GLshort y);
void ( APIENTRY *pglRasterPos2sv )(const GLshort *v);
void ( APIENTRY *pglRasterPos3d )(GLdouble x, GLdouble y, GLdouble z);
void ( APIENTRY *pglRasterPos3dv )(const GLdouble *v);
void ( APIENTRY *pglRasterPos3f )(GLfloat x, GLfloat y, GLfloat z);
void ( APIENTRY *pglRasterPos3fv )(const GLfloat *v);
void ( APIENTRY *pglRasterPos3i )(GLint x, GLint y, GLint z);
void ( APIENTRY *pglRasterPos3iv )(const GLint *v);
void ( APIENTRY *pglRasterPos3s )(GLshort x, GLshort y, GLshort z);
void ( APIENTRY *pglRasterPos3sv )(const GLshort *v);
void ( APIENTRY *pglRasterPos4d )(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void ( APIENTRY *pglRasterPos4dv )(const GLdouble *v);
void ( APIENTRY *pglRasterPos4f )(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void ( APIENTRY *pglRasterPos4fv )(const GLfloat *v);
void ( APIENTRY *pglRasterPos4i )(GLint x, GLint y, GLint z, GLint w);
void ( APIENTRY *pglRasterPos4iv )(const GLint *v);
void ( APIENTRY *pglRasterPos4s )(GLshort x, GLshort y, GLshort z, GLshort w);
void ( APIENTRY *pglRasterPos4sv )(const GLshort *v);
void ( APIENTRY *pglReadBuffer )(GLenum mode);
void ( APIENTRY *pglReadPixels )(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
void ( APIENTRY *pglRectd )(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
void ( APIENTRY *pglRectdv )(const GLdouble *v1, const GLdouble *v2);
void ( APIENTRY *pglRectf )(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
void ( APIENTRY *pglRectfv )(const GLfloat *v1, const GLfloat *v2);
void ( APIENTRY *pglRecti )(GLint x1, GLint y1, GLint x2, GLint y2);
void ( APIENTRY *pglRectiv )(const GLint *v1, const GLint *v2);
void ( APIENTRY *pglRects )(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
void ( APIENTRY *pglRectsv )(const GLshort *v1, const GLshort *v2);
void ( APIENTRY *pglRotated )(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
void ( APIENTRY *pglRotatef )(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void ( APIENTRY *pglScaled )(GLdouble x, GLdouble y, GLdouble z);
void ( APIENTRY *pglScalef )(GLfloat x, GLfloat y, GLfloat z);
void ( APIENTRY *pglScissor )(GLint x, GLint y, GLsizei width, GLsizei height);
void ( APIENTRY *pglSelectBuffer )(GLsizei size, GLuint *buffer);
void ( APIENTRY *pglShadeModel )(GLenum mode);
void ( APIENTRY *pglStencilFunc )(GLenum func, GLint ref, GLuint mask);
void ( APIENTRY *pglStencilMask )(GLuint mask);
void ( APIENTRY *pglStencilOp )(GLenum fail, GLenum zfail, GLenum zpass);
void ( APIENTRY *pglTexCoord1d )(GLdouble s);
void ( APIENTRY *pglTexCoord1dv )(const GLdouble *v);
void ( APIENTRY *pglTexCoord1f )(GLfloat s);
void ( APIENTRY *pglTexCoord1fv )(const GLfloat *v);
void ( APIENTRY *pglTexCoord1i )(GLint s);
void ( APIENTRY *pglTexCoord1iv )(const GLint *v);
void ( APIENTRY *pglTexCoord1s )(GLshort s);
void ( APIENTRY *pglTexCoord1sv )(const GLshort *v);
void ( APIENTRY *pglTexCoord2d )(GLdouble s, GLdouble t);
void ( APIENTRY *pglTexCoord2dv )(const GLdouble *v);
void ( APIENTRY *pglTexCoord2f )(GLfloat s, GLfloat t);
void ( APIENTRY *pglTexCoord2fv )(const GLfloat *v);
void ( APIENTRY *pglTexCoord2i )(GLint s, GLint t);
void ( APIENTRY *pglTexCoord2iv )(const GLint *v);
void ( APIENTRY *pglTexCoord2s )(GLshort s, GLshort t);
void ( APIENTRY *pglTexCoord2sv )(const GLshort *v);
void ( APIENTRY *pglTexCoord3d )(GLdouble s, GLdouble t, GLdouble r);
void ( APIENTRY *pglTexCoord3dv )(const GLdouble *v);
void ( APIENTRY *pglTexCoord3f )(GLfloat s, GLfloat t, GLfloat r);
void ( APIENTRY *pglTexCoord3fv )(const GLfloat *v);
void ( APIENTRY *pglTexCoord3i )(GLint s, GLint t, GLint r);
void ( APIENTRY *pglTexCoord3iv )(const GLint *v);
void ( APIENTRY *pglTexCoord3s )(GLshort s, GLshort t, GLshort r);
void ( APIENTRY *pglTexCoord3sv )(const GLshort *v);
void ( APIENTRY *pglTexCoord4d )(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
void ( APIENTRY *pglTexCoord4dv )(const GLdouble *v);
void ( APIENTRY *pglTexCoord4f )(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
void ( APIENTRY *pglTexCoord4fv )(const GLfloat *v);
void ( APIENTRY *pglTexCoord4i )(GLint s, GLint t, GLint r, GLint q);
void ( APIENTRY *pglTexCoord4iv )(const GLint *v);
void ( APIENTRY *pglTexCoord4s )(GLshort s, GLshort t, GLshort r, GLshort q);
void ( APIENTRY *pglTexCoord4sv )(const GLshort *v);
void ( APIENTRY *pglTexCoordPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void ( APIENTRY *pglTexEnvf )(GLenum target, GLenum pname, GLfloat param);
void ( APIENTRY *pglTexEnvfv )(GLenum target, GLenum pname, const GLfloat *params);
void ( APIENTRY *pglTexEnvi )(GLenum target, GLenum pname, GLint param);
void ( APIENTRY *pglTexEnviv )(GLenum target, GLenum pname, const GLint *params);
void ( APIENTRY *pglTexGend )(GLenum coord, GLenum pname, GLdouble param);
void ( APIENTRY *pglTexGendv )(GLenum coord, GLenum pname, const GLdouble *params);
void ( APIENTRY *pglTexGenf )(GLenum coord, GLenum pname, GLfloat param);
void ( APIENTRY *pglTexGenfv )(GLenum coord, GLenum pname, const GLfloat *params);
void ( APIENTRY *pglTexGeni )(GLenum coord, GLenum pname, GLint param);
void ( APIENTRY *pglTexGeniv )(GLenum coord, GLenum pname, const GLint *params);
void ( APIENTRY *pglTexImage1D )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void ( APIENTRY *pglTexImage2D )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void ( APIENTRY *pglTexParameterf )(GLenum target, GLenum pname, GLfloat param);
void ( APIENTRY *pglTexParameterfv )(GLenum target, GLenum pname, const GLfloat *params);
void ( APIENTRY *pglTexParameteri )(GLenum target, GLenum pname, GLint param);
void ( APIENTRY *pglTexParameteriv )(GLenum target, GLenum pname, const GLint *params);
void ( APIENTRY *pglTexSubImage1D )(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
void ( APIENTRY *pglTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void ( APIENTRY *pglTranslated )(GLdouble x, GLdouble y, GLdouble z);
void ( APIENTRY *pglTranslatef )(GLfloat x, GLfloat y, GLfloat z);
void ( APIENTRY *pglVertex2d )(GLdouble x, GLdouble y);
void ( APIENTRY *pglVertex2dv )(const GLdouble *v);
void ( APIENTRY *pglVertex2f )(GLfloat x, GLfloat y);
void ( APIENTRY *pglVertex2fv )(const GLfloat *v);
void ( APIENTRY *pglVertex2i )(GLint x, GLint y);
void ( APIENTRY *pglVertex2iv )(const GLint *v);
void ( APIENTRY *pglVertex2s )(GLshort x, GLshort y);
void ( APIENTRY *pglVertex2sv )(const GLshort *v);
void ( APIENTRY *pglVertex3d )(GLdouble x, GLdouble y, GLdouble z);
void ( APIENTRY *pglVertex3dv )(const GLdouble *v);
void ( APIENTRY *pglVertex3f )(GLfloat x, GLfloat y, GLfloat z);
void ( APIENTRY *pglVertex3fv )(const GLfloat *v);
void ( APIENTRY *pglVertex3i )(GLint x, GLint y, GLint z);
void ( APIENTRY *pglVertex3iv )(const GLint *v);
void ( APIENTRY *pglVertex3s )(GLshort x, GLshort y, GLshort z);
void ( APIENTRY *pglVertex3sv )(const GLshort *v);
void ( APIENTRY *pglVertex4d )(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void ( APIENTRY *pglVertex4dv )(const GLdouble *v);
void ( APIENTRY *pglVertex4f )(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void ( APIENTRY *pglVertex4fv )(const GLfloat *v);
void ( APIENTRY *pglVertex4i )(GLint x, GLint y, GLint z, GLint w);
void ( APIENTRY *pglVertex4iv )(const GLint *v);
void ( APIENTRY *pglVertex4s )(GLshort x, GLshort y, GLshort z, GLshort w);
void ( APIENTRY *pglVertex4sv )(const GLshort *v);
void ( APIENTRY *pglVertexPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void ( APIENTRY *pglViewport )(GLint x, GLint y, GLsizei width, GLsizei height);
void ( APIENTRY *pglPointParameterfEXT)( GLenum param, GLfloat value );
void ( APIENTRY *pglPointParameterfvEXT)( GLenum param, const GLfloat *value );
void ( APIENTRY *pglLockArraysEXT) (int , int);
void ( APIENTRY *pglUnlockArraysEXT) (void);
void ( APIENTRY *pglActiveTextureARB)( GLenum );
void ( APIENTRY *pglClientActiveTextureARB)( GLenum );
void ( APIENTRY *pglGetCompressedTexImage)( GLenum target, GLint lod, const GLvoid* data );
void ( APIENTRY *pglDrawRangeElements)( GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices );
void ( APIENTRY *pglDrawRangeElementsEXT)( GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices );
//void ( APIENTRY *pglDrawElements)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
//void ( APIENTRY *pglVertexPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
//void ( APIENTRY *pglNormalPointer)(GLenum type, GLsizei stride, const GLvoid *ptr);
//void ( APIENTRY *pglColorPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
//void ( APIENTRY *pglTexCoordPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
//void ( APIENTRY *pglArrayElement)(GLint i);
void ( APIENTRY *pglMultiTexCoord1f) (GLenum, GLfloat);
void ( APIENTRY *pglMultiTexCoord2f) (GLenum, GLfloat, GLfloat);
void ( APIENTRY *pglMultiTexCoord3f) (GLenum, GLfloat, GLfloat, GLfloat);
void ( APIENTRY *pglMultiTexCoord4f) (GLenum, GLfloat, GLfloat, GLfloat, GLfloat);
void ( APIENTRY *pglActiveTexture) (GLenum);
void ( APIENTRY *pglClientActiveTexture) (GLenum);
void ( APIENTRY *pglCompressedTexImage3DARB)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
void ( APIENTRY *pglCompressedTexImage2DARB)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border,  GLsizei imageSize, const void *data);
void ( APIENTRY *pglCompressedTexImage1DARB)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data);
void ( APIENTRY *pglCompressedTexSubImage3DARB)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
void ( APIENTRY *pglCompressedTexSubImage2DARB)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
void ( APIENTRY *pglCompressedTexSubImage1DARB)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
void ( APIENTRY *pglDeleteObjectARB)(GLhandleARB obj);
GLhandleARB ( APIENTRY *pglGetHandleARB)(GLenum pname);
void ( APIENTRY *pglDetachObjectARB)(GLhandleARB containerObj, GLhandleARB attachedObj);
GLhandleARB ( APIENTRY *pglCreateShaderObjectARB)(GLenum shaderType);
void ( APIENTRY *pglShaderSourceARB)(GLhandleARB shaderObj, GLsizei count, const GLcharARB **string, const GLint *length);
void ( APIENTRY *pglCompileShaderARB)(GLhandleARB shaderObj);
GLhandleARB ( APIENTRY *pglCreateProgramObjectARB)(void);
void ( APIENTRY *pglAttachObjectARB)(GLhandleARB containerObj, GLhandleARB obj);
void ( APIENTRY *pglLinkProgramARB)(GLhandleARB programObj);
void ( APIENTRY *pglUseProgramObjectARB)(GLhandleARB programObj);
void ( APIENTRY *pglValidateProgramARB)(GLhandleARB programObj);
void ( APIENTRY *pglBindProgramARB)(GLenum target, GLuint program);
void ( APIENTRY *pglDeleteProgramsARB)(GLsizei n, const GLuint *programs);
void ( APIENTRY *pglGenProgramsARB)(GLsizei n, GLuint *programs);
void ( APIENTRY *pglProgramStringARB)(GLenum target, GLenum format, GLsizei len, const GLvoid *string);
void ( APIENTRY *pglProgramEnvParameter4fARB)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void ( APIENTRY *pglProgramLocalParameter4fARB)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void ( APIENTRY *pglUniform1fARB)(GLint location, GLfloat v0);
void ( APIENTRY *pglUniform2fARB)(GLint location, GLfloat v0, GLfloat v1);
void ( APIENTRY *pglUniform3fARB)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
void ( APIENTRY *pglUniform4fARB)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void ( APIENTRY *pglUniform1iARB)(GLint location, GLint v0);
void ( APIENTRY *pglUniform2iARB)(GLint location, GLint v0, GLint v1);
void ( APIENTRY *pglUniform3iARB)(GLint location, GLint v0, GLint v1, GLint v2);
void ( APIENTRY *pglUniform4iARB)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
void ( APIENTRY *pglUniform1fvARB)(GLint location, GLsizei count, const GLfloat *value);
void ( APIENTRY *pglUniform2fvARB)(GLint location, GLsizei count, const GLfloat *value);
void ( APIENTRY *pglUniform3fvARB)(GLint location, GLsizei count, const GLfloat *value);
void ( APIENTRY *pglUniform4fvARB)(GLint location, GLsizei count, const GLfloat *value);
void ( APIENTRY *pglUniform1ivARB)(GLint location, GLsizei count, const GLint *value);
void ( APIENTRY *pglUniform2ivARB)(GLint location, GLsizei count, const GLint *value);
void ( APIENTRY *pglUniform3ivARB)(GLint location, GLsizei count, const GLint *value);
void ( APIENTRY *pglUniform4ivARB)(GLint location, GLsizei count, const GLint *value);
void ( APIENTRY *pglUniformMatrix2fvARB)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void ( APIENTRY *pglUniformMatrix3fvARB)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void ( APIENTRY *pglUniformMatrix4fvARB)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void ( APIENTRY *pglGetObjectParameterfvARB)(GLhandleARB obj, GLenum pname, GLfloat *params);
void ( APIENTRY *pglGetObjectParameterivARB)(GLhandleARB obj, GLenum pname, GLint *params);
void ( APIENTRY *pglGetInfoLogARB)(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog);
void ( APIENTRY *pglGetAttachedObjectsARB)(GLhandleARB containerObj, GLsizei maxCount, GLsizei *count, GLhandleARB *obj);
GLint ( APIENTRY *pglGetUniformLocationARB)(GLhandleARB programObj, const GLcharARB *name);
void ( APIENTRY *pglGetActiveUniformARB)(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
void ( APIENTRY *pglGetUniformfvARB)(GLhandleARB programObj, GLint location, GLfloat *params);
void ( APIENTRY *pglGetUniformivARB)(GLhandleARB programObj, GLint location, GLint *params);
void ( APIENTRY *pglGetShaderSourceARB)(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *source);
//void ( APIENTRY *pglPolygonStipple)(const GLubyte *mask);
void ( APIENTRY *pglTexImage3D)( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels );
void ( APIENTRY *pglTexSubImage3D)( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels );
void ( APIENTRY *pglCopyTexSubImage3D)( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height );
void ( APIENTRY *pglBlendEquationEXT)(GLenum);
void ( APIENTRY *pglStencilOpSeparate)(GLenum, GLenum, GLenum, GLenum);
void ( APIENTRY *pglStencilFuncSeparate)(GLenum, GLenum, GLint, GLuint);
void ( APIENTRY *pglActiveStencilFaceEXT)(GLenum);
void ( APIENTRY *pglVertexAttribPointerARB)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
void ( APIENTRY *pglEnableVertexAttribArrayARB)(GLuint index);
void ( APIENTRY *pglDisableVertexAttribArrayARB)(GLuint index);
void ( APIENTRY *pglBindAttribLocationARB)(GLhandleARB programObj, GLuint index, const GLcharARB *name);
void ( APIENTRY *pglGetActiveAttribARB)(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
GLint ( APIENTRY *pglGetAttribLocationARB)(GLhandleARB programObj, const GLcharARB *name);
void ( APIENTRY *pglBindBufferARB) (GLenum target, GLuint buffer);
void ( APIENTRY *pglDeleteBuffersARB) (GLsizei n, const GLuint *buffers);
void ( APIENTRY *pglGenBuffersARB) (GLsizei n, GLuint *buffers);
GLboolean ( APIENTRY *pglIsBufferARB) (GLuint buffer);
GLvoid* ( APIENTRY *pglMapBufferARB) (GLenum target, GLenum access);
GLboolean ( APIENTRY *pglUnmapBufferARB) (GLenum target);
void ( APIENTRY *pglBufferDataARB) (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
void ( APIENTRY *pglBufferSubDataARB) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
void ( APIENTRY *pglGenQueriesARB) (GLsizei n, GLuint *ids);
void ( APIENTRY *pglDeleteQueriesARB) (GLsizei n, const GLuint *ids);
GLboolean ( APIENTRY *pglIsQueryARB) (GLuint id);
void ( APIENTRY *pglBeginQueryARB) (GLenum target, GLuint id);
void ( APIENTRY *pglEndQueryARB) (GLenum target);
void ( APIENTRY *pglGetQueryivARB) (GLenum target, GLenum pname, GLint *params);
void ( APIENTRY *pglGetQueryObjectivARB) (GLuint id, GLenum pname, GLint *params);
void ( APIENTRY *pglGetQueryObjectuivARB) (GLuint id, GLenum pname, GLuint *params);
void ( APIENTRY * pglSelectTextureSGIS) ( GLenum );
void ( APIENTRY * pglMTexCoord2fSGIS) ( GLenum, GLfloat, GLfloat );
void ( APIENTRY * pglSwapInterval) ( int interval );
extern void *pglGetProcAddress( const GLubyte * );
BOOL  ( WINAPI * pwglSwapBuffers)(HDC);
BOOL  ( WINAPI * pwglCopyContext)(HGLRC, HGLRC, UINT);
HGLRC ( WINAPI * pwglCreateContext)(HDC);
HGLRC ( WINAPI * pwglCreateLayerContext)(HDC, int);
BOOL  ( WINAPI * pwglDeleteContext)(HGLRC);
HGLRC ( WINAPI * pwglGetCurrentContext)(VOID);
PROC  ( WINAPI * pwglGetProcAddress)(LPCSTR);
BOOL  ( WINAPI * pwglMakeCurrent)(HDC, HGLRC);
BOOL  ( WINAPI * pwglShareLists)(HGLRC, HGLRC);
BOOL  ( WINAPI * pwglUseFontBitmaps)(HDC, DWORD, DWORD, DWORD);
BOOL  ( WINAPI * pwglUseFontOutlines)(HDC, DWORD, DWORD, DWORD, FLOAT, FLOAT, int, LPGLYPHMETRICSFLOAT);
BOOL  ( WINAPI * pwglDescribeLayerPlane)(HDC, int, int, UINT, LPLAYERPLANEDESCRIPTOR);
int   ( WINAPI * pwglSetLayerPaletteEntries)(HDC, int, int, int, CONST COLORREF *);
int   ( WINAPI * pwglGetLayerPaletteEntries)(HDC, int, int, int, COLORREF *);
BOOL  ( WINAPI * pwglRealizeLayerPalette)(HDC, int, BOOL);
BOOL  ( WINAPI * pwglSwapLayerBuffers)(HDC, UINT);
BOOL  ( WINAPI * pwglSwapIntervalEXT)( int interval );
HGLRC ( WINAPI * pwglCreateContextAttribsARB)( HDC hDC, HGLRC hShareContext, const int *attribList );

#define VID_DEFAULTMODE		"1"
#define DISP_CHANGE_BADDUALVIEW	-6 // MSVC 6.0 doesn't
#define num_vidmodes		((int)(sizeof(vidmode) / sizeof(vidmode[0])) - 1)
#define WINDOW_STYLE		(WS_OVERLAPPED|WS_BORDER|WS_SYSMENU|WS_CAPTION|WS_VISIBLE)
#define WINDOW_EX_STYLE		(0)
#define WINDOW_NAME			"Xash Window" // Half-Life

//convar_t	*renderinfo;
convar_t	*gl_allow_software;
convar_t	*gl_extensions;
convar_t	*gl_alphabits;
convar_t	*gl_stencilbits;
convar_t	*gl_ignorehwgamma;
convar_t	*gl_texture_anisotropy;
convar_t	*gl_compress_textures;
convar_t	*gl_luminance_textures;
convar_t	*gl_compensate_gamma_screenshots;
convar_t	*gl_keeptjunctions;
convar_t	*gl_texture_lodbias;
convar_t	*gl_showtextures;
convar_t	*gl_detailscale;
convar_t	*gl_swapInterval;
convar_t	*gl_check_errors;
convar_t	*gl_allow_static;
convar_t	*gl_allow_mirrors;
convar_t	*gl_texturemode;
convar_t	*gl_wireframe;
convar_t	*gl_round_down;
convar_t	*gl_overview;
convar_t	*gl_max_size;
convar_t	*gl_picmip;
convar_t	*gl_skymip;
convar_t	*gl_finish;
convar_t	*gl_nosort;
convar_t	*gl_clear;
convar_t	*gl_test;

convar_t	*r_xpos;
convar_t	*r_ypos;
convar_t	*r_width;
convar_t	*r_height;
convar_t	*r_speeds;
convar_t	*r_fullbright;
convar_t	*r_norefresh;
convar_t	*r_lighting_extended;
convar_t	*r_lighting_modulate;
convar_t	*r_lighting_ambient;
convar_t	*r_detailtextures;
convar_t	*r_faceplanecull;
convar_t	*r_drawentities;
convar_t	*r_adjust_fov;
convar_t	*r_flaresize;
convar_t	*r_lefthand;
convar_t	*r_decals;
convar_t	*r_novis;
convar_t	*r_nocull;
convar_t	*r_lockpvs;
convar_t	*r_lockcull;
convar_t	*r_dynamic;
convar_t	*r_lightmap;
convar_t	*r_fastsky;

convar_t	*vid_displayfrequency;
convar_t	*vid_fullscreen;
convar_t	*vid_gamma;
convar_t	*vid_texgamma;
convar_t	*vid_mode;

byte		*r_temppool;

ref_globals_t	tr;
glconfig_t	glConfig;
glstate_t		glState;
glwstate_t	glw_state;
uint		num_instances;

typedef enum
{
	rserr_ok,
	rserr_invalid_fullscreen,
	rserr_invalid_mode,
	rserr_unknown
} rserr_t;

typedef struct vidmode_s
{
	const char	*desc;
	int		width;
	int		height;
	qboolean		wideScreen;
} vidmode_t;

vidmode_t vidmode[] =
{
{ "Mode  0: 4x3",	640,	480,	false	},
{ "Mode  1: 4x3",	800,	600,	false	},
{ "Mode  2: 4x3",	960,	720,	false	},
{ "Mode  3: 4x3",	1024,	768,	false	},
{ "Mode  4: 4x3",	1152,	864,	false	},
{ "Mode  5: 4x3",	1280,	800,	false	},
{ "Mode  6: 4x3",	1280,	960,	false	},
{ "Mode  7: 4x3",	1280,	1024,	false	},
{ "Mode  8: 4x3",	1600,	1200,	false	},
{ "Mode  9: 4x3",	2048,	1536,	false	},
{ "Mode 10: 16x9",	800,	480,	true	},
{ "Mode 11: 16x9",	856,	480,	true	},
{ "Mode 12: 16x9",	960,	540,	true	},
{ "Mode 13: 16x9",	1024,	576,	true	},
{ "Mode 14: 16x9",	1024,	600,	true	},
{ "Mode 15: 16x9",	1280,	720,	true	},
{ "Mode 16: 16x9",	1360,	768,	true	},
{ "Mode 17: 16x9",	1366,	768,	true	},
{ "Mode 18: 16x9",	1440,	900,	true	},
{ "Mode 19: 16x9",	1680,	1050,	true	},
{ "Mode 20: 16x9",	1920,	1080,	true	},
{ "Mode 21: 16x9",	1920,	1200,	true	},
{ "Mode 22: 16x9",	2560,	1600,	true	},
};

static dllfunc_t opengl_110funcs[] =
{
{ "glClearColor"         , (void **)&pglClearColor },
{ "glClear"              , (void **)&pglClear },
{ "glAlphaFunc"          , (void **)&pglAlphaFunc },
{ "glBlendFunc"          , (void **)&pglBlendFunc },
{ "glCullFace"           , (void **)&pglCullFace },
{ "glDrawBuffer"         , (void **)&pglDrawBuffer },
{ "glReadBuffer"         , (void **)&pglReadBuffer },
{ "glEnable"             , (void **)&pglEnable },
{ "glDisable"            , (void **)&pglDisable },
{ "glEnableClientState"  , (void **)&pglEnableClientState },
{ "glDisableClientState" , (void **)&pglDisableClientState },
{ "glGetBooleanv"        , (void **)&pglGetBooleanv },
{ "glGetDoublev"         , (void **)&pglGetDoublev },
{ "glGetFloatv"          , (void **)&pglGetFloatv },
{ "glGetIntegerv"        , (void **)&pglGetIntegerv },
{ "glGetError"           , (void **)&pglGetError },
{ "glGetString"          , (void **)&pglGetString },
{ "glFinish"             , (void **)&pglFinish },
{ "glFlush"              , (void **)&pglFlush },
{ "glClearDepth"         , (void **)&pglClearDepth },
{ "glDepthFunc"          , (void **)&pglDepthFunc },
{ "glDepthMask"          , (void **)&pglDepthMask },
{ "glDepthRange"         , (void **)&pglDepthRange },
{ "glFrontFace"          , (void **)&pglFrontFace },
{ "glDrawElements"       , (void **)&pglDrawElements },
{ "glColorMask"          , (void **)&pglColorMask },
{ "glIndexPointer"       , (void **)&pglIndexPointer },
{ "glVertexPointer"      , (void **)&pglVertexPointer },
{ "glNormalPointer"      , (void **)&pglNormalPointer },
{ "glColorPointer"       , (void **)&pglColorPointer },
{ "glTexCoordPointer"    , (void **)&pglTexCoordPointer },
{ "glArrayElement"       , (void **)&pglArrayElement },
{ "glColor3f"            , (void **)&pglColor3f },
{ "glColor3fv"           , (void **)&pglColor3fv },
{ "glColor4f"            , (void **)&pglColor4f },
{ "glColor4fv"           , (void **)&pglColor4fv },
{ "glColor3ub"           , (void **)&pglColor3ub },
{ "glColor4ub"           , (void **)&pglColor4ub },
{ "glColor4ubv"          , (void **)&pglColor4ubv },
{ "glTexCoord1f"         , (void **)&pglTexCoord1f },
{ "glTexCoord2f"         , (void **)&pglTexCoord2f },
{ "glTexCoord3f"         , (void **)&pglTexCoord3f },
{ "glTexCoord4f"         , (void **)&pglTexCoord4f },
{ "glTexGenf"            , (void **)&pglTexGenf },
{ "glTexGenfv"           , (void **)&pglTexGenfv },
{ "glTexGeni"            , (void **)&pglTexGeni },
{ "glVertex2f"           , (void **)&pglVertex2f },
{ "glVertex3f"           , (void **)&pglVertex3f },
{ "glVertex3fv"          , (void **)&pglVertex3fv },
{ "glNormal3f"           , (void **)&pglNormal3f },
{ "glNormal3fv"          , (void **)&pglNormal3fv },
{ "glBegin"              , (void **)&pglBegin },
{ "glEnd"                , (void **)&pglEnd },
{ "glLineWidth"          , (void**)&pglLineWidth },
{ "glPointSize"          , (void**)&pglPointSize },
{ "glMatrixMode"         , (void **)&pglMatrixMode },
{ "glOrtho"              , (void **)&pglOrtho },
{ "glRasterPos2f"        , (void **)&pglRasterPos2f },
{ "glFrustum"            , (void **)&pglFrustum },
{ "glViewport"           , (void **)&pglViewport },
{ "glPushMatrix"         , (void **)&pglPushMatrix },
{ "glPopMatrix"          , (void **)&pglPopMatrix },
{ "glPushAttrib"         , (void **)&pglPushAttrib },
{ "glPopAttrib"          , (void **)&pglPopAttrib },
{ "glLoadIdentity"       , (void **)&pglLoadIdentity },
{ "glLoadMatrixd"        , (void **)&pglLoadMatrixd },
{ "glLoadMatrixf"        , (void **)&pglLoadMatrixf },
{ "glMultMatrixd"        , (void **)&pglMultMatrixd },
{ "glMultMatrixf"        , (void **)&pglMultMatrixf },
{ "glRotated"            , (void **)&pglRotated },
{ "glRotatef"            , (void **)&pglRotatef },
{ "glScaled"             , (void **)&pglScaled },
{ "glScalef"             , (void **)&pglScalef },
{ "glTranslated"         , (void **)&pglTranslated },
{ "glTranslatef"         , (void **)&pglTranslatef },
{ "glReadPixels"         , (void **)&pglReadPixels },
{ "glDrawPixels"         , (void **)&pglDrawPixels },
{ "glStencilFunc"        , (void **)&pglStencilFunc },
{ "glStencilMask"        , (void **)&pglStencilMask },
{ "glStencilOp"          , (void **)&pglStencilOp },
{ "glClearStencil"       , (void **)&pglClearStencil },
{ "glIsEnabled"          , (void **)&pglIsEnabled },
{ "glIsList"             , (void **)&pglIsList },
{ "glIsTexture"          , (void **)&pglIsTexture },
{ "glTexEnvf"            , (void **)&pglTexEnvf },
{ "glTexEnvfv"           , (void **)&pglTexEnvfv },
{ "glTexEnvi"            , (void **)&pglTexEnvi },
{ "glTexParameterf"      , (void **)&pglTexParameterf },
{ "glTexParameterfv"     , (void **)&pglTexParameterfv },
{ "glTexParameteri"      , (void **)&pglTexParameteri },
{ "glHint"               , (void **)&pglHint },
{ "glPixelStoref"        , (void **)&pglPixelStoref },
{ "glPixelStorei"        , (void **)&pglPixelStorei },
{ "glGenTextures"        , (void **)&pglGenTextures },
{ "glDeleteTextures"     , (void **)&pglDeleteTextures },
{ "glBindTexture"        , (void **)&pglBindTexture },
{ "glTexImage1D"         , (void **)&pglTexImage1D },
{ "glTexImage2D"         , (void **)&pglTexImage2D },
{ "glTexSubImage1D"      , (void **)&pglTexSubImage1D },
{ "glTexSubImage2D"      , (void **)&pglTexSubImage2D },
{ "glCopyTexImage1D"     , (void **)&pglCopyTexImage1D },
{ "glCopyTexImage2D"     , (void **)&pglCopyTexImage2D },
{ "glCopyTexSubImage1D"  , (void **)&pglCopyTexSubImage1D },
{ "glCopyTexSubImage2D"  , (void **)&pglCopyTexSubImage2D },
{ "glScissor"            , (void **)&pglScissor },
{ "glGetTexEnviv"        , (void **)&pglGetTexEnviv },
{ "glPolygonOffset"      , (void **)&pglPolygonOffset },
{ "glPolygonMode"        , (void **)&pglPolygonMode },
{ "glPolygonStipple"     , (void **)&pglPolygonStipple },
{ "glClipPlane"          , (void **)&pglClipPlane },
{ "glGetClipPlane"       , (void **)&pglGetClipPlane },
{ "glShadeModel"         , (void **)&pglShadeModel },
{ "glFogfv"              , (void **)&pglFogfv },
{ "glFogf"               , (void **)&pglFogf },
{ "glFogi"               , (void **)&pglFogi },
{ NULL, NULL }
};

static dllfunc_t pointparametersfunc[] =
{
{ "glPointParameterfEXT"  , (void **)&pglPointParameterfEXT },
{ "glPointParameterfvEXT" , (void **)&pglPointParameterfvEXT },
{ NULL, NULL }
};

static dllfunc_t drawrangeelementsfuncs[] =
{
{ "glDrawRangeElements" , (void **)&pglDrawRangeElements },
{ NULL, NULL }
};

static dllfunc_t drawrangeelementsextfuncs[] =
{
{ "glDrawRangeElementsEXT" , (void **)&pglDrawRangeElementsEXT },
{ NULL, NULL }
};

static dllfunc_t sgis_multitexturefuncs[] =
{
{ "glSelectTextureSGIS" , (void **)&pglSelectTextureSGIS },
{ "glMTexCoord2fSGIS"   , (void **)&pglMTexCoord2fSGIS },
{ NULL, NULL }
};

static dllfunc_t multitexturefuncs[] =
{
{ "glMultiTexCoord1fARB"     , (void **)&pglMultiTexCoord1f },
{ "glMultiTexCoord2fARB"     , (void **)&pglMultiTexCoord2f },
{ "glMultiTexCoord3fARB"     , (void **)&pglMultiTexCoord3f },
{ "glMultiTexCoord4fARB"     , (void **)&pglMultiTexCoord4f },
{ "glActiveTextureARB"       , (void **)&pglActiveTextureARB },
{ "glClientActiveTextureARB" , (void **)&pglClientActiveTexture },
{ "glClientActiveTextureARB" , (void **)&pglClientActiveTextureARB },
{ NULL, NULL }
};

static dllfunc_t compiledvertexarrayfuncs[] =
{
{ "glLockArraysEXT"   , (void **)&pglLockArraysEXT },
{ "glUnlockArraysEXT" , (void **)&pglUnlockArraysEXT },
{ "glDrawArrays"      , (void **)&pglDrawArrays },
{ NULL, NULL }
};

static dllfunc_t texture3dextfuncs[] =
{
{ "glTexImage3DEXT"        , (void **)&pglTexImage3D },
{ "glTexSubImage3DEXT"     , (void **)&pglTexSubImage3D },
{ "glCopyTexSubImage3DEXT" , (void **)&pglCopyTexSubImage3D },
{ NULL, NULL }
};

static dllfunc_t atiseparatestencilfuncs[] =
{
{ "glStencilOpSeparateATI"   , (void **)&pglStencilOpSeparate },
{ "glStencilFuncSeparateATI" , (void **)&pglStencilFuncSeparate },
{ NULL, NULL }
};

static dllfunc_t gl2separatestencilfuncs[] =
{
{ "glStencilOpSeparate"   , (void **)&pglStencilOpSeparate },
{ "glStencilFuncSeparate" , (void **)&pglStencilFuncSeparate },
{ NULL, NULL }
};

static dllfunc_t stenciltwosidefuncs[] =
{
{ "glActiveStencilFaceEXT" , (void **)&pglActiveStencilFaceEXT },
{ NULL, NULL }
};

static dllfunc_t blendequationfuncs[] =
{
{ "glBlendEquationEXT" , (void **)&pglBlendEquationEXT },
{ NULL, NULL }
};

static dllfunc_t shaderobjectsfuncs[] =
{
{ "glDeleteObjectARB"             , (void **)&pglDeleteObjectARB },
{ "glGetHandleARB"                , (void **)&pglGetHandleARB },
{ "glDetachObjectARB"             , (void **)&pglDetachObjectARB },
{ "glCreateShaderObjectARB"       , (void **)&pglCreateShaderObjectARB },
{ "glShaderSourceARB"             , (void **)&pglShaderSourceARB },
{ "glCompileShaderARB"            , (void **)&pglCompileShaderARB },
{ "glCreateProgramObjectARB"      , (void **)&pglCreateProgramObjectARB },
{ "glAttachObjectARB"             , (void **)&pglAttachObjectARB },
{ "glLinkProgramARB"              , (void **)&pglLinkProgramARB },
{ "glUseProgramObjectARB"         , (void **)&pglUseProgramObjectARB },
{ "glValidateProgramARB"          , (void **)&pglValidateProgramARB },
{ "glUniform1fARB"                , (void **)&pglUniform1fARB },
{ "glUniform2fARB"                , (void **)&pglUniform2fARB },
{ "glUniform3fARB"                , (void **)&pglUniform3fARB },
{ "glUniform4fARB"                , (void **)&pglUniform4fARB },
{ "glUniform1iARB"                , (void **)&pglUniform1iARB },
{ "glUniform2iARB"                , (void **)&pglUniform2iARB },
{ "glUniform3iARB"                , (void **)&pglUniform3iARB },
{ "glUniform4iARB"                , (void **)&pglUniform4iARB },
{ "glUniform1fvARB"               , (void **)&pglUniform1fvARB },
{ "glUniform2fvARB"               , (void **)&pglUniform2fvARB },
{ "glUniform3fvARB"               , (void **)&pglUniform3fvARB },
{ "glUniform4fvARB"               , (void **)&pglUniform4fvARB },
{ "glUniform1ivARB"               , (void **)&pglUniform1ivARB },
{ "glUniform2ivARB"               , (void **)&pglUniform2ivARB },
{ "glUniform3ivARB"               , (void **)&pglUniform3ivARB },
{ "glUniform4ivARB"               , (void **)&pglUniform4ivARB },
{ "glUniformMatrix2fvARB"         , (void **)&pglUniformMatrix2fvARB },
{ "glUniformMatrix3fvARB"         , (void **)&pglUniformMatrix3fvARB },
{ "glUniformMatrix4fvARB"         , (void **)&pglUniformMatrix4fvARB },
{ "glGetObjectParameterfvARB"     , (void **)&pglGetObjectParameterfvARB },
{ "glGetObjectParameterivARB"     , (void **)&pglGetObjectParameterivARB },
{ "glGetInfoLogARB"               , (void **)&pglGetInfoLogARB },
{ "glGetAttachedObjectsARB"       , (void **)&pglGetAttachedObjectsARB },
{ "glGetUniformLocationARB"       , (void **)&pglGetUniformLocationARB },
{ "glGetActiveUniformARB"         , (void **)&pglGetActiveUniformARB },
{ "glGetUniformfvARB"             , (void **)&pglGetUniformfvARB },
{ "glGetUniformivARB"             , (void **)&pglGetUniformivARB },
{ "glGetShaderSourceARB"          , (void **)&pglGetShaderSourceARB },
{ "glVertexAttribPointerARB"      , (void **)&pglVertexAttribPointerARB },
{ "glEnableVertexAttribArrayARB"  , (void **)&pglEnableVertexAttribArrayARB },
{ "glDisableVertexAttribArrayARB" , (void **)&pglDisableVertexAttribArrayARB },
{ "glBindAttribLocationARB"       , (void **)&pglBindAttribLocationARB },
{ "glGetActiveAttribARB"          , (void **)&pglGetActiveAttribARB },
{ "glGetAttribLocationARB"        , (void **)&pglGetAttribLocationARB },
{ NULL, NULL }
};

static dllfunc_t vertexshaderfuncs[] =
{
{ "glVertexAttribPointerARB"      , (void **)&pglVertexAttribPointerARB },
{ "glEnableVertexAttribArrayARB"  , (void **)&pglEnableVertexAttribArrayARB },
{ "glDisableVertexAttribArrayARB" , (void **)&pglDisableVertexAttribArrayARB },
{ "glBindAttribLocationARB"       , (void **)&pglBindAttribLocationARB },
{ "glGetActiveAttribARB"          , (void **)&pglGetActiveAttribARB },
{ "glGetAttribLocationARB"        , (void **)&pglGetAttribLocationARB },
{ NULL, NULL }
};

static dllfunc_t vbofuncs[] =
{
{ "glBindBufferARB"    , (void **)&pglBindBufferARB },
{ "glDeleteBuffersARB" , (void **)&pglDeleteBuffersARB },
{ "glGenBuffersARB"    , (void **)&pglGenBuffersARB },
{ "glIsBufferARB"      , (void **)&pglIsBufferARB },
{ "glMapBufferARB"     , (void **)&pglMapBufferARB },
{ "glUnmapBufferARB"   , (void **)&pglUnmapBufferARB },
{ "glBufferDataARB"    , (void **)&pglBufferDataARB },
{ "glBufferSubDataARB" , (void **)&pglBufferSubDataARB },
{ NULL, NULL}
};

static dllfunc_t occlusionfunc[] =
{
{ "glGenQueriesARB"        , (void **)&pglGenQueriesARB },
{ "glDeleteQueriesARB"     , (void **)&pglDeleteQueriesARB },
{ "glIsQueryARB"           , (void **)&pglIsQueryARB },
{ "glBeginQueryARB"        , (void **)&pglBeginQueryARB },
{ "glEndQueryARB"          , (void **)&pglEndQueryARB },
{ "glGetQueryivARB"        , (void **)&pglGetQueryivARB },
{ "glGetQueryObjectivARB"  , (void **)&pglGetQueryObjectivARB },
{ "glGetQueryObjectuivARB" , (void **)&pglGetQueryObjectuivARB },
{ NULL, NULL }
};

static dllfunc_t texturecompressionfuncs[] =
{
{ "glCompressedTexImage3DARB"    , (void **)&pglCompressedTexImage3DARB },
{ "glCompressedTexImage2DARB"    , (void **)&pglCompressedTexImage2DARB },
{ "glCompressedTexImage1DARB"    , (void **)&pglCompressedTexImage1DARB },
{ "glCompressedTexSubImage3DARB" , (void **)&pglCompressedTexSubImage3DARB },
{ "glCompressedTexSubImage2DARB" , (void **)&pglCompressedTexSubImage2DARB },
{ "glCompressedTexSubImage1DARB" , (void **)&pglCompressedTexSubImage1DARB },
{ "glGetCompressedTexImageARB"   , (void **)&pglGetCompressedTexImage },
{ NULL, NULL }
};

static dllfunc_t wgl_funcs[] =
{
{ "wglSwapBuffers"         , (void **)&pwglSwapBuffers },
{ "wglCreateContext"       , (void **)&pwglCreateContext },
{ "wglDeleteContext"       , (void **)&pwglDeleteContext },
{ "wglMakeCurrent"         , (void **)&pwglMakeCurrent },
{ "wglGetCurrentContext"   , (void **)&pwglGetCurrentContext },
{ NULL, NULL }
};

static dllfunc_t wglproc_funcs[] =
{
{ "wglGetProcAddress"  , (void **)&pwglGetProcAddress },
{ NULL, NULL }
};

static dllfunc_t wglswapintervalfuncs[] =
{
{ "wglSwapIntervalEXT" , (void **)&pwglSwapIntervalEXT },
{ NULL, NULL }
};

dll_info_t opengl_dll = { "opengl32.dll", wgl_funcs, true };

/*
=================
GL_SetExtension
=================
*/
void GL_SetExtension( int r_ext, int enable )
{
	if( r_ext >= 0 && r_ext < GL_EXTCOUNT )
		glConfig.extension[r_ext] = enable ? GL_TRUE : GL_FALSE;
	else MsgDev( D_ERROR, "GL_SetExtension: invalid extension %d\n", r_ext );
}

/*
=================
GL_Support
=================
*/
qboolean GL_Support( int r_ext )
{
	if( r_ext >= 0 && r_ext < GL_EXTCOUNT )
		return glConfig.extension[r_ext] ? true : false;
	MsgDev( D_ERROR, "GL_Support: invalid extension %d\n", r_ext );

	return false;
}

/*
=================
GL_MaxTextureUnits
=================
*/
int GL_MaxTextureUnits( void )
{
	if( GL_Support( GL_SHADER_GLSL100_EXT ))
		return min( max( glConfig.max_texture_coords, glConfig.max_teximage_units ), MAX_TEXTURE_UNITS );
	return glConfig.max_texture_units;
}

/*
=================
GL_GetProcAddress
=================
*/
void *GL_GetProcAddress( const char *name )
{
	void	*p = NULL;

	if( pwglGetProcAddress != NULL )
		p = (void *)pwglGetProcAddress( name );
	if( !p ) p = (void *)Sys_GetProcAddress( &opengl_dll, name );

	return p;
}

/*
=================
GL_CheckExtension
=================
*/
void GL_CheckExtension( const char *name, const dllfunc_t *funcs, const char *cvarname, int r_ext )
{
	const dllfunc_t	*func;
	convar_t		*parm;

	MsgDev( D_NOTE, "GL_CheckExtension: %s ", name );

	if( cvarname )
	{
		// system config disable extensions
		parm = Cvar_Get( cvarname, "1", CVAR_GLCONFIG, va( "enable or disable %s", name ));

		if( parm->integer == 0 || ( gl_extensions->integer == 0 && r_ext != GL_OPENGL_110 ))
		{
			MsgDev( D_NOTE, "- disabled\n" );
			GL_SetExtension( r_ext, 0 );
			return; // nothing to process at
		}
		GL_SetExtension( r_ext, 1 );
	}

	if(( name[2] == '_' || name[3] == '_' ) && !Q_strstr( glConfig.extensions_string, name ))
	{
		GL_SetExtension( r_ext, false );	// update render info
		MsgDev( D_NOTE, "- ^1failed\n" );
		return;
	}

	// clear exports
	for( func = funcs; func && func->name; func++ )
		*func->func = NULL;

	GL_SetExtension( r_ext, true ); // predict extension state
	for( func = funcs; func && func->name != NULL; func++ )
	{
		// functions are cleared before all the extensions are evaluated
		if(!(*func->func = (void *)GL_GetProcAddress( func->name )))
			GL_SetExtension( r_ext, false ); // one or more functions are invalid, extension will be disabled
	}

	if( GL_Support( r_ext ))
		MsgDev( D_NOTE, "- ^2enabled\n" );
	else MsgDev( D_NOTE, "- ^1failed\n" );
}

/*
===============
GL_BuildGammaTable
===============
*/
void GL_BuildGammaTable( void )
{
	int	i, v;
	double	invGamma, div;

	invGamma = 1.0 / bound( 0.5, vid_gamma->value, 2.3 );
	div = (double) 1.0 / 255.5;

	Q_memcpy( glState.gammaRamp, glState.stateRamp, sizeof( glState.gammaRamp ));

	for( i = 0; i < 256; i++ )
	{
		v = (int)(65535.0 * pow(((double)i + 0.5 ) * div, invGamma ) + 0.5 );
		glState.gammaRamp[i+0]   = ((word)bound( 0, v, 65535 ));
		glState.gammaRamp[i+256] = ((word)bound( 0, v, 65535 ));
		glState.gammaRamp[i+512] = ((word)bound( 0, v, 65535 ));
	}
}

/*
===============
GL_UpdateGammaRamp
===============
*/
void GL_UpdateGammaRamp( void )
{
	if( !glConfig.deviceSupportsGamma ) return;

	GL_BuildGammaTable();

	SetDeviceGammaRamp( glw_state.hDC, glState.gammaRamp );
}

/*
===============
GL_UpdateSwapInterval
===============
*/
void GL_UpdateSwapInterval( void )
{
	if( gl_swapInterval->modified )
	{
		gl_swapInterval->modified = false;

		if( pwglSwapIntervalEXT )
			pwglSwapIntervalEXT( gl_swapInterval->integer );
	}
}

/*
===============
GL_SetDefaultTexState
===============
*/
static void GL_SetDefaultTexState( void )
{
	int	i;

	Q_memset( glState.currentTextures, -1, MAX_TEXTURE_UNITS * sizeof( *glState.currentTextures ));
	Q_memset( glState.texCoordArrayMode, 0, MAX_TEXTURE_UNITS * sizeof( *glState.texCoordArrayMode ));
	Q_memset( glState.genSTEnabled, 0, MAX_TEXTURE_UNITS * sizeof( *glState.genSTEnabled ));

	for( i = 0; i < MAX_TEXTURE_UNITS; i++ )
	{
		glState.currentTextureTargets[i] = GL_NONE;
		glState.texIdentityMatrix[i] = true;
	}
}

/*
===============
GL_SetDefaultState
===============
*/
static void GL_SetDefaultState( void )
{
	Q_memset( &glState, 0, sizeof( glState ));
	GL_SetDefaultTexState ();
}

/*
===============
GL_ContextError
===============
*/
static void GL_ContextError( void )
{
	DWORD error = GetLastError();

	if( error == ( 0xc0070000|ERROR_INVALID_VERSION_ARB ))
		MsgDev( D_ERROR, "Unsupported OpenGL context version (%s).\n", "2.0" );
	else if( error == ( 0xc0070000|ERROR_INVALID_PROFILE_ARB ))
		MsgDev( D_ERROR, "Unsupported OpenGL profile (%s).\n", "compat" );
	else if( error == ( 0xc0070000|ERROR_INVALID_OPERATION ))
		MsgDev( D_ERROR, "wglCreateContextAttribsARB returned invalid operation.\n" );
	else if( error == ( 0xc0070000|ERROR_DC_NOT_FOUND ))
		MsgDev( D_ERROR, "wglCreateContextAttribsARB returned dc not found.\n" );
	else if( error == ( 0xc0070000|ERROR_INVALID_PIXEL_FORMAT ))
		MsgDev( D_ERROR, "wglCreateContextAttribsARB returned dc not found.\n" );
	else if( error == ( 0xc0070000|ERROR_NO_SYSTEM_RESOURCES ))
		MsgDev( D_ERROR, "wglCreateContextAttribsARB ran out of system resources.\n" );
	else if( error == ( 0xc0070000|ERROR_INVALID_PARAMETER ))
		MsgDev( D_ERROR, "wglCreateContextAttribsARB reported invalid parameter.\n" );
	else MsgDev( D_ERROR, "Unknown error creating an OpenGL (%s) Context.\n", "2.0" );
}

/*
=================
GL_CreateContext
=================
*/
qboolean GL_CreateContext( void )
{
	HGLRC hBaseRC;

	if(!( glw_state.hGLRC = pwglCreateContext( glw_state.hDC )))
		return GL_DeleteContext();

	if(!( pwglMakeCurrent( glw_state.hDC, glw_state.hGLRC )))
		return GL_DeleteContext();

	if( host.developer <= 1 )
		return true;

	pwglCreateContextAttribsARB = reinterpret_cast<HGLRC ( WINAPI * )( HDC hDC, HGLRC hShareContext, const int *attribList )>(GL_GetProcAddress( "wglCreateContextAttribsARB" ));

	if( pwglCreateContextAttribsARB != NULL )
	{
		int attribs[] =
		{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 2,
		WGL_CONTEXT_MINOR_VERSION_ARB, 0,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
//		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		0
		};

		hBaseRC = glw_state.hGLRC; // backup
		glw_state.hGLRC = NULL;

		if( !( glw_state.hGLRC = pwglCreateContextAttribsARB( glw_state.hDC, NULL, attribs )))
		{
			glw_state.hGLRC = hBaseRC;
			GL_ContextError();
			return true; // just use old context
		}

		if(!( pwglMakeCurrent( glw_state.hDC, glw_state.hGLRC )))
		{
			pwglDeleteContext( glw_state.hGLRC );
			glw_state.hGLRC = hBaseRC;
			GL_ContextError();
			return true;
		}

		MsgDev( D_NOTE, "GL_CreateContext: using extended context\n" );
		pwglDeleteContext( hBaseRC );	// release first context
	}

	return true;
}

/*
=================
GL_UpdateContext
=================
*/
qboolean GL_UpdateContext( void )
{
	if(!( pwglMakeCurrent( glw_state.hDC, glw_state.hGLRC )))
		return GL_DeleteContext();

	return true;
}

/*
=================
GL_DeleteContext
=================
*/
qboolean GL_DeleteContext( void )
{
	if( pwglMakeCurrent )
		pwglMakeCurrent( NULL, NULL );

	if( glw_state.hGLRC )
	{
		if( pwglDeleteContext )
			pwglDeleteContext( glw_state.hGLRC );
		glw_state.hGLRC = NULL;
	}

	if( glw_state.hDC )
	{
		ReleaseDC( host.hWnd, glw_state.hDC );
		glw_state.hDC = NULL;
	}

	return false;
}

/*
=================
VID_ChoosePFD
=================
*/
static int VID_ChoosePFD( PIXELFORMATDESCRIPTOR *pfd, int colorBits, int alphaBits, int depthBits, int stencilBits )
{
	int	pixelFormat = 0;

	MsgDev( D_NOTE, "VID_ChoosePFD( color %i, alpha %i, depth %i, stencil %i )\n", colorBits, alphaBits, depthBits, stencilBits );

	// Fill out the PFD
	pfd->nSize = sizeof (PIXELFORMATDESCRIPTOR);
	pfd->nVersion = 1;
	pfd->dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	pfd->iPixelType = PFD_TYPE_RGBA;

	pfd->cColorBits = colorBits;
	pfd->cRedBits = 0;
	pfd->cRedShift = 0;
	pfd->cGreenBits = 0;
	pfd->cGreenShift = 0;
	pfd->cBlueBits = 0;
	pfd->cBlueShift = 0;	// wow! Blue Shift %)

	pfd->cAlphaBits = alphaBits;
	pfd->cAlphaShift = 0;

	pfd->cAccumBits = 0;
	pfd->cAccumRedBits = 0;
	pfd->cAccumGreenBits = 0;
	pfd->cAccumBlueBits = 0;
	pfd->cAccumAlphaBits= 0;

	pfd->cDepthBits = depthBits;
	pfd->cStencilBits = stencilBits;

	pfd->cAuxBuffers = 0;
	pfd->iLayerType = PFD_MAIN_PLANE;
	pfd->bReserved = 0;

	pfd->dwLayerMask = 0;
	pfd->dwVisibleMask = 0;
	pfd->dwDamageMask = 0;

	// count PFDs
	pixelFormat = ChoosePixelFormat( glw_state.hDC, pfd );

	if( !pixelFormat )
	{
		MsgDev( D_ERROR, "VID_ChoosePFD failed\n" );
		return 0;
	}

	return pixelFormat;
}

BOOL CALLBACK pfnEnumWnd( HWND hwnd, LPARAM lParam )
{
	string	wndname;

	if( GetClassName( hwnd, wndname, sizeof( wndname ) - 1 ))
	{
		if( !Q_strcmp( wndname, WINDOW_NAME ))
			num_instances++;
	}
	return true;
}

uint VID_EnumerateInstances( void )
{
	num_instances = 0;

	if( EnumWindows( &pfnEnumWnd, 0 ))
		return num_instances;
	return 1;
}

void VID_StartupGamma( void )
{
	size_t	gamma_size;
	byte	*savedGamma;

	// init gamma ramp
	Q_memset( glState.stateRamp, 0, sizeof( glState.stateRamp ));

	glConfig.deviceSupportsGamma = false;//GetDeviceGammaRamp( glw_state.hDC, glState.stateRamp );

	if( !glConfig.deviceSupportsGamma )
	{
		// force to set cvar
		Cvar_FullSet( "gl_ignorehwgamma", "1", CVAR_GLCONFIG );
	}

	if( gl_ignorehwgamma->integer )
	{
		glConfig.deviceSupportsGamma = false;	// even if supported!
		BuildGammaTable( vid_gamma->value, vid_texgamma->value );
		MsgDev( D_NOTE, "VID_StartupGamma: software gamma initialized\n" );
		return;
	}

	// share this extension so engine can grab them
	GL_SetExtension( GL_HARDWARE_GAMMA_CONTROL, glConfig.deviceSupportsGamma );

	savedGamma = FS_LoadFile( "gamma.dat", reinterpret_cast<fs_offset_t*>(&gamma_size), false );

	if( !savedGamma || gamma_size != sizeof( glState.stateRamp ))
	{
		// saved gamma not found or corrupted file
		FS_WriteFile( "gamma.dat", glState.stateRamp, sizeof( glState.stateRamp ));
		MsgDev( D_NOTE, "VID_StartupGamma: gamma.dat initialized\n" );
		if( savedGamma ) Mem_Free( savedGamma );
	}
	else
	{
		GL_BuildGammaTable();

		// validate base gamma
		if( !Q_memcmp( savedGamma, glState.stateRamp, sizeof( glState.stateRamp )))
		{
			// all ok, previous gamma is valid
			MsgDev( D_NOTE, "VID_StartupGamma: validate screen gamma - ok\n" );
		}
		else if( !Q_memcmp( glState.gammaRamp, glState.stateRamp, sizeof( glState.stateRamp )))
		{
			// screen gamma is equal to render gamma (probably previous instance crashed)
			// run additional check to make sure for it
			if( Q_memcmp( savedGamma, glState.stateRamp, sizeof( glState.stateRamp )))
			{
				// yes, current gamma it's totally wrong, restore it from gamma.dat
				MsgDev( D_NOTE, "VID_StartupGamma: restore original gamma after crash\n" );
				Q_memcpy( glState.stateRamp, savedGamma, sizeof( glState.gammaRamp ));
			}
			else
			{
				// oops, savedGamma == glState.stateRamp == glState.gammaRamp
				// probably r_gamma set as default
				MsgDev( D_NOTE, "VID_StartupGamma: validate screen gamma - disabled\n" );
			}
		}
		else if( !Q_memcmp( glState.gammaRamp, savedGamma, sizeof( glState.stateRamp )))
		{
			// saved gamma is equal render gamma, probably gamma.dat wroted after crash
			// run additional check to make sure it
			if( Q_memcmp( savedGamma, glState.stateRamp, sizeof( glState.stateRamp )))
			{
				// yes, saved gamma it's totally wrong, get origianl gamma from screen
				MsgDev( D_NOTE, "VID_StartupGamma: merge gamma.dat after crash\n" );
				FS_WriteFile( "gamma.dat", glState.stateRamp, sizeof( glState.stateRamp ));
			}
			else
			{
				// oops, savedGamma == glState.stateRamp == glState.gammaRamp
				// probably r_gamma set as default
				MsgDev( D_NOTE, "VID_StartupGamma: validate screen gamma - disabled\n" );
			}
		}
		else
		{
			// current gamma unset by other application, so we can restore it here
			MsgDev( D_NOTE, "VID_StartupGamma: restore original gamma after crash\n" );
			Q_memcpy( glState.stateRamp, savedGamma, sizeof( glState.gammaRamp ));
		}

		Mem_Free( savedGamma );
	}

	vid_gamma->modified = true;
}

void VID_RestoreGamma( void )
{
	if( !glw_state.hDC || !glConfig.deviceSupportsGamma )
		return;

	// don't touch gamma if multiple instances was running
	if( VID_EnumerateInstances( ) > 1 ) return;

	SetDeviceGammaRamp( glw_state.hDC, glState.stateRamp );
}

/*
=================
GL_SetPixelformat
=================
*/
qboolean GL_SetPixelformat( void )
{
	PIXELFORMATDESCRIPTOR	PFD;
	int			alphaBits;
	int			stencilBits;
	int			pixelFormat;

	if(( glw_state.hDC = GetDC( host.hWnd )) == NULL )
		return false;

	// set alpha/stencil
	alphaBits = bound( 0, gl_alphabits->integer, 8 );
	stencilBits = bound( 0, gl_stencilbits->integer, 8 );

	if( glw_state.desktopBitsPixel < 32 )
	{
		// clear alphabits in case we in 16-bit mode
		alphaBits = 0;
	}

	// choose a pixel format
	pixelFormat = VID_ChoosePFD( &PFD, 24, alphaBits, 32, stencilBits );

	if( !pixelFormat )
	{
		// try again with default color/depth/stencil
		pixelFormat = VID_ChoosePFD( &PFD, 24, 0, 32, 0 );

		if( !pixelFormat )
		{
			MsgDev( D_ERROR, "GL_SetPixelformat: failed to find an appropriate PIXELFORMAT\n" );
			return false;
		}
	}

	// set the pixel format
	if( !SetPixelFormat( glw_state.hDC, pixelFormat, &PFD ))
	{
		MsgDev( D_ERROR, "GL_SetPixelformat: failed\n" );
		return false;
	}

	DescribePixelFormat( glw_state.hDC, pixelFormat, sizeof( PIXELFORMATDESCRIPTOR ), &PFD );

	if( PFD.dwFlags & PFD_GENERIC_FORMAT )
	{
		if( PFD.dwFlags & PFD_GENERIC_ACCELERATED )
		{
			MsgDev( D_NOTE, "VID_ChoosePFD: using Generic MCD acceleration\n" );
			glw_state.software = false;
		}
		else if( gl_allow_software->integer )
		{
			MsgDev( D_NOTE, "VID_ChoosePFD: using software emulation\n" );
			glw_state.software = true;
		}
		else
		{
			MsgDev( D_ERROR, "GL_SetPixelformat: no hardware acceleration found\n" );
			return false;
		}
	}
	else
	{
		MsgDev( D_NOTE, "VID_ChoosePFD: using hardware acceleration\n");
		glw_state.software = false;
	}

	glConfig.color_bits = PFD.cColorBits;
	glConfig.alpha_bits = PFD.cAlphaBits;
	glConfig.depth_bits = PFD.cDepthBits;
	glConfig.stencil_bits = PFD.cStencilBits;

	if( PFD.cStencilBits != 0 )
		glState.stencilEnabled = true;
	else glState.stencilEnabled = false;

	// print out PFD specifics
	MsgDev( D_NOTE, "GL PFD: color( %d-bits ) alpha( %d-bits ) Z( %d-bit )\n", PFD.cColorBits, PFD.cAlphaBits, PFD.cDepthBits );

	return true;
}

void R_SaveVideoMode( int vid_mode )
{
	int	mode = bound( 0, vid_mode, num_vidmodes ); // check range

	glState.width = vidmode[mode].width;
	glState.height = vidmode[mode].height;
	glState.wideScreen = vidmode[mode].wideScreen;

	Cvar_FullSet( "width", va( "%i", vidmode[mode].width ), CVAR_READ_ONLY );
	Cvar_FullSet( "height", va( "%i", vidmode[mode].height ), CVAR_READ_ONLY );
	Cvar_SetFloat( "vid_mode", (float)mode ); // merge if it out of bounds

	MsgDev( D_NOTE, "Set: %s [%dx%d]\n", vidmode[mode].desc, vidmode[mode].width, vidmode[mode].height );
}

qboolean R_DescribeVIDMode( int width, int height )
{
	int	i;

	for( i = 0; i < sizeof( vidmode ) / sizeof( vidmode[0] ); i++ )
	{
		if( vidmode[i].width == width && vidmode[i].height == height )
		{
			// found specified mode
			Cvar_SetFloat( "vid_mode", (float)i );
			return true;
		}
	}

	return false;
}

qboolean VID_CreateWindow( int width, int height, qboolean fullscreen )
{
	WNDCLASS		wc;
	RECT		rect;
	int		x = 0, y = 0, w, h;
	int		stylebits = WINDOW_STYLE;
	int		exstyle = WINDOW_EX_STYLE;
	static string	wndname;
	HWND		window;

	Q_strncpy( wndname, GI->title, sizeof( wndname ));

	// register the frame class
	wc.style         = CS_OWNDC|CS_NOCLOSE;
	wc.lpfnWndProc   = (WNDPROC)IN_WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = host.hInst;
	wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_3DSHADOW);
	wc.lpszClassName = WINDOW_NAME;
	wc.lpszMenuName  = 0;

	// find the icon file in the filesystem
	if( FS_FileExists( GI->iconpath, true ))
	{
		char	localPath[MAX_PATH];

		Q_snprintf( localPath, sizeof( localPath ), "%s/%s", GI->gamedir, GI->iconpath );
		wc.hIcon = reinterpret_cast<HICON>(LoadImage( NULL, localPath, IMAGE_ICON, 0, 0, LR_LOADFROMFILE|LR_DEFAULTSIZE ));

		if( !wc.hIcon )
		{
			MsgDev( D_INFO, "Extract %s from pak if you want to see it.\n", GI->iconpath );
			wc.hIcon = LoadIcon( host.hInst, MAKEINTRESOURCE( 101 ));
		}
	}
	else wc.hIcon = LoadIcon( host.hInst, MAKEINTRESOURCE( 101 ));

	if( !RegisterClass( &wc ))
	{
		MsgDev( D_ERROR, "VID_CreateWindow: couldn't register window class %s\n" WINDOW_NAME );
		return false;
	}

	if( fullscreen )
	{
		stylebits = WS_POPUP|WS_VISIBLE;
		exstyle = WS_EX_TOPMOST;
	}

	rect.left = 0;
	rect.top = 0;
	rect.right  = width;
	rect.bottom = height;

	AdjustWindowRect( &rect, stylebits, FALSE );
	w = rect.right - rect.left;
	h = rect.bottom - rect.top;

	if( !fullscreen )
	{
		x = r_xpos->integer;
		y = r_ypos->integer;

		// adjust window coordinates if necessary
		// so that the window is completely on screen
		if( x < 0 ) x = 0;
		if( y < 0 ) y = 0;

		if( Cvar_VariableInteger( "vid_mode" ) != glConfig.prev_mode )
		{
			// adjust window in the screen size
			if(( x + w > glw_state.desktopWidth ) || ( y + h > glw_state.desktopHeight ))
			{
				x = ( glw_state.desktopWidth - w ) / 2;
				y = ( glw_state.desktopHeight - h ) / 2;
			}
		}
	}

	window = CreateWindowEx( exstyle, WINDOW_NAME, wndname, stylebits, x, y, w, h, NULL, NULL, host.hInst, NULL );

	if( host.hWnd != window )
	{
		// probably never happens
		MsgDev( D_WARN, "VID_CreateWindow: bad hWnd for '%s'\n", wndname );
	}

	// host.hWnd must be filled in IN_WndProc
	if( !host.hWnd )
	{
		MsgDev( D_ERROR, "VID_CreateWindow: couldn't create '%s'\n", wndname );
		return false;
	}

	ShowWindow( host.hWnd, SW_SHOW );
	UpdateWindow( host.hWnd );

	// init all the gl stuff for the window
	if( !GL_SetPixelformat( ))
	{
		ShowWindow( host.hWnd, SW_HIDE );
		DestroyWindow( host.hWnd );
		host.hWnd = NULL;

		UnregisterClass( WINDOW_NAME, host.hInst );
		MsgDev( D_ERROR, "OpenGL driver not installed\n" );

		return false;
	}

	if( !glw_state.initialized )
	{
		if( !GL_CreateContext( ))
			return false;

		VID_StartupGamma();
	}
	else
	{
		if( !GL_UpdateContext( ))
			return false;
	}

	SetForegroundWindow( host.hWnd );
	SetFocus( host.hWnd );

	return true;
}

void VID_DestroyWindow( void )
{
	if( pwglMakeCurrent )
		pwglMakeCurrent( NULL, NULL );

	if( glw_state.hDC )
	{
		ReleaseDC( host.hWnd, glw_state.hDC );
		glw_state.hDC = NULL;
	}

	if( host.hWnd )
	{
		DestroyWindow ( host.hWnd );
		host.hWnd = NULL;
	}

	UnregisterClass( WINDOW_NAME, host.hInst );

	if( glState.fullScreen )
	{
		ChangeDisplaySettings( 0, 0 );
		glState.fullScreen = false;
	}
}

rserr_t R_ChangeDisplaySettings( int vid_mode, qboolean fullscreen )
{
	int	width, height;
	int	cds_result;
	HDC	hDC;

	R_SaveVideoMode( vid_mode );

	width = r_width->integer;
	height = r_height->integer;

	// check our desktop attributes
	hDC = GetDC( GetDesktopWindow( ));
	glw_state.desktopBitsPixel = GetDeviceCaps( hDC, BITSPIXEL );
	glw_state.desktopWidth = GetDeviceCaps( hDC, HORZRES );
	glw_state.desktopHeight = GetDeviceCaps( hDC, VERTRES );
	ReleaseDC( GetDesktopWindow(), hDC );

	// destroy the existing window
	if( host.hWnd ) VID_DestroyWindow();

	// do a CDS if needed
	if( fullscreen )
	{
		DEVMODE	dm;

		Q_memset( &dm, 0, sizeof( dm ));
		dm.dmSize = sizeof( dm );
		dm.dmPelsWidth = width;
		dm.dmPelsHeight = height;
		dm.dmFields = DM_PELSWIDTH|DM_PELSHEIGHT;

		if( vid_displayfrequency->integer > 0 )
		{
			if( vid_displayfrequency->integer < 60 ) Cvar_SetFloat( "vid_displayfrequency", 60 );
			if( vid_displayfrequency->integer > 100 ) Cvar_SetFloat( "vid_displayfrequency", 100 );

			dm.dmFields |= DM_DISPLAYFREQUENCY;
			dm.dmDisplayFrequency = vid_displayfrequency->integer;
		}

		cds_result = ChangeDisplaySettings( &dm, CDS_FULLSCREEN );

		if( cds_result == DISP_CHANGE_SUCCESSFUL )
		{
			glState.fullScreen = true;

			if( !VID_CreateWindow( width, height, true ))
				return rserr_invalid_mode;
			return rserr_ok;
		}
		else if( cds_result == DISP_CHANGE_BADDUALVIEW )
		{
			dm.dmPelsWidth = width * 2;
			dm.dmPelsHeight = height;
			dm.dmFields = DM_PELSWIDTH|DM_PELSHEIGHT;

			// our first CDS failed, so maybe we're running on some weird dual monitor system
			if( ChangeDisplaySettings( &dm, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
			{
				ChangeDisplaySettings( 0, 0 );
				glState.fullScreen = false;
				if( !VID_CreateWindow( width, height, false ))
					return rserr_invalid_mode;
				return rserr_invalid_fullscreen;
			}
			else
			{
				if( !VID_CreateWindow( width, height, true ))
					return rserr_invalid_mode;
				glState.fullScreen = true;
				return rserr_ok;
			}
		}
		else
		{
			int	freq_specified = 0;

			if( vid_displayfrequency->integer > 0 )
			{
				// clear out custom frequency
				freq_specified = vid_displayfrequency->integer;
				Cvar_SetFloat( "vid_displayfrequency", 0.0f );
				dm.dmFields &= ~DM_DISPLAYFREQUENCY;
				dm.dmDisplayFrequency = 0;
			}

			// our first CDS failed, so maybe we're running with too high displayfrequency
			if( ChangeDisplaySettings( &dm, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
			{
				ChangeDisplaySettings( 0, 0 );
				glState.fullScreen = false;
				if( !VID_CreateWindow( width, height, false ))
					return rserr_invalid_mode;
				return rserr_invalid_fullscreen;
			}
			else
			{
				if( !VID_CreateWindow( width, height, true ))
					return rserr_invalid_mode;

				if( freq_specified )
					MsgDev( D_ERROR, "VID_SetMode: display frequency %i Hz not supported by your display\n", freq_specified );
				glState.fullScreen = true;
				return rserr_ok;
			}
		}
	}
	else
	{
		ChangeDisplaySettings( 0, 0 );
		glState.fullScreen = false;
		if( !VID_CreateWindow( width, height, false ))
			return rserr_invalid_mode;
	}

	return rserr_ok;
}

/*
==================
VID_SetMode

Set the described video mode
==================
*/
qboolean VID_SetMode( void )
{
	qboolean	fullscreen;
	rserr_t	err;

	fullscreen = vid_fullscreen->integer;
	gl_swapInterval->modified = true;

	if(( err = R_ChangeDisplaySettings( vid_mode->integer, fullscreen )) == rserr_ok )
	{
		glConfig.prev_mode = vid_mode->integer;
	}
	else
	{
		if( err == rserr_invalid_fullscreen )
		{
			Cvar_SetFloat( "fullscreen", 0 );
			MsgDev( D_ERROR, "VID_SetMode: fullscreen unavailable in this mode\n" );
			if(( err = R_ChangeDisplaySettings( vid_mode->integer, false )) == rserr_ok )
				return true;
		}
		else if( err == rserr_invalid_mode )
		{
			Cvar_SetFloat( "vid_mode", (float)glConfig.prev_mode );
			MsgDev( D_ERROR, "VID_SetMode: invalid mode\n" );
		}

		// try setting it back to something safe
		if(( err = R_ChangeDisplaySettings( glConfig.prev_mode, false )) != rserr_ok )
		{
			MsgDev( D_ERROR, "VID_SetMode: could not revert to safe mode\n" );
			return false;
		}
	}

	return true;
}

/*
==================
VID_CheckChanges

check vid modes and fullscreen
==================
*/
void VID_CheckChanges( void )
{
	if( cl_allow_levelshots->modified )
		  {
		GL_FreeTexture( cls.loadingBar );
		SCR_RegisterTextures(); // reload 'lambda' image
		cl_allow_levelshots->modified = false;
		  }

	if( renderinfo->modified )
	{
		if( !VID_SetMode())
		{
			// can't initialize video subsystem
			Host_NewInstance( va("#%s", GI->gamefolder ), "fallback to dedicated mode\n" );
		}
		else
		{
			renderinfo->modified = false;
			SCR_VidInit(); // tell the client.dll what vid_mode has changed
		}
	}
}

/*
==================
R_Init_OpenGL
==================
*/
qboolean R_Init_OpenGL( void )
{
	Sys_LoadLibrary( &opengl_dll );	// load opengl32.dll

	if( !opengl_dll.link )
		return false;

	GL_CheckExtension( "OpenGL Internal ProcAddress", wglproc_funcs, NULL, GL_WGL_PROCADDRESS );

	return VID_SetMode();
}

/*
==================
R_Free_OpenGL
==================
*/
void R_Free_OpenGL( void )
{
	VID_RestoreGamma ();

	GL_DeleteContext ();

	VID_DestroyWindow ();

	Sys_FreeLibrary( &opengl_dll );

	// now all extensions are disabled
	Q_memset( glConfig.extension, 0, sizeof( glConfig.extension[0] ) * GL_EXTCOUNT );
	glw_state.initialized = false;
}

/*
===============
GL_SetDefaults
===============
*/
static void GL_SetDefaults( void )
{
	int	i;

	pglFinish();

	pglClearColor( 0.5f, 0.5f, 0.5f, 1.0f );

	pglDisable( GL_DEPTH_TEST );
	pglDisable( GL_CULL_FACE );
	pglDisable( GL_SCISSOR_TEST );
	pglDepthFunc( GL_LEQUAL );
	pglDepthMask( GL_FALSE );

	pglColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

	if( glState.stencilEnabled )
	{
		pglDisable( GL_STENCIL_TEST );
		pglStencilMask( ( GLuint ) ~0 );
		pglStencilFunc( GL_EQUAL, 0, ~0 );
		pglStencilOp( GL_KEEP, GL_INCR, GL_INCR );
	}

	pglPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	pglPolygonOffset( -1.0f, -2.0f );

	// properly disable multitexturing at startup
	for( i = (MAX_TEXTURE_UNITS - 1); i > 0; i-- )
	{
		if( i >= GL_MaxTextureUnits( ))
			continue;

		GL_SelectTexture( i );
		pglTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		pglDisable( GL_BLEND );
		pglDisable( GL_TEXTURE_2D );
	}

	GL_SelectTexture( 0 );
	pglDisable( GL_BLEND );
	pglDisable( GL_ALPHA_TEST );
	pglDisable( GL_POLYGON_OFFSET_FILL );
	pglAlphaFunc( GL_GREATER, 0.0f );
	pglEnable( GL_TEXTURE_2D );
	pglShadeModel( GL_FLAT );

	pglPointSize( 1.2f );
	pglLineWidth( 1.2f );

	GL_Cull( 0 );
	GL_FrontFace( 0 );

	R_SetTextureParameters();

	pglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	pglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	pglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	pglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
}

/*
=================
R_RenderInfo_f
=================
*/
void R_RenderInfo_f( void )
{
	Msg( "\n" );
	Msg( "GL_VENDOR: %s\n", glConfig.vendor_string );
	Msg( "GL_RENDERER: %s\n", glConfig.renderer_string );
	Msg( "GL_VERSION: %s\n", glConfig.version_string );
	Msg( "GL_EXTENSIONS: %s\n", glConfig.extensions_string );

	Msg( "GL_MAX_TEXTURE_SIZE: %i\n", glConfig.max_2d_texture_size );

	if( GL_Support( GL_ARB_MULTITEXTURE ))
		Msg( "GL_MAX_TEXTURE_UNITS_ARB: %i\n", glConfig.max_texture_units );
	if( GL_Support( GL_TEXTURECUBEMAP_EXT ))
		Msg( "GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB: %i\n", glConfig.max_cubemap_size );
	if( GL_Support( GL_ANISOTROPY_EXT ))
		Msg( "GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT: %.1f\n", glConfig.max_texture_anisotropy );
	if( glConfig.texRectangle )
		Msg( "GL_MAX_RECTANGLE_TEXTURE_SIZE_NV: %i\n", glConfig.max_2d_rectangle_size );
	if( GL_Support( GL_SHADER_GLSL100_EXT ))
	{
		Msg( "GL_MAX_TEXTURE_COORDS_ARB: %i\n", glConfig.max_texture_coords );
		Msg( "GL_MAX_TEXTURE_IMAGE_UNITS_ARB: %i\n", glConfig.max_teximage_units );
	}

	Msg( "\n" );
	Msg( "MODE: %i, %i x %i %s\n", vid_mode->integer, r_width->integer, r_height->integer );
	Msg( "GAMMA: %s\n", (glConfig.deviceSupportsGamma) ? "hardware" : "software" );
	Msg( "\n" );
	Msg( "PICMIP: %i\n", gl_picmip->integer );
	Msg( "SKYMIP: %i\n", gl_skymip->integer );
	Msg( "TEXTUREMODE: %s\n", gl_texturemode->string );
	Msg( "VERTICAL SYNC: %s\n", gl_swapInterval->integer ? "enabled" : "disabled" );
	Msg( "Color %d bits, Alpha %d bits, Depth %d bits, Stencil %d bits\n", glConfig.color_bits,
		glConfig.alpha_bits, glConfig.depth_bits, glConfig.stencil_bits );
}

//=======================================================================

void GL_InitCommands( void )
{
	Cbuf_AddText( "vidlatch\n" );
	Cbuf_Execute();

	// system screen width and height (don't suppose for change from console at all)
	r_width = Cvar_Get( "width", "640", CVAR_READ_ONLY, "screen width" );
	r_height = Cvar_Get( "height", "480", CVAR_READ_ONLY, "screen height" );
	renderinfo = Cvar_Get( "@renderinfo", "0", CVAR_READ_ONLY, "" ); // use ->modified value only
	r_speeds = Cvar_Get( "r_speeds", "0", CVAR_ARCHIVE, "shows renderer speeds" );
	r_fullbright = Cvar_Get( "r_fullbright", "0", CVAR_CHEAT, "disable lightmaps, get fullbright for entities" );
	r_norefresh = Cvar_Get( "r_norefresh", "0", 0, "disable 3D rendering (use with caution)" );
	r_lighting_extended = Cvar_Get( "r_lighting_extended", "1", CVAR_ARCHIVE, "allow to get lighting from world and bmodels" );
	r_lighting_modulate = Cvar_Get( "r_lighting_modulate", "0.6", CVAR_ARCHIVE, "lightstyles modulate scale" );
	r_lighting_ambient = Cvar_Get( "r_lighting_ambient", "0.3", CVAR_ARCHIVE, "map ambient lighting scale" );
	r_adjust_fov = Cvar_Get( "r_adjust_fov", "1", CVAR_ARCHIVE, "making FOV adjustment for wide-screens" );
	r_novis = Cvar_Get( "r_novis", "0", 0, "ignore vis information (perfomance test)" );
	r_nocull = Cvar_Get( "r_nocull", "0", 0, "ignore frustrum culling (perfomance test)" );
	r_faceplanecull = Cvar_Get( "r_faceplanecull", "1", 0, "ignore face plane culling (perfomance test)" );
	r_detailtextures = Cvar_Get( "r_detailtextures", "1", CVAR_ARCHIVE, "enable detail textures support, use \"2\" for auto-generate mapname_detail.txt" );
	r_lockpvs = Cvar_Get( "r_lockpvs", "0", CVAR_CHEAT, "lockpvs area at current point (pvs test)" );
	r_lockcull = Cvar_Get( "r_lockcull", "0", CVAR_CHEAT, "lock frustrum area at current point (cull test)" );
	r_dynamic = Cvar_Get( "r_dynamic", "1", CVAR_ARCHIVE, "allow dynamic lighting (dlights, lightstyles)" );
	r_lightmap = Cvar_Get( "r_lightmap", "0", CVAR_CHEAT, "lightmap debugging tool" );
	r_fastsky = Cvar_Get( "r_fastsky", "0", CVAR_ARCHIVE, "enable algorhytm fo fast sky rendering (for old machines)" );
	r_drawentities = Cvar_Get( "r_drawentities", "1", CVAR_CHEAT|CVAR_ARCHIVE, "render entities" );
	r_flaresize = Cvar_Get( "r_flaresize", "200", CVAR_ARCHIVE, "set flares size" );
	r_lefthand = Cvar_Get( "hand", "0", CVAR_ARCHIVE, "viewmodel handedness" );
	r_decals = Cvar_Get( "r_decals", "4096", CVAR_ARCHIVE, "sets the maximum number of decals" );
	r_xpos = Cvar_Get( "r_xpos", "130", CVAR_GLCONFIG, "window position by horizontal" );
	r_ypos = Cvar_Get( "r_ypos", "48", CVAR_GLCONFIG, "window position by vertical" );

	gl_picmip = Cvar_Get( "gl_picmip", "0", CVAR_GLCONFIG, "reduces resolution of textures by powers of 2" );
	gl_skymip = Cvar_Get( "gl_skymip", "0", CVAR_GLCONFIG, "reduces resolution of skybox textures by powers of 2" );
	gl_ignorehwgamma = Cvar_Get( "gl_ignorehwgamma", "0", CVAR_GLCONFIG, "ignore hardware gamma" );
	gl_allow_software = Cvar_Get( "gl_allow_software", "0", CVAR_ARCHIVE, "allow OpenGL software emulation" );
	gl_alphabits = Cvar_Get( "gl_alphabits", "8", CVAR_GLCONFIG, "pixelformat alpha bits (0 - auto)" );
	gl_texturemode = Cvar_Get( "gl_texturemode", "GL_LINEAR_MIPMAP_LINEAR", CVAR_ARCHIVE, "texture filter" );
	gl_round_down = Cvar_Get( "gl_round_down", "0", CVAR_GLCONFIG, "down size non-power of two textures" );
	gl_max_size = Cvar_Get( "gl_max_size", "512", CVAR_ARCHIVE, "no effect in Xash3D just a legacy" );
	gl_stencilbits = Cvar_Get( "gl_stencilbits", "8", CVAR_GLCONFIG, "pixelformat stencil bits (0 - auto)" );
	gl_check_errors = Cvar_Get( "gl_check_errors", "1", CVAR_ARCHIVE, "ignore video engine errors" );
	gl_swapInterval = Cvar_Get( "gl_swapInterval", "0", CVAR_ARCHIVE,  "time beetween frames (in msec)" );
	gl_extensions = Cvar_Get( "gl_extensions", "1", CVAR_GLCONFIG, "allow gl_extensions" );
	gl_detailscale = Cvar_Get( "gl_detailscale", "4.0", CVAR_ARCHIVE, "default scale applies while auto-generate list of detail textures" );
	gl_texture_anisotropy = Cvar_Get( "gl_anisotropy", "2.0", CVAR_ARCHIVE, "textures anisotropic filter" );
	gl_texture_lodbias =  Cvar_Get( "gl_texture_lodbias", "0.0", CVAR_ARCHIVE, "LOD bias for mipmapped textures" );
	gl_compress_textures = Cvar_Get( "gl_compress_textures", "0", CVAR_GLCONFIG, "compress textures to safe video memory" );
	gl_luminance_textures = Cvar_Get( "gl_luminance_textures", "0", CVAR_GLCONFIG, "force all textures to luminance" );
	gl_compensate_gamma_screenshots = Cvar_Get( "gl_compensate_gamma_screenshots", "0", CVAR_ARCHIVE, "allow to apply gamma value for screenshots and snapshots" );
	gl_keeptjunctions = Cvar_Get( "gl_keeptjunctions", "1", CVAR_ARCHIVE, "disable to reduce vertexes count but removing tjuncs causes blinking pixels" );
	gl_allow_static = Cvar_Get( "gl_allow_static", "0", CVAR_ARCHIVE, "force to drawing non-moveable brushes as part of world (save FPS)" );
	gl_allow_mirrors = Cvar_Get( "gl_allow_mirrors", "1", CVAR_ARCHIVE, "allow to draw mirror surfaces" );
	gl_showtextures = Cvar_Get( "r_showtextures", "0", CVAR_CHEAT, "show all uploaded textures (type values from 1 to 13)" );
	gl_finish = Cvar_Get( "gl_finish", "0", CVAR_ARCHIVE, "use glFinish instead of glFlush" );
	gl_nosort = Cvar_Get( "gl_nosort", "0", CVAR_ARCHIVE, "disable sorting of translucent surfaces" );
	gl_clear = Cvar_Get( "gl_clear", "0", CVAR_ARCHIVE, "clearing screen after each frame" );
	gl_test = Cvar_Get( "gl_test", "0", 0, "engine developer cvar for quick testing new features" );
	gl_wireframe = Cvar_Get( "gl_wireframe", "0", 0, "show wireframe overlay" );
	gl_overview = Cvar_Get( "dev_overview", "0", 0, "show level overview" );

	// these cvar not used by engine but some mods requires this
	Cvar_Get( "gl_polyoffset", "-0.1", 0, "polygon offset for decals" );

	// make sure r_swapinterval is checked after vid_restart
	gl_swapInterval->modified = true;

	vid_gamma = Cvar_Get( "gamma", "1.0", CVAR_ARCHIVE, "gamma amount" );
	vid_texgamma = Cvar_Get( "texgamma", "2.2", CVAR_GLCONFIG, "texgamma amount (default Half-Life artwork gamma)" );
	vid_mode = Cvar_Get( "vid_mode", VID_DEFAULTMODE, CVAR_RENDERINFO, "display resolution mode" );
	vid_fullscreen = Cvar_Get( "fullscreen", "0", CVAR_RENDERINFO, "set in 1 to enable fullscreen mode" );
	vid_displayfrequency = Cvar_Get ( "vid_displayfrequency", "0", CVAR_RENDERINFO, "fullscreen refresh rate" );

	Cmd_AddCommand( "r_info", R_RenderInfo_f, "display renderer info" );
	Cmd_AddCommand( "texturelist", R_TextureList_f, "display loaded textures list" );
}

void GL_RemoveCommands( void )
{
	Cmd_RemoveCommand( "r_info");
	Cmd_RemoveCommand( "texturelist" );
}

void GL_InitExtensions( void )
{
	// initialize gl extensions
	GL_CheckExtension( "OpenGL 1.1.0", opengl_110funcs, NULL, GL_OPENGL_110 );

	// get our various GL strings
	glConfig.vendor_string = reinterpret_cast<const char*>(pglGetString( GL_VENDOR ));
	glConfig.renderer_string = reinterpret_cast<const char*>(pglGetString( GL_RENDERER ));
	glConfig.version_string = reinterpret_cast<const char*>(pglGetString( GL_VERSION ));
	glConfig.extensions_string = reinterpret_cast<const char*>(pglGetString( GL_EXTENSIONS ));
	MsgDev( D_INFO, "Video: %s\n", glConfig.renderer_string );

	// initalize until base opengl functions loaded
	GL_CheckExtension( "WGL_EXT_swap_control", wglswapintervalfuncs, NULL, GL_WGL_SWAPCONTROL );

	GL_CheckExtension( "glDrawRangeElements", drawrangeelementsfuncs, "gl_drawrangeelments", GL_DRAW_RANGEELEMENTS_EXT );

	if( !GL_Support( GL_DRAW_RANGEELEMENTS_EXT ))
		GL_CheckExtension( "GL_EXT_draw_range_elements", drawrangeelementsextfuncs, "gl_drawrangeelments", GL_DRAW_RANGEELEMENTS_EXT );

	// multitexture
	glConfig.max_texture_units = glConfig.max_texture_coords = glConfig.max_teximage_units = 1;
	GL_CheckExtension( "GL_ARB_multitexture", multitexturefuncs, "gl_arb_multitexture", GL_ARB_MULTITEXTURE );

	if( GL_Support( GL_ARB_MULTITEXTURE ))
	{
		pglGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, &glConfig.max_texture_units );
		GL_CheckExtension( "GL_ARB_texture_env_combine", NULL, "gl_texture_env_combine", GL_ENV_COMBINE_EXT );

		if( !GL_Support( GL_ENV_COMBINE_EXT ))
			GL_CheckExtension( "GL_EXT_texture_env_combine", NULL, "gl_texture_env_combine", GL_ENV_COMBINE_EXT );

		if( GL_Support( GL_ENV_COMBINE_EXT ))
			GL_CheckExtension( "GL_ARB_texture_env_dot3", NULL, "gl_texture_env_dot3", GL_DOT3_ARB_EXT );
	}
	else
	{
		GL_CheckExtension( "GL_SGIS_multitexture", sgis_multitexturefuncs, "gl_sgis_multitexture", GL_ARB_MULTITEXTURE );
		if( GL_Support( GL_ARB_MULTITEXTURE )) glConfig.max_texture_units = 2;
	}

	if( glConfig.max_texture_units == 1 )
		GL_SetExtension( GL_ARB_MULTITEXTURE, false );

	// 3d texture support
	GL_CheckExtension( "GL_EXT_texture3D", texture3dextfuncs, "gl_texture_3d", GL_TEXTURE_3D_EXT );

	if( GL_Support( GL_TEXTURE_3D_EXT ))
	{
		pglGetIntegerv( GL_MAX_3D_TEXTURE_SIZE, &glConfig.max_3d_texture_size );

		if( glConfig.max_3d_texture_size < 32 )
		{
			GL_SetExtension( GL_TEXTURE_3D_EXT, false );
			MsgDev( D_ERROR, "GL_EXT_texture3D reported bogus GL_MAX_3D_TEXTURE_SIZE, disabled\n" );
		}
	}

	GL_CheckExtension( "GL_SGIS_generate_mipmap", NULL, "gl_sgis_generate_mipmaps", GL_SGIS_MIPMAPS_EXT );

	// hardware cubemaps
	GL_CheckExtension( "GL_ARB_texture_cube_map", NULL, "gl_texture_cubemap", GL_TEXTURECUBEMAP_EXT );

	if( GL_Support( GL_TEXTURECUBEMAP_EXT ))
		pglGetIntegerv( GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB, &glConfig.max_cubemap_size );

	// point particles extension
	GL_CheckExtension( "GL_EXT_point_parameters", pointparametersfunc, NULL, GL_EXT_POINTPARAMETERS );

	GL_CheckExtension( "GL_ARB_texture_non_power_of_two", NULL, "gl_texture_npot", GL_ARB_TEXTURE_NPOT_EXT );
	GL_CheckExtension( "GL_ARB_texture_compression", texturecompressionfuncs, "gl_dds_hardware_support", GL_TEXTURE_COMPRESSION_EXT );
	GL_CheckExtension( "GL_EXT_compiled_vertex_array", compiledvertexarrayfuncs, "gl_cva_support", GL_CUSTOM_VERTEX_ARRAY_EXT );

	if( !GL_Support( GL_CUSTOM_VERTEX_ARRAY_EXT ))
		GL_CheckExtension( "GL_SGI_compiled_vertex_array", compiledvertexarrayfuncs, "gl_cva_support", GL_CUSTOM_VERTEX_ARRAY_EXT );

	GL_CheckExtension( "GL_EXT_texture_edge_clamp", NULL, "gl_clamp_to_edge", GL_CLAMPTOEDGE_EXT );

	if( !GL_Support( GL_CLAMPTOEDGE_EXT ))
		GL_CheckExtension("GL_SGIS_texture_edge_clamp", NULL, "gl_clamp_to_edge", GL_CLAMPTOEDGE_EXT );

	glConfig.max_texture_anisotropy = 0.0f;
	GL_CheckExtension( "GL_EXT_texture_filter_anisotropic", NULL, "gl_ext_anisotropic_filter", GL_ANISOTROPY_EXT );

	if( GL_Support( GL_ANISOTROPY_EXT ))
		pglGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &glConfig.max_texture_anisotropy );

	GL_CheckExtension( "GL_EXT_texture_lod_bias", NULL, "gl_ext_texture_lodbias", GL_TEXTURE_LODBIAS );
	if( GL_Support( GL_TEXTURE_LODBIAS ))
		pglGetFloatv( GL_MAX_TEXTURE_LOD_BIAS_EXT, &glConfig.max_texture_lodbias );

	GL_CheckExtension( "GL_ARB_texture_border_clamp", NULL, "gl_ext_texborder_clamp", GL_CLAMP_TEXBORDER_EXT );

	GL_CheckExtension( "GL_EXT_blend_minmax", blendequationfuncs, "gl_ext_customblend", GL_BLEND_MINMAX_EXT );
	GL_CheckExtension( "GL_EXT_blend_subtract", blendequationfuncs, "gl_ext_customblend", GL_BLEND_SUBTRACT_EXT );

	GL_CheckExtension( "glStencilOpSeparate", gl2separatestencilfuncs, "gl_separate_stencil", GL_SEPARATESTENCIL_EXT );

	if( !GL_Support( GL_SEPARATESTENCIL_EXT ))
		GL_CheckExtension("GL_ATI_separate_stencil", atiseparatestencilfuncs, "gl_separate_stencil", GL_SEPARATESTENCIL_EXT );

	GL_CheckExtension( "GL_EXT_stencil_two_side", stenciltwosidefuncs, "gl_stenciltwoside", GL_STENCILTWOSIDE_EXT );
	GL_CheckExtension( "GL_ARB_vertex_buffer_object", vbofuncs, "gl_vertex_buffer_object", GL_ARB_VERTEX_BUFFER_OBJECT_EXT );

	// we don't care if it's an extension or not, they are identical functions, so keep it simple in the rendering code
	if( pglDrawRangeElementsEXT == NULL ) pglDrawRangeElementsEXT = pglDrawRangeElements;

	GL_CheckExtension( "GL_ARB_texture_env_add", NULL, "gl_texture_env_add", GL_TEXTURE_ENV_ADD_EXT );

	// vp and fp shaders
	GL_CheckExtension( "GL_ARB_shader_objects", shaderobjectsfuncs, "gl_shaderobjects", GL_SHADER_OBJECTS_EXT );
	GL_CheckExtension( "GL_ARB_shading_language_100", NULL, "gl_glslprogram", GL_SHADER_GLSL100_EXT );
	GL_CheckExtension( "GL_ARB_vertex_shader", vertexshaderfuncs, "gl_vertexshader", GL_VERTEX_SHADER_EXT );
	GL_CheckExtension( "GL_ARB_fragment_shader", NULL, "gl_pixelshader", GL_FRAGMENT_SHADER_EXT );

	GL_CheckExtension( "GL_ARB_depth_texture", NULL, "gl_depthtexture", GL_DEPTH_TEXTURE );
	GL_CheckExtension( "GL_ARB_shadow", NULL, "gl_arb_shadow", GL_SHADOW_EXT );

	GL_CheckExtension( "GL_ARB_texture_float", NULL, "gl_arb_texture_float", GL_ARB_TEXTURE_FLOAT_EXT );

	// occlusion queries
	GL_CheckExtension( "GL_ARB_occlusion_query", occlusionfunc, "gl_occlusion_queries", GL_OCCLUSION_QUERIES_EXT );

	if( GL_Support( GL_SHADER_GLSL100_EXT ))
	{
		pglGetIntegerv( GL_MAX_TEXTURE_COORDS_ARB, &glConfig.max_texture_coords );
		pglGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &glConfig.max_teximage_units );
	}
	else
	{
		// just get from multitexturing
		glConfig.max_texture_coords = glConfig.max_teximage_units = glConfig.max_texture_units;
	}

	// rectangle textures support
	if( Q_strstr( glConfig.extensions_string, "GL_NV_texture_rectangle" ))
	{
		glConfig.texRectangle = GL_TEXTURE_RECTANGLE_NV;
		pglGetIntegerv( GL_MAX_RECTANGLE_TEXTURE_SIZE_NV, &glConfig.max_2d_rectangle_size );
	}
	else if( Q_strstr( glConfig.extensions_string, "GL_EXT_texture_rectangle" ))
	{
		glConfig.texRectangle = GL_TEXTURE_RECTANGLE_EXT;
		pglGetIntegerv( GL_MAX_RECTANGLE_TEXTURE_SIZE_EXT, &glConfig.max_2d_rectangle_size );
	}
	else glConfig.texRectangle = glConfig.max_2d_rectangle_size = 0; // no rectangle

	glConfig.max_2d_texture_size = 0;
	pglGetIntegerv( GL_MAX_TEXTURE_SIZE, &glConfig.max_2d_texture_size );
	if( glConfig.max_2d_texture_size <= 0 ) glConfig.max_2d_texture_size = 256;

	Cvar_Get( "gl_max_texture_size", "0", CVAR_INIT, "opengl texture max dims" );
	Cvar_Set( "gl_max_texture_size", va( "%i", glConfig.max_2d_texture_size ));

	// MCD has buffering issues
	if(Q_strstr( glConfig.renderer_string, "gdi" ))
		Cvar_SetFloat( "gl_finish", 1 );

	Cvar_Set( "gl_anisotropy", va( "%f", bound( 0, gl_texture_anisotropy->value, glConfig.max_texture_anisotropy )));

	// software mipmap generator does wrong result with NPOT textures ...
	if( !GL_Support( GL_SGIS_MIPMAPS_EXT ))
		GL_SetExtension( GL_ARB_TEXTURE_NPOT_EXT, false );

	glw_state.initialized = true;

	tr.framecount = tr.visframecount = 1;
}

/*
===============
R_Init
===============
*/
qboolean R_Init( void )
{
	if( glw_state.initialized )
		return true;

	// give initial OpenGL configuration
	Cbuf_AddText( "exec opengl.cfg\n" );

	GL_InitCommands();
	GL_SetDefaultState();

	// create the window and set up the context
	if( !R_Init_OpenGL( ))
	{
		GL_RemoveCommands();
		R_Free_OpenGL();

		// can't initialize video subsystem
		Host_NewInstance( va("#%s", GI->gamefolder ), "fallback to dedicated mode\n" );
		return false;
	}

	renderinfo->modified = false;
	r_temppool = Mem_AllocPool( "Render Zone" );

	GL_InitExtensions();
	GL_SetDefaults();
	R_InitImages();
	R_SpriteInit();
	R_StudioInit();
	R_ClearDecals();
	R_ClearScene();

	// initialize screen
	SCR_Init();

	return true;
}

/*
===============
R_Shutdown
===============
*/
void R_Shutdown( void )
{
	int	i;

	if( !glw_state.initialized )
		return;

	// release SpriteTextures
	for( i = 1; i < MAX_IMAGES; i++ )
	{
		if( !clgame.sprites[i].name[0] ) continue;
		Mod_UnloadSpriteModel( &clgame.sprites[i] );
	}
	Q_memset( clgame.sprites, 0, sizeof( clgame.sprites ));

	GL_RemoveCommands();
	R_ShutdownImages();

	Mem_FreePool( &r_temppool );

	// shut down OS specific OpenGL stuff like contexts, etc.
	R_Free_OpenGL();
}

/*
=================
GL_CheckForErrors
=================
*/
void GL_CheckForErrors_( const char *filename, const int fileline )
{
	int	err;
	char	*str;

	if( !gl_check_errors->integer )
		return;

	if(( err = pglGetError( )) == GL_NO_ERROR )
		return;

	switch( err )
	{
	case GL_STACK_OVERFLOW:
		str = "GL_STACK_OVERFLOW";
		break;
	case GL_STACK_UNDERFLOW:
		str = "GL_STACK_UNDERFLOW";
		break;
	case GL_INVALID_ENUM:
		str = "GL_INVALID_ENUM";
		break;
	case GL_INVALID_VALUE:
		str = "GL_INVALID_VALUE";
		break;
	case GL_INVALID_OPERATION:
		str = "GL_INVALID_OPERATION";
		break;
	case GL_OUT_OF_MEMORY:
		str = "GL_OUT_OF_MEMORY";
		break;
	default:
		str = "UNKNOWN ERROR";
		break;
	}

	Host_Error( "GL_CheckForErrors: %s (called at %s:%i)\n", str, filename, fileline );
}
