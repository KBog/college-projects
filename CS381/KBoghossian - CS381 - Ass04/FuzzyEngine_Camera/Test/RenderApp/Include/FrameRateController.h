#ifndef FRAMERATECONTROLLER_H
#define FRAMERATECONTROLLER_H

#include <string>

class FrameRateController
{
public:
    static void Start();
	static void End();
	static void SetFrameRate(int);
	static int GetFrameRate();
	static float GetCurrentFrameRate();
	static const std::string& GetFrameRateAsString();
};

#endif