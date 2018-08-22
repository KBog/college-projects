#ifndef FILLINGSTATE_H
#define FILLINGSTATE_H


enum FillMode
{
    FillModePoint,
	FillModeWireframe,
	FillModeSolid
};


class FillingState
{
public:
	FillingState(FillMode fillMode = FillModeSolid)
	{
		FillMode = fillMode;
	}

    FillMode FillMode;    
};

#endif