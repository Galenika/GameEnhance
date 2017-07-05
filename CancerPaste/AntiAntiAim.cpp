#include "Interfaces.h"
#include "Menu.h"
#include "AntiAntiAim.h"
#include "MiscDefinitions.h"

int Globals::Shots = 0;
bool Globals::change;
CUserCmd* Globals::UserCmd;
int Globals::TargetID;
IClientEntity* Globals::Target;
float YawDelta[64];
float reset[64];
float Delta[64];
float OldLowerBodyYaw[64];
float Resolved_angles[64];
int iSmart;
static int jitter = -1;

/* void LowerBodyYawFix(int CurrentTarget)
{
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocal) return;
	if (pLocal->GetClientClass()->m_ClassID != SourceEngine::EClassIds::CCSPlayer) return;
	auto pCSPlayer = pLocal->GetEyeAnglesXY();
	if (!pLocal->IsAlive() || !pLocal->GetWeaponHandle()) return;
	if (pCSPlayer == GetLocalPlayer()) return;

	auto EyeAngles = pLocal->GetEyeAngles();
	if (pLocal->GetVelocity().Length() > 1 && (pLocal->GetFlags() & FL_ONGROUND)
		EyeAngles->y = pCSPlayer->GetLowerBodyYawTarget();

	// PoseParam when speed > 80.0f
} */

void Resolver()
{
	// This is basically a bruteforce resolver
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	for (int i = 0; i < Interfaces::Engine->GetMaxClients(); ++i)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

		if (!pEntity || pEntity->IsDormant() || !pEntity->IsAlive())
			continue;

		if (pEntity->GetTeamNum() == pLocal->GetTeamNum() || !pLocal->IsAlive())
			continue;

		Vector* eyeAngles = pEntity->GetEyeAnglesPointer();

		if (Menu::Window.RageBotTab.AccuracyResolverYaw.GetIndex()  || Menu::Window.RageBotTab.AccuracyResolverPitch.GetIndex())
		{
			if (pEntity != Globals::Target) continue;

			Vector AimAngle;
			CalcAngleYawOnly(pEntity->GetEyePosition(), pLocal->GetAbsOrigin2(), AimAngle);
			NormalizeVector(AimAngle);
			static float StoredYaw = 0;
			static bool bLowerBodyIsUpdated = false;
			if (pEntity->GetLowerBodyYaw() != StoredYaw) bLowerBodyIsUpdated = true;
			else bLowerBodyIsUpdated = false;
			if (bLowerBodyIsUpdated) StoredYaw = pEntity->GetLowerBodyYaw();
			if (pEntity->GetFlags() & FL_ONGROUND && pEntity->GetVelocity().Length2D() != 0) eyeAngles->y = pEntity->GetLowerBodyYaw();
			else
			{
				if (Menu::Window.RageBotTab.AccuracyResolverYaw.GetIndex() == 1)
				{
					int num = Globals::Shots % 9;
					switch (num) {
					case 0:eyeAngles->y = pEntity->GetLowerBodyYaw() + 180; break;
					case 1:eyeAngles->y = pEntity->GetLowerBodyYaw() - 90; break;
					case 2:eyeAngles->y = pEntity->GetLowerBodyYaw() + 90; break;
					case 3:eyeAngles->y = pEntity->GetLowerBodyYaw() + 179.95; break;
					case 4:eyeAngles->y = pEntity->GetLowerBodyYaw() - 179.95; break;
					case 5:eyeAngles->y = pEntity->GetLowerBodyYaw() + 40; break;
					case 6:eyeAngles->y = pEntity->GetLowerBodyYaw() + 30; break;
					case 7:eyeAngles->y = pEntity->GetLowerBodyYaw() - 30; break;
					case 8:eyeAngles->y = pEntity->GetLowerBodyYaw() + 15; break;
					}
				}
				if (Menu::Window.RageBotTab.AccuracyResolverYaw.GetIndex() == 2)
				{
					int num = Globals::Shots % 4;
					switch (num) {
					case 1:eyeAngles->y = pEntity->GetLowerBodyYaw() - 15; break;
					case 2:eyeAngles->y = pEntity->GetLowerBodyYaw() + 40; break;
					case 3:eyeAngles->y = pEntity->GetLowerBodyYaw() - 40; break;
					case 4:eyeAngles->y = pEntity->GetLowerBodyYaw() + 15; break;
					}
				}
				if (Menu::Window.RageBotTab.AccuracyResolverYaw.GetIndex() == 3 && eyeAngles->y != pEntity->GetLowerBodyYaw())
				{
					int value = (iSmart) % 13;
					switch (value) {
					case 0: eyeAngles->y += 0.0f; break;
					case 1: eyeAngles->y += 10; break;
					case 2: eyeAngles->y += 20; break;
					case 3: eyeAngles->y += 30; break;
					case 4: eyeAngles->y += 40; break;
					case 5: eyeAngles->y += 50; break;
					case 6: eyeAngles->y += 60; break;
					case 7: eyeAngles->y += 70; break;
					case 8: eyeAngles->y += 80; break;
					case 9: eyeAngles->y += 90; break;
					case 10: eyeAngles->y += 100; break;
					case 11: eyeAngles->y += 110; break;
					case 12: eyeAngles->y += 120; break;
					case 13: eyeAngles->y += 130; break;
					case 14: eyeAngles->y += 140; break;
					case 15: eyeAngles->y += 150; break;
					case 16: eyeAngles->y += 160; break;
					case 17: eyeAngles->y += 170; break;
					case 18: eyeAngles->y += 180; break;
					case 19: eyeAngles->y += 190; break;
					case 20: eyeAngles->y += 200; break;
					case 21: eyeAngles->y += 210; break;
					case 22: eyeAngles->y += 220; break;
					case 23: eyeAngles->y += 230; break;
					case 24: eyeAngles->y += 240; break;
					case 25: eyeAngles->y += 250; break;
					case 26: eyeAngles->y += 260; break;
					case 27: eyeAngles->y += 270; break;
					case 28: eyeAngles->y += 280; break;
					case 29: eyeAngles->y += 290; break;
					case 30: eyeAngles->y += 300; break;
					case 31: eyeAngles->y += 310; break;
					case 32: eyeAngles->y += 320; break;
					case 33: eyeAngles->y += 330; break;
					case 34: eyeAngles->y += 340; break;
					case 35: eyeAngles->y += 350; break;
					case 36: eyeAngles->y += 360; break;
					case 37: eyeAngles->y += 0.0f; break;
					case 38: eyeAngles->y -= 10; break;
					case 39: eyeAngles->y -= 20; break;
					case 40: eyeAngles->y -= 30; break;
					case 41: eyeAngles->y -= 40; break;
					case 42: eyeAngles->y -= 50; break;
					case 43: eyeAngles->y -= 60; break;
					case 44: eyeAngles->y -= 70; break;
					case 45: eyeAngles->y -= 80; break;
					case 46: eyeAngles->y -= 90; break;
					case 47: eyeAngles->y -= 100; break;
					case 48: eyeAngles->y -= 110; break;
					case 49: eyeAngles->y -= 120; break;
					case 50: eyeAngles->y -= 130; break;
					case 51: eyeAngles->y -= 140; break;
					case 52: eyeAngles->y -= 150; break;
					case 53: eyeAngles->y -= 160; break;
					case 54: eyeAngles->y -= 170; break;
					case 55: eyeAngles->y -= 180; break;
					case 56: eyeAngles->y -= 190; break;
					case 57: eyeAngles->y -= 200; break;
					case 58: eyeAngles->y -= 210; break;
					case 59: eyeAngles->y -= 220; break;
					case 60: eyeAngles->y -= 230; break;
					case 61: eyeAngles->y -= 240; break;
					case 62: eyeAngles->y -= 250; break;
					case 63: eyeAngles->y -= 260; break;
					case 64: eyeAngles->y -= 270; break;
					case 65: eyeAngles->y -= 280; break;
					case 66: eyeAngles->y -= 290; break;
					case 67: eyeAngles->y -= 300; break;
					case 68: eyeAngles->y -= 310; break;
					case 69: eyeAngles->y -= 320; break;
					case 70: eyeAngles->y -= 330; break;
					case 71: eyeAngles->y -= 340; break;
					case 72: eyeAngles->y -= 350; break;
					case 73: eyeAngles->y -= 360; break;
						if (jitter)
							eyeAngles->y += 90;
						else
							eyeAngles->y + 90;
						break;
						if (jitter)
							eyeAngles->y -= 90;
						else
							eyeAngles->y = -90;
						break;
					}
				}
				if (Menu::Window.RageBotTab.AccuracyResolverYaw.GetIndex() == 4)
				{
					int num = Globals::Shots % 5;
					switch (num) {
					case 0:eyeAngles->y = AimAngle.y - 0; break;
					case 1:eyeAngles->y = AimAngle.y - 15; break;
					case 2:eyeAngles->y = AimAngle.y + 40; break;
					case 3:eyeAngles->y = AimAngle.y - 40; break;
					case 4:eyeAngles->y = AimAngle.y + 15; break;
					}
				}
				if (Menu::Window.RageBotTab.AccuracyResolverYaw.GetIndex() == 5)
				{
					for (int i = 0; i < 64; ++i)
					{
						IClientEntity* ent = Interfaces::EntList->GetClientEntity(i);

						if (!ent || !ent->IsAlive())
							continue;

						*(float*)((uintptr_t)ent + 0xAA08 + 4) += Delta[i];
						reset[i] = *(float*)((uintptr_t)ent + 0xAA08 + 4);

						float LowerBodyYaw = *(float*)((uintptr_t)ent + 0x39F8);

						if (OldLowerBodyYaw[i] != LowerBodyYaw)
						{
							YawDelta[i] = reset[i] - LowerBodyYaw;
							OldLowerBodyYaw[i] = LowerBodyYaw;
							*(float*)((uintptr_t)ent + 0xAA08 + 4) = LowerBodyYaw;
						}
						else
						{
							*(float*)((uintptr_t)ent + 0xAA08 + 4) = reset[i] - YawDelta[i];

							while (*(float*)((uintptr_t)ent + 0xAA08 + 4) < -180.0f)
								*(float*)((uintptr_t)ent + 0xAA08 + 4) += 360.0f;

							while (*(float*)((uintptr_t)ent + 0xAA08 + 4) > 180.0f)
								*(float*)((uintptr_t)ent + 0xAA08 + 4) -= 360.0f;
						}
						Resolved_angles[i] = *(float*)((uintptr_t)ent + 0xAA08 + 4);
					}
				}
				if (Menu::Window.RageBotTab.AccuracyResolverYaw.GetIndex() == 6)
				{
					IClientEntity* ent = Interfaces::EntList->GetClientEntity(i);

					for (int i = 0; i < 32; i++) {
						if (!pEntity || !pEntity->IsAlive() || !(pEntity->GetEyeAngles().y >= 85.f && pEntity->GetEyeAngles().y <= 89.f))
							continue;

						float CurYaw = pEntity->GetLowerBodyYaw();
						if (OldLowerBodyYaw[i] != CurYaw) {
							Delta[i] = pEntity->GetEyeAngles().y - CurYaw;
							OldLowerBodyYaw[i] = CurYaw;
							*(float*)((uintptr_t)ent + 0xAA08 + 4) = CurYaw;
							continue;
						}
						else {
							*(float*)((uintptr_t)ent + 0xAA08 + 4) = pEntity->GetEyeAngles().y - Delta[i];
						}

						float yClamped = pEntity->GetEyeAngles().y;
						while (yClamped < -180.0f)
							yClamped += 360.0f;

						while (yClamped > 180.0f)
							yClamped -= 360.0f;
						*(float*)((uintptr_t)ent + 0xAA08 + 4) = yClamped;
					}
					Resolved_angles[i] = *(float*)((uintptr_t)ent + 0xAA08 + 4);
				}
				if (Menu::Window.RageBotTab.AccuracyResolverYaw.GetIndex() == 7)
				{
					IClientEntity* ent = Interfaces::EntList->GetClientEntity(i);

					for (int i = 0; i < 64; i++) {
						if (!pEntity || !pEntity->IsAlive() || !(pEntity->GetEyeAngles().y >= 85.f && pEntity->GetEyeAngles().y <= 89.f))
							continue;

						float CurYaw = pEntity->GetLowerBodyYaw();
						if (OldLowerBodyYaw[i] != CurYaw) {
							Delta[i] = pEntity->GetEyeAngles().y - CurYaw;
							OldLowerBodyYaw[i] = CurYaw;
							*(float*)((uintptr_t)ent + 0xAA08 + 4) = CurYaw;
							continue;
						}
						else {
							*(float*)((uintptr_t)ent + 0xAA08 + 4) = pEntity->GetEyeAngles().y - Delta[i];
						}

						float yClamped = pEntity->GetEyeAngles().y;
						while (yClamped < -180.0f)
							yClamped += 360.0f;

						while (yClamped > 180.0f)
							yClamped -= 360.0f;
						*(float*)((uintptr_t)ent + 0xAA08 + 4) = yClamped;
					}
					Resolved_angles[i] = *(float*)((uintptr_t)ent + 0xAA08 + 4);
				}
				
				/* if (Menu::Window.RageBotTab.AccuracyResolver.GetState() == 4 && eyeAngles->y != pEntity->GetLowerBodyYaw())
				{
				int num = Globals::Shots % 9;
				switch (num) {
				case 0:eyeAngles->y = AimAngle.y - 0; break;
				case 1:eyeAngles->y = AimAngle.y + 45; break;
				case 2:eyeAngles->y = AimAngle.y + 90; break;
				case 3:eyeAngles->y = AimAngle.y + 135; break;
				case 4:eyeAngles->y = AimAngle.y + 180; break;
				case 5:eyeAngles->y = AimAngle.y + 225; break;
				case 6:eyeAngles->y = AimAngle.y + 270; break;
				case 7:eyeAngles->y = AimAngle.y + 315; break;
				case 8:eyeAngles->y = AimAngle.y + 360; break;
				}
				} */
			}



			// Pitch Resolver
			if (Menu::Window.RageBotTab.AccuracyResolverPitch.GetIndex() == 1)
			{
				std::string strPitch = std::to_string(eyeAngles->x);

				if (eyeAngles->x < -179.f) eyeAngles->x += 360.f;
				else if (eyeAngles->x > 90.0 || eyeAngles->x < -90.0) eyeAngles->x = 89.f;
				else if (eyeAngles->x > 89.0 && eyeAngles->x < 91.0) eyeAngles->x -= 90.f;
				else if (eyeAngles->x > 179.0 && eyeAngles->x < 181.0) eyeAngles->x -= 180;
				else if (eyeAngles->x > -179.0 && eyeAngles->x < -181.0) eyeAngles->x += 180;
				else if (fabs(eyeAngles->x) == 0) eyeAngles->x = std::copysign(89.0f, eyeAngles->x);

			}
		}
	}
}

void FixY(const CRecvProxyData *pData, void *pStruct, void *pOut) // Resolver for Yaw
{
	static Vector vLast[65];
	static bool bShotLastTime[65];
	static bool bJitterFix[65];

	float *flPitch = (float*)((DWORD)pOut - 4);
	float flYaw = pData->m_Value.m_Float;
	bool bHasAA;
	bool bSpinbot;

	if (Menu::Window.RageBotTab.AccuracyResolverYaw.GetIndex() || Menu::Window.RageBotTab.AccuracyResolverPitch.GetIndex())
	{
		bHasAA = ((*flPitch == 90.0f) || (*flPitch == 270.0f));
		bSpinbot = false;

		if (!bShotLastTime[((IClientEntity*)(pStruct))->GetIndex()]
			&& (fabsf(flYaw - vLast[((IClientEntity*)(pStruct))->GetIndex()].y) > 15.0f) && !bHasAA)
		{
			flYaw = vLast[((IClientEntity*)(pStruct))->GetIndex()].y;
			bShotLastTime[((IClientEntity*)(pStruct))->GetIndex()] = true;
		}
		else
		{
			if (bShotLastTime[((IClientEntity*)(pStruct))->GetIndex()]
				&& (fabsf(flYaw - vLast[((IClientEntity*)(pStruct))->GetIndex()].y) > 15.0f))
			{
				bShotLastTime[((IClientEntity*)(pStruct))->GetIndex()] = true;
				bSpinbot = true;
			}
			else
			{
				bShotLastTime[((IClientEntity*)(pStruct))->GetIndex()] = false;
			}
		}

		vLast[((IClientEntity*)(pStruct))->GetIndex()].y = flYaw;


		bool bTmp = bJitterFix[((IClientEntity*)(pStruct))->GetIndex()];

		bJitterFix[((IClientEntity*)(pStruct))->GetIndex()] = (flYaw >= 180.0f && flYaw <= 360.0f);

		if (bTmp && (flYaw >= 0.0f && flYaw <= 180.0f))
		{
			flYaw += 359.0f;
		}
	}

	*(float*)(pOut) = flYaw;
}

// Resolver for Pitch
void FixX(const CRecvProxyData* pData, void* pStruct, void* pOut) // Clamp other player angles to fix fakedown or lisp
{
	float* ang = (float*)pOut;
	*ang = pData->m_Value.m_Float;

	if (!Menu::Window.RageBotTab.AccuracyResolverYaw.GetIndex() || !Menu::Window.RageBotTab.AccuracyResolverPitch.GetIndex()) return;

	if (pData->m_Value.m_Float > 180.0f)
		*ang -= 360.0f;
	else if (pData->m_Value.m_Float < -180.0f)
		*ang += 360.0f;

	if (pData->m_Value.m_Float > 89.0f && pData->m_Value.m_Float < 91.0f)
		*ang -= 90.0f;
	else if (pData->m_Value.m_Float > -89.0f && pData->m_Value.m_Float < -91)
		*ang += 90.0f;
}

RecvVarProxyFn oRecvnModelIndex;

void Hooked_RecvProxy_Viewmodel(CRecvProxyData *pData, void *pStruct, void *pOut)
{
	// Get the knife view model id's
	int default_t = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
	int default_ct = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
	int iBayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
	int iButterfly = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
	int iFlip = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
	int iGut = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
	int iKarambit = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
	int iM9Bayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
	int iHuntsman = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
	int iFalchion = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
	int iDagger = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
	int iBowie = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");

	int iGunGame = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");

	// Get local player (just to stop replacing spectators knifes)
	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (Menu::Window.SkinTab.SkinEnable.GetState() && pLocal)
	{
		// If we are alive and holding a default knife(if we already have a knife don't worry about changing)
		if (pLocal->IsAlive() && (
			pData->m_Value.m_Int == default_t ||
			pData->m_Value.m_Int == default_ct ||
			pData->m_Value.m_Int == iBayonet ||
			pData->m_Value.m_Int == iButterfly ||
			pData->m_Value.m_Int == iFlip ||
			pData->m_Value.m_Int == iGunGame ||
			pData->m_Value.m_Int == iGut ||
			pData->m_Value.m_Int == iKarambit ||
			pData->m_Value.m_Int == iM9Bayonet ||
			pData->m_Value.m_Int == iHuntsman ||
			pData->m_Value.m_Int == iFalchion ||
			pData->m_Value.m_Int == iDagger ||
			pData->m_Value.m_Int == iBowie))
		{
			// Set whatever knife we want
			if (Menu::Window.SkinTab.KnifeModel.GetIndex() == 0)
				pData->m_Value.m_Int = iBayonet;
			else if (Menu::Window.SkinTab.KnifeModel.GetIndex() == 1)
				pData->m_Value.m_Int = iBowie;
			else if (Menu::Window.SkinTab.KnifeModel.GetIndex() == 2)
				pData->m_Value.m_Int = iButterfly;
			else if (Menu::Window.SkinTab.KnifeModel.GetIndex() == 3)
				pData->m_Value.m_Int = iFalchion;
			else if (Menu::Window.SkinTab.KnifeModel.GetIndex() == 4)
				pData->m_Value.m_Int = iFlip;
			else if (Menu::Window.SkinTab.KnifeModel.GetIndex() == 5)
				pData->m_Value.m_Int = iGut;
			else if (Menu::Window.SkinTab.KnifeModel.GetIndex() == 6)
				pData->m_Value.m_Int = iHuntsman;
			else if (Menu::Window.SkinTab.KnifeModel.GetIndex() == 7)
				pData->m_Value.m_Int = iKarambit;
			else if (Menu::Window.SkinTab.KnifeModel.GetIndex() == 8)
				pData->m_Value.m_Int = iM9Bayonet;
			else if (Menu::Window.SkinTab.KnifeModel.GetIndex() == 9)
				pData->m_Value.m_Int = iDagger;
		}
	}

	// Carry on the to original proxy
	oRecvnModelIndex(pData, pStruct, pOut);
}

void ApplyAAAHooks()
{
	ClientClass *pClass = Interfaces::Client->GetAllClasses();
	while (pClass)
	{
		const char *pszName = pClass->m_pRecvTable->m_pNetTableName;
		if (!strcmp(pszName, "DT_CSPlayer"))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;

				// Pitch Fix
				if (!strcmp(name, "m_angEyeAngles[0]"))
				{
					pProp->m_ProxyFn = FixX;
				}

				// Yaw Fix
				if (!strcmp(name, "m_angEyeAngles[1]"))
				{
					Utilities::Log("Yaw Fix Applied");
					pProp->m_ProxyFn = FixY;
				}
			}
		}
		else if (!strcmp(pszName, "DT_BaseViewModel"))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;

				// Knives
				if (!strcmp(name, "m_nModelIndex"))
				{
					oRecvnModelIndex = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = Hooked_RecvProxy_Viewmodel;
				}
			}
		}
		pClass = pClass->m_pNext;
	}
}