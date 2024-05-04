#include "Mesh.h"
#include "CheckML.h"
#include <array>
#include <fstream>
#include <iostream>
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
		setGL();
		draw();
		resetGL();
	}
}

void Mesh::setGL() const
{
	// transfer data
	// transfer the coordinates of the vertices
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(
		3, GL_DOUBLE, 0, vVertices.data()); // number of coordinates per vertex, type of
	// each coordinate, stride, pointer

	/// si tiene vertices de color
	if (!vColors.empty())
	{
		// transfer colors
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(
			4, GL_DOUBLE, 0, vColors.data()); // components number (rgba=4), type of
		// each component, stride, pointer
	}

	/// si tiene vertices de textura
	if (!vTexCoords.empty())
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_DOUBLE, 0, vTexCoords.data());
	}

	if (!vNormals.empty())
	{
		//glEnable(GL_NORMALIZE);
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_DOUBLE, 0, vNormals.data());
	}
}

void Mesh::resetGL() const
{
	//glDisable(GL_NORMALIZE);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
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
		mesh->vVertices.emplace_back(r * cos(alpha + radians(dividido * i)), r * sin(alpha + radians(dividido * i)),
		                             1.0);
	}

	glEnd(); //end drawing of line loop

	return mesh;
}

Mesh* Mesh::generateRGBTriangle(GLdouble r, GLuint x, GLuint y)
{
	auto mesh = generateRegularPolygon(3, r);

	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}

Mesh* Mesh::generateRectangle(GLdouble w, GLdouble h)
{
	auto* mesh = new Mesh();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 4;
	mesh->vVertices.reserve(mesh->mNumVertices);

	glBegin(mesh->mPrimitive); // start drawing a line loop
	mesh->vVertices.emplace_back(-w / 2, h / 2, 0.0);
	mesh->vVertices.emplace_back(w / 2, h / 2, 0.0);
	mesh->vVertices.emplace_back(-w / 2, -h / 2, 0.0);
	mesh->vVertices.emplace_back(w / 2, -h / 2, 0.0);
	glEnd(); //end drawing of line loop

	return mesh;
}

Mesh* Mesh::generateRGBRectangle(GLdouble w, GLdouble h)
{
	auto* mesh = generateRectangle(w, h);

	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}

Mesh* Mesh::generateCube(GLdouble l)
{
	auto* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	/*glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_POINT);*/
	mesh->mNumVertices = 14;
	mesh->vVertices.reserve(mesh->mNumVertices);

	/// EN CLASE:
	// 14 VERTICES!!!!!!!
	const GLdouble m = l / 2;

	mesh->vVertices.emplace_back(-m, -m, -m); // v0
	mesh->vVertices.emplace_back(m, -m, -m); // v1
	mesh->vVertices.emplace_back(-m, -m, m); // v2

	mesh->vVertices.emplace_back(m, -m, m); // v3

	mesh->vVertices.emplace_back(m, m, m); // v4
	mesh->vVertices.push_back(mesh->vVertices[1]); // v5 = v1
	mesh->vVertices.emplace_back(m, m, -m); // v6

	mesh->vVertices.push_back(mesh->vVertices[0]); // v7 = v0
	mesh->vVertices.emplace_back(-m, m, -m); // v8
	mesh->vVertices.push_back(mesh->vVertices[2]); // v9 = v2
	mesh->vVertices.emplace_back(-m, m, m); // v10
	mesh->vVertices.push_back(mesh->vVertices[4]); // v11 = v4
	mesh->vVertices.push_back(mesh->vVertices[8]); // v12 = v8
	mesh->vVertices.push_back(mesh->vVertices[6]); // v13 = v6

	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);

	return mesh;
}

Mesh* Mesh::generateRGBCube(GLdouble l)
{
	auto* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLES;
	/*glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);*/
	mesh->mNumVertices = 100;
	mesh->vVertices.reserve(mesh->mNumVertices);

	/// APARTADO 10:
	///	Con primitiva Triangle normal, para poder darle color a cada cara
	///
	///	algo tiene que estar mal de lo que he copiado de la pizarra porque no funciona
	///	prefiero hacerlo de manera manual, pero lo dejo aqui como referencia
	///	o por si acaso


	GLdouble m = l / 2;

	// triangulo 1 cara 1
	mesh->vVertices.emplace_back(-m, -m, -m); // v0
	mesh->vVertices.emplace_back(m, -m, -m); // v1
	mesh->vVertices.emplace_back(-m, -m, m); // v2
	// triangulo 2 cara 1
	mesh->vVertices.emplace_back(m, -m, m); // v3
	mesh->vVertices.push_back(mesh->vVertices[1]); // v4 = v1
	mesh->vVertices.push_back(mesh->vVertices[2]); // v5 = v2


	// triangulo 1 cara 2
	mesh->vVertices.push_back(mesh->vVertices[3]); // v6 = v3
	mesh->vVertices.emplace_back(m, m, m); // v7
	mesh->vVertices.push_back(mesh->vVertices[1]); // v8 = v5 = v2
	// triangulo 2 cara 2
	mesh->vVertices.emplace_back(m, m, -m); // v9
	mesh->vVertices.push_back(mesh->vVertices[7]); // v10 = v7
	mesh->vVertices.push_back(mesh->vVertices[1]); // v11 = v1


	// triangulo 1 cara 3
	mesh->vVertices.push_back(mesh->vVertices[9]); // v12 = v9
	mesh->vVertices.push_back(mesh->vVertices[0]); // v13 = v0
	mesh->vVertices.emplace_back(-m, m, -m); // v14
	// triangulo 2 cara 3
	mesh->vVertices.push_back(mesh->vVertices[9]); // v12 = v9
	mesh->vVertices.push_back(mesh->vVertices[0]); // v13 = v0
	mesh->vVertices.push_back(mesh->vVertices[1]); // v14 = v1

	// triangulo 1 cara 4
	mesh->vVertices.emplace_back(-m, m, -m); // v15 = -v0
	mesh->vVertices.emplace_back(m, m, -m); // v16 = -v1
	mesh->vVertices.emplace_back(-m, m, m); // v17 = -v2
	// triangulo 2 cara 4
	mesh->vVertices.emplace_back(m, m, m); // v18 = -v3
	mesh->vVertices.emplace_back(m, m, -m); // v19 = v16 = -v4 = -v1
	mesh->vVertices.emplace_back(-m, m, m); // v20 = v17 = -v5 = -v2
	// ^^^^ - en y


	// triangulo 1 cara 5
	mesh->vVertices.emplace_back(-m, -m, m); // v21 = -v6 = -v3
	mesh->vVertices.emplace_back(-m, m, m); // v22 = -v7
	mesh->vVertices.emplace_back(-m, -m, -m); // v23 = 
	// triangulo 2 cara 5
	mesh->vVertices.emplace_back(-m, m, -m); // v24 = -v9
	mesh->vVertices.emplace_back(-m, m, m); // v25 = -v10 = -v7
	mesh->vVertices.emplace_back(-m, -m, -m); // v26 = -v11 = -v1
	// ^^^^ - en x

	// triangulo 1 cara 6
	mesh->vVertices.emplace_back(m, m, m); // v27 = -v9
	mesh->vVertices.emplace_back(-m, -m, m); // v28 = -v0
	mesh->vVertices.emplace_back(-m, m, m); // v29
	// triangulo 2 cara 6
	mesh->vVertices.emplace_back(m, m, m); // v30 = v27
	mesh->vVertices.emplace_back(-m, -m, m); // v31 = v28
	mesh->vVertices.emplace_back(m, -m, m); // v32 = v3
	// ^^^^ - en z


	mesh->vColors.reserve(mesh->mNumVertices);
	// cara 1
	for (int i = 0; i < 6; i++)
		mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);

	// cara 2
	for (int i = 0; i < 6; i++)
		mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);

	// cara 3
	for (int i = 0; i < 6; i++)
		mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	// cara 4
	for (int i = 0; i < 6; i++)
		mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);

	// cara 5
	for (int i = 0; i < 6; i++)
		mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);

	// cara 6
	for (int i = 0; i < 6; i++)
		mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	//mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);


	return mesh;
}


/// Rectangle
Mesh* Mesh::generateRectangleTexCor(GLdouble w, GLdouble h)
{
	auto mesh = generateRectangle(w, h);

	mesh->vTexCoords.emplace_back(0, 0);
	mesh->vTexCoords.emplace_back(1, 0);
	mesh->vTexCoords.emplace_back(0, 1);
	mesh->vTexCoords.emplace_back(1, 1);

	return mesh;
}

Mesh* Mesh::generateRectangleTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh)
{
	auto mesh = generateRectangle(w, h);

	mesh->vTexCoords.reserve(mesh->vVertices.size());
	mesh->vTexCoords.emplace_back(0, 0);
	mesh->vTexCoords.emplace_back(rw, 0);
	mesh->vTexCoords.emplace_back(0, rh);
	mesh->vTexCoords.emplace_back(rw, rh);

	return mesh;
}


/// Box Outline
Mesh* Mesh::generateBoxOutline(GLdouble length)
{
	auto* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 10;
	mesh->vVertices.reserve(mesh->mNumVertices);

	const GLdouble m = length / 2;

	mesh->vVertices.emplace_back(m, -m, m); // v0
	mesh->vVertices.emplace_back(m, m, m); // v1
	mesh->vVertices.emplace_back(m, -m, -m); // v2

	mesh->vVertices.emplace_back(m, m, -m); // v3
	mesh->vVertices.emplace_back(-m, -m, -m); // v4
	mesh->vVertices.emplace_back(-m, m, -m); // v5

	mesh->vVertices.emplace_back(-m, -m, m); // v6
	mesh->vVertices.emplace_back(-m, m, m); // v7

	mesh->vVertices.push_back(mesh->vVertices[0]); // v8
	mesh->vVertices.push_back(mesh->vVertices[1]); // v9

	mesh->vColors.reserve(mesh->mNumVertices);

	return mesh;
}

Mesh* Mesh::generateBoxOutlineTexColor(GLdouble longitud)
{
	auto mesh = generateBoxOutline(longitud);
	mesh->vTexCoords.reserve(mesh->mNumVertices);

	mesh->vTexCoords.emplace_back(0, 1);
	mesh->vTexCoords.emplace_back(0, 0);
	mesh->vTexCoords.emplace_back(1, 1);
	mesh->vTexCoords.emplace_back(1, 0);

	mesh->vTexCoords.emplace_back(0, 1);
	mesh->vTexCoords.emplace_back(0, 0);
	mesh->vTexCoords.emplace_back(1, 1);
	mesh->vTexCoords.emplace_back(1, 0);

	mesh->vTexCoords.emplace_back(0, 1);
	mesh->vTexCoords.emplace_back(0, 0);

	return mesh;
}

Mesh* Mesh::generateStar3D(GLdouble re, GLuint np, GLdouble h)
{
	auto* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_FAN;
	mesh->mNumVertices = np * 2 + 2;
	mesh->vVertices.reserve(mesh->mNumVertices);

	// numero de vertices
	const double alpha = 360.0 / np;

	// angulo inicial
	constexpr double offset = radians(90.0);

	glBegin(mesh->mPrimitive); //start drawing a line loop

	mesh->vVertices.emplace_back(0, 0, 0);
	for (int i = 0; i < np; i++)
	{
		// x = Cx + R*cos(alpha)
		// y = Cy + R*sen(alpha)

		mesh->vVertices.emplace_back(
			re * cos(offset + radians(alpha * i)),
			re * sin(offset + radians(alpha * i)),
			h);
		mesh->vVertices.emplace_back(
			re / 2 * cos(offset + radians((alpha) * (i + 1)) - radians(alpha / 2)),
			re / 2 * sin(offset + radians((alpha) * (i + 1)) - radians(alpha / 2)),
			h);
	}
	mesh->vVertices.push_back(mesh->vVertices[1]);


	glEnd(); //end drawing of line loop

	return mesh;
}

Mesh* Mesh::generateStar3DTexCor(GLdouble re, GLuint np, GLdouble h)
{
	auto mesh = generateStar3D(re, np, h);
	mesh->vTexCoords.reserve(mesh->mNumVertices);

	mesh->vTexCoords.emplace_back(0.5, 0.5);
	for (int i = 0; i < 4; i++)
	{
		mesh->vTexCoords.emplace_back(0, 1);
		mesh->vTexCoords.emplace_back(0.5, 1);
		mesh->vTexCoords.emplace_back(0, 1);
		mesh->vTexCoords.emplace_back(0, 0.5);
	}
	mesh->vTexCoords.emplace_back(0.0, 1.0);

	return mesh;
}

Mesh* Mesh::generateGlassParapet(GLdouble w, GLdouble h)
{
	auto* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 10;
	mesh->vVertices.reserve(mesh->mNumVertices);

	auto x = w / 2;
	auto y = h / 2;

	mesh->vVertices.emplace_back(x, -y, x); // v0
	mesh->vVertices.emplace_back(x, y, x); // v1
	mesh->vVertices.emplace_back(x, -y, -x); // v2

	mesh->vVertices.emplace_back(x, y, -x); // v3
	mesh->vVertices.emplace_back(-x, -y, -x); // v4
	mesh->vVertices.emplace_back(-x, y, -x); // v5

	mesh->vVertices.emplace_back(-x, -y, x); // v6
	mesh->vVertices.emplace_back(-x, y, x); // v7

	mesh->vVertices.push_back(mesh->vVertices[0]); // v8
	mesh->vVertices.push_back(mesh->vVertices[1]); // v9


	mesh->vColors.reserve(mesh->mNumVertices);
	for (int i = 0; i < 4; i++)
	{
		mesh->vTexCoords.emplace_back(0, 0);
		mesh->vTexCoords.emplace_back(1, 0);
		mesh->vTexCoords.emplace_back(0, 1);
		mesh->vTexCoords.emplace_back(1, 1);
	}

	return mesh;
}

Mesh* Mesh::generateTIEWing(GLdouble h, GLdouble w, GLdouble d)
{
	auto* mesh = new Mesh();
	auto x = h / 2;
	auto y = w / 2;

	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 8;
	mesh->vVertices.reserve(mesh->mNumVertices);


	// 
	mesh->vVertices.emplace_back(x, y, d); // v0
	mesh->vVertices.emplace_back(x, -y, d); // v1
	mesh->vVertices.emplace_back(d, y, 0); // v2
	mesh->vVertices.emplace_back(d, -y, 0); // v3
	mesh->vVertices.emplace_back(-d, y, 0); // v4
	mesh->vVertices.emplace_back(-d, -y, 0); // v5
	mesh->vVertices.emplace_back(-x, y, d); // v0
	mesh->vVertices.emplace_back(-x, -y, d); // v0

	/*mesh->vColors.reserve(mesh->mNumVertices);
	for (int i = 0; i < 4; i++)
	{
		mesh->vTexCoords.emplace_back(0, 0);
		mesh->vTexCoords.emplace_back(1, 0);
		mesh->vTexCoords.emplace_back(0, 1);
		mesh->vTexCoords.emplace_back(1, 1);
	}*/


	return mesh;
}
