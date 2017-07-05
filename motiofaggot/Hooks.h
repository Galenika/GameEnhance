/*
Syn's AyyWare Framework 2015
*/

#pragma once
#include "UTIL Functions.h"
// It's actually in DLLMain but w/e
extern bool DoUnload;
typedef unsigned long long VPANEL;
typedef unsigned long long HFont;
extern bool bGlovesNeedUpdate;
class IClientMode;
class ILauncherMgr;
class ICommandLine;
class KeyValues;
class CBaseClientState;
struct CGlowObjectManager;

/* function prototypes */

#include "Utilities.h"

namespace Hooks
{
	void Initialise();
	void UndoHooks();

	// VMT Managers
	extern Utilities::Memory::CVMTHookManager VMTPanel; // Hooking drawing functions
	extern Utilities::Memory::CVMTHookManager VMTClient; // Maybe CreateMove
	extern Utilities::Memory::CVMTHookManager VMTClientMode; // CreateMove for functionality
	extern Utilities::Memory::CVMTHookManager VMTModelRender; // DrawModelEx for chams
	extern Utilities::Memory::CVMTHookManager VMTPrediction; // InPrediction for no vis recoil
};