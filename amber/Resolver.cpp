#include "Resolver.h"
#include "Hooks.h"
#include "Globals.h"

int StepResolver::iTargetID;
IClientEntity* StepResolver::iTarget;
int StepResolver::Shots = 0;
bool StepResolver::change = true;

std::vector<AngleFix> R::angleFixArray;
std::vector<Resolver> R::resolverArray;
std::map<int, ResolveData> R::PlayerData;

bool resetpl = true;
int iPlayerCount;
int iOldPlayerCount;

//note: this resolver is very bad lool
//in actuality it doesn't even include a proper resolver

void AngleFix::StartResolver()
{
	if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected() && hackManager.pLocal())
	{
		for each (AngleFix current in R::angleFixArray)
		{
			IClientEntity* ent = (IClientEntity*)Interfaces::EntList->GetClientEntity(current.index);
			if (!ent || ent == hackManager.pLocal() || ent->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer || !ent->IsAlive()) continue;
			ent->GetEyeAngles()->y = current.realAngles.y;
			ent->GetEyeAngles()->x = current.realAngles.x;
		}
	}
}

bool IsAbleToMofuckinShoot(IClientEntity* pLocal)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle((HANDLE)pLocal->GetActiveWeaponHandle());

	if (!pLocal)
		return false;

	if (!pWeapon)
		return false;

	float flServerTime = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;

	return (!(pWeapon->GetNextPrimaryAttack() > flServerTime));
}

void R::CheckAddPlayers()
{
	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity* ent = (IClientEntity*)Interfaces::EntList->GetClientEntity(i);
		if (!ent || ent == hackManager.pLocal() || ent->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer || !ent->IsAlive()) continue;
		iPlayerCount += 1;
	}

	resetpl = iOldPlayerCount != iPlayerCount ? true : false;

	if (resetpl)
		iPlayerCount = 0;
}

void R::AddPlayers()
{
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (resetpl)
	{
		R::resolverArray.clear();
		for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
		{
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

			if (!pEntity || pEntity->IsDormant() || !pEntity->IsAlive())
				continue;

			if (pEntity->GetTeamNum() == pLocal->GetTeamNum() || !pLocal->IsAlive())
				continue;

			if (!pEntity || pEntity == hackManager.pLocal() || pEntity->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer || !pEntity->IsAlive()) continue;
			R::resolverArray.push_back(Resolver(0, pEntity->GetLBY(), i));
		}
		resetpl = false;
	}
}

/*==================================================================================
  _   _ ________          __  _____  ______  _____  ____  _ __      ________ _____  
 | \ | |  ____\ \        / / |  __ \|  ____|/ ____|/ __ \| |\ \    / /  ____|  __ \ 
 |  \| | |__   \ \  /\  / /  | |__) | |__  | (___ | |  | | | \ \  / /| |__  | |__) |
 | . ` |  __|   \ \/  \/ /   |  _  /|  __|  \___ \| |  | | |  \ \/ / |  __| |  _  / 
 | |\  | |____   \  /\  /    | | \ \| |____ ____) | |__| | |___\  /  | |____| | \ \ 
 |_| \_|______|   \/  \/     |_|  \_\______|_____/ \____/|______\/   |______|_|  \_\

==================================================================================*/

static void pNormalizeAngle(float& flAngle)
{
	if (std::isnan(flAngle)) flAngle = 0.0f;
	if (std::isinf(flAngle)) flAngle = 0.0f;

	float flRevolutions = flAngle / 360;

	if (flAngle > 180 || flAngle < -180)
	{
		if (flRevolutions < 0)
			flRevolutions = -flRevolutions;

		flRevolutions = round(flRevolutions);

		if (flAngle < 0)
			flAngle = (flAngle + 360 * flRevolutions);
		else
			flAngle = (flAngle - 360 * flRevolutions);
	}
}
static void pNormalizeAngles(QAngle &angles)
{
	pNormalizeAngle(angles.x);
	pNormalizeAngle(angles.y);
	angles.z = 0;
}

static void pClampAngles(QAngle &angles)
{
	if (std::isnan(angles.x)) angles.x = 0.0f;
	if (std::isnan(angles.y)) angles.y = 0.0f;
	if (std::isnan(angles.z)) angles.z = 0.0f;
	if (std::isinf(angles.x)) angles.x = 0.0f;
	if (std::isinf(angles.y)) angles.y = 0.0f;
	if (std::isinf(angles.z)) angles.z = 0.0f;

	if (angles.x > 89.0f && angles.x <= 180.0f)
		angles.x = 89.0f;
	while (angles.x > 180.0f)
		angles.x -= 360.0f;
	while (angles.x < -89.0f)
		angles.x = -89.0f;
	while (angles.y > 180.0f)
		angles.y -= 360.0f;
	while (angles.y < -180.0f)
		angles.y += 360.0f;

	angles.z = 0;
}

static void pClamp(QAngle &angles)
{
	pNormalizeAngles(angles);
	pClampAngles(angles);
}

int GetLowsestDenominator(int a, int b)
{
	for (int i = 2; i <= min(a, b); i++)
		if ((a % i == 0) && (b % i == 0)) { return i; }
	return 1;
}

int GetServerSideID(int EntityIndex)
{
	player_info_t pInfo;
	Interfaces::Engine->GetPlayerInfo(EntityIndex, &pInfo);
	return pInfo.userID;
}

void YawStep(QAngle& StepAngle)
{
	//do this urself
}

void PitchStep(QAngle& StepAngle)
{
	//do this urself

}

void SingleStep(QAngle& StepAngle)
{
	//do this urself
}

void R::AddShot(int EntityIndex)
{
	int ID = GetServerSideID(EntityIndex);
	PlayerData[ID].MissedShots++;
	SingleStep(PlayerData[ID].LastAttemptedAngles);
}

void R::DamageCallback(IGameEvent* pEvent)
{
	//do this urself
}

void R::LowerBodyFix()
{
	//do this urself
	//just check if players velo is above 0 and is in the air, and if these conditions are true then set eyeangles to lby
}

void R::Resolve()
{
	//do this urself
}
