#ifndef LIGHTINGSTATE_H
#define LIGHTINGSTATE_H

class LightingState
{
public:

	LightingState(Color ambientLight = Color(0.2f, 0.2f, 0.2f, 1.0f), bool localViewer = true, bool status = false)
	{
		AmbientLight = ambientLight;
		LocalViewer = localViewer;
		Status = status;
	}

	Color AmbientLight;
	bool LocalViewer;
	bool Status;
};



#endif