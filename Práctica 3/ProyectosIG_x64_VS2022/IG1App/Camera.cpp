#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include <glm/gtc/matrix_access.hpp>

using namespace glm;

Camera::Camera(Viewport* vp)
	: mViewMat(1.0)
	  , mProjMat(1.0)
	  , xRight(vp->width() / 2.0)
	  , xLeft(-xRight)
	  , yTop(vp->height() / 2.0)
	  , yBot(-yTop)
	  , mViewPort(vp)
{
	setPM();
}

void
Camera::uploadVM() const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(mViewMat)); // transfers view matrix to the GPU
}

void
Camera::setVM()
{
	mViewMat = lookAt(mEye, mLook, mUp); // glm::lookAt defines the view matrix
	setAxes();
}

void
Camera::set2D()
{
	mEye = dvec3(0, 0, 500);
	mLook = dvec3(0, 0, 0);
	mUp = dvec3(0, 1, 0);
	setVM();
}

void
Camera::set3D()
{
	mEye = dvec3(500, 500, 500);
	mLook = dvec3(0, 10, 0);
	mUp = dvec3(0, 1, 0);
	setVM();
}

void
Camera::pitch(GLdouble a)
{
	mViewMat = rotate(mViewMat, radians(a), dvec3(1.0, 0, 0));
	// glm::rotate returns mViewMat * rotationMatrix
}

void
Camera::yaw(GLdouble a)
{
	mViewMat = rotate(mViewMat, radians(a), dvec3(0, 1.0, 0));
	// glm::rotate returns mViewMat * rotationMatrix
}

void
Camera::roll(GLdouble a)
{
	mViewMat = rotate(mViewMat, radians(a), dvec3(0, 0, 1.0));
	// glm::rotate returns mViewMat * rotationMatrix
}

void Camera::pitchReal(GLdouble cs)
{
}

void Camera::yawReal(GLdouble cs)
{
}

void Camera::rollReal(GLdouble cs)
{
}

void
Camera::setSize(GLdouble xw, GLdouble yh)
{
	xRight = xw / 2.0;
	xLeft = -xRight;
	yTop = yh / 2.0;
	yBot = -yTop;
	setPM();
}

void
Camera::setScale(GLdouble s)
{
	mScaleFact -= s;
	if (mScaleFact < 0)
		mScaleFact = 0.01;
	setPM();
}

void Camera::setRadius(GLdouble r)
{
	mRadio = r;
}

void
Camera::setPM()
{
	if (bOrto)
	{
		//  if orthogonal projection
		mProjMat = ortho(xLeft * mScaleFact,
		                 xRight * mScaleFact,
		                 yBot * mScaleFact,
		                 yTop * mScaleFact,
		                 mNearVal,
		                 mFarVal);
		// glm::ortho defines the orthogonal projection matrix
	}
	else
	{
		mProjMat = frustum (xLeft * mScaleFact, 
			xRight * mScaleFact, 
			yBot * mScaleFact, 
			yTop * mScaleFact , 
			mNearVal * 400.0 , 
			mFarVal);
	}
}

void Camera::setAxes()
{
	mRight = row(mViewMat, 0);
	mUpward = row(mViewMat, 1);
	mFront = -row(mViewMat, 2);
}

dvec3 Camera::row(dmat4 matrix, int index)
{
	return matrix[index];
}

void Camera::moveLR(GLdouble cs)
{
	mEye += mRight * cs;
	mLook += mRight * cs;
	setVM();
}

void Camera::moveFB(GLdouble cs)
{
	mEye += mFront * cs;
	mLook += mFront * cs;
	setVM();
}

void Camera::moveUD(GLdouble cs)
{
	mEye += mUpward * cs;
	mLook += mUpward * cs;
	setVM();
}

void Camera::orbit(GLdouble incAng, GLdouble incY)
{
	// x = Cx + R*cos(alpha)
	// y = Cy + R*sen(alpha)
	mAng += incAng;
	mEye.x = mLook.x + cos(radians(mAng)) * mRadio;
	mEye.z = mLook.z - sin(radians(mAng)) * mRadio;
	mEye.y += incAng;
	setVM();

}

void Camera::changePrj()
{
	bOrto = !bOrto;
	setPM();
}

void
Camera::uploadPM() const
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(value_ptr(mProjMat)); // transfers projection matrix to the GPU
	glMatrixMode(GL_MODELVIEW);
}
