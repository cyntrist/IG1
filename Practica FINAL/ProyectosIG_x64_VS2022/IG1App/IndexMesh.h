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
	~IndexMesh() override { delete [] vIndices; }
	void render() const override;
	void draw() const override;
	//...

	glm::vec3 calculoVectorNormalPorNewell(Cara c);
	void buildNormalVectors(); //obsoleto
	void buildNormalVectorsV2();
	static IndexMesh* generateIndexedBox(GLdouble l); //obsoleto
	static IndexMesh* generateIndexedBoxV2(GLdouble l);
};

class MbR : public IndexMesh
{
	GLint mProfilePoints; // de abajo a arriba, sentido antihorario
	GLint mRotationNumber;
	glm::dvec3* mProfile;

public:
	MbR(GLint, GLint, glm::dvec3*);
	~MbR() override = default;
	static MbR* generateIndexMbR(GLint, GLint, glm::dvec3*);
	void render() const override;
	void draw() const override;
};
