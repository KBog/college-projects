	
		
// NETWORK HEADERS AND LIBRARIES//

#ifndef _NETWORK_H
#define _NETWORK_H

#include <windows.h>
#include <mmsystem.h>
#include <tchar.h>
#include <stdio.h> 
#include <stdarg.h>
#include <dplay8.h>
#include "dxutil.h"
#include <d3dx8.h>
#include "TestRenderApp.h"
#include "Test.h"
#include "Game.h"

#define PACKET_TYPE_POSITION	4

class	PacketGeneric
{
	public:
		DWORD	dwType;
		DWORD	dwSize;
};

class	PacketCharacter : public PacketGeneric
{
	public:
		Point3D position;
		Point3D scale;
		BodyCoordinates Body_Coordinates;
		vector<MainCharacter_Part *> Children;
};


class NetworkS
{
	public:
		static int iAddPlayer(DPNID dpid, D3DXVECTOR3 pos, float fRot, char *szName);
		static void vUpdateNetwork(void);
		static int iGetPlayerID(DPNID dpid);
		static HRESULT hrInitializeDirectPlay( HWND hWindow );
		static HRESULT	hrSendPeerMessage( DPNID player, DWORD dwMessageType, PVOID pMsgBuffer );
		static HRESULT WINAPI DirectPlayMessageHandler( PVOID pvUserContext, DWORD dwMessageId, PVOID pMsgBuffer );
		static HRESULT	hrHostGame( HWND hwnd );
		static HRESULT	hrJoinGame( HWND hWnd );
		static HRESULT	hrCreatePlayer( PVOID pvUserContext, PVOID pMsgBuffer );
		static HRESULT	hrDestroyPlayer( PVOID pvUserContext, PVOID pMsgBuffer );

		static DPNID SecondPlayerId;
};

#endif