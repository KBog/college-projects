/*
- The glider must stole at a certain angle (how ?)
- Ask Nakh about Impact Collision
- Ask About Normal Reaction (floor)
- Ask about collision with other object --> How to do the change in momentum
- Do the List of pointers in the each object for collision
- Adjust the Constraint Angles !!!!
- Do the CollisionDetection + CollisionResponse (2Fctions, then store the collided vertices & use:
																1- Apply On each vertex the force (Normal*Mag) Mag=Mag/Number of collided vertices
																2- Apply on the average point the force)
- Do the Area to have RigidBody List (render)
- Test 2 Objects IDPart + BBoxes	(We can use 2 BBox Classification coz we don't have fast objects, only the bullet (ray))
- Do One Virtual function for each character
- Do Collision BSP Save
- Do each Object will have its own BSP (area will remove the AreaBSP) && Transform it at runtime
- Do Object Culling against frustrum (do the hierarchy function)
*/ 

#include ".\test.h"

#include "TestRenderApp.h"
#include "MainCharacter.h"
#include "Camera.h"
#include "Game.h"
#include "time.h"
#include "KeyboardBoss.h"
#include "GTime.h"
#include "Network.h"
#include "bass.h"
#include "FuzzyCamera.h"

extern MyApplication app;
extern HWND g_HWND;

Vector3D Reflection;
Point3D InterPoint;

using namespace std;

Game *Game_Engine;
MainCharacter *GliCycle, *GliCycle2;
Camera *Main_Camera;
KeyboardBoss *Keyboard_Boss;
GameTime *Game_Time;
Camera_Fuzzy fuzzy_camera(3);

float Slow_Motion=1;
float Glider_SlowMotion=1;
#define WORLD_SLOWMOTION 0.3f
#define GLIDER_SLOWMOTION 0.1f

Vector3D TEST_AOA;

extern int NumLeafOctant;
extern float AngleRange;
extern float DistanceRange;

/* Game Constructor (here all the globals must be allocated */
Test::Test(void)
{
	Game_Time= new GameTime;
	Game_Time->Set_Timer(3);
	//Game_Time->Convert_StrToTime("12:44:12");

	/* Global Variables Creation */
	Game_Engine = new Game(Create_L1());
	GliCycle = new MainCharacter(app.GetDirectory() + "Resources\\Byc.3DS","NO_3DS",3,100,Point3D(3.75f,3.75f,12.5f),6.25f,
				Point3D(-1,0,15),new BodyCoordinates,Vector3D(0,-90,0),Vector3D(0,0,0),Point3D(1,1,1), 1.25f, ROH_AIR, Vector3D(0,0,0),OFF,new Constraint(Vector3D(0,0,0),Vector3D(0,0,0),Vector3D(0,0,0), Vector3D(-0,0,-0)));
	GliCycle2 = new MainCharacter(app.GetDirectory() + "Resources\\Byc.3DS","NO_3DS",3,100,Point3D(3.75f,3.75f,12.5f),6.25f,
				Point3D(-2,0,-5),new BodyCoordinates,Vector3D(0,-90,0),Vector3D(0,0,0),Point3D(1,1,1), 1.25f, ROH_AIR, Vector3D(0,0,0),OFF,new Constraint(Vector3D(0,0,0),Vector3D(0,0,0),Vector3D(0,0,0), Vector3D(-0,0,-0)));

	/*GliCycle = new MainCharacter(app.GetDirectory() + "Resources\\Back_Wheel.3DS",3,100,Point3D(3.75f,3.75f,12.5f),6.25f,
				Point3D(0,0,0),new BodyCoordinates,Vector3D(0,0,0),Vector3D(0,0,0),Point3D(1,1,1), Point3D(0,0,0), 0.8f, 1.29f, Material_Properties(1,0.5f,0.3f,15),Vector3D(0,0,0),Vector3D(0,0,0),new Physics_Sphere(Point3D(0,0,0),Material_Properties(0.7f),0.25f),PhysicsSphere_Type,1,3,ON,1);*/
	Main_Camera = new Camera(new BodyCoordinates, Point3D(0,-5,38), 0.025f);


	//Keyboard_Boss = new KeyboardBoss;
	//Keyboard_Boss->Attach_Manager(new KeyboardManager(app.GetDirectory() + "Resources\\Char_Cur.3DS",Point3D(-0,0,-4.5f),new BodyCoordinates,Vector3D(0,180,0),Vector3D(0,1,0),Point3D(1,1,1),0.25f,0.3f,10,2,ManagerNormal_Type,HUDRendering_Type));
	//Keyboard_Boss->Attach_Manager(new KeyboardManager(app.GetDirectory() + "Resources\\Char_Cur.3DS",Point3D(-0,-2,-4.5f),new BodyCoordinates,Vector3D(0,180,0),Vector3D(0,1,0),Point3D(1,1,1),0.25f,0.3f,10,2,ManagerNormal_Type,WorldRendering_Type));

	if( (NetworkS::hrInitializeDirectPlay(g_HWND)) == -1 )
	{
		MessageBox( g_HWND, "DirectPlay Error", "Unable to initialize Direct Play.", MB_ICONERROR );
		exit(1);
	}
	MessageBox( g_HWND, "I, K, J and L to control the ship", "Controls Info", MB_OK);
}

/* Game Destructor (here all the globals must be deleted */
Test::~Test(void)
{
	delete Keyboard_Boss;
	delete GliCycle;
	delete Game_Engine;
	BASS_Stop();
	BASS_Free();
}

/* Handling Stage */
void Test::Update()
{
	//char arr[1000];
	string s=FrameRateController::GetFrameRateAsString();
	float V_Length = GliCycle->Velocity.GetLength();

	string tmpS, tmpS2;
	tmpS= Game_Time->Get_TimeStr();
	tmpS2= Game_Time->Get_TimerStr();
	//sprintf(arr, "%s - Timer:%s    X:%f Y:%f Z:%f ||| Pos = X:%f | Y:%f | Z:%f | VX:%f | VY:%f | VZ:%f | V:%f | OZ:%f | %s", tmpS.c_str(),tmpS2.c_str(),TEST_AOA.X, TEST_AOA.Y, TEST_AOA.Z, GliCycle->Position.X, GliCycle->Position.Y, GliCycle->Position.Z, GliCycle->Velocity.X, GliCycle->Velocity.Y, GliCycle->Velocity.Z, V_Length, GliCycle->Orientation.Z, s.c_str());
	//sprintf(arr, "%s - Timer:%s    X:%f Y:%f Z:%f ||| X:%f | Y:%f | Z:%f | VX:%f | VY:%f | VZ:%f | V:%f | OZ:%f | %s", tmpS.c_str(),tmpS2.c_str(),TEST_AOA.X, TEST_AOA.Y, TEST_AOA.Z, GliCycle->Position.X, GliCycle->Position.Y, GliCycle->Position.Z, GliCycle->Velocity.X, GliCycle->Velocity.Y, GliCycle->Velocity.Z, V_Length, GliCycle->Orientation.Z, s.c_str());
	//sprintf(arr, "FPS: %s - Leafs: %i", s.c_str(), NumLeafOctant);
	//sprintf(arr, "%s - %f", s.c_str(),V_Length);
	//app.SetWindowTitle(arr);

	char temp[500];
	//sprintf (temp, "%s -- Reflection= X:%f Y:%f Z:%f  ||  InterPoint= X:%f Y:%f Z:%f", FrameRateController::GetFrameRateAsString().c_str(),Reflection.X,Reflection.Y,Reflection.Z, InterPoint.X, InterPoint.Y, InterPoint.Z);
	//sprintf(temp,"%f -- %f",AngleRange,DistanceRange);
	//sprintf(temp, "GliCycle: X:%f, Y:%f, Z:%f, Camera: X:%f, Y:%f, Z:%f", 
		//GliCycle->Position.X, GliCycle->Position.Y, GliCycle->Position.Z,
		//Main_Camera->Position.X, Main_Camera->Position.Y, Main_Camera->Position.Z);

	//app.SetWindowTitle(string (temp));

	/*if(Game_Time->Mission_Accomplished)
	{
		MessageBox(g_HWND, "MissionAccomplished","",0); 
		Game_Time->Mission_Accomplished= false;
	}*/

	/*if(Keyboard::IsTriggered(KeyboardKeyHome))
	{
		GliCycle->Apply_Force(Force(GliCycle->Position + 
	}*/

	static bool Update_Engine=ON;
	if(Keyboard::IsTriggered(KeyboardKeyP))
	{
		if(Update_Engine)
			Update_Engine=OFF;
		else
			Update_Engine=ON;
	}
	if(Keyboard::IsPressed(KeyboardKeyM))
	{
		Slow_Motion=1;
		Glider_SlowMotion=1;
	}
	else if(Keyboard::IsPressed(KeyboardKeyN))
	{
		Slow_Motion=WORLD_SLOWMOTION;
		Glider_SlowMotion=GLIDER_SLOWMOTION;
	}
	if(Keyboard::IsTriggered(KeyboardKeyB))
	{
        static bool wireFrame;
		wireFrame = !wireFrame;
		FillingState fs;
		if(wireFrame)
			fs.FillMode = FillModeWireframe;
		else
			fs.FillMode = FillModeSolid;
		app.GetRenderer()->SetState(fs);
	}

	if(Keyboard::IsTriggered(KeyboardKeyY))
	{
		GliCycle->Apply_Force(Force(GliCycle->Position + GliCycle->Body_Coordinates->z,Vector3D(0,0,-1000)));
	}
	GliCycle->GliCycleState=Bicycle_State;

	//if(NetIsOn==false)
	//{
	//	if(Keyboard::IsTriggered(KeyboardKeyH))
	//	{
	//		NetworkS::hrHostGame(g_HWND);
	//		NetIsOn= true;
	//	}
	//	else if(Keyboard::IsTriggered(KeyboardKeyJ))
	//	{
	//		NetworkS::hrJoinGame(g_HWND);
	//		NetIsOn= true;
	//	}
	//}
	//else
	//{
	//	//there is a network:
	//	if(Keyboard::IsTriggered(KeyboardKeyJ))
	//	{
	//		PacketCharacter pp;
	//		pp.dwType = PACKET_TYPE_POSITION;
	//		pp.dwSize = sizeof(PacketCharacter);

	//		void *packet = (VOID *)&pp;

	//		NetworkS::hrSendPeerMessage(NetworkS::SecondPlayerId,PACKET_TYPE_POSITION,packet);
	//	}
	//}

	//Camera::Update_AccordingToBody(GliCycle);
	Point3D GliderBackPos = GliCycle->Position-GliCycle->Body_Coordinates->z*3 + GliCycle->Body_Coordinates->y*1;

	float AngleAdjust=0, NewSpeed=0;
	fuzzy_camera.GetAdjustment_AngleSpeed(Main_Camera->Position,Main_Camera->Body_Coordinates->z,GliderBackPos,GliCycle->Body_Coordinates->z,AngleAdjust,NewSpeed);
	Main_Camera->Speed = NewSpeed;
	Main_Camera->Body_Coordinates->BodyCoordinates_Orientation= Matrix3D::GetRotateArbitraryAxis(Main_Camera->Body_Coordinates->y,AngleAdjust) * Main_Camera->Body_Coordinates->BodyCoordinates_Orientation;

	Vector3D CamTarget = GliderBackPos-Main_Camera->Position;	CamTarget.SetNormalize();
	Main_Camera->Position += CamTarget*Main_Camera->Speed;

	Game_Engine->Update(Update_Engine);
	NetworkS::vUpdateNetwork();
}

/* Rendering Stage */
void Test::Render()
{
	Game_Engine->Render();
}