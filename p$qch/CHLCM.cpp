#include "CHLCM.h"

#include "CLagCompensation.h"
#include <time.h>

SSDK::QAngle CreateMoveAngles = SSDK::QAngle(0, 0, 0);

namespace CHLCM {
	static CFixMove *FixMoveManager = new CFixMove();
	static CPredictionSystem *Pred = new CPredictionSystem();

	static float timer;
	static int iShotFired;

	float GetTraceFractionWorldProps(SSDK::Vector startpos, SSDK::Vector endpos) {
		SSDK::Ray_t ray;
		SSDK::Trace_t tr;
		SSDK::CTraceFilterWorldOnly filter;

		ray.Init(startpos, endpos);

		SSDK::I::EngineTrace()->TraceRay(ray, MASK_SHOT, &filter, &tr);

		return tr.fraction;
	}

	void CircleStrafer(SSDK::CUserCmd* pCmd, bool& active)
	{
		static float StrafeAngle;

		CBaseEntity *pLocal = CBaseEntity::GetLocalPlayer();
		SSDK::Vector Velocity = *pLocal->GetVelocity();
		Velocity.z = 0;

		float Speed = Velocity.Length();
		if (Speed < 45) Speed = 45;
		if (Speed > 750) Speed = 750;


		float FinalPath = GetTraceFractionWorldProps(*pLocal->GetOrigin() + SSDK::Vector(0, 0, 10), *pLocal->GetOrigin() + SSDK::Vector(0, 0, 10) + Velocity / 2.0f);
		float DeltaAngle = fmax((275.0f / Speed) * (2.0f / FinalPath) * (128.0f / (1.7f / SSDK::I::GlobalVars()->interval_per_tick)) * 4.f, 2.0f);
		StrafeAngle += DeltaAngle;

		if (fabs(StrafeAngle) >= 360.0f) {
			StrafeAngle = 0.0f;
			active = false;
		}
		else {
			pCmd->forwardmove = cos((StrafeAngle + 90) * (M_PI / 180.0f)) * 450.f;
			pCmd->sidemove = sin((StrafeAngle + 90) * (M_PI / 180.0f)) * 450.f;
		}
	}

	CreateMoveCHL_t g_fnOriginalCreateMoveCHL;
	void __stdcall Hooked_CreateMove_CHL(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket) {
		g_fnOriginalCreateMoveCHL((void*)SSDK::I::Client(), sequence_number, input_sample_frametime, active);

		if (SSDK::I::Engine()->isInGame() && SSDK::I::Engine()->isConnected()) {
			//VARS
			CBaseEntity *pLocalPlayer = CBaseEntity::GetLocalPlayer();

			if (!pLocalPlayer && !pLocalPlayer->IsAlive())
				return;

			//CHECK USERCMD
			SSDK::CVerifiedUserCmd* pVerif = &(*(SSDK::CVerifiedUserCmd**)((DWORD)SSDK::I::Input() + 0xF0))[sequence_number % 150];
			SSDK::CUserCmd *pCmd = &(*(SSDK::CUserCmd**)((DWORD)SSDK::I::Input() + 0xEC))[sequence_number % 150];

			if (pCmd && pVerif) {
				if (GetAsyncKeyState(0x4E) & 0x8000) {
					pCmd->tick_count = INT_MAX;
				}

				if (GetAsyncKeyState(VK_MBUTTON) & 0x1)
					OptionsManager.bAutoHop = !OptionsManager.bAutoHop;

				if (OptionsManager.bAutoHop) {
					pCmd->buttons |= IN_JUMP;

					if (pCmd->buttons & IN_JUMP && !(*pLocalPlayer->GetFlags() & (int)SSDK::EntityFlags::FL_ONGROUND)) {
						pCmd->buttons &= ~IN_JUMP;

						static bool isActive = false;

						if (GetAsyncKeyState(0x43) || isActive) {
							if (!isActive)
								isActive = true;
							CircleStrafer(pCmd, isActive);
						} else {
							if (pCmd->mousedx > 1 || pCmd->mousedx < -1) {
								pCmd->sidemove = pCmd->mousedx < 0.f ? -450.f : 450.f;
							}
							else {
								pCmd->forwardmove = (10000.f) / (pLocalPlayer->GetVelocity()->Length2D() + 1);
								pCmd->sidemove = (pCmd->command_number % 2) == 0 ? -450.f : 450.f;
							}
						}
					}


				} else {
					if (pCmd->buttons & IN_JUMP && !(*pLocalPlayer->GetFlags() & (int)SSDK::EntityFlags::FL_ONGROUND)) {
						pCmd->buttons &= ~IN_JUMP;

						static bool isActive = false;

						if (GetAsyncKeyState(0x43) || isActive) {
							if (!isActive)
								isActive = true;
							CircleStrafer(pCmd, isActive);
						}
						else {
							if (pCmd->mousedx > 1 || pCmd->mousedx < -1) {
								pCmd->sidemove = pCmd->mousedx < 0.f ? -450.f : 450.f;
							}
							else {
								pCmd->forwardmove = (10000.f) / (pLocalPlayer->GetVelocity()->Length2D() + 1);
								pCmd->sidemove = (pCmd->command_number % 2) == 0 ? -450.f : 450.f;
							}
						}
					}
				}

				//START PREDICTION
				Pred->StartPrediction(pCmd);
				FixMoveManager->Start(pCmd);

				//AIMBOT ROUTINE
				float flCurrentTime = SSDK::I::GlobalVars()->curtime;
				CBaseCombatWeapon *activeWeapon = pLocalPlayer->GetActiveWeapon();

				if (OptionsManager.bAutoPistol) // Badly Coded Auto Pistol will make new later.
				{
					if (pLocalPlayer->IsAlive() && pCmd->buttons & IN_ATTACK && activeWeapon->isPistol())
					{
						static bool FlipPistol = false;
						if (FlipPistol)
							pCmd->buttons &= ~IN_ATTACK;
						FlipPistol = !FlipPistol;
					}
				}

				/*static bool TargetFound = false;
				if (OptionsManager.bAutoRevolver)
				{
					if (activeWeapon->GetId() == SSDK::EItemDefinitionIndex::weapon_revolver)
					{
						float flPostponeFireReady = pWeapon->PostponeFireReady();
						pCmd->buttons |= IN_ATTACK;
						if (flPostponeFireReady > 0 && flPostponeFireReady <= (*pLocalPlayer->GetTickBase() * SSDK::I::GlobalVars()->interval_per_tick))
							if (!TargetFound)
								pCmd->buttons &= ~IN_ATTACK;
					}
				}*/

				for (int i = 1; i < SSDK::I::Engine()->GetMaxClients(); ++i) {
					CBaseEntity *pClient = (CBaseEntity*)SSDK::I::EntityList()->GetClientEntity(i);

					if (pClient->isValidPlayer()) {
						if (*pLocalPlayer->GetTeamNum() != *pClient->GetTeamNum()) {

							bool bPrioritize = OptionsManager.bPrioritize;
							int iHitbox = OptionsManager.iHitbox;
							bool bAutoBaim = OptionsManager.bAutoBaim;
							int iAutoBaimAferShot = OptionsManager.iAutoBaimAferShot;
							int iHitboxAutoBaim = OptionsManager.iHitboxAutoBaim;
							float flMinDmg = OptionsManager.flMinDmg;
							bool bInterpLagComp = OptionsManager.bInterpLagComp;

							if (OptionsManager.aCacheAngle[i].bCustomSetts) {
								bPrioritize = OptionsManager.aCacheAngle[i].bPrioritize;
								iHitbox = OptionsManager.aCacheAngle[i].iHitbox;
								bAutoBaim = OptionsManager.aCacheAngle[i].bAutoBaim;
								iAutoBaimAferShot = OptionsManager.aCacheAngle[i].iAutoBaimAferShot;
								iHitboxAutoBaim = OptionsManager.aCacheAngle[i].iHitboxAutoBaim;
								flMinDmg = OptionsManager.aCacheAngle[i].flMinDmg;
								bInterpLagComp = OptionsManager.aCacheAngle[i].bInterpLagComp;
							}

							if (bInterpLagComp)
								StartLagCompensation(pClient, pCmd);

							//RADAR
							*pClient->GetSpotted() = true;

							//AIMBOT
							SSDK::Vector vecClientTarget;

							if (iShotFired > iAutoBaimAferShot && bAutoBaim)
								vecClientTarget = CAutowall::CalculateBestPoint(pClient, iHitboxAutoBaim, flMinDmg, bPrioritize);
							else
								vecClientTarget = CAutowall::CalculateBestPoint(pClient, iHitbox, flMinDmg, bPrioritize);

							if (vecClientTarget.IsValid()) {
								SSDK::QAngle qaNewViewAngle = Utilities::CalcAngle(pLocalPlayer->GetEyePos(), vecClientTarget) - *pLocalPlayer->AimPunch() * 2.f;

								if (Utilities::Clamp(qaNewViewAngle)) {
									pCmd->viewangles = qaNewViewAngle;

									//DEBUG
									//SSDK::I::Engine()->SetViewAngles(pCmd->viewangles);
									if (activeWeapon && activeWeapon->isValidWeapon()) {
										if (activeWeapon->NextPrimaryAttack() <= flCurrentTime) {
											if (!pLocalPlayer->isScoped() && activeWeapon->isWeaponScope()) {
												if (OptionsManager.bAutoScope)
													pCmd->buttons |= IN_ATTACK2;
											}
											else if (Utilities::HitChance(pLocalPlayer, pClient, activeWeapon, qaNewViewAngle, pCmd->random_seed, OptionsManager.iHitchance)) {
												if (OptionsManager.bAutoShoot) {
													if (activeWeapon->GetCurrentAmmo())
														pCmd->buttons |= IN_ATTACK;
												}
											}
										}
									}
								}
							}

						}
					}
				}

				//Calculate shot fired
				if (activeWeapon) {
					if (!(pCmd->buttons & IN_ATTACK) && activeWeapon->NextPrimaryAttack() <= flCurrentTime) {
						iShotFired = 0;
					}
					else {
						iShotFired += pLocalPlayer->GetShotFired();
					}
				}

				if (OptionsManager.bSilentAim) {
					if (pCmd->buttons & IN_ATTACK) {
						bSendPacket = false;
					}
					else {
						bSendPacket = true;
						pCmd->viewangles = FixMoveManager->GetOldAngle();
					}
				}

				//END PREDICTION
				FixMoveManager->Stop(pCmd);
				Pred->EndPrediction();

				//FAKE LAG FILS DE
				static int iTick;
				if (OptionsManager.bFakeLag)
					OptionsManager.iFakeLagAmount = Utilities::FakelagCompensationBreak();
				if (iTick < OptionsManager.iFakeLagAmount && !(pCmd->buttons & IN_ATTACK)) {
					bSendPacket = false;
					iTick++;
				}
				else {
					bSendPacket = true;
					iTick = 0;
				}

				SSDK::QAngle cache = pCmd->viewangles;

				//AA
				if (OptionsManager.bAA) {
					if (activeWeapon) {
						if (!(pCmd->buttons & IN_USE) &&
							!(pCmd->buttons & IN_ATTACK) ||
							(activeWeapon->isGrenade())) {
							if (!(activeWeapon->getGrenade()->GetThrowTime() > 0.f)) {
								FixMoveManager->Start(pCmd);
								SSDK::QAngle newAngle = pCmd->viewangles;

								float yaw;
								static int state = 0;
								static bool bFlip = false;

								float flCurTime = SSDK::I::GlobalVars()->curtime;
								static float flTimeUpdate = 1.09f;
								static float flNextTimeUpdate = flCurTime + flTimeUpdate;
								if (flCurTime >= flNextTimeUpdate) {
									bFlip = !bFlip;
									state = 0;
								}

								if (flNextTimeUpdate < flCurTime || flNextTimeUpdate - flCurTime > 10.f)
									flNextTimeUpdate = flCurTime + flTimeUpdate;

								if (pLocalPlayer->isMoving()) {
									//MOVING YAW
									if (bSendPacket)
									{
										yaw = 89.f;
									}
									else {
										yaw = Utilities::RandomFloat(165, 195);
									}
									newAngle.y += yaw;
								}
								else {
									if (bSendPacket) {
										//REAL YAW
										if (OptionsManager.iAARealYaw == 0) {
											if (bFlip)
												yaw = -90;
											else
												yaw = 90;
										}
										else if (OptionsManager.iAARealYaw == 1) {
											yaw = 90;
										}
									}
									else {
										//FAKE YAW
										if (OptionsManager.iAAFakeYaw == 0) {
											if (bFlip) {
												yaw = 90.f + state * 34.f;
												if (state > 1)
													yaw = 90.f + 34.8f;
											}
											else {
												yaw = -90.f - state * 34.f;
												if (state > 1)
													yaw = -90.f - 34.8f;
											}
										}
										else if (OptionsManager.iAAFakeYaw == 1) {
											if (bFlip) {
												yaw = 110.f + state * 34.f;
												if (state < 3)
													state++;
											}
											else {
												yaw = -110.f - state * 34.f;
												if (state < 3)
													state++;
											}
										}
										else if (OptionsManager.iAAFakeYaw == 2) {
											yaw = 180.f;
										}
										else if (OptionsManager.iAAFakeYaw == 3) {
											newAngle = *pLocalPlayer->GetEyeAngles();
											yaw = 34.f;
											state++;
										}
									}
									if (yaw)
										newAngle.y += yaw;
								}

								//REAL PITCH
								if (OptionsManager.iAAPitch == 0)
									newAngle.x = 89.f;
								else if (OptionsManager.iAAPitch == 1)
									newAngle.x = -89.f;
								else if (OptionsManager.iAAPitch == 2)
									newAngle.x = 0.f;

								if (Utilities::Clamp(newAngle)) {
									pCmd->viewangles = newAngle;
								}

								FixMoveManager->Stop(pCmd);
							}
						}
					}
				}

				pVerif->m_cmd = *pCmd;
				pVerif->m_crc = pCmd->GetChecksum();
			}
			if (!bSendPacket && OptionsManager.iAAFakeYaw >= 0)
				CreateMoveAngles = pCmd->viewangles;
		}
	}
}