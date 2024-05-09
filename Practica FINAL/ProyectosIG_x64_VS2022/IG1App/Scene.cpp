#include "Scene.h"
#include <iostream>
#include "CheckML.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Material.h"
#include "Light.h"

using namespace glm;

void
Scene::init()
{
	setGL(); // OpenGL settings
	gObjects.push_back(new EjesRGB(400.0));
	setLights();
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

	for (Light* l : lights)
	{
		delete l;
		l = nullptr;
	}
	lights.resize(0);
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

	sceneID = index;
	switch (index)
	{
	case 0:
		{
			auto p = new RegularPolygon(32, 250);
			auto r = new RGBRectangle(500, 250);
			auto t = new RGBTriangle(50, 0);


			inventedNodeRotate = new CompoundEntity();
			fatherInventedNode = new CompoundEntity();
			inventedNode = new CompoundEntity();

			t->setModelMat(
				translate(dmat4(1.0), dvec3(0, 0, 0))
			);

			fatherInventedNode->addEntity(inventedNode);
			fatherInventedNode->addEntity(p);
			fatherInventedNode->addEntity(r);

			inventedNode->addEntity(inventedNodeRotate);

			// se settea el radio de la esfera para el tie
			inventedNodeRotate->setModelMat(translate(inventedNode->modelMat(), dvec3(100, 0, 0)));

			// añade el tie al nodo pequeño
			inventedNodeRotate->addEntity(t);

			addObject(fatherInventedNode);
		}

		break;
	case 1:
		addObject(new IndexedBox(200));
		break;
	case 2:
		{
			auto head = new Sphere(100);
			auto hat = new Disk(50, 100);
			auto smile = new PartialDisk(50, 80, 90, 180);

			auto br = 15;
			auto tr = 0;
			auto h = 30;
			auto eye1 = new Cylinder(br, tr, h);
			auto eye2 = new Cylinder(br, tr, h);

			head->setRGB(1, 0.5, 0);
			hat->setRGB(1, 0, 0);
			smile->setRGB(0, 1, 0);
			eye1->setRGB(0, 0, 1);
			eye2->setRGB(0, 0.5, 1);

			auto rPos = 80;
			auto lPos = 34;
			auto dist = 33;

			eye1->setModelMat(
				translate(dmat4(1.0), dvec3(lPos, dist, rPos))
				* rotate(dmat4(1.0), radians(45.0), dvec3(0.0, 1.0, 0.0))
				* eye1->modelMat()
			);

			eye2->setModelMat(
				translate(dmat4(1.0), dvec3(rPos, dist, lPos))
				* rotate(dmat4(1.0), radians(45.0), dvec3(0.0, 1.0, 0.0))
				* eye2->modelMat()
			);

			hat->setModelMat(
				translate(dmat4(1.0), dvec3(-10, 80, -10))
				* rotate(dmat4(1.0), radians(90.0), dvec3(1.0, 0.0, 0.0))
				* rotate(dmat4(1.0), radians(10.0), dvec3(0.0, 1.0, 0.0))
				* rotate(dmat4(1.0), radians(-10.0), dvec3(1.0, 0.0, 1.0))
				* hat->modelMat()
			);

			smile->setModelMat(
				translate(dmat4(1.0), dvec3(55, 10, 55))
				* rotate(dmat4(1.0), radians(0.0), dvec3(1.0, 0.0, 0.0))
				* rotate(dmat4(1.0), radians(45.0), dvec3(0.0, 1.0, 0.0))
				* smile->modelMat()
			);

			addObject(head);
			addObject(hat);
			addObject(smile);
			addObject(eye1);
			addObject(eye2);
		}
		break;
	case 3:
		{
			int p = 50;
			int r = 100;
			auto offset = 200.0;


			auto goldTatooie = new RevSphere(r, p, p, true);
			goldTatooie->setModelMat(
				translate(dmat4(1.0), dvec3(0, 0, offset))
			);
			goldTatooie->setmColor(dvec4(1, 0.9, 0, 1));
			const auto mat = new Material;
			mat->setGolden();
			goldTatooie->setMaterial(mat);
			addObject(goldTatooie);

			auto yellowTatooie = new RevSphere(r, p, p);
			yellowTatooie->setmColor(dvec4(1, 0.9, 0, 1));
			yellowTatooie->setModelMat(
				translate(dmat4(1.0), dvec3(offset, 0, 0))
			);
			addObject(yellowTatooie);


			break;
		}
	case 4:
		addObject(new Toroid(100, 100, 30, 20));
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
		{
			//glClearColor(0.0, 0.0, 0.0, 0.0);

			tieF = new AdvancedTIE();

			constexpr GLdouble tatDiam = 200;
			tatooie = new Sphere(tatDiam);
			tatooie->setRGB(1, 0.9, 0.0);

			fatherInventedNode = new CompoundEntity();
			inventedNode = new CompoundEntity();
			inventedNodeRotate = new CompoundEntity();

			fatherInventedNode->addEntity(tatooie);
			fatherInventedNode->addEntity(inventedNode);

			inventedNode->addEntity(inventedNodeRotate); // se mete el el nodo grande el planeta y el nodo pequeño
			inventedNodeRotate->setModelMat( // se settea el radio de la esfera para el tie
				translate(
					inventedNode->modelMat(),
					dvec3(0, tatDiam / 2, 0)
				)
			);
			inventedNodeRotate->addEntity(tieF); // añade el tie al nodo pequeño

			addObject(fatherInventedNode);
		}
		break;
	default:
		break;
	}
}

void Scene::setLights()
{
	// TO DO

	// DIR LIGHT
	dirLight = new DirLight();

	// settea el dirLight
	fvec4 v = { 1, 1, 1, 0 };							// posicion MUNDIAL de la luz
	fvec4 ambient = { 0, 0, 0, 1 };						//
	fvec4 diffuse = { 1, 1, 1, 1 };						//
	fvec4 specular = { 0.5, 0.5, 0.5, 1 };				//

	dirLight->setPosDir(v);
	dirLight->setAmbient(ambient);
	dirLight->setDiffuse(diffuse);
	dirLight->setSpecular(specular);
	dirLight->setID(GL_LIGHT0);							// settea el id del objeto????

	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(v));	// relaciona el id y la posicion

	lights.push_back(dirLight);




	posLight = new PosLight();	
	// IDENTIFICADOR GL_LIGHT1 ????
	// diff -> (1.0,1.0,0.0) 
	// settea el posLight

	// settea el poslight
	v = { 200, 200, 0, 1};							// posicion LOCAL de la luz
	ambient = { 0, 0, 0, 1 };						//
	diffuse = { 1, 1, 0, 0 };						//
	specular = { 0.5, 0.5, 0.5, 1 };				//

	posLight->setPosDir(v);
	posLight->setAmbient(ambient);
	posLight->setDiffuse(diffuse);
	posLight->setSpecular(specular);
	posLight->setID(GL_LIGHT1);							// settea el id del objeto????

	glLightfv(GL_LIGHT1, GL_POSITION, value_ptr(v));	// relaciona el id y la posicion

	lights.push_back(posLight);



	spotLight = new SpotLight();;
	// IDENTIFICADOR GL_LIGHT2 ????
	// settea el spotLight

	// settea el spotLight
	fvec3 v2 = { 0, 1, 1 };
	v = { 0, 200, 200, 1 };	// 
	ambient = { 0, 0, 0, 1 };						//
	diffuse = { 1, 1, 1, 1 };						//
	specular = { 0.5, 0.5, 0.5, 1 };				//

	spotLight->setPosDir(v);
	spotLight->setSpot(v2, 100.0, 100.0);
	spotLight->setAmbient(ambient);
	spotLight->setDiffuse(diffuse);
	spotLight->setSpecular(specular);
	spotLight->setID(GL_LIGHT2);
	spotLight->setAtte(1,0,1);


	lights.push_back(spotLight);



	spotLight2 = new SpotLight();;
	// IDENTIFICADOR GL_LIGHT2 ????
	// settea el spotLight

	// settea el spotLight
	v2 = { 0, 1, 1 };
	v = { 0, 200, 200, 1 };	// 
	ambient = { 0, 0, 0, 1 };						//
	diffuse = { 1, 1, 1, 1 };						//
	specular = { 0.5, 0.5, 0.5, 1 };				//

	spotLight2->setPosDir(v);
	spotLight2->setSpot(v2, 100.0, 100.0);
	spotLight2->setAmbient(ambient);
	spotLight2->setDiffuse(diffuse);
	spotLight2->setSpecular(specular);
	spotLight2->setID(GL_LIGHT2);
	spotLight2->setAtte(1, 0, 1);





	lights.push_back(spotLight2);
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

void Scene::activateDirLight(bool a)
{
	if (a) {
		lights[0]->enable();

	}
	else {
		lights[0]->disable();
	}
}

void Scene::activatePosLight(bool a)
{
	if (a) {
		lights[1]->enable();

	}
	else {
		lights[1]->disable();
	}
}

void Scene::activateSpotLight(bool a)
{
	if (a) {
		lights[2]->enable();

	}
	else {
		lights[2]->disable();
	}
}

void Scene::rotateEntity()
{
	switch (sceneID)
	{
	case 0:
		{
			inventedNodeRotate->setModelMat(
				rotate(
					inventedNodeRotate->modelMat(),
					radians(3.0),
					dvec3(0, 0, 1)
				)
			);
			//inventedNode1->setModelMat(glm::rotate(inventedNode1->modelMat(),
			//	radians(3.0), dvec3(0, 0, 1)));
		}
		break;
	case 7:
		{
			// gira el nodo
			inventedNodeRotate->setModelMat(
				rotate(
					inventedNodeRotate->modelMat(),
					radians(3.0),
					dvec3(0, 1, 0)
				)
			);

			// para hacerlo circular y que no salgan numeros muy grandes
			ang = static_cast<int>(ang + 3.0) % 360;
		}

		break;
	}
	//inventedNodeRotate->setModelMat(translate(inventedNode->modelMat(), dvec3(1100, 0, 0)));
	// coloca la entidad justo encima del nodo
	//tieF->setModelMat(translate(inventedNodeRotate->modelMat(), dvec3(0, 0, 0)));
}

void Scene::orbitEntity()
{
	switch (sceneID)
	{
	case 0:
		{
			// 
			inventedNode->setModelMat(
				rotate(inventedNode->modelMat(), radians(3.0), dvec3(0, 0, 1)));
		}
		break;
	case 7:
		/*inventedNodeRotate->setModelMat(
			rotate(inventedNodeRotate->modelMat(), radians(3.0), dvec3(0, 0, 1)));*/


		// calculo con el sen y cos TO DO
		GLdouble x = sin(radians(ang));
		GLdouble z = cos(radians(ang));


		inventedNode->setModelMat(
			rotate(
				inventedNode->modelMat(),
				radians(1.0),
				dvec3(x, 0, z))
		);

		break;
	}

	//// orbit idem al anterior
	//// coloca la entidad justo encima del nodo
	////tieF->setModelMat(translate(inventedNode->modelMat(), dvec3(1100, 0, 0)));
	////inventedNodeRotate->setModelMat(translate(inventedNode->modelMat(), dvec3(1100, 0, 0)));

	//// calculo con el sen y cos TO DO
	//GLdouble x = sin(radians(ang));
	//GLdouble z = cos(radians(ang));

	//// gira el nodo
	//inventedNode->setModelMat( rotate(inventedNode->modelMat(), radians(3.0), dvec3(x,0,z)));
}


void
Scene::render(const Camera& cam) const
{
	// practica ultima
	//sceneDirLight(cam);
	// se deberia hacer en un array como las entidades pero me da perza

	glEnable(GL_LIGHTING);

	for (Light* l : lights)
		if (l != nullptr)
			l->upload(cam.viewMat());
	
	cam.upload();

	for (Abs_Entity* el : gObjects)
		if (el != nullptr)
			el->render(cam.viewMat());

	for (Abs_Entity* el : gTransparentObjects)
		if (el != nullptr)
			el->render(cam.viewMat());

	glDisable(GL_LIGHTING);
}

void Scene::update()
{
	glutPostRedisplay();
	for (auto* i : gObjects)
		i->update();
	for (auto* i : gTransparentObjects)
		i->update();
}
