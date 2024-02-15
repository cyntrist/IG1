#include "Scene.h"
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
	gObjects.push_back(new RGBCube(100));
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
}
void
Scene::resetGL()
{
	glClearColor(.0, .0, .0, .0); // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);     // disable Depth test
}

void
Scene::render(Camera const& cam) const
{
	cam.upload();

	for (Abs_Entity* el : gObjects) {
		el->render(cam.viewMat());
	}
}
