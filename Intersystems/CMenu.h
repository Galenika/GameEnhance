#pragma once

bool get_system_font_path(const std::string& name, std::string& path)
{
	//
	// This code is not as safe as it should be.
	// Assumptions we make:
	//  -> GetWindowsDirectoryA does not fail.
	//  -> The registry key exists.
	//  -> The subkeys are ordered alphabetically
	//  -> The subkeys name and data are no longer than 260 (MAX_PATH) chars.
	//

	char buffer[MAX_PATH];
	HKEY registryKey;

	GetWindowsDirectoryA(buffer, MAX_PATH);
	std::string fontsFolder = buffer + std::string("\\Fonts\\");

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &registryKey)) {
		return false;
	}

	uint32_t valueIndex = 0;
	char valueName[MAX_PATH];
	uint8_t valueData[MAX_PATH];
	std::wstring wsFontFile;

	do {
		uint32_t valueNameSize = MAX_PATH;
		uint32_t valueDataSize = MAX_PATH;
		uint32_t valueType;

		auto error = RegEnumValueA(
			registryKey,
			valueIndex,
			valueName,
			reinterpret_cast<DWORD*>(&valueNameSize),
			0,
			reinterpret_cast<DWORD*>(&valueType),
			valueData,
			reinterpret_cast<DWORD*>(&valueDataSize));

		valueIndex++;

		if (error == ERROR_NO_MORE_ITEMS) {
			RegCloseKey(registryKey);
			return false;
		}

		if (error || valueType != REG_SZ) {
			continue;
		}

		if (_strnicmp(name.data(), valueName, name.size()) == 0) {
			path = fontsFolder + std::string((char*)valueData, valueDataSize);
			RegCloseKey(registryKey);
			return true;
		}
	} while (true);

	return false;
}

namespace CMenu
{
	bool aimbotTab = true;
	bool visualsTab = false;
	bool radarTab = false;
	bool SkinchangerTab = false;
	bool colorsTab = false;
	bool miscTab = false;

	bool visEspTab = true;
	bool visChamsTab = false;
	bool visHandsTab = false;
	bool visMiscTab = false;

	int windowWidth = 800;
	int windowHeight = 625;
	int curWidth = 1;
	int curHeight = 1;
	int curX = 0;
	int curY = 0;
	int tabHeight = 30;

	LONGLONG LastFrameTime;
	float deltaTime;

	LONGLONG milliseconds_now() {
		static LARGE_INTEGER s_frequency;
		static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
		if (s_use_qpc) {
			LARGE_INTEGER now;
			QueryPerformanceCounter(&now);
			return (1000LL * now.QuadPart) / s_frequency.QuadPart;
		}
		else {
			return GetTickCount();
		}
	}

	float GetDeltaTime()
	{
		LONGLONG ms = milliseconds_now();
		float ret = ms - LastFrameTime;
		LastFrameTime = ms;
		return ret;
		//return 1000.0f / ImGui::GetIO().Framerate;
	}

	int CalcTabWidth(int tabs)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		//return ((windowWidth - (style.WindowPadding.x * 2)) / tabs) - (style.ItemSpacing.x * tabs);
		return (curWidth - ((style.WindowPadding.x ) + (style.ItemSpacing.x * tabs))) / tabs;
		//return windowWidth / tabs;
	}

	void SelectTab(bool* tab)
	{
		aimbotTab = false;
		visualsTab = false;
		radarTab = false;
		SkinchangerTab = false;
		colorsTab = false;
		miscTab = false;

		*tab = true;
	}

	void SelectVisualsSubTab(bool* tab)
	{
		visEspTab = false;
		visChamsTab = false;
		visHandsTab = false;
		visMiscTab = false;

		*tab = true;
	}

	bool DrawTab(char* name, int tabWidth, int tabHeight, bool *boole, bool forceTabs = true)
	{
		bool ret = false;
		if (ImGui::Tab(name, ImVec2(tabWidth, tabHeight), *boole))
		{
			ret = true;
			*boole = true;
			if (forceTabs) SelectTab(boole);
		}

		ImGui::SameLine(0);
		return ret;
	}

	bool DrawTab(char* name, int tabWidth, int tabHeight, bool boole, bool forceTabs = true)
	{
		return DrawTab(name, tabWidth, tabHeight, &boole, forceTabs);
	}

	void DrawTabs()
	{
		int w, h;
		Interface.Engine->GetScreenSize(w, h);
		bool notFull = curWidth < windowWidth || curHeight < windowHeight;


		if (G::MenuOpened && notFull)
		{
				ImGui::SetNextWindowPos(ImVec2(curX, curY));
				ImGui::SetNextWindowSize(ImVec2(curWidth, curHeight));
		}
		if (ImGui::Begin(XorStr("InterSystems for Counter-Strike: Global Offensive"), &G::MenuOpened, ImVec2(0, 0), 1.0F, (notFull ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
		{
			if (notFull) return;
			ImGui::NewLine(3);

			ImGuiContext* io = ImGui::GetCurrentContext();
			ImGuiStyle& style = ImGui::GetStyle();
			int tabWidth = CalcTabWidth(6);
			
			float lastSize = io->FontSize;
			io->FontSize = 17;
			DrawTab(XorStr("Aimbot"), tabWidth, tabHeight, &aimbotTab);
			DrawTab(XorStr("Visuals"), tabWidth, tabHeight, &visualsTab);
			DrawTab(XorStr("Radar"), tabWidth, tabHeight, &radarTab);
			DrawTab(XorStr("Skin Changer"), tabWidth, tabHeight, &SkinchangerTab);
			DrawTab(XorStr("Misc"), tabWidth, tabHeight, &miscTab);
			DrawTab(XorStr("Settings"), tabWidth, tabHeight, &colorsTab);
			io->FontSize = lastSize;

			//ImGui::GetWindowDrawList()->AddRect(ImVec2(windowPos.x + style.WindowPadding.x - 1, windowPos.y + style.WindowPadding.x - 1),
			//														  ImVec2(windowPos.x + windowWidth + style.WindowPadding.x - 1, windowPos.y + 53), ImGui::GetColorU32(ImVec4(0, 0, 0, 1)), 0.0F, -1, 2.0F);

			ImGui::NewLine();
			ImGui::Separator(6, 2);
			//ImGui::NewLine();
			ImGui::NewLine();
		}
	}

	void DrawRadar()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 oldPadding = style.WindowPadding;
		float oldAlpha = style.Colors[ImGuiCol_WindowBg].w;
		style.WindowPadding = ImVec2(0, 0);
		style.Colors[ImGuiCol_WindowBg].w = (float)Options::Radar::Alpha / 255.0f;
		if (ImGui::Begin(XorStr("Radar"), &G::MenuOpened, ImVec2(200, 200), 0.4F, ImGuiWindowFlags_NoTitleBar | /*ImGuiWindowFlags_NoResize | */ImGuiWindowFlags_NoCollapse))
		{
			ImVec2 siz = ImGui::GetWindowSize();
			ImVec2 pos = ImGui::GetWindowPos();

			_drawList->AddRect(ImVec2(pos.x - 6, pos.y - 6), ImVec2(pos.x + siz.x + 6, pos.y + siz.y + 6), Color::Black().GetU32(), 0.0F, -1, 1.5f);
			//_drawList->AddRect(ImVec2(pos.x - 2, pos.y - 2), ImVec2(pos.x + siz.x + 2, pos.y + siz.y + 2), Color::Black().GetU32(), 0.0F, -1, 1);

			//_drawList->AddRect(ImVec2(pos.x - 2, pos.y - 2), ImVec2(pos.x + siz.x + 2, pos.y + siz.y + 2), Color::Black().GetU32(), 0.0F, -1, 2);
			//_drawList->AddRect(ImVec2(pos.x - 2, pos.y - 2), ImVec2(pos.x + siz.x + 2, pos.y + siz.y + 2), Color::Silver().GetU32(), 0.0F, -1, 1.1f);

			if (G::NextResetRadar)
			{
				ImGui::SetWindowSize(ImVec2(200, 200));
				G::NextResetRadar = false;
			}

			ImDrawList* windowDrawList = ImGui::GetWindowDrawList();
			windowDrawList->AddLine(ImVec2(pos.x + (siz.x / 2), pos.y +  0), ImVec2(pos.x + (siz.x / 2), pos.y + siz.y), Color::Black().GetU32(), 1.5f);
			windowDrawList->AddLine(ImVec2(pos.x + 0   , pos.y + (siz.y / 2)), ImVec2(pos.x + siz.x, pos.y + (siz.y / 2)), Color::Black().GetU32(), 1.5f);

			// Rendering players

			if (Interface.Engine->IsInGame() && Interface.Engine->IsConnected())
			{
				CPlayer* pLocalEntity = Interface.EntityList->GetClientEntity<CPlayer>(Interface.Engine->GetLocalPlayer());
				if (pLocalEntity)
				{
					Vector3 LocalPos = pLocalEntity->GetEyePosition();
					Vector3 ang;
					Interface.Engine->GetViewAngles(ang);
					for (int i = 0; i < Interface.Engine->GetMaxClients(); i++) {
						CPlayer* pBaseEntity = Interface.EntityList->GetClientEntity<CPlayer>(i);

						if (!pBaseEntity)
							continue;
						if (!pBaseEntity->IsValid())
							continue;
						if (pBaseEntity == pLocalEntity)
							continue;

						bool bIsEnemy = pLocalEntity->GetTeam() != pBaseEntity->GetTeam();
						bool isVisibled = G::VisibledPlayers[i]; /*U::IsVisible(LocalPos, pBaseEntity->GetEyePosition(), pLocalEntity, pBaseEntity, Options::Radar::SmokeCheck);*/

						if (Options::Radar::EnemyOnly && !bIsEnemy)
							continue;

						bool viewCheck = false;
						Vector2 EntityPos = U::RotatePoint(pBaseEntity->GetOrigin(), LocalPos, pos.x, pos.y, siz.x, siz.y, ang.y, Options::Radar::Zoom, &viewCheck);

						if (!viewCheck && Options::Radar::ViewCheck)
							isVisibled = false;
						//ImU32 clr = (bIsEnemy ? (isVisibled ? Color::LightGreen() : Color::Blue()) : Color::White()).GetU32();
						ImU32 clr = (bIsEnemy ? (isVisibled ? Color::White() : Color::Black()) : Color::Lightblue()).GetU32();

						if (Options::Radar::VisibleOnly && !isVisibled)
							continue;

						int s = 4;
						switch (Options::Radar::Type) // 0 - Box; 1 - Filled box; 2 - Circle; 3 - Filled circle;
						{
						case 0:
						{
							windowDrawList->AddRect(ImVec2(EntityPos.x - s, EntityPos.y - s),
								ImVec2(EntityPos.x + s, EntityPos.y + s),
								clr);
							break;
						}
						case 1:
						{
							windowDrawList->AddRectFilled(ImVec2(EntityPos.x - s, EntityPos.y - s),
								ImVec2(EntityPos.x + s, EntityPos.y + s),
								clr);
							break;
						}
						case 2:
						{
							windowDrawList->AddCircle(ImVec2(EntityPos.x, EntityPos.y), s, clr);
							break;
						}
						case 3:
						{
							windowDrawList->AddCircleFilled(ImVec2(EntityPos.x, EntityPos.y), s, clr);
							break;
						}
						default:
							break;
						}
					}
				}
			}
		}
		ImGui::End();
		style.WindowPadding = oldPadding;
		style.Colors[ImGuiCol_WindowBg].w = oldAlpha;
	}

	void DrawSpectatorList()
	{
		int specs = 0;
		std::string spect = "";
		if (Interface.Engine->IsInGame() && Interface.Engine->IsConnected())
		{
			int localIndex = Interface.Engine->GetLocalPlayer();
			CPlayer* pLocalEntity = Interface.EntityList->GetClientEntity<CPlayer>(localIndex);
			if (pLocalEntity)
			{
				for (int i = 0; i < Interface.Engine->GetMaxClients(); i++) {
					CPlayer* pBaseEntity = Interface.EntityList->GetClientEntity<CPlayer>(i);
					if (!pBaseEntity)
						continue;
					if (pBaseEntity->GetHealth() > 0)
						continue;
					if (pBaseEntity == pLocalEntity)
						continue;
					if (pBaseEntity->GetDormant())
						continue;
					if (pBaseEntity->GetObserverTarget() != pLocalEntity)
						continue;

					player_info_t pInfo;
					Interface.Engine->GetPlayerInfo(pBaseEntity->GetIndex(), &pInfo);
					if (pInfo.ishltv)
						continue;

					spect += pInfo.name;
					spect += "\n";
					specs++;
				}
			}
		}

		if (ImGui::Begin(XorStr("Spectator List"), &G::MenuOpened, ImVec2(0, 0), 0.4F, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
		{
			if (specs > 0) spect += "\n";
			ImVec2 siz = ImGui::CalcTextSize(spect.c_str());
			ImGui::SetWindowSize(ImVec2(200, 20 + siz.y));
			ImGui::Text(spect.c_str());
		}
		ImGui::End();
	}

	void DrawAimbotTab()
	{
		ImVec2 lPos = ImGui::GetCursorPos();
		ImGui::Checkbox(XorStr("Enabled"), &Options::Aimbot::Enabled);
		ImGui::Checkbox(XorStr("Deathmatch"), &Options::Aimbot::Deathmatch);
		ImGui::Checkbox(XorStr("Smoke Check"), &Options::Aimbot::SmokeCheck);
		ImGui::Checkbox(XorStr("Draw Fov"), &Options::Aimbot::DrawFov);
		ImGui::Checkbox(XorStr("Auto Fire"), &Options::Aimbot::AutoFire);
		ImGui::Text(XorStr("Smooth type"));
		ImGui::Combo(XorStr(""), &Options::Aimbot::AimType, "Default\0Better\0Curve\0\0", -1, ImVec2(130, 0));
		ImGui::Checkbox(XorStr("Kill Delay"), &Options::Aimbot::KillDelay);
		ImGui::SliderFloat(XorStr("##0"), &Options::Aimbot::KillDelayTime, 0, 2, "%.2f", 1.0F, ImVec2(130, 0));

		int cw = U::SafeWeaponID();
		if (cw == 0)
			return;
		if (U::IsNonAimWeapon(cw))
			return;

		ImGui::SetCursorPos(ImVec2(160, lPos.y));
		ImGui::BeginChild(XorStr("AimChild"), ImVec2(0, 0), true);
		{
			ImGui::Checkbox(XorStr("Enabled"), &weapons[cw].Enabled);
			ImGui::Checkbox(XorStr("Nearest"), &weapons[cw].Nearest);
			if (weapons[cw].Nearest)
			{
				ImGui::Combo(XorStr("##1"), &weapons[cw].NearestType, "Basic\0Basic & Legs\0Max\0\0", -1);
			}
			else
				ImGui::InputInt(XorStr("Bone"), &weapons[cw].Bone);
			ImGui::SliderFloat(XorStr("FOV"), &weapons[cw].Fov, 0, 180);
			ImGui::SliderFloat(XorStr("Smooth"), &weapons[cw].Smooth, 0, 10);
			ImGui::NewLine();

			ImGui::Checkbox(XorStr("Fire delay"), &weapons[cw].FireDelayEnabled);
			ImGui::Checkbox(XorStr("Repeat"), &weapons[cw].FireDelayRepeat);
			ImGui::SliderFloat(XorStr("##2"), &weapons[cw].FireDelay, 0, 2, "%.4f");

			ImGui::NewLine();

			ImGui::SliderInt(XorStr("RCS X"), &weapons[cw].RcsX, 0, 200, "%.0f %");
			ImGui::SliderInt(XorStr("RCS Y"), &weapons[cw].RcsY, 0, 200, "%.0f %");

			ImGui::NewLine();

			ImGui::SliderInt(XorStr("Start bullet"), &weapons[cw].StartBullet, 0, 30);
			ImGui::SliderInt(XorStr("End bullet"), &weapons[cw].EndBullet, 0, 30);

			ImGui::NewLine();

			ImGui::Checkbox(XorStr("pSilent"), &weapons[cw].pSilent);
			ImGui::SliderInt(XorStr("Hitchance"), &weapons[cw].pSilentHitchance, 0, 100);
			ImGui::SliderInt(XorStr("Bullet"), &weapons[cw].pSilentBullet, 0, 30);
			ImGui::SliderFloat(XorStr("FOV##0"), &weapons[cw].pSilentFov, 0, 180);
			ImGui::SliderFloat(XorStr("Smooth##0"), &weapons[cw].pSilentSmooth, 0, 10);
		}
		ImGui::EndChild();
	}

	void DrawColorsTab()
	{
		static int selectedItem = 0;

		ImVec2 lastCursor = ImGui::GetCursorPos();

		ImGui::ListBoxHeader("##0", ImVec2(240, 520));
		for (int i = 0; i < G::ColorsForPicker.size(); i++)
		{
			bool selected = i == selectedItem;

			if (ImGui::Selectable(G::ColorsForPicker[i].Name, selected))
				selectedItem = i;
		}
		ImGui::ListBoxFooter();

		float nc = lastCursor.x + 260;
		ImGui::SetCursorPos(ImVec2(nc, lastCursor.y));

		ColorP col = G::ColorsForPicker[selectedItem];
		int r = (col.Ccolor[0] * 255.f);
		int g = (col.Ccolor[1] * 255.f);
		int b = (col.Ccolor[2] * 255.f);
		//int a = (*col.Ccolor[3] * 255.f);

		ImGui::NewLine(); ImGui::SetCursorPosX(nc);
		ImGui::SliderInt("##R", &r, 0, 255, "%.0f", ImVec4(0.40f, 0, 0, 1));
		ImGui::NewLine(); ImGui::SetCursorPosX(nc);
		ImGui::SliderInt("##G", &g, 0, 255, "%.0f", ImVec4(0, 0.40f, 0, 1));
		ImGui::NewLine(); ImGui::SetCursorPosX(nc);
		ImGui::SliderInt("##B", &b, 0, 255, "%.0f", ImVec4(0, 0, 0.40f, 1));
		//ImGui::NewLine(); ImGui::SetCursorPosX(nc);
		//ImGui::SliderInt("A", &a, 0, 255, "%.0f", ImVec4(0, 0, 0, 255));

		col.Ccolor[0] = r / 255.0f;
		col.Ccolor[1] = g / 255.0f;
		col.Ccolor[2] = b / 255.0f;
		//col.Ccolor[3] = a / 255.0f;

		ImGui::NewLine(); ImGui::SetCursorPosX(nc);
		ImVec2 curPos = ImGui::GetCursorPos();
		ImVec2 curWindowPos = ImGui::GetWindowPos();
		curPos.x += curWindowPos.x;
		curPos.y += curWindowPos.y;
		

		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(curPos.x + 255, curPos.y),
			ImVec2(curWindowPos.x + curWidth - 25, curPos.y + 200),
			ImGui::GetColorU32(ImVec4(col.Ccolor[0], col.Ccolor[1], col.Ccolor[2], 255)));

		ImGui::ColorPicker("##COLOR_PICKER", col.Ccolor);

		//ImGui::NewLine(); ImGui::NewLine(); ImGui::SetCursorPosX(nc);
	}

	void DrawVisualsTab()
	{
		ImGui::SameLine();
		ImGui::GetCurrentWindow()->DC.CursorPos.y += 6;
		int tabWidth = CalcTabWidth(4);

		/*if (DrawTab(XorStr("ESP"), tabWidth, tabHeight, visEspTab, false)) SelectVisualsSubTab(&visEspTab);
		if (DrawTab(XorStr("Chams"), tabWidth, tabHeight, visChamsTab, false)) SelectVisualsSubTab(&visChamsTab);
		if (DrawTab(XorStr("Hands"), tabWidth, tabHeight, visHandsTab, false)) SelectVisualsSubTab(&visHandsTab);
		if (DrawTab(XorStr("Misc##0"), tabWidth, tabHeight, visMiscTab, false)) SelectVisualsSubTab(&visMiscTab);*/
		ImGui::NewLine();

		ImVec2 siz = ImVec2(185, curHeight - ImGui::GetCursorPosY() - 40);
		ImVec2 csize = ImVec2(siz.x - 28, 0);
		ImVec2 asize = ImVec2(csize.x - 10, 0);

		if (ImGui::BeginChild(XorStr("##ESP_CHILD"), siz, true))
		{
			ImGui::Text(XorStr("ESP"));
			ImGui::Checkbox(XorStr("Enabled"), &Options::Visuals::ESP::Enabled);
			ImGui::Text(XorStr("Style"));
			ImGui::Combo(XorStr(""), &Options::Visuals::ESP::Style, "Default\0Default outlined\0Corner\0Corner outlined\0\r3D\0\r3D filled\0\r3D filled outline\0\0", -1, csize);
			ImGui::Checkbox(XorStr("Enemy only"), &Options::Visuals::ESP::EnemyOnly);
			ImGui::Checkbox(XorStr("Visible only"), &Options::Visuals::ESP::VisibleOnly);
			ImGui::Checkbox(XorStr("Smoke check"), &Options::Visuals::ESP::SmokeCheck);
			ImGui::Checkbox(XorStr("Box"), &Options::Visuals::ESP::Box);
			ImGui::Checkbox(XorStr("Health bar"), &Options::Visuals::ESP::Health);
			ImGui::Checkbox(XorStr("Name"), &Options::Visuals::ESP::Name);
			ImGui::Checkbox(XorStr("Weapon"), &Options::Visuals::ESP::Weapon);
			ImGui::Checkbox(XorStr("Ammo"), &Options::Visuals::ESP::WeaponAmmo);
		} ImGui::EndChild(); ImGui::SameLine();
		if (ImGui::BeginChild(XorStr("##CHAMS_CHILD"), siz, true))
		{
			ImGui::Text(XorStr("Chams"));
			ImGui::Checkbox(XorStr("Enabled"), &Options::Visuals::Chams::Enabled);
			ImGui::Checkbox(XorStr("Enemy only"), &Options::Visuals::Chams::EnemyOnly);
			ImGui::Checkbox(XorStr("Visible only"), &Options::Visuals::Chams::VisibleOnly);
			//ImGui::Checkbox(XorStr("Draw head"), &Options::Visuals::Chams::DrawHead);
			ImGui::Text(XorStr("Style"));
			ImGui::Combo(XorStr(""), &Options::Visuals::Chams::Style, "Default\0Flat\0Wallhack\0\0", -1, csize);
			ImGui::NewLine();
		} ImGui::EndChild(); ImGui::SameLine();
		if (ImGui::BeginChild(XorStr("##HANDS_CHILD"), siz, true))
		{
			ImGui::Text(XorStr("Hands"));
			ImGui::Checkbox(XorStr("Enabled"), &Options::Visuals::Hands::Enabled);
			ImGui::Text(XorStr("Style"));
			ImGui::Combo(XorStr(""), &Options::Visuals::Hands::Style, "Disabled\0Wireframe\0Chams\0Chams & Wireframe\0Rainbow\0Rainbow & Wireframe\0\0", -1, csize);
		} ImGui::EndChild(); ImGui::SameLine();
		if (ImGui::BeginChild(XorStr("##MISC_CHILD"), siz, true))
		{
			ImGui::Text(XorStr("Misc"));
			ImGui::Checkbox(XorStr("Drop ESP"), &Options::Visuals::Misc::DropESP);
			ImGui::Checkbox(XorStr("Bomb ESP"), &Options::Visuals::Misc::BombTimer);
			ImGui::Text(XorStr("Bomb ESP Style"));
			ImGui::Combo(XorStr("##0"), &Options::Visuals::Misc::BombTimerType, "World\0Screen\0Both\0\0", -1, csize);
			ImGui::Checkbox(XorStr("No Flash"), &Options::Misc::NoFlash);
			ImGui::SliderFloat(XorStr("##1"), &Options::Misc::NoFlashAlpha, 0, 255, "%.1f", 1.0F, asize);
			ImGui::Checkbox(XorStr("Fov Changer"), &Options::Visuals::Misc::FovChanger);
			ImGui::SliderFloat(XorStr("##2"), &Options::Visuals::Misc::FovChangerValue, 70, 160, "%.1f", 1.0F, asize);
			ImGui::Checkbox(XorStr("Viewmodel Changer"), &Options::Visuals::Misc::ViewmodelChanger);
			ImGui::SliderFloat(XorStr("##3"), &Options::Visuals::Misc::ViewmodelChangerValue, 70, 160, "%.1f", 1.0F, asize);
		} ImGui::EndChild(); ImGui::SameLine();
	}

	void DrawRadarTab()
	{
		ImGui::Checkbox(XorStr("Enabled"), &Options::Radar::Enabled);
		ImGui::Checkbox(XorStr("Enemy only"), &Options::Radar::EnemyOnly);
		ImGui::Checkbox(XorStr("Visible only"), &Options::Radar::VisibleOnly);
		ImGui::Checkbox(XorStr("Smoke check"), &Options::Radar::SmokeCheck);
		ImGui::Checkbox(XorStr("View check"), &Options::Radar::ViewCheck);
		ImGui::Text(XorStr("Alpha")); ImGui::SameLine();
		ImGui::SliderInt(XorStr("Alpha##0"), &Options::Radar::Alpha, 0, 255);
		ImGui::Text(XorStr("Zoom")); ImGui::SameLine();
		ImGui::SliderFloat(XorStr("Zoom##0"), &Options::Radar::Zoom, 0, 4);
		ImGui::Text(XorStr("Radar style"));
		ImGui::Combo(XorStr("##0"), &Options::Radar::Style, "External\0In-Game\0\0");
		ImGui::Text(XorStr("Draw style"));
		ImGui::Combo(XorStr("##1"), &Options::Radar::Type, "Box\0Filled box\0Circle\0Circle filled\0\0", -1);

		ImGui::SetCursorPosY(curHeight - 60);
		if (ImGui::Button(XorStr("Reset size"), ImVec2(120, 0))) G::NextResetRadar = true;
	}

	void DrawSkinchangerTab()
	{
		ImVec2 lPos = ImGui::GetCursorPos();
		if (ImGui::Checkbox(XorStr("Skin Changer##0"), &Options::SkinChanger::EnabledSkin)) U::FullUpdate();

		if (ImGui::Checkbox(XorStr("Knife Changer"), &Options::SkinChanger::EnabledKnife)) U::FullUpdate();
		if (Options::SkinChanger::EnabledKnife)
			if (ImGui::Combo(XorStr("##0"), &Options::SkinChanger::Knife, "Bayonet\0Flip\0Gut\0Karambit\0M9Bayonet\0Huntsman\0Falchion\0Bowie\0Butterfly\0Daggers\0\0", -1, ImVec2(130, 0)))
				U::FullUpdate();

		if (ImGui::Checkbox(XorStr("Glove Changer"), &Options::SkinChanger::EnabledGlove)) U::FullUpdate();
		if (Options::SkinChanger::EnabledGlove)
		{
			if (ImGui::Combo(XorStr("##1"), &Options::SkinChanger::Glove, "Bloodhound\0Sport\0Driver\0Wraps\0Moto\0Specialist\0\0", -1, ImVec2(130, 0)))
				U::FullUpdate();

			const char* gstr;
			if (Options::SkinChanger::Glove == 0)
			{
				gstr = "Charred\0\rSnakebite\0\rBronzed\0\rGuerilla\0\0";
			}
			else if (Options::SkinChanger::Glove == 1)
			{
				gstr = "Hedge Maze\0\rPandoras Box\0\rSuperconductor\0\rArid\0\0";
			}
			else if (Options::SkinChanger::Glove == 2)
			{
				gstr = "Lunar Weave\0\rConvoy\0\rCrimson Weave\0\rDiamondback\0\0";
			}
			else if (Options::SkinChanger::Glove == 3)
			{
				gstr = "Leather\0\rSpruce DDPAT\0\rSlaughter\0\rBadlands\0\0";
			}
			else if (Options::SkinChanger::Glove == 4)
			{
				gstr = "Eclipse\0\rSpearmint\0\rBoom!\0\rCool Mint\0\0";
			}
			else if (Options::SkinChanger::Glove == 5)
			{
				gstr = "Forest DDPAT\0\rCrimson Kimono\0\rEmerald Web\0\rFoundation\0\0";
			}
			else
				gstr = "";

			if (ImGui::Combo(XorStr("##2"), &Options::SkinChanger::GloveSkin, gstr, -1, ImVec2(130, 0)))
				U::FullUpdate();
		}



		//int cw = /*U::SafeWeaponID();*/ 7;
		int cw = U::SafeWeaponID();
		if (cw == 0)
			return;
		if (U::IsWeaponDefaultKnife(cw))
			return;

		ImGui::SetCursorPos(ImVec2(160, lPos.y));
		ImGui::BeginChild(XorStr("AimChild"), ImVec2(0, 0), true);
		{
			ImGui::Checkbox(XorStr("Enabled"), &weapons[cw].ChangerEnabled);

			std::string skinName = U::GetWeaponNameById(cw);
			if (skinName.compare("") == 0)
				ImGui::InputInt(XorStr("Skin"), &weapons[cw].ChangerSkin);
			else
			{
				std::string skinStr = "";
				int curItem = -1;
				int s = 0;
				for (auto skin : G::weaponSkins[skinName])
				{
					int pk = G::skinMap[skin].paintkit;
					if (pk == weapons[cw].ChangerSkin)
						curItem = s;

					skinStr += G::skinNames[G::skinMap[skin].tagName].c_str();
					skinStr.push_back('\0');
					s++;
				}
				skinStr.push_back('\0');
				if (ImGui::Combo(XorStr("Skin"), &curItem, skinStr.c_str()))
				{
					int pk = 0;
					int c = 0;
					for (auto skin : G::weaponSkins[skinName])
					{
						if (curItem == c)
						{
							pk = G::skinMap[skin].paintkit;
							break;
						}

						c++;
					}
					weapons[cw].ChangerSkin = pk;
					if (weapons[cw].ChangerEnabled) U::FullUpdate();
				}
			}
			ImGui::InputText(XorStr("Name"), weapons[cw].ChangerName, 32);
			ImGui::InputInt(XorStr("StatTrak"), &weapons[cw].ChangerStatTrak);
			ImGui::InputInt(XorStr("Seed"), &weapons[cw].ChangerSeed);
			ImGui::SliderFloat(XorStr("Wear"), &weapons[cw].ChangerWear, 0, 1);

			ImGui::NewLine();
			if (ImGui::Button(XorStr("Apply")))
				U::FullUpdate();
		}
		ImGui::EndChild();
	}

	void DrawMiscTab()
	{
		ImGui::Checkbox(XorStr("Reveal ranks"), &Options::Misc::RevealRanks);
		ImGui::Checkbox(XorStr("Recoil crosshair"), &Options::Misc::RecoilCrosshair);
		ImGui::Checkbox(XorStr("Sniper crosshair"), &Options::Misc::SniperCrosshair);
		ImGui::Checkbox(XorStr("Bunnyhop"), &Options::Misc::Bunnyhop);
		ImGui::Checkbox(XorStr("Spectator List"), &Options::Misc::SpectatorList);
		ImGui::Checkbox(XorStr("Hit info"), &Options::Misc::HitInfo);
		ImGui::Checkbox(XorStr("Knife Bot"), &Options::Misc::KnifeBot);
		if (Options::Misc::KnifeBot)
		{
			ImGui::SameLine(); ImGui::Checkbox(XorStr("Auto"), &Options::Misc::KnifeBotAuto);
			ImGui::SameLine(); ImGui::Checkbox(XorStr("360"), &Options::Misc::KnifeBot360);
		}
		ImGui::Checkbox(XorStr("Copyright"), &Options::Misc::Copyright);
		ImGui::SliderFloat(XorStr("Speed Control"), &Options::Misc::SpeedControl, 0, 1, "%.3f", 1.0f, ImVec2(300, 0));
		//ImGui::Checkbox(XorStr("Animated Clan Tag"), &Options::Misc::ClanTagAnim);
		//ImGui::Combo(XorStr("##0"), &Options::Misc::ClanTagAnimStyle, XorStr("Text\0Empty\0Alahu akbar\0Sex\0\0"), -1, ImVec2(300, 0));
		//if (Options::Misc::ClanTagAnimStyle == 0)
		//	ImGui::InputText(XorStr("##1"), Options::Misc::ClanTabAnimText, 32, ImVec2(300, 0));


		ImGui::SetCursorPosY(curHeight - 100);
		ImGui::Text(XorStr("Config:"));
		if (ImGui::Button(XorStr("Load"), ImVec2(120, 0))) Config::Read();
		if (ImGui::Button(XorStr("Save"), ImVec2(120, 0))) Config::Save();
	}

	void Render()
	{
		deltaTime = GetDeltaTime(); 

		ImGui::GetIO().MouseDrawCursor = G::MenuOpened;

		if (Options::Radar::Enabled && Options::Radar::Style == 0) DrawRadar();
		if (Options::Misc::SpectatorList) DrawSpectatorList();
		if (G::MenuOpened)
		{
			float deltaSize = 2 * deltaTime;
			if (curWidth < windowWidth)
				curWidth += min(windowWidth - curWidth, deltaSize);
			else if (curHeight < windowHeight)
				curHeight += min(windowHeight - curHeight, deltaSize);

			curX = (screenWidth / 2) - (curWidth / 2);
			curY = (screenHeight / 2) - (windowHeight / 2);

			int pX, pY;
			Interface.Input->GetCursorPosition(&pX, &pY);
			ImGuiIO& io = ImGui::GetIO();
			io.MousePos.x = (float)(pX);
			io.MousePos.y = (float)(pY);

			DrawTabs();
			if (curWidth >= windowWidth &&
				curHeight >= windowHeight)
			{
				if (aimbotTab) DrawAimbotTab();
				if (visualsTab) DrawVisualsTab();
				if (SkinchangerTab) DrawSkinchangerTab();
				if (radarTab) DrawRadarTab();
				if (colorsTab) DrawColorsTab();
				if (miscTab) DrawMiscTab();
			}

			ImGui::End();
		}
	}

	void OpenMenu()
	{
		static bool is_down = false;
		static bool is_clicked = false;
		static bool lastCursor = false;

		if (G::PressedKeys[VK_INSERT])
		{
			is_clicked = false;
			is_down = true;
		}
		else if (!G::PressedKeys[VK_INSERT] && is_down)
		{
			is_clicked = true;
			is_down = false;
		}
		else
		{
			is_clicked = false;
			is_down = false;
		}

		if (is_clicked)
		{
			curWidth = 40;
			curHeight = 40;
			curX = 0;
			curY = 0;

			G::MenuOpened = !G::MenuOpened;
			std::string msg = XorStr("cl_mouseenable ") + std::to_string(!G::MenuOpened);
			Interface.Engine->ExecuteConsoleCommand(msg.c_str());
		}
	}

	LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
		case WM_LBUTTONDOWN:
			G::PressedKeys[VK_LBUTTON] = true;
			break;
		case WM_LBUTTONUP:
			G::PressedKeys[VK_LBUTTON] = false;
			break;
		case WM_RBUTTONDOWN:
			G::PressedKeys[VK_RBUTTON] = true;
			break;
		case WM_RBUTTONUP:
			G::PressedKeys[VK_RBUTTON] = false;
			break;
		case WM_KEYDOWN:
			G::PressedKeys[wParam] = true;
			break;
		case WM_KEYUP:
			G::PressedKeys[wParam] = false;
			break;
		default: break;
		}

		OpenMenu();

		if (G::Inited && G::MenuOpened && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return true;

		return CallWindowProc(g_pOldWindowProc, hWnd, uMsg, wParam, lParam);
	}
};