
/*Define Window Dimensions and FrameRate*/
#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600
#define FRAMERATE		60

/*Structure specifying all different key states*/
typedef struct
{
	unsigned char m_IsPressed;
	unsigned char m_IsTriggered;
	unsigned char m_WasPressed;
}KeyState;
