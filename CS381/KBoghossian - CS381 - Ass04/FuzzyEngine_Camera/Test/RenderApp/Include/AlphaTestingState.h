#ifndef ALPHATESTINGSTATE_H
#define ALPHATESTINGSTATE_H

enum AlphaFunction
{
	AlphaFunctionNever,
	AlphaFunctionLess,
	AlphaFunctionEqual,
	AlphaFunctionLessOrEqual,
	AlphaFunctionGreater,
	AlphaFunctionGreaterOrEqual,
	AlphaFunctionNotEqual,
	AlphaFunctionAlways
};

class AlphaTestingState
{
public:

	AlphaTestingState(AlphaFunction alphaFunction = AlphaFunctionAlways, float reference = 0, bool status = false)
	{
        AlphaFunction = alphaFunction;
		Reference = reference;
		Status = status;
	}
	
	AlphaFunction AlphaFunction;
	float Reference;
	bool Status;
};


#endif