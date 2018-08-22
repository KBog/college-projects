#ifndef SURFACE_H
#define SURFACE_H

#include <iostream>

enum SurfaceType
{
    SurfaceTypeR8G8B8A8,
	SurfaceTypeR8G8B8,
	SurfaceTypeR5G5B5A1,
	SurfaceTypeR5G6B5
	//...Alpha only ...
};

class Surface
{
public:
	Surface(const std::string&);
	Surface(int width = 1, int height = 1, SurfaceType surfaceType = SurfaceTypeR8G8B8A8);
	~Surface();

	int GetWidth() const;
	int GetHeight() const;
	int GetPitch() const;
	SurfaceType GetSurfaceType() const;
	void * GetBuffer() const;

private:
	SurfaceType m_SurfaceType;
	int m_Width;
	int m_Height;
	int m_Pitch;
	void * m_Buffer;

	void ConstructSurface(int width, int height, SurfaceType surfaceType);
};









#endif