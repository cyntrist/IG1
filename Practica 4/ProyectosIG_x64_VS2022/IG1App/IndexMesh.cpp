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

void IndexMesh::render() const
{
	// Comandos OpenGL para enviar datos de arrays a GPU
	// Nuevos comandos para la tabla de índices
	if (vIndices != nullptr)
	{
		glEnableClientState(GL_INDEX_ARRAY);
		glIndexPointer(GL_UNSIGNED_INT, 0, vIndices);
	}
	// Comandos OpenGL para deshabilitar datos enviados
	// Nuevo comando para la tabla de índices :
	glDisableClientState(GL_INDEX_ARRAY);
}

void IndexMesh::draw() const
{
	glDrawElements(mPrimitive, nNumIndices,
	               GL_UNSIGNED_INT, vIndices);
}
