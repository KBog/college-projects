#include "MainCharacter.h"
#include "Rocket.h"
#include "Bullet.h"


/* Main character constructor */
MainCharacter::MainCharacter (string Render_Poly, string Collision_Poly, int lives_number, int Max_Health, Point3D CrossHair_Dimensions, float CharacterCrossHair_Distance, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed,Point3D scale, 
				float mass, float fluid_density, Vector3D velocity, bool Apply_Gravity, Constraint *_constraint) :
Lives_Number(lives_number), Health(Max_Health),
RigidBody(Render_Poly,Collision_Poly,position,bodycoordinates,orientation,rotational_speed,scale,mass,fluid_density,velocity,Apply_Gravity,_constraint)
{	
	//Gravity_ON=OFF;
	//Drag_ON=OFF;
	//GliCycleState=Bicycle_State;
	GliCycleState=Glider_State;
	///
	Brake=5;
	//MAX VELOCITY
	//Add break to CONST
	//Add Traction Magnitude to CONST
	//When on ground with bycicle --> just translate without the Y
	///

	/* Setting all the available weapons for the main character in the whole game */
	Weapon_List[MachineGun_Type]	 = new Weapon("NO_3DS","NO_3DS",MachineGun_Type, new Bullet("NO_3DS","NO_3DS",Bullet_Type,2,1000,100), -1, 10, Point3D(0,0,0), new BodyCoordinates, Vector3D(0,0,0), Vector3D(0,0,0), Point3D(1,1,1));
	Weapon_List[RocketLauncher_Type] = new Weapon("NO_3DS","NO_3DS",RocketLauncher_Type, new Rocket("NO_3DS","NO_3DS",Rocket_Type,15,5000,10,NULL), 0, 120, Point3D(0,0,0), new BodyCoordinates, Vector3D(0,0,0), Vector3D(0,0,0), Point3D(1,1,1));

	/* Building the CrossHair Only once */
	/* Dimensions.X = Width ;; Dimensions.Y = Height ;; Dimensions.Z = Length */
	float Mid_Width = CrossHair_Dimensions.X/2;
	float Mid_Height = CrossHair_Dimensions.Y/2;
	float Mid_Length = CrossHair_Dimensions.Z/2;
	/* Setting the center of the box in the front of the character */
	Point3D CrossHair_Center = Point3D(0,0,0) + Body_Coordinates->z*CharacterCrossHair_Distance;
	Initial_CrossHair.ptA =	Point3D(CrossHair_Center.X+Mid_Width , CrossHair_Center.Y+Mid_Height , CrossHair_Center.Z+Mid_Length);
	Initial_CrossHair.ptB =	Point3D(CrossHair_Center.X+Mid_Width , CrossHair_Center.Y+Mid_Height , CrossHair_Center.Z-Mid_Length);
	Initial_CrossHair.ptC =	Point3D(CrossHair_Center.X-Mid_Width , CrossHair_Center.Y+Mid_Height , CrossHair_Center.Z-Mid_Length);
	Initial_CrossHair.ptD =	Point3D(CrossHair_Center.X-Mid_Width , CrossHair_Center.Y+Mid_Height , CrossHair_Center.Z+Mid_Length);
	Initial_CrossHair.ptE =	Point3D(CrossHair_Center.X+Mid_Width , CrossHair_Center.Y-Mid_Height , CrossHair_Center.Z+Mid_Length);
	Initial_CrossHair.ptF =	Point3D(CrossHair_Center.X+Mid_Width , CrossHair_Center.Y-Mid_Height , CrossHair_Center.Z-Mid_Length);
	Initial_CrossHair.ptG =	Point3D(CrossHair_Center.X-Mid_Width , CrossHair_Center.Y-Mid_Height , CrossHair_Center.Z-Mid_Length);
	Initial_CrossHair.ptH =	Point3D(CrossHair_Center.X-Mid_Width , CrossHair_Center.Y-Mid_Height , CrossHair_Center.Z+Mid_Length);

	/* Init the target to null */
	Target=NULL;

	Old_Position = Position;
	
	Attach_Child((Object *)new MainCharacter_GliderTop(app.GetDirectory() + "Resources\\GliderTop.3DS", "NO_3DS", Point3D(0,0.7f,0),new BodyCoordinates,Vector3D(0,0,0),Vector3D(1,0,0),Point3D(1,1,1),0.078f,0.3f));

	//Attach_Child((Object *)new MainCharacter_BackRudder(app.GetDirectory() + "Resources\\Tank_Bottom.3DS", "NO_3DS", Point3D(0,0.2f,0.426f),new BodyCoordinates,Vector3D(0,0,0),Vector3D(5,0,0),Point3D(0.2f,0.2f,0.2f),Material_Properties(0),Vector3D(30,0,0)));
	Attach_Child((Object *)new MainCharacter_FrontWheel(app.GetDirectory() + "Resources\\Back_Wheel.3DS", "NO_3DS", Point3D(0,0.3f,-0.426f),new BodyCoordinates,Vector3D(0,0,0),Vector3D(1,0,0),Point3D(1,1,1),0.575f));
	Attach_Child((Object *)new MainCharacter_BackWheel(app.GetDirectory() + "Resources\\Back_Wheel.3DS", "NO_3DS", Point3D(0,0.3f,0.426f),new BodyCoordinates,Vector3D(0,0,0),Vector3D(1,0,0),Point3D(1,1,1),0.575f));
	
	//Attach_Child((Object *)new MainCharacter_LeftRudder(app.GetDirectory() + "Resources\\Tank_Bottom.3DS", "NO_3DS", Point3D(-0.3f,0,0),new BodyCoordinates,Vector3D(0,0,0),Vector3D(0.25f,0,0),Point3D(0.2f,0.2f,0.2f),Vector3D(30,0,0)));
	//Attach_Child((Object *)new MainCharacter_RightRudder(app.GetDirectory() + "Resources\\Tank_Bottom.3DS", "NO_3DS", Point3D(0.3f,0,0),new BodyCoordinates,Vector3D(0,0,0),Vector3D(0.25f,0,0),Point3D(0.2f,0.2f,0.2f),Vector3D(30,0,0)));

	//((MainCharacter_GliderTop *)Object_Children)->Upward_Gliding_Ratio*=Global_Mass;
	//((MainCharacter_GliderTop *)Object_Children)->Frontal_Gliding_Ratio*=Global_Mass;
	Compute_GlobalBoundingBox();
	Old_Position = Position;
}

/* Main Character Destructor */
MainCharacter::~MainCharacter ()
{
}

/* Updating the CrossHair from the initial crosshair to the last status */
void MainCharacter::Update_Character_Objects (void)
{
	/*** Update the Cross Hair ***/
	/* Looping through the 8 points of the box */
	CrossHair.ptA = Final_ObjectTransformation*Initial_CrossHair.ptA;
	CrossHair.ptB = Final_ObjectTransformation*Initial_CrossHair.ptB;
	CrossHair.ptC = Final_ObjectTransformation*Initial_CrossHair.ptC;
	CrossHair.ptD = Final_ObjectTransformation*Initial_CrossHair.ptD;
	CrossHair.ptE = Final_ObjectTransformation*Initial_CrossHair.ptE;
	CrossHair.ptF = Final_ObjectTransformation*Initial_CrossHair.ptF;
	CrossHair.ptG = Final_ObjectTransformation*Initial_CrossHair.ptG;
	CrossHair.ptH = Final_ObjectTransformation*Initial_CrossHair.ptH;

	/*** Update the wheels ... ***/
	for(Object *tmp=Object_Children; tmp; tmp=tmp->next)
	{
		tmp->Update_AI();
		//Apply_Force(Force(tmp->World_Position,((MainCharacter_ObjectPart *)tmp)->Synchronize_ChildParent()));
	}
}

/* Function to update the behavior of the character with its children */
void MainCharacter::Custom_Loads (void)
{
	if(Keyboard::IsTriggered(KeyboardKeySpace))
	{
		if(GliCycleState==Glider_State)
			GliCycleState=Bicycle_State;
		else
			GliCycleState=Glider_State;
	}

	/* ---------------------------------- GLIDER CONTROLS & BEHAVIOR ---------------------------------- */
	if(GliCycleState==Glider_State)
	{
		/* Front Key will pitch the glider and oppose the gravity */
		if(Keyboard::IsPressed(KeyboardKeyI))
		{
			Custom_Forces_Sum += Vector3D(0,1,0)*13;
			Custom_Forces_Sum += Body_Coordinates->z*6;
			if(Orientation.X>-GLIDER_MAX_PITCH_ANGLE)
			{
				float Delta = (GLIDER_MAX_PITCH_ANGLE-fabsf(Orientation.X));
				Rotational_Speed.X = -Delta/100;
				Orientation.X += Rotational_Speed.X;
				Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->x,Rotational_Speed.X) * Body_Coordinates->BodyCoordinates_Orientation;
			}
		}
		/* No key is pressed --> will return the pitching to its initial smoothly and applying an opposing force 
			less than the weight to be able to glide */
		else
		{
			//Custom_Forces_Sum += Vector3D(0,1,0)*GLIDER_GLIDING_RATIO;
			//Custom_Forces_Sum += Body_Coordinates->z*GLIDER_GLIDING_FRONT_RATIO;
			Custom_Forces_Sum += /*((MainCharacter *)Parent)->Body_Coordinates->y*/Vector3D(0,1,0)*(Mass*GRAVITY_VALUE*GLIDER_GLIDING_RATIO);
			Custom_Forces_Sum += Body_Coordinates->world_z*(GLIDER_GLIDING_FRONT_RATIO);

			if(Orientation.X<0)
			{
				Orientation.X += GLIDER_INTERP_PITCH_ANGLE_N;
				Rotational_Speed.X=GLIDER_INTERP_PITCH_ANGLE_N;
				Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->x,Rotational_Speed.X) * Body_Coordinates->BodyCoordinates_Orientation;
			}
		}
		/* Braking the glider will increase the drag force by 10 */
		if(Keyboard::IsPressed(KeyboardKeyK))
			Cd=GLIDER_BRAKE_RATIO;
		else
			Cd=1;
		
		/* Only Right is pressed will roll by an angle to allow the it to strafe right */
		if(Keyboard::IsPressed(KeyboardKeyL))
		{
			Right_WasPressed=true;
			Left_WasPressed=false;
			if(Orientation.Z>-GLIDER_MAX_ROLL_ANGLE)
			{	
				float Delta = (-GLIDER_MAX_ROLL_ANGLE-Orientation.Z);
				Rotational_Speed.Z = Delta/100;
				//Rotational_Speed.Z = GLIDER_INTERP_ROLL_ANGLE_P;
				Orientation.Z += Rotational_Speed.Z;
				Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->z,-Rotational_Speed.Z) * Body_Coordinates->BodyCoordinates_Orientation;
			}
			//Custom_Forces_Sum -= Body_Coordinates->x*50;
			//Custom_Forces_Sum += Vector3D(0,1,0)*20;
		}
		/* Only Left is pressed will roll by an angle to allow the it to strafe left */
		else if(Keyboard::IsPressed(KeyboardKeyJ))
		{
			Right_WasPressed=false;
			Left_WasPressed=true;
			if(Orientation.Z<GLIDER_MAX_ROLL_ANGLE)
			{
				float Delta = (GLIDER_MAX_ROLL_ANGLE-Orientation.Z);
				Rotational_Speed.Z = Delta/100;
				Orientation.Z += Rotational_Speed.Z;
				Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->z,-Rotational_Speed.Z) * Body_Coordinates->BodyCoordinates_Orientation;
			}
			//Custom_Forces_Sum += Body_Coordinates->x*50;
			//Custom_Forces_Sum += Vector3D(0,1,0)*20;
		}
		/* No Right & No Left is pressed will return the rolling angle to its initial state */
		else
		{
			if(Right_WasPressed)
			{
				/* Was rotated to the right */
				if(Orientation.Z+INTERP_ROLL_ANGLE_N<=0)
				{
					Rotational_Speed.Z=INTERP_ROLL_ANGLE_N;
					Orientation.Z += INTERP_ROLL_ANGLE_N;
					Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->z,-Rotational_Speed.Z) * Body_Coordinates->BodyCoordinates_Orientation;
				}
				/* Was rotated to the left */
				else if(Orientation.Z-INTERP_ROLL_ANGLE_N>0)
				{
					Rotational_Speed.Z=INTERP_ROLL_ANGLE_N;
					Orientation.Z -= INTERP_ROLL_ANGLE_N;
					Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->z,Rotational_Speed.Z) * Body_Coordinates->BodyCoordinates_Orientation;
				}
				/* Reset the Right Key status */
				else
					Right_WasPressed=false;
			}
			if(Left_WasPressed)
			{
				/* Was rotated to the left */
				if(Orientation.Z-INTERP_ROLL_ANGLE_N>=0)
				{
					Rotational_Speed.Z=INTERP_ROLL_ANGLE_N;
					Orientation.Z -= INTERP_ROLL_ANGLE_N;
					Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->z,Rotational_Speed.Z) * Body_Coordinates->BodyCoordinates_Orientation;
				}
				/* Was rotated to the right */
				else if(Orientation.Z+INTERP_ROLL_ANGLE_N<0)
				{
					Rotational_Speed.Z=INTERP_ROLL_ANGLE_N;
					Orientation.Z += INTERP_ROLL_ANGLE_N;
					Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->z,-Rotational_Speed.Z) * Body_Coordinates->BodyCoordinates_Orientation;
				}
				/* Reset the Left Key status */
				else
					Left_WasPressed=false;
			}
		}
		
		
		/*Plane3D P(Position, Vector3D(0,1,0));
		Point3D ProjPt;
		ProjPt = Position + Velocity;
		ProjPt.ProjectionOnPlane(P,ProjPt);
		
		Vector3D Proj_Vel(ProjPt.X-Position.X, ProjPt.Y-Position.Y, ProjPt.Z-Position.Z);
		float Proj_V_Length = Proj_Vel.GetLength();*/
////////////////////////
		/* Getting the current rotation of the front wheel */
		float Alpha = Orientation.Z;
		/* Converting the angle to radians */
		Alpha = Alpha*PI_OVER_ONEEIGHTY;

		/* If the front is rotated */
		if(Alpha)
		{
			/* Apply a magic number for the rotation */
			float Angular_Damp=0.1f;
			
			/* Compute the circle that the bicycle will describe */
			/* compute the radius --> R = D/WheelAngle */
			float R =(15/sinf(Alpha));
			/* compute angular velocity of the bicycle 
			--> AV = V/R */
			float Angular_V = (Speed)/R;
			/* converting the angle to degrees */
			Angular_V = (Angular_V*ONEEIGHTY_OVER_PI)*Angular_Damp;

			/* Rotating the bicycle */
			Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Vector3D(0,1,0),Angular_V)*Body_Coordinates->BodyCoordinates_Orientation;
		}
		
	}
	/* ---------------------------------- BICYCLE CONTROLS & BEHAVIOR ---------------------------------- */
	else
	{
		//if(Speed>13)
		//	GliCycleState=Glider_State;

		if(Orientation.X + GLIDER_INTERP_PITCH_ANGLE_N<0)
		{
			Orientation.X += GLIDER_INTERP_PITCH_ANGLE_N;
			Rotational_Speed.X=GLIDER_INTERP_PITCH_ANGLE_N;
			Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->x,Rotational_Speed.X) * Body_Coordinates->BodyCoordinates_Orientation;
		}
		else if(Orientation.X)
		{
			Rotational_Speed.X = Orientation.X;
			Orientation.X-=Rotational_Speed.X;
			Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->x,-Rotational_Speed.X) * Body_Coordinates->BodyCoordinates_Orientation;
		}
		/* Only Right is pressed will roll by an angle to allow the it to strafe right */
		if(Keyboard::IsPressed(KeyboardKeyL))
		{
			Right_WasPressed=true;
			Left_WasPressed=false;
			if(Orientation.Z>-GLIDER_MAX_ROLL_ANGLE)
			{	
				float Delta = (-GLIDER_MAX_ROLL_ANGLE-Orientation.Z);
				Rotational_Speed.Z = (Delta*Speed/10)/100;
				Orientation.Z += Rotational_Speed.Z;
				Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->z,-Rotational_Speed.Z) * Body_Coordinates->BodyCoordinates_Orientation;
			}
		}
		/* Only Left is pressed will roll by an angle to allow the it to strafe left */
		else if(Keyboard::IsPressed(KeyboardKeyJ))
		{
			Right_WasPressed=false;
			Left_WasPressed=true;
			if(Orientation.Z<GLIDER_MAX_ROLL_ANGLE)
			{
				float Delta = (GLIDER_MAX_ROLL_ANGLE-Orientation.Z);
				Rotational_Speed.Z = (Delta*Speed/10)/100;
				Orientation.Z += Rotational_Speed.Z;
				Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->z,-Rotational_Speed.Z) * Body_Coordinates->BodyCoordinates_Orientation;
			}
		}
		/* No Right & No Left is pressed will return the rolling angle to its initial state */
		else
		{
			if(Right_WasPressed)
			{
				/* Was rotated to the right */
				if(Orientation.Z+INTERP_ROLL_ANGLE_N<=0)
				{
					Rotational_Speed.Z=INTERP_ROLL_ANGLE_N;
					Orientation.Z += INTERP_ROLL_ANGLE_N;
					Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->z,-Rotational_Speed.Z) * Body_Coordinates->BodyCoordinates_Orientation;
				}
				/* Was rotated to the left */
				else if(Orientation.Z-INTERP_ROLL_ANGLE_N>0)
				{
					Rotational_Speed.Z=INTERP_ROLL_ANGLE_N;
					Orientation.Z -= INTERP_ROLL_ANGLE_N;
					Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->z,Rotational_Speed.Z) * Body_Coordinates->BodyCoordinates_Orientation;
				}
				/* Reset the Right Key status */
				else
					Right_WasPressed=false;
			}
			if(Left_WasPressed)
			{
				/* Was rotated to the left */
				if(Orientation.Z-INTERP_ROLL_ANGLE_N>=0)
				{
					Rotational_Speed.Z=INTERP_ROLL_ANGLE_N;
					Orientation.Z -= INTERP_ROLL_ANGLE_N;
					Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->z,Rotational_Speed.Z) * Body_Coordinates->BodyCoordinates_Orientation;
				}
				/* Was rotated to the right */
				else if(Orientation.Z+INTERP_ROLL_ANGLE_N<0)
				{
					Rotational_Speed.Z=INTERP_ROLL_ANGLE_N;
					Orientation.Z += INTERP_ROLL_ANGLE_N;
					Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->z,-Rotational_Speed.Z) * Body_Coordinates->BodyCoordinates_Orientation;
				}
				/* Reset the Left Key status */
				else
					Left_WasPressed=false;
			}
		}

		/* Resetting the default brake value */
		Brake=1;
		/* Applying a force equal to the weight of the glicycle to oppose the gravity */
		Custom_Forces_Sum += Vector3D(0,1,0)*Weight;

		/* Getting the current rotation of the front wheel */
		float Alpha = Orientation.Z;//(Children[Front_Wheel]->Orientation.Y);
		/* Converting the angle to radians */
		Alpha = Alpha*PI_OVER_ONEEIGHTY;

		/* If the front is rotated */
		if(Alpha)
		{
			//float Angular_Damp = 1;
			//if(Speed>5)
			float Angular_Damp=0.4f;
			
			//if(Speed)
			//	Angular_Damp = 1/10*Speed;
			
			/* Compute the circle that the bicycle will describe */
			/* compute the radius --> R = D/WheelAngle */
			float R =(FRONT_BACK_WHEELS_DISTANCE/sinf(Alpha));
			/* compute angular velocity of the bicycle 
				--> AV = V/R */
			float Angular_V = (Speed)/R;
			/* converting the angle to degrees */
			Angular_V = (Angular_V*ONEEIGHTY_OVER_PI)*Angular_Damp;
		
			/* Getting the center of the circle */
			//float d = R*cosf(Alpha);
			//Vertex Rotation_Center = Children[Back_Wheel]->Position - Children[Back_Wheel]->Body_Coordinates->x*d;

			///WHEN ROLLING U ARE MODIFYING THE Y Axis which affects the wheels rotation
			/*if(Right_WasPressed)
			{
				if(Orientation.Z>-15)
				{
					Orientation.Z += Angular_V*0.2f;
					Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->z,-Angular_V*0.2f)*Body_Coordinates->BodyCoordinates_Orientation;
				}
			}
			if(Left_WasPressed)
			{
				if(Orientation.Z<15)
				{
					Orientation.Z += Angular_V*0.2f;
					Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->z,-Angular_V)*Body_Coordinates->BodyCoordinates_Orientation;
				}
			}*/

			/* Rotating the bicycle */
			Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Vector3D(0,1,0),Angular_V)*Body_Coordinates->BodyCoordinates_Orientation;
		}

		/* Setting the velocity vector to be the current bicycle direction */
		Plane3D P(Position, Vector3D(0,1,0));
		Point3D ProjPt;
		ProjPt = Position + Body_Coordinates->z;
		ProjPt.ProjectionOnPlane(P,ProjPt);
		//Velocity = Body_Coordinates->z*Speed;
		Vector3D ProjVect = ProjPt-Position;	ProjVect.SetNormalize();
		Velocity = (ProjVect)*Speed;
		
		/* Doing the pedaling force */
		if(Keyboard::IsPressed(KeyboardKeyI))
			Custom_Forces_Sum += Body_Coordinates->z*BICYCLE_TRACTION_RATIO;
		/* Applying the brake */
		else if(Keyboard::IsPressed(KeyboardKeyK))
			Custom_Forces_Sum -= Body_Coordinates->z*Speed*BICYCLE_BRAKE_RATIO;

		/* Taking the delta distance traveled between 2 consecutinve
			loops to be able to rotate the wheels accordingly */
		Delta_Distance = Old_Position.Distance(Position);
		/* Saving the old position */
		Old_Position=Position;
	}
}

/* Function to draw the cross hair */
void MainCharacter::Draw_CrossHair (void)
{
	glDisable(GL_TEXTURE_2D);
	glColor4f(1,0,0,1);

	Renderer *renderer = app.GetRenderer();

	unsigned short Indices [] = {0,1, 1,2, 2,3, 3,0,  0,4, 1,5, 2,6, 3,7,  4,5, 5,6, 6,7, 7,4};
	renderer->SetIndexBuffer(Indices, 24);

	Vertex Vertices[8];
	Vertices[0] = CrossHair.ptA*TO_UNIT;	Vertices[4] = CrossHair.ptE*TO_UNIT;
	Vertices[1] = CrossHair.ptB*TO_UNIT;	Vertices[5] = CrossHair.ptF*TO_UNIT;
	Vertices[2] = CrossHair.ptC*TO_UNIT;	Vertices[6] = CrossHair.ptG*TO_UNIT;
	Vertices[3] = CrossHair.ptD*TO_UNIT;	Vertices[7] = CrossHair.ptH*TO_UNIT;
	renderer->SetVertexBuffer(Vertices);	
	renderer->SetTextureCoordBuffer(0);
	renderer->SetWorldTransformation(Matrix3D::GetIdentity());
	renderer->SetNormalBuffer(0);

	renderer->SetPrimitivesType(PrimitiveTypeLineList);
	renderer->Draw();

	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
}

void MainCharacter::Draw_MainCharacter (void)
{
	Renderer *renderer = app.GetRenderer();

	renderer->SetActiveTextureUnit(0);
	/* If we are testing the engine --> Draw Body coordinates ... */
	if(TEST_ENGINE)
	{
		//Draw_BodyCoordinates();
		Draw_CrossHair();
		//Draw_BoundingBox();
	}
	Render(renderer);
}

/* Function to check whether there is collision between the character and an item */
bool MainCharacter::Collision_WithItem (Object *item)
{
	if(BBox.World_BBox.ClassifyToBox(item->BBox.World_BBox)==INTERSECT)
		return true;
	return false;
}

/* Function to update the object physics properties */
void MainCharacter::Update_Physics (void)
{
	/* Update the Position, orientation */
	Update_RigidBody();
	/* Get the Final_WorldTransformation & the Parent_ObjectMatrix (without the scale) after getting the position ... */
	Update_ObjectMatrix(Final_ObjectTransformation);
	/* Update the body coordinates */
	Update_ObjectBodyCoordinates();
	/* Update the children */
	//Update_PhysicsBoundings(Final_ObjectTransformation);
	/* Update the character's children + cross hair */
	Update_Character_Objects();	
	/* Update the character's bounding box */
	Update_ObjectBoundingBox(Final_ObjectTransformation);
}