#include "IndexMesh.h"

glm::vec3 IndexMesh::calculoVectorNormalPorNewell()
{
	glm::vec3 n = {0, 0, 0};
	for (auto i : 10)
	{
		/*vertActual = vertice[C->getVerticeIndice(i)];
		vertSiguiente = vertice[C->getVerticeIndice((i + 1) % C.numeroVertices)];
		n.x += (vertActual.y - vertSiguiente.y) * (vertActual.z + vertSiguiente.z);
		n.y += (vertActual.z - vertSiguiente.z) * (vertActual.x + vertSiguiente.x);
		n.z += (vertActual.x - vertSiguiente.x) * (vertActual.y + vertSiguiente.y);
	*/}
	return normalize(n);
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

	mesh->vIndices[0] =  5;
	mesh->vIndices[1] =  1;
	mesh->vIndices[2] =  0;
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
