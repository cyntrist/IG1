#include "Entity.h"
#include "IndexMesh.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Abs_Entity::upload(const dmat4& modelViewMat) const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(modelViewMat)); // transfers modelView matrix to the GPU
}


/// EJES RGB
EjesRGB::EjesRGB(GLdouble l)
	: Abs_Entity()
{
	mMesh = Mesh::createRGBAxes(l);
}

EjesRGB::~EjesRGB()
{
	delete mMesh;
	mMesh = nullptr;
};

void
EjesRGB::render(const dmat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
	}
}


/// REGULAR POLYGON
RegularPolygon::RegularPolygon(GLuint num, GLdouble r, dvec3 trans)
{
	mMesh = Mesh::generateRegularPolygon(num, r);
	translation = trans;
}

RegularPolygon::~RegularPolygon()
{
	delete mMesh;
	mMesh = nullptr;
}

RegularPolygon::RegularPolygon(GLuint num, GLdouble r, dvec4 color, dvec3 trans)
{
	mMesh = Mesh::generateRegularPolygon(num, r);
	translation = trans;
	setmColor(color);
}

void RegularPolygon::render(const dmat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		glColor4d(mColor.r, mColor.g, mColor.b, mColor.a);
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
		glColor4d(255, 255, 255, 255);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}


/// RGB TRIANGLE
RGBTriangle::RGBTriangle(GLdouble l) : radio(0), trans(dvec2(0, 0))
{
	mMesh = Mesh::generateRGBTriangle(l, 0, 0);

}

RGBTriangle::RGBTriangle(GLdouble l, GLdouble x) : radio(x), trans(dvec2(x, 0))
{
	mMesh = Mesh::generateRGBTriangle(l, 0, 0);
	mModelMat = translate(mModelMat, dvec3(radio, 0, 0));
}

RGBTriangle::RGBTriangle(GLdouble l, GLuint x, GLuint y) : radio(x), trans(dvec2(x, y))
{
	mMesh = Mesh::generateRGBTriangle(l, x, y);
}

RGBTriangle::~RGBTriangle()
{
	delete mMesh;
	mMesh = nullptr;
}

void RGBTriangle::render(const dmat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_POINTS);

		dmat4 aMat = modelViewMat
			* rotate(dmat4(1.0), radians(-angle), dvec3(0, 0.0, 1.0)) // rotacion alrededor de la circunferencia en sentido antihorario
			* translate(mModelMat, dvec3(trans.x, trans.y, 0)) // traslacion al radio de la circunferencia
			* rotate(dmat4(1.0), radians(angle * 3), dvec3(0, 0, 1)); // rotaci�n sobre si mismo en sentido horario
		upload(aMat);
		glLineWidth(2);
		mMesh->render();


		glLineWidth(1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void RGBTriangle::update()
{
	angle -= 2.0;
}


/// RGB RECTANGLE
RGBRectangle::RGBRectangle(GLdouble w, GLdouble h)
{
	mMesh = Mesh::generateRGBRectangle(w, h);
}

RGBRectangle::~RGBRectangle()
{
	delete mMesh;
	mMesh = nullptr;
}

void RGBRectangle::render(const dmat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		glPolygonMode(GL_BACK, GL_LINE);
		glPolygonMode(GL_FRONT, GL_FILL);
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}


/// CUBE
Cube::Cube(GLdouble longitud)
{
	mMesh = Mesh::generateCube(longitud);
}

Cube::~Cube()
{
	delete mMesh;
	mMesh = nullptr;
}

void Cube::render(const dmat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_POINT);
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

/// RGBCUBE
RGBCube::RGBCube(GLdouble longitud)
{
	mMesh = Mesh::generateRGBCube(longitud);
	int l = longitud / 2;
	mModelMat = translate(mModelMat, dvec3(l, l, -longitud + l)); // pos inicial
}

RGBCube::~RGBCube()
{
	delete mMesh;
	mMesh = nullptr;
}

void RGBCube::render(const dmat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void RGBCube::update()
{
	mModelMat = rotateAroundCenter(mModelMat, vec3(0, 0, 0), radians(angle), vec3(x, y, z));
	totalRotation += angle;
	if (totalRotation >= 180.0f)
	{
		// esto es TERRIBLE per ofunciona no me mires mal
		if (x == 1)
		{
			x = 0;
			y = 0;
			z = 1;
		}
		else if (y == 1)
		{
			x = 1;
			y = 0;
			x = 0;
		}
		else if (z == 1)
		{
			x = 0;
			y = 1;
			z = 0;
		}

		if (x == 0 && y == 0 && z == 0)
		{
			x = 1;
		}

		totalRotation = 0.0f;
	}
}

mat4 Abs_Entity::rotateAroundCenter(const mat4& matrix, const vec3& center, float angle, const vec3& axis)
{
	const mat4 translateToOriginMatrix = translate(mat4(1.0f), center);
	// matriz de traslacion de vuelta al origen inicial
	const mat4 rotationMatrix = rotate(mat4(1.0f), angle, axis); // matriz de la rotacion en cuestion
	const mat4 translateToCenterMatrix = translate(mat4(1.0f), -center);
	// matriz de traslsacion al centro que queremos????

	// lo que nos explico ruben en clase sobre el orden de multiplicacion de matrices que es inverso
	return translateToOriginMatrix * rotationMatrix
		* translateToCenterMatrix * matrix;
}


/// GROUND
Ground::Ground(GLdouble w, GLdouble h, dvec3 trans)
{
	mMesh = Mesh::generateRectangleTexCor(w, h);
	mModelMat = rotate(mModelMat, radians(-90.0), dvec3(1.0, 0.0, 0.0));
	translation = trans;
}

Ground::Ground(GLdouble w, GLdouble h, std::string t, dvec3 trans) : Ground(w, h, trans)
{
	mTexture = new Texture();
	setTexture(t, mTexture, 255);
}

Ground::Ground(GLdouble w, GLdouble h, GLdouble rw, GLdouble rh, std::string t, dvec3 trans)
{
	mMesh = Mesh::generateRectangleTexCor(w, h, rw, rh);
	translation = trans;
	mModelMat = translate(dmat4(1), translation) *
		rotate(mModelMat, radians(-90.0), dvec3(1.0, 0.0, 0.0));
	mTexture = new Texture();
	setTexture(t, mTexture, 255);
}

Ground::~Ground()
{
	delete mMesh;
	delete mTexture;
	mMesh = nullptr;
	mTexture = nullptr;
}

void Ground::render(const dmat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mTexture->bind(GL_MODULATE); // GL_REPLACE, GL_MODULATE, GL_ADD
		upload(aMat);
		mMesh->render();
		mTexture->unbind();
	}
}


/// BOX OUTLINE
BoxOutline::BoxOutline(GLdouble length, dvec3 trans)
{
	mMesh = Mesh::generateBoxOutline(length);
	translation = trans;
	mModelMat = translate(dmat4(1), dvec3(0.0, 0.0, 0.0)) *
		rotate(mModelMat, radians(-90.0), dvec3(1.0, 0.0, 0.0));
}

BoxOutline::BoxOutline(GLdouble length, std::string t, dvec3 trans)
{
	mMesh = Mesh::generateBoxOutlineTexColor(length);
	translation = trans;
	mModelMat = translate(dmat4(1), dvec3(0.0, 0.0, 0.0)) *
		rotate(mModelMat, radians(-90.0), dvec3(1.0, 0.0, 0.0));
	mTexture = new Texture();
	setTexture(t, mTexture, 255);
}

BoxOutline::BoxOutline(GLdouble length, std::string t, std::string t2, dvec3 trans)
{
	mMesh = Mesh::generateBoxOutlineTexColor(length);
	translation = trans;
	mModelMat = translate(dmat4(1), translation)
		* rotate(mModelMat, radians(-90.0), dvec3(0.0, 1.0, 0.0));

	mTexture = new Texture();
	setTexture(t, mTexture, 255);
	mTexture2 = new Texture();
	setTexture(t2, mTexture2, 255);
}

BoxOutline::~BoxOutline()
{
	delete mMesh;
	delete mTexture;
	delete mTexture2;
	mMesh = nullptr;
	mTexture = nullptr;
	mTexture2 = nullptr;
}

void BoxOutline::render(const dmat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication

		upload(aMat);

		glEnable(GL_CULL_FACE);

		// back
		glPolygonMode(GL_BACK, GL_FILL); // fill
		mTexture2->bind(GL_MODULATE); // GL_REPLACE, GL_MODULATE, GL_ADD
		glCullFace(GL_BACK); // culleo
		mMesh->render(); // render
		mTexture2->unbind(); // unbind

		// front
		glPolygonMode(GL_FRONT, GL_FILL); // fill
		mTexture->bind(GL_MODULATE); // GL_REPLACE, GL_MODULATE, GL_ADD
		glCullFace(GL_FRONT); // culleo
		mMesh->render(); // render
		mTexture->unbind(); // unbind

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);

		mMesh->render();
	}
}


/// BOX
Box::Box(GLdouble length, const std::string& t, const std::string& t2, dvec3 trans) : length(length), angle(0)
{
	mMesh = Mesh::generateBoxOutlineTexColor(length);
	mTopMesh = Mesh::generateRectangleTexCor(length, length);
	mBottomMesh = Mesh::generateRectangleTexCor(length, length);

	translation = trans;
	mTexture = new Texture();
	mTexture2 = new Texture();
	setTexture(t, mTexture, 255);
	setTexture(t2, mTexture2, 255);
}

Box::~Box()
{
	delete mMesh;
	delete mTexture;
	delete mTopMesh;
	delete mBottomMesh;
	delete mTexture2;
	mTexture2 = nullptr;
	mTopMesh = nullptr;
	mBottomMesh = nullptr;
	mTexture = nullptr;
	mMesh = nullptr;
}

void Box::render(const dmat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		/// Inicializacion de culling
		glEnable(GL_CULL_FACE);

		///	Caras traseras
		glPolygonMode(GL_BACK, GL_FILL);
		mTexture2->bind(GL_MODULATE); // GL_REPLACE, GL_MODULATE, GL_ADD
		glCullFace(GL_BACK);
		renderBox(modelViewMat);
		mTexture2->unbind(); // quita la textura del buffer

		/// Caras delanteras
		glPolygonMode(GL_FRONT, GL_FILL);
		mTexture->bind(GL_MODULATE); // GL_REPLACE, GL_MODULATE, GL_ADD
		glCullFace(GL_FRONT);
		renderBox(modelViewMat);
		mTexture->unbind(); // quita la textura del buffer

		/// Reseteo
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // fill
	}
}

void Box::renderTop(const dmat4& modelViewMat) const
{
	const auto aMat = modelViewMat
		* translate(mModelMat, translation); // glm matrix multiplication
	upload(aMat);
	mMesh->render();
}

void Box::renderMain(const dmat4& modelViewMat) const
{
	const auto aMat = modelViewMat
		* translate(mTopMat, translation) // traslacion al punto en la escena
		* translate(mTopMat, dvec3(0, length / 2, 0)) // traslacion final a arriba de la caja
		* translate(mTopMat, dvec3(0.0, 0.0, -length / 2)) // deshacer la traslacion del origen
		* rotate(mBotMat, radians(-angle), dvec3(1.0, 0.0, 0.0)) // rotacion desde un lado de la tapa
		* translate(mTopMat, dvec3(0.0, 0.0, length / 2)) // traslacion para hacer la rotacion con otro origen
		* rotate(mTopMat, radians(-90.0), dvec3(1.0, 0.0, 0.0)); // rotacion inicial para que este tumbada
	upload(aMat);
	mTopMesh->render();
}

void Box::renderBot(const dmat4& modelViewMat) const
{
	const auto aMat = modelViewMat
		* translate(mBotMat, translation)
		* translate(mBotMat, dvec3(0, -length / 2, 0))
		* rotate(mBotMat, radians(90.0), dvec3(1.0, 0.0, 0.0));
	upload(aMat);
	mBottomMesh->render();
}

void Box::renderBox(glm::dmat4 const& modelViewMat) const
{
	renderMain(modelViewMat); // Cuerpo principal
	renderTop(modelViewMat); // Tapa (primero rotate luego translate)
	renderBot(modelViewMat); // Culo (primero rotate luego translate)
}

void Box::update()
{
	if (clockwise)
		angle += speed;
	else
		angle -= speed;

	if (angle <= 0 || angle >= 180.0)
		clockwise = !clockwise;
}


/// STAR 3D
Star3D::Star3D(GLdouble re, GLuint np, GLdouble h, std::string text, dvec3 trans)
{
	mMesh = Mesh::generateStar3DTexCor(re, np, h);
	translation = trans;
	mModelMat = translate(dmat4(1), translation);
	mTexture = new Texture();
	setTexture(text, mTexture, 255);
}

Star3D::~Star3D()
{
	delete mMesh;
	delete mTexture;
	mTexture = nullptr;
	mMesh = nullptr;
}

void Star3D::render(const dmat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		glLineWidth(2);
		mTexture->bind(GL_MODULATE); // GL_REPLACE, GL_MODULATE, GL_ADD
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat); // upload de primera estrella
		mMesh->render(); // renderiza la primera estrella

		// matriz de la segunda estrella
		dmat4 bMat = modelViewMat * mModelMat
			* rotate(dmat4(1), radians(180.0), dvec3(0.0, 1.0, 0.0));
		upload(bMat); // upload de la segunda estrella rotada
		mMesh->render(); // renderiza la segunda estrella

		mTexture->unbind();
		glLineWidth(1);
	}
}

void Star3D::update()
{
	angle += rotationFactor;

	//// rotacion en el eje z
	//mModelMat *= rotate(dmat4(1.0), radians(angle), dvec3(0.0, 1.0, 0.0));
	//// rotacion en el eje y
	//mModelMat *= rotate(dmat4(1.0), radians(angle), dvec3(0.0, 0.0, 1.0));
	//// ambas rotaciones tienen el mismo angulo

	mModelMat = translate(dmat4(1), translation)
		* rotate(dmat4(1), radians(angle), dvec3(0.0, 1.0, 0.0))
		* rotate(dmat4(1), radians(angle), dvec3(0.0, 0.0, 1.0));
}


/// GLASS PARAPET
GlassParapet::GlassParapet(GLdouble width, GLdouble height, std::string t, dvec3 trans)
{
	mMesh = Mesh::generateGlassParapet(width, height); //Mesh::generateBoxOutlineTexColor(width);
	translation = trans;
	mTexture = new Texture();
	setTexture(t, mTexture, 128);
	mModelMat = translate(dmat4(1), translation);
}

GlassParapet::~GlassParapet()
{
	delete mMesh;
	delete mTexture;
	mMesh = nullptr;
	mTexture = nullptr;
}

void GlassParapet::render(const dmat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mTexture->bind(GL_MODULATE); // GL_REPLACE, GL_MODULATE, GL_ADD
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
		mTexture->unbind();
	}
}


/// PHOTO
Photo::Photo(GLdouble w, GLdouble h, dvec3 trans)
{
	pW = w;
	pH = h;
	mMesh = Mesh::generateRectangleTexCor(w, h);
	mModelMat = translate(dmat4(1), trans)
		* rotate(dmat4(1), radians(90.0), dvec3(1.0, 0.0, 0.0));
	mTexture = new Texture();
	mTexture->loadColorBuffer(800.0, 600.0);
}

Photo::~Photo()
{
	delete mMesh;
	delete mTexture;
	mMesh = nullptr;
	mTexture = nullptr;
}

void Photo::render(const dmat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mTexture->bind(GL_REPLACE); // GL_REPLACE, GL_MODULATE, GL_ADD


		upload(aMat);
		glLineWidth(2);
		mMesh->render();


		glLineWidth(1);
		mTexture->unbind();
	}
}

void Photo::update()
{
	// actualiza la textura 
	mTexture->loadColorBuffer(800.0, 600.0);
}


/// GRASS
Grass::Grass(GLdouble w, GLdouble h, std::string t, dvec3 trans, GLint n) : w(w), h(h), nBlades(n)
{
	mMesh = Mesh::generateRectangleTexCor(w, h);
	translation = trans;
	mTexture = new Texture();
	mTexture->load(t, u8vec3{0, 0, 0}, 0); // no renderizar negro
}

Grass::~Grass()
{
	delete mMesh;
	delete mTexture;
	mMesh = nullptr;
	mTexture = nullptr;
}

void Grass::render(const dmat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mTexture->setWrap(GL_REPEAT);
		mTexture->bind(GL_REPLACE); // GL_REPLACE, GL_MODULATE, GL_ADD

		for (int i = 0; i < nBlades; i++)
		{
			dmat4 aMat = modelViewMat
				* translate(mModelMat, translation)
				* translate(mModelMat, dvec3(0, h / 2, 0))
				* rotate(mModelMat, radians(180.0), dvec3(0, 0, 1))
				* rotate(mModelMat, radians(180.0 * i / nBlades), dvec3(0, 1, 0));
			upload(aMat);
			mMesh->render();
		}
		mTexture->unbind();
	}
}


/////////////////////////////////////////////////////
void QuadricEntity::setRGB(GLdouble rv = 1.0, GLdouble gv = 1.0, GLdouble bv = 1.0)
{
	red = rv;
	green = gv;
	blue = bv;
}

Sphere::Sphere(GLdouble rr)
{
	r = rr;


}

void Sphere::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat; 
	upload(aMat); 
	// Aqu� se puede fijar el color de la esfera as�: 
	glEnable(GL_COLOR_MATERIAL); 
	glColor3f(red, green, blue); 
	// Aqu� se puede fijar el modo de dibujar la esfera: 
	gluQuadricDrawStyle(q, GLU_FILL); 
	// GLU_LINE
	// GLU_SILHOUETTE
	// GLU_POINT

	gluSphere(q, r, 50, 50); 
	// Aqu� se debe recuperar el color: 
	glColor3f(1.0, 1.0, 1.0);
}

Cylinder::Cylinder(GLdouble brr, GLdouble trr, GLdouble hh)
{
	br = brr;
	tr = trr;
	h = hh;

	
}

void Cylinder::render(glm::dmat4 const& modelViewMat) const
{

	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	// Aqu� se puede fijar el color de la esfera as�: 
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(red, green, blue); 
	// Aqu� se puede fijar el modo de dibujar la esfera: 
	gluQuadricDrawStyle(q, GLU_FILL);
	// GLU_LINE
	// GLU_SILHOUETTE
	// GLU_POINT

	
	gluCylinder(q, br, tr, h, 50, 50);
	// Aqu� se debe recuperar el color: 
	glColor3f(1.0, 1.0, 1.0);
}

Disk::Disk(GLdouble irr, GLdouble orr)
{
	iR = irr;
	oR = orr;
}

void Disk::render(glm::dmat4 const& modelViewMat) const
{

	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	// Aqu� se puede fijar el color de la esfera as�: 
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(red, green, blue); 
	// Aqu� se puede fijar el modo de dibujar la esfera: 
	gluQuadricDrawStyle(q, GLU_FILL);
	// GLU_LINE
	// GLU_SILHOUETTE
	// GLU_POINT

	gluDisk(q, iR, oR, 50, 50);
	// Aqu� se debe recuperar el color: 
	glColor3f(1.0, 1.0, 1.0);
}

PartialDisk::PartialDisk(GLdouble irr, GLdouble orr, GLdouble sAng, GLdouble swAng)
{
	iR = irr;
	oR = orr;
	sa = sAng;
	swa = swAng;

}

void PartialDisk::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	// Aqu� se puede fijar el color de la esfera as�: 
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(red, green, blue); 
	// Aqu� se puede fijar el modo de dibujar la esfera: 
	gluQuadricDrawStyle(q, GLU_FILL);
	// GLU_LINE
	// GLU_SILHOUETTE
	// GLU_POINT

	gluPartialDisk(q, iR, oR, 50, 50, sa, swa);

	// Aqu� se debe recuperar el color: 
	glColor3f(1.0, 1.0, 1.0);
}

CompoundEntity::CompoundEntity()
{


}

CompoundEntity::~CompoundEntity()
{
	for (auto& ae : gObjects) {

		ae->~Abs_Entity(); // ????
	}
}

void CompoundEntity::addEntity(Abs_Entity* ae)
{
	gObjects.push_back(ae);
}

void CompoundEntity::render(glm::dmat4 const& modelViewMat) const
{
	for (auto& ae : gObjects) {

		ae->render(modelViewMat);
	}
}

IndexedBox::IndexedBox(GLdouble l)
{
	mMesh = IndexMesh::generateIndexedBox(l);
}

IndexedBox::~IndexedBox()
{
	delete mMesh;
	mMesh = nullptr;
}

void IndexedBox::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat);

		mMesh->render();
	}
}

void IndexedBox::update()
{
	Abs_Entity::update();
}


// -------------- TIE FIGHTER

AdvancedTIE::AdvancedTIE()
{
	// ./bmps/noche.bmp
	// genera las partes por separado
	leftWing = new WingAdvancedTIE(30, 40, 90, "./bmps/noche.bmp");
	rightWing = new WingAdvancedTIE(30, -40, -90, "./bmps/noche.bmp"); // FALTA
	base = new Sphere(20);
	base->setModelMat(
		translate(dmat4(1.0), dvec3(0, 30, 0))
		* base->modelMat()
	);

	
	morro = new BaseAdvancedTIE();
	morro->setModelMat(
		translate(dmat4(1.0), dvec3(0, -100, 0))
		* morro->modelMat()
	);

	// CYL WIP
	cyl = new Cylinder(3, 3, 10);
	cyl->setModelMat(
		translate(dmat4(1.0), dvec3(-40, 27, 0))
		* rotate(dmat4(1.0), radians(90.0), dvec3(0.0, 1.0, 0.0))
		* scale(dmat4(1.0), dvec3(1, 1, 8))
		* cyl->modelMat()
	);

	

	// a�ade las entidades al vector de entidades del compound entity
	CompoundEntity::addEntity(leftWing);
	CompoundEntity::addEntity(rightWing);
	CompoundEntity::addEntity(base);
	CompoundEntity::addEntity(morro);
	CompoundEntity::addEntity(cyl);
}

AdvancedTIE::~AdvancedTIE()
{
	CompoundEntity::~CompoundEntity();
}

void AdvancedTIE::render(glm::dmat4 const& modelViewMat) const
{
	CompoundEntity::render(modelViewMat);
}

// -------------- ALA DEL TIE

WingAdvancedTIE::WingAdvancedTIE(GLdouble x, GLdouble y, GLdouble rot, const std::string& t)
{
	mMesh = Mesh::generateTIEWing(60, 20, 20);
	
	// hay que rotar el ala porque se genera apoyada en el plano xy 
	mModelMat = rotate(mModelMat, radians(-90.0), dvec3(0.0, 0.0, 1.0)) *
				translate(mModelMat, dvec3(-x, y, 0)) *
				rotate(mModelMat, radians(rot), dvec3(1.0, 0.0, 0.0));

	mTexture = new Texture();
	setTexture(t, mTexture, 255);
}

WingAdvancedTIE::~WingAdvancedTIE()
{
	//
}

void WingAdvancedTIE::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		glPolygonMode(GL_BACK, GL_FILL);
		glPolygonMode(GL_FRONT, GL_FILL);
		mTexture->bind(GL_MODULATE);
		
		
		upload(aMat);
		glLineWidth(2);
		mMesh->render();


		glLineWidth(1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mTexture->unbind();

	}

}

// ----------------- MORRO DEL TIE

BaseAdvancedTIE::BaseAdvancedTIE()
{
	cyl = new Cylinder(3, 3, 10);
	cyl->setModelMat(
		translate(dmat4(1.0), dvec3(0, 30, 25))
		* rotate(dmat4(1.0), radians(180.0), dvec3(1.0, 0.0, 0.0))
		* scale(dmat4(1.0), dvec3(3, 3, 1))
		* cyl->modelMat()
	);


	disk = new Disk(1, 3);
	disk->setModelMat(
		translate(dmat4(1.0), dvec3(-0, 30, 25))
		* rotate(dmat4(1.0), radians(180.0), dvec3(0.0, 1.0, 0.0))
		* scale(dmat4(1.0), dvec3(4, 4, 1))
		* disk->modelMat()
	);

	CompoundEntity::addEntity(cyl);
	CompoundEntity::addEntity(disk);
}

BaseAdvancedTIE::~BaseAdvancedTIE()
{
	CompoundEntity::~CompoundEntity();
}

void BaseAdvancedTIE::render(glm::dmat4 const& modelViewMat) const
{
	CompoundEntity::render(modelViewMat);
}
