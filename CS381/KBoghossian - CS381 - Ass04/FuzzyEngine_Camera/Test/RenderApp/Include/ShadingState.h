#ifndef SHADINGSTATE_H
#define SHADINGSTATE_H


enum ShadingModel
{
	ShadingModelFlat,
	ShadingModelGouraud,
};

class ShadingState
{
public:
    
	ShadingState(ShadingModel shadingModel = ShadingModelGouraud)
	{
        ShadingModel = shadingModel;         
	}
	ShadingModel  ShadingModel;
};

#endif