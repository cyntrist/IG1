#ifndef _H_Mesh_H_
#define _H_Mesh_H_

#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include <vector>

class Mesh
{
public:
	Mesh() = default;
	virtual ~Mesh() = default;

	Mesh(const Mesh& m) = delete; // no copy constructor
	Mesh& operator=(const Mesh& m) = delete; // no copy assignment

	virtual void render() const;
	virtual void setGL() const;
	virtual void resetGL() const;

	GLuint size() const { return mNumVertices; }; // number of elements
	const std::vector<glm::dvec3>& vertices() const { return vVertices; };
	const std::vector<glm::dvec4>& colors() const { return vColors; };

	static Mesh* createRGBAxes(GLdouble l); // creates a new 3D-RGB axes mesh
	static Mesh* generateRegularPolygon(GLuint num, GLdouble r);
	static Mesh* generateRGBTriangle(GLdouble r, GLuint x, GLuint y);
	static Mesh* generateRectangle(GLdouble w, GLdouble h);
	static Mesh* generateRGBRectangle(GLdouble w, GLdouble h);
	static Mesh* generateCube(GLdouble longitud);
	static Mesh* generateRGBCube(GLdouble longitud);

	// ---- EXAMEN ------------
	static Mesh* generateDiamond(GLdouble longitud);

	/// PRACTICA 2
	static Mesh* generateRectangleTexCor(GLdouble w, GLdouble h);
	static Mesh* generateRectangleTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh);
	static Mesh* generateBoxOutline(GLdouble length);
	static Mesh* generateBoxOutlineTexColor(GLdouble longitud);
	static Mesh* generateStar3D(GLdouble re, GLuint np, GLdouble h);
	static Mesh* generateStar3DTexCor(GLdouble re, GLuint np, GLdouble h);
	static Mesh* generateGlassParapet(GLdouble w, GLdouble h);
	static Mesh* generateTIEWing(GLdouble h, GLdouble w, GLdouble d);

protected:
	GLuint mPrimitive =
		GL_TRIANGLES; // graphic primitive: GL_POINTS, GL_LINES, GL_TRIANGLES, ...
	GLuint mNumVertices = 0; // number of elements ( = vVertices.size())
	std::vector<glm::dvec3> vVertices; // vertex array
	std::vector<glm::dvec4> vColors; // color array
	virtual void draw() const;
	std::vector<glm::dvec3> vNormals; // vectores normales de los vertices
	std::vector<glm::dvec2> vTexCoords; // vertices de texturas (analogo a vVertices vColor)
};

#endif //_H_Scene_H_
