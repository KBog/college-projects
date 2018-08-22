#ifndef TEST_H
#define TEST_H

#include "3dsfile.h"

class Test
{
public:
	Test(void);
	~Test(void);

	void Update();
	void Render();

private:
	_3DSLoader loader;
};


#endif
