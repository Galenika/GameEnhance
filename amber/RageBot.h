
#pragma once

#include "Hacks.h"
#include "UTIL Functions.h"

class CRageBot : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool& bSendPacket);
	void ClampAngles(Vector& vecAngles);
	void NormalizeVector(Vector& vec);

private:
	// Targetting
	int GetTargetCrosshair();
	int GetTargetDistance(CUserCmd *pCmd);
	int GetTargetHealth();
	bool TargetMeetsRequirements(IClientEntity* pEntity);
	float FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int HitBox);
	int HitScan(IClientEntity* pEntity);
	bool AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool& bSendPacket);
	void PosAdjust(CUserCmd *pCmd);
	// Functionality
	void DoAimbot(CUserCmd *pCmd, bool& bSendPacket);
	void DoNoSpread(CUserCmd *pCmd);
	void DoNoRecoil(CUserCmd *pCmd);
	// AntiAim
	void DoAntiAim(CUserCmd *pCmd, bool& bSendPacket);

	// AimStep
	bool IsAimStepping;
	Vector LastAimstepAngle;
	Vector LastAngle;

	// Aimbot
	bool IsLocked;
	int TargetID;
	int HitBox;
	Vector AimPoint;
};