#include "IndexMesh.h"

glm::vec3 IndexMesh::calculoVectorNormalPorNewell()
{
	glm::vec3 n = {0, 0, 0};
	//for (auto i : 10)
	//{
	//	/*vertActual = vertice[C->getVerticeIndice(i)];
	//	vertSiguiente = vertice[C->getVerticeIndice((i + 1) % C.numeroVertices)];
	//	n.x += (vertActual.y - vertSiguiente.y) * (vertActual.z + vertSiguiente.z);
	//	n.y += (vertActual.z - vertSiguiente.z) * (vertActual.x + vertSiguiente.x);
	//	n.z += (vertActual.x - vertSiguiente.x) * (vertActual.y + vertSiguiente.y);
	//*/}
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
	mesh->vColors.reserve(mesh->mNumVertices);

	/// vertices del cubo
	mesh->vVertices.emplace_back(l, l, -l); // v0
	mesh->vVertices.emplace_back(l, -l, -l); // v1
	mesh->vVertices.emplace_back(-l, -l, -l); // v2
	mesh->vVertices.emplace_back(-l, l, -l); // v3
	mesh->vVertices.emplace_back(l, l, l); // v4
	mesh->vVertices.emplace_back(l, -l, l); // v5
	mesh->vVertices.emplace_back(-l, -l, l); // v6
	mesh->vVertices.emplace_back(-l, l, l); // v7

	for (int i = 0; i < mesh->mNumVertices; i++) 
		mesh->vColors.emplace_back(0, 1, 0, 1);

	/// Define cuidadosamente los 36 índices que, de 3 en 3,
	/// determinan las 12 caras triangulares de la malla.
	/// Recuerda que los índices de estas caras deben darse
	/// en sentido anti-horario según se mira el cubo desde su exterior
	mesh->vIndices = new GLuint[36];

	mesh->vIndices[0] = 5;
	mesh->vIndices[1] = 1;
	mesh->vIndices[2] = 0;
	mesh->vIndices[3] = 0;
	mesh->vIndices[4] = 4;
	mesh->vIndices[5] = 5;
	mesh->vIndices[6] = 6;
	mesh->vIndices[7] = 2;
	mesh->vIndices[8] = 1;
	mesh->vIndices[9] = 6;
	mesh->vIndices[10] = 1;
	mesh->vIndices[11] = 5;
	mesh->vIndices[12] = 1;
	mesh->vIndices[13] = 3;
	mesh->vIndices[14] = 0;
	mesh->vIndices[15] = 2;
	mesh->vIndices[16] = 3;
	mesh->vIndices[17] = 1;
	mesh->vIndices[18] = 7;
	mesh->vIndices[19] = 4;
	mesh->vIndices[20] = 0;
	mesh->vIndices[21] = 0;
	mesh->vIndices[22] = 3;
	mesh->vIndices[23] = 7;
	mesh->vIndices[24] = 5;
	mesh->vIndices[25] = 4;
	mesh->vIndices[26] = 7;
	mesh->vIndices[27] = 6;
	mesh->vIndices[28] = 5;
	mesh->vIndices[29] = 7;
	mesh->vIndices[30] = 2;
	mesh->vIndices[31] = 7;
	mesh->vIndices[32] = 3;
	mesh->vIndices[33] = 6;
	mesh->vIndices[34] = 7;
	mesh->vIndices[35] = 2;

#pragma region RGBCUBE original
	//GLdouble m = l / 2;

	//// triangulo 1 cara 1
	//mesh->vVertices.emplace_back(-m, -m, -m); // v0
	//mesh->vVertices.emplace_back(m, -m, -m); // v1
	//mesh->vVertices.emplace_back(-m, -m, m); // v2
	//// triangulo 2 cara 1
	//mesh->vVertices.emplace_back(m, -m, m); // v3
	//mesh->vVertices.push_back(mesh->vVertices[1]); // v4 = v1
	//mesh->vVertices.push_back(mesh->vVertices[2]); // v5 = v2


	//// triangulo 1 cara 2
	//mesh->vVertices.push_back(mesh->vVertices[3]); // v6 = v3
	//mesh->vVertices.emplace_back(m, m, m); // v7
	//mesh->vVertices.push_back(mesh->vVertices[1]); // v8 = v5 = v2
	//// triangulo 2 cara 2
	//mesh->vVertices.emplace_back(m, m, -m); // v9
	//mesh->vVertices.push_back(mesh->vVertices[7]); // v10 = v7
	//mesh->vVertices.push_back(mesh->vVertices[1]); // v11 = v1


	//// triangulo 1 cara 3
	//mesh->vVertices.push_back(mesh->vVertices[9]); // v12 = v9
	//mesh->vVertices.push_back(mesh->vVertices[0]); // v13 = v0
	//mesh->vVertices.emplace_back(-m, m, -m); // v14
	//// triangulo 2 cara 3
	//mesh->vVertices.push_back(mesh->vVertices[9]); // v12 = v9
	//mesh->vVertices.push_back(mesh->vVertices[0]); // v13 = v0
	//mesh->vVertices.push_back(mesh->vVertices[1]); // v14 = v1

	//// triangulo 1 cara 4
	//mesh->vVertices.emplace_back(-m, m, -m); // v15 = -v0
	//mesh->vVertices.emplace_back(m, m, -m); // v16 = -v1
	//mesh->vVertices.emplace_back(-m, m, m); // v17 = -v2
	//// triangulo 2 cara 4
	//mesh->vVertices.emplace_back(m, m, m); // v18 = -v3
	//mesh->vVertices.emplace_back(m, m, -m); // v19 = v16 = -v4 = -v1
	//mesh->vVertices.emplace_back(-m, m, m); // v20 = v17 = -v5 = -v2
	//// ^^^^ - en y


	//// triangulo 1 cara 5
	//mesh->vVertices.emplace_back(-m, -m, m); // v21 = -v6 = -v3
	//mesh->vVertices.emplace_back(-m, m, m); // v22 = -v7
	//mesh->vVertices.emplace_back(-m, -m, -m); // v23 = 
	//// triangulo 2 cara 5
	//mesh->vVertices.emplace_back(-m, m, -m); // v24 = -v9
	//mesh->vVertices.emplace_back(-m, m, m); // v25 = -v10 = -v7
	//mesh->vVertices.emplace_back(-m, -m, -m); // v26 = -v11 = -v1
	//// ^^^^ - en x

	//// triangulo 1 cara 6
	//mesh->vVertices.emplace_back(m, m, m); // v27 = -v9
	//mesh->vVertices.emplace_back(-m, -m, m); // v28 = -v0
	//mesh->vVertices.emplace_back(-m, m, m); // v29
	//// triangulo 2 cara 6
	//mesh->vVertices.emplace_back(m, m, m); // v30 = v27
	//mesh->vVertices.emplace_back(-m, -m, m); // v31 = v28
	//mesh->vVertices.emplace_back(m, -m, m); // v32 = v3
	//// ^^^^ - en z
#pragma endregion

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
