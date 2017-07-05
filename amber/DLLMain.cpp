

// General shit
#include "DLLMain.h"
#include "Utilities.h"

// Stuff to initialise
#include "Offsets.h"
#include "Interfaces.h"
#include "Hooks.h"
#include "RenderManager.h"
#include "Hacks.h"
#include "Menu.h"
#include "AntiAntiAim.h"
#include "MiscHacks.h"
#include "Dumping.h"
#include "LBYHook.h"
#include "Knife.h"
#include <mmsystem.h>
#include <mmsyscom.h>
#include <iostream>
#include <Windows.h>

//Security
#include "Security.h"
#include "sha256.h"
#include "AntiLeak.h"

// Used as part of the reflective DLL injection
extern HINSTANCE hAppInstance;

// Our DLL Instance
HINSTANCE HThisModule;
bool DoUnload;

void InitialThread()
{
	//---------------------------------------------------------
	// Initialise all our shit
	Offsets::Initialise(); // Set our VMT offsets and do any pattern scans
	Interfaces::Initialise(); // Get pointers to the valve classes
	NetVar.RetrieveClasses(); // Setup our NetVar manager (thanks shad0w bby)
	Render::Initialise();
	Hacks::SetupHacks();
	Menu::SetupMenu();
	Hooks::Initialise();
	LBYHook();
	ApplyKnifeHooks();
	//ApplyAAAHooks();
}

// DllMain
// Entry point for our module
DWORD WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
	switch (fdwReason) { // If we have a reason to call this function, do as follows.
	case DLL_PROCESS_ATTACH: {
		//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadHiding, NULL, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitialThread, NULL, NULL, NULL);
		return TRUE;
	}
	case DLL_PROCESS_DETACH: {
		return TRUE;
	}
							 return FALSE;
	}
}