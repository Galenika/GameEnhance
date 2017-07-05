

// Credits to Valve and Shad0w
#include "Interfaces.h"
#include "Menu.h"

// Shad0ws Yaw fix
// (FIX ME UP LATER)
void FixY(const CRecvProxyData *pData, void *pStruct, void *pOut)
{
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	float flYaw = pData->m_Value.m_Float;

	if (Menu::Window.RageBotTab.ResolverType.GetIndex() == 1)
	{
		for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); ++i)
		{
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

			flYaw = pEntity->GetLowerBodyYaw(); //Sets entity's eye angles to their current LBY
		}
	}
	else if (Menu::Window.RageBotTab.ResolverType.GetIndex() == 2)
	{
		for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); ++i)
		{
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

			static float lowerDelta[64];
			static float lastYaw[64];

			float curLower = pEntity->GetLowerBodyYaw();
			float curYaw = flYaw;

			lowerDelta[i] = curYaw - curLower;
			pNormalize(lowerDelta[i]);

			if (fabs(lowerDelta[i]) > 15.f) {
				if (pEntity->GetVelocity().Length() > 0.1f) //If the entity is moving, Set EyeAngles to LBY. LBY Updates when moving.
				{
					flYaw = pEntity->GetLBY();
				}
				else
				{
					if (curYaw != lastYaw[i])
					{
						flYaw += lowerDelta[i];
						lastYaw[i] = curYaw;
					}
				}
			}
		}
	}
	else if (Menu::Window.RageBotTab.ResolverType.GetIndex() == 3)
	{
		for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); ++i)
		{
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

			static float OldLowerBodyYaws[64];
			static float OldYawDeltas[64];

			float CurYaw = pEntity->GetLowerBodyYaw();

			if (OldLowerBodyYaws[i] != CurYaw)
			{
				OldYawDeltas[i] = flYaw - CurYaw;
				OldLowerBodyYaws[i] = CurYaw;
				flYaw = CurYaw;
				continue;
			}
			else
			{
				flYaw = flYaw - OldYawDeltas[i];
			}
		}
	}
	else if (Menu::Window.RageBotTab.ResolverType.GetIndex() == 4)
	{
		static bool flip;

		for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); ++i)
		{
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

			if (!pEntity->GetVelocity().Length() > 0.1f)
			{
				float flCurTime = Interfaces::Globals->curtime;
				static float flTimeUpdate = 0.5f;
				static float flNextTimeUpdate = flCurTime + flTimeUpdate;

				if (flCurTime >= flNextTimeUpdate)
				{
					flip != flip;
				}

				if (flNextTimeUpdate < flCurTime || flNextTimeUpdate - flCurTime > 10.f)
					flNextTimeUpdate = flCurTime + flTimeUpdate;

				if (flip)
				{
					flYaw += 35.f;
				}
				else {
					flYaw -= 35.f;
				}
			}

			pEntity->GetEyeAngles()->y = flYaw;
		}
	}

	*(float*)(pOut) = flYaw;
}

void FixX(const CRecvProxyData *pData, void *pStruct, void *pOut)
{
	float p = pData->m_Value.m_Float;

	if (!Menu::Window.RageBotTab.ResolverType.GetIndex() == 0) return;
	else
	{
		if (p > 89.0f && p < 91.0f)
			p -= 90.0f;
		else if (p > -89.0f && p < -91.0f)
			p += 90.0f;

		if (p > 179.0f && p < 181.0f)
			p -= 180.0f;

		else if (p > -179.0f && p < -181.0f)
			p += 180.0f;

	}
	*(float*)pOut = p;
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

					pProp->m_ProxyFn = FixY;
				}
			}
		}
	}
}
