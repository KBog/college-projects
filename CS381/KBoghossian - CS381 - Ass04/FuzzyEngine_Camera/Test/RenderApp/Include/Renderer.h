#ifndef RENDERER_H
#define RENDERER_H

#include "Material.h" 
#include "Light.h"
#include "LineState.h"
#include "PointState.h"
#include "DitheringState.h"
#include "TriangleOffsetState.h"
#include "StencilBufferState.h"
#include "LightingState.h"
#include "TexturingState.h"
#include "DepthBufferState.h"
#include "ShadingState.h"
#include "FillingState.h"
#include "ScissorState.h"
#include "FogState.h"
#include "CullingState.h"
#include "AlphaTestingState.h"
#include "AlphaBlendingState.h"
#include "NormalizeNormalsState.h"
#include "Color.h"
#include "Texture.h"
#include "Surface.h"
//#include "Matrix3D.h"
#include "Headers.h"
#include <string>


enum Resolution
{
	Resolution320X200,
    Resolution320X240,
	Resolution400X300,
	Resolution480X360,
	Resolution512X384,
	Resolution640X400,
	Resolution640X480,
	Resolution800X600,
	Resolution1024X768,
	Resolution1152X864,
	Resolution1280X1024,
	Resolution1600X1200,
};

enum WindowMode
{
    WindowModeWindowed,
	WindowModeFullScreen
};

enum RendererType
{
	RendererTypeOpenGL,
	RendererTypeDirectX,
	RendererTypeSoftware
};

enum PrimitiveType
{
	PrimitiveTypePointList,
	PrimitiveTypeLineList,
	PrimitiveTypeLineStrip,
	PrimitiveTypeTriangleList,
	PrimitiveTypeTriangleStrip,
	PrimitiveTypeTriangleFan
};


class DisplaySettings
{
public:
	DisplaySettings()
	{
        Resolution = Resolution800X600;
		WindowMode = WindowModeWindowed;
        VSync= true;
    }

	Resolution Resolution;
	WindowMode WindowMode;
	bool VSync;
};

class DisplaySettings2
{
public:
	void * WindowHandle;
	WindowMode WindowMode;
	int Width;
	int Height;
	int RefreshRate;
	int ColorBits;
	int DepthBits;
	int StencilBits;
	bool VSync;
};

class Viewport
{
public:
	unsigned int X;
    unsigned int Y;
    unsigned  int Width; 
    unsigned int Height;
    float ZNear;
    float ZFar;

	Viewport(unsigned int x = 0, unsigned int y = 0, 
		unsigned int width = 640, unsigned int height = 480,
		float znear = 0.0f, float zfar = 1.0f)
	{
		X = x;
		Y = y;
		Width = width; 
		Height = height;
		ZNear = znear;
		ZFar = zfar;
	}
};

class Rectangle
{

};

class Capabilities
{
public:
    Capabilities()
	{
		MultiTexturing = false;
		TextureUnits = 1;
		VSyncControl = false;
	}

	bool MultiTexturing;
	int TextureUnits;
	bool VSyncControl;
};

/*
class Vertex
{
public:
	Vertex(float x = 0, float y = 0, float z = 0):X(x), Y(y), Z(z){}
	float X;
	float Y;
	float Z;
};*/

class UV
{
public:
	UV(float u = 0, float v = 0):U(u), V(v){}
	float U;
	float V;
};

class Renderer
{
public:

	virtual ~Renderer(){};
    
	virtual void SetState(const ScissorState&) = 0;
	virtual void GetState(ScissorState&) = 0;

	virtual void SetState(const AlphaBlendingState&) = 0;
	virtual void GetState(AlphaBlendingState&) = 0;

	virtual void SetState(const AlphaTestingState&) = 0;
	virtual void GetState(AlphaTestingState&) = 0;

	virtual void SetState(const CullingState&) = 0;
	virtual void GetState(CullingState&) = 0;

	virtual void SetState(const FogState&) = 0;
	virtual void GetState(FogState&) = 0;
 
	virtual void SetState(const FillingState&) = 0;
	virtual void GetState(FillingState&) = 0;
 
	virtual void SetState(const ShadingState&) = 0;
	virtual void GetState(ShadingState&) = 0;
	
	virtual void SetState(const DepthBufferState&) = 0;
	virtual void GetState(DepthBufferState&) = 0;
	
	virtual void SetState(const StencilBufferState&) = 0;
	virtual void GetState(StencilBufferState&) = 0;
	
	virtual void SetState(const TriangleOffsetState&) = 0;
	virtual void GetState(TriangleOffsetState&) = 0;
	
	virtual void SetState(const DitheringState&) = 0;
	virtual void GetState(DitheringState&) = 0;
	
	virtual void SetState(const PointState&) = 0;
	virtual void GetState(PointState&) = 0;
	
	virtual void SetState(const LineState&) = 0;
	virtual void GetState(LineState&) = 0;

	virtual void SetState(const TexturingState&) = 0;
	virtual void GetState(TexturingState&) = 0;
	
	virtual void SetState(const LightingState&) = 0;
	virtual void GetState(LightingState&) = 0;

	virtual void SetState(const NormalizeNormalsState&) = 0;
	virtual void GetState(NormalizeNormalsState&) = 0;
	
	virtual void SetMaterial(const Material&) = 0;
	virtual Material GetMaterial() = 0;

	virtual void SetActiveTextureUnit(int) = 0;
	virtual int GetActiveTextureUnit() = 0;

	virtual void SetTexture(Texture*) = 0;
	virtual Texture* GetTexture() = 0;

	virtual void SetLight(int, const Light&) = 0;
	virtual Light GetLight(int) = 0;

	virtual void SetLightStatus(int, bool) = 0;
	virtual  bool GetLightStatus(int) = 0;

	virtual void SetVertexBuffer(Vertex*) = 0;
	virtual void SetNormalBuffer(Point3D*) = 0;
	virtual void SetColorBuffer(Color*) = 0;
	virtual void SetTextureCoordBuffer(UV*) = 0;
	virtual void SetIndexBuffer(unsigned short*, int) = 0;
	virtual void SetPrimitivesType(PrimitiveType) = 0;
	virtual PrimitiveType GetPrimitivesType() = 0;
	virtual void Draw() = 0;

	virtual void BeginRasterGraphics() = 0;
	virtual void EndRasterGraphics() = 0;
	
	virtual void DrawSurface(Surface* surface, int dx, int dy, int sx, int sy, int width, int height, bool hFlip, bool vFlip) = 0;
	virtual void DrawString(int dx, int dy, const std::string& text) = 0;
	


	virtual void SwapBuffers() = 0; 

	virtual void SetViewport(const Viewport&) = 0;
	virtual Viewport GetViewport() = 0;

	virtual void SetWindowMode(WindowMode) = 0;
	virtual WindowMode GetWindowMode() = 0;

	virtual void SetWorldTransformation(const Matrix3D&) = 0;
	virtual Matrix3D GetWorldTransformation() = 0;
	
	virtual void SetViewingTransformation(const Matrix3D&) = 0;
	virtual Matrix3D GetViewingTransformation() = 0;
	
	virtual void SetProjectionTransformation(const Matrix3D&) = 0;
	virtual Matrix3D GetProjectionTransformation() = 0;

	virtual void SetTextureTransformation(int, const Matrix3D&) = 0;
	virtual Matrix3D GetTextureTransformation(int) = 0;
	
	virtual void ClearBackBuffer(const Color& = Color::Black) = 0;
	virtual void ClearDepthBuffer(float) = 0;
	virtual void ClearStencilBuffer(int) = 0;
	virtual void ClearBuffers(const Color&, float, int) = 0;
		
	virtual Capabilities GetCapabilities() = 0; 
	//virtual void SetDisplaySettings(const DisplaySettings&) = 0; 
	//virtual DisplaySettings GetDisplaySettings() = 0;

	virtual void ToggleWindowMode() = 0;
};








#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "winmm.lib")



#endif