#include "Resolver.h"

int Globals::Shots = 0;
bool Globals::change;
CUserCmd* Globals::UserCmd;
int Globals::TargetID;
IClientEntity* Globals::Target;

void R::Resolve()
{
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	for (int i = 0; i < Interfaces::Engine->GetMaxClients(); ++i)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

		if (!pEntity || pEntity->IsDormant() || !pEntity->IsAlive())
			continue;

		if (pEntity->GetTeamNum() == pLocal->GetTeamNum() || !pLocal->IsAlive())
			continue;

		Vector* eyeAngles = pEntity->GetEyeAnglesPointer();

		if (Menu::Window.RageBotTab.AccuracyAngleFixY.GetIndex() > 1)
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
				//BruteForce Resolver
				if (Menu::Window.RageBotTab.AccuracyAngleFixY.GetIndex() == 4)
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
				// Static
				if (Menu::Window.RageBotTab.AccuracyAngleFixY.GetIndex() == 2)
				{
					int value = rand() % 10;
					switch (value) {
					case 0:eyeAngles->y = AimAngle.y; break;
					case 1:eyeAngles->y += 0; break;
					case 2:eyeAngles->y = AimAngle.y + 180; break;
					case 3:eyeAngles->y += 15; break;
					case 4:eyeAngles->y = AimAngle.y + 90; break;
					case 5:eyeAngles->y -= 15; break;
					case 6:eyeAngles->y = AimAngle.y + 270; break;
					case 7:eyeAngles->y += 0; break;
					case 8:eyeAngles->y = AimAngle.y + 180; break;
					case 9:eyeAngles->y -= 45; break;
					case 10:eyeAngles->y += 45; break;
						break;
					}
				}
				if (Menu::Window.RageBotTab.AccuracyAngleFixY.GetIndex() == 3)
				{
					int num = Globals::Shots % 4;
					switch (num) {
					case 0:eyeAngles->y = rand() % 360; break;
					case 1:eyeAngles->y = 0; break;
					case 2:eyeAngles->y = rand() % 180; break;
					case 3:eyeAngles->y = 180; break;
						break;
					}
				}
			}
		}
			if (Menu::Window.RageBotTab.AccuracyAngleFix.GetState()) //pitch
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