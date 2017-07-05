#include "OwnAimbot.h"


#define IA 16807
#define IM 2147483647
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define MAX_RANDOM_RANGE 0x7FFFFFFFUL

#define AM (1.0/IM)
#define EPS 1.2e-7
#define RNMX (1.0-EPS) 
#define TAB_AIMBOT 0
#define TAB_VISUALS 1
#define TAB_MISC 3
#define M_PI 3.14159265358979323846
#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.

//#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( M_PI ) / 180.0f ) )
// --- (Skapad av ViKiNG & Steady) ---
void AngleVectorsx(const Vector angles, Vector* forward) {
	float sp, sy, cp, cy;
	sp = sin(DEG2RAD(angles.x));
	sy = sin(DEG2RAD(angles.y));
	cp = cos(DEG2RAD(angles.x));
	cy = cos(DEG2RAD(angles.y));
	forward->x = cp*cy;
	forward->y = cp*sy;
	forward->z = -sp;
}

float GetFOV(Vector CmdViewAngle, Vector LocalEyePos, Vector Destination) {
	Vector ang, aim;
	CalcAngle(LocalEyePos, Destination, ang);
	AngleVectorsx(CmdViewAngle, &aim);
	AngleVectorsx(ang, &ang);

	float mag_s = std::sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
	float mag_d = mag_s;
	float u_dot_v = aim[0] * ang[0] + aim[1] * ang[1] + aim[2] * ang[2];
	float fov = std::acos(u_dot_v / (mag_s * mag_d)) * (180.f / M_PI_F);
	return fov;
}

void NormaliseViewAngle(Vector &angle)
{
	while (angle.y <= -180) angle.y += 360;
	while (angle.y > 180) angle.y -= 360;
	while (angle.x <= -180) angle.x += 360;
	while (angle.x > 180) angle.x -= 360;


	if (angle.x > 89.0) angle.x = 89;
	if (angle.x < -89.0) angle.x = -89;
	if (angle.y < -180) angle.y = -179.999;
	if (angle.y > 180) angle.y = 179.999;

	angle.z = 0;
}

// END --- (Skapad av ViKiNG & Steady) ---


bool IsVisible(IClientEntity* pEntity, IClientEntity* pLocal)
{
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	ray.Init(pLocal->GetEyePosition(), pEntity->GetEyePosition());
	Interfaces::Trace->TraceRay(ray, 0x46004003, &filter, &tr);

	return (tr.m_pEnt == pEntity || tr.fraction > 0.99f);
}

bool BoneTrace(IClientEntity* pLocalClientBaseEntity, Vector vecOrigin, Vector vecFinal, IClientEntity* pClientBaseEntity)
{
	Ray_t ray;
	ray.Init(vecOrigin, vecFinal);

	CTraceFilter TraceFilter;

	trace_t trace;
	Interfaces::Trace->TraceRay(ray, 0x4600400B, &TraceFilter, &trace);

	return(trace.m_pEnt == pClientBaseEntity || trace.fraction > 0.99f);
}

bool GetBonePosition(IClientEntity* pEntity, Vector &Hitbox, int Bone) {
	matrix3x4 MatrixArray[128];
	if (!pEntity->SetupBones(MatrixArray, 128, 0x100, 0)) {
		return false;
	}
	Hitbox = Vector(MatrixArray[Bone][0][3], MatrixArray[Bone][1][3], MatrixArray[Bone][2][3]);
	return true;
}

Vector Hitscans(IClientEntity* pEntity, IClientEntity* pLocal, Vector& Hitbox) {

	int hitboxarray[18] = { 6, 7, 8, 9, 10, 11, 1, 4, 2, 5, 3, 6, 12, 15, 13, 16, 14, 17 };

	for (int i = 0; i < 18; i++) {


		GetBonePosition(pEntity, Hitbox, hitboxarray[i]);
		if (BoneTrace(pLocal, pLocal->GetEyePosition(), Hitbox, pEntity)) {
			return Hitbox;
		}
	}

}

void NormalizeVector2(Vector& angle)
{
	for (int axis = 0; axis < 3; ++axis)
	{
		while (angle[axis] > 180.f)
		{
			angle[axis] -= 360.f;
		}

		while (angle[axis] < -180.f)
		{
			angle[axis] += 360.f;
		}
	}

	angle[3] = 0.0f;
}

Vector Normalized(Vector& vec) {
	if (vec.x > 180)  vec.x -= 360;
	if (vec.y > 180)  vec.y -= 360;
	if (vec.x < -180) vec.x += 360;
	if (vec.y < -180) vec.y += 360;
	vec.z = 0;
	return vec;
}

//void SetSmoothAngles(CUserCmd* pCmd, Vector& dst) {
//	if (aim_smooth.getValve()) {
//		if (aim_smoothBar.getValue() > 0) {
//			Vector delta = pCmd->viewangles - dst;
//			Normalized(delta);
//			int smoothx = aim_smoothBar.getValue() * 3;
//			dst = pCmd->viewangles - delta / smoothx;
//			dst.z = 0, delta.z = 0;
//		}
//	}
//}



void CorrectMovement(Vector vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove)
{
	//side/forward move correction
	float deltaView = pCmd->viewangles.y - vOldAngles.y;
	float f1;
	float f2;

	if (vOldAngles.y < 0.f)
		f1 = 360.0f + vOldAngles.y;
	else
		f1 = vOldAngles.y;

	if (pCmd->viewangles.y < 0.0f)
		f2 = 360.0f + pCmd->viewangles.y;
	else
		f2 = pCmd->viewangles.y;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);
	deltaView = 360.0f - deltaView;

	pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
	pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
}

#define C_BaseEntity IClientEntity
#define pEntList Interfaces::EntList
#define pEngine Interfaces::Engine
float damage = 5;
void Rage2Bot::doAim(CUserCmd* pCmd, bool aim_active, bool aim_silent, bool aim_autoshoote, bool aim_psilent) {

	Vector oldView = pCmd->viewangles;
	float oldForwardmove = pCmd->forwardmove;
	float oldSidemove = pCmd->sidemove;

	if (aim_active) {
		IClientEntity* pLocal = pEntList->GetClientEntity(pEngine->GetLocalPlayer());
		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());





		int BestID = -1;
		float maxDistance = 99999;
		for (int i = 1; i < pEntList->GetHighestEntityIndex(); i++) {
			C_BaseEntity* pEntity = pEntList->GetClientEntity(i);

			if (pEntity && pEntity->GetHealth() > 0 && pLocal->GetHealth() > 0 && !pEntity->IsDormant() && pEntity->GetTeamNum() != pLocal->GetTeamNum() && !pEntity->IsImmune()) {
				Vector BoneFrom, BoneTo;

				GetBonePosition(pEntity, BoneFrom, Menu::Window.RageBotTab.TargetHitbox.GetIndex());
				//BoneFrom = Hitscans(pEntity, pLocal, BoneFrom);
				CalcAngle(pLocal->GetEyePosition(), BoneFrom, BoneTo);
				//SetSmoothAngles(pCmd, BoneTo);
				GameUtils::ClampViewAngle(BoneTo);
				NormalizeVector2(BoneTo);
				//	CorrectMovement(oldView, pCmd, oldForwardmove, oldSidemove);
				if (IsVisible(pEntity, pLocal) && !Menu::Window.RageBotTab.AccuracyAutoWall.GetState()) {
					if (!GameUtils::IsKnife(pWeapon)) {
						if (GetFOV(pCmd->viewangles, pLocal->GetEyePosition(), BoneFrom) < Menu::Window.RageBotTab.AimbotFov.GetValue()) {
							if (GetFOV(pCmd->viewangles, pLocal->GetEyePosition(), BoneFrom) < maxDistance) {
								maxDistance = GetFOV(pCmd->viewangles, pLocal->GetEyePosition(), BoneFrom);
								BestID = i;
							}
						}
					}
				}
				else if (CanHit(BoneFrom, &damage) && Menu::Window.RageBotTab.AccuracyAutoWall.GetState()) {
					if (!GameUtils::IsKnife(pWeapon)) {
						if (GetFOV(pCmd->viewangles, pLocal->GetEyePosition(), BoneFrom) < Menu::Window.RageBotTab.AimbotFov.GetValue()) {
							if (GetFOV(pCmd->viewangles, pLocal->GetEyePosition(), BoneFrom) < maxDistance) {
								maxDistance = GetFOV(pCmd->viewangles, pLocal->GetEyePosition(), BoneFrom);
								BestID = i;
							}
						}
					}
				}
			}
		}

		C_BaseEntity* pEntity = pEntList->GetClientEntity(BestID);

		if (pEntity && pEntity->GetHealth() > 0 && pLocal->GetHealth() > 0 && !pEntity->IsDormant() && pEntity->GetTeamNum() != pLocal->GetTeamNum() && !pEntity->IsImmune()) {
			Vector BoneFrom, BoneTo;
			Vector aimbotAngle;
			GetBonePosition(pEntity, BoneFrom, Menu::Window.RageBotTab.TargetHitbox.GetIndex());
			//BoneFrom = Hitscans(pEntity, pLocal, BoneFrom);
			CalcAngle(pLocal->GetEyePosition(), BoneFrom, BoneTo);
			//SetSmoothAngles(pCmd, BoneTo);
			//	NormalizeVector(BoneTo);
			GameUtils::ClampViewAngle(BoneTo);
			NormalizeVector2(BoneTo);
			aimbotAngle = BoneTo;
			if (IsVisible(pEntity, pLocal) && !Menu::Window.RageBotTab.AccuracyAutoWall.GetState()) {
				if (!GameUtils::IsKnife(pWeapon)) {
					if (GetFOV(pCmd->viewangles, pLocal->GetEyePosition(), BoneFrom) < Menu::Window.RageBotTab.AimbotFov.GetValue()) {


						if (aim_autoshoote) { // To work with.
							pCmd->buttons |= IN_ATTACK;
						}
						if (pCmd->buttons & IN_ATTACK) {
							pCmd->viewangles = BoneTo;
						}
						//		CorrectMovement(oldView, pCmd, oldForwardmove, oldSidemove);
						if (!aim_silent) {
							pEngine->SetViewAngles(BoneTo);
						}

					}
				}
			}
			else if (CanHit(BoneFrom, &damage) && Menu::Window.RageBotTab.AimbotFov.GetValue()) {
				if (!GameUtils::IsKnife(pWeapon)) {
					if (GetFOV(pCmd->viewangles, pLocal->GetEyePosition(), BoneFrom) < Menu::Window.RageBotTab.AimbotFov.GetValue()) {


						if (aim_autoshoote) { // To work with.
							pCmd->buttons |= IN_ATTACK;
						}
						if (pCmd->buttons & IN_ATTACK) {
							pCmd->viewangles = BoneTo;
						}
						if (!aim_silent) {
							pEngine->SetViewAngles(BoneTo);
						}

					}
				}
			}
		}
	}
}
