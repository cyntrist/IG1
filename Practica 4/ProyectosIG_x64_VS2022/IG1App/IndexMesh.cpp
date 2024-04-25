#include "IndexMesh.h"

#include <iostream>
#include <string>

glm::vec3 IndexMesh::calculoVectorNormalPorNewell(Cara c)
{
	glm::vec3 n = {0, 0, 0};
	for (int i = 0; i < c.numeroVertices; i++)
	{
		/*
	      Set Vertex Current to Polygon.verts[Index]
	      Set Vertex Next    to Polygon.verts[(Index plus 1) mod Polygon.vertexNumber]

	      Set Normal.x to Sum of Normal.x and (multiply (Current.y minus Next.y) by (Current.z plus Next.z))
	      Set Normal.y to Sum of Normal.y and (multiply (Current.z minus Next.z) by (Current.x plus Next.x))
	      Set Normal.z to Sum of Normal.z and (multiply (Current.x minus Next.x) by (Current.y plus Next.y))
		 **/

		/*std::cout << c.getIndiceVertice(c.vertices[i], vVertices) << std::endl;
		std::cout << c.getIndiceVertice(c.vertices[(i + 1) % c.numeroVertices]) << std::endl;

		const auto vertActual = vVertices[c.getIndiceVertice(c.vertices[i], vVertices)];
		const auto vertSiguiente = vVertices[c.getIndiceVertice(c.vertices[(i + 1) % c.numeroVertices])];*/

		std::cout << c.getIndice(i) << std::endl;
		std::cout << c.getIndice((i + 1) % c.numeroVertices) << std::endl;

		const auto vertActual = vVertices[c.getIndice(i)];
		const auto vertSiguiente = vVertices[c.getIndice((i + 1) % c.numeroVertices)];

		n.x += (vertActual.y - vertSiguiente.y) * (vertActual.z + vertSiguiente.z);
		n.y += (vertActual.z - vertSiguiente.z) * (vertActual.x + vertSiguiente.x);
		n.z += (vertActual.x - vertSiguiente.x) * (vertActual.y + vertSiguiente.y);
	}
	return normalize(n);
}

void IndexMesh::buildNormalVectors()
{
	int i = 0;
	vNormals.resize(mNumVertices);
	for (int i = 0; i < mNumVertices; i++)
		vNormals.emplace_back(0, 0, 0);
	for (const auto cara : vCaras)
	{
		vNormals[i] = calculoVectorNormalPorNewell(cara);
		i++;
	}
}

void IndexMesh::buildNormalVectorsV2() // version de la profe
{
	vNormals.resize(mNumVertices);

	std::vector<glm::dvec3> vAuxNormals = vNormals; // vector auxiliar

	for (int i = 0; i < nNumIndices / 3; i++)
	{
		glm::dvec3 v0 = vVertices[vIndices[i * 3]];
		glm::dvec3 v1 = vVertices[vIndices[i * 3 + 1]];
		glm::dvec3 v2 = vVertices[vIndices[i * 3 + 2]];

		glm::dvec3 v = v1 - v0;
		glm::dvec3 w = v2 - v0;

		const glm::dvec3 n = normalize(cross(v, w));


		vAuxNormals[vIndices[i * 3]] += n;
		vAuxNormals[vIndices[i * 3 + 1]] += n;
		vAuxNormals[vIndices[i * 3 + 2]] += n;
	}

	for (int i = 0; i < mNumVertices; i++)
		vNormals[i] = normalize(vAuxNormals[i]);
}

IndexMesh* IndexMesh::generateIndexedBox(GLdouble l)
{
	const auto mesh = new IndexMesh();

	/// VERTICES
	mesh->mNumVertices = 8;
	mesh->vVertices.reserve(mesh->mNumVertices);

	mesh->vVertices.emplace_back(l, l, -l); // v0
	mesh->vVertices.emplace_back(l, -l, -l); // v1
	mesh->vVertices.emplace_back(-l, -l, -l); // v2
	mesh->vVertices.emplace_back(-l, l, -l); // v3
	mesh->vVertices.emplace_back(l, l, l); // v4
	mesh->vVertices.emplace_back(l, -l, l); // v5
	mesh->vVertices.emplace_back(-l, -l, l); // v6
	mesh->vVertices.emplace_back(-l, l, l); // v7


	/// INDICES
	/// Define cuidadosamente los 36 índices que, de 3 en 3,
	/// determinan las 12 caras triangulares de la malla.
	/// Recuerda que los índices de estas caras deben darse
	/// en sentido ¡¡¡ANTI-HORARIO!!! según se mira el cubo desde su exterior
	mesh->nNumIndices = 36;
	mesh->vIndices = new GLuint[mesh->nNumIndices];

	// cara de abajo
	mesh->vIndices[0] = 0;
	mesh->vIndices[1] = 3;
	mesh->vIndices[2] = 2;

	mesh->vIndices[3] = 0;
	mesh->vIndices[4] = 2;
	mesh->vIndices[5] = 1;

	// cara de arriba
	mesh->vIndices[6] = 4;
	mesh->vIndices[7] = 5;
	mesh->vIndices[8] = 6;

	mesh->vIndices[9] = 4;
	mesh->vIndices[10] = 6;
	mesh->vIndices[11] = 7;

	// cara lateral cercana izquierda
	mesh->vIndices[12] = 0;
	mesh->vIndices[13] = 4;
	mesh->vIndices[14] = 7;

	mesh->vIndices[15] = 0;
	mesh->vIndices[16] = 7;
	mesh->vIndices[17] = 3;

	// cara lateral trasera derecha
	mesh->vIndices[18] = 1;
	mesh->vIndices[19] = 2;
	mesh->vIndices[20] = 6;

	mesh->vIndices[21] = 1;
	mesh->vIndices[22] = 6;
	mesh->vIndices[23] = 5;

	// cara lateral cercana derecha
	mesh->vIndices[24] = 0;
	mesh->vIndices[25] = 1;
	mesh->vIndices[26] = 5;

	mesh->vIndices[27] = 0;
	mesh->vIndices[28] = 5;
	mesh->vIndices[29] = 4;

	// cara lateral trasera izquierda
	mesh->vIndices[30] = 2;
	mesh->vIndices[31] = 3;
	mesh->vIndices[32] = 7;

	mesh->vIndices[33] = 2;
	mesh->vIndices[34] = 7;
	mesh->vIndices[35] = 6;

	/// CARAS
	int nV = 3;
	mesh->vCaras.resize(mesh->nNumIndices / nV);
	for (int i = 0; i < mesh->nNumIndices / nV; i++)
	{
		//std::cout << mesh->vIndices[i * nV] << std::endl;
		//std::cout << mesh->vIndices[i * nV + 1] << std::endl;
		//std::cout << mesh->vIndices[i * nV + 2] << std::endl;

		mesh->vCaras[i] = Cara(
			mesh->vIndices[i * nV],
			mesh->vIndices[i * nV + 1],
			mesh->vIndices[i * nV + 2]
		);
	}
	//std::cout << std::endl;

	/// COLORES
	mesh->vColors.reserve(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; i++)
		mesh->vColors.emplace_back(0, 1, 0, 1);

	/// NORMALES
	mesh->buildNormalVectors();

	return mesh;
}

IndexMesh* IndexMesh::generateIndexedBoxV2(GLdouble l) /// la version de la profe
{
	const auto mesh = new IndexMesh();

	/// VERTICES
	mesh->mNumVertices = 8;
	mesh->vVertices.reserve(mesh->mNumVertices);

	const GLdouble m = l / 2;

	mesh->vVertices.emplace_back(-m, m, m); // v0
	mesh->vVertices.emplace_back(-m, -m, m); // v1
	mesh->vVertices.emplace_back(m, m, m); // v2
	mesh->vVertices.emplace_back(m, -m, m); // v3
	mesh->vVertices.emplace_back(m, m, -m); // v4
	mesh->vVertices.emplace_back(m, -m, -m); // v5
	mesh->vVertices.emplace_back(-m, m, -m); // v6
	mesh->vVertices.emplace_back(-m, -m, -m); // v7

	/// INDICES
	mesh->nNumIndices = 36;
	mesh->vIndices = new GLuint[mesh->nNumIndices];
	const GLuint arr[36] =
	{
		0, 1, 2, 1, 3, 2, 2, 3, 4,
		3, 5, 4, 4, 5, 6, 5, 7, 6,
		//diagonal como el resto en la cara lateral izquierda
		//6, 7, 0, 7, 1, 0,
		//diagonal al contrario del resto en la cara lateral izquierda
		0, 6, 1, 6, 7, 1,
		0, 2, 4, 4, 6, 0, 1, 5, 3, 1, 7, 5
	};

	for (int i = 0; i < mesh->nNumIndices; i++)
		mesh->vIndices[i] = arr[i];

	/// CARAS
	int nV = 3;
	mesh->vCaras.resize(mesh->nNumIndices / nV);
	for (int i = 0; i < mesh->nNumIndices / nV; i++)
	{
		//std::cout << mesh->vIndices[i * nV] << std::endl;
		//std::cout << mesh->vIndices[i * nV + 1] << std::endl;
		//std::cout << mesh->vIndices[i * nV + 2] << std::endl;

		mesh->vCaras[i] = Cara(
			mesh->vIndices[i * nV],
			mesh->vIndices[i * nV + 1],
			mesh->vIndices[i * nV + 2]
		);
	}

	/// COLORES
	mesh->vColors.reserve(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; i++)
		mesh->vColors.emplace_back(0, 1, 0, 1);


	/// NORMALES
	mesh->buildNormalVectorsV2();

	return mesh;
}

void IndexMesh::render() const
{
	if (vVertices.empty()) return;

	setGL();
	if (vIndices != nullptr)
	{
		glEnableClientState(GL_INDEX_ARRAY);
		glIndexPointer(GL_UNSIGNED_INT, 0, vIndices);
	}

	glColorMaterial(GL_FRONT_AND_BACK, GL_FILL);
	draw();
	glDisableClientState(GL_INDEX_ARRAY);
	resetGL();
}

void IndexMesh::draw() const
{
	glDrawElements(mPrimitive, nNumIndices,
	               GL_UNSIGNED_INT, vIndices);
}
