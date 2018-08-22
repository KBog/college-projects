#include<windows.h> 				
#include "Keyboard.h" 				
#include "Settings.h"

static KeyState m_Keys[256]={0};
int Keyboard_IsPressed(int vKey) 				
{ 				
	return m_Keys[vKey].m_IsPressed; 				
}

int Keyboard_IsTriggered(int vKey) 				
{ 				
	return m_Keys[vKey].m_IsTriggered; 				
}

void Keyboard_Update() 				
{ 				
	int i=0; 				
	BYTE KeyStatus[256]={0}; 				
	GetKeyboardState(KeyStatus); 				
 				
	for(i=0;i<256;i++) 				
	{ 				
		m_Keys[i].m_IsPressed = (KeyStatus[i] & (1 << 7))?1:0;				
 				
		if(m_Keys[i].m_IsPressed) 				
		{ 				
			if(m_Keys[i].m_WasPressed) 				
				m_Keys[i].m_IsTriggered = 0; 				
			else{ 				
				m_Keys[i].m_WasPressed  = 1; 				
				m_Keys[i].m_IsTriggered = 1; 				
			} 				
		} 				
		else{ 				
			m_Keys[i].m_IsTriggered = 0; 				
			m_Keys[i].m_WasPressed  = 0; 				
		} 				
	} 				
}
