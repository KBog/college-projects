#include "Network.h"

// #DEFINES & Constants
//----------------------
#define MAX_PLAYERS		2


// DPlay
#define TIMERID_CONNECT_COMPLETE    1
GUID	DP_SPACEPIRATES = { 0x74171a50, 0xe7c0, 0x4257, { 0xa7, 0x8c, 0x14, 0xdb, 0x54, 0xaf, 0x31, 0xa0 } };
#define PACKET_TYPE_GENERIC		0
#define PACKET_TYPE_CHAT		1
#define PACKET_TYPE_ANGLE		2
#define PACKET_TYPE_VELOCITY	3

#define PACKET_TYPE_BULLET		5
#define PACKET_TYPE_SCORE		6	
// D3D
#define D3DFVF_MYVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define DegToRad(deg) ((deg/180.0f)*PI)
#define VIDEODEVICEERROR_NOCOMPATIBLE	0x82000001
#define VIDEODEVICEERROR_NOWINDOWED		0x82000002
// DInput
#define KEYBOARD_BUFFERSIZE			10
#define INPUTERROR_NODI				0x81000001
#define INPUTERROR_NOKEYBOARD		0x81000002
#define INPUTERROR_KEYBOARDEXISTS	0x81000003

// Structures & Classes
//----------------------
struct PLAYER_INFORMATION
{
	bool		bActive;
    DPNID		dpnidPlayer;
	D3DXVECTOR3 vecCurPos;
	D3DXVECTOR3 vecLastPos;
	double		fVelocity;
	long		lScore;
	int			iFrame;
	float		fRot;
    char		szPlayerName[32];
};


DWORD			dwUpdateTime = timeGetTime();


extern MainCharacter *GliCycle;
extern MainCharacter *GliCycle2;
extern Game *Game_Engine;


//Globals
char			szSystemMessage[256];
int				g_iCommandMode = 0;
char			szCommand[256];


// Direct Play Objects
//---------------------
IDirectPlay8Peer*       g_pDP = NULL;		//Creating a peer session pointer
IDirectPlay8Address*    g_pDeviceAddress = NULL;
IDirectPlay8Address*    g_pHostAddress = NULL;
DPNHANDLE				g_hConnectAsyncOp = NULL;
DPNID					g_dpnidLocalPlayer = 0;
PLAYER_INFORMATION		PlayerInfo[MAX_PLAYERS];
int						g_iMyPlayerId = 0;
LONG					g_lNumberOfActivePlayers = 0;
BOOL					bHost = 0;



// Multi-Threading Variables
//---------------------------
HANDLE                  g_hConnectCompleteEvent = NULL;
HRESULT					g_hrConnectComplete;
CRITICAL_SECTION        g_csModifyPlayer;



DPNID NetworkS::SecondPlayerId = -1;



HRESULT NetworkS::hrInitializeDirectPlay( HWND hWindow ) 
{
	HRESULT	hReturn;
	
	// Initialize COM
	hReturn = CoInitialize( NULL );
	if( FAILED(hReturn) ) {
		MessageBox( hWindow, "Error Initializing COM", "DirectPlay Error", MB_ICONERROR );
		return hReturn;
	}

	// Initialize critical sections for multi-threading
	InitializeCriticalSection( &g_csModifyPlayer );
	
	// Create IDirectPlay8Peer Object
	if( FAILED( hReturn = CoCreateInstance( CLSID_DirectPlay8Peer, 
		NULL, 
		CLSCTX_INPROC_SERVER, 
		IID_IDirectPlay8Peer, 
		(LPVOID*) &g_pDP ) ) )
        MessageBox( hWindow, "Can't Create DPlayPeer", "DirectPlay Error", MB_ICONERROR );

	// Init IDirectPlay8Peer Message Handler
	if( FAILED( hReturn = g_pDP->Initialize( NULL, DirectPlayMessageHandler, 0 ) ) ) {
		MessageBox( hWindow, "Failed to Message Handler", "DirectPlay Error", MB_ICONERROR );
		return -1;
	}

	// Create a device address
	hReturn = CoCreateInstance( CLSID_DirectPlay8Address, NULL,CLSCTX_INPROC_SERVER, IID_IDirectPlay8Address, (LPVOID*) &g_pDeviceAddress );
	if( FAILED(hReturn) ) {
		MessageBox( hWindow, "Failed to Create Device", "CoCreateInstance()", MB_ICONERROR );
		return -1;
	}

	// Set our service provider to TCP/IP
	if( FAILED( hReturn = g_pDeviceAddress->SetSP( &CLSID_DP8SP_TCPIP ) ) ) {
		MessageBox( hWindow, "Failed to SetSP() for Device Address", "Invalid Param", MB_ICONERROR );
		return -1;
	}

	// Create a host address
	hReturn = CoCreateInstance( CLSID_DirectPlay8Address, NULL,CLSCTX_INPROC_SERVER, IID_IDirectPlay8Address, (LPVOID*) &g_pHostAddress );
	if( FAILED(hReturn) ) {
		MessageBox( hWindow, "Failed to Create Host Address()", "Invalid Param", MB_ICONERROR );
		return -1;
	}
	// Set the host address to TCP/IP
	if( FAILED( hReturn = g_pHostAddress->SetSP( &CLSID_DP8SP_TCPIP ) ) ) {
		MessageBox( hWindow, "Failed to SetSP() for Host Address", "Invalid Param", MB_ICONERROR );
		return -1;
	}

	// Create connection complete event for later use
	g_hConnectCompleteEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

	return S_OK;
}



	//-----------------------------------------------------------------------------
// Name: DirectPlayMessageHandler()
// Desc: Handles all Direct Play messages
//-----------------------------------------------------------------------------
HRESULT WINAPI NetworkS::DirectPlayMessageHandler( PVOID pvUserContext, DWORD dwMessageId, PVOID pMsgBuffer )
{
	HRESULT				hReturn = S_OK;
	PacketCharacter		*pacChar;
	int					iPlayerID;
	
	switch( dwMessageId ) 
	{
		case DPN_MSGID_CREATE_PLAYER:
		{
			hrCreatePlayer(pvUserContext,pMsgBuffer);
			break;
		}
		
		case DPN_MSGID_DESTROY_PLAYER:
		{
			hrDestroyPlayer(pvUserContext,pMsgBuffer);
			break;
		}
		
		case DPN_MSGID_HOST_MIGRATE:
		{
			PDPNMSG_HOST_MIGRATE pHostMigrateMsg;
			pHostMigrateMsg = (PDPNMSG_HOST_MIGRATE)pMsgBuffer;
			
			// Check to see if we are the new host
			if( pHostMigrateMsg->dpnidNewHost == g_dpnidLocalPlayer ) {
			}
			break;
		}
		
		case DPN_MSGID_TERMINATE_SESSION:
		{
			PDPNMSG_TERMINATE_SESSION pTerminateSessionMsg;
			pTerminateSessionMsg = (PDPNMSG_TERMINATE_SESSION)pMsgBuffer;
			break;
		}
		
		case DPN_MSGID_RECEIVE:
		{
			PDPNMSG_RECEIVE pReceiveMsg;
			PacketGeneric	*PGen;

			pReceiveMsg = (PDPNMSG_RECEIVE)pMsgBuffer;
			
			// Cast it to a generic packet so we can check the type
			PGen = (PacketGeneric*)pReceiveMsg->pReceiveData;
			
			if( PGen->dwType == PACKET_TYPE_POSITION ) {
				// Convert the packet to a chat packet
				
				pacChar = (PacketCharacter*)pReceiveMsg->pReceiveData;
				iPlayerID = iGetPlayerID(pReceiveMsg->dpnidSender);
				EnterCriticalSection( &g_csModifyPlayer );
				//Object Pos
				GliCycle2->Position= pacChar->position;
				GliCycle2->Scale = pacChar->scale;
				*GliCycle2->Body_Coordinates= pacChar->Body_Coordinates;
				//GliCycle2->Children = pacChar->Children;
				LeaveCriticalSection( &g_csModifyPlayer );
			}
		}
		
		case DPN_MSGID_CONNECT_COMPLETE:
		{
			PDPNMSG_CONNECT_COMPLETE pConnectCompleteMsg;
			pConnectCompleteMsg = (PDPNMSG_CONNECT_COMPLETE)pMsgBuffer;
			
			g_hrConnectComplete = pConnectCompleteMsg->hResultCode;
			SetEvent( g_hConnectCompleteEvent );
			break;
		}
	}
	
	return hReturn;
}

HRESULT	NetworkS::hrJoinGame( HWND hWnd )
{
	HRESULT					hReturn = S_OK;
	WCHAR					wszHostName[256];
	WCHAR					wszPeerName[256];
	char					szPeerName[256];
	char					szIP[256];
	DWORD					dwPort;
	DWORD					dwLength = 256;
	DPN_APPLICATION_DESC	dpnAppDesc;
	DPN_PLAYER_INFO			dpPlayerInfo;

	// Read some settings from the config file
	FILE *fp = fopen("config.txt","r");
	fgets(szIP,32,fp);
	szIP[strlen(szIP)-1] = '\0';
	fgets(szPeerName,32,fp);
	szPeerName[strlen(szPeerName)-1] = '\0';
	fclose(fp);

	// Set the peer info
	DXUtil_ConvertGenericStringToWide( wszPeerName, szPeerName );
	
	ZeroMemory( &dpPlayerInfo, sizeof(DPN_PLAYER_INFO) );
	dpPlayerInfo.dwSize = sizeof(DPN_PLAYER_INFO);
	dpPlayerInfo.dwInfoFlags = DPNINFO_NAME;
	dpPlayerInfo.pwszName = wszPeerName;
	
	// Make this a synchronous call
	if( FAILED( hReturn = g_pDP->SetPeerInfo( &dpPlayerInfo, NULL, NULL, DPNSETPEERINFO_SYNC ) ) ) {
		return -1;
	}
		
	// Prepare the application description
	ZeroMemory( &dpnAppDesc, sizeof( DPN_APPLICATION_DESC ) );
	dpnAppDesc.dwSize = sizeof( DPN_APPLICATION_DESC );
	dpnAppDesc.guidApplication = DP_SPACEPIRATES;

	// Set the IP
	DXUtil_ConvertGenericStringToWide( wszHostName, szIP );
	// Set the port number
	dwPort = 6000;

	// Add host name to address
	hReturn = g_pHostAddress->AddComponent(DPNA_KEY_HOSTNAME,wszHostName,((int)wcslen(wszHostName)+1)*sizeof(WCHAR),DPNA_DATATYPE_STRING);
	if( hReturn != S_OK ) {
		MessageBox( hWnd, "Failed to AddComponent()", "hrJoinGame()", MB_ICONERROR );
		return -1;
	}
	// Add port number to address
	hReturn = g_pHostAddress->AddComponent(DPNA_KEY_PORT,&dwPort,sizeof(DWORD),DPNA_DATATYPE_DWORD);
	if( hReturn != S_OK ) {
		MessageBox( hWnd, "Failed to AddComponent()", "hrJoinGame()", MB_ICONERROR );
		return -1;
	}

	// Connect to the session
	hReturn = g_pDP->Connect(	&dpnAppDesc,
								g_pHostAddress,    
								g_pDeviceAddress,
								NULL,
								NULL,
								NULL,	
								0,
								NULL,
								NULL, 
								&g_hConnectAsyncOp,
								NULL); // flags
	
	if( hReturn != E_PENDING && FAILED(hReturn) ) {
		return -1;
	}
	SetTimer( hWnd, TIMERID_CONNECT_COMPLETE, 100, NULL );

	return(hReturn);
}

HRESULT	NetworkS::hrHostGame( HWND hWindow )
{
	HRESULT					hReturn;
	char					szPeerName[256];
	char					szSessionName[256];
	WCHAR					wszPeerName[256];
	WCHAR					wszSessionName[256];
	DPN_APPLICATION_DESC	dnAppDesc;
	char					szIP[32];
	DWORD					dwLength = 256;
	DPN_PLAYER_INFO			dpPlayerInfo;
	DWORD					dwPort = 6000;
	
	// Read some settings from the config file
	FILE *fp = fopen("config.txt","r");
	fgets(szIP,16,fp);
	szIP[strlen(szIP)-1] = '\0';
	fgets(szPeerName,32,fp);
	szPeerName[strlen(szPeerName)-1] = '\0';
	fclose(fp);

	//
	// Setup our player information
	//
	DXUtil_ConvertGenericStringToWide( wszPeerName, szPeerName );
	ZeroMemory( &dpPlayerInfo, sizeof(DPN_PLAYER_INFO) );
	dpPlayerInfo.dwSize = sizeof(DPN_PLAYER_INFO);
	dpPlayerInfo.dwInfoFlags = DPNINFO_NAME;
	dpPlayerInfo.pwszName = wszPeerName;
	
	// Set us up to be non-asynchronous
	if( FAILED( hReturn = g_pDP->SetPeerInfo( &dpPlayerInfo, NULL, NULL, DPNSETPEERINFO_SYNC ) ) ) {
		MessageBox( hWindow, "Failed to SetPeerInfo()", "DirectPlay Error", MB_ICONERROR );
		return -1;
	}
	
	// Setup the application description
	sprintf(szSessionName,"%s's Game",szPeerName);
	DXUtil_ConvertGenericStringToWide( wszSessionName, szSessionName );
	
	ZeroMemory( &dnAppDesc, sizeof(DPN_APPLICATION_DESC) );
	dnAppDesc.dwSize			= sizeof(DPN_APPLICATION_DESC);
	dnAppDesc.guidApplication	= DP_SPACEPIRATES;
	dnAppDesc.pwszSessionName	= wszSessionName;
	dnAppDesc.dwMaxPlayers		= MAX_PLAYERS;
	dnAppDesc.dwFlags			= DPNSESSION_MIGRATE_HOST;
	
	// Set the port number
	dwPort = 6000;
	
	// Add port number to address
	hReturn = g_pDeviceAddress->AddComponent(DPNA_KEY_PORT,&dwPort,sizeof(DWORD),DPNA_DATATYPE_DWORD);
	if( hReturn != S_OK ) {
		MessageBox( hWindow, "Failed to AddComponent()", "hrHostGame()", MB_ICONERROR );
		return -1;
	}
	
	// Host the game
	hReturn = g_pDP->Host(	&dnAppDesc,               
		&g_pDeviceAddress,        
		1,                        
		NULL, 
		NULL,               
		NULL,                     
		NULL );
	if( FAILED( hReturn ) ) {
		if( hReturn == DPNERR_INVALIDPARAM ) 
			MessageBox( hWindow, "Failed to Host()", "Invalid Param", MB_ICONERROR );
		else if( hReturn == DPNERR_INVALIDDEVICEADDRESS  ) 
			MessageBox( hWindow, "Failed to Host()", "Invalid Device Address", MB_ICONERROR );
		else
			MessageBox( hWindow, "Failed to Host()", "DirectPlay Error", MB_ICONERROR );
		return -1;
	}
	
	// Let us know we are the host
	bHost = 1;

	sprintf(szSystemMessage,"Hosting");

	return hReturn;
}

HRESULT	NetworkS::hrCreatePlayer( PVOID pvUserContext, PVOID pMsgBuffer )
{
	HRESULT					hReturn = S_OK;
    PDPNMSG_CREATE_PLAYER	pCreatePlayerMsg;
	char					strName[256];
	DWORD					dwSize = 0;
	DPN_PLAYER_INFO			*pdpPlayerInfo = NULL;
	int						i;
		
	// Get a Create Message pointer to the buffer
	pCreatePlayerMsg = (PDPNMSG_CREATE_PLAYER)pMsgBuffer;
	
    // Get the peer info and extract its name
    hReturn = g_pDP->GetPeerInfo( pCreatePlayerMsg->dpnidPlayer, pdpPlayerInfo, &dwSize, 0 );
    if( FAILED(hReturn) && hReturn != DPNERR_BUFFERTOOSMALL ) {
        hReturn = -1;
	}
	else {
		// Allocate memory for the player info
		pdpPlayerInfo = (DPN_PLAYER_INFO*) new BYTE[ dwSize ];
		
		ZeroMemory( pdpPlayerInfo, dwSize );
		pdpPlayerInfo->dwSize = sizeof(DPN_PLAYER_INFO);
		// Load the player info into the newly allocated data
		hReturn = g_pDP->GetPeerInfo( pCreatePlayerMsg->dpnidPlayer, pdpPlayerInfo, &dwSize, 0 );
		if( FAILED(hReturn) ) {
			hReturn = -1;
		}
		else {
			EnterCriticalSection( &g_csModifyPlayer );
			
			// Convert player name to ANSI
			DXUtil_ConvertWideStringToGeneric( strName, pdpPlayerInfo->pwszName );
			// Check if we are adding ourselves
			if( pdpPlayerInfo->dwPlayerFlags & DPNPLAYER_LOCAL ) {
				g_dpnidLocalPlayer = pCreatePlayerMsg->dpnidPlayer;
				g_iMyPlayerId = iAddPlayer(g_dpnidLocalPlayer,D3DXVECTOR3(0.0f,0.0f,0.0f),0.0f,strName);
			}
			else 
			{
				// Add the player to our game				
				i = iAddPlayer(pCreatePlayerMsg->dpnidPlayer,D3DXVECTOR3(0.0f,0.0f,0.0f),0.0f,strName);
				SecondPlayerId = pCreatePlayerMsg->dpnidPlayer;
			}

			SAFE_DELETE_ARRAY( pdpPlayerInfo );
			
			// Update the number of active players in a thread safe way
			InterlockedIncrement( &g_lNumberOfActivePlayers );

			LeaveCriticalSection( &g_csModifyPlayer );
		}
	}
		
	return hReturn;
}
HRESULT	NetworkS::hrDestroyPlayer( PVOID pvUserContext, PVOID pMsgBuffer )
{
	PDPNMSG_DESTROY_PLAYER	pDestroyPlayerMsg;
	HRESULT					hReturn = S_OK;
	int						i;
	char					szOutput[256];
		
	// Get a Destroy Message pointer to the buffer
	pDestroyPlayerMsg = (PDPNMSG_DESTROY_PLAYER)pMsgBuffer;
	
	// Update the number of active players in a thread safe way
	InterlockedDecrement( &g_lNumberOfActivePlayers );

	EnterCriticalSection( &g_csModifyPlayer );

	// Remove Player from list
	for( i = 0 ; i < MAX_PLAYERS ; i++ ) {
		if( PlayerInfo[i].bActive ) {
			if( PlayerInfo[i].dpnidPlayer == pDestroyPlayerMsg->dpnidPlayer ) {
				PlayerInfo[i].bActive = 0;
				sprintf(szOutput,"<%s> Left The Game",PlayerInfo[i].szPlayerName);
				sprintf(szSystemMessage,szOutput);
				break;
			}
		}
	}
	
	LeaveCriticalSection( &g_csModifyPlayer );
	
	return(hReturn);
}
int NetworkS::iAddPlayer(DPNID dpid, D3DXVECTOR3 pos, float fRot, char *szName)
{
	int i;

	for( i = 0 ; i < MAX_PLAYERS ; i++ ) {
		if( !PlayerInfo[i].bActive ) {
			break;
		}
	}	
	// No free slots
	if( i == MAX_PLAYERS )
		return(-1);

	PlayerInfo[i].bActive = 1;
	PlayerInfo[i].dpnidPlayer = dpid;
	PlayerInfo[i].vecCurPos = pos;
	PlayerInfo[i].vecLastPos = pos;
	PlayerInfo[i].iFrame = 0;
	PlayerInfo[i].fRot = fRot;
	PlayerInfo[i].fVelocity = 0.0f;
	strcpy(PlayerInfo[i].szPlayerName,szName);
	return(i);
}

HRESULT	NetworkS::hrSendPeerMessage( DPNID player, DWORD dwMessageType, PVOID pMsgBuffer )
{
	DPNHANDLE		hAsync;
    DWORD			dwLength;
	DPN_BUFFER_DESC bufferDesc;
	PacketGeneric	*PGen;

	// Cast to a generic packet to get the size
	PGen = (PacketGeneric*)pMsgBuffer;
	dwLength = PGen->dwSize;

	// Return if an empty packet
	if( dwLength == 0 ) 
		return(0);

	bufferDesc.dwBufferSize = dwLength;
	bufferDesc.pBufferData  = (BYTE*)pMsgBuffer;

	// Broadcast message
	if( player == -1 )
		g_pDP->SendTo( DPNID_ALL_PLAYERS_GROUP, &bufferDesc, 1, 0, NULL, &hAsync, DPNSEND_NOLOOPBACK );
	// Send to a particular player
	else
		g_pDP->SendTo( /*PlayerInfo[player].dpnidPlayer*/player, &bufferDesc, 1, 0, NULL, &hAsync, 0 );

	
	return S_OK;
}

void NetworkS::vUpdateNetwork(void)
{
	void			*packet;
	PacketCharacter	pacChar;
	HRESULT			hReturn;

	//if( timeGetTime() > dwUpdateTime+100 ) {
		// Only send packets if other players are in the game
		if( g_lNumberOfActivePlayers > 1 ) 
		{
			// Send our world_position
			pacChar.dwSize = sizeof(PacketCharacter);
			pacChar.dwType = PACKET_TYPE_POSITION;
			pacChar.position = GliCycle->Position;
			pacChar.scale = GliCycle->Scale;
			pacChar.Body_Coordinates= *GliCycle->Body_Coordinates;
			//pacChar.Children = GliCycle->Children;
			packet = (VOID*)&pacChar;
			hReturn = hrSendPeerMessage(SecondPlayerId,PACKET_TYPE_POSITION,packet);
		}
		//dwUpdateTime = timeGetTime();
	//}
}

int NetworkS::iGetPlayerID(DPNID dpid)
{
	int i;

	// Remove Player from list
	for( i = 0 ; i < MAX_PLAYERS ; i++ ) {
		if( PlayerInfo[i].bActive ) {
			if( PlayerInfo[i].dpnidPlayer == dpid ) {
				return(i);
			}
		}
	}
	return(0);
}
