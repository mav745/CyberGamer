#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include "VGUI_KeyCode.h"

class Window : public QOpenGLWidget, public QOpenGLFunctions
{
	Q_OBJECT
public:
	Window(QWidget *parent = 0);
	~Window();

	void glBegin(GLenum mode);
	void glEnd();

	void glShadeModel(GLenum mode);

	void glTexGeniv(GLenum coord, GLenum pname, const GLint * params);
	void glTexGenfv(GLenum coord, GLenum pname, const GLfloat * params);
	void glTexGendv(GLenum coord, GLenum pname, const GLdouble * params);

	void glClipPlane(GLenum plane, const GLdouble *equation);
	void glPolygonMode(GLenum face, GLenum mode);
	void glPointSize(GLfloat size);
	
	void _glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	void _glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);

	void glDrawBuffer(GLenum buf);
	void glDepthRange(GLclampd nearVal, GLclampd farVal);


	void glVertex2f(float x, float y);
	void glVertex3f(float x, float y, float z);
	void glVertex3fv(const float *v);
	void glNormal3fv(const float *v);
	
	void glTexGeni(	GLenum coord, GLenum pname, GLint param);
	void glTexGenf(	GLenum coord, GLenum pname, GLfloat param);
	void glTexGend(	GLenum coord, GLenum pname, GLdouble param);

	void glTexCoord2f(float u, float v);
	void glMultiTexCoord2f(GLenum target, GLfloat s, GLfloat t);
	
	void glMatrixMode( GLenum mode );
	void glLoadIdentity();
	void glLoadMatrixf(const float *m);
	void glColor4f(float r, float g, float b, float a);
	void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
	void glColor4ub(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
	void glColor3ub(GLubyte r, GLubyte g, GLubyte b);
	void glColor4ubv(GLubyte *v);
	void glColor3f(float r, float g, float b);

	void glDeleteTextures(int n, const GLuint *ts);
	void glFogfv(int param, const float *v);
	void glFogi(int param, int v);
	void glFogf(int param, float v);
	void glTexEnvf(GLenum target, GLenum pname, GLfloat param);
	void glTexEnvi(GLenum target, GLenum pname, GLint param);
	void glAlphaFunc(GLenum func, GLclampf ref);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

private:
	QBasicTimer theTimer;
	void timerEvent(QTimerEvent *te); // window frame
	bool eventFilter(QObject *target, QEvent *event);

	QOpenGLShaderProgram program;
	QMatrix4x4 projection;
	QMatrix4x4 modelview;
	QMatrix4x4 texmat;

	bool hasBegun;
	bool isPainting;
	GLenum drawMode;
	GLenum matMode;

	QOpenGLBuffer drawVerts;
	QOpenGLBuffer drawColors;
	QOpenGLBuffer drawCoords;
	float *tempVerts;
	float *tempColors;
	float *tempCoords;
	int numVerts;
	int numCoords;
	QVector4D drawColor;

};

#endif // WINDOW_HPP
