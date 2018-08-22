#include "TestRenderApp.h"
#include "Test.h"
#include "Game.h"
#include "Camera.h"
#include "MainCharacter.h"
#include "KeyboardInput.h"
#include "Network.h"
#include "sound.h"

extern HWND g_HWND;
bool NetIsOn= false;

MyApplication app;
extern MainCharacter *GliCycle, *GliCycle2;

extern Game *Game_Engine;
extern Camera *Main_Camera;
extern KeyboardManager *Keyboard_Manager;

// The order of these should match the order in which the samples are loaded
extern enum {sample_engine = 1,sample_carhorn};
// The order of these should match the order in which the channels are created
extern enum {channel_engine = 1,channel_carhorn};


void MyApplication::OnSetInitialDisplaySettings(DisplaySettings& displaySettings)
{
	displaySettings.WindowMode = WindowModeWindowed;
	displaySettings.Resolution = Resolution800X600;
	displaySettings.VSync = true;
}

void MyApplication::OnPreUpdate()
{
	FrameRateController::Start();
}

void MyApplication::OnPostUpdate()
{
	FrameRateController::End();
}

void MyApplication::OnInit()
{
	FrameRateController::SetFrameRate(100);

	//InitializeBASS();             // Initialize the BASS sound system
	//CreateSounds();               // Load the samples and create the channels

	//PlaySound ("back.WAV", NULL, SND_FILENAME | SND_ASYNC |SND_LOOP);

	//new Physics_Cylinder(Point3D(0,0,0),0.7f,0.25f,0.05f,Point3D(0,90,0)),PhysicsCylinder_Type,1
	/*new MainCharacter_FrontWheel(0.25f, app.GetDirectory() + "Resources\\Back_Wheel.3DS", Point3D(0,-8,-17),new BodyCoordinates,Vector3D(0,0,0),Vector3D(10,0,0),Point3D(1,1,1),Point3D(0,-8,-17),0.5f,1,Material_Properties(0.8f,0.01f,1,1),Vector3D(0,0,0),Vector3D(0,0,0),new Physics_Cylinder(Point3D(0,0,0),0.3f,0.01f),PhysicsCylinder_Type,1,0,1),
				new MainCharacter_BackWheel(0.25f, app.GetDirectory() + "Resources\\Back_Wheel.3DS", Point3D(0,-8,17),new BodyCoordinates,Vector3D(0,0,0),Vector3D(10,0,0),Point3D(1,1,1),Point3D(0,-8,-17),0.5f,1,Material_Properties(0.8f,0.01f,1,1),Vector3D(0,0,0),Vector3D(0,0,0),new Physics_Cylinder(Point3D(0,0,0),0.3f,0.01f),PhysicsCylinder_Type,1,0,1),*/
	t = new Test();
	//FrameRateController::GetFrameRateAsString()

	int fov = 60;
	float zNear=1.0f, zFar=70000.0f, windowWidth = 800, windowHeight = 600;
	GetRenderer()->SetProjectionTransformation(Matrix3D::GetPerspectiveAsOGL(fov, windowWidth/windowHeight, zNear, zFar));

	CullingState cs;
	cs.CullFace = CullFaceBack;
	cs.Status = true;
	GetRenderer()->SetState(cs);

	GetRenderer()->SetActiveTextureUnit(0);//for correcting an internal bug.

	if( (NetworkS::hrInitializeDirectPlay(g_HWND)) == -1 )
	{
		MessageBox( g_HWND, "DirectPlay Error", "Unable to initialize Direct Play.", MB_ICONERROR );
		exit(1);
	}
}

void MyApplication::OnExit()
{
    delete t;
	t=NULL;
}

void MyApplication::OnUpdate()
{	
	if(Keyboard::IsTriggered(KeyboardKeyEscape))
	{
        OnExit();
		Exit();
	}

	if(Keyboard::IsTriggered(KeyboardKeyF))
		GetRenderer()->ToggleWindowMode();

	//UpdateListener();  
	//Vector3D *s=new Vector3D;
	//s->X=0;
	//s->Y=0;
	//s->Z=0;
	//UpdateChannel(channel_engine,s, NULL, NULL);  // Update the engine sound
	//s->X=Main_Camera->Position.X;
	//s->Y=Main_Camera->Position.Y;
	//s->Z=Main_Camera->Position.Z;
 //   UpdateChannel(channel_carhorn, s, NULL, NULL);  // Update the car horn sound

	//if(Keyboard::IsTriggered(KeyboardKeyReturn))
	//	 PlayChannel(channel_carhorn);
	
	GetRenderer()->ClearBackBuffer(Color(0, 0, 0)); // if needed
	GetRenderer()->ClearDepthBuffer(1);

	//GetRenderer()->SetViewingTransformation(Matrix3D::GetLookAtAsOGL(Point3D(0,0,0),Point3D(0,0,500),Point3D(0,1,0)));
	
	if(t)
	{
		t->Update();
		t->Render();
	}

	//currently the raster graphics is not fast and have some problems, It should be avoided
	//GetRenderer()->BeginRasterGraphics(); 
	//GetRenderer()->DrawString(0, 0, FrameRateController::GetFrameRateAsString());         
	//GetRenderer()->EndRasterGraphics();
    
	GetRenderer()->SwapBuffers();
}

