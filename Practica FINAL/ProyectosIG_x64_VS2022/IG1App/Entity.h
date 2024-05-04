#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Texture.h"
#include <vector>

class Material;

class Abs_Entity // abstract class
{
public:
	Abs_Entity()
		: mModelMat(1.0), translation(glm::vec3(0))
	{
	} // 4x4 identity matrix
	virtual ~Abs_Entity() = default;

	Abs_Entity(const Abs_Entity& e) = delete; // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete; // no copy assignment

	virtual void render(const glm::dmat4& modelViewMat) const = 0; // abstract method
	virtual void update()
	{
	}

	// modeling matrix
	const glm::dmat4& modelMat() const { return mModelMat; };
	void setModelMat(const glm::dmat4& aMat) { mModelMat = aMat; };

	// color
	glm::dvec4 getmColor() { return mColor; } // getter
	void setmColor(glm::dvec4 color) { mColor = color; } // setter

	// texture
	void setTexture(const std::string& text, Texture* mtext, GLubyte alpha) const { mtext->load(text, alpha); }

	// rotate
	glm::mat4 rotateAroundCenter(const glm::mat4& matrix, const glm::vec3& center, float angle, const glm::vec3& axis);

protected:
	Mesh* mMesh = nullptr; // the mesh
	glm::dmat4 mModelMat; // modeling matrix
	glm::dvec4 mColor; // color
	Texture* mTexture; // texture
	glm::dvec3 translation; // final translation
	//std::vector<Texture*> mTextureVector:

	// transfers modelViewMat to the GPU
	virtual void upload(const glm::dmat4& mModelViewMat) const;
};

class EjesRGB : public Abs_Entity
{
public:
	explicit EjesRGB(GLdouble l);
	~EjesRGB() override;
	void render(const glm::dmat4& modelViewMat) const override;
};

class RegularPolygon : public Abs_Entity
{
public:
	RegularPolygon() = default;
	~RegularPolygon() override;
	RegularPolygon(GLuint num, GLdouble r, glm::dvec3 trans = glm::dvec3(0));
	RegularPolygon(GLuint num, GLdouble r, glm::dvec4 color, glm::dvec3 trans = glm::dvec3(0));
	void render(const glm::dmat4& modelViewMat) const override;
};

class RGBTriangle : public Abs_Entity
{
	GLdouble angle = 5.0;
	GLdouble radio;
	glm::dvec2 trans;

public:
	explicit RGBTriangle(GLdouble l); // lado
	explicit RGBTriangle(GLdouble l, GLdouble x); // distancia x desde el origen
	explicit RGBTriangle(GLdouble l, GLuint x, GLuint y); // distancia x e y
	~RGBTriangle() override;
	void render(const glm::dmat4& modelViewMat) const override;
	void update() override;

	void rotateObj();
};

class RGBRectangle : public Abs_Entity
{
public:
	explicit RGBRectangle(GLdouble w, GLdouble h);
	~RGBRectangle() override;
	void render(const glm::dmat4& modelViewMat) const override;
};

class Cube : public Abs_Entity
{
public:
	explicit Cube(GLdouble longitud);
	~Cube() override;
	void render(const glm::dmat4& modelViewMat) const override;
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
	~RGBCube() override;
	void render(const glm::dmat4& modelViewMat) const override;
	void update() override;
};

/// PARTE 2
class Ground : public Abs_Entity
{
public:
	explicit Ground(GLdouble w, GLdouble h, glm::dvec3 trans = glm::dvec3(0));
	explicit Ground(GLdouble w, GLdouble h, std::string t, glm::dvec3 trans = glm::dvec3(0));
	explicit Ground(GLdouble w, GLdouble h, GLdouble rw, GLdouble rh, std::string t, glm::dvec3 trans = glm::dvec3(0));
	~Ground() override;
	void render(const glm::dmat4& modelViewMat) const override;
};

class BoxOutline : public Abs_Entity
{
	Texture* mTexture2; // texture
public:
	explicit BoxOutline(GLdouble length, glm::dvec3 trans = glm::dvec3(0));
	explicit BoxOutline(GLdouble length, std::string t, glm::dvec3 trans = glm::dvec3(0));
	explicit BoxOutline(GLdouble length, std::string t, std::string t2, glm::dvec3 trans = glm::dvec3(0));
	~BoxOutline() override;
	void render(const glm::dmat4& modelViewMat) const override;
};

class Box : public Abs_Entity
{
	Texture* mTexture2;
	GLdouble length;
	GLdouble angle;
	GLdouble speed = 1;
	bool clockwise = true;
	Mesh* mTopMesh;
	Mesh* mBottomMesh;
	glm::dmat4 mTopMat = glm::dmat4(1.0);
	glm::dmat4 mBotMat = glm::dmat4(1.0);

public:
	explicit Box(GLdouble length, const std::string& t, const std::string& t2, glm::dvec3 trans = glm::dvec3(0));
	~Box() override;
	void render(const glm::dmat4& modelViewMat) const override;
	void renderTop(const glm::dmat4& modelViewMat) const;
	void renderMain(const glm::dmat4& modelViewMat) const;
	void renderBot(const glm::dmat4& modelViewMat) const;
	void renderBox(const glm::dmat4& modelViewMat) const;
	void update() override;
};

class Star3D : public Abs_Entity
{
	GLdouble angle = 0;
	GLdouble rotationFactor = 1;

public:
	explicit Star3D(GLdouble re, GLuint np, GLdouble h, std::string, glm::dvec3 trans = glm::dvec3(0));
	~Star3D() override;
	void render(const glm::dmat4& modelViewMat) const override;
	void update() override;
};

class GlassParapet : public Abs_Entity
{
public:
	explicit GlassParapet(GLdouble width, GLdouble height, std::string t, glm::dvec3 trans = glm::dvec3(0));
	~GlassParapet() override;

	void render(const glm::dmat4& modelViewMat) const override;
};

class Photo : public Abs_Entity
{
public:
	explicit Photo(GLdouble w, GLdouble h, glm::dvec3 trans = glm::dvec3(0));
	~Photo() override;

	void render(const glm::dmat4& modelViewMat) const override;
	void update() override;

private:
	int pW, pH;
};

class Grass : public Abs_Entity
{
	GLdouble w, h;
	int nBlades;

public:
	explicit Grass(GLdouble w, GLdouble h, std::string t, glm::dvec3 trans = glm::dvec3(0), GLint n = 3);
	~Grass() override;
	void render(const glm::dmat4& modelViewMat) const override;
};


// ------------------------------------------ PRACTICA 4 ----------------------------------------

class QuadricEntity : public Abs_Entity
{
public:
	QuadricEntity() : red(), green(), blue() { q = gluNewQuadric(); };
	~QuadricEntity() override { gluDeleteQuadric(q); };
	void setRGB(GLdouble, GLdouble, GLdouble);

protected:
	GLdouble red, green, blue;
	GLUquadricObj* q;
};

class Sphere : public QuadricEntity
{
public:
	Sphere(GLdouble rr); // r es el radio de la esfera 
	void render(const glm::dmat4& modelViewMat) const override;

protected:
	GLdouble r;
};


class Cylinder : public QuadricEntity
{
public:
	Cylinder(GLdouble brr, GLdouble trr, GLdouble hh); // brr = radio base; tr = radio top; h = altura
	void render(const glm::dmat4& modelViewMat) const override;

protected:
	GLdouble br, tr, h;
};

class Disk : public QuadricEntity
{
public:
	Disk(GLdouble irr, GLdouble orr); // irr radio interior; orr raido exterior (outter)
	void render(const glm::dmat4& modelViewMat) const override;

protected:
	GLdouble iR, oR;
};

class PartialDisk : public QuadricEntity
{
public:
	PartialDisk(GLdouble irr, GLdouble orr, GLdouble sAng, GLdouble swAng); // r es el radio de la esfera 
	void render(const glm::dmat4& modelViewMat) const override;

protected:
	GLdouble iR, oR, sa, swa;
};


class CompoundEntity : public Abs_Entity
{
public:
	CompoundEntity();
	~CompoundEntity() override;
	void addEntity(Abs_Entity* ae);
	void render(const glm::dmat4& modelViewMat) const override;


	//GLdouble getAngle() { return ang; };
	//void setAng(GLdouble a) { ang = a; };
protected:
	GLUquadricObj* q;
	std::vector<Abs_Entity*> gObjects;

	GLdouble ang = 0;
};

class IndexedBox : public Abs_Entity
{
public:
	IndexedBox(GLdouble);
	~IndexedBox() override;
	void render(const glm::dmat4& modelViewMat) const override;
	void update() override;

protected:
};


// -------------------------  TIE FIGHTER -----------------
class AdvancedTIE : public CompoundEntity
{
public:
	AdvancedTIE();
	~AdvancedTIE() override;
	void render(const glm::dmat4& modelViewMat) const override;

	void update() override;

protected:
	Abs_Entity* leftWing;
	Abs_Entity* rightWing;
	Abs_Entity* morro;
	QuadricEntity* base;
	QuadricEntity* cyl;
};

class WingAdvancedTIE : public Abs_Entity
{
public:
	WingAdvancedTIE(GLdouble x, GLdouble y, GLdouble rot, const std::string& t);
	~WingAdvancedTIE() override;
	void render(const glm::dmat4& modelViewMat) const override;

protected:
	Mesh* wing;
	glm::dmat4 mMat = glm::dmat4();
};

class BaseAdvancedTIE : public CompoundEntity
{
public:
	BaseAdvancedTIE();
	~BaseAdvancedTIE() override;
	void render(const glm::dmat4& modelViewMat) const override;

protected:
	QuadricEntity* cyl;
	QuadricEntity* disk;
};


/// ENTIDADES CON MALLAS POR REVOLUCION
class EntityWithMaterial : public Abs_Entity
{
protected:
	Material* material = nullptr;

public:
	EntityWithMaterial() = default;
	~EntityWithMaterial() override;
	void setMaterial(Material* m) { material = m; }
};

class RevSphere : public EntityWithMaterial
{
	glm::dvec3* profile;
public:
	RevSphere(GLint r, GLint p, GLint m);
	~RevSphere() override;
	void render(const glm::dmat4& modelViewMat) const override;
};

class Toroid : public Abs_Entity
{
	glm::dvec3* profile;
public:
	Toroid(GLint r, GLint R, GLint m, GLint p);
	~Toroid() override { delete mMesh; delete[] profile; }
	void render(const glm::dmat4& modelViewMat) const override;
};
#endif //_H_Entities_H_
