#include "Resolver.h"

int Globals::Shots = 0;
bool Globals::change;
CUserCmd* Globals::UserCmd;
int Globals::TargetID;
IClientEntity* Globals::Target;
float OldLowerBodyYaw[64];
float YawDelta[64];
float reset[64];
float Delta[64];
float Resolved_angles[64];
int iSmart;
static int jitter = -1;





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

		if (Menu::Window.RageBotTab.AccuracyResolver.GetIndex() == 1 || Menu::Window.RageBotTab.AccuracyResolver.GetIndex() == 2)
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
				int num = Globals::Shots % 5;
				switch (num) {
				case 0:eyeAngles->y = AimAngle.y - 15; break;
				case 1:eyeAngles->y = AimAngle.y + 40; break;
				case 2:eyeAngles->y = AimAngle.y - 40; break;
				case 3:eyeAngles->y = AimAngle.y + 15; break;
				}
			}

			if (Menu::Window.RageBotTab.AccuracyResolver.GetIndex() == 2) //pitch
			{
				std::string strPitch = std::to_string(eyeAngles->x);

				if (eyeAngles->x < -179.f) eyeAngles->x += 360.f;
				else if (eyeAngles->x > 90.0 || eyeAngles->x < -90.0) eyeAngles->x = 89.f;
				else if (eyeAngles->x > 89.0 && eyeAngles->x < 91.0) eyeAngles->x -= 90.f;
				else if (eyeAngles->x > 179.0 && eyeAngles->x < 181.0) eyeAngles->x -= 180;
				else if (eyeAngles->x > -179.0 && eyeAngles->x < -181.0) eyeAngles->x += 180;
				else if (fabs(eyeAngles->x) == 0) eyeAngles->x = std::copysign(89.0f, eyeAngles->x);
			}
			if (Menu::Window.RageBotTab.AccuracyResolver.GetIndex() == 1)
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

				if (Menu::Window.RageBotTab.AccuracyResolver.GetIndex() == 3 || Menu::Window.RageBotTab.AccuracyResolver.GetIndex() == 3)
				{
					int value = (iSmart) % 11;
					switch (value) {
					case 0:eyeAngles->y = AimAngle.y + 45; break;
					case 1:eyeAngles->y += 0; break;
					case 2:eyeAngles->y = AimAngle.y + 135; break;
					case 3:eyeAngles->y += 15; break;
					case 4:eyeAngles->y = AimAngle.y + 180; break;
					case 5:eyeAngles->y -= 15; break;
					case 6:eyeAngles->y = AimAngle.y + 270; break;
					case 7:eyeAngles->y += 0; break;
					case 8:eyeAngles->y = AimAngle.y + 360; break;
					case 9:eyeAngles->y -= 45; break;
					case 10:eyeAngles->y += 45; break;
					}

					if (Menu::Window.RageBotTab.AccuracyResolver.GetIndex() == 4 || Menu::Window.RageBotTab.AccuracyResolver.GetIndex() == 4)
					{
						int value = (iSmart) % 13;
						switch (value) {
						case 0: eyeAngles->y += 0.0f; break;
						case 1: eyeAngles->y += 25; break;
						case 2: eyeAngles->y += 0; break;
						case 3: eyeAngles->y -= 25; break;
						case 4: eyeAngles->y += 0; break;
						case 5: eyeAngles->y += 80; break;
						case 6: eyeAngles->y += 0; break;
						case 7: eyeAngles->y -= 80; break;
						case 8: eyeAngles->y += 0; break;
						case 9: eyeAngles->y += 40; break;
						case 10: eyeAngles->y += 0; break;
						case 11: eyeAngles->y -= 40; break;
						case 12: eyeAngles->y += 0; break;
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
						}
						if (Menu::Window.RageBotTab.AccuracyResolver.GetIndex() == 5 || Menu::Window.RageBotTab.AccuracyResolver.GetIndex() == 5)
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
							}
							if (Menu::Window.RageBotTab.AccuracyAutoResolver.GetIndex() == 4 && eyeAngles->y != pEntity->GetLowerBodyYaw())
							{
								static int jitter = 1;
								int lastpelvisang = 0;

								{
									DWORD* togoto0 = reinterpret_cast<DWORD*>(static_cast<DWORD> (+18));//array size/ list gotta clear it
									togoto0 = 0;
									DWORD* someothershit = reinterpret_cast<DWORD*>(static_cast<DWORD>(+12)); //invalidate the head interpolation
									someothershit = (DWORD*)0xFFFF;
									Vector orginalview = Vector(0, 0, 0); orginalview.x = eyeAngles->x; orginalview.y = eyeAngles->y;

								}
								if (jitter % 2 == 0)
									jitter++;
							}

							if (Menu::Window.RageBotTab.AccuracyAutoResolver.GetIndex() == 5 && eyeAngles->y != pEntity->GetLowerBodyYaw())
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
						}
					}
				}
			}
		}
	}
}