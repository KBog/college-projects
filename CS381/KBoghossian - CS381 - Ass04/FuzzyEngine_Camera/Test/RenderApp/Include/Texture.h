#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Surface;
enum SurfaceType;
class Texture
{
public:
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual SurfaceType GetSurfaceType() = 0;

	static Texture* Construct(const std::string&, bool);
	static Texture* Construct(const Surface*, bool);
	static void Destruct(Texture*);
};


#endif