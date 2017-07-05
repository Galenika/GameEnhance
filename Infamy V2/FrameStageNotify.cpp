#include "hooks.h"
#include "Globals.h"
#include "Accessors.h"

FrameStageNotifyFn oFrameStageNotify;

#define	LIFE_ALIVE 0

#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);

void StoreYaw(const CRecvProxyData* pData, void* pStruct, void* pOut)
{
	Global::oAAA[1] = pData->m_Value.m_Float;
}
void StorePitch(const CRecvProxyData* pData, void* pStruct, void* pOut)
{
	Global::oAAA[0] = pData->m_Value.m_Float;
}


class CCacheAngle {
public:
	//Menu
	bool bCustomSetts = false;
	bool bCorrect = true;
	int iResolverType = 0;
	bool bPrioritize = false;
	int iHitbox = 1;
	bool bAutoBaim = false;
	int iAutoBaimAferShot = 2;
	int iHitboxAutoBaim = 4;
	bool bPrioritizeVis = true;
	float flMinDmg = 5.f;
	bool bInterpLagComp = false;

	//GlobalPlayer
	bool bFlip = false;
};

CCacheAngle aCacheAngle[64];

void __stdcall Game::hkdFrameStageNotify(ClientFrameStage_t curStage)
{
	QAngle aimold;
	QAngle viewold;
	QAngle* aim = nullptr;
	QAngle* view = nullptr;
	if (Engine->IsConnected() && Engine->IsInGame())
	{
		if (FRAME_RENDER_START)
		{
			auto LocalPlayer = static_cast<CBaseEntity*>(ClientEntList->GetClientEntity(Engine->GetLocalPlayer()));
			if (!LocalPlayer)
				return;
			if (LocalPlayer->GetAlive())
			{
				if (Options.Visuals.Removals.VisualRecoil)
				{

					aim = reinterpret_cast<QAngle*>(DWORD(Global::LocalPlayer) + offsets.m_aimPunchAngle);
					view = reinterpret_cast<QAngle*>(DWORD(Global::LocalPlayer) + offsets.m_viewPunchAngle);
					aimold = *aim;
					viewold = *view;
					*aim = QAngle(0, 0, 0);
					*view = QAngle(0, 0, 0);

				}

				auto ThirdPerson = *reinterpret_cast<bool*>(reinterpret_cast<DWORD>(g_pCInput) + 0xA5);

				if (ThirdPerson)
				{
					*reinterpret_cast<QAngle*>(reinterpret_cast<DWORD>(LocalPlayer) + offsets.dwDeadFlag + 0x4) = Global::VisualAngle;
				}
			}
		}
		//Resolver
		if (FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		{
			if (Options.Rage.Aimbot.AntiAntiAim)
			{

				Vector ShootPos[125];
				int index = -1;
				CBaseEntity *entity;
				Vector reset = Vector(0, 0, 0);
				float lastsim = 0;
				Vector lastorigin = Vector(0, 0, 0);
				std::vector< float > pastangles;
				int ScannedNumber = 0;
				int BestIndex = 0;
				float difference = 0.f;
				float Backtrack[360];
				float flLastPelvisAng = 0.f;
				float flEyeAng = 0.f;
				float resolved = 0.f;
				float posedifference = 0.f;
				static int iLastUpdatedTick = 0;
				static float lowerDelta[64];
				static float lastYaw[64];


				for (auto i = 0; i < ClientEntList->GetHighestEntityIndex(); i++)
				{
					CBaseEntity* pEnt = nullptr;
					if (!(pEnt = ClientEntList->GetClientEntity(i)))
						continue;

					if ((pEnt == Global::LocalPlayer || !pEnt->GetHealth() || pEnt->GetDormant()))
						continue;

					//Options.Rage.Aimbot.AAA.oAngle[i].x = *reinterpret_cast<float*>(reinterpret_cast<DWORD>(pEnt) + offsets.m_angEyeAnglesX);
					//Options.Rage.Aimbot.AAA.oAngle[i].y = *reinterpret_cast<float*>(reinterpret_cast<DWORD>(pEnt) + offsets.m_angEyeAnglesY);
					auto *Pitch = reinterpret_cast<float*>(reinterpret_cast<DWORD>(pEnt) + offsets.m_angEyeAnglesX);
					auto *Yaw = reinterpret_cast<float*>(reinterpret_cast<DWORD>(pEnt) + offsets.m_angEyeAnglesY);
					auto flLowerBodyYaw = reinterpret_cast<float*>(reinterpret_cast<DWORD>(pEnt) + offsets.m_flLowerBodyYawTarget);

					float flYaw = (reinterpret_cast<DWORD>(pEnt) + offsets.m_flLowerBodyYawTarget);;


					if (!pEnt->GetVelocity().Length2D() != 0)
					{
						float flCurTime = Globals->curtime;
						static float flTimeUpdate = 0.5f;
						static float flNextTimeUpdate = flCurTime + flTimeUpdate;

						if (flCurTime >= flNextTimeUpdate) {
							aCacheAngle[i].bFlip = !aCacheAngle[i].bFlip;
						}

						if (flNextTimeUpdate < flCurTime || flNextTimeUpdate - flCurTime > 10.f)
							flNextTimeUpdate = flCurTime + flTimeUpdate;

						if (aCacheAngle[i].bFlip) {
							flYaw += 35.f;
						}
						else {
							flYaw -= 35;
						}
						//*Yaw = *flLowerBodyYaw;
					}
					else
					{
						if (*Yaw != lastYaw[i]) {
							*Yaw += lowerDelta[i]; //This may not work because of ayyware! If this don't work, do usuall method of lowerDelta, subtract eyeAngles.y from LowerbodyAngles.
							lastYaw[i] = *Yaw;
						}
					}


					*Yaw = flYaw;
				}
			}

		}
	}
	oFrameStageNotify(curStage);
	if (aim && view && Options.Visuals.Removals.VisualRecoil)
	{
		*aim = aimold;
		*view = viewold;
	}
}
void __stdcall Game::hkdHudUpdate(bool active)
{
	static auto dwHudUpdate = Hooks::pMovePacket->dwGetMethodAddress(11);
	__asm
	{
		push active
		call dwHudUpdate
	}
	if (Engine->IsInGame() && Engine->IsConnected())
	{
		NetVars->HookProp("DT_CSPlayer", "m_angEyeAngles[0]", StorePitch);
		NetVars->HookProp("DT_CSPlayer", "m_angEyeAngles[1]", StoreYaw);
	}
}