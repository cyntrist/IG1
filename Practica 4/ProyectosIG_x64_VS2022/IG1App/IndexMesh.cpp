#include "IndexMesh.h"

#include "Cara.h"

glm::vec3 IndexMesh::calculoVectorNormalPorNewell()
{
	glm::vec3 n = {0, 0, 0};
	for (int i = 0; i < nNumIndices; i++)
	{
		/*
	      Set Vertex Current to Polygon.verts[Index]
	      Set Vertex Next    to Polygon.verts[(Index plus 1) mod Polygon.vertexNumber]

	      Set Normal.x to Sum of Normal.x and (multiply (Current.y minus Next.y) by (Current.z plus Next.z))
	      Set Normal.y to Sum of Normal.y and (multiply (Current.z minus Next.z) by (Current.x plus Next.x))
	      Set Normal.z to Sum of Normal.z and (multiply (Current.x minus Next.x) by (Current.y plus Next.y))
		 **/

		/// O ES ESTE
		auto vertActual = vVertices[i];
		auto vertSiguiente = vVertices[(i+1)%mNumVertices];

		n.x +=( vertActual.y- vertSiguiente .y) * ( vertActual .z+ vertSiguiente .z);
		n.y +=( vertActual.z- vertSiguiente .z) * ( vertActual .x+ vertSiguiente .x);
		n.z +=( vertActual.x- vertSiguiente .x) * ( vertActual .y+ vertSiguiente .y);
	}
	return normalize(n);
}

void IndexMesh::buildNormalVectors()
{
}

IndexMesh* IndexMesh::generateIndexedBox(GLdouble l)
{
	const auto mesh = new IndexMesh();

	mesh->mNumVertices = 8;
	mesh->vVertices.reserve(mesh->mNumVertices);

	/// vertices del cubo
	mesh->vVertices.emplace_back(l, l, -l); // v0
	mesh->vVertices.emplace_back(l, -l, -l); // v1
	mesh->vVertices.emplace_back(-l, -l, -l); // v2
	mesh->vVertices.emplace_back(-l, l, -l); // v3
	mesh->vVertices.emplace_back(l, l, l); // v4
	mesh->vVertices.emplace_back(l, -l, l); // v5
	mesh->vVertices.emplace_back(-l, -l, l); // v6
	mesh->vVertices.emplace_back(-l, l, l); // v7


	/// Define cuidadosamente los 36 índices que, de 3 en 3,
	/// determinan las 12 caras triangulares de la malla.
	/// Recuerda que los índices de estas caras deben darse
	/// en sentido anti-horario según se mira el cubo desde su exterior
	mesh->nNumIndices = 36;
	mesh->vIndices = new GLuint[mesh->nNumIndices];

	// cara de abajo
	mesh->vIndices[0] = 0;
	mesh->vIndices[1] = 1;
	mesh->vIndices[2] = 2;

	mesh->vIndices[3] = 0;
	mesh->vIndices[4] = 2;
	mesh->vIndices[5] = 4;

	// cara de arriba
	mesh->vIndices[6] = 4;
	mesh->vIndices[7] = 5;
	mesh->vIndices[8] = 6;

	mesh->vIndices[9] = 4;
	mesh->vIndices[10] = 6;
	mesh->vIndices[11] = 7;

	// cara lateral cercana izquierda
	mesh->vIndices[12] = 3;
	mesh->vIndices[13] = 0;
	mesh->vIndices[14] = 4;

	mesh->vIndices[15] = 3;
	mesh->vIndices[16] = 4;
	mesh->vIndices[17] = 7;

	// cara lateral trasera derecha
	mesh->vIndices[18] = 2;
	mesh->vIndices[19] = 1;
	mesh->vIndices[20] = 5;

	mesh->vIndices[21] = 2;
	mesh->vIndices[22] = 5;
	mesh->vIndices[23] = 6;

	// cara lateral cercana derecha
	mesh->vIndices[24] = 0;
	mesh->vIndices[25] = 1;
	mesh->vIndices[26] = 5;

	mesh->vIndices[27] = 0;
	mesh->vIndices[28] = 5;
	mesh->vIndices[29] = 4;

	// cara lateral trasera izquierda
	mesh->vIndices[30] = 3;
	mesh->vIndices[31] = 2;
	mesh->vIndices[32] = 6;

	mesh->vIndices[33] = 3;
	mesh->vIndices[34] = 6;
	mesh->vIndices[35] = 7;

	/// COLORES
	mesh->vColors.reserve(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; i++)
		mesh->vColors.emplace_back(0, 1, 0, 1);

	return mesh;
}

void IndexMesh::render() const
{
	setGL();
	// Comandos OpenGL para enviar datos de arrays a GPU
	// Nuevos comandos para la tabla de índices
	if (vIndices != nullptr)
	{
		glEnableClientState(GL_INDEX_ARRAY);
		glIndexPointer(GL_UNSIGNED_INT, 0, vIndices);
	}
	// Comandos OpenGL para deshabilitar datos enviados
	// Nuevo comando para la tabla de índices :
	draw();
	glDisableClientState(GL_INDEX_ARRAY);
	resetGL();
}

void IndexMesh::draw() const
{
	glDrawElements(mPrimitive, nNumIndices,
	               GL_UNSIGNED_INT, vIndices);
}
