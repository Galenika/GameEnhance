/*
Syn's AyyWare Framework 2015
*/

#pragma once

// It's actually in DLLMain but w/e
extern bool DoUnload;
extern bool bGlovesNeedUpdate;

#include "Utilities.h"
#include "ViewSetup.h"

typedef void(__stdcall *OverrideViewFn)(CViewSetup*);
extern OverrideViewFn oOverrideView;

namespace Hooks
{
	void Initialise();
	void UndoHooks();

	// VMT Managers
	extern Utilities::Memory::CVMTHookManager VMTPanel; // Hooking drawing functions
	extern Utilities::Memory::VMTManager VMTClient; // Maybe CreateMove
	extern Utilities::Memory::VMTManager VMTClientMode; // CreateMove for functionality
	extern Utilities::Memory::CVMTHookManager VMTPrediction; // InPrediction for no vis recoil
	extern void __stdcall OverrideView(CViewSetup* vsView);
};
