#pragma once

#include "SDK.h"
#include "Vector.h"

namespace G
{
	extern int selectedPlayer;
	extern Vector qLastTickAngle;
	extern Vector LastAngle;
	extern bool initializedPlayerList;
	extern CUserCmd* UserCmd;
	extern IClientEntity* iTarget;
	extern int Shots;
	extern float fov;
	extern float GetOutgoingLatency();
	

	//angles
	extern float RealAngles;
	extern float FakeAngles;


	//
	extern bool LBYUpdate;
	extern float LastLBYUpdateTime;
	extern float LastLBYUpdateTime;
	extern float NextLBYUpdate;
	extern bool bSwitch;
}