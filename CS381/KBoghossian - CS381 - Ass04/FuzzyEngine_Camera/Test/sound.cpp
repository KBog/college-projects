#include "sound.h"

extern Camera *Main_Camera;


// Sound
int device;		                        // Selected device
BOOL lowqual;	                        // Low quality option flag
int SphereSector;                       // Current sector (leaf of bsp tree) of the sphere
int numSamples = 0;                     // The number of sound samples in the list
int numChannels = 0;                    // The number of channels in the list
LinkedList<SOUND_SAMPLE> SampleList;    // The list of samples
LinkedList<SOUND_CHANNEL> ChannelList;  // The list of channels

extern HWND g_HWND;

// The order of these should match the order in which the samples are loaded
enum {sample_engine = 1,sample_carhorn};
// The order of these should match the order in which the channels are created
enum {channel_engine = 1,channel_carhorn};


int SOUND_CHANNEL::Compare(const SOUND_CHANNEL& Channel)
{
	if (linkPosition < Channel.linkPosition)
        return smaller;
  	if (linkPosition > Channel.linkPosition)
        return bigger;
  	else
        return same;
}

int SOUND_SAMPLE::Compare(const SOUND_SAMPLE& Sample)
{
	if (linkPosition < Sample.linkPosition)
        return smaller;
  	if (linkPosition > Sample.linkPosition)
        return bigger;
  	else
        return same;
}
void InitializeBASS()
{
    /* Check that BASS 1.4 was loaded */
	if (BASS_GetVersion() != MAKELONG(1, 4))
		MessageBox(0, "BASS version 1.4 was not loaded", "Incorrect BASS.DLL", 0);

	/* Initialize the output device (syncs not used) */
	if (!BASS_Init(device, lowqual?22050:44100, BASS_DEVICE_3D, g_HWND))
		MessageBox(0, "Could not initialize output device", "Error", 0);

	/* Use meters as distance unit, real world rolloff, real doppler effect */
	BASS_Set3DFactors(1.0, 1.0, 1.0);

	/* Turn EAX off (volume=0.0), if error then EAX is not supported */
	BASS_SetEAXParameters(-1, 0.0, -1.0, -1.0);

    BASS_Start();	/* Start digital output */
}

void LoadSample(SOUND_SAMPLE* Sample)
{
    /* Load a sample from a file */
    Sample->hSample = BASS_SampleLoad(FALSE, Sample->name, 0, 0, Sample->max, Sample->flags);
   if (Sample->hSample)
   {
        BASS_SAMPLE sampleInfo;
        BASS_SampleGetInfo(Sample->hSample, &sampleInfo);

        sampleInfo.volume = Sample->volume;
        sampleInfo.mindist = Sample->mindist;
        sampleInfo.maxdist = Sample->maxdist;
        BASS_SampleSetInfo(Sample->hSample, &sampleInfo);

        Sample->linkPosition = ++numSamples;
        SampleList.Insert(Sample);
    }
    else
        MessageBox(g_HWND,"Could not load sound sample","Error",MB_OK|MB_ICONERROR);
}

void CreateChannel(int SampleNumber)
{
    SOUND_SAMPLE* Sample = SampleList.Get(SampleNumber);
    SOUND_CHANNEL* Channel = new SOUND_CHANNEL;
    Channel->hSample = Sample->hSample;
    Channel->linkPosition = ++numChannels;
    ChannelList.Insert(Channel);
}

void UpdateListener()
{
    BASS_3DVECTOR Player;
    BASS_3DVECTOR Bass_Front;
    BASS_3DVECTOR Bass_Top;
    BASS_3DVECTOR Bass_Velocity;

	Player.x = Main_Camera->Position.X;       // Get the player's position
    Player.y = Main_Camera->Position.Y;
    Player.z = Main_Camera->Position.Z;
	Vector3D Front = Main_Camera->Body_Coordinates->z;   // Get the player's front vector
	Bass_Front.x = -Front.X;                   // Reverse the front vector for OpenGL
    Bass_Front.y = -Front.Y;
    Bass_Front.z = -Front.Z;
    Bass_Top.x = 0;
    Bass_Top.y = 1;
    Bass_Top.z = 0;
    Bass_Velocity.x = 10;
    Bass_Velocity.y = 10;
    Bass_Velocity.z = 10;

    BASS_Set3DPosition(&Player, &Bass_Velocity, &Bass_Front, &Bass_Top);  // Make changes to the player position/orientation

	/* Apply the 3D changes */
	BASS_Apply3D();
}

void UpdateChannel(int ChannelNumber, Vector3D* Position, Vector3D* Orientation, Vector3D* Velocity)
{
    SOUND_CHANNEL* Channel = ChannelList.Get(ChannelNumber);

    if (Position)
    {
        Channel->position.x = Position->X;
        Channel->position.y = Position->Y;
        Channel->position.z = Position->Z;
    }
    if (Orientation)
    {
        Channel->orientation.x = Orientation->X;
        Channel->orientation.y = Orientation->Y;
        Channel->orientation.z = Orientation->Z;
    }
    if (Velocity)
    {
        Channel->velocity.x = Velocity->X;
        Channel->velocity.y = Velocity->Y;
        Channel->velocity.z = Velocity->Z;
    }

    /* If the channel's playing then update it's position */
    if (BASS_ChannelIsActive(Channel->hChannel))
        BASS_ChannelSet3DPosition(Channel->hChannel, &Channel->position, NULL, &Channel->velocity);

	/* Apply the 3D changes */
	BASS_Apply3D();
}

void PlayChannel(int ChannelNumber)
{
    SOUND_CHANNEL* Channel = ChannelList.Get(ChannelNumber);
    Channel->hChannel = BASS_SamplePlay3D(Channel->hSample, &Channel->position, NULL, &Channel->velocity);

    char temp[256];
    int result = BASS_ErrorGetCode();
    if (result)
    {
        sprintf(temp, "BASS error code No. %d", result);
        MessageBox(g_HWND, temp, "Error", MB_OK|MB_ICONERROR);
    }
}

VOID CALLBACK SoundTimerProc(HWND g_HWND, UINT uMsg, UINT idEvent, DWORD dwTime)
{
    SOUND_CHANNEL* Channel = ChannelList.Get((int)idEvent);
    PlayChannel((int)idEvent);
    if ((Channel->looped && !Channel->intermittent) || !Channel->looped)
        KillTimer(g_HWND, idEvent);
    if (Channel->looped && Channel->intermittent && Channel->random)
    {
        int randomtime = (rand()%(Channel->upperrand - Channel->lowerrand)) + Channel->lowerrand;
        SetTimer(g_HWND, idEvent, randomtime,	(TIMERPROC)SoundTimerProc);
    }
}

void CreateSounds()
{
    SOUND_SAMPLE* Sample;
    SOUND_CHANNEL* Channel;
    Vector3D Position;

	// engine sound
    Sample = new SOUND_SAMPLE;
    sprintf(Sample->name, "%s", "bip.wav");
    Sample->max = 10;
    Sample->volume = 100;
    Sample->mindist = 10.0;
    Sample->maxdist = 1000.0;
    Sample->flags =  BASS_SAMPLE_OVER_DIST | BASS_SAMPLE_LOOP | BASS_SAMPLE_3D | BASS_SAMPLE_VAM;
    LoadSample(Sample);           // Load the sample and put it in the sample list

    CreateChannel(sample_engine); // Create a new channel with the sample and put it in the channel list
    Channel = ChannelList.Get(channel_engine);
    Channel->looped = true;
    Channel->intermittent = false;
    Channel->random = false;
    Channel->interval = 2000;
    Channel->idEvent = (unsigned int)channel_engine;
    SetTimer(g_HWND, Channel->idEvent, Channel->interval, (TIMERPROC)SoundTimerProc);

// car horn sound      (played on a keystroke so we don't set a timer)
    Sample = new SOUND_SAMPLE;
    sprintf(Sample->name, "%s", "carhorn.wav");
    Sample->max = 10;
    Sample->volume = 100;
    Sample->mindist = 10.0;
    Sample->maxdist = 1000.0;
    Sample->flags = BASS_SAMPLE_OVER_DIST | BASS_SAMPLE_3D | BASS_SAMPLE_VAM;
    LoadSample(Sample);

    CreateChannel(sample_carhorn);
}