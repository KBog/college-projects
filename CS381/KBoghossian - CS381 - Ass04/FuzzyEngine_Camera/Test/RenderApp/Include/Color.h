#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
	float R,G,B,A;

	Color(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f)
	{
		R = red;
		G = green;
		B = blue;
		A = alpha;
	}
    
    static const Color Black;
	static const Color White;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
};


#endif