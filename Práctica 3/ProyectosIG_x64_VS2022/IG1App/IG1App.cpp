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

void IG1App::mouse(int button, int state, int x, int y)
{
	mMouseButt = button;									// settea el boton
	mMouseCoord.x = glutGet(GLUT_WINDOW_WIDTH) - x;			// settea las coordenadas
	mMouseCoord.y = glutGet(GLUT_WINDOW_HEIGHT) - y;
	mMouseState = state;									// settea el estado (pulsado no pulsado)

}

void IG1App::motion(int x, int y)
{
	glm::dvec2 mp = { mMouseCoord.x - x, mMouseCoord.y - y };
	mMouseCoord = { x, y };
	

	// izq = 0 (en mi raton(ines))
	if (mMouseButt == 0) {
		//
		mCamera->orbit(mp.x, mp.y);
	}
	// der =  2 (en mi raton(ines))
	else if (mMouseButt == 2) {
		//
		mCamera->moveLR(mp.x);
		mCamera->moveUD(-mp.y);
	}

	glutPostRedisplay();
}

void IG1App::mouseWheel(int n, int d, int x, int y)
{
	int mod = glutGetModifiers();

	// nada
	if (mod == 0) {
		mCamera->moveFB(d);
	}
	// CTRL
	if (mod == 2 && GLUT_ACTIVE_CTRL) {
		//
		mCamera->setScale(d);
	}
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
	mCamera = new Camera(mViewPort);

	mScene = new Scene();

	changeScene(5);

	mScene->init();
	/*for (auto i : scenes)
		i->init();*/

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
}

void
IG1App::display() const
{
	// double buffering

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears the back buffer

	if (m2Vistas) {
		render2Vistas();
	}
	else
	{
		mViewPort->setSize(mWinW, mWinH);
		mViewPort->setPos(0, 0);
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

	// Resize Scene Visible Area such that the scale is not modified
	mCamera->setSize(mViewPort->width(), mViewPort->height());
}

void
IG1App::key(unsigned char key, int x, int y)
{
	bool need_redisplay = true;
	switch (key)
	{
	case 27: // Escape key
		glutLeaveMainLoop(); // stops main loop and destroy the OpenGL context
		break;
	case '+':
		mCamera->setScale(+0.01); // zoom in  (increases the scale)
		break;
	case '-':
		mCamera->setScale(-0.01); // zoom out (decreases the scale)
		break;
	case 'l':
		mCamera->set3D();
		break;
	case 'o':
		mCamera->set2D();
		break;
	case 'p':
		mCamera->changePrj();
		break;

	case '0':
		mCamera->set2D();
		changeScene(0);
		break;
	case '1':
		changeScene(1);
		break;
	case '2':
		changeScene(2);
		break;
	case '3':
		changeScene(3);
		break;
	case '4':
		changeScene(4);
		break;
	case '5':
		changeScene(5);
		break;
	case '6':
		changeScene(6);
		break;

	case 'u':
		mScene->update();
		break;
	case 'U':
		toggleUpdate();
		glutIdleFunc(s_update);
		break;
	case 'F':
		screenshot();
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

	switch (key)
	{
	case GLUT_KEY_RIGHT:
		if (mdf == GLUT_ACTIVE_CTRL)
			mCamera->pitchReal(-1);
			//mCamera->moveLR(1);
			//mCamera->pitch(-1); // rotates -1 on the X axis
		else
			mCamera->pitchReal(1);
			//mCamera->moveLR(-1);
			//mCamera->pitch(1); // rotates 1 on the X axis
		break;
	case GLUT_KEY_LEFT:
		if (mdf == GLUT_ACTIVE_CTRL)
			mCamera->yawReal(-1);
			//mCamera->moveUD(1);
			//mCamera->yaw(1); // rotates 1 on the Y axis
		else
			mCamera->yawReal(1);
			//mCamera->moveUD(-1);
			//mCamera->yaw(-1); // rotate -1 on the Y axis
		break;
	case GLUT_KEY_UP:
		mCamera->rollReal(-1);
		//mCamera->moveFB(1);
		//mCamera->roll(1); // rotates 1 on the Z axis
		break;
	case GLUT_KEY_DOWN:
		mCamera->rollReal(1);
		//mCamera->moveFB(-1);
		//mCamera->roll(-1); // rotates -1 on the Z axis
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
	// Apartado 16
	if (mUpdate)
	{
		mScene->update();
		glutPostRedisplay();
	}
}

void IG1App::changeScene(int id)
{
	mScene->setScene(id);
}

void IG1App::screenshot()
{
	auto texture = new Texture();
	texture->loadColorBuffer(800, 600);
	texture->saveBMP("./bmps/screenshot.bmp");
}

void IG1App::render2Vistas() const
{
	Camera auxCam = *mCamera;	// Camara auxiliar copiando mCamera

	mViewPort->setSize(mWinW / 2, mWinH);
	auxCam.setSize(mViewPort->width(), mViewPort->height());

	mViewPort->setPos(0, 0);
	mScene->render(auxCam);

	mViewPort->setPos(mWinW / 2, 0);
	auxCam.setCenital();
	mScene->render(auxCam);
}
