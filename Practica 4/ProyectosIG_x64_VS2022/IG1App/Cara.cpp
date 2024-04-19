#include "Cara.h"

Cara::Cara(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
	numeroVertices = 3;

	vertices.resize(3);
	vertices[0] = v0;
	vertices[1] = v1;
	vertices[2] = v2;
}
