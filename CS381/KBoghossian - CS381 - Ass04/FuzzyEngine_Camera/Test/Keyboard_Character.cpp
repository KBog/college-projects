#include "Keyboard_Character.h"

void Jump_KeyboardCharacter::Update_Effect(void)
{
	Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->y,Rotational_Speed.Z) * Body_Coordinates->BodyCoordinates_Orientation;
	//Do the twist
	Update_ObjectBodyCoordinates();
	Update_ObjectMatrix(Local_ObjectTransformation);
	Update_ObjectBoundingBox(Local_ObjectTransformation,Parent->Parent_ObjectMatrix);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Normal_KeyboardCharacter::Update_Effect(void)
{
	//Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->z,Rotational_Speed.Z) * Body_Coordinates->BodyCoordinates_Orientation;
	//Do the twist
	Update_ObjectBodyCoordinates();
	Update_ObjectMatrix(Local_ObjectTransformation);
	Update_ObjectBoundingBox(Local_ObjectTransformation,Parent->Parent_ObjectMatrix);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Cursor_KeyboardCharacter::Update_Effect(void)
{
	//Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->x,Rotational_Speed.X) * Body_Coordinates->BodyCoordinates_Orientation;
	//Do the twist
	Update_ObjectBodyCoordinates();
	Update_ObjectMatrix(Local_ObjectTransformation);
	Update_ObjectBoundingBox(Local_ObjectTransformation,Parent->Parent_ObjectMatrix);
}