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

	Scene(const Scene& s) = delete;            // no copy constructor
	Scene& operator=(const Scene& s) = delete; // no copy assignment

	void init();
	// apartado 11
	void addObject(Abs_Entity* ent);
	void addTransparentObject(Abs_Entity* ent);
	void render(Camera const& cam) const;
	void update();
	void setCulling();
	void unsetCulling();
	void reset();
	void setScene(int);
	void sceneDirLight(Camera const&cam) const;

protected:
	void free();
	void setGL();
	void resetGL();
	

	std::vector<Abs_Entity*> gObjects; // Entities (graphic objects) of the scene
	std::vector<Abs_Entity*> gTransparentObjects; // Entities (graphic objects) of the scene
};

#endif //_H_Scene_H_
