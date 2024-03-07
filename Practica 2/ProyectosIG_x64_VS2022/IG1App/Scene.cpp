#include "Scene.h"

#include <iostream>

#include "CheckML.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

void
Scene::init()
{
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene
	gObjects.push_back(new EjesRGB(400.0));

	// Apartado 3
	//gObjects.push_back(new RegularPolygon(7, 200));

	// Apartado 4 y 5
	//gObjects.push_back(new RegularPolygon(3, 200, glm::dvec4(0, 1, 1, 1)));
	//gObjects.push_back(new RegularPolygon(64, 200, glm::dvec4(1, 0, 1, 1)));

	// Apartado 6 y 7	
	//gObjects.push_back(new RGBTriangle(3, 200));

	// Apartado 8
	//gObjects.push_back(new RGBRectangle(400,200));

	// Apartado 9
	//gObjects.push_back(new Cube(100));

	// Apartado 10
	//gObjects.push_back(new RGBCube(100));
}

void
Scene::free()
{ // release memory and resources
	for (Abs_Entity* el : gObjects) {
		delete el;
		el = nullptr;
	}
}
void
Scene::setGL()
{
	// OpenGL basic setting
	glClearColor(0.6, 0.7, 0.8, 1.0); // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);          // enable Depth test
	glEnable(GL_TEXTURE_2D);		  // enable Texture mode
	glEnable(GLUT_MULTISAMPLE);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);									// enable Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// enable Alpha channel
}
void

Scene::resetGL()
{
	glClearColor(.0, .0, .0, .0); // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);     // disable Depth test
	glDisable(GL_TEXTURE_2D);	  // disable Texture mode
	glDisable(GLUT_MULTISAMPLE);
}

void Scene::setCulling()
{
	glEnable(GL_CULL_FACE);
}

void Scene::unsetCulling()
{
	glDisable(GL_CULL_FACE);
}

void Scene::addObject(Abs_Entity* ent)
{
	gObjects.push_back(ent);
}

void
Scene::render(Camera const& cam) const
{
	cam.upload();

	for (Abs_Entity* el : gObjects) {
		el->render(cam.viewMat());
	}
}

void Scene::update()
{
	glutPostRedisplay();
	for (auto* i : gObjects)
		i->update();
}
