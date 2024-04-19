#pragma once
#include <vector>
#include <glm/vec3.hpp>

class Cara
{
public:
	Cara() = default;
	Cara(glm::vec3, glm::vec3, glm::vec3);
	~Cara() = default;
	int numeroVertices;
	std::vector<glm::vec3> vertices;

	int getIndiceVertice(glm::vec3 vec) const
	{
		const auto it
			= std::ranges::find(vertices.begin(), 
			                    vertices.end(), 
			                    vec);

		int index = -1;
		if (it != vertices.end())
			index = it - vertices.begin();

		return index;
	}

	glm::vec3 getVerticeIndice(int index) const
	{
		return vertices[index];
	}
};
