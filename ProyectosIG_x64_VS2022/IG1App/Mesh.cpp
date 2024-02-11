#include "Mesh.h"
#include "CheckML.h"
#include <fstream>
using namespace std;
using namespace glm;

void
Mesh::draw() const
{
	glDrawArrays(
		mPrimitive,
		0,
		size()); // primitive graphic, first index and number of elements to be rendered
}

void
Mesh::render() const
{
	if (vVertices.size() > 0)
	{
		// transfer data
		// transfer the coordinates of the vertices
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(
			3, GL_DOUBLE, 0, vVertices.data()); // number of coordinates per vertex, type of
		// each coordinate, stride, pointer
		if (vColors.size() > 0)
		{
			// transfer colors
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(
				4, GL_DOUBLE, 0, vColors.data()); // components number (rgba=4), type of
			// each component, stride, pointer
		}
		draw();
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

Mesh* Mesh::generateRegularPolygon(GLuint num, GLdouble r)
{
	auto* mesh = new Mesh();
	mesh->mPrimitive = GL_LINE_LOOP;
	mesh->mNumVertices = num;
	mesh->vVertices.reserve(mesh->mNumVertices);

	// numero de vertices
	const double dividido = 360.0 / num;

	// angulo inicial
	constexpr double alpha = radians(90.0);

	glBegin(mesh->mPrimitive); //start drawing a line loop

	for (int i = 0; i < num; i++)
	{
		// x = Cx + R*cos(alpha)
		// y = Cy + R*sen(alpha) 
		mesh->vVertices.emplace_back(r * cos(alpha + radians(dividido * i)), alpha +r * sin(radians(dividido * i)), 1.0);
	}

	glEnd(); //end drawing of line loop

	return mesh;
}

Mesh* Mesh::generateRGBTriangle(GLuint num, GLdouble r)
{
	auto mesh = new Mesh();
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_POINTS);
	mesh->mNumVertices = num;
	mesh->vVertices.reserve(mesh->mNumVertices);
	// prpara una polilinea (okolilinea)

	// angulo entre vertices
	double dividido = 360.0 / num;

	// angulo inicial
	constexpr double alpha = radians(90.0);

	glBegin(mesh->mPrimitive); //start drawing a line loop

	for (int i = 0; i < num; i++)
	{
		// x = Cx + R*cos(alpha)
		// y = Cy + R*sen(alpha) 
		mesh->vVertices.emplace_back(r * cos(alpha + radians(dividido * i)), r * sin(alpha + radians(dividido * i)),
		                             1.0);
	}

	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	glEnd(); //end drawing of line loop

	return mesh;
}

Mesh* Mesh::generateRectangle(GLdouble w, GLdouble h)
{
	auto* mesh = new Mesh();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 4;
	mesh->vVertices.reserve(mesh->mNumVertices);

	glBegin(mesh->mPrimitive); // start drawing a line loop
	mesh->vVertices.emplace_back(-w/2, h/2, 0.0);
	mesh->vVertices.emplace_back(w/2, h/2, 0.0);
	mesh->vVertices.emplace_back(-w/2, -h/2, 0.0);
	mesh->vVertices.emplace_back(w/2, -h/2, 0.0);
	glEnd(); //end drawing of line loop

	return mesh;
}

Mesh* Mesh::generateRGBRectangle(GLdouble w, GLdouble h)
{
	auto* mesh = new Mesh();
	glPolygonMode(GL_BACK, GL_LINE);
	glPolygonMode(GL_FRONT, GL_FILL);
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 4;
	mesh->vVertices.reserve(mesh->mNumVertices);

	glBegin(mesh->mPrimitive); // start drawing a line loop
	mesh->vVertices.emplace_back(-w/2, h/2, 0.0);
	mesh->vVertices.emplace_back(w/2, h/2, 0.0);
	mesh->vVertices.emplace_back(-w/2, -h/2, 0.0);
	mesh->vVertices.emplace_back(w/2, -h/2, 0.0);


	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	glEnd(); //end drawing of line loop

	return mesh;
}

Mesh* Mesh::generateCube(GLdouble longitud)
{
	auto* mesh = new Mesh();
	return mesh;
}


Mesh*
Mesh::createRGBAxes(GLdouble l)
{
	auto mesh = new Mesh();

	mesh->mPrimitive = GL_LINES;

	mesh->mNumVertices = 6;
	mesh->vVertices.reserve(mesh->mNumVertices);

	// X axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(l, 0.0, 0.0);
	// Y axis vertices
	mesh->vVertices.emplace_back(0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, l, 0.0);
	// Z axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, 0.0, l);

	mesh->vColors.reserve(mesh->mNumVertices);
	// X axis color: red  (Alpha = 1 : fully opaque)
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	// Y axis color: green
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	// Z axis color: blue
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}
