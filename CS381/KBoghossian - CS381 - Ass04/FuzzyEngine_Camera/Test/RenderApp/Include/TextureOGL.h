#ifndef TEXTUREOGL_H
#define TEXTUREOGL_H
#include <windows.h>
#include <iostream>
#include "gl.h"
#include "glprocs.h"
#include <gl/glu.h>

#include "Texture.h"
#include "Surface.h" 

class TextureOGL: public Texture
{
public:
	TextureOGL(const std::string&, bool);
	TextureOGL(const Surface*, bool);
	~TextureOGL();
	int GetWidth();
	int GetHeight();
	GLuint GetID();
	SurfaceType GetSurfaceType();

private:
	void ConstructFromSurface(const Surface*, bool);
	int m_Width;
	int m_Height;
	GLuint m_ID;
	SurfaceType m_SurfaceType;
};



#endif