#include "SDK.h"
#include "VMT Hook.h"
#include "Utilities.h"
#include "Client.h"
#include "Memory Tools.h"
#include "Extra Functions.h"
#include "Model Render.h"
#include "Sound Engine.h"
//===================================================================================
HMODULE hmThisModule = NULL;
DWORD dwWeaponIDToAlias = NULL;
DWORD dwPredictionRandSeed = NULL;
DWORD dwSaveUserCmd0 = NULL;
DWORD dwSaveUserCmd1 = NULL;
//===================================================================================
// Base Interfaces
//===================================================================================
IBaseClientDLL*					g_pClient = NULL;
IMatSystemSurface*				g_pMatSurface = NULL;
vgui::ISurface*					g_pSurface = NULL;
IVEngineClient*					g_pEngine = NULL;
IEngineVGui*					g_pEngineVGUI = NULL;
IMaterialSystem*				g_pMatSystem = NULL;
IClientEntityList*				g_pEntList = NULL;
IVDebugOverlay*					g_pDebugOverlay = NULL;
CInput*							g_pInput = NULL;
IVRenderView*				    g_pRender = NULL;
IFileSystem*				    g_pFileSystem = NULL;
IVModelRender*				    g_pModelRender = NULL;
IVEfx*					        g_pEffects = NULL;
ICvar*					        g_pCvar = NULL;
IEngineTrace*				    g_pEngineTrace = NULL;
IVModelInfoClient*			    g_pModelInfo = NULL;
CGlobalVarsBase*			    g_pGlobals = NULL;
IGameEventManager2*			    g_pGameEventManager = NULL;
IPhysicsSurfaceProps*			g_pPhysicsAPI = NULL;
IPhysics*						g_pPhysics = NULL;
IPanel*							g_pIPanel = NULL;
//CUserMessages*				g_pUserMessages = NULL;
IEngineSound*					g_pEngineSound = NULL;
IPrediction*					g_pPrediction = NULL;
IUniformRandomStream*			g_pRandom = NULL;
IShadowMgr*						g_pShadowMgr = NULL;
//===================================================================================
// Factories
//===================================================================================
CreateInterfaceFn				g_ClientFactory = NULL;
CreateInterfaceFn				g_EngineFactory = NULL;
CreateInterfaceFn				g_AppSysFactory = NULL;
CreateInterfaceFn				g_VGUIFactory = NULL;
CreateInterfaceFn				g_PhysicsFactory = NULL;
CreateInterfaceFn				g_MaterialFactory = NULL;
CreateInterfaceFn				g_FileSystemFactory = NULL;
//===================================================================================
CVMTHookManager*				g_pClientHook = NULL;
//===================================================================================


//===================================================================================
// Steam Interfaces & Factory
//===================================================================================
ISteamClient*					g_pSteamClient = NULL;
ISteamFriends*					g_pSteamFriends = NULL;
ISteamUtils*					g_pSteamUtils = NULL;
ISteamUser*						g_pSteamUser = NULL;
ISteamUserStats*				g_pSteamStats = NULL;
ISteamMatchmaking*				g_pMatchMaking = NULL;
ISteamMatchmakingServers*		g_pMatchMakingSvr =  NULL;
ISteamApps*						g_pSteamApps = NULL;
//===================================================================================
CreateInterfaceFn				g_SteamClientFactory = NULL;
//===================================================================================
DWORD WINAPI dwSteamThread( LPVOID lpArguments )
{
	HMODULE hmSteam = gBaseAPI.GetModuleHandleSafe( "SteamClient.dll" );
	g_SteamClientFactory = ( CreateInterfaceFn ) GetProcAddress( hmSteam, "CreateInterface" );

	if( !g_pSteamClient )
	{
		g_pSteamClient = ( ISteamClient* ) g_SteamClientFactory( STEAMCLIENT_INTERFACE_VERSION, NULL );
		gBaseAPI.LogToFile( "g_pSteamClient: [0x%.8X]", (DWORD)g_pSteamClient );
		XASSERT( g_pSteamClient );
	}

	HSteamPipe hNewPipe = g_pSteamClient->CreateSteamPipe( );
	HSteamUser hNewUser = g_pSteamClient->ConnectToGlobalUser( hNewPipe );

	if( !g_pSteamUtils )
	{
		g_pSteamUtils = g_pSteamClient->GetISteamUtils( hNewPipe, STEAMUTILS_INTERFACE_VERSION );
		gBaseAPI.LogToFile( "g_pSteamUtils: [0x%.8X]", (DWORD)g_pSteamUtils );
		XASSERT( g_pSteamUtils );
	}

	if( !g_pSteamFriends )
	{
		g_pSteamFriends	= g_pSteamClient->GetISteamFriends( hNewUser, hNewPipe, STEAMFRIENDS_INTERFACE_VERSION );
		gBaseAPI.LogToFile( "g_pSteamFriends: [0x%.8X]", (DWORD)g_pSteamFriends );
		XASSERT( g_pSteamFriends );
	}

	if( !g_pSteamUser )
	{
		g_pSteamUser = g_pSteamClient->GetISteamUser( hNewUser, hNewPipe, STEAMUSER_INTERFACE_VERSION );
		gBaseAPI.LogToFile( "g_pSteamUser: [0x%.8X]", (DWORD)g_pSteamUser );
		XASSERT( g_pSteamUser );
	}

	if( !g_pSteamStats )
	{
		g_pSteamStats = g_pSteamClient->GetISteamUserStats( hNewUser, hNewPipe, STEAMUSERSTATS_INTERFACE_VERSION );
		gBaseAPI.LogToFile( "g_pSteamStats: [0x%.8X]", (DWORD)g_pSteamStats );
		XASSERT( g_pSteamStats );
	}

	if( !g_pSteamApps )
	{
		g_pSteamApps = g_pSteamClient->GetISteamApps( hNewUser, hNewPipe, STEAMAPPS_INTERFACE_VERSION );
		gBaseAPI.LogToFile( "g_pSteamApps: [0x%.8X]", (DWORD)g_pSteamApps );
		XASSERT( g_pSteamApps );
	}

	if( !g_pMatchMaking )
	{
		g_pMatchMaking = g_pSteamClient->GetISteamMatchmaking( hNewUser, hNewPipe, STEAMMATCHMAKING_INTERFACE_VERSION );
		gBaseAPI.LogToFile( "g_pMatchMaking: [0x%.8X]", (DWORD)g_pMatchMaking );
		XASSERT( g_pMatchMaking );
	}

	if( !g_pMatchMakingSvr )
	{
		g_pMatchMakingSvr = g_pSteamClient->GetISteamMatchmakingServers( hNewUser, hNewPipe, STEAMMATCHMAKING_INTERFACE_VERSION );
		gBaseAPI.LogToFile( "g_pMatchMakingSvr: [0x%.8X]", (DWORD)g_pMatchMakingSvr );
		XASSERT( g_pMatchMakingSvr );
	}

	return 0;
}
//===================================================================================
DWORD WINAPI dwMainThread( LPVOID lpArguments )
{
	HMODULE hmClient = gBaseAPI.GetModuleHandleSafe( "client.dll" );
	g_ClientFactory = ( CreateInterfaceFn ) GetProcAddress( hmClient, "CreateInterface" );

	HMODULE hmEngine = gBaseAPI.GetModuleHandleSafe( "engine.dll" );
	g_EngineFactory = ( CreateInterfaceFn ) GetProcAddress( hmEngine, "CreateInterface" );

	HMODULE hmVGUI = gBaseAPI.GetModuleHandleSafe( "vguimatsurface.dll" );
	g_VGUIFactory = ( CreateInterfaceFn ) GetProcAddress( hmVGUI, "CreateInterface" );

	HMODULE hmMaterial = gBaseAPI.GetModuleHandleSafe( "MaterialSystem.dll" );
	g_MaterialFactory = ( CreateInterfaceFn ) GetProcAddress( hmMaterial, "CreateInterface" );

	HMODULE hmPhysics = gBaseAPI.GetModuleHandleSafe( "vphysics.dll" );
	g_PhysicsFactory = ( CreateInterfaceFn ) GetProcAddress( hmPhysics, "CreateInterface" );

	HMODULE hmFileSystem = gBaseAPI.GetModuleHandleSafe( "FileSystem_Steam.dll" );
	g_FileSystemFactory = ( CreateInterfaceFn ) GetProcAddress( hmFileSystem, "CreateInterface" );

	if( !g_pEngine )
	{
		g_pEngine = ( IVEngineClient* ) g_EngineFactory( VENGINE_CLIENT_INTERFACE_VERSION, NULL );
		gBaseAPI.LogToFile( "g_pEngine: [0x%.8X]", (DWORD)g_pEngine);
		XASSERT( g_pEngine );
	}

	if( !g_pClient )
	{
		g_pClient = ( IBaseClientDLL* ) g_ClientFactory( CLIENT_DLL_INTERFACE_VERSION, NULL );
		gBaseAPI.LogToFile( "g_pClient: [0x%.8X]", (DWORD)g_pClient );
		XASSERT( g_pClient );

		PDWORD* m_pdwClient = ( PDWORD* ) g_ClientFactory( CLIENT_DLL_INTERFACE_VERSION, NULL );

		if( m_pdwClient )
		{
			RtlCopyMemory( ( void* )&gClient,( void* )*m_pdwClient , sizeof ( CClient ) );
			
			PDWORD pdwAddress = ( PDWORD ) ( ( ( DWORD ) gClient.CreateMove ) + 0x28 );
			PDWORD pdwTable = ( PDWORD ) *pdwAddress;

			g_pClientHook = new CVMTHookManager( m_pdwClient );
			g_pClientHook->dwHookMethod( ( DWORD )Hooked_Init, 0 );
			g_pClientHook->dwHookMethod( ( DWORD )Hooked_HudUpdate, 9 ); 
			g_pClientHook->dwHookMethod( ( DWORD )Hooked_IN_KeyEvent, 17 );
			g_pClientHook->dwHookMethod( ( DWORD )Hooked_CreateMove, 18 );

			if( !g_pEntList )
			{
				g_pEntList = ( IClientEntityList* ) g_ClientFactory( VCLIENTENTITYLIST_INTERFACE_VERSION, NULL );
				gBaseAPI.LogToFile( "g_pEntList: [0x%.8X]", (DWORD)g_pEntList );
				XASSERT( g_pEntList );
			}

			if( !g_pPrediction )
			{
				g_pPrediction = ( IPrediction* ) g_ClientFactory( VCLIENT_PREDICTION_INTERFACE_VERSION, NULL );
				gBaseAPI.LogToFile( "g_pPrediction: [0x%.8X]", (DWORD)g_pPrediction );
				XASSERT( g_pPrediction );
			}

			if( !g_pInput )
			{
				g_pInput = ( CInput* )*pdwTable;
				gBaseAPI.LogToFile( "g_pInput: [0x%.8X]", (DWORD)g_pInput );
				XASSERT( g_pInput );
			}
		}
	}

	dwWeaponIDToAlias = gMemoryTools.dwFindPattern( (DWORD)hmClient, 0x5F6000, (BYTE*)"\x8B\x44\x24\x04\x83\xF8\x3C\x73\x0C\x85\xC0\x7C\x08\x8B\x04\x85\x00\x00\x00\x00\xC3\x33\xC0\xC3", "xxxxxxxxxxxxxxxx????xxxx" );
	XASSERT( dwWeaponIDToAlias );

	dwSaveUserCmd0		= gMemoryTools.dwFindPattern( (DWORD)hmClient, 0x5F6000, (BYTE*)"\x8B\xC1\x8B\x4C\x24\x04\x3B\xC1\x74\x64\x8B\x51\x04\x89\x50\x04\x8B\x51\x08", "xxxxxxxxxxxxxxxxxxx" );
	XASSERT( dwSaveUserCmd0 );

	dwSaveUserCmd1		= gMemoryTools.dwFindPattern( (DWORD)hmClient, 0x5F6000, (BYTE*)"\x51\x56\x8D\x44\x24\x04\x50\x8B\xF1\xE8\x00\x00\x00\x00\x6A\x04\x8D\x4E\x04\x51\x8D\x54\x24\x10", "xxxxxxxxxx????xxxxxxxxxx" );
	XASSERT( dwSaveUserCmd1 );

	dwPredictionRandSeed	= gMemoryTools.dwFindPattern( (DWORD)hmClient, 0x5F6000, (BYTE*)"\x8B\x44\x24\x04\x85\xC0\x75\x0B\xC7\x05\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xC3", "xxxxxxxxxx????xxxxx" );
	XASSERT( dwPredictionRandSeed );

	if( dwSaveUserCmd0 != NULL )
	{
		gVerifyFuncs.SaveUserCmd0 = (void*)dwSaveUserCmd0;
		gBaseAPI.LogToFile( "dwSaveUserCmd0: [0x%.8X]", (DWORD)dwSaveUserCmd0 );
	}
	if( dwSaveUserCmd1 != NULL )
	{
		gVerifyFuncs.SaveUserCmd1 = (void*)dwSaveUserCmd1;
		gBaseAPI.LogToFile( "dwSaveUserCmd1: [0x%.8X]", (DWORD)dwSaveUserCmd1 );
	}
	if( dwPredictionRandSeed != NULL )
	{
		SetPredictionRandomSeed = ( void (__cdecl *)(const CUserCmd *) )dwPredictionRandSeed;
		gBaseAPI.LogToFile( "dwPredictionRandSeed: [0x%.8X]", (DWORD)dwPredictionRandSeed );
	}

	if( dwWeaponIDToAlias != NULL )
	{
		gBaseAPI.LogToFile( "dwWeaponIDToAlias: [0x%.8X]", (DWORD)dwWeaponIDToAlias );
	}

	if( !g_pEngineTrace )
	{
		g_pEngineTrace = ( IEngineTrace* ) g_EngineFactory( INTERFACEVERSION_ENGINETRACE_CLIENT, NULL );
		gBaseAPI.LogToFile( "g_pEngineTrace: [0x%.8X]", (DWORD)g_pEngineTrace );
		XASSERT( g_pEngineTrace );
	}

	if( !g_pModelInfo )
	{
		g_pModelInfo = ( IVModelInfoClient* ) g_EngineFactory( VMODELINFO_CLIENT_INTERFACE_VERSION, NULL );
		gBaseAPI.LogToFile( "g_pModelInfo: [0x%.8X]", (DWORD)g_pModelInfo );
		XASSERT( g_pModelInfo );
	}

	if( !g_pEngineVGUI )
	{
		g_pEngineVGUI = ( IEngineVGui* ) g_EngineFactory( VENGINE_VGUI_VERSION, NULL );
		gBaseAPI.LogToFile( "g_pEngineVGUI: [0x%.8X]", (DWORD)g_pEngineVGUI );
		XASSERT( g_pEngineVGUI );
	}

	if( !g_pEffects )
	{
		g_pEffects = ( IVEfx* ) g_EngineFactory( VENGINE_EFFECTS_INTERFACE_VERSION, NULL );
		gBaseAPI.LogToFile( "g_pEffects: [0x%.8X]", (DWORD)g_pEffects );
		XASSERT( g_pEffects );
	}

	if( !g_pGameEventManager )
	{
		g_pGameEventManager = ( IGameEventManager2* ) g_EngineFactory( INTERFACEVERSION_GAMEEVENTSMANAGER2, NULL );
		gBaseAPI.LogToFile( "g_pGameEventManager: [0x%.8X]", (DWORD)g_pGameEventManager );
		XASSERT( g_pGameEventManager );
	}

	if( !g_pMatSystem )
	{
		g_pMatSystem = ( IMaterialSystem* ) g_MaterialFactory( MATERIAL_SYSTEM_INTERFACE_VERSION, NULL );
		gBaseAPI.LogToFile( "g_pMatSystem: [0x%.8X]", (DWORD)g_pMatSystem );
		XASSERT( g_pMatSystem );
	}

	if( !g_pSurface )
	{
		g_pSurface = ( vgui::ISurface* ) g_VGUIFactory( VGUI_SURFACE_INTERFACE_VERSION, NULL );
		gBaseAPI.LogToFile( "g_pSurface: [0x%.8X]", (DWORD)g_pSurface );
		XASSERT( g_pSurface );
	}

	if( !g_pDebugOverlay )
	{
		g_pDebugOverlay = ( IVDebugOverlay* ) g_EngineFactory( VDEBUG_OVERLAY_INTERFACE_VERSION, NULL );
		gBaseAPI.LogToFile( "g_pDebugOverlay: [0x%.8X]", (DWORD)g_pDebugOverlay );
		XASSERT( g_pDebugOverlay );
	}


	if( !g_pModelRender )
	{
		g_pModelRender = ( IVModelRender* ) g_EngineFactory( VENGINE_HUDMODEL_INTERFACE_VERSION, NULL );
		gBaseAPI.LogToFile( "g_pModelRender: [0x%.8X]", (DWORD)g_pModelRender );
		XASSERT( g_pModelRender );

		if( g_pModelRender )
			BuildModelRenderHook( ( PDWORD* )g_pModelRender );
	}

	if( !g_pRender )
	{
		g_pRender = ( IVRenderView* ) g_EngineFactory( VENGINE_RENDERVIEW_INTERFACE_VERSION, NULL );
		gBaseAPI.LogToFile( "g_pRender: [0x%.8X]", (DWORD)g_pRender );
		XASSERT( g_pRender );
	}

	if( !g_pPhysicsAPI )
	{
		g_pPhysicsAPI = ( IPhysicsSurfaceProps *) g_PhysicsFactory( VPHYSICS_SURFACEPROPS_INTERFACE_VERSION, NULL);
		gBaseAPI.LogToFile( "g_pPhysicsAPI: [0x%.8X]", (DWORD)g_pPhysicsAPI);
		XASSERT( g_pPhysicsAPI );
	}

	if( !g_pPhysics )
	{
		g_pPhysics = ( IPhysics* ) g_PhysicsFactory( VPHYSICS_INTERFACE_VERSION, NULL );
		gBaseAPI.LogToFile( "g_pPhysics: [0x%.8X]", (DWORD)g_pPhysics);
		XASSERT( g_pPhysics );
	}

	if( !g_pRandom )
	{
		g_pRandom = ( IUniformRandomStream* ) g_EngineFactory( VENGINE_CLIENT_RANDOM_INTERFACE_VERSION, NULL );
		gBaseAPI.LogToFile( "g_pRandom: [0x%.8X]", (DWORD)g_pRandom );
		XASSERT( g_pRandom );
	}

	if( !g_pFileSystem )
	{
		g_pFileSystem = ( IFileSystem* ) g_FileSystemFactory( FILESYSTEM_INTERFACE_VERSION, NULL );
		gBaseAPI.LogToFile( "g_pFileSystem: [0x%.8X]", (DWORD)g_pFileSystem );
		XASSERT( g_pFileSystem );
	}

	if( !g_pEngineSound )
	{
		g_pEngineSound = ( IEngineSound* ) g_EngineFactory( IENGINESOUND_CLIENT_INTERFACE_VERSION, NULL );
		gBaseAPI.LogToFile( "g_pEngineSound: [0x%.8X]", (DWORD)g_pEngineSound );
		XASSERT( g_pEngineSound );

		if( g_pEngineSound )
			BuildEngineSoundHook( (PDWORD*)g_pEngineSound );
	}

	if( !g_pShadowMgr )
	{
		g_pShadowMgr = ( IShadowMgr* ) g_EngineFactory( ENGINE_SHADOWMGR_INTERFACE_VERSION, NULL );
		gBaseAPI.LogToFile( "g_pShadowMgr: [0x%.8X]", (DWORD)g_pShadowMgr );
		XASSERT( g_pShadowMgr );
	}

	return 0;
}

BOOL WINAPI DllMain( HINSTANCE hInstance, DWORD dwReasonOfCall, LPVOID lpReserved )
{
	if ( dwReasonOfCall == DLL_PROCESS_ATTACH )
	{
		hmThisModule = hInstance;
		gBaseAPI.BaseUponModule( (HMODULE)hInstance );
		gMemoryTools.RemovePEHeader( (DWORD)hInstance );
		DisableThreadLibraryCalls( (HMODULE)hInstance );

		CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)dwSteamThread, NULL, 0, NULL );
		CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)dwMainThread, NULL, 0, NULL );
	}

	return TRUE;
}