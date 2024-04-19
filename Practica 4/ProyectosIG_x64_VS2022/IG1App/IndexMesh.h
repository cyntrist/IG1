#pragma once
#include "Mesh.h"
class Cara;
class IndexMesh : public Mesh
{
protected :
	GLuint* vIndices = nullptr; // tabla de índices
	GLuint nNumIndices = 0;
	//...

public:
	IndexMesh() { mPrimitive = GL_TRIANGLES; }
	~ IndexMesh() override { delete [] vIndices; }
	void render() const override;
	void draw() const override;
	//...

	glm::vec3 calculoVectorNormalPorNewell();
	void buildNormalVectors();
	static IndexMesh* generateIndexedBox(GLdouble l);
};
