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
	gObjects.push_back(new EjesRGB(400.0));
}

void
Scene::free()
{
	// release memory and resources
	for (Abs_Entity* el : gObjects)
	{
		delete el;
		el = nullptr;
	}
	gObjects.resize(0); // ???????? esto no deberia hacerse solo al ser un vector? si no lo hago da error de acceso 
	for (Abs_Entity* el : gTransparentObjects)
	{
		delete el;
		el = nullptr;
	}
	gTransparentObjects.resize(0);
}

void
Scene::setGL()
{
	// OpenGL basic setting
	glClearColor(0.6, 0.7, 0.8, 1.0); // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST); // enable Depth test
	glEnable(GL_TEXTURE_2D); // enable Texture mode
	glEnable(GLUT_MULTISAMPLE);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND); // enable Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // enable Alpha channel
	glEnable(GL_COLOR_MATERIAL);
}

void

Scene::resetGL()
{
	glClearColor(.0, .0, .0, .0); // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST); // disable Depth test
	glDisable(GL_TEXTURE_2D); // disable Texture mode
	glDisable(GLUT_MULTISAMPLE);
	glDisable(GL_COLOR_MATERIAL);
}

void Scene::setCulling()
{
	glEnable(GL_CULL_FACE);
}

void Scene::unsetCulling()
{
	glDisable(GL_CULL_FACE);
}

void Scene::reset()
{
	free();
	resetGL();
	init();
}

void Scene::addObject(Abs_Entity* ent)
{
	gObjects.push_back(ent);
}

void Scene::addTransparentObject(Abs_Entity* ent)
{
	gTransparentObjects.push_back(ent);
}

void Scene::addCompoundObject(CompoundEntity* ent)
{
	gObjects.push_back(ent); // AQUI INES
}

void Scene::setScene(int index)
{
	reset();
	switch (index)
	{
	case 0:
		addObject(new RegularPolygon(32, 250));
		addObject(new RGBRectangle(500, 250));
		addObject(new RGBTriangle(50, 250));
		break;
	case 1:
		addObject(new IndexedBox(200));
		break;
	case 2:
		{
			auto head = new Sphere(100);
			auto hat = new Disk(50, 150);
			auto smile = new PartialDisk(50, 100, 90, 180);

			auto br = 30;
			auto tr = 1;
			auto h = 50;
			auto eye1 = new Cylinder(br, tr, h);
			auto eye2 = new Cylinder(br, tr, h);

			eye1->setRGB(0, 0, 1);
			eye2->setRGB(0, 0.5, 1);


			addObject(head);
			addObject(hat);
			addObject(smile);
			addObject(eye1);
			addObject(eye2);
		}
		break;
	case 3:
		addObject(new Star3D(200, 8, 300, "./bmps/baldosaP.bmp"));
		break;
	case 4:
		addTransparentObject(new GlassParapet(200, 200, "./bmps/windowV.bmp"));
		break;
	case 5:
		addObject(new Photo(200, 100, dvec3(0.0, 10.0, 0.0)));
		addObject(new Ground(600, 600, 4, 4, "./bmps/baldosaC.bmp", dvec3(0.0, 0.0, 0.0)));
		addObject(
			new Box(150, "./bmps/container.bmp", "./bmps/papelC.bmp", dvec3(-224.5, 75.0, -224.5)));
		addTransparentObject(new GlassParapet(600, 300, "./bmps/windowV.bmp", dvec3(0.0, 0.0, 0.0)));
		addObject(new Star3D(75, 8, 100, "./bmps/baldosaP.bmp", dvec3(-225, 200, -225)));
		addTransparentObject(new Grass(200, 200, "./bmps/grass.bmp", dvec3(200, 0, 200)));

		break;
	case 6:
		addObject(new Box(200, "./bmps/container.bmp", "./bmps/papelC.bmp"));
		break;
	case 7:
		addCompoundObject(new AdvancedTIE());

		break;
	default:
		break;
	}
}

void Scene::sceneDirLight(const Camera& cam) const
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	fvec4 posDir = {1, 1, 1, 0};
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(cam.viewMat()));
	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(posDir));
	fvec4 ambient = {0, 0, 0, 1};
	fvec4 diffuse = {1, 1, 1, 1};
	fvec4 specular = {0.5, 0.5, 0.5, 1};
	glLightfv(GL_LIGHT0, GL_AMBIENT, value_ptr(ambient));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, value_ptr(diffuse));
	glLightfv(GL_LIGHT0, GL_SPECULAR, value_ptr(specular));
}

void
Scene::render(const Camera& cam) const
{
	sceneDirLight(cam);
	cam.upload();

	for (Abs_Entity* el : gObjects)
		if (el != nullptr)
			el->render(cam.viewMat());

	for (Abs_Entity* el : gTransparentObjects)
		if (el != nullptr)
			el->render(cam.viewMat());
}

void Scene::update()
{
	glutPostRedisplay();
	for (auto* i : gObjects)
		i->update();
	for (auto* i : gTransparentObjects)
		i->update();
}
