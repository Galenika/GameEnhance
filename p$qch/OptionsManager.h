#pragma once

#include "EntityStructs.h"

class COptions {
public:
	//Global
	bool bAutoHop = false;

	//Menu related
	bool bShowMenu = false;
	bool vecPressedKeys[256] = {};

	//TargetList related
	CCacheAngle aCacheAngle[64];

	//VISUALS
	bool bEspEnabled = false;
	bool bGlow = false;
	bool bHealthBar = false;
	bool bArmorBar = false;
	bool bPlayerName = false;
	bool bPlayerIndex = false;
	bool bBoneESP = false;
	bool bWeaponESP = false;
	bool bNoScope = false;

	//AIMBOT
	bool bAutoShoot = true;
	bool bAutoPistol = true;
	bool bAutoRevolver = true;
	bool bAutoScope = true;
	bool bSilentAim = true;
	int iHitchance = 0;
	int iHitChanceSeed = 256;

	bool bCorrect = true;
	int iResolverType = 0;
	bool aMultiHitboxes[15] = {
		true, true, true, true, true,
		true, true, true, true, true,
		true, true, true, true, true
	};
	bool bPrioritize = true;
	int iHitbox = 0;
	bool bAutoBaim = false;
	int iAutoBaimAferShot = 3;
	int iHitboxAutoBaim = 4;
	bool bPrioritizeVis = false;
	float flMinDmg = 5.f;
	bool bInterpLagComp = true;

	//ANTI-AIM
	bool bFakeLag = false;
	int iFakeLagAmount = 1;
	bool bAA = true;
	int iAAPitch = 0;
	int iAARealYaw = 0;
	int iAAFakeYaw = 0;

	// Misc
	bool bNoVisRecoil = false;
	bool bNoFlash = false;
	bool bWaterMark = false;
	bool bIndicators = false;
	int iFov = 0;
};

extern COptions OptionsManager;