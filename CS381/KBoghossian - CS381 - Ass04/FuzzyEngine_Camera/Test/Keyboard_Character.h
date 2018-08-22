#include "Object.h"

//======================================================================================
//KeyboardClass class used for individual character behavior!
class KeyboardCharacter : public Object 
{
	public:
		/* KeyboardCharacter variable that points to the Previous & Next nodes */
		//KeyboardCharacter * Previous, * Next;

		/* KeyboardManager Default Constructor */
		KeyboardCharacter():Object()
		{
		}
		/* KeyboardManager Constructor */
		KeyboardCharacter(string Render_Poly, string Collision_Poly, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale):	
		Object(Render_Poly,Collision_Poly,position,bodycoordinates,orientation,rotational_speed,scale)
		{

		}
		/* KeyboardManager Constructor */
		KeyboardCharacter(_3DSLoader *loader, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale):	
		Object(loader,position,bodycoordinates,orientation,rotational_speed,scale)
		{

		}
		/* Function to update the Effect of each group of characters or KeyboardManager */
		virtual void Update_Effect(void) {}
};
//=============================================================================================================
//This function is like an Add-On to the KeyboardCharacter class, to apply the virtual update of each character
//depending on the given effect when creating it at the begining!
class Jump_KeyboardCharacter : public KeyboardCharacter
{
	public:
		Jump_KeyboardCharacter(_3DSLoader *loader, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale):	
		KeyboardCharacter(loader,position,bodycoordinates,orientation,rotational_speed,scale)
		{
		}
		void Update_Effect(void);
};

class Normal_KeyboardCharacter : public KeyboardCharacter
{
	public:
		Normal_KeyboardCharacter(_3DSLoader *loader, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale):	
		KeyboardCharacter(loader,position,bodycoordinates,orientation,rotational_speed,scale)
		{
		}
		void Update_Effect(void);
};

//This function is used to update the Cursor which is 
class Cursor_KeyboardCharacter : public KeyboardCharacter
{
	public:
		Cursor_KeyboardCharacter(string Render_Poly, string Collision_Poly, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale):	
		KeyboardCharacter(Render_Poly,Collision_Poly,position,bodycoordinates,orientation,rotational_speed,scale)
		{
		}
		void Update_Effect(void);
};