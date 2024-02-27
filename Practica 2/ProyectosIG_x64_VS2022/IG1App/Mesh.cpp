#include "Mesh.h"
#include <array>
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

		if (vTexCoords.size() > 0)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_DOUBLE, 0, vTexCoords.data());
		}
		draw();
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

	}
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
		mesh->vVertices.emplace_back(r * cos(alpha + radians(dividido * i)),r * sin(alpha + radians(dividido * i)), 1.0);
	}

	glEnd(); //end drawing of line loop

	return mesh;
}

Mesh* Mesh::generateRGBTriangle(GLdouble r, GLuint x, GLuint y)
{
	auto mesh = new Mesh();
	/*glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_POINTS);*/
	mesh->mNumVertices = 3;
	mesh->vVertices.reserve(mesh->mNumVertices);
	// prpara una polilinea (okolilinea)

	// angulo entre vertices
	double dividido = 360.0 / mesh->mNumVertices;

	// angulo inicial
	constexpr double alpha = radians(90.0);

	glBegin(mesh->mPrimitive); //start drawing a line loop

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		// x = Cx + R*cos(alpha)
		// y = Cy + R*sen(alpha) 
		mesh->vVertices.emplace_back(r * cos((alpha + radians(dividido * i))) + x, (r * sin(alpha + radians(dividido * i))) + y,
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
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
	
	/*
	constexpr std::array PIVOTS = {
		dvec2(-1, 1),
		dvec2(1, -1),
		dvec2(1, 1)
	};

	for (int edge = 0; edge < 3; ++edge)
	{
		for (int value : {-n, n})
		{
			dvec3 center = { 0, 0, 0 };
			center[edge] = value;

			dvec3 u = { 0, 0, 0 };
			u[(edge + 1) % 3] = 1;
			dvec3 v = cross(u, normalize(-center));

			for (double triangle : {n, -n})
				for (const dvec2& m : PIVOTS)
					mesh->vVertices.push_back(center + triangle + (m.x * u + m.y * v));
		}
	}

	*/

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
	mesh->vVertices.emplace_back(-m,- m, m); // v31 = v28
	mesh->vVertices.emplace_back(m, -m, m); // v32 = v3
	// ^^^^ - en z


	mesh->vColors.reserve(mesh->mNumVertices);
	// cara 1
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	// cara 2
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	// cara 3
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	// cara 4
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	// cara 5
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	// cara 6
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);



	//mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	

	return mesh;
}

Mesh* Mesh::generateRectangleTexCor(GLdouble w, GLdouble h)
{
	auto mesh = generateRectangle(w, h);

	mesh->vTexCoords.emplace_back(0,0);
	mesh->vTexCoords.emplace_back(1,0);
	mesh->vTexCoords.emplace_back(0,1);
	mesh->vTexCoords.emplace_back(1,1);

	return mesh;
}

Mesh* Mesh::generateRectangleTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh)
{
	auto mesh = generateRectangle(w, h);

	mesh->vTexCoords.emplace_back(0,0);
	mesh->vTexCoords.emplace_back(rw,0);
	mesh->vTexCoords.emplace_back(0,rh);
	mesh->vTexCoords.emplace_back(rw,rh);

	return mesh;
}