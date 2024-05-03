#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Entity.h"

#include <vector>

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
	void sceneDirLight(const Camera& cam) const;

	// PRACTICA 4 ej 68
	void rotateEntity();
	void orbitEntity();

protected:
	void free();
	void setGL();
	void resetGL();

	int sceneID;

	std::vector<Abs_Entity*> gObjects; // Entities (graphic objects) of the scene
	std::vector<Abs_Entity*> gTransparentObjects; // Entities (graphic objects) of the scene

	CompoundEntity* fatherInventedNode;
	CompoundEntity* inventedNode;
	CompoundEntity* inventedNodeRotate;
	CompoundEntity* tieF;
	QuadricEntity* tatooie;

	CompoundEntity* node66;
	CompoundEntity* node66_2;
	GLdouble ang;
};

#endif //_H_Scene_H_
