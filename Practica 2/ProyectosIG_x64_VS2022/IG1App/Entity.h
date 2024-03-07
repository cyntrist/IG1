#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Texture.h"
#include <vector>

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

	void setTexture(std::string text, Texture* mtext, GLubyte alpha) const { mtext->load(text, alpha); }

protected:
	Mesh* mMesh = nullptr; // the mesh
	glm::dmat4 mModelMat;  // modeling matrix
	glm::dvec4 mColor;	   // color
	Texture* mTexture;	   // texture
	Texture* mTexture2;	   // texture
	//std::vector<Texture*> mTextureVector:


	//void setTexture(std::string text) const { mTexture->load(text, 255);}

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
	GLdouble angle = 5.0;
	GLdouble radio;
public:
	explicit RGBTriangle(GLdouble l); // lado
	explicit RGBTriangle(GLdouble l, GLdouble x); // distancia x desde el origen
	explicit RGBTriangle(GLdouble l, GLuint x, GLuint y); // distancia x e y
	~RGBTriangle();
	void render(glm::dmat4 const& modelViewMat) const override;
	void update() override;
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
	GLdouble angle = 5.0;
	GLdouble totalRotation = 0.0f;
	GLint x = 1;
	GLint y = 0;
	GLint z = 0;
	explicit RGBCube(GLdouble longitud);
	~RGBCube();
	void render(glm::dmat4 const& modelViewMat) const override;
	void update() override;
	glm::mat4 rotateAroundCenter(const glm::mat4& matrix, const glm::vec3& center, float angle, const glm::vec3& axis);
};


class Ground : public Abs_Entity {
public:
	explicit Ground(GLdouble w, GLdouble h);
	explicit Ground(GLdouble w, GLdouble h, std::string t);
	explicit Ground(GLdouble w, GLdouble h, GLdouble rw, GLdouble rh, std::string t);
	~Ground() override;
	void render(glm::dmat4 const& modelViewMat) const override;
	
};

class BoxOutline : public Abs_Entity
{

public:
	explicit BoxOutline(GLdouble length);
	explicit BoxOutline(GLdouble length, std::string t);
	explicit BoxOutline(GLdouble length, std::string t, std::string t2);
	~BoxOutline() override;
	void render(glm::dmat4 const& modelViewMat) const override;
};

class Star3D : public Abs_Entity
{
	GLdouble angle = 0;
	GLdouble rotationFactor = 5;
public:
	explicit Star3D(GLdouble re, GLuint np, GLdouble h);
	void render(glm::dmat4 const& modelViewMat) const override;
	void update() override;
};

class GlassParapet : public Abs_Entity {
public:
	explicit GlassParapet(GLdouble length, std::string t);
	~GlassParapet() override;

	void render(glm::dmat4 const& modelViewMat) const override;

};
#endif //_H_Entities_H_
