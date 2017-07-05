/*
Syn's AyyWare Framework 2015
*/

#include "RageBot.h"
#include "RenderManager.h"
#include "Autowall.h"

void CRageBot::Init()
{
	IsAimStepping = false;
	IsLocked = false;
	TargetID = -1;
}

void CRageBot::Draw()
{

}

bool CanOpenFire()
{
	IClientEntity* pLocalEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalEntity)
		return false;

	CBaseCombatWeapon* entwep = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocalEntity->GetActiveWeaponHandle());

	float flServerTime = (float)pLocalEntity->GetTickBase() * Interfaces::Globals->interval_per_tick;
	float flNextPrimaryAttack = entwep->GetNextPrimaryAttack();

	std::cout << flServerTime << " " << flNextPrimaryAttack << std::endl;

	return !(flNextPrimaryAttack > flServerTime);
}

template<class T, class U>
T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

float InterpolationFix()
{
	static ConVar* cvar_cl_interp = Interfaces::CVar->FindVar(XorStr("cl_interp"));
	static ConVar* cvar_cl_updaterate = Interfaces::CVar->FindVar(XorStr("cl_updaterate"));
	static ConVar* cvar_sv_maxupdaterate = Interfaces::CVar->FindVar(XorStr("sv_maxupdaterate"));
	static ConVar* cvar_sv_minupdaterate = Interfaces::CVar->FindVar(XorStr("sv_minupdaterate"));
	static ConVar* cvar_cl_interp_ratio = Interfaces::CVar->FindVar(XorStr("cl_interp_ratio"));

	IClientEntity* pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	float cl_interp = cvar_cl_interp->GetFloat();
	int cl_updaterate = cvar_cl_updaterate->GetInt();
	int sv_maxupdaterate = cvar_sv_maxupdaterate->GetInt();
	int sv_minupdaterate = cvar_sv_minupdaterate->GetInt();
	int cl_interp_ratio = cvar_cl_interp_ratio->GetInt();

	if (sv_maxupdaterate <= cl_updaterate)
		cl_updaterate = sv_maxupdaterate;

	if (sv_minupdaterate > cl_updaterate)
		cl_updaterate = sv_minupdaterate;

	float new_interp = (float)cl_interp_ratio / (float)cl_updaterate;

	if (new_interp > cl_interp)
		cl_interp = new_interp;

	return max(cl_interp, cl_interp_ratio / cl_updaterate);
}

float HitChance(IClientEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	float hitchance = 101;
	if (!pWeapon) return 0;
	if (Menu::Window.RageBotTab.AccuracyMinimumSpread.GetValue() > 0.01)
	{
		float inaccuracy = pWeapon->GetInaccuracy();
		if (inaccuracy == 0) inaccuracy = 0.0000001;
		inaccuracy = 1 / inaccuracy;
		hitchance = inaccuracy;
	}
	return hitchance;
}

void CRageBot::Move(CUserCmd *pCmd, bool& bSendPacket)
{
	IClientEntity* pLocal = hackManager.pLocal();
	// Aimbot
	if (pLocal->IsAlive()) {
		if (Menu::Window.RageBotTab.AimbotEnable.GetState()) {
			DoAimbot(pCmd, bSendPacket);
			DoAntiAim(pCmd, bSendPacket);
		}

		if (Menu::Window.RageBotTab.ResolverFLFix.GetIndex() == 1)
			pCmd->tick_count = TIME_TO_TICKS(InterpolationFix());
		else if (Menu::Window.RageBotTab.ResolverFLFix.GetIndex() == 2)
			PosAdjust(pCmd);

		// Recoil
		if (Menu::Window.RageBotTab.AccuracyRecoil.GetState())
			DoNoRecoil(pCmd);

		// No Spread
		if (Menu::Window.RageBotTab.AccuracySpread.GetState())
			DoNoSpread(pCmd);
	}
}

bool Ready2Silent(IClientEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	float serverTime = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;
	if (pWeapon->GetNextPrimaryAttack() > serverTime)
		return false;

	return true;
}

bool IsAbleToShoot(IClientEntity* pLocal)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!pLocal)
		return false;

	if (!pWeapon)
		return false;

	float flServerTime = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;

	return (!(pWeapon->GetNextPrimaryAttack() > flServerTime));
}

void CRageBot::NormalizeVector(Vector& vec)
{
	for (int i = 0; i < 3; ++i)
	{
		while (vec[i] > 180.f)
			vec[i] -= 360.f;

		while (vec[i] < -180.f)
			vec[i] += 360.f;
	}

	vec[2] = 0.f;
}

void CRageBot::ClampAngles(Vector& vecAngles)
{
	if (vecAngles[0] > 89.f)
		vecAngles[0] = 89.f;
	if (vecAngles[0] < -89.f)
		vecAngles[0] = -89.f;
	if (vecAngles[1] > 180.f)
		vecAngles[1] = 180.f;
	if (vecAngles[1] < -180.f)
		vecAngles[1] = -180.f;

	vecAngles[2] = 0.f;
}

Vector TickPrediction(Vector AimPoint, IClientEntity* pTarget)
{
	return AimPoint + (pTarget->GetVelocity() * Interfaces::Globals->interval_per_tick);
}

// Functionality
void CRageBot::DoAimbot(CUserCmd *pCmd, bool& bSendPacket)
{
	IClientEntity* pTarget = nullptr;
	IClientEntity* pLocal = hackManager.pLocal();
	Vector Start = pLocal->GetViewOffset() + pLocal->GetOrigin();
	bool FindNewTarget = true;
	IsLocked = false;

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	CSWeaponInfo* weapInfo = ((CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle()))->GetCSWpnData();

	// Don't aimbot with the knife..
	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || !GameUtils::IsBallisticWeapon(pWeapon) || GameUtils::IsKnife(pWeapon))
		{
			//TargetID = 0;
			//pTarget = nullptr;
			//HitBox = -1;
			return;
		}
	}
	else
		return;

	// Make sure we have a good target
	if (IsLocked && TargetID >= 0 && HitBox >= 0)
	{
		pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		if (pTarget  && TargetMeetsRequirements(pTarget))
		{
			HitBox = HitScan(pTarget);
			if (HitBox >= 0)
			{
				Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
				Vector View;
				Interfaces::Engine->GetViewAngles(View);
				float FoV = FovToPlayer(ViewOffset, View, pTarget, HitBox);
				if (FoV < Menu::Window.RageBotTab.AimbotFov.GetValue())
					FindNewTarget = false;
			}
		}
	}

	// Find a new target, apparently we need to
	if (FindNewTarget)
	{
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;

		// Target selection type
		switch (Menu::Window.RageBotTab.TargetSelection.GetIndex())
		{
		case 0:
			TargetID = GetTargetCrosshair();
			break;
		case 1:
			TargetID = GetTargetDistance(pCmd);
			break;
		case 2:
			TargetID = GetTargetHealth();
			break;
		}

		// Memes
		if (TargetID >= 0)
		{
			pTarget = Interfaces::EntList->GetClientEntity(TargetID);
			StepResolver::iTarget = pTarget;
			StepResolver::iTargetID = TargetID;
		}
		else
		{
			pTarget = nullptr;
			HitBox = -1;
		}
	}

	// If we finally have a good target
	if (TargetID >= 0 && pTarget)
	{
		// Get the hitbox to shoot at
		HitBox = HitScan(pTarget);

		if (!CanOpenFire())
			return;

		Vector Point;
		Vector AimPoint = GetHitBoxPosV2(pTarget, HitBox);
		pTarget->GetPredicted(AimPoint); /*Velocity Prediction*/

		Point = AimPoint;

		if (Menu::Window.RageBotTab.ResolverFLFix.GetIndex() == 4)
			Point = TickPrediction(Point, pTarget);
		else
			Point = AimPoint;

		if (Menu::Window.RageBotTab.AccuracyAutoStop.GetState() && pCmd->buttons &IN_ATTACK)
		{
			pCmd->forwardmove = 0.f;
			pCmd->sidemove = 0.f;
			pCmd->buttons |= IN_DUCK;
		}

		if (GameUtils::IsScopedWeapon(pWeapon) && !pWeapon->IsScoped() && Menu::Window.RageBotTab.AccuracyAutoScope.GetState()) /*Auto-Sniper-Zoom*/
		{
			pCmd->buttons |= IN_ATTACK2;
		}
		else
		{
			if ((Menu::Window.RageBotTab.AccuracyMinimumSpread.GetValue() * 1.5 <= HitChance(pLocal, pWeapon)) || Menu::Window.RageBotTab.AccuracyMinimumSpread.GetValue() == 0 || *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() == 64)
			{
				if (Menu::Window.RageBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
				{
					if (*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() == WEAPON_REVOLVER)
					{
						if (pCmd->buttons |= IN_ATTACK) {
							AimAtPoint(pLocal, AimPoint, pCmd, bSendPacket);
						}

						float flPostponeFireReady = pWeapon->GetFireReadyTime();
						if (flPostponeFireReady > 0 && flPostponeFireReady < Interfaces::Globals->curtime)
						{
							pCmd->buttons &= ~IN_ATTACK;
						}
					}
					else if (pCmd->buttons |= IN_ATTACK && *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() != WEAPON_REVOLVER)
					{
						AimAtPoint(pLocal, AimPoint, pCmd, bSendPacket);
					}
				}
			}
		}
	}

	static bool WasFiring = false;
	CSWeaponInfo* WeaponInfo = pWeapon->GetCSWpnData();
	if (!WeaponInfo->m_IsFullAuto && Menu::Window.RageBotTab.AimbotAutoPistol.GetState())
	{
		if (pCmd->buttons & IN_ATTACK)
		{
			if (WasFiring)
			{
				pCmd->buttons &= ~IN_ATTACK;
			}
		}

		WasFiring = pCmd->buttons & IN_ATTACK ? true : false;
	}

}

bool CRageBot::TargetMeetsRequirements(IClientEntity* pEntity)
{
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{
		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && Interfaces::Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum())
			{
				if (!pEntity->HasGunGameImmunity())
				{
					return true;
				}
			}
		}
	}
	return false;
}

float CRageBot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
{
	// Get local view / eye position
	Vector Origin = ViewOffSet;

	// Create and intiialize vectors for calculations below
	Vector Delta(0, 0, 0);
	Vector Forward(0, 0, 0);

	// Convert angles to normalized directional forward vector
	AngleVectors(View, &Forward);
	Vector AimPos = GetHitboxPosition(pEntity, aHitBox);

	// Get delta vector between our local eye position and passed vector
	Delta = AimPos - Origin;
	//Delta = AimPos - Origin;

	// Normalize our delta vector
	Normalize(Delta, Delta);

	// Get dot product between delta position and directional forward vectors
	FLOAT DotProduct = Forward.Dot(Delta);

	// Time to calculate the field of view
	return (acos(DotProduct) * (180.f / PI));
}

int CRageBot::GetTargetCrosshair()
{
	// Target selection
	int target = -1;
	float minFoV = Menu::Window.RageBotTab.AimbotFov.GetValue();

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++) //GetHighestEntityIndex()
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (fov < minFoV)
				{
					minFoV = fov;
					target = i;
				}
			}
		}
	}

	return target;
}

int CRageBot::GetTargetDistance(CUserCmd* pCmd) /*Selection - Distance*/
{
	auto iBestTarget = -1;
	float flDistance = 8192.f;

	IClientEntity* pLocal = hackManager.pLocal();

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			auto vecHitbox = pEntity->GetBonePos(NewHitBox);
			if (NewHitBox >= 0)
			{
				Vector Difference = pLocal->GetOrigin() - pEntity->GetOrigin();
				QAngle TempTargetAbs;
				CalcAngle(pLocal->GetEyePosition(), vecHitbox, TempTargetAbs);
				float flTempFOVs = GetFov(pCmd->viewangles, TempTargetAbs);
				float flTempDistance = VectorDistance(pLocal->GetOrigin(), pEntity->GetOrigin());
				if (flTempDistance < flDistance && flTempFOVs < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					flDistance = flTempDistance;
					iBestTarget = i;
				}
			}
		}
	}
	return iBestTarget;
}

int CRageBot::GetTargetHealth()
{
	// Target selection
	int target = -1;
	int minHealth = 101;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				int Health = pEntity->GetHealth();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Health < minHealth && fov < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					minHealth = Health;
					target = i;
				}
			}
		}
	}

	return target;
}

//HitScan
int CRageBot::HitScan(IClientEntity* pEntity)
{
	IClientEntity* pLocal = hackManager.pLocal();
	std::vector<int> HitBoxesToScan;
	int HitScanMode = Menu::Window.RageBotTab.TargetHitscan.GetIndex();
	int iSmart = Menu::Window.RageBotTab.AccuracySmart.GetValue();
	bool AWall = Menu::Window.RageBotTab.AccuracyAutoWall.GetState();
	bool Multipoint = Menu::Window.RageBotTab.AimbotMultiPoint.GetState();

	if (iSmart > 0 && pLocal->GetShotsFired() + 1 > iSmart)
	{
		HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
	}
	else
	{
		if (HitScanMode == 0)
		{
			switch (Menu::Window.RageBotTab.TargetHitbox.GetIndex()) /*Single-Hitbox*/
			{
			case 0:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				break;
			case 1:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
				break;
			case 2:
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				break;
			case 3:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				break;
			case 4:
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
				break;
			}
		}
		else
		{
			switch (HitScanMode)
			{
			case 1:
				/*Scan-Normal*/
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
				break;
			case 2:
				/*Scan-High*/
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
			case 3:
				/*Scan-All*/
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Max);
			}
		}

		for (int HitBoxID : HitBoxesToScan)
		{
			if (AWall)
			{
				Vector Point = GetHitboxPosition(pEntity, HitBoxID);
				float Damage = 0.f;
				if (CanHit(Point, &Damage))
				{
					if (Damage >= Menu::Window.RageBotTab.AccuracyMinimumDamage.GetValue())
					{
						return HitBoxID;
					}
				}
			}
			else
			{
				if (GameUtils::IsVisible(hackManager.pLocal(), pEntity, HitBoxID))
					return HitBoxID;
			}
		}
		return -1;
	}
}

void CRageBot::PosAdjust(CUserCmd *pCmd)
{
	static ConVar* cvar_cl_interp = Interfaces::CVar->FindVar(XorStr("cl_interp"));
	static ConVar* cvar_cl_updaterate = Interfaces::CVar->FindVar(XorStr("cl_updaterate"));
	static ConVar* cvar_sv_maxupdaterate = Interfaces::CVar->FindVar(XorStr("sv_maxupdaterate"));
	static ConVar* cvar_sv_minupdaterate = Interfaces::CVar->FindVar(XorStr("sv_minupdaterate"));
	static ConVar* cvar_cl_interp_ratio = Interfaces::CVar->FindVar(XorStr("cl_interp_ratio"));

	IClientEntity* pLocal = hackManager.pLocal();

	if (!pLocal)
		return;

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (!pWeapon)
		return;

	float cl_interp = cvar_cl_interp->GetFloat();
	int cl_updaterate = cvar_cl_updaterate->GetInt();
	int sv_maxupdaterate = cvar_sv_maxupdaterate->GetInt();
	int sv_minupdaterate = cvar_sv_minupdaterate->GetInt();
	int cl_interp_ratio = cvar_cl_interp_ratio->GetInt();

	if (sv_maxupdaterate <= cl_updaterate)
		cl_updaterate = sv_maxupdaterate;

	if (sv_minupdaterate > cl_updaterate)
		cl_updaterate = sv_minupdaterate;

	float new_interp = (float)cl_interp_ratio / (float)cl_updaterate;

	if (new_interp > cl_interp)
		cl_interp = new_interp;

	float flSimTime = pLocal->GetSimulationTime();
	float flOldAnimTime = pLocal->GetAnimTime();

	int iTargetTickDiff = (int)(0.5f + (flSimTime - flOldAnimTime) / Interfaces::Globals->interval_per_tick);
}

void CRageBot::DoNoSpread(CUserCmd *pCmd)
{

}

void CRageBot::DoNoRecoil(CUserCmd *pCmd)
{
	// Ghetto rcs shit, implement properly later
	IClientEntity* pLocal = hackManager.pLocal();
	if (pLocal)
	{
		Vector AimPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			pCmd->viewangles -= AimPunch * 2;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}
}

void MrsLuethIsFatNigger()
{
	std::string howManyTimesMrsLuethGaveMeDetention = "No one knows since she is a nigger";
	double salt = 3.14 * 5;
	double newSalt = salt / 3.14;
	newSalt / 5;
}

float FovToPoint(Vector ViewOffSet, Vector View, Vector Point)
{
	// Get local view / eye position
	Vector Origin = ViewOffSet;

	// Create and intiialize vectors for calculations below
	Vector Delta(0, 0, 0);
	Vector Forward(0, 0, 0);

	// Convert angles to normalized directional forward vector
	AngleVectors(View, &Forward);
	Vector AimPos = Point;

	// Get delta vector between our local eye position and passed vector
	Delta = AimPos - Origin;
	//Delta = AimPos - Origin;

	// Normalize our delta vector
	Normalize(Delta, Delta);

	// Get dot product between delta position and directional forward vectors
	FLOAT DotProduct = Forward.Dot(Delta);

	// Time to calculate the field of view
	return (acos(DotProduct) * (360.f / PI));
}

bool CRageBot::AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool& bSendPacket)
{
	bool ReturnValue = false;
	// Get the full angles
	if (point.Length() == 0) return ReturnValue;

	Vector angles;
	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();

	CalcAngle(src, point, angles);
	GameUtils::NormaliseViewAngle(angles);

	IsLocked = true;
	//-----------------------------------------------


	// Aim Step Calcs
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	if (!IsAimStepping)
		LastAimstepAngle = LastAngle; // Don't just use the viewangs because you need to consider aa

	float fovLeft = FovToPlayer(ViewOffset, LastAimstepAngle, Interfaces::EntList->GetClientEntity(TargetID), 0);

	if (fovLeft > 25.0f && Menu::Window.RageBotTab.AimbotAimStep.GetState())
	{
		Vector AddAngs = angles - LastAimstepAngle;
		Normalize(AddAngs, AddAngs);
		AddAngs *= 25;
		LastAimstepAngle += AddAngs;
		GameUtils::NormaliseViewAngle(LastAimstepAngle);
		angles = LastAimstepAngle;
	}
	else
	{
		ReturnValue = true;
	}

	// Silent Aim
	if (Menu::Window.RageBotTab.AimbotSilentAim.GetState() && !Menu::Window.RageBotTab.AimbotPSilent.GetState())
	{
		pCmd->viewangles = angles;
	}

	// Normal Aim
	if (!Menu::Window.RageBotTab.AimbotSilentAim.GetState() && !Menu::Window.RageBotTab.AimbotPSilent.GetState())
	{
		Interfaces::Engine->SetViewAngles(angles);
	}

	// pSilent Aim 
	Vector Oldview = pCmd->viewangles;

	if (Menu::Window.RageBotTab.AimbotPSilent.GetState())
	{
		static int ChokedPackets = -1;
		ChokedPackets++;

		if (ChokedPackets < 6)
		{
			bSendPacket = false;
			pCmd->viewangles = angles;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles = Oldview;
			ChokedPackets = -1;
			ReturnValue = false;
		}

		//pCmd->viewangles.z = 0;
	}

	return ReturnValue;
}

void AtTarget(IClientEntity *Target, CUserCmd *pCmd) {
	if (!Target)
		return;

	if ((Target->GetTeamNum() == hackManager.pLocal()->GetTeamNum()) || Target->IsDormant() || !Target->IsAlive() || Target->GetHealth() <= 0)
		return;

	Vector TargetPosition = Target->GetEyePosition();
	CalcAngle(hackManager.pLocal()->GetEyePosition(), TargetPosition, pCmd->viewangles);
}

void BreakLBY(CUserCmd* pCmd, bool& bSendPacket) //call this in your antiaim shit
{
	if (G::LBYUpdate)
	{
		//owned
	}
}

// AntiAim
void CRageBot::DoAntiAim(CUserCmd *pCmd, bool& bSendPacket)
{
	IClientEntity* pTarget = nullptr;
	IClientEntity* pLocal = hackManager.pLocal();

	float CalculatedCurTime = (Interfaces::Globals->curtime * (Menu::Window.RageBotTab.AntiAimSpeed.GetValue() * 500));

	bool dormant = true;

	if (pLocal->IsAlive() == false) {
		return;
	}
	else {
		// If the aimbot is doing something don't do anything
		if (IsAimStepping || pCmd->buttons & IN_ATTACK)
			return;

		// Weapon shit
		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
		IClientEntity* pLocal = hackManager.pLocal();
		if (pWeapon) {
			CSWeaponInfo* pWeaponInfo = pWeapon->GetCSWpnData();
			// Knives or grenades
			if (!GameUtils::IsBallisticWeapon(pWeapon) || pCmd->buttons &IN_USE || hackManager.pLocal()->GetMoveType() == MOVETYPE_LADDER || hackManager.pLocal()->GetMoveType() == MOVETYPE_OBSERVER || hackManager.pLocal()->GetMoveType() == MOVETYPE_NOCLIP) {
				return;
			}
			else {

			}
		}
	}

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity* ent = Interfaces::EntList->GetClientEntity(i);
		if (!ent || ent->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer || ent->GetTeamNum() == hackManager.pLocal()->GetTeamNum() || !ent->IsAlive()) continue;
		if (ent->IsDormant() == false)
			dormant = false;

	}

	if (Menu::Window.RageBotTab.AntiAimTarget.GetState()) {
		IClientEntity *Target = Interfaces::EntList->GetClientEntity(this->TargetID);
		AtTarget(Target, pCmd);
	}

	if (!Menu::Window.RageBotTab.AntiAimDormant.GetState())
		dormant = false;

	if (!dormant) {
		switch (Menu::Window.RageBotTab.AntiAimPitch.GetIndex())
		{
		case 0:
			// No Pitch AA
			break;
		case 1:
			// Default a.k.a Emotion
			pCmd->viewangles.x = 89.f;
			break;
		case 2:
			//Up
			pCmd->viewangles.x = -89.f;
			break;
		case 3:
			//Down UT
			pCmd->viewangles.x = 179.f;
			break;
		case 4:
			//Minimal a.k.a Zero
			pCmd->viewangles.x = 0.f;
			break;
		}
		switch (Menu::Window.RageBotTab.AntiAimYaw.GetIndex())
		{
		case 0:
			//owned (find out how to do this urself lol)
			break;
		}
		switch (Menu::Window.RageBotTab.AntiAimFakeYaw.GetIndex())
		{
		case 0:
			//owned (find out how to do this urself lol)
			break;
		}

		if (Menu::Window.RageBotTab.AntiAimYaw.GetIndex() != 0 && Menu::Window.RageBotTab.AntiAimFakeYaw.GetIndex() == 0)
		{
			pCmd->viewangles.y += G::RealAngles;
		}
		else if (Menu::Window.RageBotTab.AntiAimYaw.GetIndex() != 0 && Menu::Window.RageBotTab.AntiAimFakeYaw.GetIndex() != 0)
		{
			//owned (find out how to do this urself lol)
		}

		if (Menu::Window.RageBotTab.AntiAimDynamic.GetState())
		BreakLBY(pCmd, bSendPacket);
	}
}

bool EdgeAntiAim(IClientEntity* pLocalBaseEntity, CUserCmd* pCmd, float flWall, float flCornor)
	{
		Ray_t ray;
		trace_t tr;

		CTraceFilter traceFilter;
		traceFilter.pSkip = pLocalBaseEntity;

		auto bRetVal = false;
		auto vecCurPos = pLocalBaseEntity->GetOrigin() + pLocalBaseEntity->GetViewOffset();

		for (float i = 0; i < 360; i++)
		{
			Vector vecDummy(10.f, pCmd->viewangles.y, 0.f);
			vecDummy.y += i;

			vecDummy.NormalizeInPlace();

			Vector vecForward;
			AngleVectors(vecDummy, &vecForward);

			auto flLength = ((16.f + 3.f) + ((16.f + 3.f) * sin(DEG2RAD(10.f)))) + 7.f;
			vecForward *= flLength;

			ray.Init(vecCurPos, (vecCurPos + vecForward));
			Interfaces::Trace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);

			if (tr.fraction != 1.0f)
			{
				Vector qAngles;
				auto vecNegate = tr.plane.normal;

				vecNegate *= -1.f;
				VectorAngles(vecNegate, qAngles);

				vecDummy.y = qAngles.y;

				vecDummy.NormalizeInPlace();
				trace_t leftTrace, rightTrace;

				Vector vecLeft;
				AngleVectors(vecDummy + Vector(0.f, 30.f, 0.f), &vecLeft);

				Vector vecRight;
				AngleVectors(vecDummy - Vector(0.f, 30.f, 0.f), &vecRight);

				vecLeft *= (flLength + (flLength * sin(DEG2RAD(30.f))));
				vecRight *= (flLength + (flLength * sin(DEG2RAD(30.f))));

				ray.Init(vecCurPos, (vecCurPos + vecLeft));
				Interfaces::Trace->TraceRay(ray, MASK_SHOT, &traceFilter, &leftTrace);

				ray.Init(vecCurPos, (vecCurPos + vecRight));
				Interfaces::Trace->TraceRay(ray, MASK_SHOT, &traceFilter, &rightTrace);

				if ((leftTrace.fraction == 1.f) && (rightTrace.fraction != 1.f))
					vecDummy.y -= flCornor; // left
				else if ((leftTrace.fraction != 1.f) && (rightTrace.fraction == 1.f))
					vecDummy.y += flCornor; // right           

				pCmd->viewangles.y = vecDummy.y;
				pCmd->viewangles.y -= flWall;
				bRetVal = true;
			}
		}

		return bRetVal;
	}

	void EdgeOut(IClientEntity* pLocal, float &fYawOut)
	{
		Vector vEyePos = pLocal->GetOrigin() + pLocal->GetViewOffset();

		CTraceFilter filter;
		filter.pSkip = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		for (int y = 0; y < 360; y++)
		{
			Vector qTmp(10.0f, fYawOut, 0.0f);
			qTmp.y += y;

			if (qTmp.y > 180.0)
				qTmp.y -= 360.0;
			else if (qTmp.y < -180.0)
				qTmp.y += 360.0;

			GameUtils::NormaliseViewAngle(qTmp);

			Vector vForward;

			VectorAngles(qTmp, vForward);

			float fLength = (19.0f + (19.0f * sinf(DEG2RAD(10.0f)))) + 7.0f;
			vForward *= fLength;

			trace_t tr;

			Vector vTraceEnd = vEyePos + vForward;

			Ray_t ray;

			ray.Init(vEyePos, vTraceEnd);
			Interfaces::Trace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &tr);

			if (tr.fraction != 1.0f)
			{
				Vector angles;

				Vector vNegative = Vector(tr.plane.normal.x * -1.0f, tr.plane.normal.y * -1.0f, tr.plane.normal.z * -1.0f);

				VectorAngles(vNegative, angles);

				GameUtils::NormaliseViewAngle(angles);

				qTmp.y = angles.y;

				GameUtils::NormaliseViewAngle(qTmp);

				trace_t trLeft, trRight;

				Vector vLeft, vRight;
				VectorAngles(qTmp + Vector(0.0f, 30.0f, 0.0f), vLeft);
				VectorAngles(qTmp + Vector(0.0f, 30.0f, 0.0f), vRight);

				vLeft *= (fLength + (fLength * sinf(DEG2RAD(30.0f))));
				vRight *= (fLength + (fLength * sinf(DEG2RAD(30.0f))));

				vTraceEnd = vEyePos + vLeft;

				ray.Init(vEyePos, vTraceEnd);
				Interfaces::Trace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &trLeft);

				vTraceEnd = vEyePos + vRight;

				ray.Init(vEyePos, vTraceEnd);
				Interfaces::Trace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &trRight);

				if ((trLeft.fraction == 1.0f) && (trRight.fraction != 1.0f))
					qTmp.y -= 90.f;
				else if ((trLeft.fraction != 1.0f) && (trRight.fraction == 1.0f))
					qTmp.y += 90.f;

				if (qTmp.y > 180.0)
					qTmp.y -= 360.0;
				else if (qTmp.y < -180.0)
					qTmp.y += 360.0;

				fYawOut = qTmp.y;

				if (fYawOut > 180.0)
					fYawOut -= 360.0;
				else if (fYawOut < -180.0)
					fYawOut += 360.0;

				return;
			}
		}

		fYawOut -= 180.0f;

		if (fYawOut > 180.0)
			fYawOut -= 360.0;
		else if (fYawOut < -180.0)
			fYawOut += 360.0;
	}

	
