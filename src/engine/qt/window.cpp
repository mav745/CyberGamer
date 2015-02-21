#include "c_gate.h"
#include "window.hpp"

#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include <QGraphicsDropShadowEffect>
#include <QIcon>
#include <QMouseEvent>

Window::Window(QWidget *parent)
	: QOpenGLWidget(parent)
	, hasBegun(false)
	, drawMode(GL_TRIANGLES)
	, matMode(GL_PROJECTION)
	, drawColor(QVector4D(1.f,1.f,1.f,1.f))
{
	//setFocusPolicy(Qt::StrongFocus);
	setWindowIcon(QIcon("advanced/game.ico"));

	projection.setToIdentity();
	modelview.setToIdentity();
	texmat.setToIdentity();

	//theTimer.start(5, Qt::PreciseTimer, this);


//	QLabel *lawl = new QLabel(this);
//	lawl->setStyleSheet("QLabel { font: bold 25px; color: #eebb34; }");
//	lawl->setText("Fuck the VGUI!");
//	lawl->resize(220, 50);
//	lawl->setGeometry(QRect(QPoint(100,50), lawl->size()));

//	QGraphicsDropShadowEffect *dse = new QGraphicsDropShadowEffect();
//	dse->setBlurRadius(10);
//	dse->setColor(0xffff66);
//	dse->setOffset(2.f);
//	lawl->setGraphicsEffect(dse);
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

	QApplication::instance()->installEventFilter(this);
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

#include "vgui_main.h"

bool Window::eventFilter(QObject *target, QEvent *event)
{
	if(target != this)
		return false;

	if(VGui_GetPanel( ))
	{
		SurfaceBase *surface = NULL;
		CEnginePanel *panel = NULL;
		App *pApp= NULL;
		panel = (CEnginePanel *)VGui_GetPanel();
		surface = panel->getSurfaceBase();
		pApp = panel->getApp();
		if (event->type() == QEvent::MouseMove)
		{
			QMouseEvent *moveEvent = static_cast<QMouseEvent*>(event);
			//qDebug() << "cursor" << moveEvent->x()<< moveEvent->y();
			pApp->internalCursorMoved(moveEvent->x(), moveEvent->y(), surface );
		}
		else if(event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
			int k = keyEvent->key();
			if(!( k & ( 1 << 30 )))
				pApp->internalKeyPressed( VGUI_MapKey( k ), surface );
			pApp->internalKeyTyped( VGUI_MapKey( k ), surface );
		}
		else if(event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease)
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
			if(mouseEvent->button() == Qt::LeftButton)
				event->type() == QEvent::MouseButtonPress
						? pApp->internalMousePressed( MOUSE_LEFT, surface )
						: pApp->internalMouseReleased( MOUSE_LEFT, surface );
			else if(mouseEvent->button() == Qt::RightButton)
				event->type() == QEvent::MouseButtonPress
						? pApp->internalMousePressed( MOUSE_RIGHT, surface )
						: pApp->internalMouseReleased( MOUSE_RIGHT, surface );
			else if(mouseEvent->button() == Qt::MiddleButton)
				event->type() == QEvent::MouseButtonPress
						? pApp->internalMousePressed( MOUSE_MIDDLE, surface )
						: pApp->internalMouseReleased( MOUSE_MIDDLE, surface );
		}
		else if(event->type() == QEvent::MouseButtonDblClick)
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
			if(mouseEvent->button() == Qt::LeftButton)
				pApp->internalMouseDoublePressed( MOUSE_LEFT, surface );
			else if(mouseEvent->button() == Qt::RightButton)
				pApp->internalMouseDoublePressed( MOUSE_RIGHT, surface );
			else if(mouseEvent->button() == Qt::MidButton)
				pApp->internalMouseDoublePressed( MOUSE_MIDDLE, surface );
		}
		else if (event->type() == QEvent::Wheel)
		{
			QWheelEvent *mouseEvent = static_cast<QWheelEvent*>(event);
			short ammount = mouseEvent->delta();
			pApp->internalMouseWheeled(ammount, surface );
		}
	}

	if (event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

		if(event->type() == QEvent::MouseMove)
			UI_MouseMove( mouseEvent->x(), mouseEvent->y() );
		//else
			IN_MouseEvent( mouseEvent->buttons() );
	}
	else if(event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		int k1 = keyEvent->nativeScanCode();
		int k2 = Host_MapKey( k1 << 16 );
		//int k3 = keyEvent->nativeScanCode();

		Key_Event( k2, event->type() == QEvent::KeyPress );

		if(event->type() == QEvent::KeyPress && !keyEvent->text().isEmpty())
			CL_CharEvent( keyEvent->text().toLatin1().constData()[0] );
	}
//	else if(event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease)
//	{
//		QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

//	}
	else if (event->type() == QEvent::Wheel)
	{
		QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
		if(wheelEvent->delta() > 0 )
		{
			Key_Event( K_MWHEELUP, true );
			Key_Event( K_MWHEELUP, false );
		}
		else
		{
			Key_Event( K_MWHEELDOWN, true );
			Key_Event( K_MWHEELDOWN, false );
		}
	}
	else if(host.state != HOST_SHUTDOWN && (event->type() == QEvent::WindowActivate || event->type() == QEvent::WindowDeactivate) )
	{
		bool justActivated = event->type() == QEvent::WindowActivate;
		qboolean boolActivate = 0;

		if( host.state != HOST_RESTART )
		{
			if( isMinimized() )
				host.state = HOST_SLEEP;
			else if( !justActivated )
				host.state = HOST_NOFOCUS;
			else host.state = HOST_FRAME;
			boolActivate = (host.state == HOST_FRAME) ? true : false;
		}
		else boolActivate = 1; // video sucessfully restarted

//		//wnd_caption = GetSystemMetrics( SM_CYCAPTION ) + WND_BORDER;

		S_Activate( boolActivate, host.hWnd );
		IN_ActivateCursor();
		IN_ActivateMouse( boolActivate );
		Key_ClearStates();

		if( host.state == HOST_FRAME )
			raise();
		else if( Cvar_VariableInteger( "fullscreen" ) && host.state != HOST_RESTART )
			showMinimized();
	}
	else
		return QOpenGLWidget::eventFilter(target, event);

	event->accept();
	return true;
}

/*
long IN_WndProc( void *hWnd, uint uMsg, uint wParam, long lParam )
{
	int	i, temp = 0;
	qboolean	fActivate;

	if( uMsg == in_mouse_wheel )
	{
		//Sys_Print("mouse wheel!\n");
		uMsg = WM_MOUSEWHEEL_V;
	}
	if( uMsg == WM_MOUSEWHEEL )
	{
		//Sys_Print("new mouse wheel!\n");
		uMsg = WM_MOUSEWHEEL_V;
	}
	VGUI_SurfaceWndProc( hWnd, uMsg, wParam, lParam );

	switch( uMsg )
	{
	case WM_KILLFOCUS:
		if( Cvar_VariableInteger( "fullscreen" ))
			ShowWindow( host.hWnd, SW_SHOWMINNOACTIVE );
		break;
	case WM_SETCURSOR:
		IN_ActivateCursor();
		break;
	case WM_MOUSEWHEEL_V:
		if( !in_mouseactive ) break;
		if(( short )HIWORD( wParam ) > 0 )
		{
			Key_Event( K_MWHEELUP, true );
			Key_Event( K_MWHEELUP, false );
		}
		else
		{
			Key_Event( K_MWHEELDOWN, true );
			Key_Event( K_MWHEELDOWN, false );
		}
		break;
	case WM_CREATE:
		host.hWnd = hWnd;
		GetWindowRect( host.hWnd, &real_rect );
		RegisterHotKey( host.hWnd, PRINTSCREEN_ID, 0, VK_SNAPSHOT );
		break;
	case WM_CLOSE:
		Sys_Quit();
		break;
	case WM_ACTIVATE:
		if( host.state == HOST_SHUTDOWN )
			break; // no need to activate
		if( host.state != HOST_RESTART )
		{
			if( HIWORD( wParam ))
				host.state = HOST_SLEEP;
			else if( LOWORD( wParam ) == WA_INACTIVE )
				host.state = HOST_NOFOCUS;
			else host.state = HOST_FRAME;
			fActivate = (host.state == HOST_FRAME) ? true : false;
		}
		else fActivate = true; // video sucessfully restarted

		wnd_caption = GetSystemMetrics( SM_CYCAPTION ) + WND_BORDER;

		S_Activate( fActivate, host.hWnd );
		IN_ActivateMouse( fActivate );
		Key_ClearStates();

		if( host.state == HOST_FRAME )
		{
			SetForegroundWindow( hWnd );
			ShowWindow( hWnd, SW_RESTORE );
		}
		else if( Cvar_VariableInteger( "fullscreen" ) && host.state != HOST_RESTART )
		{
			ShowWindow( hWnd, SW_MINIMIZE );
		}
		break;
	case WM_MOVE:
		if( !Cvar_VariableInteger( "fullscreen" ))
		{
			RECT	rect;
			int	xPos, yPos, style;

			xPos = (short)LOWORD( lParam );    // horizontal position
			yPos = (short)HIWORD( lParam );    // vertical position

			rect.left = rect.top = 0;
			rect.right = rect.bottom = 1;
			style = GetWindowLong( hWnd, GWL_STYLE );
			AdjustWindowRect( &rect, style, FALSE );

			Cvar_SetFloat( "r_xpos", (float)(xPos + rect.left) );
			Cvar_SetFloat( "r_ypos", (float)(yPos + rect.top) );
			GetWindowRect( host.hWnd, &real_rect );
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEMOVE:
		for( i = 0; i < in_mouse_buttons; i++ )
		{
			if( wParam & mouse_buttons[i] )
				temp |= (1<<i);
		}
		IN_MouseEvent( temp );
		break;
	case WM_SYSCOMMAND:
		// never turn screensaver while Xash is active
		if( wParam == SC_SCREENSAVE && host.state != HOST_SLEEP )
			return 0;
		break;
	case WM_SYSKEYDOWN:
		if( wParam == VK_RETURN )
		{
			// alt+enter fullscreen switch
			Cvar_SetFloat( "fullscreen", (float)(!Cvar_VariableValue( "fullscreen" )));
			return 0;
		}
		// intentional fallthrough
	case WM_KEYDOWN:
		Key_Event( Host_MapKey( lParam ), true );
		if( Host_MapKey( lParam ) == K_ALT )
			return 0;	// prevent WC_SYSMENU call
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		Key_Event( Host_MapKey( lParam ), false );
		break;
	case WM_CHAR:
		CL_CharEvent( wParam );
		break;
	case WM_HOTKEY:
		switch( LOWORD( wParam ))
		{
		case PRINTSCREEN_ID:
			// anti FiEctro system: prevent to write snapshot without Xash version
			Q_strncpy( cls.shotname, "clipboard.bmp", sizeof( cls.shotname ));
			cls.scrshot_action = scrshot_snapshot; // build new frame for screenshot
			host.write_to_clipboard = true;
			cls.envshot_vieworg = NULL;
			break;
		}
		break;
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}
*/
