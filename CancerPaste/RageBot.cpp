#include "RageBot.h"
#include "RenderManager.h"
#include "Autowall.h"
#include <iostream>
#include "UTIL Functions.h"
#include "AntiAntiAim.h"

#include <stdlib.h>

#define TICK_INTERVAL			( Interfaces::Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )

void CRageBot::Init()
{
	IsAimStepping = false;
	IsLocked = false;
	TargetID = -1;
}

void CRageBot::Draw()
{

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

float InterpolationFix()
{
	static ConVar* cvar_cl_interp = Interfaces::CVar->FindVar("cl_interp");
	static ConVar* cvar_cl_updaterate = Interfaces::CVar->FindVar("cl_updaterate");
	static ConVar* cvar_sv_maxupdaterate = Interfaces::CVar->FindVar("sv_maxupdaterate");
	static ConVar* cvar_sv_minupdaterate = Interfaces::CVar->FindVar("sv_minupdaterate");
	static ConVar* cvar_cl_interp_ratio = Interfaces::CVar->FindVar("cl_interp_ratio");

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

float hitchance(IClientEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	//	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	float hitchance = 101;
	if (!pWeapon) return 0;
	if (Menu::Window.RageBotTab.AccuracyHitchance.GetValue() > 1)
	{
		float inaccuracy = pWeapon->GetInaccuracy();
		if (inaccuracy == 0) inaccuracy = 0.0000001;
		inaccuracy = 1 / inaccuracy;
		hitchance = inaccuracy;
	}
	return hitchance;
}

// (DWORD)g_pNetVars->GetOffset("DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
// You need something like this
bool CanOpenFire() // Creds to untrusted guy
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

void CRageBot::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocalEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalEntity)
		return;

	// Master switch
	if (!Menu::Window.RageBotTab.Active.GetState())
		return;

	// Anti Aim 
	if (Menu::Window.RageBotTab.AntiAimEnable.GetState())
	{
		static int ChokedPackets = -1;

		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
		if (!pWeapon)
			return;

		if (ChokedPackets < 1 && pLocalEntity->GetLifeState() == LIFE_ALIVE && pCmd->buttons & IN_ATTACK && CanOpenFire() && GameUtils::IsBallisticWeapon(pWeapon))
		{
			bSendPacket = false;
		}
		else
		{
			if (pLocalEntity->GetLifeState() == LIFE_ALIVE)
			{
				DoAntiAim(pCmd, bSendPacket);
			}
			ChokedPackets = -1;
		}
	}

	// Aimbot
	if (Menu::Window.RageBotTab.AimbotEnable.GetState())
		DoAimbot(pCmd, bSendPacket);

	// Recoil
	if (Menu::Window.RageBotTab.AccuracyRecoil.GetState())
		DoNoRecoil(pCmd);

	// Aimstep
	if (Menu::Window.RageBotTab.AimbotAimStep.GetState())
	{
		Vector AddAngs = pCmd->viewangles - LastAngle;
		if (AddAngs.Length2D() > 25.f)
		{
			Normalize(AddAngs, AddAngs);
			AddAngs *= 25;
			pCmd->viewangles = LastAngle + AddAngs;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}

	LastAngle = pCmd->viewangles;
}

Vector BestPoint(IClientEntity *targetPlayer, Vector &final)
{
	IClientEntity* pLocal = hackManager.pLocal();

	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = targetPlayer;
	ray.Init(final + Vector(0, 0, 10), final);
	Interfaces::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	final = tr.endpos;
	return final;
}

// Functionality
void CRageBot::DoAimbot(CUserCmd *pCmd,bool &bSendPacket) // Ragebot, Aimbot
{
	IClientEntity* pTarget = nullptr;
	IClientEntity* pLocal = hackManager.pLocal();
	Vector Start = pLocal->GetViewOffset() + pLocal->GetOrigin();
	IClientEntity *pEntity = nullptr;
	bool FindNewTarget = true;
	//IsLocked = false;

	CSWeaponInfo* weapInfo = ((CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle()))->GetCSWpnData();

	// Don't aimbot with the knife..
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (GameUtils::IsRevolver(pWeapon) && Menu::Window.RageBotTab.AimbotAutoRevolver.GetState())
	{
		static int delay = 0;
		delay++;

		if (delay <= 15)
			pCmd->buttons |= IN_ATTACK;
		else
			delay = 0;
	}

	/* if (GameUtils::IsRevolver(pWeapon) && Menu::Window.RageBotTab.AimbotAutoRevolver.GetState())
	{
		if (pCmd->buttons |= IN_ATTACK)
			AimAtPoint(pLocal, AimPoint, pCmd, bSendPacket);

		float flPostponeFireReady = pWeapon->GetFireReadyTime();
		if (flPostponeFireReady > 0 && flPostponeFireReady < Interfaces::Globals->curtime)
		{
			pCmd->buttons &= ~IN_ATTACK;
		}
	} */

	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || !GameUtils::IsBallisticWeapon(pWeapon))
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
			TargetID = GetTargetDistance();
			break;
		case 2:
			TargetID = GetTargetHealth();
			break;
		}

		// Memes
		if (TargetID >= 0)
		{
			pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		}
		else
		{
			pTarget = nullptr;
			HitBox = -1;
		}
	} 

	Globals::Target = pTarget;
	Globals::TargetID = TargetID;

	// If we finally have a good target
	if (TargetID >= 0 && pTarget)
	{
		// Get the hitbox to shoot at
		HitBox = HitScan(pTarget);

		if (!CanOpenFire())
			return;

		// Key
		if (Menu::Window.RageBotTab.AimbotKeyPress.GetState())
		{
			int Key = Menu::Window.RageBotTab.AimbotKeyBind.GetKey();
			if (Key >= 0 && !GUI.GetKeyState(Key))
			{
				TargetID = -1;
				pTarget = nullptr;
				HitBox = -1;
				return;
			}
		}

		// Stop key
		int StopKey = Menu::Window.RageBotTab.AimbotStopKey.GetKey();
		if (StopKey >= 0 && GUI.GetKeyState(StopKey))
		{
			TargetID = -1;
			pTarget = nullptr;
			HitBox = -1;
			return;
		}

		float pointscale = Menu::Window.RageBotTab.TargetPointscale.GetValue(); // Pointscale
		// float value = Menu::Window.RageBotTab.AccuracyHitchance.GetValue();
		// Hitchance
		Vector Point;
		Vector AimPoint = GetHitboxPosition(pTarget, HitBox) + Vector(0, 0, pointscale);

		if (Menu::Window.RageBotTab.TargetMultipoint.GetState())
		{
			Point = BestPoint(pTarget, AimPoint);
		}
		else
		{
			Point = AimPoint;
		}

		// Velocity Prediction
		pTarget->GetPredicted(AimPoint);

		if (GameUtils::IsScopedWeapon(pWeapon) && !pWeapon->IsScoped() && Menu::Window.RageBotTab.AccuracyAutoScope.GetState()) // Autoscope
		{
			pCmd->buttons |= IN_ATTACK2;
		}
		else
		{
			if ((Menu::Window.RageBotTab.AccuracyHitchance.GetValue() * 1.5 <= hitchance(pLocal, pWeapon)) || Menu::Window.RageBotTab.AccuracyHitchance.GetValue() == 0 || *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() == 64)
			{
				if (AimAtPoint(pLocal, Point, pCmd, bSendPacket))
				{
					if (Menu::Window.RageBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
					{
						pCmd->buttons |= IN_ATTACK;
					}
					else
					{
						return;
					}
				}
				else if (Menu::Window.RageBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
				{
					pCmd->buttons |= IN_ATTACK;
				}
			}
		}

		if (IsAbleToShoot(pLocal) && pCmd->buttons & IN_ATTACK)
			Globals::Shots += 1;

		// Stop and Crouch
		if (TargetID >= 0 && pTarget)
		{
			if (Menu::Window.RageBotTab.AccuracyAutoStop.GetState())
			{
				pCmd->forwardmove = 0.f;
				pCmd->sidemove = 0.f;
				pCmd->buttons |= IN_DUCK;
			}
		}
	}

	// Auto Pistol
	if (GameUtils::IsPistol(pWeapon) && Menu::Window.RageBotTab.AimbotAutoPistol.GetState())
	{
		if (pCmd->buttons & IN_ATTACK)
		{
			static bool WasFiring = false;
			WasFiring = !WasFiring;
			
			if (WasFiring)
			{
				pCmd->buttons |= IN_ATTACK2;
			}
		}
	}
}

bool CRageBot::TargetMeetsRequirements(IClientEntity* pEntity)
{
	// Is a valid player
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{
		// Entity Type checks
		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && Interfaces::Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			// Team Check
			if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || Menu::Window.RageBotTab.TargetFriendlyFire.GetState())
			{
				// Spawn Check
				if (!pEntity->HasGunGameImmunity())
				{
					return true;
				}
			}
		}
	}

	// They must have failed a requirement
	return false;
}

float CRageBot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
{
	// Anything past 180 degrees is just going to wrap around
	CONST FLOAT MaxDegrees = 180.0f;

	// Get local angles
	Vector Angles = View;

	// Get local view / eye position
	Vector Origin = ViewOffSet;

	// Create and intiialize vectors for calculations below
	Vector Delta(0, 0, 0);
	//Vector Origin(0, 0, 0);
	Vector Forward(0, 0, 0);

	// Convert angles to normalized directional forward vector
	AngleVectors(Angles, &Forward);
	Vector AimPos = GetHitboxPosition(pEntity, aHitBox);
	// Get delta vector between our local eye position and passed vector
	VectorSubtract(AimPos, Origin, Delta);
	//Delta = AimPos - Origin;

	// Normalize our delta vector
	Normalize(Delta, Delta);

	// Get dot product between delta position and directional forward vectors
	FLOAT DotProduct = Forward.Dot(Delta);

	// Time to calculate the field of view
	return (acos(DotProduct) * (MaxDegrees / PI));
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

int CRageBot::GetTargetDistance()
{
	// New Distance Aimbot
	int target = -1;
	int minDist = 8192;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				Vector Difference = pLocal->GetOrigin() - pEntity->GetOrigin();
				int Distance = Difference.Length();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Distance < minDist && fov < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					minDist = Distance;
					target = i;
				}
			}
		}
	}
	return target;

	/* 
	// Target selection
	int target = -1;
	int minDist = 9999999999;

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
				Vector Difference = pLocal->GetOrigin() - pEntity->GetOrigin();
				int Distance = Difference.Length();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Distance < minDist && fov < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					minDist = Distance;
					target = i;
				}
			}
		}
	}

	return target;
	*/
}

int CRageBot::GetTargetHealth()
{
	int target = -1;
	int minHealth = 101;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
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
	/*
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
	*/
}

int CRageBot::HitScan(IClientEntity* pEntity)
{
	std::vector<int> HitBoxesToScan;
	bool AWall = Menu::Window.RageBotTab.AccuracyAutoWall.GetState();

	// Get the hitboxes to scan
	int HitScanMode = Menu::Window.RageBotTab.TargetHitscan.GetIndex();
	if (HitScanMode == 0)
	{
		// No Hitscan, just a single hitbox
		switch (Menu::Window.RageBotTab.TargetHitbox.GetIndex())
		{
		case 0:
			HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
			break;
		case 1:
			HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
			HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
			break;
		case 2:
			HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
			break;
		case 3:
			HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
			break;
		case 4:
			HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
			break;
		}
	}
	else
	{
		switch (HitScanMode)
		{
		case 0:
			// Nothing
			break;
		case 1:
			// Head only
			HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
			break;
		case 2:
			// Upperbody Meme
			HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
		case 3:
			// Everything Meme
			HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
			HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
			HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
		}
	}

	for (auto HitBoxID : HitBoxesToScan)
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

void CRageBot::DoNoRecoil(CUserCmd *pCmd)
{
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

bool CRageBot::AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket)
{
	bool ReturnValue = false;
	if (point.Length() == 0) return ReturnValue;
	Vector angles;
	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();
	CalcAngle(src, point, angles);
	GameUtils::NormaliseViewAngle(angles);
	IsLocked = true;

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
	if (Menu::Window.RageBotTab.AimbotSilentAim.GetState() && !Menu::Window.RageBotTab.AimbotPerfectSilentAim.GetState())
	{
		pCmd->viewangles = angles;
	}

	// Normal Aim
	if (!Menu::Window.RageBotTab.AimbotSilentAim.GetState() && !Menu::Window.RageBotTab.AimbotPerfectSilentAim.GetState())
	{
		Interfaces::Engine->SetViewAngles(angles);
	}

	// pSilent Aim 
	Vector Oldview = pCmd->viewangles;

	if (Menu::Window.RageBotTab.AimbotPerfectSilentAim.GetState())
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
	}
	return ReturnValue;
}

namespace AntiAims
{
	static bool flip = false;
	// Pitches
	void StaticPitch(CUserCmd *pCmd)
	{
		// Up
		pCmd->viewangles.x = 89.f;
	}

	void JitterPitch(CUserCmd *pCmd)
	{
		static bool up = true;
		if (up) pCmd->viewangles.x = -89.f;
		else pCmd->viewangles.x = 89.f;
		up = !up;
	}
	void Emotion2(CUserCmd *pCmd, bool& bSendPacket) {
		pCmd->viewangles.x = -89.f;
		if (bSendPacket)
			pCmd->viewangles.x = 89.f;
	}
	void clickbait(CUserCmd *pCmd)
	{
		{
			int random = rand() % 100;
			int random2 = rand() % 1000;

			static bool dir;
			static float current_x = pCmd->viewangles.x;

			if (random == 1) dir = !dir;

			if (dir)
				current_x += 700;
			else
				current_x -= 34;

			pCmd->viewangles.x = current_x;

			if (random == random2)
				pCmd->viewangles.x += random;

		}

	}

	// Yaws
	void FastSpint(CUserCmd *pCmd)
	{
		int random = rand() % 100;
		int random2 = rand() % 1000;

		static bool dir;
		static float current_y = pCmd->viewangles.y;

		if (random == 1) dir = !dir;

		if (dir)
			current_y += 100;
		else
			current_y -= 100;

		pCmd->viewangles.y = current_y;

		if (random == random2)
			pCmd->viewangles.y += random;
	}

	void SlowSpin(CUserCmd *pCmd)
	{
		int random = rand() % 100;
		int random2 = rand() % 1000;

		static bool dir;
		static float current_y = pCmd->viewangles.y;

		if (random == 1) dir = !dir;

		if (dir)
			current_y += 10;
		else
			current_y -= 10;

		pCmd->viewangles.y = current_y;

		if (random == random2)
			pCmd->viewangles.y += random;

	}
	void fakebackwards(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			pCmd->viewangles.y -= 180; // this is real yaw
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y -= 90; // this is fake aa
			ChokedPackets = -1;
		}
	}

	void BackJitter(CUserCmd *pCmd)
	{
		int random = 179 - rand() % 100;
		static float current_y = pCmd->viewangles.y;
		current_y += random;
		pCmd->viewangles.y = current_y;

	}
	void MoveFix(CUserCmd *cmd, Vector &realvec)
	{
		Vector vMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
		float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
		Vector vMove2;
		VectorAngles(vMove, vMove2);

		flYaw = DEG2RAD(cmd->viewangles.y - realvec.y + vMove2.y);
		cmd->forwardmove = cos(flYaw) * flSpeed;
		cmd->sidemove = sin(flYaw) * flSpeed;

		if (cmd->viewangles.x < -90.f || cmd->viewangles.x > 90.f)
			cmd->forwardmove = -cmd->forwardmove;
	}

	void Flip(CUserCmd *pCmd)
	{
		static bool back = false;
		back = !back;
		if (back)
			pCmd->viewangles.y -= rand() % 100;
		else
			pCmd->viewangles.y += rand() % 100;

	}
}

void CRageBot::aimAtPlayer(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (!pLocal || !pWeapon)
		return;

	Vector eye_position = pLocal->GetEyePosition();

	float best_dist = pWeapon->GetCSWpnData()->m_flRange;

	IClientEntity* target = nullptr;

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			if (Globals::TargetID != -1)
				target = Interfaces::EntList->GetClientEntity(Globals::TargetID);
			else
				target = pEntity;

			Vector target_position = target->GetEyePosition();

			float temp_dist = eye_position.DistTo(target_position);

			if (best_dist > temp_dist)
			{
				best_dist = temp_dist;
				CalcAngle(eye_position, target_position, pCmd->viewangles);
			}
		}
	}
}

void VectorAngles2(const Vector &vecForward, Vector &vecAngles)
{
	Vector vecView;
	if (vecForward[1] == 0.f && vecForward[0] == 0.f)
	{
		vecView[0] = 0.f;
		vecView[1] = 0.f;
	}
	else
	{
		vecView[1] = atan2(vecForward[1], vecForward[0]) * 180.f / M_PI;

		if (vecView[1] < 0.f)
			vecView[1] += 360.f;

		vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);

		vecView[0] = atan2(vecForward[2], vecView[2]) * 180.f / M_PI;
	}

	vecAngles[0] = -vecView[0];
	vecAngles[1] = vecView[1];
	vecAngles[2] = 0.f;
}
void AngleVectors2(const Vector& qAngles, Vector& vecForward)
{
	float sp, sy, cp, cy;
	SinCos((float)(qAngles[1] * (M_PI / 180.f)), &sy, &cy);
	SinCos((float)(qAngles[0] * (M_PI / 180.f)), &sp, &cp);

	vecForward[0] = cp*cy;
	vecForward[1] = cp*sy;
	vecForward[2] = -sp;
}

namespace AntiAims // CanOpenFire checks for fake anti aims?
{


	/*
	________________________________________________________
	______             ______
	/      |           /      \
	$$$$$$/  _______  /$$$$$$  |______    _______
	$$ |  /       \ $$ |_ $$//      \  /       |
	$$ |  $$$$$$$  |$$   |  /$$$$$$  |/$$$$$$$/
	$$ |  $$ |  $$ |$$$$/   $$ |  $$ |$$      \
	_$$ |_ $$ |  $$ |$$ |    $$ \__$$ | $$$$$$  |
	/ $$   |$$ |  $$ |$$ |    $$    $$/ /     $$/
	$$$$$$/ $$/   $$/ $$/      $$$$$$/  $$$$$$$/
	________________________________________________________
	*/
	/*
	- 4 Boxen namen siehe unten
	- bSendPacket = true  <-- Fake Angle
	- bSendPacket = false <-- True Angle
	-

	*/
	/*
	__________________________________________________________________________________________________________________________________________________________________________
	_______                       __        _______   __    __                __
	/       \                     /  |      /       \ /  |  /  |              /  |
	$$$$$$$  |  ______    ______  $$ |      $$$$$$$  |$$/  _$$ |_     _______ $$ |____
	$$ |__$$ | /      \  /      \ $$ |      $$ |__$$ |/  |/ $$   |   /       |$$      \
	$$    $$< /$$$$$$  | $$$$$$  |$$ |      $$    $$/ $$ |$$$$$$/   /$$$$$$$/ $$$$$$$  |
	$$$$$$$  |$$    $$ | /    $$ |$$ |      $$$$$$$/  $$ |  $$ | __ $$ |      $$ |  $$ |
	$$ |  $$ |$$$$$$$$/ /$$$$$$$ |$$ |      $$ |      $$ |  $$ |/  |$$ \_____ $$ |  $$ |
	$$ |  $$ |$$       |$$    $$ |$$ |      $$ |      $$ |  $$  $$/ $$       |$$ |  $$ |
	$$/   $$/  $$$$$$$/  $$$$$$$/ $$/       $$/       $$/    $$$$/   $$$$$$$/ $$/   $$/
	___________________________________________________________________________________________________________________________________________________________________________
	*/

	void Emotion(CUserCmd *pCmd)
	{
		pCmd->viewangles.x = 90.f;
	}

	void Down(CUserCmd *pCmd)
	{
		pCmd->viewangles.x = 89.0f;
	}

	void FakeDown(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			pCmd->viewangles.x = 89;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.x = -89.0f;
			ChokedPackets = -1;
		}
	}

	void Up(CUserCmd *pCmd)
	{
		pCmd->viewangles.x = -89.0f;
	}

	void FakeUp(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			pCmd->viewangles.x = 89;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.x = -89.0f;
			ChokedPackets = -1;
		}
	}

	void Zero(CUserCmd *pCmd)
	{
		pCmd->viewangles.x = 0.f;
	}

	void FakeZero(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false; //true angle
			pCmd->viewangles.x = -89;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.x = 0.f; //fake angle
			ChokedPackets = -1;
		}
	}

	void Mixed(CUserCmd *pCmd) //Test
	{
		static float pDance = 0.0f;

		pDance += 45.0f;
		if (pDance > 100)
			pDance = 0.0f;
		else if (pDance > 75.f)
			pCmd->viewangles.x = -11.295f;
		else if (pDance < 45.f)
			pCmd->viewangles.x = 26.3491651f;
		else if (pDance < 65)
			pCmd->viewangles.x = -9.91545f;
		else if (pDance < 75.f)
			pCmd->viewangles.x = 67.945324f;
		else if (pDance < 89.f)
			pCmd->viewangles.x = -72.62294519f;
		else if (pDance < 85.f)
			pCmd->viewangles.x = 35.19245635f;
	}

	void LegitFake(CUserCmd *pCmd, bool &bSendPacket) //Bool sind Zahlen und INT ist ja oder nein
	{
		static bool choke = false;
		if (choke)
		{
			bSendPacket = false; //true angle
			pCmd->viewangles.x = -90.f;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.x = 0.f; //fake angle
		}
		choke = !choke;
	}

	void LegitFakeJitter(CUserCmd *pCmd, bool &bSendPacket)
	{
		static bool choke = false;
		static bool s = false;
		if (choke)
		{
			bSendPacket = false; //true angle
			if (s) {
				pCmd->viewangles.x = 90.f;
			}
			else {
				pCmd->viewangles.x = -90.f;
			}
			s = !s;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.x = 0.f; //fake angle
		}
		choke = !choke;
	}

	/* void CustomPitch(CUserCmd *pCmd)
	{
		static bool custompitchX;
		int pitchOffset = Menu::Window.RageBotTab2.CustomPitch.GetValue(); //AA Offset

		if (custompitchX = true)
		{
			pCmd->viewangles.x = pitchOffset;
		}

	} */

	/*
	___________________________________________________________________________________________________________________________________________________________________________
	_______                       __        __      __
	/       \                     /  |      /  \    /  |
	$$$$$$$  |  ______    ______  $$ |      $$  \  /$$/______   __   __   __
	$$ |__$$ | /      \  /      \ $$ |       $$  \/$$//      \ /  | /  | /  |
	$$    $$< /$$$$$$  | $$$$$$  |$$ |        $$  $$/ $$$$$$  |$$ | $$ | $$ |
	$$$$$$$  |$$    $$ | /    $$ |$$ |         $$$$/  /    $$ |$$ | $$ | $$ |
	$$ |  $$ |$$$$$$$$/ /$$$$$$$ |$$ |          $$ | /$$$$$$$ |$$ \_$$ \_$$ |
	$$ |  $$ |$$       |$$    $$ |$$ |          $$ | $$    $$ |$$   $$   $$/
	$$/   $$/  $$$$$$$/  $$$$$$$/ $$/           $$/   $$$$$$$/  $$$$$/$$$$/
	___________________________________________________________________________________________________________________________________________________________________________
	*/
	int random;
	int maxJitter;
	float temp;
	//real yaw
	void Arizona(CUserCmd *pCmd) //Jitter Movement mit static bool Fast :)
	{
		static bool Fast = true;
		if (Fast)
		{
			pCmd->viewangles.y = pCmd->viewangles.y - 134.0;
			Fast = !Fast;
		}
		else
		{
			pCmd->viewangles.y = pCmd->viewangles.y - 226.0;
			Fast = !Fast;
		}
	}

	void Backwards(CUserCmd *pCmd)
	{
		pCmd->viewangles.y -= 180.0f;
	}

	void Forward(CUserCmd *pCmd)
	{
		pCmd->viewangles.y += 180.0f;
	}

	void SidewaysLeft(CUserCmd *pCmd)
	{
		pCmd->viewangles.y -= 90.0f;
	}

	void SidewaysRight(CUserCmd *pCmd)
	{
		pCmd->viewangles.y += 90.0f;
	}

	void SpinSlow(CUserCmd *pCmd)
	{

		int random = rand() % 100;
		int random2 = rand() % 1000;

		static bool dir;
		static float current_y = pCmd->viewangles.y;

		if (random == 1) dir = !dir;

		if (dir)
			current_y += 5.0f;
		else
			current_y -= 5.0f;

		pCmd->viewangles.y = current_y;

		if (random == random2)
			pCmd->viewangles.y += random;

	}

	void SpinFast(CUserCmd *pCmd)
	{
		int random = 160 + rand() % 40;
		static float current_y = pCmd->viewangles.y;
		current_y += random;
		pCmd->viewangles.y = current_y;
	}

	void RealSpinBot(CUserCmd *pCmd)
	{
		int speed = Menu::Window.RageBotTab.AntiAimSpinspeed.GetValue();
		float speedmulti = speed * 1000.0f;
		int yaw = Interfaces::Globals->curtime * speedmulti;
		pCmd->viewangles.y = yaw;
	}

	void FakeSpinBot(CUserCmd *pCmd, bool &bSendPacket)
	{
		bSendPacket = true;
		int speed = Menu::Window.RageBotTab.AntiAimSpinspeed.GetValue();
		float speedmulti = speed * 1000.0f;
		int yaw = Interfaces::Globals->curtime * speedmulti;
		pCmd->viewangles.y = yaw;
	}

	void JitterBackward(CUserCmd *pCmd)
	{
		pCmd->viewangles.y -= 180.0f;
		random = rand() % 100;
		maxJitter = rand() % (85 - 70 + 1) + 70;
		temp = maxJitter - (rand() % maxJitter);
		if (random < 35 + (rand() % 15))
			pCmd->viewangles.y -= temp;
		else if (random < 85 + (rand() % 15))
			pCmd->viewangles.y += temp;
	}

	void JitterForward(CUserCmd *pCmd)
	{
		pCmd->viewangles.y += 180.0f;
		random = rand() % 100;
		maxJitter = rand() % (85 - 70 + 1) + 70;
		temp = maxJitter - (rand() % maxJitter);
		if (random < 35 + (rand() % 15))
			pCmd->viewangles.y -= temp;
		else if (random < 85 + (rand() % 15))
			pCmd->viewangles.y += temp;
	}

	void JitterSideways(CUserCmd *pCmd)
	{
		pCmd->viewangles.y += 90.0f;
		random = rand() % 100;
		maxJitter = rand() % (85 - 70 + 1) + 70;
		temp = maxJitter - (rand() % maxJitter);
		if (random < 35 + (rand() % 15))
			pCmd->viewangles.y -= temp;
		else if (random < 85 + (rand() % 15))
			pCmd->viewangles.y += temp;
	}

	void YawZero(CUserCmd *pCmd)
	{
		pCmd->viewangles.y = 0.f;
	}

	/* void CustomYaw(CUserCmd *pCmd)
	{
		static bool custompitchY;
		int pitchOffset = Menu::Window.RageBotTab2.CustomYaw.GetValue(); //AA Offset

		if (custompitchY = true)
		{
			pCmd->viewangles.y = pitchOffset;
		}

	} */

	void LowerBody(CUserCmd *pCmd, bool &bSendPacket)
	{
		static bool f_flip = true;
		f_flip = !f_flip;

		if (f_flip)
		{
			pCmd->viewangles.y -= hackManager.pLocal()->GetLowerBodyYaw() + 90.00f;
			bSendPacket = true;
		}
		else if (!f_flip)
		{
			pCmd->viewangles.y += hackManager.pLocal()->GetLowerBodyYaw() - 90.00f;
			bSendPacket = false;
		}
		f_flip = !f_flip;
	}

	void LowerBodyBackwards(CUserCmd *pCmd, bool &bSendPacket)
	{
		pCmd->viewangles.y -= hackManager.pLocal()->GetLowerBodyYaw() + 180.00f;
	}

	void FakeSpin(CUserCmd* pCmd, bool& bSendPacket) {

		int random = rand() % 100;
		int random2 = rand() % 1000;
		static bool dir;
		static float current_y = pCmd->viewangles.y;
		IClientEntity* pLocal = hackManager.pLocal();
		float server_time = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;

		if (bSendPacket) {
			pCmd->viewangles.y = (float)(fmod(server_time / 0.39f * 360.0f, 360.0f));
		}
		else
		{
			if (random == 1) dir = !dir;
			if (dir)
				current_y -= 100.9;
			else
				current_y += 100.9;
			pCmd->viewangles.y = current_y;
			if (random == random2)
				pCmd->viewangles.y -= random;
		}
	}

	void FakeBackward(CUserCmd* pCmd, bool& bSendPacket) {

		if (pCmd->command_number % 2)
		{
			bSendPacket = false;
			pCmd->viewangles.y -= 58.f;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y += 180.f;
		}
	}

	void FakeForward(CUserCmd* pCmd, bool& bSendPacket) {

		if (pCmd->command_number % 2)
		{
			bSendPacket = false;
			pCmd->viewangles.y += 125.f;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y -= 0.f;
		}
	}

	void FakeStatic(CUserCmd* pCmd, bool& bSendPacket) {

		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			static int y2 = -179;
			int spinBotSpeedFast = 360.0f / 1.618033988749895f;;

			y2 += spinBotSpeedFast;

			if (y2 >= 179)
				y2 = -179;

			pCmd->viewangles.y = y2;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y -= 180;
			ChokedPackets = -1;
		}
	}



	void BreakLBY1(CUserCmd *pCmd)
	{
		int flip = (int)floorf(Interfaces::Globals->curtime / 1.1) % 2;
		pCmd->viewangles.y += -135.0f + flip * 135.0f;
		// Look backwards
		pCmd->viewangles.y += 180;
	}

	void BreakLBY2(CUserCmd *pCmd)
	{
		int flip = (int)floorf(Interfaces::Globals->curtime / 1.1) % 2;
		pCmd->viewangles.y += -90.0f + flip * 90.0f;
		// Look backwards
		pCmd->viewangles.y += 90.f;
	}

	void BreakLBY3(CUserCmd *pCmd)
	{
		int flip = (int)floorf(Interfaces::Globals->curtime / 1.1) % 2;
		pCmd->viewangles.y += 225.f + flip * 145.0f;
		// Look backwards
		pCmd->viewangles.y += 90.f;
	}

	void BreakLBY4(CUserCmd *pCmd)
	{
		int flip = (int)floorf(Interfaces::Globals->curtime / 1.1) % 2;
		pCmd->viewangles.y += hackManager.pLocal()->GetLowerBodyYaw() + 225.f + flip * 135.0f;
		// Look backwards
		pCmd->viewangles.y += hackManager.pLocal()->GetLowerBodyYaw() + 90.f;
	}

	void BreakLBY5(CUserCmd *pCmd)
	{
		int flip = (int)floorf(Interfaces::Globals->curtime / 1.1) % 2;
		pCmd->viewangles.y += hackManager.pLocal()->GetLowerBodyYaw() + 90.f + flip * 45.0f;
		// Look backwards
		pCmd->viewangles.y -= (hackManager.pLocal()->GetLowerBodyYaw() + 90.f);
	}

	void BreakLBY6(CUserCmd *pCmd)
	{
		int flip = (int)floorf(Interfaces::Globals->curtime / 1.1) % 2;
		pCmd->viewangles.y -= flip * 187.f;
		// Look backwards
		pCmd->viewangles.y += 90.f;
		pCmd->viewangles.y -= flip * 187.f;
	}

	void TestBreaker(CUserCmd *pCmd)
	{
		int meme = Menu::Window.RageBotTab.RealAntiAimYawSlider.GetValue();
		int flip = (int)floorf(Interfaces::Globals->curtime / 1.1) % 2;
		pCmd->viewangles.y += flip * meme;
		// Look backwards
		pCmd->viewangles.y += 90.f;
		pCmd->viewangles.y += flip * meme;
	}

	void TJitter(CUserCmd *pCmd)
	{
		static bool Turbo = true;
		if (Turbo)
		{
			pCmd->viewangles.y -= 90;
			Turbo = !Turbo;
		}
		else
		{
			pCmd->viewangles.y += 90;
			Turbo = !Turbo;
		}
	}

	void TFake(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			pCmd->viewangles.y = -90;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y = 90;
			ChokedPackets = -1;
		}
	}

	void JitterLBY(CUserCmd *pCmd, bool &bSendPacket)
	{
		static bool f_flip = true;
		f_flip = !f_flip;

		if (f_flip)
		{
			pCmd->viewangles.y -= hackManager.pLocal()->GetLowerBodyYaw() + 90.00f; //Yaw setzt sich zusammen aus LowerBody + 90° Drehung
			bSendPacket = false;
		}
		else if (!f_flip)
		{
			//pCmd->viewangles.y += hackManager.pLocal()->GetLowerBodyYaw() - 90.00f;
			bSendPacket = true;
		}
		f_flip = !f_flip;
	}

	void FJitter(CUserCmd* pCmd, bool& bSendPacket) {

		static int jitterangle = 0;

		if (jitterangle <= 1)
		{
			pCmd->viewangles.y += 135;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			pCmd->viewangles.y += 225;
		}

		static int iChoked = -1;
		iChoked++;
		if (iChoked < 1)
		{
			bSendPacket = false;
			if (jitterangle <= 1)
			{
				pCmd->viewangles.y += 45;
				jitterangle += 1;
			}
			else if (jitterangle > 1 && jitterangle <= 3)
			{
				pCmd->viewangles.y -= 45;
				jitterangle += 1;
			}
			else
			{
				jitterangle = 0;
			}
		}
		else
		{
			bSendPacket = true;
			iChoked = -1;
		}
	}

	void FakeLowerBody(CUserCmd *pCmd, bool &bSendPacket)
	{

		random = rand() % 100;
		maxJitter = rand() % (85 - 70 + 1) + 70;
		temp = hackManager.pLocal()->GetLowerBodyYaw() - (rand() % maxJitter);
		if (random < 35 + (rand() % 15))
		{
			bSendPacket = false;
			pCmd->viewangles.y -= temp;
		}

		else if (random < 85 + (rand() % 15))
		{
			bSendPacket = true;
			pCmd->viewangles.y += temp;
		}
	}

	void LegitFake1(CUserCmd *pCmd, bool &bSendPacket) //Bool sind Zahlen und INT ist ja oder nein
	{
		static bool choke = false;
		if (choke)
		{
			bSendPacket = false; //true angle
			pCmd->viewangles.y = -90.f;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y = 0.f; //fake angle
		}
		choke = !choke;
	}

	void LegitFakeJitter1(CUserCmd *pCmd, bool &bSendPacket)
	{
		static bool choke = false;
		static bool s = false;
		if (choke)
		{
			bSendPacket = false; //true angle
			if (s) {
				pCmd->viewangles.y = 90.f;
			}
			else {
				pCmd->viewangles.y = -90.f;
			}
			s = !s;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y = 0.f; //fake angle
		}
		choke = !choke;
	}

	void CustomPitch(CUserCmd *pCmd, bool &bSendPacket)
	{
		int realcustompitch = Menu::Window.RageBotTab.AntiAimPitchSlider.GetValue();
		pCmd->viewangles.y = realcustompitch;
	}

	void CustomRealYaw(CUserCmd *pCmd, bool &bSendPacket)
	{
		int realcustomyaw = Menu::Window.RageBotTab.RealAntiAimYawSlider.GetValue();
		pCmd->viewangles.y = realcustomyaw;
	}

	void FakeJitter(CUserCmd* pCmd, bool &bSendPacket)
	{
		static int jitterangle = 0;

		if (jitterangle <= 1)
		{
			pCmd->viewangles.y += 135;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			pCmd->viewangles.y += 225;
		}

		static int iChoked = -1;
		iChoked++;
		if (iChoked < 1)
		{
			bSendPacket = false;
			if (jitterangle <= 1)
			{
				pCmd->viewangles.y += 45;
				jitterangle += 1;
			}
			else if (jitterangle > 1 && jitterangle <= 3)
			{
				pCmd->viewangles.y -= 45;
				jitterangle += 1;
			}
			else
			{
				jitterangle = 0;
			}
		}
		else
		{
			bSendPacket = true;
			iChoked = -1;
		}
	}
	/* void CustomSlider(CUserCmd *pCmd, bool &bSendPacket)
	{
		float customrealyaw = Menu::Window.RageBotTab2.AASliderRY.GetValue();
		pCmd->viewangles.y = customrealyaw;
	} */


	/*
	___________________________________________________________________________________________________________________________________________________________________________
	________         __                        __      __
	/        |       /  |                      /  \    /  |
	$$$$$$$$/______  $$ |   __   ______        $$  \  /$$/______   __   __   __
	$$ |__  /      \ $$ |  /  | /      \        $$  \/$$//      \ /  | /  | /  |						bSendPacket = true  <-- Fake Angle
	$$    | $$$$$$  |$$ |_/$$/ /$$$$$$  |        $$  $$/ $$$$$$  |$$ | $$ | $$ |						bSendPacket = false <-- True Angle
	$$$$$/  /    $$ |$$   $$<  $$    $$ |         $$$$/  /    $$ |$$ | $$ | $$ |
	$$ |   /$$$$$$$ |$$$$$$  \ $$$$$$$$/           $$ | /$$$$$$$ |$$ \_$$ \_$$ |
	$$ |   $$    $$ |$$ | $$  |$$       |          $$ | $$    $$ |$$   $$   $$/
	$$/     $$$$$$$/ $$/   $$/  $$$$$$$/           $$/   $$$$$$$/  $$$$$/$$$$/
	___________________________________________________________________________________________________________________________________________________________________________
	*/
	//fake yaw

	void FakeCustomSlider(CUserCmd *pCmd, bool &bSendPacket)
	{
		bSendPacket = true;
		int fakecustomsliddder = Menu::Window.RageBotTab.FakeAntiAimYawSlider.GetValue();
		pCmd->viewangles.y = fakecustomsliddder;

	}

	void Arizona1(CUserCmd *pCmd, bool &bSendPacket) //Jitter Movement mit static bool Fast :)
	{
		static bool Fast = false;
		if (Fast)
		{
			bSendPacket = false; //true angle
			pCmd->viewangles.y = pCmd->viewangles.y - 134.0;
		}
		else
		{
			bSendPacket = true; //fake angle
			pCmd->viewangles.y = pCmd->viewangles.y - 226.0;
		}
		Fast = !Fast;
	}

	void Backwards1(CUserCmd *pCmd, bool &bSendPacket)
	{
		bSendPacket = true; //fake angle
		pCmd->viewangles.y -= 180.0f;
	}

	void Forward1(CUserCmd *pCmd, bool &bSendPacket)
	{
		bSendPacket = true; //fake angle
		pCmd->viewangles.y += 180.0f;
	}

	void SidewaysLeft1(CUserCmd *pCmd, bool &bSendPacket)
	{
		bSendPacket = true; //fake angle
		pCmd->viewangles.y -= 90.0f;
	}

	void SidewaysRight1(CUserCmd *pCmd, bool &bSendPacket)
	{
		bSendPacket = true; //fake angle
		pCmd->viewangles.y += 90.0f;
	}

	void SpinSlow1(CUserCmd *pCmd, bool &bSendPacket)
	{

		int random = rand() % 100;
		int random2 = rand() % 1000;

		static bool dir;
		static float current_y = pCmd->viewangles.y;

		if (random == 1) dir = !dir;

		if (dir)
			current_y += 5.0f;
		else
			current_y -= 5.0f;
		bSendPacket = true; //fake angle
		pCmd->viewangles.y = current_y;

		if (random == random2)
			pCmd->viewangles.y += random;

	}

	void SpinFast1(CUserCmd *pCmd, bool &bSendPacket)
	{
		int random = 160 + rand() % 40;
		static float current_y = pCmd->viewangles.y;
		current_y += random;
		bSendPacket = true; //fake angle
		pCmd->viewangles.y = current_y;
	}

	void JitterBackward1(CUserCmd *pCmd, bool &bSendPacket)
	{
		bSendPacket = true; //fake angle
		pCmd->viewangles.y += 180.0f;
		random = rand() % 100;
		maxJitter = rand() % (85 - 70 + 1) + 70;
		temp = maxJitter - (rand() % maxJitter);
		if (random < 35 + (rand() % 15))
			pCmd->viewangles.y -= temp;
		else if (random < 85 + (rand() % 15))
			pCmd->viewangles.y += temp;
	}

	void JitterForward1(CUserCmd *pCmd, bool &bSendPacket)
	{
		bSendPacket = true; //fake angle
		pCmd->viewangles.y += 180.0f;
		random = rand() % 100;
		maxJitter = rand() % (85 - 70 + 1) + 70;
		temp = maxJitter - (rand() % maxJitter);
		if (random < 35 + (rand() % 15))
			pCmd->viewangles.y -= temp;
		else if (random < 85 + (rand() % 15))
			pCmd->viewangles.y += temp;
	}

	void JitterSideways1(CUserCmd *pCmd, bool &bSendPacket)
	{
		bSendPacket = true; //fake angle
		pCmd->viewangles.y += 90.0f;
		random = rand() % 100;
		maxJitter = rand() % (85 - 70 + 1) + 70;
		temp = maxJitter - (rand() % maxJitter);
		if (random < 35 + (rand() % 15))
			pCmd->viewangles.y -= temp;
		else if (random < 85 + (rand() % 15))
			pCmd->viewangles.y += temp;
	}

	void YawZero1(CUserCmd *pCmd, bool &bSendPacket)
	{
		bSendPacket = true; //fake angle
		pCmd->viewangles.y = 0.f;
	}

	/* void CustomFakeYaw1(CUserCmd *pCmd, bool &bSendPacket)
	{
		int pitchOffset = Menu::Window.RageBotTab2.CustomYaw.GetValue(); //AA Offset
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false; // +=180?
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y = pitchOffset;
			ChokedPackets = -1;
		}
	} */

	void LowerBody1(CUserCmd *pCmd, bool &bSendPacket)
	{
		static bool f_flip = true;
		f_flip = !f_flip;

		if (f_flip)
		{
			pCmd->viewangles.y -= hackManager.pLocal()->GetLowerBodyYaw() + 45.00f;
			bSendPacket = false;
		}
		else if (!f_flip)
		{
			pCmd->viewangles.y += hackManager.pLocal()->GetLowerBodyYaw() - 45.00f;
			bSendPacket = true;
		}
		f_flip = !f_flip;
	}

	void FakeLowerBody1(CUserCmd *pCmd, bool &bSendPacket)
	{
		random = rand() % 100;
		maxJitter = rand() % (85 - 70 + 1) + 70;
		temp = hackManager.pLocal()->GetLowerBodyYaw() - (rand() % maxJitter);
		if (random < 35 + (rand() % 15))
		{
			bSendPacket = false;
			pCmd->viewangles.y -= temp;
		}

		else if (random < 85 + (rand() % 15))
		{
			bSendPacket = true;
			pCmd->viewangles.y += temp;
		}
	}
};

/* bool EdgeAntiAim(IClientEntity* pLocalBaseEntity, CUserCmd* cmd, float flWall, float flCornor)
{
	IClientEntity* pLocal = hackManager.pLocal();

	CTraceFilter traceFilter;
	traceFilter.pSkip = pLocal;

	bool bEdge = false;

	Vector angle;
	Vector eyePos = pLocal->GetOrigin() + pLocal->GetViewOffset();

	for (float i = 0; i < 360; i++)
	{
		Vector vecDummy(10.f, cmd->viewangles.y, 0.f);
		vecDummy.y += i;

		Vector forward = vecDummy.Forward();

		//vecDummy.NormalizeInPlace();

		float flLength = ((16.f + 3.f) + ((16.f + 3.f) * sin(DEG2RAD(10.f)))) + 7.f;
		forward *= flLength;

		Ray_t ray;
		CGameTrace tr;

		ray.Init(eyePos, (eyePos + forward));
		Interfaces::Trace->EdgeTraceRay(ray, traceFilter, tr, true);

		if (tr.fraction != 1.0f)
		{
			Vector negate = tr.plane.normal;
			negate *= -1;

			Vector vecAng = negate.Angle();

			vecDummy.y = vecAng.y;

			//vecDummy.NormalizeInPlace();
			trace_t leftTrace, rightTrace;

			Vector left = (vecDummy + Vector(0, 30, 0)).Forward();
			Vector right = (vecDummy - Vector(0, 30, 0)).Forward();

			left *= (flLength * cosf(rad(30)) * 2); //left *= (len * cosf(rad(30)) * 2);
			right *= (flLength * cosf(rad(30)) * 2); // right *= (len * cosf(rad(30)) * 2);

			ray.Init(eyePos, (eyePos + left));
			Interfaces::Trace->EdgeTraceRay(ray, traceFilter, leftTrace, true);

			ray.Init(eyePos, (eyePos + right));
			Interfaces::Trace->EdgeTraceRay(ray, traceFilter, rightTrace, true);

			if ((leftTrace.fraction == 1.f) && (rightTrace.fraction != 1.f))
			{
				vecDummy.y -= 45; // left
			}
			else if ((leftTrace.fraction != 1.f) && (rightTrace.fraction == 1.f))
			{
				vecDummy.y += 45; // right     
			}

			angle.y = vecDummy.y;
			angle.y += 360;
			bEdge = true;
		}
	}
	return bEdge;
} */
// AntiAim
void CRageBot::DoAntiAim(CUserCmd *pCmd, bool &bSendPacket) // pCmd->viewangles.y = 0xFFFFF INT_MAX or idk
{
	IClientEntity* pLocal = hackManager.pLocal();
	IClientEntity *pEntity;
	IClientEntity* pLocalEntity;

	if ((pCmd->buttons & IN_USE) || pLocal->GetMoveType() == MOVETYPE_LADDER)
		return;
	
	// If the aimbot is doing something don't do anything
	if ((IsAimStepping || pCmd->buttons & IN_ATTACK) && !Menu::Window.RageBotTab.AimbotPerfectSilentAim.GetState())
		return;

	// Weapon shit
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (pWeapon)
	{
		CSWeaponInfo* pWeaponInfo = pWeapon->GetCSWpnData();
		// Knives or grenades
		if (!GameUtils::IsBallisticWeapon(pWeapon))
		{
			if (Menu::Window.RageBotTab.AntiAimKnife.GetState())
			{
				if (!CanOpenFire() || pCmd->buttons & IN_ATTACK2)
					return;
			}
			else
			{
				return;
			}
		}
	}

	/* if (Menu::Window.RageBotTab.AntiAimEdge.GetState()) 
	{
	
	} */

	if (Menu::Window.RageBotTab.AntiAimTarget.GetState())
	{
		aimAtPlayer(pCmd);
	}

	// Anti-Aim Pitch
	switch (Menu::Window.RageBotTab.AntiAimPitch.GetIndex()) // Magic pitch is 69.69?
	{
	case 0:
		// No Pitch AA
		break;
	case 1:
		pCmd->viewangles.x = 89.0000;
		break;

	case 2:
		pCmd->viewangles.x = -991; /*Fake-Down*/
		break;

	case 3:
		pCmd->viewangles.x = 991; /*Fake-UP*/
		break;

	case 4:
		pCmd->viewangles.x = 1080; /*Zero-Down*/
		break;

	case 5:
		pCmd->viewangles.x = -1080; /*Zero-UP*/
		break;
	}

	//Anti-Aim Yaw
	switch (Menu::Window.RageBotTab.AntiAimYaw.GetIndex())
	{
	case 0:
		// No Yaw AA
		break;
	case 1:
		// Backward
		AntiAims::Backwards(pCmd);
		break;
	case 2:
		// Breaker 1
		AntiAims::BreakLBY1(pCmd);
		break;
	case 3:
		// Breaker 2
		AntiAims::BreakLBY2(pCmd);
		break;
	case 4:
		// Breaker 3
		AntiAims::BreakLBY3(pCmd);
		break;
	case 5:
		// Breaker 4
		AntiAims::TestBreaker(pCmd);
		break;
	case 6:
		// LBY Breaker 1
		AntiAims::BreakLBY4(pCmd);
		break;
	case 7:
		// LBY Breaker 1
		AntiAims::BreakLBY5(pCmd);
		break;
	case 8:
		AntiAims::LowerBody(pCmd, bSendPacket);
		break;
	case 9:
		AntiAims::LowerBodyBackwards(pCmd, bSendPacket);
		break;
	case 10:
		AntiAims::FakeLowerBody(pCmd, bSendPacket);
		break;
	case 11:
		AntiAims::FJitter(pCmd, bSendPacket);
		break;
	case 12:
		AntiAims::FakeSpin(pCmd, bSendPacket);
		break;
	case 13:
		AntiAims::FakeBackward(pCmd, bSendPacket);
		break;
	case 14:
		AntiAims::FakeForward(pCmd, bSendPacket);
		break;
	case 15:
		AntiAims::FakeStatic(pCmd, bSendPacket);
		break;
	case 16:
		AntiAims::TFake(pCmd, bSendPacket);
		break;
	case 17:
		AntiAims::TJitter(pCmd);
		break;
	}
}