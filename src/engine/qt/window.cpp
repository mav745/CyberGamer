#include "c_gate.h"
#include "window.hpp"

#include <QDebug>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

Window::Window(QWidget *parent)
	: QOpenGLWidget(parent)
{
	theTimer.start(5, Qt::PreciseTimer, this);
}

void Window::drawQuad(float x, float y, float w, float h, float *tcoord, GLuint tex)
{
	GLfloat vertices[] = {	x,		y,		// bottom left corner
							x+w,	y,		// top left corner
							x+w,	y+h,	// top right corner
							x,		y+h };	// bottom right corner
	GLfloat uv[] = {	0, 0,	// bottom left corner
						1, 0,	// top left corner
						1, 1,	// top right corner
						0, 1};	// bottom right corner
	if(!tcoord)
		tcoord = uv;

	QOpenGLBuffer vertexPositionBuffer(QOpenGLBuffer::VertexBuffer);
	vertexPositionBuffer.create();
	vertexPositionBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vertexPositionBuffer.bind();
	vertexPositionBuffer.allocate(vertices, 12 * sizeof(float));

	QOpenGLBuffer texCoordBuffer(QOpenGLBuffer::VertexBuffer);
	texCoordBuffer.create();
	texCoordBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	texCoordBuffer.bind();
	texCoordBuffer.allocate(tcoord, 12 * sizeof(float));

	program.bind();

	vertexPositionBuffer.bind();
	program.enableAttributeArray("position");
	program.setAttributeBuffer("position", GL_FLOAT, 0, 2);

	texCoordBuffer.bind();
	program.enableAttributeArray("coord");
	program.setAttributeBuffer("coord", GL_FLOAT, 0, 2);

	program.setUniformValue("ortho", m_ortho);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);

	program.setUniformValue("texture", 0);

	glDrawArrays(GL_QUADS, 0, 4);
}

void Window::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);


	program.addShaderFromSourceCode(
		QOpenGLShader::Vertex,
		"#version 330\n"
		"in vec2 position;\n"
		"in vec2 coord;\n"
		"out vec2 fragCoord;\n"
		"uniform mat4 ortho;"
		"void main() {\n"
		"    gl_Position = ortho*vec4(position.x, position.y, 0.0, 1.0);\n"
		"    fragCoord = coord;\n"
		"}\n"
	);
	program.addShaderFromSourceCode(
		QOpenGLShader::Fragment,
		"#version 330\n"
		"in vec2 fragCoord;\n"
		"out vec4 color;\n"
		"uniform sampler2D texture;"
		"void main() {\n"
		"    color = (texture2D(texture, fragCoord.st)+vec4(1.0, fragCoord.x, fragCoord.y, 1.0))*0.5; /*vec4(1.0, fragCoord.x, fragCoord.y, 1.0);*/\n"
		"}\n"
	);
	program.link();
}

void Window::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	// Update projection matrix and other size related settings:
	m_projection.setToIdentity();
	m_projection.perspective(70.0f, w / float(h), 0.01f, 1000000.0f);

	//float aratio = (float)w / h;
	m_ortho.setToIdentity();
	m_ortho.ortho(0, w, h, 0, -1.0f, 1.0f);
}

void Window::paintGL()
{
	glClearColor(0.0f, 0.0f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SCR_UpdateScreen( );
	//R_DrawStretchRaw( 10, 10, 100, 100, 2, 2, 0, 0 );
	//drawQuad(10, 10, 100, 100, 0);
	//drawQuad(0.10,  0.10, 0.85, 0.85, 0);
	//drawQuad(5,  5, 2, 7, 0);
	//R_DrawStretchRaw( 0.10,  0.10, 0.89, 0.89, 2, 2, 0, 0 );
	//qDebug() << "painted oglw";
}

void Window::timerEvent(QTimerEvent *te)
{
	Q_UNUSED(te);
	update();
}
