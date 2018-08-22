#ifndef FOGSTATE_H
#define FOGSTATE_H

enum FogMode
{
    FogModeLinear,
	FogModeExp,
	FogModeExp2
};

class FogState
{
public:

	FogState(FogMode fogMode = FogModeLinear, Color color = Color::White, float start = 0.0f, float end = 100.0f, 
		float density = 0.0f, bool status = false)
	{
        FogMode = fogMode;
		Color = color;
		Start = start;
		End = end; 
		Density = density;
		Status = false; 
	}

	FogMode FogMode;
	Color Color;
	float Start;
	float End; 
	float Density;
	bool Status; 
};



#endif
