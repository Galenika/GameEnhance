#define Madness 1213810804					//my hwid is here to test your shit nigger dont say shit
#define XenoConnor -931427315				//2	
#define Pinky 1181487738                    //
#define xCyrpo -998590402                   //Best Friend
#define Snup -499128537
#define Buyer1  -1329616640
#define brobo -1029606714
#define Buyer2 -971321103
#define Buyer3 -1669550497
#define Buyer4 1078741992
#define Hard 2022560954
#define Buyer5 -956351151
#define Buyer6 519941512
#define Buyer7 38182903
#define Anue 785519428
#define Russian 1784079837
// General shit
#include "DLLMain.h"
#include "Utilities.h"

// Injection stuff
#include "INJ/ReflectiveLoader.h"

// Stuff to initialise
#include "Offsets.h"
#include "Interfaces.h"
#include "Hooks.h"
#include "RenderManager.h"
#include "Hacks.h"
#include "Menu.h"
#include "AntiAntiAim.h"
#include "xor.h"
#include "AntiLeak.h"
#include "Dumping.h"

void CAntiLeak::ErasePE()
{
	char *pBaseAddr = (char*)GetModuleHandle(NULL);
	DWORD dwOldProtect = 0;
	VirtualProtect(pBaseAddr, 4096, PAGE_READWRITE, &dwOldProtect);
	ZeroMemory(pBaseAddr, 4096);
	VirtualProtect(pBaseAddr, 4096, dwOldProtect, &dwOldProtect);
}

// Used as part of the reflective DLL injection
extern HINSTANCE hAppInstance;

bool DoUnload;

HINSTANCE HThisModule;


UCHAR szFileSys[255], szVolNameBuff[255];
DWORD dwMFL, dwSysFlags;
DWORD dwSerial;
LPCTSTR szHD = "C:\\"; // location HWID


					   // Our thread we use to setup everything we need
					   // Everything appart from code in hooks get's called from inside 
					   // here.

int InitialThread()
{
	//MessageBox(NULL, "PinkyWare is injected.", "PinkyWare Injector", MB_OK);
	HANDLE hOlly = FindWindow(TEXT("OLLYDBG"), NULL);
	HANDLE hWinDbg = FindWindow(TEXT("WinDbgFrameClass"), NULL);
	HANDLE hScylla1 = FindWindow(NULL, TEXT("Scylla x86 v0.9.7c"));
	HANDLE hScylla2 = FindWindow(NULL, TEXT("Scylla x64 v0.9.7c"));
	HANDLE x32_dbg = FindWindow(NULL, TEXT("x32_dbg"));
	HANDLE x64_dbg = FindWindow(NULL, TEXT("x64_dbg"));
	HANDLE IDA = FindWindow(NULL, TEXT("IDA"));

	/*if (IsDebuggerPresent())
		ExitProcess(0);
	if (IsDbgPresentPrefixCheck())
		ExitProcess(0);
	if (Int2DCheck())
		ExitProcess(0);
	if (hOlly)
		ExitProcess(0);
	if (hWinDbg)
		ExitProcess(0);
	if (hScylla1)
		ExitProcess(0);
	if (hScylla2)
		ExitProcess(0);
	if (x32_dbg)
		ExitProcess(0);
	if (x64_dbg)
		ExitProcess(0);*/
	OutputDebugString(TEXT("%s%s%s%s%s%s%s%s%s%s%s")TEXT("%s%s%s%s%s%s%s%s%s%s%s%s%s")TEXT("%s%s%s%s%s%s%s%s%s%s%s%s%s")TEXT("%s%s%s%s%s%s%s%s%s%s%s%s%s"));

	char szFileName[MAX_PATH];
	GetModuleFileName(NULL, szFileName, MAX_PATH);
	std::string wsProcessName = szFileName;
	wsProcessName = wsProcessName.substr(wsProcessName.find_last_of("\\") + 1);
	if (wsProcessName == /*csgo.exe*/XorStr<0xFE, 9, 0x2CB977B1>("\x9D\x8C\x67\x6E\x2C\x66\x7C\x60" + 0x2CB977B1).s)
	{
		Sleep(100);
	}

	while (FindWindowA("Valve001", 0) == 0)
		Sleep(100);

	while (GetModuleHandleA("engine.dll") == 0 || GetModuleHandleA("client.dll") == 0)
		Sleep(100);

	// Intro banner with info
	PrintMetaHeader();

	//---------------------------------------------------------
	// Initialise all our shit
	Offsets::Initialise(); // Set our VMT offsets and do any pattern scans
	Interfaces::Initialise(); // Get pointers to the valve classes
	NetVar.RetrieveClasses(); // Setup our NetVar manager (thanks shad0w bby)
	NetvarManager::Instance()->CreateDatabase();
	Render::Initialise();
	Hacks::SetupHacks();
	Menu::SetupMenu();
	Hooks::Initialise();
	ApplyAAAHooks();

	//GUI.LoadWindowState(&Menu::Window, "config.xml");

	// Dumping
	//Dump::DumpClassIds();

	//---------------------------------------------------------
	Utilities::Log("Ready");

	// While our cheat is running
	while (DoUnload == false)
	{
		Sleep(1000);
	}

	Hooks::UndoHooks();
	Sleep(2000); // Make sure none of our hooks are running
	FreeLibraryAndExitThread(HThisModule, 0);

	return 0;
}

// DllMain
// Entry point for our module
BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
)
{
	switch (fdwReason)
	{
	case DLL_QUERY_HMODULE:
		if (lpvReserved != NULL)
			*(HMODULE *)lpvReserved = hAppInstance;
		break;
	case DLL_PROCESS_ATTACH:
		if (!GetModuleHandle(/*client.dll*/XorStr<0xFE, 11, 0x528052E7>("\x9D\x93\x69\x64\x6C\x77\x2A\x61\x6A\x6B" + 0x528052E7).s))
			Sleep(300);
		if (!GetModuleHandle(/*engine.dll*/XorStr<0xD8, 11, 0x190E3235>("\xBD\xB7\xBD\xB2\xB2\xB8\xF0\xBB\x8C\x8D" + 0x190E3235).s))
			Sleep(300);

		HideThread(0);
		DisableThreadLibraryCalls(hinstDLL);
		// if the Hardware ID in the switch to remove the sign : /* behind GetVolume
		(GetVolumeInformation(szHD, (LPTSTR)szVolNameBuff, 255, &dwSerial, &dwMFL, &dwSysFlags, (LPTSTR)szFileSys, 255));;

		// Serial Number or your HWID
		if (dwSerial == Madness ||
			dwSerial == XenoConnor ||
			dwSerial == Pinky ||
			dwSerial == xCyrpo ||
			dwSerial == Snup ||
			dwSerial == Buyer1 ||
			dwSerial == brobo ||
			dwSerial == Buyer2 ||
			dwSerial == Buyer3 ||
			dwSerial == Buyer4 ||
			dwSerial == Hard ||
			dwSerial == Buyer5 ||
			dwSerial == Buyer6 ||
			dwSerial == Buyer7 ||
			dwSerial == Anue ||
			dwSerial == Russian)
		{

			//	MessageBox(NULL, "Valid HWID.", "PASTEWARE", MB_OK);
			Sleep(100);
		}
		else
		{
			// when HWID rejected
			MessageBox(NULL, "Invalid HWID.", "FUCK NIGGERS", MB_OK);
			exit(0);
			return TRUE;
		}

		//	HThisModule = hinstDLL;
		if (GetModuleHandleA(/*csgo.exe*/XorStr<0xCE, 9, 0xF6281E21>("\xAD\xBC\xB7\xBE\xFC\xB6\xAC\xB0" + 0xF6281E21).s))
		{

			//Sleep(1000);
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitialThread, NULL, NULL, NULL);
		}
		else if (!GetModuleHandleA(/*csgo.exe*/XorStr<0xCE, 9, 0xF6281E21>("\xAD\xBC\xB7\xBE\xFC\xB6\xAC\xB0" + 0xF6281E21).s))
		{

			exit(0);
			return TRUE;
		}
		break;
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}
