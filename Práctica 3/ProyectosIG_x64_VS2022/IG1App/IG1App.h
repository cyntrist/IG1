#ifndef _H_IG1App_H_
#define _H_IG1App_H_

// #include <gl/GL.h>    // OpenGL
// #include <gl/GLU.h>   // OpenGL Utility Library
// #include <GL/glut.h>  // OpenGL Utility Toolkit

#include <array>
#include <GL/freeglut.h> // OpenGL Utility Toolkit
#include <glm/glm.hpp>   // OpenGL Mathematics

#include "Camera.h"
#include "Scene.h"
#include "Viewport.h"

//constexpr int MAX_SCENES = 7;
//
//	static int sceneIndex = 0;
//	static std::array<Scene*, MAX_SCENES> scenes; // array de escenas

class IG1App
{
public:
	// static single instance (singleton pattern)
	static IG1App s_ig1app;

	IG1App(IG1App const& J) = delete;         // no copy constructor
	void operator=(IG1App const& J) = delete; // no copy assignment

	// Viewport position and size
	Viewport const& viewPort() { return *mViewPort; };
	// Camera position, view volume and projection
	Camera const& camera() { return *mCamera; };
	// Graphics objects of the scene
	//Scene const& scene() { return *mScene; };

	// mouse callbacks
	void mouse(int button, int state, int x, int y);
	void motion(int x, int y);
	void mouseWheel(int n, int d, int x, int y);

	static void s_mouse(int button, int state, int x, int y);
	static void s_motion(int x, int y);
	static void s_mouseWheel(int n, int d, int x, int y);

	void run();   // the main event processing loop
	void close(); // the application

protected:
	IG1App(){};
	~IG1App() { close(); };

	void init();
	void iniWinOpenGL();
	void free();
	void update();
	void changeScene(int id);
	void screenshot();
	void render2Vistas() const;

	void display() const;                      // the scene
	void resize(int newWidth, int newHeight);  // the viewport (without changing the scale)
	void key(unsigned char key, int x, int y); // keypress event
	void specialKey(int key, int x, int y);    // keypress event for special characters

	void toggle2Vistas();
	void toggleUpdate();

	// static callbacks
	static void s_display() { s_ig1app.display(); };
	static void s_resize(int newWidth, int newHeight) { s_ig1app.resize(newWidth, newHeight); };
	static void s_key(unsigned char key, int x, int y) { s_ig1app.key(key, x, y); };
	static void s_specialKey(int key, int x, int y) { s_ig1app.specialKey(key, x, y); };
	static void s_update() { s_ig1app.update(); };


	// Viewport position and size
	Viewport* mViewPort = nullptr;
	Viewport* mViewPort2 = nullptr;
	// Camera position, view volume and projection
	Camera* mCamera = nullptr;
	Camera* mCamera2 = nullptr;
	// Graphics objects of the scene
	Scene* mScene = nullptr; // escena renderizada actual
	Scene* mScene2 = nullptr; // escena renderizada actual

	// raton
	glm::dvec2 mMouseCoord;
	int mMouseButt;
	bool mMouseState;		// true -> pulsado 




	bool mStop = false; // main event processing loop
	bool mUpdate = false; // scene updates flag
	int mWinId = 0;     // window's identifier
	int mWinW = 800;    // window's width
	int mWinH = 600;    // window's height
	bool m2Vistas = false;
};

#endif //_H_IG1App_H_
