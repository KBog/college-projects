#ifndef OGLRENDERER_H
#define OGLRENDERER_H

#include <windows.h>
#include "gl.h"
#include "glprocs.h"
#include <gl/glu.h>
#include "Renderer.h"

class OGLRenderer: public Renderer
{
public:

	OGLRenderer(const DisplaySettings2&);
	~OGLRenderer();
            
	void SetState(const ScissorState&);
	void GetState(ScissorState&);

	void SetState(const AlphaBlendingState&);
	void GetState(AlphaBlendingState&);

	void SetState(const AlphaTestingState&);
	void GetState(AlphaTestingState&);

	void SetState(const CullingState&);
	void GetState(CullingState&);

	void SetState(const FogState&);
	void GetState(FogState&);
 
	void SetState(const FillingState&);
	void GetState(FillingState&);
 
	void SetState(const ShadingState&);
	void GetState(ShadingState&); 

	void SetState(const DepthBufferState&);
	void GetState(DepthBufferState&);
	
	void SetState(const StencilBufferState&);
	void GetState(StencilBufferState&);
	
	void SetState(const TriangleOffsetState&);
	void GetState(TriangleOffsetState&);
	
	void SetState(const DitheringState&);
	void GetState(DitheringState&);
	
	void SetState(const PointState&);
	void GetState(PointState&);
	
	void SetState(const LineState&);
	void GetState(LineState&);

	void SetState(const TexturingState&);
	void GetState(TexturingState&);
	
	void SetState(const LightingState&);
	void GetState(LightingState&);

	void SetState(const NormalizeNormalsState&);
	void GetState(NormalizeNormalsState&);
	
	void SetMaterial(const Material&);
	Material GetMaterial();

	void SetActiveTextureUnit(int);
	int GetActiveTextureUnit();

	void SetTexture(Texture*);
	Texture* GetTexture();

	void SetLight(int, const Light&);
	Light GetLight(int);

	void SetLightStatus(int, bool);
	bool GetLightStatus(int);

	void SetVertexBuffer(Vertex*);
	void SetNormalBuffer(Point3D*);
	void SetColorBuffer(Color*);
	void SetTextureCoordBuffer(UV*);
	void SetIndexBuffer(unsigned short*, int);
	void SetPrimitivesType(PrimitiveType);
	PrimitiveType GetPrimitivesType();
	void Draw();

	
	void BeginRasterGraphics();
	void EndRasterGraphics();
	
	void DrawSurface(Surface* surface, int dx, int dy, int sx, int sy, int width, int height, bool hFlip, bool vFlip);
	void DrawString(int dx, int dy, const std::string& text);

	void SwapBuffers();

	void SetViewport(const Viewport&);
	Viewport GetViewport();

	void SetWindowMode(WindowMode);
	WindowMode GetWindowMode();

	void SetWorldTransformation(const Matrix3D&);
	Matrix3D GetWorldTransformation();
	
	void SetViewingTransformation(const Matrix3D&);
	Matrix3D GetViewingTransformation();
	
	void SetProjectionTransformation(const Matrix3D&);
	Matrix3D GetProjectionTransformation();

	void SetTextureTransformation(int, const Matrix3D&);
	Matrix3D GetTextureTransformation(int);
	
	void ClearBackBuffer(const Color& = Color::Black);
	void ClearDepthBuffer(float);
	void ClearStencilBuffer(int);
	void ClearBuffers(const Color&, float, int);
	
	Capabilities GetCapabilities();
	//void SetDisplaySettings(const DisplaySettings&);
	//DisplaySettings GetDisplaySettings();

	void ToggleWindowMode();

private:

	void DrawPointList();
	void DrawLineList();
	void DrawLineStrip();
	void DrawTriangleList();
	void DrawTriangleStrip();
	void DrawTriangleFan();
	void SetModelViewTransformation();
	void FillCapabilities();

	void ConstructDefaultFont();
	void DestructDefaultFont();
    
	DisplaySettings2 m_DisplaySettings;
	Capabilities m_Capabilities;
	HDC m_DC;
	HGLRC m_GLRC;
	Texture* m_CurrentTexture;
	Matrix3D m_WorldMatrix;
	Matrix3D m_ViewingMatrix;
	Matrix3D m_ProjectionMatrix;
	Matrix3D m_TextureMatrix;
	Viewport m_Viewport;
    
	int m_ActiveTextureUnit;
	GLuint m_DefaultFont;

	Vertex* m_VertexBuffer;
	Color* m_ColorBuffer;
	UV* m_UVBuffer;
	Point3D* m_NormalBuffer;
	unsigned short* m_IndexBuffer;
	int m_ElementCount;
	PrimitiveType m_PrimitiveType;
};





#endif