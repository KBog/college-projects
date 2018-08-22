#ifndef TESTRENDERAPP_H
#define TESTRENDERAPP_H

#include <Application.h>

class Test;

class MyApplication: public Application
{
public:

	void OnSetInitialDisplaySettings(DisplaySettings& displaySettings);
	void OnPreUpdate();
	void OnPostUpdate();
	void OnInit();
	void OnExit();
	void OnUpdate();	


private:

    Test * t;
    
};

extern MyApplication app;

#endif