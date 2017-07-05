#pragma once

#include "Interfaces.h"
#include "Hacks.h"

namespace R
{
	extern std::vector<AngleFix> angleFixArray;
	extern std::vector<Resolver> resolverArray;
	extern std::map<int, ResolveData> PlayerData;

	void CheckAddPlayers();
	void AddPlayers();

	void DamageCallback(IGameEvent* pEvent);
	void AddShot(int EntityIndex);
	void LowerBodyFix();
	void Resolve();
};