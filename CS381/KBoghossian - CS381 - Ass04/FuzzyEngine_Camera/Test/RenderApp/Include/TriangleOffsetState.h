#ifndef TRIANGLEOFFSETSTATE_H
#define TRIANGLEOFFSETSTATE_H

class TriangleOffsetState
{
public:
    TriangleOffsetState(float factor = 0, float units = 0, bool status = false)
	{
        Factor = factor;
		Units = units;
		Status = status;
	}
	float Factor;
	float Units;
	bool Status;
};



#endif