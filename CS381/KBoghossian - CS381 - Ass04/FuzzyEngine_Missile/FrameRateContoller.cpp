#include <windows.h>
#include "FrameRateController.h"
#include "Settings.h"

int m_FrameRate; 		        
float m_FrameTime; 	            
float m_CurrentFrameTime; 				
unsigned int m_OldTime;

void FRC_Init() 				
{ 		
	timeBeginPeriod(1); 	
	m_OldTime = 0; 				
	FRC_SetFrameRate(FRAMERATE); 				
	m_CurrentFrameTime = m_FrameTime; 				
}

void FRC_ShutDown() 				
{ 				
	timeEndPeriod(1); 				
}

void FRC_SetFrameRate(int frameRate) 				
{ 				
	m_FrameRate = frameRate; 				
	m_FrameTime = 1000.0f/m_FrameRate; 				
}

float FRC_GetCurrentFrameRate() 				
{ 				
	return 1000.0f/m_CurrentFrameTime; 				
}

void FRC_Start() 				
{ 				
	m_OldTime = timeGetTime(); 				
}

void FRC_End() 				
{ 				
	float delta; 				
	m_CurrentFrameTime = (float)(timeGetTime() - m_OldTime); 				
	delta = m_FrameTime - m_CurrentFrameTime; 				
 				
	if(delta>0) 				
	{ 				
		m_OldTime = timeGetTime(); 				
 				
		while(timeGetTime()-m_OldTime<delta); 				
		//m_CurrentFrameTime = m_FrameTime; 				
	} 				
}
