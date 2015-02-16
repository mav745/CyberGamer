#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>

class Window : public QOpenGLWidget, public QOpenGLFunctions
{
	Q_OBJECT
public:
	Window(QWidget *parent = 0);

	void drawQuad(float x, float y, float w, float h, float *tcoord, GLuint tex);


protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

private:
	QBasicTimer theTimer;
	void timerEvent(QTimerEvent *te); // window frame

	QMatrix4x4 m_projection;
	QMatrix4x4 m_ortho;

	QOpenGLShaderProgram program;

};

#endif // WINDOW_HPP
