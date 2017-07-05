#include "DirectX.h"

#include <fstream>

namespace DirectX {
	EndScene_t g_fnOriginalEndScene;
	Reset_t g_fnOriginalReset;

	WNDPROC g_pOldWindowProc;
	HWND g_hWindow;

	bool g_IsInitialized = false;

	HRESULT __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice){
		if (!g_IsInitialized) {
			ImGui_ImplDX9_Init(g_hWindow, pDevice);

			ImGuiStyle * style = &ImGui::GetStyle();

			style->WindowPadding = ImVec2(15, 15);
			style->WindowRounding = 5.0f;
			style->FramePadding = ImVec2(5, 5);
			style->FrameRounding = 4.0f;
			style->ItemSpacing = ImVec2(12, 8);
			style->ItemInnerSpacing = ImVec2(8, 6);
			style->IndentSpacing = 25.0f;
			style->ScrollbarSize = 15.0f;
			style->ScrollbarRounding = 9.0f;
			style->GrabMinSize = 5.0f;
			style->GrabRounding = 3.0f;

			style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
			style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
			style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
			style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
			style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
			style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
			style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
			style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
			style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
			style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
			style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
			style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style->Colors[ImGuiCol_Header] = ImVec4(0.12f, 0.09f, 0.12f, 1.00f);
			style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
			style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
			style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
			style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
			style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
			style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
			style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
			style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
			style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

			g_IsInitialized = true;
		}
		else {
			ImGui::GetIO().MouseDrawCursor = OptionsManager.bShowMenu;

			//Begins a new ImGui frame.
			ImGui_ImplDX9_NewFrame();


			ImGui::SetNextWindowPos(ImVec2(100, 200), ImGuiSetCond_FirstUseEver);

			// Testing Not 100% Done Badly Coded Will Recode
			if (OptionsManager.bIndicators)
			{
				ImGui::Begin(XorStr("Indicator"), &OptionsManager.bShowMenu, ImVec2(100, 100), 0.2f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
				{
					if (CBaseEntity::GetLocalPlayer()) {
						if (CBaseEntity::GetLocalPlayer()->IsAlive()) {
							if (CreateMoveAngles.y - *CBaseEntity::GetLocalPlayer()->GetLowerBodyYaw() >= -15 && CreateMoveAngles.y - *CBaseEntity::GetLocalPlayer()->GetLowerBodyYaw() <= 15)
								ImGui::TextColored(ImVec4(255, 0, 0, 255), XorStr("LBY"));
							else
								ImGui::TextColored(ImVec4(0, 255, 0, 255), XorStr("LBY"));
						}
					}
				}
				ImGui::End();
			}

			if (OptionsManager.bShowMenu) {
				//Begin Main window code
				ImGui::Begin(XorStr("Narcissisti.cc v72 ALPHA - Main"), &OptionsManager.bShowMenu, ImVec2(400, 500), 1.f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
				{
					if (ImGui::CollapsingHeader(XorStr("Visuals"))) {
						ImGui::Checkbox(XorStr("Enable ESP"), &OptionsManager.bEspEnabled);
						if (OptionsManager.bEspEnabled) {
							ImGui::Checkbox(XorStr("Glow"), &OptionsManager.bGlow);
							ImGui::Checkbox(XorStr("Health"), &OptionsManager.bHealthBar);
							ImGui::Checkbox(XorStr("Armor"), &OptionsManager.bArmorBar);
							ImGui::Checkbox(XorStr("Player Name"), &OptionsManager.bPlayerName);
							ImGui::Checkbox(XorStr("Player Index"), &OptionsManager.bPlayerIndex);
							ImGui::Checkbox(XorStr("Bone ESP"), &OptionsManager.bBoneESP);
							ImGui::Checkbox(XorStr("Weapon ESP"), &OptionsManager.bWeaponESP);
							ImGui::Checkbox(XorStr("No Scope"), &OptionsManager.bNoScope);
						}
					}

					if (ImGui::CollapsingHeader(XorStr("Aimbot"))) {
						ImGui::Checkbox(XorStr("Auto Shoot"), &OptionsManager.bAutoShoot);
						ImGui::Checkbox(XorStr("Auto Pistol"), &OptionsManager.bAutoPistol);
						ImGui::Checkbox(XorStr("Auto Scope"), &OptionsManager.bAutoScope);
						ImGui::Checkbox(XorStr("Silent aim"), &OptionsManager.bSilentAim);
						ImGui::SliderInt(XorStr("HitChance"), &OptionsManager.iHitchance, 0, 100);
						ImGui::SliderInt(XorStr("HitChance Max Seeds"), &OptionsManager.iHitChanceSeed, 0, 256);

						ImGui::Checkbox(XorStr("Resolve angle"), &OptionsManager.bCorrect);
						static const char *szResolveType[] = { XorStr("Simple"), XorStr("Delta calc"), XorStr("Delta calc 2"), XorStr("Anti-Anti-Resolver") };
						ImGui::Combo(XorStr("Resolver type"), &OptionsManager.iResolverType, szResolveType, ARRAYSIZE(szResolveType));

						
						static const char *szMultiHitboxes[] = { 
							XorStr("Head"), XorStr("Neck"), XorStr("Upper Chest"), XorStr("Chest"), XorStr("Thorax"),
							XorStr("Body"), XorStr("Left Forearm"), XorStr("Right Forearm"), XorStr("Hands"), XorStr("Left Thigh"),
							XorStr("Right Thigh"), XorStr("Left Calf"), XorStr("Right Calf"), XorStr("Left Foot"), XorStr("Right Foot")
						};

						ImGui::Text("Multipoint hitboxes scan :");
						ImGui::BeginChild("#multipoint", ImVec2(0, 200), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
						for (int i = 0; i < ARRAYSIZE(szMultiHitboxes); ++i) {
							ImGui::Selectable(szMultiHitboxes[i], &OptionsManager.aMultiHitboxes[i]);
						}
						ImGui::EndChild();

						ImGui::Checkbox(XorStr("Awall Prioritize Visible Parts"), &OptionsManager.bPrioritizeVis);
						ImGui::Checkbox(XorStr("Prioritize Selected Hitbox"), &OptionsManager.bPrioritize);
						ImGui::SliderInt(XorStr("Select Hitbox"), &OptionsManager.iHitbox, 0, 20);
						ImGui::SliderFloat(XorStr("Min Damage"), &OptionsManager.flMinDmg, 0, 100);
						ImGui::Checkbox(XorStr("Auto Body-Aim"), &OptionsManager.bAutoBaim);
						ImGui::SliderInt(XorStr("Body-Aim After Shots"), &OptionsManager.iAutoBaimAferShot, 0, 10);
						ImGui::SliderInt(XorStr("Select Hitbox Auto Body-Aim"), &OptionsManager.iHitboxAutoBaim, 0, 20);
						ImGui::Checkbox(XorStr("Disable Interp/Lag Compensate"), &OptionsManager.bInterpLagComp);
					}

					if (ImGui::CollapsingHeader(XorStr("Anti-aim"))) {
						ImGui::Checkbox(XorStr("Fakelag"), &OptionsManager.bFakeLag);
						ImGui::SliderInt(XorStr("Choke Packets"), &OptionsManager.iFakeLagAmount, 0, 50);
						
						ImGui::Checkbox(XorStr("AA Enable"), &OptionsManager.bAA);
						static const char *szAAPitch[] = { XorStr("Down"), XorStr("Up"), XorStr("Mid") };
						ImGui::Combo(XorStr("Pitch AA"), &OptionsManager.iAAPitch, szAAPitch, ARRAYSIZE(szAAPitch));

						static const char *szRealAAYaw[] = { XorStr("Simple 90 Jitter"), XorStr("Static side") };
						ImGui::Combo(XorStr("Real Yaw AA"), &OptionsManager.iAARealYaw, szRealAAYaw, ARRAYSIZE(szRealAAYaw));

						static const char *szFakeAAYaw[] = { XorStr("Kidua A-Resolver"), XorStr("Sqch A-Resolver"), XorStr("Backward"), XorStr("TEST") };
						ImGui::Combo(XorStr("Fake Yaw AA"), &OptionsManager.iAAFakeYaw, szFakeAAYaw, ARRAYSIZE(szFakeAAYaw));
					}

					if (ImGui::CollapsingHeader(XorStr("Misc"))) {
						ImGui::Checkbox(XorStr("No Flash"), &OptionsManager.bNoFlash);
						ImGui::Checkbox(XorStr("No Visual Recoil"), &OptionsManager.bNoVisRecoil);
						ImGui::SliderInt(XorStr("Fov"), &OptionsManager.iFov, 0, 90);
						ImGui::Checkbox(XorStr("Water Mark"), &OptionsManager.bWaterMark);
						ImGui::Checkbox(XorStr("Indicators"), &OptionsManager.bIndicators);
					}
				}
				ImGui::End();

				ImGui::SetNextWindowPos(ImVec2(550, 200), ImGuiSetCond_FirstUseEver);
				//Begin Target window code
				ImGui::Begin(XorStr("Narcissisti.cc v72 ALPHA - Target Selector"), &OptionsManager.bShowMenu, ImVec2(400, 500), 1.f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
				{
					for (int i = 1; i < SSDK::I::Engine()->GetMaxClients(); ++i) {
						CBaseEntity *pClient = (CBaseEntity*)SSDK::I::EntityList()->GetClientEntity(i);
						if (pClient) {
							if (*pClient->GetTeamNum() != *CBaseEntity::GetLocalPlayer()->GetTeamNum()) {
								SSDK::PlayerInfo entInfo;
								SSDK::I::Engine()->GetPlayerInfo(i, &entInfo);
								if (ImGui::CollapsingHeader(entInfo.szName)) {
									ImGui::Checkbox(XorStr("Use custom settings"), &OptionsManager.aCacheAngle[i].bCustomSetts);
									ImGui::Checkbox(XorStr("Resolve angle"), &OptionsManager.aCacheAngle[i].bCorrect);
									static const char *szResolveType[] = { XorStr("Simple"), XorStr("Delta calc"), XorStr("Delta calc 2"), XorStr("Anti-Anti-Resolver") };
									ImGui::Combo(XorStr("Resolver type"), &OptionsManager.aCacheAngle[i].iResolverType, szResolveType, ARRAYSIZE(szResolveType));
									ImGui::Checkbox(XorStr("Awall Prioritize visible parts"), &OptionsManager.aCacheAngle[i].bPrioritizeVis);
									ImGui::Checkbox(XorStr("Prioritize selected hitbox"), &OptionsManager.aCacheAngle[i].bPrioritize);
									ImGui::SliderInt(XorStr("Select hitbox"), &OptionsManager.aCacheAngle[i].iHitbox, 0, 20);
									ImGui::SliderFloat(XorStr("Min dmg"), &OptionsManager.aCacheAngle[i].flMinDmg, 0, 100);
									ImGui::Checkbox(XorStr("Auto baim"), &OptionsManager.aCacheAngle[i].bAutoBaim);
									ImGui::SliderInt(XorStr("Baim after shot"), &OptionsManager.aCacheAngle[i].iAutoBaimAferShot, 0, 10);
									ImGui::SliderInt(XorStr("Select hitbox auto baim"), &OptionsManager.aCacheAngle[i].iHitboxAutoBaim, 0, 20);
									ImGui::Checkbox(XorStr("Disable interp/Lag compensate"), &OptionsManager.aCacheAngle[i].bInterpLagComp);
								}
							}
						}
					}
				}
				ImGui::End();
			}
			ImGui::Render();
		}

		return g_fnOriginalEndScene(pDevice);
	}

	HRESULT __stdcall Hooked_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters){
		if (!g_IsInitialized) return g_fnOriginalReset(pDevice, pPresentationParameters);
		ImGui_ImplDX9_InvalidateDeviceObjects();

		auto hr = g_fnOriginalReset(pDevice, pPresentationParameters);
		ImGui_ImplDX9_CreateDeviceObjects();
		return hr;
	}

	LRESULT __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
		case WM_LBUTTONDOWN:
			OptionsManager.vecPressedKeys[VK_LBUTTON] = true;
			break;
		case WM_LBUTTONUP:
			OptionsManager.vecPressedKeys[VK_LBUTTON] = false;
			break;
		case WM_RBUTTONDOWN:
			OptionsManager.vecPressedKeys[VK_RBUTTON] = true;
			break;
		case WM_RBUTTONUP:
			OptionsManager.vecPressedKeys[VK_RBUTTON] = false;
			break;
		case WM_KEYDOWN:
			OptionsManager.vecPressedKeys[wParam] = true;
			break;
		case WM_KEYUP:
			OptionsManager.vecPressedKeys[wParam] = false;
			break;
		default: break;
		}

		static SSDK::ConVar *conMouseEnable = SSDK::I::CVar()->FindVar(XorStr("cl_mouseenable"));
		static bool isDown = false;
		static bool isClicked = false;
		if (OptionsManager.vecPressedKeys[VK_INSERT]) {
			isClicked = false;
			isDown = true;
		}
		else if (!OptionsManager.vecPressedKeys[VK_INSERT] && isDown) {
			isClicked = true;
			isDown = false;
		}
		else {
			isClicked = false;
			isDown = false;
		}

		if (isClicked) {
			if (OptionsManager.bShowMenu) {
				//CONFIG SAVER - LOAD
				std::ofstream ofs("narcissisticc.save", std::ios::binary);
				ofs.write((char*)&OptionsManager, sizeof(OptionsManager));
			}
			OptionsManager.bShowMenu = !OptionsManager.bShowMenu;
			conMouseEnable->SetValue(!OptionsManager.bShowMenu);
		}


		if (g_IsInitialized && OptionsManager.bShowMenu && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return true;

		return CallWindowProc(g_pOldWindowProc, hWnd, uMsg, wParam, lParam);
	}
}