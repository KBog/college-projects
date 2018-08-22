#ifndef STENCILBUFFERSTATE_H
#define STENCILBUFFERSTATE_H


enum StencilFunction
{
	StencilFunctionNever,
	StencilFunctionLess,
	StencilFunctionEqual,
	StencilFunctionLessOrEqual,
	StencilFunctionGreater,
	StencilFunctionGreaterOrEqual,
	StencilFunctionNotEqual,
	StencilFunctionAlways
};

enum StencilOperation
{
	StencilOperationKeep,
	StencilOperationZero,
	StencilOperationReplace,
	StencilOperationIncr,
	StencilOperationDecr,
	StencilOperationInvert 
};

class StencilBufferState
{
public:
	StencilBufferState(StencilFunction stencilFunction = StencilFunctionAlways,
		StencilOperation  failOperation = StencilOperationKeep,
		StencilOperation zfailOperation = StencilOperationKeep,
		StencilOperation passOperation = StencilOperationKeep,
		int reference = 0,
		unsigned int mask = 0xFFFFFFFF,
		unsigned int writeMask = 0xFFFFFFFF,
		bool status = false)
	{
		StencilFunction = stencilFunction;
		FailOperation = failOperation;
		ZFailOperation = zfailOperation;
		PassOperation = passOperation;
		Reference = reference;
		Mask = mask;
		WriteMask = writeMask;
		Status = status;
	}
	StencilFunction StencilFunction;
	StencilOperation FailOperation;
	StencilOperation ZFailOperation;
	StencilOperation PassOperation;
	int Reference;
	unsigned int Mask;
	unsigned int WriteMask;
	bool Status;
};

#endif