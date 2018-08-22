#ifndef _MAINCHARACTER_OBJECTPARTS_H_
#define _MAINCHARACTER_OBJECTPARTS_H_

#include "Object.h"


enum ObjectChildren_Names { Back_Rudder };

/* Front Wheel class */
class MainCharacter_FrontWheel : public Object
{
	public:
			/* Wheel's Radius in meters */
			float Radius;

			/* Front Wheel constructor */
			MainCharacter_FrontWheel(string Render_Poly, string Collision_Poly, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale, float radius);
			void Update_AI (void);
};

/* Back Wheel class */
class MainCharacter_BackWheel : public Object
{
	public:
			/* Wheel's Radius in meters */
			float Radius;

			/* Back Wheel constructor */
			MainCharacter_BackWheel(string Render_Poly, string Collision_Poly, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale, float radius);
			void Update_AI (void);
};

/* Front Wheel class */
class MainCharacter_GliderTop : public Object
{
	public:
			float Upward_Gliding_Ratio , Frontal_Gliding_Ratio;
			/* Front Wheel constructor */
			MainCharacter_GliderTop(string Render_Poly, string Collision_Poly, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale, float upward_gliding_ratio , float frontal_gliding_ratio);
				
			void Update_AI (void);
};
#endif