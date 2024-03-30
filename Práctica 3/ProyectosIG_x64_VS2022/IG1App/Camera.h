#ifndef _H_Camera_H_
#define _H_Camera_H_

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Viewport.h"

class Camera
{
public:
	explicit Camera(Viewport* vp);
	~Camera() = default;

	// viewPort
	const Viewport& viewPort() const { return *mViewPort; };

	// view matrix
	const glm::dmat4& viewMat() const { return mViewMat; };

	void set2D();
	void set3D();

	void pitch(GLdouble a); // rotates a degrees on the X axis
	void yaw(GLdouble a); // rotates a degrees on the Y axis
	void roll(GLdouble a); // rotates a degrees on the Z axis

	void pitchReal(GLdouble cs);
	void yawReal(GLdouble cs);
	void rollReal(GLdouble cs);

	// projection matrix
	const glm::dmat4& projMat() const { return mProjMat; };

	// sets scene visible area size
	void setSize(GLdouble xw, GLdouble yh);
	// updates the scale factor
	void setScale(GLdouble s);
	//
	void setRadius(GLdouble r);

	// transfers its viewport, the view matrix and projection matrix to the GPU
	void upload() const
	{
		mViewPort->upload();
		uploadVM();
		uploadPM();
	};

	/// Practica 3
	void changePrj();
	void moveLR(GLdouble cs); // A izquierda/A derecha
	void moveFB(GLdouble cs); // Adelante/Atras
	void moveUD(GLdouble cs); // Arriba/Abajo
	void orbit(GLdouble incAng, GLdouble incY); 

protected:
	glm::dvec3 mEye = {0.0, 0.0, 500.0}; // camera's position
	glm::dvec3 mLook = {0.0, 0.0, 0.0}; // target's position
	glm::dvec3 mUp = {0.0, 1.0, 0.0}; // the up vector

	// marco de coordenadas
	glm::dvec3 mRight; // right = u
	glm::dvec3 mUpward; // upward = v
	glm::dvec3 mFront; // front = -n

	GLdouble mRadio = 0, mAng = 0;

	glm::dmat4 mViewMat; // view matrix = inverse of modeling matrix
	void uploadVM() const; // transfers viewMat to the GPU

	glm::dmat4 mProjMat; // projection matrix
	void uploadPM() const; // transfers projMat to the GPU

	GLdouble xRight, xLeft, yTop, yBot; // size of scene visible area
	GLdouble mNearVal = 1, mFarVal = 10000; // view volume
	GLdouble mScaleFact = 1; // scale factor
	bool bOrto = true; // orthogonal or perspective projection

	Viewport* mViewPort; // the viewport

	void setVM();
	void setPM();

	/// practica 3
	void setAxes();
	void setCenital();
};

#endif //_H_Camera_H_
