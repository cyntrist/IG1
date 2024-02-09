#pragma once
#include "Entity.h"

class RegularPolygon : public Abs_Entity {

public:
	RegularPolygon() = default;
	RegularPolygon(GLuint num, GLdouble r);

	virtual void render(glm::dmat4 const& modelViewMat) const;

};