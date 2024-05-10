#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Entity.h"

#include <vector>

class Light;
class DirLight;
class SpotLight;
class PosLight;

class Scene
{
public:
	Scene() = default;

	~Scene()
	{
		free();
		resetGL();
	};

	Scene(const Scene& s) = delete; // no copy constructor
	Scene& operator=(const Scene& s) = delete; // no copy assignment

	void init();
	// apartado 11
	void addObject(Abs_Entity* ent);
	void addTransparentObject(Abs_Entity* ent);
	void addCompoundObject(CompoundEntity* ent);
	void render(const Camera& cam) const;
	void update();
	void setCulling();
	void unsetCulling();
	void reset();
	void setScene(int);
	void setLights();
	void sceneDirLight(const Camera& cam) const;

	// true para activar false para apagar
	void activateDirLight(bool a);
	void activatePosLight(bool a);
	void activateSpotLight(bool a);
	void activateTIELight(bool a);


	void moveLight();

	// PRACTICA 4 ej 68
	void rotateEntity();
	void orbitEntity();

	void setCamera(Camera* cam_);

protected:
	void free();
	void setGL();
	void resetGL();

	int sceneID;

	Camera* cam;

	std::vector<Abs_Entity*> gObjects; // Entities (graphic objects) of the scene
	std::vector<Abs_Entity*> gTransparentObjects; // Entities (graphic objects) of the scene
	std::vector<Light*> lights;	// lights

	CompoundEntity* fatherInventedNode;
	CompoundEntity* inventedNode;
	CompoundEntity* inventedNodeRotate;
	CompoundEntity* tieF;
	QuadricEntity* tatooie;

	CompoundEntity* node66;
	CompoundEntity* node66_2;
	GLdouble ang;


	// light
	DirLight* dirLight = nullptr;
	PosLight* posLight = nullptr;	// IDENTIFICADOR GL_LIGHT1 ????
		// diff -> (1.0,1.0,0.0) 
		// 
	SpotLight* spotLight = nullptr; // IDENTIFICADOR GL_LIGHT2 ????
};

#endif //_H_Scene_H_
