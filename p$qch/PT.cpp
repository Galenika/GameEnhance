#include "PT.h"

namespace PT {
	PaintTraverse_t g_fnOriginalPaintTraverse;
	void __fastcall Hooked_PaintTraverse(void* thisptr, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce) {
		static unsigned int iSelectedPanelZoom = 0;
		if (!iSelectedPanelZoom) {
			if (strstr(SSDK::I::VGUIPanel()->GetName(vguiPanel), XorStr("HudZoom")))
				iSelectedPanelZoom = vguiPanel;
		}
		else if (vguiPanel == iSelectedPanelZoom) {
			return;
		}

		g_fnOriginalPaintTraverse(thisptr, vguiPanel, forceRepaint, allowForce);

		static unsigned int iSelectedPanelMat = 0;

		if (!iSelectedPanelMat) {
			if (strstr(SSDK::I::VGUIPanel()->GetName(vguiPanel), XorStr("FocusOverlayPanel")))
				iSelectedPanelMat = vguiPanel;
		} else if (vguiPanel == iSelectedPanelMat) {
			if (OptionsManager.bWaterMark) {
				int iScreenWidth, iScreenHeight;
				SSDK::I::Engine()->GetScreenSize(iScreenWidth, iScreenHeight);
				DrawManager::DrawString(SSDK::Color(255, 255, 255), DrawManager::hFont, XorStr("narcissisti.cc"), iScreenWidth - 300, 10); // LAFF
				DrawManager::DrawString(SSDK::Color(151, 51, 255), DrawManager::hFont, XorStr("ALPHA v0.72"), iScreenWidth - 300, 22.5); // B1G WATERMARK LAFF
			}

			if (SSDK::I::Engine()->isInGame()) {
				if (OptionsManager.bNoScope) {
					CBaseEntity *pLocalPlayer = CBaseEntity::GetLocalPlayer();
					if (pLocalPlayer->isValidPlayer(true)) {
						if (pLocalPlayer->isScoped()) {
							DrawManager::DrawZoom();
						}
					}
				}

				for (int i = 0; i < SSDK::I::EntityList()->GetHighestEntityIndex(); ++i) {
					//if (SSDK::O::GlowManager()->m_GlowObjectDefinitions[i].IsEmpty() || !SSDK::O::GlowManager()->m_GlowObjectDefinitions[i].getEntity())
						//continue;

					CBaseEntity *pEntity = (CBaseEntity*)SSDK::I::EntityList()->GetClientEntity(i);
					SSDK::CGlowObjectManager::GlowObjectDefinition_t* glowEntity = (SSDK::CGlowObjectManager::GlowObjectDefinition_t*)SSDK::I::EntityList()->GetClientEntity(i);

					if (!pEntity)
						continue;

					if (OptionsManager.bEspEnabled && pEntity->isValidPlayer()) {
						//ESP
						SSDK::Vector vecHeadHitBox = pEntity->GetEntityHitbox(0), vecOrigin = *pEntity->GetOrigin();
						SSDK::Vector vecHeadHitBox2D, vecOrigin2D;

						vecHeadHitBox.z += 10.f;

						if (Utilities::W2S(vecHeadHitBox, vecHeadHitBox2D) &&
							Utilities::W2S(vecOrigin, vecOrigin2D)) {
							float height = vecOrigin2D.y - vecHeadHitBox2D.y;
							float width = height * 0.45f;

							if (OptionsManager.bHealthBar)
								DrawManager::DrawHealthBar(pEntity->GetHealth(), vecHeadHitBox2D.x - (width * 0.5), vecHeadHitBox2D.y - (height * 0.105), vecHeadHitBox2D.x + (width * 0.5), vecHeadHitBox2D.y - (height * 0.070));
							if (OptionsManager.bArmorBar)
								DrawManager::DrawArmorBar(pEntity->GetArmor(), vecHeadHitBox2D.x - (width * 0.5), vecHeadHitBox2D.y - (height * 0.070), vecHeadHitBox2D.x + (width * 0.5), vecHeadHitBox2D.y - (height * 0.040));
							if (OptionsManager.bPlayerName)
								DrawManager::DrawPlayerName(pEntity->EntIndex(), vecHeadHitBox2D.x, vecHeadHitBox2D.y + (height * 1.10));
							if (OptionsManager.bPlayerIndex)
								DrawManager::DrawIndex(SSDK::Color(255, 255, 0), pEntity, vecHeadHitBox2D.x, vecHeadHitBox2D.y);
							if (OptionsManager.bBoneESP)
								DrawManager::DrawBoneESP(SSDK::Color(255, 255, 255), pEntity);
							if (OptionsManager.bWeaponESP)
								DrawManager::DrawWeaponESP(SSDK::Color(255, 255, 255), pEntity, vecHeadHitBox2D.x, vecHeadHitBox2D.y + (height * 1.15));
						}

						//GLOW
						if (OptionsManager.bGlow) {
							/*SSDK::Color colTeam;
							switch (*pEntity->GetTeamNum()) {
							case 2:
								colTeam = SSDK::Color(204, 102, 50, 200);
								break;
							case 3:
								colTeam = SSDK::Color(50, 102, 204, 200);
								break;
							default:
								colTeam = SSDK::Color(255, 255, 255);
								break;
							}
							glowEntity->set(colTeam);*/
						}
					}
				}
			}
		}
	}
}