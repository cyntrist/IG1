#include "RegularPolygon.h"
using namespace glm;

RegularPolygon::RegularPolygon(GLuint num, GLdouble r) : Abs_Entity()
{
	mMesh = Mesh::generateRegularPolygon(num, r);
}

void RegularPolygon::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
	}
}
