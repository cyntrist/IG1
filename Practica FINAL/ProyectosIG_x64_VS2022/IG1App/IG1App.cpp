#include "IG1App.h"
#include "CheckML.h"
#include <iostream>

using namespace std;

// static single instance (singleton pattern)

IG1App IG1App::s_ig1app; // default constructor (constructor with no parameters)

void
IG1App::close()
{
	if (!mStop)
	{
		// if main loop has not stopped
		cout << "Closing glut...\n";
		glutLeaveMainLoop(); // stops main loop and destroy the OpenGL context
		mStop = true; // main loop stopped
	}
	free();
}

void
IG1App::run() // enters the main event processing loop
{
	if (mWinId == 0)
	{
		// if not initialized
		init(); // initialize the application
		glutMainLoop(); // enters the main event processing loop
		mStop = true; // main loop has stopped
	}
}

void
IG1App::init()
{
	// create an OpenGL Context
	iniWinOpenGL();

	// create the scene after creating the context
	// allocate memory and resources
	mViewPort =
		new Viewport(mWinW, mWinH); // glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)
	mViewPort2 =
		new Viewport(mWinW / 2, mWinH);

	mCamera = new Camera(mViewPort);
	mCamera2 = new Camera(mViewPort2);

	mScene = new Scene;
	mScene2 = new Scene;

	mCamera->set3D();
	mCamera2->set2D();

	mScene->init();
	mScene2->init();

	mScene->setScene(3);
	//mScene2->setScene(0);
	mCamera2->setEjer47(true, 500.0);

	m2Vistas = false;
	//toggle2Vistas();

	// registra los callbacks
	glutMouseFunc(s_mouse);
	glutMotionFunc(s_motion);
	glutMouseWheelFunc(s_mouseWheel);
}

void
IG1App::iniWinOpenGL()
{
	// Initialization
	cout << "Starting glut...\n";
	int argc = 0;
	glutInit(&argc, nullptr);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); // GLUT_CORE_PROFILE
	glutInitContextFlags(GLUT_DEBUG); // GLUT_FORWARD_COMPATIBLE

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(mWinW, mWinH); // window size
	// glutInitWindowPosition (140, 140);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE |
		GLUT_DEPTH /*| GLUT_STENCIL*/); // RGBA colors, double buffer, depth
	// buffer and stencil buffer

	mWinId = glutCreateWindow(
		"IG1"); // with its associated OpenGL context, return window's identifier

	// Callback registration
	glutReshapeFunc(s_resize);
	glutKeyboardFunc(s_key);
	glutSpecialFunc(s_specialKey);
	glutDisplayFunc(s_display);

	cout << glGetString(GL_VERSION) << '\n';
	cout << glGetString(GL_VENDOR) << '\n';
}

void
IG1App::free()
{
	// release memory and resources
	/*for (auto i : scenes)
		delete i;*/
	delete mScene;
	mScene = nullptr;
	delete mCamera;
	mCamera = nullptr;
	delete mViewPort;
	mViewPort = nullptr;

	delete mScene2;
	mScene2 = nullptr;
	delete mCamera2;
	mCamera2 = nullptr;
	delete mViewPort2;
	mViewPort2 = nullptr;
}

void
IG1App::display() const
{
	// double buffering

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears the back buffer

	if (m2Vistas)
	{
		render2Vistas();
	}
	else
	{
		mViewPort->setSize(mWinW, mWinH);
		mViewPort->setPos(0, 0);
		mCamera->setSize(mViewPort->width(), mViewPort->height());
		mScene->render(*mCamera); // uploads the viewport and camera to the GPU
	}

	glutSwapBuffers(); // swaps the front and back buffer
}

void
IG1App::resize(int newWidth, int newHeight)
{
	mWinW = newWidth;
	mWinH = newHeight;

	// Resize Viewport to the new window size
	mViewPort->setSize(newWidth, newHeight);
	mViewPort2->setSize(newWidth / 2, newHeight);

	// Resize Scene Visible Area such that the scale is not modified
	mCamera->setSize(mViewPort->width(), mViewPort->height());
	mCamera2->setSize(mViewPort2->width(), mViewPort2->height());
}

void
IG1App::key(unsigned char key, int x, int y)
{
	auto currentCam = mCamera;
	auto currentScene = mScene;
	if (m2Vistas && mMouseCoord.x > mWinW / 2)
	{
		currentCam = mCamera2;
		currentScene = mScene2;
	}

	bool need_redisplay = true;
	switch (key)
	{
	case 27: // Escape key
		glutLeaveMainLoop(); // stops main loop and destroy the OpenGL context
		break;
	case '+':
		currentCam->setScale(+0.01); // zoom in  (increases the scale)
		break;
	case '-':
		currentCam->setScale(-0.01); // zoom out (decreases the scale)
		break;
	case 'l':
		currentCam->set3D();
		break;
	case 'o':
		currentCam->set2D();
		break;
	case 'p':
		currentCam->changePrj();
		break;

	case '0':
		currentCam->set2D();
		currentScene->setScene(0);
		break;
	case '1':
		currentScene->setScene(1);
		break;
	case '2':
		currentScene->setScene(2);
		break;
	case '3':
		currentScene->setScene(3);
		break;
	case '4':
		currentScene->setScene(4);
		break;
	case '5':
		currentScene->setScene(5);
		break;
	case '6':
		currentScene->setScene(6);
		break;
	case '7':
		currentScene->setScene(7);
		break;

	case 'u':
		currentScene->update();
		currentCam->update();
		break;
	case 'f':
		currentScene->rotateEntity();
		currentCam->update();
		break;
	case 'g':
		currentScene->orbitEntity();
		currentCam->update();
		break;
	case 'U':
		toggleUpdate();
		glutIdleFunc(s_update);
		break;
	case 'F':
		screenshot();
		break;
	case 'q':
		// ACTIVAR LUZ AQUI INES EJ 76
		currentScene->activateDirLight(true);
		break;
	case 'w':
		// DESACTIVAR LUZ AQUI INES EJ 76
		currentScene->activateDirLight(false);
		break;
	case 'a':
		// ACTIVAR LUZ AQUI INES EJ 77
		currentScene->activatePosLight(true);
		break;
	case 's':
		// ACTIVAR LUZ AQUI INES EJ 77
		currentScene->activatePosLight(false);
		break;
	case 'z':
		// ACTIVAR LUZ AQUI INES EJ 78
		currentScene->activateSpotLight(true);
		break;
	case 'x':
		// ACTIVAR LUZ AQUI INES EJ 78
		currentScene->activateSpotLight(false);
		break;
	case 'v':
		// ACTIVAR LUZ AQUI INES EJ 79
		// currentScene->
		break;
	case 'b':
		// ACTIVAR LUZ AQUI INES EJ 79
		// currentScene->
		break;
	case 'k':
		toggle2Vistas();
		break;
	default:
		need_redisplay = false;
		break;
	} // switch

	if (need_redisplay)
		glutPostRedisplay(); // marks the window as needing to be redisplayed -> calls to
	// display()
}

void
IG1App::specialKey(int key, int x, int y)
{
	bool need_redisplay = true;
	int mdf = glutGetModifiers(); // returns the modifiers (Shift, Ctrl, Alt)

	auto currentCam = mCamera;
	if (m2Vistas && mMouseCoord.x > mWinW / 2)
		currentCam = mCamera2;

	switch (key)
	{
	case GLUT_KEY_RIGHT:
		if (mdf == GLUT_ACTIVE_CTRL)
			currentCam->pitchReal(-1);
		else
			currentCam->pitchReal(1);
		break;
	case GLUT_KEY_LEFT:
		if (mdf == GLUT_ACTIVE_CTRL)
			currentCam->yawReal(-1);
		else
			currentCam->yawReal(1);
		break;
	case GLUT_KEY_UP:
		currentCam->rollReal(-1);
		break;
	case GLUT_KEY_DOWN:
		currentCam->rollReal(1);
		break;
	default:
		need_redisplay = false;
		break;
	} // switch

	if (need_redisplay)
		glutPostRedisplay(); // marks the window as needing to be redisplayed -> calls to
	// display()
}

void IG1App::toggle2Vistas()
{
	m2Vistas = !m2Vistas;
}

void IG1App::toggleUpdate()
{
	mUpdate = !mUpdate;
}

void IG1App::update()
{
	if (mUpdate)
	{
		auto currentScene = mScene;
		auto currentCam = mCamera;
		if (m2Vistas && mMouseCoord.x > mWinW / 2)
		{
			currentScene = mScene2;
			currentCam = mCamera2;
		}

		currentScene->update();
		currentCam->update();
		glutPostRedisplay();
	}
}

void IG1App::screenshot()
{
	auto texture = new Texture();
	texture->loadColorBuffer(800, 600);
	texture->saveBMP("./bmps/screenshot.bmp");
}

void IG1App::mouse(int button, int state, int x, int y)
{
	mMouseButt = button; // settea el boton
	mMouseCoord.x = x; // settea las coordenadas
	mMouseCoord.y = glutGet(GLUT_WINDOW_HEIGHT) - y;
	mMouseState = state; // settea el estado (pulsado no pulsado)
}

void IG1App::motion(int x, int y)
{
	glm::dvec2 mp = {mMouseCoord.x - x, mMouseCoord.y - y};
	mMouseCoord = {x, y};

	auto currentCam = mCamera;
	if (m2Vistas && mMouseCoord.x > mWinW / 2)
		currentCam = mCamera2;

	// izq = 0 (en mi raton(ines))
	if (mMouseButt == 0)
	{
		//
		currentCam->orbit(mp.x, mp.y);
	}
	// der =  2 (en mi raton(ines))
	else if (mMouseButt == 2)
	{
		//
		currentCam->moveLR(mp.x);
		currentCam->moveUD(-mp.y);
	}

	glutPostRedisplay();
}

void IG1App::mouseWheel(int n, int d, int x, int y)
{
	int mod = glutGetModifiers();

	auto currentCam = mCamera;
	if (mMouseCoord.x > mWinW / 2)
		currentCam = mCamera2;

	// nada
	if (mod == 0)
		currentCam->moveFB(d);
	// CTRL
	if (mod == 2 && GLUT_ACTIVE_CTRL)
		currentCam->setScale(d);
	glutPostRedisplay();
}

void IG1App::s_mouse(int button, int state, int x, int y)
{
	s_ig1app.mouse(button, state, x, y);
}

void IG1App::s_motion(int x, int y)
{
	s_ig1app.motion(x, y);
}

void IG1App::s_mouseWheel(int n, int d, int x, int y)
{
	s_ig1app.mouseWheel(n, d, x, y);
}

void IG1App::render2Vistas() const
{
	constexpr int nViews = 2;

	mViewPort->setSize(mWinW / 2, mWinH);
	mCamera->setSize(mViewPort->width() * nViews, mViewPort->height() * nViews);
	mViewPort->setPos(0, 0);
	mScene->render(*mCamera);

	mViewPort2->setSize(mWinW / 2, mWinH);
	mCamera2->setSize(mViewPort->width() * nViews, mViewPort->height() * nViews);
	mViewPort2->setPos(mWinW / 2, 0);
	//mCamera2->setCenital();
	mScene2->render(*mCamera2);
}
