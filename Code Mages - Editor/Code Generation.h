#include<fstream>
using namespace std;

/*Defining Game_Settings structure type*/
struct Game_Settings
{
	int Width,Height;
	char Style[150];
	int FrameRate;
};

void Generate_GameSettings(Game_Settings Settings);
void Generate_Main(Game_Settings GameSettings);
void Generate_Graphics(void);
void GenerateCode_Mouse(void);
void GenerateCode_Keyboard(void);
void GenerateCode_FrameRateController(void);
void GenerateCode_Clipping(void);
void GenerateCode_StandardTypes(void);
void GenerateCode_Collision(void);
void GenerateCode_CharacterDesign(void);
