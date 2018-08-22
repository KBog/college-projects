#ifndef DITHERINGSTATE_H
#define DITHERINGSTATE_H


class DitheringState
{
public:
	DitheringState(bool status = true)
	{
		Status = status;
	}

	bool Status;
};


#endif