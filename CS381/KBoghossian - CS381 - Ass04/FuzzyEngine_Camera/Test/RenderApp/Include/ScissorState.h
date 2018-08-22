#ifndef SCISSORSTATE_H
#define SCISSORSTATE_H

class ScissorState
{
public:
	ScissorState(int x = 0, int y = 0, int width = 640, int height = 480, bool status = false)
	{
        X = x;
		Y = y;
		Width = width;
		Height = height;
		Status = status;
	}
    
	int X;
	int Y;
	int Width;
	int Height;

	bool Status;
};


#endif
