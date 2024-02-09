#include "RegularPolygon.h"
using namespace glm;

RegularPolygon::RegularPolygon(GLuint num, GLdouble r, glm::dvec4 color) : Abs_Entity()
{
	mMesh = Mesh::generateRegularPolygon(num, r);
	setmColor(color);
	
}

void RegularPolygon::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		glColor4d(mColor.r, mColor.g, mColor.b, mColor.a);
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
		glColor4d(0, 0, 0, 0);
	}
}
