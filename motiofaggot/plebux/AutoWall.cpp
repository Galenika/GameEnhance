#include "AutoWall.h"

float AutoWall::GetDamageAtPoint(Vector dst, int* Hitgroup)
{
	return SimulateFireBullet(C_CSPlayer::GetLocalPlayer()->GetEyePos(), dst, Hitgroup);
}

float AutoWall::GetHitgroupDamage(int iHitGroup)
{
	switch (iHitGroup)
	{
	case HITGROUP_HEAD: return 4.0f;
	case HITGROUP_STOMACH: return 1.5f;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG: return 0.75f;
	}
	return 1.0f;
}

float AutoWall::ScaleDamage(int Hitgroup, C_CSPlayer *pEntity, float weapon_armor_ratio, float current_damage)
{
	current_damage *= AutoWall::GetHitgroupDamage(Hitgroup);
	if (pEntity->GetArmor())
	{
		if (Hitgroup == HITGROUP_HEAD) { if (pEntity->HasHelmet()) { current_damage *= weapon_armor_ratio; } }
		else { current_damage *= weapon_armor_ratio; }
	}
	return current_damage;
}

bool AutoWall::HandleBulletPenetration(float WeaponPenetration, surfacedata_t* EnterSurfaceData, surfacedata_t* ExitSurfaceData, trace_t& Trace, float Thickness, int &HitsLeft, float& CurrentDamage)
{
	// what the fuck, the thickness gets stuck at multiples of 4
	Thickness = max(Thickness - 2.0f, 0);	// Compensate for imprecision
	if (!EnterSurfaceData || !ExitSurfaceData) { return true; }
	short EnterMaterial = EnterSurfaceData->game.material;
	short ExitMaterial = ExitSurfaceData->game.material;
	float SurfacePenetrationModifier = ExitSurfaceData->game.flPenetrationModifier;
	float SurfaceDamageModifier = ExitSurfaceData->game.flDamageModifier;
	if (HitsLeft <= 0) { return true; }
	float v49 = 1.0f;
	float v50 = 0.0f;
	if (Trace.surface.flags & 0x1)
	{
		v49 = 1.0f;
		v50 = 1.0f;
	}
	else
	{
		v49 = min(EnterSurfaceData->game.flPenetrationModifier, SurfacePenetrationModifier);
		v50 = 1.0f - min(EnterSurfaceData->game.flDamageModifier, SurfaceDamageModifier);
	}
	if (EnterMaterial == ExitMaterial && (ExitMaterial == 87 || ExitMaterial == 77)) { v49 *= 2.0f; }
	if (Thickness > v49 * WeaponPenetration * 64) { return true; }
	CurrentDamage *= v50;

	float v72 = 0.16f;
	float v37 = 0.0f;

	if (Trace.surface.flags & 0x1)
	{
		if (EnterMaterial != 89 && EnterMaterial != 71) { v37 = 1.0f; }
		else
		{
			v37 = 3.0f;
			v72 = 0.05f;
		}
	}
	else if (EnterMaterial != 89 && EnterMaterial != 71) { v37 = (EnterSurfaceData->game.flPenetrationModifier + ExitSurfaceData->game.flPenetrationModifier) / 2.0f; }

	if (EnterMaterial == ExitMaterial)
	{
		if (ExitMaterial == 87 || ExitMaterial == 85) { v37 = 3.0f; }
		else if (ExitMaterial == 76) { v37 = 2.0f; }
	}
	float v39 = max(1.0f / v37, 0.0f);
	float v43 = max((3.0f / WeaponPenetration) * 1.25f, 0.0f);
	float DamageLost = (v39 * 3.0f) * v43 + CurrentDamage * v72 + (pow(Thickness, 2) * v39 / 24.0f) / 2.0f;

	if (CurrentDamage - max(DamageLost, 0.0f) < 0.0f) { return true; }
	CurrentDamage -= max(DamageLost, 0.0f);

	HitsLeft--;
	return false;
}

float AutoWall::SimulateFireBullet(Vector StartPos, Vector Point, int* HitGroup)
{
	auto pLocal = C_CSPlayer::GetLocalPlayer();
	auto WpnData = pLocal->GetActiveWeapon()->GetCSWpnData();

	trace_t EnterTrace;
	CTraceFilter TraceFilter;
	TraceFilter.pSkip = pLocal;

	int PenetrationCount = 4;
	float Damage = WpnData->m_iDamage;

	float RemainingLength = 0;
	float CurrentLength = 0;
	bool Solid = false;

	Vector Direction = (Point - StartPos).Normalized();
	Vector Result;
	Vector LastPoint = Vector(0, 0, 0);
	Ray_t Ray;

	while (PenetrationCount && Config.g_iAutoWall_MinDamage > 0)
	{
		RemainingLength = WpnData->m_flRange - CurrentLength;
		Vector Dest = StartPos + Direction * 64.f;

		Ray.Init(StartPos, Dest + Direction * RemainingLength);
		Interfaces::EngineTrace()->TraceRay(Ray, 0x4600400B, &TraceFilter, &EnterTrace);
		if (EnterTrace.fraction == 1.0f) { break; }

		if (CurrentLength < WpnData->m_flRange)
		{
			if (EnterTrace.hitgroup)
			{
				if (EnterTrace.m_pEnt && EnterTrace.m_pEnt->GetClientClass()->m_ClassID == EClassIds::CCSPlayer)
				{
					if (static_cast<C_CSPlayer*>(EnterTrace.m_pEnt)->GetTeamNum() == pLocal->GetTeamNum()) { return 0; }
					float ScaledDamage = ScaleDamage(EnterTrace.hitgroup, static_cast<C_CSPlayer*>(EnterTrace.m_pEnt), WpnData->m_flArmorRatio, Damage);
					if (HitGroup) *HitGroup = EnterTrace.hitgroup;
					return ScaledDamage;
				}
			}
			else
			{
				trace_t Result;
				Vector Dummy;
				if (!Utils::TraceToExit(Dummy, EnterTrace, EnterTrace.endpos, Direction, &Result)) { break; }
				Damage *= pow(WpnData->m_flRangeModifier, CurrentLength * 0.002);
				if (Result.startpos.IsValid() && EnterTrace.endpos.IsValid())
				{
					surfacedata_t* EnterSurfaceData = Interfaces::PhysProps()->GetSurfaceData(EnterTrace.surface.surfaceProps);
					surfacedata_t* ExitSurfaceData = Interfaces::PhysProps()->GetSurfaceData(Result.surface.surfaceProps);
					float Thickness = EnterTrace.endpos.DistTo(Result.startpos);
					if (AutoWall::HandleBulletPenetration(WpnData->m_flPenetration, EnterSurfaceData, ExitSurfaceData, Result, Thickness, PenetrationCount, Damage)) { break; }
				}
				else { break; }
				StartPos = Result.startpos;
			}
		}
	}
	return 0;
}