#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class Window : public QOpenGLWidget, public QOpenGLFunctions
{
	Q_OBJECT
public:
	Window(QWidget *parent = 0);

	void glBegin(GLenum mode);
	void glEnd();

	void glVertex2f(float x, float y);
	void glVertex3f(float x, float y, float z);
	void glVertex3fv(const float *v);
	void glTexCoord2f(float u, float v);
	void glMatrixMode( GLenum mode );
	void glLoadIdentity();
	void glLoadMatrixf(const float *m );
	void glColor4f(float r, float g, float b, float a);
	void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
	void glColor4ub(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
	void glColor3ub(GLubyte r, GLubyte g, GLubyte b);
	void glColor4ubv(GLubyte *v);
	void glColor3f(float r, float g, float b);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

private:
	QBasicTimer theTimer;
	void timerEvent(QTimerEvent *te); // window frame

	QOpenGLShaderProgram program;
	QMatrix4x4 projection;
	QMatrix4x4 modelview;
	QMatrix4x4 texmat;

	bool hasBegun;
	GLenum drawMode;
	GLenum matMode;

	QOpenGLBuffer drawVerts;
	QOpenGLBuffer drawCoords;
	QVector<float> tempVerts;
	QVector<float> tempCoords;
	QVector4D drawColor;

};

#endif // WINDOW_HPP
