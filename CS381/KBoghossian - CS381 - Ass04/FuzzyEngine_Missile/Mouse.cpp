#include <windows.h> 				
#include "Mouse.h" 				
#include "Settings.h"

extern HWND GlobalHwnd; 				
static KeyState m_Keys[3] = {0};

int Mouse_IsPressedLButton() 				
{ 				
	return m_Keys[0].m_IsPressed; 				
}

int Mouse_IsTriggeredLButton() 				
{ 				
	return m_Keys[0].m_IsTriggered; 				
}

int Mouse_IsPressedMButton() 				
{ 				
	return m_Keys[1].m_IsPressed; 				
}

int Mouse_IsTriggeredMButton() 				
{ 				
	return m_Keys[1].m_IsTriggered; 				
}

int Mouse_IsPressedRButton() 				
{ 				
	return m_Keys[2].m_IsPressed; 				
}

int Mouse_IsTriggeredRButton() 				
{ 				
	return m_Keys[2].m_IsTriggered; 				
}

void Mouse_GetCursorPosition(int *x, int *y) 				
{ 				
	POINT pt = {0}; 				
 				
	GetCursorPos(&pt); 				
	ScreenToClient(GlobalHwnd, &pt); 				
	*x = pt.x; 				
	*y = pt.y; 				
}

void Mouse_ShowCursor(int b) 				
{ 				
	if(b) 				
		while(ShowCursor(TRUE)<0); 				
	else 				
		while(ShowCursor(FALSE)>=0); 				
}

void Mouse_Update() 				
{ 				
	int i=0; 				
 				
	SHORT ks[3] = {0}; 				
	ks[0] = GetAsyncKeyState(VK_LBUTTON); 				
	ks[1] = GetAsyncKeyState(VK_MBUTTON); 				
	ks[2] = GetAsyncKeyState(VK_RBUTTON); 				
 				
	for(i=0; i<3; i++) 				
	{ 				
		m_Keys[i].m_IsPressed = (ks[i] & (1 << 15)) ? 1:0; 				
		if(m_Keys[i].m_IsPressed) 				
		{ 				
			if(m_Keys[i].m_WasPressed) 				
				m_Keys[i].m_IsTriggered = 0; 				
			else 				
			{ 				
				m_Keys[i].m_WasPressed = 1; 				
				m_Keys[i].m_IsTriggered = 1; 				
			} 				
		} 				
		else 				
		{ 				
			m_Keys[i].m_WasPressed = 0; 				
			m_Keys[i].m_IsTriggered = 0; 				
		} 				
	} 				
}
