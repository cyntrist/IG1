#pragma once
#include "Mesh.h"
#include "Cara.h"
class IndexMesh : public Mesh
{
protected :
	GLuint* vIndices = nullptr; // tabla de índices
	GLuint nNumIndices = 0;
	std::vector<Cara> vCaras;

public:
	IndexMesh() : Mesh() { mPrimitive = GL_TRIANGLES; }
	~ IndexMesh() override { delete [] vIndices; }
	void render() const override;
	void draw() const override;
	//...

	glm::vec3 calculoVectorNormalPorNewell(Cara c);
	void buildNormalVectors();
	static IndexMesh* generateIndexedBox(GLdouble l);
};
