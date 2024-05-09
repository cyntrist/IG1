#pragma once

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "Texture.h"

class Material;

class Light
{
public:
	static GLuint cont; // Atributo para poder generar un nuevo id cada vez 
	GLuint id = GL_LIGHT0 + GL_MAX_LIGHTS; // Primer id no válido 

	// Atributos lumínicos y geométrico de la fuente de luz 
	glm::fvec4 ambient = {0.1, 0.1, 0.1, 1};
	glm::fvec4 diffuse = {0.5, 0.5, 0.5, 1};
	glm::fvec4 specular = {0.5, 0.5, 0.5, 1};
	glm::fvec4 posDir = {0, 0, 1, 0};

	// Añade setters para cambiar el valor de los atributos lumínicos public: 
	// GETTERS
	glm::fvec4 getAmbient() const { return ambient; }
	glm::fvec4 getDiffuse() const { return diffuse; }
	glm::fvec4 getSpecular() const { return specular; }
	glm::fvec4 getPosDir() const { return posDir; }

	// SETTERS
	void setAmbient(glm::fvec4 a)
	{
		ambient = a;
		uploadL();
	};

	void setDiffuse(glm::fvec4 d)
	{
		diffuse = d;
		uploadL();
	};

	void setSpecular(glm::fvec4 s)
	{
		specular = s;
		uploadL();
	};

	void setPosDir(glm::fvec4 p)
	{
		posDir = p;
		uploadL();
	};

	void setID(GLuint id_)
	{
		id = id_;
	}


	Light();

	virtual ~Light()
	{
		disable(); // disable???
	}

	void uploadL() const; // Método abstracto 

	virtual void upload(const glm::dmat4& modelViewMat) const = 0;


	void disable()
	{
		if (id < GL_LIGHT0 + GL_MAX_LIGHTS)
			glDisable(id);
	};

	void enable()
	{
		if (id < GL_LIGHT0 + GL_MAX_LIGHTS)
			glEnable(id);
	};
};

class DirLight : public Light
{
public:
	void upload(const glm::dmat4& modelViewMat) const override;
	void setPosDir(glm::fvec3 dir);
};


class PosLight : public Light
{
protected:
	// Factores de atenuación 
	GLfloat kc = 1, kl = 0, kq = 0;

public:
	void upload(const glm::dmat4& modelViewMat) const override;
	void setPosDir(glm::fvec3 dir);
	void setAtte(GLfloat kc_, GLfloat kl_, GLfloat kc2_);
};

class SpotLight : public PosLight
{
protected:
	// Atributos del foco 
	glm::fvec4 direction = {0, 0, -1, 0};
	GLfloat cutoff = 180;
	GLfloat exp = 0;

public:
	SpotLight(glm::fvec3 pos = {0, 0, 0}) : PosLight()
	{
		posDir = glm::fvec4(pos, 1.0);
	};
	void upload(const glm::dmat4& modelViewMat) const override;
	void setSpot(glm::fvec3 dir, GLfloat cf, GLfloat e);
};
