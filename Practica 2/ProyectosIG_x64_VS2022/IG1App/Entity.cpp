#include "Entity.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Abs_Entity::upload(const dmat4& modelViewMat) const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(modelViewMat)); // transfers modelView matrix to the GPU
}


/// EJES RGB
EjesRGB::EjesRGB(GLdouble l)
	: Abs_Entity()
{
	mMesh = Mesh::createRGBAxes(l);
}

EjesRGB::~EjesRGB()
{
	delete mMesh;
	mMesh = nullptr;
};

void
EjesRGB::render(const dmat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
	}
}


/// REGULAR POLYGON
RegularPolygon::RegularPolygon(GLuint num, GLdouble r)
{
	mMesh = Mesh::generateRegularPolygon(num, r);
}

RegularPolygon::~RegularPolygon()
{
	delete mMesh;
	mMesh = nullptr;
}

RegularPolygon::RegularPolygon(GLuint num, GLdouble r, dvec4 color)
{
	mMesh = Mesh::generateRegularPolygon(num, r);
	setmColor(color);
}

void RegularPolygon::render(const dmat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		glColor4d(mColor.r, mColor.g, mColor.b, mColor.a);
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
		glColor4d(0, 0, 0, 0);
	}
}


/// RGB TRIANGLE
RGBTriangle::RGBTriangle(GLdouble l) : radio(0)
{
	mMesh = Mesh::generateRGBTriangle(l, 0, 0);
}

RGBTriangle::RGBTriangle(GLdouble l, GLdouble x) : radio(x)
{
	mMesh = Mesh::generateRGBTriangle(l, 0, 0);
	mModelMat = translate(mModelMat, dvec3(radio, 0, 0));
}

RGBTriangle::RGBTriangle(GLdouble l, GLuint x, GLuint y) : radio(x)
{
	mMesh = Mesh::generateRGBTriangle(l, x, y);
	mModelMat = translate(mModelMat, dvec3(x, y, 0));
}

RGBTriangle::~RGBTriangle()
{
	delete mMesh;
	mMesh = nullptr;
}

void RGBTriangle::render(const dmat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_POINTS);
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
	}
}

void RGBTriangle::update()
{
	// 15...
	// giro sobre si mismo
	mModelMat = rotate(mModelMat, radians(-angle), dvec3(0, 0.0, 1.0));

	// giro sobre la circunferencia
	int nose = 22; // si el radio es 25, por que me sale este numero, no entiendo cual es la ecuacion lo siento!
	mModelMat = translate(mModelMat, dvec3(0.0, -nose, 0.0));
}


/// RGB RECTANGLE
RGBRectangle::RGBRectangle(GLdouble w, GLdouble h)
{
	mMesh = Mesh::generateRGBRectangle(w, h);
}

RGBRectangle::~RGBRectangle()
{
	delete mMesh;
	mMesh = nullptr;
}

void RGBRectangle::render(const dmat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		glPolygonMode(GL_BACK, GL_LINE);
		glPolygonMode(GL_FRONT, GL_FILL);
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
	}
}


/// CUBE
Cube::Cube(GLdouble longitud)
{
	mMesh = Mesh::generateCube(longitud);
}

Cube::~Cube()
{
	delete mMesh;
	mMesh = nullptr;
}

void Cube::render(const dmat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_POINT);
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
	}
}

/// RGBCUBE
RGBCube::RGBCube(GLdouble longitud)
{
	mMesh = Mesh::generateRGBCube(longitud);
	int l = longitud / 2;
	mModelMat = translate(mModelMat, dvec3(l, l, -longitud + l)); // pos inicial
}

RGBCube::~RGBCube()
{
	delete mMesh;
	mMesh = nullptr;
}

void RGBCube::render(const dmat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
	}
}

void RGBCube::update()
{
	mModelMat = rotateAroundCenter(mModelMat, vec3(0, 0, 0), radians(angle), vec3(x, y, z));
	totalRotation += angle;
	if (totalRotation >= 180.0f)
	{
		// esto es TERRIBLE per ofunciona no me mires mal
		if (x == 1)
		{
			x = 0;
			y = 0;
			z = 1;
		}
		else if (y == 1)
		{
			x = 1;
			y = 0;
			x = 0;
		}
		else if (z == 1)
		{
			x = 0;
			y = 1;
			z = 0;
		}

		if (x == 0 && y == 0 && z == 0)
		{
			x = 1;
		}

		totalRotation = 0.0f;
	}
}

mat4 RGBCube::rotateAroundCenter(const mat4& matrix, const vec3& center, float angle, const vec3& axis)
{
	const mat4 translateToOriginMatrix = translate(mat4(1.0f), center);
	// matriz de traslacion de vuelta al origen inicial
	const mat4 rotationMatrix = rotate(mat4(1.0f), angle, axis); // matriz de la rotacion en cuestion
	const mat4 translateToCenterMatrix = translate(mat4(1.0f), -center);
	// matriz de traslsacion al centro que queremos????

	// lo que nos explico ruben en clase sobre el orden de multiplicacion de matrices que es inverso
	return translateToOriginMatrix * rotationMatrix
		* translateToCenterMatrix * matrix;
}
