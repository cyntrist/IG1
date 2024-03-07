#include "Texture.h"
#include "Mesh.h"

//-------------------------------------------------------------------------

Texture::~Texture() 
{
	if(mId!=0)
    glDeleteTextures(1, &mId);
}
//-------------------------------------------------------------------------

void Texture::init()
{
  glGenTextures(1, &mId);
  glBindTexture(GL_TEXTURE_2D, mId);
 
  //Filters and clamping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);     // GL_CLAMP
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);     // GL_CLAMP
   
}
//-------------------------------------------------------------------------

void Texture::bind(GLuint mixMode) // GL_REPLACE, GL_MODULATE, GL_ADD
{
  glBindTexture(GL_TEXTURE_2D, mId);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mixMode);  
}
//-------------------------------------------------------------------------

void Texture::load(const std::string & BMP_Name, GLubyte alpha)
{
	if (mId == 0) init();
	
	PixMap32RGBA pixMap;
  
    pixMap.load_bmp24BGR(BMP_Name);

    if (alpha != 255)
       pixMap.set_alpha(alpha);

	mWidth = pixMap.width();
	mHeight = pixMap.height();

    GLint level = 0;   //Base image level
	GLint border = 0;  //No border
	
	glBindTexture(GL_TEXTURE_2D, mId);
    glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, mWidth, mHeight, border, GL_RGBA, GL_UNSIGNED_BYTE, pixMap.data());
		
    glBindTexture(GL_TEXTURE_2D, 0); 
}
//-------------------------------------------------------------------------

void Texture::setWrap(GLuint wp) // GL_REPEAT, GL_CLAMP
{
  glBindTexture(GL_TEXTURE_2D, mId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wp);  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wp);  
  glBindTexture(GL_TEXTURE_2D, 0); 
}

void Texture::loadColorBuffer(GLsizei width, GLsizei height, GLuint buffer) //=GL_FRONT
{
	// si buffer es un buffer valido; aka si es FRONT o BACK, se sigue, si no pasa
	if (buffer == GL_FRONT || buffer == GL_BACK) {

		//glGenTextures(1, &mId);				// se genera 1 textura y se guarda su nombre en mId
		glBindTexture(GL_TEXTURE_2D, mId);	// se activa/crea (si no esta creada) la textura a 2D 
		// no hace falta porque se llama en bind

		init();

		mWidth = width;
		mHeight = height;

		// llama al bind, que llama a glBindTexture(GL_TEXTURE_2D, mixMode) y a glTextEnvi(...)
		// el primero activa la textura y el segundo settea el modo (REPLACE, MODULATE O ADD)
		bind(GL_REPLACE);

		// lee el buffere especifico que quiere
		glReadBuffer(buffer);

		// copia en la textura activa de la imagen del Color buffer
		// glCopyTexImage2D(GL_TEXTURE_2D, level, format, xLeft, yBottom, width, height, border);
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, width, height, 0);

		// devuelve el buffer al defecto (GL_BACK)
		glReadBuffer(GL_BACK);

		// desactiva la textura
		unbind();
	}


}
//-------------------------------------------------------------------------