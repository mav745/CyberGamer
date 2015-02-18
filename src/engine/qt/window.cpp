#include "c_gate.h"
#include "window.hpp"

#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include <QGraphicsDropShadowEffect>

Window::Window(QWidget *parent)
	: QOpenGLWidget(parent)
	, hasBegun(false)
	, drawMode(GL_TRIANGLES)
	, matMode(GL_PROJECTION)
	, drawColor(QVector4D(1.f,1.f,1.f,1.f))
{
	projection.setToIdentity();
	modelview.setToIdentity();
	texmat.setToIdentity();

	theTimer.start(5, Qt::PreciseTimer, this);

	QLabel *lawl = new QLabel(this);
	lawl->setStyleSheet("QLabel { font: bold 25px; color: #eebb34; }");
	lawl->setText("Fuck the VGUI!");
	lawl->resize(220, 50);
	lawl->setGeometry(QRect(QPoint(100,50), lawl->size()));

	QGraphicsDropShadowEffect *dse = new QGraphicsDropShadowEffect();
	dse->setBlurRadius(10);
	dse->setColor(0xffff66);
	dse->setOffset(2.f);
	lawl->setGraphicsEffect(dse);
}

void Window::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	program.addShaderFromSourceFile(QOpenGLShader::Vertex, "advanced/shaders/normal.vert");
	program.addShaderFromSourceFile(QOpenGLShader::Fragment, "advanced/shaders/normal.frag");
	program.link();

	drawVerts = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	drawVerts.create();
	drawVerts.setUsagePattern(QOpenGLBuffer::DynamicDraw);

	drawCoords = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	drawCoords.create();
	drawCoords.setUsagePattern(QOpenGLBuffer::DynamicDraw);
}

void Window::resizeGL(int w, int h)
{
	//glViewport(0, 0, w, h);
//	// Update projection matrix and other size related settings:
	//projection.setToIdentity();
	//projection.perspective(70.0f, w / float(h), 0.01f, 1000000.0f);

//	//float aratio = (float)w / h;
	//projection.setToIdentity();
	//projection.ortho(0, w, h, 0, -1.0f, 1.0f);
}

void Window::paintGL()
{
	glClearColor(0.0f, 0.0f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SCR_UpdateScreen( );
}

void Window::timerEvent(QTimerEvent *te)
{
	Q_UNUSED(te);
	update();
}

void Window::glBegin(GLenum mode)
{
	if(!isValid())
	{
		qDebug() << "glBegin("<<mode<<") called before widget is ready!";
		hasBegun = false;
		return;
	}

	drawMode = mode;
	hasBegun = true;
	tempVerts.clear();
	tempCoords.clear();
}

void Window::glEnd()
{
	if(!hasBegun)
	{
		qDebug() << "glEnd() with no glBegin()!";
		return;
	}
	hasBegun = false;
	int vc = tempVerts.size(), cc = tempCoords.size();
	if(!vc || vc != (cc+cc/2))
	{
		qDebug() << "glEnd(): Vertex buffer is ill-formed!";
		return;
	}

	program.bind();

	drawVerts.bind();
	drawVerts.allocate(tempVerts.constData(), vc * sizeof(float));
	program.enableAttributeArray("pos");
	program.setAttributeBuffer("pos", GL_FLOAT, 0, 3);

	drawCoords.bind();
	drawCoords.allocate(tempCoords.constData(), cc * sizeof(float));
	program.enableAttributeArray("uv");
	program.setAttributeBuffer("uv", GL_FLOAT, 0, 2);

	//QMatrix4x4 &curMat = matMode == GL_MODELVIEW? modelview:
	//					matMode == GL_PROJECTION? projection:
	//											  texmat;
	program.setUniformValue("mat", projection*modelview);
	program.setUniformValue("texture", 0);
	program.setUniformValue("col", drawColor);

	glDrawArrays(drawMode, 0, vc);
}

void Window::glVertex2f(float x, float y)
{
	tempVerts << x << y << 0.f;
}

void Window::glVertex3f(float x, float y, float z)
{
	tempVerts << x << y << z;
}

void Window::glVertex3fv(const float *v)
{
	tempVerts << v[0] << v[1] << v[2];
}

void Window::glTexCoord2f(float u, float v)
{
	tempCoords << u << v;
}

void Window::glMatrixMode(GLenum mode)
{
	matMode = mode;
}

void Window::glLoadIdentity()
{
	QMatrix4x4 &curMat = matMode == GL_MODELVIEW? modelview:
						matMode == GL_PROJECTION? projection:
												  texmat;
	curMat.setToIdentity();
}

void Window::glLoadMatrixf(const float *m)
{
	QMatrix4x4 &curMat = matMode == GL_MODELVIEW? modelview:
						matMode == GL_PROJECTION? projection:
												  texmat;
	memcpy(curMat.data(), m, 16*sizeof(float));
	curMat.optimize();
}

void Window::glColor4f(float r, float g, float b, float a)
{
	drawColor = QVector4D(r, g, b, a);
}

void Window::glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	QMatrix4x4 &curMat = matMode == GL_MODELVIEW? modelview:
						matMode == GL_PROJECTION? projection:
												  texmat;
	curMat.ortho(left, right, bottom, top, zNear, zFar);
}

void Window::glColor4ub(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
{
	drawColor = QVector4D(r, g, b, a)*(1.f/255.f);
}

void Window::glColor3ub(GLubyte r, GLubyte g, GLubyte b)
{
	glColor4ub(r,g,b,255);
}

void Window::glColor4ubv(GLubyte *v)
{
	drawColor = QVector4D(v[0], v[1], v[2], v[3])*(1.f/255.f);
}

void Window::glColor3f(float r, float g, float b)
{
	drawColor = QVector4D(r, g, b, 1);
}

