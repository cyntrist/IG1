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
	mCamera = new Camera(mViewPort);

	mScene = new Scene();

	//sceneIndex = 5;
	mCamera->set3D();

	mScene->init();
	/*for (auto i : scenes)
		i->init();*/
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

	mScene->render(*mCamera); // uploads the viewport and camera to the GPU

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

	case '0':
		mCamera->set2D();
		changeScene();

		mScene->addObject(new RegularPolygon(32, 250));
		mScene->addObject(new RGBRectangle(500, 250));
		mScene->addObject(new RGBTriangle(50, 250));
		break;
	case '1':
		mCamera->set3D();
		changeScene();

		mScene->addObject(new BoxOutline(200, "./bmps/container.bmp", "./bmps/papelC.bmp"));

		break;
	case '2':
		mCamera->set3D();
		changeScene();

		mScene->addObject(new Ground(300, 300, 4, 4, "./bmps/baldosaC.bmp")); // new Ground(20, 20, 0)

		break;
	case '3':
		mCamera->set3D();
		changeScene();

		mScene->addObject(new Star3D(200, 8, 300, "./bmps/baldosaP.bmp"));

		break;
	case '4':
		mCamera->set3D();
		changeScene();

		mScene->addObject(new GlassParapet(200, 200, "./bmps/windowV.bmp"));

		break;
	case '5':
		mCamera->set3D();
		changeScene();

		mScene->addObject(new Photo(200, 100, glm::dvec3(0.0, 10.0, 0.0)));
		mScene->addObject(new Ground(600, 600, 4, 4, "./bmps/baldosaC.bmp", glm::dvec3(0.0, 0.0, 0.0)));
		mScene->addObject(
			new Box(150, "./bmps/container.bmp", "./bmps/papelC.bmp", glm::dvec3(-224.5, 75.0, -224.5)));
		mScene->addObject(new GlassParapet(600, 300, "./bmps/windowV.bmp", glm::dvec3(0.0, 0.0, 0.0)));
		mScene->addObject(new Star3D(75, 8, 100, "./bmps/baldosaP.bmp", glm::dvec3(-225, 200, -225)));
		mScene->addObject(new Grass(200, 200, "./bmps/grass.bmp", glm::dvec3(200, 0, 200)));

		break;
	case '6':
		mCamera->set3D();
		changeScene();

		mScene->addObject(new Box(200, "./bmps/container.bmp", "./bmps/papelC.bmp"));

		break;

	case 'u':
		mScene->update();
		break;
	case 'U':
		mUpdate = !mUpdate;
		glutIdleFunc(s_update);
		break;
	case 'F':
		screenshot();
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
			mCamera->pitch(-1); // rotates -1 on the X axis
		else
			mCamera->pitch(1); // rotates 1 on the X axis
		break;
	case GLUT_KEY_LEFT:
		if (mdf == GLUT_ACTIVE_CTRL)
			mCamera->yaw(1); // rotates 1 on the Y axis
		else
			mCamera->yaw(-1); // rotate -1 on the Y axis
		break;
	case GLUT_KEY_UP:
		mCamera->roll(1); // rotates 1 on the Z axis
		break;
	case GLUT_KEY_DOWN:
		mCamera->roll(-1); // rotates -1 on the Z axis
		break;
	default:
		need_redisplay = false;
		break;
	} // switch

	if (need_redisplay)
		glutPostRedisplay(); // marks the window as needing to be redisplayed -> calls to
	// display()
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

void IG1App::changeScene()
{
	mScene->reset();
}

void IG1App::screenshot()
{
	auto texture = new Texture();
	texture->loadColorBuffer(800, 600);
	texture->saveBMP("./bmps/screenshot.bmp");
}
