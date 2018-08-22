#ifndef _KEYBOARD_INPUT_H_
#define _KEYBOARD_INPUT_H_

#include "Object.h"
#include "Keyboard_Character.h"

enum Manager_Effect{ ManagerNormal_Type, ManagerTwist_Type, ManagerRoll_Type};
enum Manager_PivotingPt { PivotCenter_Type, PivotLeft_Type, PivotRight_Type };
enum Manager_Rendering { WorldRendering_Type, HUDRendering_Type };

enum Character_Effect { CharacterNormal_Type=1, CharacterJump_Type };

/* Character Loader struct */
struct Char_Loader
{
	/* Loader to store the character's model */
	_3DSLoader *CharLoader;
	/* The character itself */
	char ch;

	/* Default constructor */
	Char_Loader()
	{
		CharLoader=NULL;
		ch='\0';
	}
	/* Constructor */
	Char_Loader (char c) : ch(c)
	{
		CharLoader=NULL;
	}
	/* Constructor */
	Char_Loader (char c, _3DSLoader *loader) :
	ch(c), CharLoader(loader)
	{
	}

	~Char_Loader ()
	{
		delete CharLoader;
	}
};

//======================================================================================
//KeyboardManager class used to group the list of characters. It helps to behave the 
//list of characters as 1 single object!
/*
- Add time class to game engine
- Test the delay effect
- Tell reine to do all the characters in the same BBox
- Tell reine to name the character as followin: A=BigA (Char_BigA) - a=SmaA (Char_SmaA)
*/
class KeyboardManager : public Object 
{
	public:
		/* This enum variable helps us to identify which type of Rendering Effect for the group of Characters */
		Manager_Effect Rendering_Effect;
		/* To specify the manager's pivotting point */
		Manager_PivotingPt PivotCenter;
		/* To specify whether the rendering must be in the world space or the camera space */
		Manager_Rendering RenderingType;
		/* Setting the space between each two characters */
		float Character_Spacing, Line_Spacing;
		/* Variables to save the paragraph specs */
		int Max_Line_Count, Max_Line_Length, Current_LineCount;
		/* Current Cursor Index and Current Number Of Characters */
		int Cursor_Index, Chars_Nbr;
		/* Depending on the current character if it was a space=0 & character=1 */
		vector<int> Tracker;
		/* Variable to help render while waiting for input characters! */
		bool WaitingForInput, DisplayMessage;
		/* Strings to store the player name and IP Address ... */
		string PlayerName, IP_Address, Dir_Path;
		/* Strings to store the word to show in the game */
		string GameOver_Str, GoodJob_Str;
		/* The cursor */
		Object *Cursor;
		/* Cursor's Position */
		Point3D Cursor_Position;
		
		Char_Loader **LoaderArray;

		/* The lists of phrases to display at run time (must be initialized in constructor) */
		Object *GameOver_List, *GoodJob_List, *PlayerName_List;

		/* KeyboardManager Constructor */
		KeyboardManager (string cursor_shape, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale, float character_spacing, float line_spacing, int max_line_length, int max_line_count, Manager_Effect rendering_effect=ManagerNormal_Type, Manager_Rendering rendering_type=WorldRendering_Type);

		KeyboardManager();

		/* Function to update the Effect of each group of characters or KeyboardManager */
		void Update_Effect(void);

		void Update_AI (void);

		/* Function to reset the cursor properties and reset the Object_Children list to the cursor only */
		void Add_Cursor (void);

		/* Function to wait for the user input to enter characters */
		void Waiting_For_Input(string &str, Object **list);

		/* Function to convert a string to a set of objects while specifying the characters' effects */
		void Convert_StringToObjects (string str, Object **ConvertedList, Character_Effect Char_Effect);

		/* Function to convert a character to a character with its effect (behavior) */
		void Convert_CharToObject(Char_Loader *CLoad, Character_Effect Char_Effect);

		void Reposition_Manager (void);

		/* Function to render the manager as HUD or in the world space */
		void Render_Manager (void);

		~KeyboardManager ();

		Char_Loader *KeyboardKeys(void);
};


#endif