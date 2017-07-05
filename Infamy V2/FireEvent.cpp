#include "hooks.h"
#include "Globals.h"
#include <unordered_map>
#include "Accessors.h"
#pragma comment(lib,"Winmm.lib")

FireEventClientSideFn oFireEventClientSide;
float  _flHurtTime;

bool __fastcall Game::FireEventClientSideThink(void* ecx, void* edx, IGameEvent* pEvent) {
	// If the event pointer is invalid just call the original.
	if (!pEvent)
		return oFireEventClientSide(ecx, pEvent);

	// Make sure we grab all instances when the bomb is not used.
	if (Options.Visuals.ESP.DrawBomb)
	{
		if (!strcmp(pEvent->GetName(), XorStr("bomb_defused")) || !strcmp(pEvent->GetName(), XorStr("bomb_exploded")) || !strcmp(pEvent->GetName(), XorStr("round_end")) || !strcmp(pEvent->GetName(), XorStr("round_start"))) {
			Global::isBombPlanted = false;
		}

		if (!strcmp(pEvent->GetName(), XorStr("bomb_planted"))) {
			Global::isBombPlanted = true;
		}
	}

	// Run the original FireEventClientSide function.
	return oFireEventClientSide(ecx, pEvent);
};