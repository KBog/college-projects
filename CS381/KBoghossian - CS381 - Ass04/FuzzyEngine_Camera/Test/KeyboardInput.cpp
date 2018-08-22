#include "KeyboardInput.h"

/* KeyboardManager Constructor */
KeyboardManager::KeyboardManager (string cursor_shape, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale, float character_spacing, float line_spacing, int max_line_length, int max_line_count, Manager_Effect rendering_effect, Manager_Rendering rendering_type):
Character_Spacing(character_spacing), Line_Spacing(line_spacing), Max_Line_Length(max_line_length), Max_Line_Count(max_line_count), Rendering_Effect(rendering_effect), RenderingType(rendering_type),
Object("NO_3DS","NO_3DS",position,bodycoordinates,orientation,rotational_speed,scale)
{
	GameOver_List=GoodJob_List=PlayerName_List=NULL;
	/* Default pivot center is the center of the phrase */
	PivotCenter=PivotCenter_Type;
	WaitingForInput=false;
	DisplayMessage=false;
	Current_LineCount= 0;
	Chars_Nbr= Cursor_Index= 0;
	
	Dir_Path=app.GetDirectory() + "Resources\\";
	GameOver_Str="GAME";
	GoodJob_Str="EMA";
	
	Attach_Child((Object *)new Cursor_KeyboardCharacter(cursor_shape,"NO_3DS",Point3D(0,1,0),new BodyCoordinates,Vector3D(0,0,0),Vector3D(1,0,0),Point3D(1,1,1)));
	Cursor=Object_Children;
	Cursor_Position=Cursor->Position;
}

KeyboardManager::KeyboardManager():Object()
{
	GameOver_List=GoodJob_List=PlayerName_List=NULL;
	/* Default pivot center is the center of the phrase */
	PivotCenter=PivotCenter_Type;
	WaitingForInput=false;
	DisplayMessage=false;
	Current_LineCount= 0;
	Chars_Nbr= Cursor_Index= 0;
}

/* Function to update the Effect of each group of characters or KeyboardManager */
void KeyboardManager::Update_Effect(void)
{
	switch(Rendering_Effect)
	{
		case ManagerNormal_Type:
			break;

		case ManagerTwist_Type:
			Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->y,Rotational_Speed.Y) * Body_Coordinates->BodyCoordinates_Orientation;
			break;

		case ManagerRoll_Type:
			break;
	}
}
void KeyboardManager::Update_AI (void)
{
	//Waiting_For_Input(PlayerName);
	//Waiting_For_IP();
	if(Keyboard::IsTriggered(KeyboardKeyG))
	{
		WaitingForInput=true;
		//DisplayMessage=true;
		//Convert_StringToObjects(GameOver_Str,&GameOver_List,CharacterNormal_Type);
		//delete Remove_FromList(&Object_Children,Object_Children);
		//GameOver_List=Object_Children;
	}
	/*if(Keyboard::IsTriggered(KeyboardKeyH))
	{
		Object_Children=NULL;
		Convert_StringToObjects(GoodJob_Str,&GoodJob_List,CharacterNormal_Type);
	}
	if(Keyboard::IsTriggered(KeyboardKeyJ))
	{
		Object_Children=GameOver_List;
	}
	if(Keyboard::IsTriggered(KeyboardKeyK))
	{
		Object_Children=GoodJob_List;
	}*/

	if(WaitingForInput)
		Waiting_For_Input(PlayerName,&PlayerName_List);
	else if(DisplayMessage)
	{
	}

	Update_ObjectBodyCoordinates();
	Update_ObjectMatrix(Final_ObjectTransformation);
	Update_ObjectBoundingBox(Final_ObjectTransformation);

	//
	//Matrix3D Inv;
	//Matrix3D::GetInverse4x4(app.GetRenderer()->GetViewingTransformation(),Inv);
	//Parent_ObjectMatrix = Inv*Parent_ObjectMatrix;
}

/* Function to reset the cursor properties and reset the Object_Children list to the cursor only */
void KeyboardManager::Add_Cursor (void)
{	
	/* Modify the cursor position as the initial cursor position */
	Cursor->Position=Cursor_Position;
	/* Init the link list children to the cursor only */
	Object_Children=Cursor;
	/* Init the counters */
	Chars_Nbr=0;
	Cursor_Index=0;
}

/* Function to wait for the user input to enter characters */
void KeyboardManager::Waiting_For_Input(string &str, Object **list)
{
	Renderer *renderer = app.GetRenderer();
	/* Take the user input and return the character */
	Char_Loader *CLoad=KeyboardKeys();
	
	/* if the char is different from enter & from NULL --> store it */
	if(CLoad->ch!=13 && CLoad->ch!=0)
	{
		/* If the character is backspace --> modify the string */
		if(CLoad->ch==8 && (Cursor_Index > 0))
		{
			const char *ds = str.c_str();
			for(unsigned int i=0; i<str.size(); i++);
			str[i-1]='\0';
			str.resize(str.size()-1);
		}
		/* Add it to the string only if there is space left in the line */
		else if(Cursor_Index < Max_Line_Length)
			str += CLoad->ch;
		/* Convert the character to object */
		Convert_CharToObject(CLoad, CharacterNormal_Type);
	}
	/* if the user pressed enter --> stop waiting */
	else if(CLoad->ch==13)
	{
		const char *sd = str.c_str();
		WaitingForInput=false;
		/* Removing the whole list without the cursor */
		Object *Obj_Next=NULL;
		for(Object *tmp_obj=Object_Children->next; tmp_obj; tmp_obj=Obj_Next)
		{
			Obj_Next=tmp_obj->next;
			tmp_obj->Loader=NULL;
			delete Remove_FromList(&Object_Children,tmp_obj);
		}
		/* Modifying the manager effect */
		Rendering_Effect=ManagerTwist_Type;
		/* Convert the string chars to objects and store them in list */
		Convert_StringToObjects(str,list,CharacterJump_Type);
		/* Reposition the objects */
		Reposition_Manager();
	}
}

/* Function to convert a string to a set of objects while specifying the characters' effects */
void KeyboardManager::Convert_StringToObjects (string str, Object **ConvertedList, Character_Effect Char_Effect)
{
	/* Re-Init the cursor to reuse it */
	Add_Cursor();
	/* Take char by char */
	for(unsigned int i=0; i<str.size(); i++)
	{
		char sd = str[i];
		/* Convert the current char to object */
		Convert_CharToObject(LoaderArray[str[i]],Char_Effect);
	}
	/* Removing the cursor */
	Cursor=Remove_FromList(&Object_Children,Object_Children);
	/* Take the new phrase */
	*ConvertedList=Object_Children;
}

/* Function to convert a character to a character with its effect (behavior) */
void KeyboardManager::Convert_CharToObject(Char_Loader *CLoad, Character_Effect Char_Effect)
{
	/* Space key was pressed */
	if(CLoad->ch == ' ')
	{
		/* if the nbr of chars didn't exceed the MaxLineLength */
		if(Cursor_Index < Max_Line_Length)
		{
			/* Just move the cursor by the cursor's width */
			Object_Children->Position += Object_Children->Body_Coordinates->x*(Object_Children->BBox.BBox_Width + Character_Spacing);
			/* Update the cursor index */
			Cursor_Index++;
			/* Insert a 0 in the tracker array */
			Tracker.push_back(0);
		}
		return;
	}
	/* Backspace key was pressed */
	else if(CLoad->ch == 8)
	{
		/* If the cursor position is not at the beginning of the phrase */
		if(Cursor_Index > 0)
		{
			/* If the current cursor position in the tracker is a 1 --> normal character */
			if(Tracker[Cursor_Index-1])
			{
				/* Move the cursor back at a distance equal to the character's width to be deleted */
				Object_Children->Position -= Object_Children->Body_Coordinates->x*(Return_AtIndex(&Object_Children,Chars_Nbr)->BBox.BBox_Width + Character_Spacing);
				Object *tmp = Return_AtIndex(&Object_Children,Chars_Nbr);
				tmp->Loader=NULL;
				/* Delete the character from the list */
				delete Remove_FromList(&Object_Children, tmp);						
				/* Update the number of characters */
				Chars_Nbr--;
				/* Remove the value from the tracker array */
				Tracker.pop_back();
				/* Update the cursor position */
				Cursor_Index--;
				return;
			}
			/* Move back at a distance equal to the cursor's width */
			Object_Children->Position -= Object_Children->Body_Coordinates->x*(Object_Children->BBox.BBox_Width + Character_Spacing);
			/* Remove the value from the tracker array */
			Tracker.pop_back();
			/* Update the cursor position */
			Cursor_Index--;
		}
		return;
	}
	/* Neither space nor backspace (normal character) 
		it will only insert a new character if the nbr 
		of chars didn't exceed the MaxLineLength */
	if(Cursor_Index < Max_Line_Length)
	{
		/* Update the current cursor index */
		Cursor_Index++;
		/* Update the number of chars */
		Chars_Nbr++;
		/* Update the tracker */
		Tracker.push_back(1);
		/* Set the path of the loader */
		string Char_Path = Dir_Path + "Char_" + CLoad->ch + ".3DS";
		
		/* Check which character effect has been chosen */
		switch(Char_Effect)
		{
			case CharacterNormal_Type://Noo effects
				Attach_Child((Object *)new Normal_KeyboardCharacter(CLoad->CharLoader,Object_Children->Position,new BodyCoordinates,Vector3D(0,0,0),Vector3D(1,0,1),Point3D(1,1,1)));
				break;
			case CharacterJump_Type://Effects
				Attach_Child((Object *)new Jump_KeyboardCharacter(CLoad->CharLoader,Object_Children->Position,new BodyCoordinates,Vector3D(0,0,0),Vector3D(1,0,1),Point3D(1,1,1)));
				break;
		}
		/* Go to the end of the list to take the last character then update the cursor position according to the new character's width (adding sequence is from left-to-right) */
		Object_Children->Position += Object_Children->Body_Coordinates->x*(Return_AtIndex(&Object_Children,Chars_Nbr)->BBox.BBox_Width + Character_Spacing);
	}
}

void KeyboardManager::Reposition_Manager (void)
{
	switch(PivotCenter)
	{
		case PivotCenter_Type:
		{
			/* Compute the half sentence width */
			float SentenceHalfWidth = (float(Cursor_Index)/2)*(Cursor->BBox.BBox_Width+Character_Spacing);
			/* Loop through the characters to modify their centers */
			for(Object *tmp_obj=Object_Children; tmp_obj; tmp_obj=tmp_obj->next)
				tmp_obj->Position -= tmp_obj->Body_Coordinates->x*SentenceHalfWidth;
			/* Modify the Manager's Center */
			Position += Body_Coordinates->x*SentenceHalfWidth;
		}
		break;

		/* No need to modify the centers coz it's by default centered at the first character */
		case PivotLeft_Type:
			break;

		//<- All Manager->
		case PivotRight_Type:
		{
			/* Compute the half sentence width */
			float SentenceWidth = (float(Cursor_Index-1))*(Cursor->BBox.BBox_Width+Character_Spacing);
			/* Loop through the characters to modify their centers */
			for(Object *tmp_obj=Object_Children; tmp_obj; tmp_obj=tmp_obj->next)
				tmp_obj->Position -= tmp_obj->Body_Coordinates->x*SentenceWidth;
			/* Modify the Manager's Center */
			Position += Body_Coordinates->x*SentenceWidth;
		}
		break;
	}
}

/* Function to render the manager as HUD or in the world space */
void KeyboardManager::Render_Manager (void)
{		
	/* World rendering type */
	if(RenderingType==WorldRendering_Type)
		Render(app.GetRenderer());
	/* HUD rendering type */
	else
		RenderAsHUD(app.GetRenderer());
}

KeyboardManager::~KeyboardManager ()
{
	/* Set the children list to NULL */
	Object_Children=NULL;
	Object *Next=NULL;
	/* Delete the gameover list */
	for(Object *tmp=GameOver_List; tmp; tmp=Next)
	{
		Next=tmp->next;
		tmp->Loader=NULL;
		delete Remove_FromList(&GameOver_List,tmp);
	}
	/* Delete the GoodJob_List list */
	for(Object *tmp=GoodJob_List; tmp; tmp=Next)
	{
		Next=tmp->next;
		tmp->Loader=NULL;
		delete Remove_FromList(&GoodJob_List,tmp);
	}
	/* Delete the PlayerName_List list */
	for(Object *tmp=PlayerName_List; tmp; tmp=Next)
	{
		Next=tmp->next;
		tmp->Loader=NULL;
		delete Remove_FromList(&PlayerName_List,tmp);
	}
}

Char_Loader *KeyboardManager::KeyboardKeys(void)
{
	bool Shift_IsPressed = false;
	
	/*if(Keyboard::IsTriggered(KeyboardKeyBack))
		//...
	else if(Keyboard::IsTriggered(KeyboardKeyTab))
		//...
	else if(Keyboard::IsTriggered(KeyboardKeyClear))
		//...
	else */
	if(Keyboard::IsTriggered(KeyboardKeyReturn))
		return LoaderArray[13];//Enter
	else if(Keyboard::IsTriggered(KeyboardKeyBack))
		return LoaderArray[8];

	if(Keyboard::IsPressed(KeyboardKeyShift))
		Shift_IsPressed = true;

	if(Keyboard::IsTriggered(KeyboardKeySpace))
		return LoaderArray[32];
	else if( Keyboard::IsTriggered(KeyboardKeyA))
	{
		if(Shift_IsPressed)
			return LoaderArray[65];//A
		else
			return LoaderArray[97];//a
	}
	else if(Keyboard::IsTriggered(KeyboardKeyB))
	{
		if(Shift_IsPressed)
			return LoaderArray[66];//B
		else
			return LoaderArray[98];//b
	}
	else if(Keyboard::IsTriggered(KeyboardKeyC))
	{
		if(Shift_IsPressed)
			return LoaderArray[67];//C
		else
			return LoaderArray[99];//c
	}
	else if(Keyboard::IsTriggered(KeyboardKeyD))
	{
		if(Shift_IsPressed)
			return LoaderArray[68];//D
		else
			return LoaderArray[100];//d
	}
	else if(Keyboard::IsTriggered(KeyboardKeyE))
	{
		if(Shift_IsPressed)
			return LoaderArray[69];//E
		else
			return LoaderArray[101];//e
	}
	else if(Keyboard::IsPressed(KeyboardKeyShift) && Keyboard::IsTriggered(KeyboardKeyF))
	{
		if(Shift_IsPressed)
			return LoaderArray[70];//F
		else
			return LoaderArray[102];//f
	}
	else if(Keyboard::IsTriggered(KeyboardKeyG))
	{
		if(Shift_IsPressed)
			return LoaderArray[71];//G
		else
			return LoaderArray[103];//g
	}
	else if(Keyboard::IsTriggered(KeyboardKeyH))
	{
		if(Shift_IsPressed)
			return LoaderArray[72];//H
		else
			return LoaderArray[104];//h
	}
	else if(Keyboard::IsTriggered(KeyboardKeyI))
	{
		if(Shift_IsPressed)
			return LoaderArray[73];//I
		else
			return LoaderArray[105];//i
	}
	else if(Keyboard::IsTriggered(KeyboardKeyJ))
	{
		if(Shift_IsPressed)
			return LoaderArray[74];//J
		else
			return LoaderArray[106];//j
	}
	else if(Keyboard::IsTriggered(KeyboardKeyK))
	{
		if(Shift_IsPressed)
			return LoaderArray[75];//K
		else
			return LoaderArray[107];//k
	}
	else if(Keyboard::IsTriggered(KeyboardKeyL))
	{
		if(Shift_IsPressed)
			return LoaderArray[76];//L
		else
			return LoaderArray[108];//l
	}
	else if(Keyboard::IsTriggered(KeyboardKeyM))
	{
		if(Shift_IsPressed)
			return LoaderArray[77];//M
		else
			return LoaderArray[109];//m
	}
	else if(Keyboard::IsTriggered(KeyboardKeyN))
	{
		if(Shift_IsPressed)
			return LoaderArray[78];//N
		else
			return LoaderArray[110];//n
	}
	else if(Keyboard::IsTriggered(KeyboardKeyO))
	{
		if(Shift_IsPressed)
			return LoaderArray[79];//O
		else
			return LoaderArray[111];//o
	}
	else if(Keyboard::IsTriggered(KeyboardKeyP))
	{
		if(Shift_IsPressed)
			return LoaderArray[80];//P
		else
			return LoaderArray[112];//p
	}
	else if(Keyboard::IsTriggered(KeyboardKeyQ))
	{
		if(Shift_IsPressed)
			return LoaderArray[81];//Q
		else
			return LoaderArray[113];//q
	}
	else if(Keyboard::IsTriggered(KeyboardKeyR))
	{
		if(Shift_IsPressed)
			return LoaderArray[82];//R
		else
			return LoaderArray[114];//r
	}
	else if(Keyboard::IsTriggered(KeyboardKeyS))
	{
		if(Shift_IsPressed)
			return LoaderArray[83];//S
		else
			return LoaderArray[115];//s
	}
	else if(Keyboard::IsTriggered(KeyboardKeyT))
	{
		if(Shift_IsPressed)
			return LoaderArray[84];//T
		else
			return LoaderArray[116];//t
	}
	else if(Keyboard::IsTriggered(KeyboardKeyU))
	{
		if(Shift_IsPressed)
			return LoaderArray[85];//U
		else
			return LoaderArray[117];//u
	}
	else if(Keyboard::IsTriggered(KeyboardKeyV))
	{
		if(Shift_IsPressed)
			return LoaderArray[86];//V
		else
			return LoaderArray[118];//v
	}
	else if(Keyboard::IsTriggered(KeyboardKeyW))
	{
		if(Shift_IsPressed)
			return LoaderArray[87];//W
		else
			return LoaderArray[119];//w
	}
	else if(Keyboard::IsTriggered(KeyboardKeyX))
	{
		if(Shift_IsPressed)
			return LoaderArray[88];//X
		else
			return LoaderArray[120];//x
	}
	else if(Keyboard::IsTriggered(KeyboardKeyY))
	{
		if(Shift_IsPressed)
			return LoaderArray[89];//Y
		else
			return LoaderArray[121];//y
	}
	else if(Keyboard::IsTriggered(KeyboardKeyZ))
	{
		if(Shift_IsPressed)
			return LoaderArray[90];//Z
		else
			return LoaderArray[122];//z
	}
	else if(Keyboard::IsTriggered(KeyboardKey0) || Keyboard::IsTriggered(KeyboardKeyNumpad0))
	{
		if(Shift_IsPressed)
			return LoaderArray[41];//)
		else
			return LoaderArray[48];//0
			
	}
	else if(Keyboard::IsTriggered(KeyboardKey1) || Keyboard::IsTriggered(KeyboardKeyNumpad1))
	{
		if(Shift_IsPressed)
			return LoaderArray[33];//!
		else
			return LoaderArray[49];//1
	}
	else if(Keyboard::IsTriggered(KeyboardKey2) || Keyboard::IsTriggered(KeyboardKeyNumpad2))
	{
		if(Shift_IsPressed)
			return LoaderArray[64];//@
		else
			return LoaderArray[50];//2
	}
	else if(Keyboard::IsTriggered(KeyboardKey3) || Keyboard::IsTriggered(KeyboardKeyNumpad3))
	{
		if(Shift_IsPressed)
			return LoaderArray[35];//#
		else
			return LoaderArray[51];//3
	}
	else if(Keyboard::IsTriggered(KeyboardKey4) || Keyboard::IsTriggered(KeyboardKeyNumpad4))
	{
		if(Shift_IsPressed)
			return LoaderArray[36];//$
		else
			return LoaderArray[52];//4
	}
	else if(Keyboard::IsTriggered(KeyboardKey5) || Keyboard::IsTriggered(KeyboardKeyNumpad5))
	{
		if(Shift_IsPressed)
			return LoaderArray[37];//%
		else
			return LoaderArray[53];//5
	}
	else if(Keyboard::IsTriggered(KeyboardKey6) || Keyboard::IsTriggered(KeyboardKeyNumpad6))
	{
		if(Shift_IsPressed)
			return LoaderArray[94];//^
		else
			return LoaderArray[54];//6
	}
	else if(Keyboard::IsTriggered(KeyboardKey7) || Keyboard::IsTriggered(KeyboardKeyNumpad7))
	{
		if(Shift_IsPressed)
			return LoaderArray[38];//&
		else
			return LoaderArray[55];//7
	}
	else if(Keyboard::IsTriggered(KeyboardKey8) || Keyboard::IsTriggered(KeyboardKeyNumpad8))
	{
		if(Shift_IsPressed)
			return LoaderArray[42];//*
		else
			return LoaderArray[56];//8
	}
	else if(Keyboard::IsTriggered(KeyboardKey9) || Keyboard::IsTriggered(KeyboardKeyNumpad9))
	{
		if(Shift_IsPressed)
			return LoaderArray[40];//(
		else
			return LoaderArray[57];//9
	}
	//else if( Keyboard::IsTriggered(KeyboardKeyOEMComma))
	//{
	//	if(Shift_IsPressed)
	//		return '<';
	//	else
	//		return ',';
	//}
	//else if( Keyboard::IsTriggered(KeyboardKeyOEMPeriod))
	//{
	//	if(Shift_IsPressed)
	//		return '>';
	//	else
	//		return '.';
	//}
	//else if( Keyboard::IsTriggered(KeyboardKeyDecimal))
	//{
	//	if(Shift_IsPressed)
	//		return '.';
	//}
	//else if(Keyboard::IsTriggered(KeyboardKeySpace))
	//	return ' ';
	//else if(Keyboard::IsTriggered(KeyboardKeyPause))
		//...
	//else if(Keyboard::IsTriggered(KeyboardKeyUp))
	//	//...
	//else if(Keyboard::IsTriggered(KeyboardKeyDown))
	//	//...
	//else if(Keyboard::IsTriggered(KeyboardKeyLeft))
	//	//...
	//else if(Keyboard::IsTriggered(KeyboardKeyRight))
	//	//...
	//else if(Keyboard::IsTriggered(KeyboardKeyOEMMinus) || Keyboard::IsTriggered(KeyboardKeySubtract))
	//	return '-';
	//else if(Keyboard::IsTriggered(KeyboardKeyOEMPeriod))
	//	return '.';	

	return LoaderArray[0];
}