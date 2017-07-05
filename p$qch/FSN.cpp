#include "FSN.h"

namespace FSN {
	FrameStageNotify_t g_fnOriginalFrameStageNotify;
	VOID __fastcall Hooked_FrameStageNotify(void* thisptr, void* edx, SSDK::ClientFrameStage_t curStage) {
		CBaseEntity *pLocalPlayer = CBaseEntity::GetLocalPlayer();

		SSDK::Vector vecAimPunch;
		SSDK::Vector vecViewPunch;

		SSDK::Vector* pAimPunch = nullptr;
		SSDK::Vector* pViewPunch = nullptr;


		if (SSDK::I::Engine()->isInGame()) {
			if (curStage == SSDK::ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
				for (int i = 1; i < SSDK::I::Engine()->GetMaxClients(); ++i) {
					CBaseEntity *pClient = (CBaseEntity*)SSDK::I::EntityList()->GetClientEntity(i);
					if (pClient->isValidPlayer()) {
						//RESOLVER 
						bool bCorrect = OptionsManager.bCorrect;
						int iResolverType = OptionsManager.iResolverType;

						if (OptionsManager.aCacheAngle[i].bCustomSetts) {
							bCorrect = OptionsManager.aCacheAngle[i].bCorrect;
							iResolverType = OptionsManager.aCacheAngle[i].iResolverType;
						}

						if (bCorrect) {
							if (iResolverType == 0) {
								pClient->GetEyeAngles()->y = *pClient->GetLowerBodyYaw();
							}

							if (iResolverType == 1) {
								static float lowerDelta[64];
								static float lastYaw[64];

								float curLower = *pClient->GetLowerBodyYaw();
								float curYaw = pClient->GetEyeAngles()->y;

								lowerDelta[i] = curYaw - curLower;
								Utilities::Normalize(lowerDelta[i]);

								if (fabs(lowerDelta[i]) > 15.f) {
									if (pClient->isMoving()) {
										pClient->GetEyeAngles()->y = *pClient->GetLowerBodyYaw();
									}
									else {
										if (curYaw != lastYaw[i]) {
											pClient->GetEyeAngles()->y += lowerDelta[i];
											lastYaw[i] = curYaw;
										}
									}
								}
							}

							if (iResolverType == 2) {
								static float OldLowerBodyYaws[64];
								static float OldYawDeltas[64];
								float CurYaw = *pClient->GetLowerBodyYaw();
								if (OldLowerBodyYaws[i] != CurYaw) {
									OldYawDeltas[i] = pClient->GetEyeAngles()->y - CurYaw;
									OldLowerBodyYaws[i] = CurYaw;
									pClient->GetEyeAngles()->y = CurYaw;
									continue;
								}
								else {
									pClient->GetEyeAngles()->y = pClient->GetEyeAngles()->y - OldYawDeltas[i];
								}
							}

							if (iResolverType == 3) {
								float flYaw = *pClient->GetLowerBodyYaw();

								if (!pClient->isMoving()) {
									float flCurTime = SSDK::I::GlobalVars()->curtime;
									static float flTimeUpdate = 0.5f;
									static float flNextTimeUpdate = flCurTime + flTimeUpdate;

									if (flCurTime >= flNextTimeUpdate) {
										OptionsManager.aCacheAngle[i].bFlip = !OptionsManager.aCacheAngle[i].bFlip;
									}

									if (flNextTimeUpdate < flCurTime || flNextTimeUpdate - flCurTime > 10.f)
										flNextTimeUpdate = flCurTime + flTimeUpdate;
								
									if (OptionsManager.aCacheAngle[i].bFlip) {
										flYaw += 35.f;
									}
									else {
										flYaw -= 35.f;
									}
								}

								pClient->GetEyeAngles()->y = flYaw;
							}
						}
					}
				}
			}

			if (curStage == SSDK::ClientFrameStage_t::FRAME_RENDER_START) {
				if (pLocalPlayer && pLocalPlayer->IsAlive())
				{
					*(int*)((uintptr_t)pLocalPlayer + 0xA30) = SSDK::I::GlobalVars()->framecount; //we'll skip occlusion checks now
					*(int*)((uintptr_t)pLocalPlayer + 0xA28) = 0;//clear occlusion flags
				}

				if (OptionsManager.bNoVisRecoil) {
					pAimPunch = pLocalPlayer->AimPunch();
					pViewPunch = pLocalPlayer->ViewPunch();

					vecAimPunch = *pAimPunch;
					vecViewPunch = *pViewPunch;

					*pAimPunch = SSDK::QAngle(0, 0, 0);
					*pViewPunch = SSDK::QAngle(0, 0, 0);
				}

				if (OptionsManager.bNoFlash)
					if (*pLocalPlayer->flashDuration() > 0.f)
						*pLocalPlayer->flashDuration() = 0.f;

				//THIRDPERSON AA
				if(*(bool*)((DWORD)SSDK::I::Input() + 0xA5))
					*(SSDK::QAngle*)((DWORD)pLocalPlayer + 0x031C8) = SSDK::QAngle(pLocalPlayer->GetEyeAngles()->x, CreateMoveAngles.y, 0);//OptionsManager.qaLocalEyesAngle;
			}
		}

		//Original
		g_fnOriginalFrameStageNotify(thisptr, curStage);

		if (OptionsManager.bNoVisRecoil) {
			if (curStage == SSDK::ClientFrameStage_t::FRAME_RENDER_START) {
				if (pAimPunch && pViewPunch) {
					*pAimPunch = vecAimPunch;
					*pViewPunch = vecViewPunch;
				}
			}
		}

		if (curStage == SSDK::ClientFrameStage_t::FRAME_RENDER_END) {
			//UpdateClientAnim
			for (int i = 1; i < SSDK::I::Engine()->GetMaxClients(); ++i) {
				CBaseEntity *pClient = (CBaseEntity*)SSDK::I::EntityList()->GetClientEntity(i);
				if (pClient->isValidPlayer()) {
					pClient->UpdateClientSideAnimation();
				}
			}
		}
	}
}