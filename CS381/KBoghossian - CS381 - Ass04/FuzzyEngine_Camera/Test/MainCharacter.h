#ifndef _MAINCHARACTER_H_
#define _MAINCHARACTER_H_

#include "RigidBody.h"
#include "Weapon.h"
#include "MainCharacter_Parts.h"
#include "MainCharacter_ObjectParts.h"


//#define MAINCHARACTER_CHILDREN_NUMBER 1
#define MAINCHARACTER_WEAPONS_NUMBER 2

/* --------------------- Defines for the Glider --------------------- */
#define GLIDER_MAX_PITCH_ANGLE 15
#define GLIDER_INTERP_PITCH_ANGLE_P 0.05f
#define GLIDER_INTERP_PITCH_ANGLE_N 0.1f

#define GLIDER_MAX_ROLL_ANGLE 35
#define GLIDER_INTERP_ROLL_ANGLE_P 0.5f
#define INTERP_ROLL_ANGLE_N 0.08f

#define GLIDER_YAW_RATIO 1
#define GLIDER_BACKWHEEL_PITCH_RATIO 0.1f

/* Ratios for the glider movements */
#define GLIDER_GLIDING_RATIO 0.95f//140
#define GLIDER_GLIDING_FRONT_RATIO 1//10

#define GLIDER_BACKWHEEL_MAX_PITCH 45
#define GLIDER_BRAKE_RATIO 10

/* --------------------- Defines for the Bicycle --------------------- */
#define MAX_FRONTWHEEL_YAW_ANGLE 25
#define INTERP_FRONTWHEEL_YAW_ANGLE_P 1
#define INTERP_FRONTWHEEL_YAW_ANGLE_N 0.1f

#define FRONT_BACK_WHEELS_DISTANCE 35	//UNITS

/* Ratios for the Bicycle's Movements */
#define BICYCLE_TRACTION_RATIO 25
#define BICYCLE_BRAKE_RATIO 3

#define SSS -1

enum GliCycle_State { Glider_State, Bicycle_State };
extern float Glider_SlowMotion;

/* Main Character class */
class MainCharacter : public RigidBody
{
	public:	
			/* Initial CrossHair and the current crosshair of type box */
			Box3D CrossHair, Initial_CrossHair;
			/* A list of all the weapons that the character will have */
			Weapon *Weapon_List[MAINCHARACTER_WEAPONS_NUMBER];
			/* To hold the number of lives */
			int Lives_Number;
			/* To hold the remaning health of the character */
			int Health;
			/* The Target that is targeted by the auto aim feature */
			Object *Target;
			float Brake;
			
			int MainCharacter_ChildrenNumber;
			int GliCycleState;

			Point3D Old_Position;
			float Delta_Distance;

			bool Right_WasPressed;
			bool Left_WasPressed;
			
			
			/* Main character constructor */
			MainCharacter (string Render_Poly, string Collision_Poly, int lives_number, int Max_Health, Point3D CrossHair_Dimensions, float CharacterCrossHair_Distance, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed,Point3D scale, 
						   float mass, float fluid_density, Vector3D velocity, bool Apply_Gravity=true, Constraint *_constraint=NULL);

			/* Main Character Destructor */
			~MainCharacter ();

			

			/* Computing the gravity center of the Parent and its children */
			void Compute_GravityCenter_WithChildren (void);
			

			/* Function to draw the cross hair */
			void Draw_CrossHair (void);
			void Draw_MainCharacter (void);

			/* Function to update the behavior of the character with its children */
			void Custom_Loads (void);
			/* Updating the CrossHair from the initial crosshair to the last status */
			void Update_Character_Objects (void);
			void Update_DiagramForces (void);
			/* Function to update the object physics properties */
			void Update_Physics (void);

			/* Function to check whether there is collision between the character and an item */
			bool Collision_WithItem (Object *item);
			//void Update_DynamicCollision_WithResponse(BSP_Tree *BSP);
			
};

#endif