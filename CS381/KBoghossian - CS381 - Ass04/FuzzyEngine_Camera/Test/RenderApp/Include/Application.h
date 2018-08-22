#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include "Renderer.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "SystemCursor.h"
#include "FrameRateController.h"

class Application
{
public:
	Application();
	virtual ~Application();

	virtual void OnInit();
	virtual void OnSetInitialDisplaySettings(DisplaySettings&);
	virtual void OnExit();
	virtual void OnPreUpdate();
	virtual void OnUpdate();
	virtual void OnPostUpdate();
	virtual void OnSuspend();
	virtual void OnResume();

	Renderer* GetRenderer();
    
	void Exit();

	void SetWindowTitle(const std::string&);
	std::string GetWindowTitle();
	std::string GetDirectory();
};


#endif
