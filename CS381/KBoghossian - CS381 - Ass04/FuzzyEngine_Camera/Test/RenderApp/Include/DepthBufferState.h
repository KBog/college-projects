#ifndef DEPTHBUFFERSTATE_H
#define DEPTHBUFFERSTATE_H

enum DepthFunction
{
	DepthFunctionNever,
	DepthFunctionLess,
	DepthFunctionEqual,
	DepthFunctionLessOrEqual,
	DepthFunctionGreater,
	DepthFunctionGreaterOrEqual,
	DepthFunctionNotEqual,
	DepthFunctionAlways
};

class DepthBufferState
{
public:

	DepthBufferState(DepthFunction depthFunction = DepthFunctionLess, bool status = true, bool writeStatus = true)
	{
		DepthFunction = depthFunction;
		Status = status;
		WriteStatus = writeStatus;
	}

	DepthFunction DepthFunction;
	bool Status;
	bool WriteStatus;
};



#endif