#ifndef TEXTURINGSTATE_H
#define TEXTURINGSTATE_H

enum TextureFunction
{
	TextureFunctionReplace,
	TextureFunctionDecal,
	TextureFunctionModulate,
	TextureFunctionBlend
};

enum FilteringMode
{
	FilteringModeNearest,
	FilteringModeLinear
};

enum WrappingMode
{
	WrappingModeRepeat,
	WrappingModeClamp
};

enum EnvironmentMapType
{
	EnvironmentMapTypeNone,
	EnvironmentMapTypeSphere
};

class TexturingState
{
public:
	TexturingState(TextureFunction textureFunction = TextureFunctionDecal,
		FilteringMode filteringMode = FilteringModeNearest, WrappingMode wrappingModeU = WrappingModeRepeat,
		WrappingMode wrappingModeV = WrappingModeRepeat, EnvironmentMapType environmentMapType = EnvironmentMapTypeNone,
		bool status = false)
	{
        TextureFunction = textureFunction;
		FilteringMode = filteringMode;	  
		WrappingModeU = wrappingModeU;
		WrappingModeV = wrappingModeV;
		EnvironmentMapType = environmentMapType;
		Status = status;
	}

	TextureFunction TextureFunction;
	FilteringMode FilteringMode;	
	WrappingMode WrappingModeU;
	WrappingMode WrappingModeV;
	EnvironmentMapType EnvironmentMapType;
	bool Status;
};


#endif