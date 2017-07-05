#include "SDK.h"
#include "Accessors.h"
#include "misc.h"
#include <gdiplus.h>
#include <TlHelp32.h>

typedef unsigned(__stdcall *PTHREAD_START) (void *);
#define _BEGINTHREAD( x ) _beginthreadex(nullptr, 0, (PTHREAD_START)x, nullptr, 0, nullptr);

static bool bVisible;
static bool Setups = true;
DWORD __stdcall HandleConfigs(void* args);
DWORD __stdcall HandleValuesThread(void* args);
DWORD __stdcall Securitythread(void* args)
{
	while (true)
	{
		if (Setups)
		{
			Setup();
			Setups = false;
			CreateThread(nullptr, 0, HandleConfigs, Global::g_hDll, 0, nullptr);
		}
		Sleep(24);
	}
}

void MainThread(LPVOID lpArgs)
{
	CreateThread(nullptr, 0, Securitythread, Global::g_hDll, 0, nullptr);
	CreateThread(nullptr, 0, HandleValuesThread, Global::g_hDll, 0, nullptr);
}

DWORD __stdcall DeAttach(void* args)
{
	while(true)
	{
		if (Options.Cheat.Uninject && GetForegroundWindow() == FindWindowA(XorStr("Valve001"), XorStr("Counter-Strike: Global Offensive")))//make sure we are in CSGO and that our first statment is true
		{
			DestroyHooks();
			Sleep(2000);
			FreeConsole();
			FreeLibraryAndExitThread(static_cast<HMODULE>(args), NULL);
		}
		Sleep(24);
	}
}
DWORD __stdcall HandleConfigs(void* args)
{
	while (true)
	{
		if (Options.Cheat.Uninject)
			ExitThread(NULL);
		Sleep(24);
	}
}

DWORD __stdcall HandleValuesThread(void* args)
{
	while (true)
	{
		Sleep(24);
	}
}

DWORD WINAPI DllMain(HMODULE hDll, DWORD dwReasonForCall, LPVOID lpReserved)
{
	switch(dwReasonForCall)
	{
	case DLL_PROCESS_ATTACH:
		//add these back when its in the loader	
		//HideModule(hDll);
		//HideModuleFromPEB(hDll);
		//RemovePeHeader(reinterpret_cast<DWORD>(hDll));
		//DestroyModuleHeader(hDll);
		Global::g_hDll = hDll;
		Sleep(3000);
		_BEGINTHREAD(MainThread);
		CreateThread(nullptr, 0,DeAttach, hDll, 0, nullptr);//LOOK MA NO CASTS
		return 1;
		
	case DLL_PROCESS_DETACH:
	{
		DestroyHooks();
		Sleep(2000);// 2 seconds to let our VMT hooks unhook
		//g_pMisc->SetConsoleState(false);
		FreeConsole();
		FreeLibraryAndExitThread(hDll, NULL);
		return 1;
	}

	default:
		return 1;
	}
}