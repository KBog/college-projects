#ifndef CULLINGSTATE_H
#define CULLINGSTATE_H

enum CullFace
{
    CullFaceBack,
	CullFaceFront
};

class CullingState
{
public:

	CullingState(CullFace cullFace = CullFaceBack, bool status = true)
	{
        CullFace = cullFace;
		Status = status;
	}

    CullFace CullFace;
    bool Status;
};

#endif