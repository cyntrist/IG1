#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include "Mesh.h"

class Abs_Entity // abstract class
{
public:
	Abs_Entity()
	  : mModelMat(1.0){} // 4x4 identity matrix
	virtual ~Abs_Entity() = default;

	Abs_Entity(const Abs_Entity& e) = delete;            // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete; // no copy assignment

	virtual void render(glm::dmat4 const& modelViewMat) const = 0; // abstract method
	virtual void update() {}

	// modeling matrix
	glm::dmat4 const& modelMat() const { return mModelMat; };
	void setModelMat(glm::dmat4 const& aMat) { mModelMat = aMat; };

	// color
	glm::dvec4 getmColor() { return mColor; } // getter
	void setmColor(glm::dvec4 color) { mColor = color; } // setter

protected:
	Mesh* mMesh = nullptr; // the mesh
	glm::dmat4 mModelMat;  // modeling matrix
	glm::dvec4 mColor;	   // color


	// transfers modelViewMat to the GPU
	virtual void upload(glm::dmat4 const& mModelViewMat) const;
};

class EjesRGB : public Abs_Entity
{
public:
	explicit EjesRGB(GLdouble l);
	~EjesRGB();
	void render(glm::dmat4 const& modelViewMat) const override;
};

class RegularPolygon : public Abs_Entity {
public:
	RegularPolygon() = default;
	~RegularPolygon();
	RegularPolygon(GLuint num, GLdouble r);
	RegularPolygon(GLuint num, GLdouble r, glm::dvec4 color);
	void render(glm::dmat4 const& modelViewMat) const override;
};

class RGBTriangle : public Abs_Entity 
{
public:
	explicit RGBTriangle(GLdouble r);
	~RGBTriangle();
	void render(glm::dmat4 const& modelViewMat) const override;
};

class RGBRectangle : public Abs_Entity
{
public:
	explicit RGBRectangle(GLdouble w, GLdouble h);
	~RGBRectangle();
	void render(glm::dmat4 const& modelViewMat) const override;
};

class Cube : public Abs_Entity
{
public:
	explicit Cube(GLdouble longitud);
	~Cube();
	void render(glm::dmat4 const& modelViewMat) const override;
};

class RGBCube : public Abs_Entity
{
public:
	explicit RGBCube(GLdouble longitud);
	~RGBCube();
	void render(glm::dmat4 const& modelViewMat) const override;
};

#endif //_H_Entities_H_