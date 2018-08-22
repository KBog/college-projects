#ifndef MOUSE_H
#define MOUSE_H

enum MouseButton
{
	MouseButtonLeft,
	MouseButtonMiddle,
	MouseButtonRight
};

class Mouse
{
public:
    static bool IsPressed(MouseButton);
	static bool IsTriggered(MouseButton);
	static void GetPosition(int* x, int* y);
	static void Update();
};

#endif
