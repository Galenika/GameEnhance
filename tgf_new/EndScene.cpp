#include "Cheat.h"
#include "Menu.h"
#include "KeyBind.h"

void GUI_Init( IDirect3DDevice9* pDevice );

EndSceneFn oEndScene;

namespace ImGui {
	IMGUI_API  bool TabLabels(int numTabs, const char** tabLabels, int& selectedIndex, const char** tabLabelTooltips, bool autoLayout, int *pOptionalHoveredIndex) {
		ImGuiStyle& style = ImGui::GetStyle();

		const ImVec2 itemSpacing = style.ItemSpacing;
		const ImVec4 color = style.Colors[ImGuiCol_Button];
		const ImVec4 colorActive = style.Colors[ImGuiCol_ButtonActive];
		const ImVec4 colorHover = style.Colors[ImGuiCol_ButtonHovered];
		style.ItemSpacing.x = 1;
		style.ItemSpacing.y = 1;

		if (numTabs>0 && (selectedIndex<0 || selectedIndex >= numTabs)) selectedIndex = 0;
		if (pOptionalHoveredIndex) *pOptionalHoveredIndex = -1;

		const float btnOffset = 2.f*style.FramePadding.x;
		const float sameLineOffset = 2.f*style.ItemSpacing.x;
		const float uniqueLineOffset = 2.f*style.WindowPadding.x;

		float windowWidth = 0.f, sumX = 0.f;
		if (autoLayout) windowWidth = ImGui::GetWindowWidth() - uniqueLineOffset;

		bool selection_changed = false;
		for (int i = 0; i < numTabs; i++)
		{
			if (i == selectedIndex)
			{
				style.Colors[ImGuiCol_Button] = colorActive;
				style.Colors[ImGuiCol_ButtonActive] = colorActive;
				style.Colors[ImGuiCol_ButtonHovered] = colorActive;
			}
			else
			{
				style.Colors[ImGuiCol_Button] = color;
				style.Colors[ImGuiCol_ButtonActive] = colorActive;
				style.Colors[ImGuiCol_ButtonHovered] = colorHover;
			}

			ImGui::PushID(i);

			if (!autoLayout) { if (i>0) ImGui::SameLine(); }
			else if (sumX > 0.f) {
				sumX += sameLineOffset;
				sumX += ImGui::CalcTextSize(tabLabels[i]).x + btnOffset;
				if (sumX>windowWidth) sumX = 0.f;
				else ImGui::SameLine();
			}

			if (ImGui::Button2(tabLabels[i], ImVec2(131.2, 20))) { selection_changed = (selectedIndex != i); selectedIndex = i; }
			if (autoLayout && sumX == 0.f) {
				sumX = ImGui::GetItemRectSize().x;
			}
			if (pOptionalHoveredIndex) {
				if (ImGui::IsItemHovered()) {
					*pOptionalHoveredIndex = i;
					if (tabLabelTooltips && tabLabelTooltips[i] && strlen(tabLabelTooltips[i])>0)  ImGui::SetTooltip("%s", tabLabelTooltips[i]);
				}
			}
			else if (tabLabelTooltips && tabLabelTooltips[i] && ImGui::IsItemHovered() && strlen(tabLabelTooltips[i])>0) ImGui::SetTooltip("%s", tabLabelTooltips[i]);
			ImGui::PopID();
		}

		style.Colors[ImGuiCol_Button] = color;
		style.Colors[ImGuiCol_ButtonActive] = colorActive;
		style.Colors[ImGuiCol_ButtonHovered] = colorHover;
		style.ItemSpacing = itemSpacing;

		return selection_changed;
	}
}

void ImGui::ShowStyleEditor(ImGuiStyle* ref)
{
	ImGuiStyle& style = ImGui::GetStyle();

	// You can pass in a reference ImGuiStyle structure to compare to, revert to and save to (else it compares to the default style)
	const ImGuiStyle default_style; // Default style
	if (ImGui::Button("Reset Style"))
		style = ref ? *ref : default_style;

	if (ref)
	{
		ImGui::SameLine();
		if (ImGui::Button("Save Style"))
			*ref = style;
	}

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.55f);



	static int output_dest = 0;
	static bool output_only_modified = false;

	//ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::Combo("##output_type", &output_dest, "To Clipboard\0To TTY\0"); ImGui::PopItemWidth();
	//ImGui::SameLine(); ImGui::Checkbox("Only Modified Fields", &output_only_modified);

	static ImGuiColorEditMode edit_mode = ImGuiColorEditMode_RGB;
	ImGui::RadioButton("RGB", &edit_mode, ImGuiColorEditMode_RGB);
	ImGui::SameLine();
	ImGui::RadioButton("HSV", &edit_mode, ImGuiColorEditMode_HSV);
	ImGui::SameLine();
	ImGui::RadioButton("HEX", &edit_mode, ImGuiColorEditMode_HEX);
	//ImGui::Text("Tip: Click on colored square to change edit mode.");

	static ImGuiTextFilter filter;
	// filter.Draw("Filter colors", 200);

	ImGui::BeginChild("#colors", ImVec2(0, 300), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
	ImGui::PushItemWidth(-160);
	ImGui::ColorEditMode(edit_mode);
	for (int i = 0; i < ImGuiCol_COUNT; i++)
	{
		const char* name = ImGui::GetStyleColName(i);
		if (!filter.PassFilter(name))
			continue;
		ImGui::PushID(i);
		ImGui::ColorEdit4(name, (float*)&style.Colors[i], true);
		if (memcmp(&style.Colors[i], (ref ? &ref->Colors[i] : &default_style.Colors[i]), sizeof(ImVec4)) != 0)
		{
			ImGui::SameLine(); if (ImGui::Button("Revert")) style.Colors[i] = ref ? ref->Colors[i] : default_style.Colors[i];
			if (ref) { ImGui::SameLine(); if (ImGui::Button("Save")) ref->Colors[i] = style.Colors[i]; }
		}
		ImGui::PopID();
	}
	ImGui::PopItemWidth();
	ImGui::EndChild();

	ImGui::TreePop();

	ImGui::PopItemWidth();
}

void DoUpdate()
{
	I::Engine->ClientCmd_Unrestricted("record x;stop", 0);
}

long __stdcall Hooks::EndScene( IDirect3DDevice9* pDevice )
{
	if( !G::d3dinit )
		GUI_Init( pDevice );

	H::D3D9->ReHook();

	ImGui::GetIO().MouseDrawCursor = Vars.Menu.Opened;

	ImGui_ImplDX9_NewFrame();

	
	Vars.Menu.LoadFaceit = true;
	Vars.Menu.LoadFaceit = false;
	if( Vars.Menu.Opened )
	{
		int pX, pY;
		I::InputSystem->GetCursorPosition( &pX, &pY );
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos.x = ( float )( pX );
		io.MousePos.y = ( float )( pY );
		ImVec2 Pos;

		ImGui::Begin( charenc( "nice paste" ), &Vars.Menu.Opened, ImVec2( 800, 600 ), 0.9f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse );
		{
			Pos = ImGui::GetWindowPos();
			static const char* tabNames[] = { "Legit" , "Rage" , "Trigger" ,"Misc", "Visuals", "Skins" };
			static const int numTabs = sizeof(tabNames) / sizeof(tabNames[0]);
			static const char* tabTooltips[numTabs] = {};
			static int selectedTab = 2;
			static int optionalHoveredTab = 3;
			ImGui::TabLabels(numTabs, tabNames, selectedTab, tabTooltips, true, &optionalHoveredTab);
			switch (selectedTab)
			{
			case 0:
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Checkbox(charenc("Enable Aimbot"), &Vars.Legitbot.Aimbot.Enabled);
				ImGui::Checkbox(charenc("Always On"), &Vars.Legitbot.Aimbot.AlwaysOn);
				ImGui::Combo(charenc("Aimbot Key"), &Vars.Legitbot.Aimbot.Key, keyNames, ARRAYSIZE(keyNames));
				ImGui::Checkbox(charenc("Aim on Key"), &Vars.Legitbot.Aimbot.OnKey);
				ImGui::Checkbox(charenc("Friendly Fire"), &Vars.Legitbot.Aimbot.FriendlyFire);
				ImGui::Combo(charenc("Hitbox"), &Vars.Legitbot.Aimbot.Hitbox, aimBones, ARRAYSIZE(aimBones));
				ImGui::SliderFloat(charenc("FOV"), &Vars.Legitbot.Aimbot.FOV, 0.1f, 45.f, "%.2f");
				ImGui::SliderFloat(charenc("Aim Speed"), &Vars.Legitbot.Aimbot.Speed, 0.1f, 50.f, "%.2f");
				ImGui::SliderFloat(charenc("RCS Amount"), &Vars.Legitbot.Aimbot.RCSAmount, 1.f, 100.f, "%.0f");
				ImGui::Checkbox(charenc("Always on RCS"), &Vars.Legitbot.Aimbot.RCS);
				break;
			case 1:
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Checkbox(charenc("Enabled"), &Vars.Ragebot.Enabled);
				ImGui::Combo(charenc("Hold Key"), &Vars.Ragebot.HoldKey, keyNames, ARRAYSIZE(keyNames));
				ImGui::Checkbox(charenc("Hold"), &Vars.Ragebot.Hold);
				ImGui::Checkbox(charenc("Auto Fire"), &Vars.Ragebot.AutoFire);
				ImGui::SliderFloat(charenc("FOV"), &Vars.Ragebot.FOV, 1.f, 180.f, "%.0f");
				ImGui::Checkbox(charenc("Silent Aim"), &Vars.Ragebot.Silent);
				ImGui::Checkbox(charenc("Aim Step"), &Vars.Ragebot.Aimstep);
				ImGui::SliderFloat(charenc("Angle Per Tick"), &Vars.Ragebot.AimstepAmount, 1.f, 180.f, "%.0f");
				ImGui::Combo(charenc("Mode"), &Vars.Ragebot.TargetMethod, targetMode, ARRAYSIZE(targetMode));
				ImGui::Combo(charenc("Hitbox"), &Vars.Ragebot.Hitbox, aimBones, ARRAYSIZE(aimBones));
				ImGui::Checkbox(charenc("Friendly Fire"), &Vars.Ragebot.FriendlyFire);
				ImGui::Checkbox(charenc("Auto Wall"), &Vars.Ragebot.AutoWall);
				ImGui::SliderFloat(charenc("Min Damage"), &Vars.Ragebot.AutoWallDmg, 0.1f, 120.f, "%.1f");
				ImGui::Checkbox(charenc("Hit Scan Hitbox"), &Vars.Ragebot.HitScanHitbox);
				ImGui::Checkbox(charenc("Hit Scan All"), &Vars.Ragebot.HitScanAll);
				ImGui::Checkbox(charenc("Auto Stop"), &Vars.Ragebot.AutoStop);
				ImGui::Checkbox(charenc("Auto Crouch"), &Vars.Ragebot.AutoCrouch);
				ImGui::Checkbox(charenc("Hit Chance"), &Vars.Ragebot.HitChance);
				ImGui::SliderFloat(charenc("Amount"), &Vars.Ragebot.HitChanceAmt, 1.f, 100.f, "%.1f");

				ImGui::Checkbox(charenc("Anti Aimbot"), &Vars.Ragebot.Antiaim.Enabled);
				ImGui::Combo(charenc("Pitch"), &Vars.Ragebot.Antiaim.Pitch, antiaimpitch, ARRAYSIZE(antiaimpitch));
				ImGui::Combo(charenc("Yaw"), &Vars.Ragebot.Antiaim.Yaw, antiaimyaw, ARRAYSIZE(antiaimyaw));
				ImGui::Checkbox(charenc("At Players"), &Vars.Ragebot.Antiaim.AimAtPlayer);
				ImGui::Checkbox(charenc("Perfect Silent"), &Vars.Ragebot.Antiaim.PSilent);
				ImGui::Checkbox(charenc("Anti Untrusted"), &Vars.Ragebot.UntrustedCheck);
				break;
			case 2:
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Checkbox(charenc("Enable Triggerbot"), &Vars.Legitbot.Triggerbot.Enabled);
				ImGui::Checkbox(charenc("Triggerbot Auto Fire"), &Vars.Legitbot.Triggerbot.AutoFire);
				ImGui::Combo(charenc("Triggerbot Key"), &Vars.Legitbot.Triggerbot.Key, keyNames, ARRAYSIZE(keyNames));
				ImGui::Checkbox(charenc("Triggerbot Hit Chance"), &Vars.Legitbot.Triggerbot.HitChance);
				ImGui::SliderFloat(charenc("Hit Chance Amount"), &Vars.Legitbot.Triggerbot.HitChanceAmt, 1.f, 100.f, "%.0f");
				ImGui::Checkbox(charenc("Triggerbot Dellay"), &Vars.Legitbot.Triggerbot.Dellay);
				ImGui::SliderFloat(charenc("Dellay Amount"), &Vars.Legitbot.Triggerbot.DellayAmt, 1.f, 1000.f, "%.0f");

				ImGui::Checkbox(charenc("Triggerbot Filter Head"), &Vars.Legitbot.Triggerbot.Filter.Head);
				ImGui::Checkbox(charenc("Triggerbot Filter Chest"), &Vars.Legitbot.Triggerbot.Filter.Chest);
				ImGui::Checkbox(charenc("Triggerbot Filter Stomach"), &Vars.Legitbot.Triggerbot.Filter.Stomach);
				ImGui::Checkbox(charenc("Triggerbot Filter Arms"), &Vars.Legitbot.Triggerbot.Filter.Arms);
				ImGui::Checkbox(charenc("Triggerbot Filter Legs"), &Vars.Legitbot.Triggerbot.Filter.Legs);
				ImGui::Checkbox(charenc("Triggerbot Filter Friendlies"), &Vars.Legitbot.Triggerbot.Filter.Friendly);
				break;
			case 3:
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Checkbox(charenc("BunnyHop"), &Vars.Misc.Bhop);
				ImGui::Checkbox(charenc("AutoStrafe"), &Vars.Misc.AutoStrafe);
				ImGui::Checkbox(charenc("NameSpam"), &Vars.Misc.Namespam);
				ImGui::Checkbox(charenc("ChatSpam"), &Vars.Misc.ChatSpam);
				ImGui::Checkbox(charenc("RanksRevival"), &Vars.Misc.Ranks);
				ImGui::Checkbox(charenc("AutoAccept"), &Vars.Misc.AutoAccept);
				ImGui::Checkbox(charenc("WaterMark"), &Vars.Misc.Watermark);
				ImGui::Checkbox(charenc("AirStuck"), &Vars.Misc.AirStuck);
				ImGui::Combo(charenc("AirStuck Key"), &Vars.Misc.AirStuckKey, keyNames, ARRAYSIZE(keyNames));

				ImGui::Combo(charenc("Config Index"), &Vars.Menu.ConfigIndex, configindex, ARRAYSIZE(configindex));
				if (ImGui::Button(charenc("Save Config"), ImVec2(93.f, 20.f))) Config->DoSave();
				if (ImGui::Button(charenc("Load Config"), ImVec2(93.f, 20.f))) Config->DoLoad();
				break;
			case 4:
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Text(charenc("Visuals"));
				ImGui::Checkbox(charenc("Enable Visuals"), &Vars.Visuals.Enabled);
				ImGui::Combo(charenc("Box"), &Vars.Visuals.Box, boxmode, ARRAYSIZE(boxmode));
				ImGui::Checkbox(charenc("Skeleton"), &Vars.Visuals.Skeleton);
				ImGui::Checkbox(charenc("Glow"), &Vars.Visuals.Glow);
				ImGui::Checkbox(charenc("Aim Direction"), &Vars.Visuals.BulletTrace);
				ImGui::SliderFloat(charenc("Direction Length"), &Vars.Visuals.BulletTraceLength, 1.f, 3000.f, "%.0f");
				ImGui::Checkbox(charenc("Show Name"), &Vars.Visuals.Info.Name);
				ImGui::Checkbox(charenc("Show Health"), &Vars.Visuals.Info.Health);
				ImGui::Checkbox(charenc("Show Weapon"), &Vars.Visuals.Info.Weapon);
				ImGui::Checkbox(charenc("Show Flashed Players"), &Vars.Visuals.Info.Flashed);
				ImGui::Checkbox(charenc("Show Enemies"), &Vars.Visuals.Filter.Enemies);
				ImGui::Checkbox(charenc("Show Friendlies"), &Vars.Visuals.Filter.Friendlies);
				ImGui::Checkbox(charenc("Show Weapons"), &Vars.Visuals.Filter.Weapons);
				ImGui::Checkbox(charenc("Show Decoy"), &Vars.Visuals.Filter.Decoy);
				ImGui::Checkbox(charenc("Show C4"), &Vars.Visuals.Filter.C4);

				ImGui::Separator();
				ImGui::Checkbox(charenc("ASUS Wallhack"), &Vars.Visuals.ASUS);
				ImGui::SliderFloat(charenc("ASUS Factor"), &Vars.Visuals.ASUSFactor, 0.0f, 1.f, "%.2f");
				ImGui::Separator();

				ImGui::Checkbox(charenc("Chams Wallhack"), &Vars.Visuals.Chams.XQZ);
				ImGui::Checkbox(charenc("Enable Chams"), &Vars.Visuals.Chams.Enabled);
				ImGui::Combo(charenc("Chams Mode"), &Vars.Visuals.Chams.Mode, chamsMode, ARRAYSIZE(chamsMode));
				ImGui::Checkbox(charenc("Chams Wireframe"), &Vars.Visuals.Chams.Wireframe);

				ImGui::SliderFloat(charenc("Chams Enemy Red"), &Vars.Visuals.Chams.EnemyColor[1], 0.0f, 255.f, "%.2f");
				ImGui::SliderFloat(charenc("Chams Enemy Green"), &Vars.Visuals.Chams.EnemyColor[2], 0.0f, 255.f, "%.2f");
				ImGui::SliderFloat(charenc("Chams Enemy Blue"), &Vars.Visuals.Chams.EnemyColor[3], 0.0f, 255.f, "%.2f");

				ImGui::SliderFloat(charenc("Chams Team Red"), &Vars.Visuals.Chams.TeamColor[1], 0.0f, 255.f, "%.2f");
				ImGui::SliderFloat(charenc("Chams Team Green"), &Vars.Visuals.Chams.TeamColor[2], 0.0f, 255.f, "%.2f");
				ImGui::SliderFloat(charenc("Chams Team Blue"), &Vars.Visuals.Chams.TeamColor[3], 0.0f, 255.f, "%.2f");

				ImGui::SliderFloat(charenc("Chams Enemy Invisible Red"), &Vars.Visuals.Chams.EnemyColorWall[1], 0.0f, 255.f, "%.2f");
				ImGui::SliderFloat(charenc("Chams Enemy Invisible Green"), &Vars.Visuals.Chams.EnemyColorWall[2], 0.0f, 255.f, "%.2f");
				ImGui::SliderFloat(charenc("Chams Enemy Invisible Blue"), &Vars.Visuals.Chams.EnemyColorWall[3], 0.0f, 255.f, "%.2f");

				ImGui::SliderFloat(charenc("Chams Team Invisible Red"), &Vars.Visuals.Chams.TeamColorWall[1], 0.0f, 255.f, "%.2f");
				ImGui::SliderFloat(charenc("Chams Team Invisible Green"), &Vars.Visuals.Chams.TeamColorWall[2], 0.0f, 255.f, "%.2f");
				ImGui::SliderFloat(charenc("Chams Team Invisible Blue"), &Vars.Visuals.Chams.TeamColorWall[3], 0.0f, 255.f, "%.2f");

				ImGui::Combo(charenc("Hands"), &Vars.Visuals.Removals.HandsMode, handsmode, ARRAYSIZE(handsmode));
				ImGui::Checkbox(charenc("No Weapon"), &Vars.Visuals.Removals.Weapon);
				ImGui::Checkbox(charenc("No Visual Recoil"), &Vars.Visuals.Removals.VisualRecoil);
				ImGui::Checkbox(charenc("No Flash"), &Vars.Visuals.Removals.Flash);

				ImGui::Checkbox(charenc("Crosshair"), &Vars.Visuals.CrosshairOn);
				ImGui::Combo(charenc("Type"), &Vars.Visuals.CrosshairType, crosshairType, ARRAYSIZE(crosshairType));
				ImGui::Combo(charenc("Style"), &Vars.Visuals.CrosshairStyle, crosshairStyle, ARRAYSIZE(crosshairStyle));

				ImGui::Checkbox(charenc("Color Manager"), &Vars.Menu.Colors);
				break;
			case 5:
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Checkbox(charenc("Enable Skin Changer"), &Vars.Misc.SkinsEnabled);

				if (ImGui::Button(charenc("Update Skin Changer"), ImVec2(193.f, 20.f))) DoUpdate();

				ImGui::Text(charenc("Rifles"));

				ImGui::Separator();

				ImGui::Combo(charenc("AK-47"), &Vars.Misc.SkinAK, aklist, ARRAYSIZE(aklist));

				ImGui::Combo(charenc("M4A4"), &Vars.Misc.SkinM4A4, m4a1list, ARRAYSIZE(m4a1list));

				ImGui::Combo(charenc("M4A1-S"), &Vars.Misc.SkinM4A1S, m4a1slist, ARRAYSIZE(m4a1slist));

				ImGui::Combo(charenc("Famas"), &Vars.Misc.SkinFamas, umplist, ARRAYSIZE(umplist));

				ImGui::Combo(charenc("Galil"), &Vars.Misc.SkinGalil, galilarlist, ARRAYSIZE(galilarlist));

				ImGui::Combo(charenc("SSG08"), &Vars.Misc.SkinSSG08, ssg08list, ARRAYSIZE(ssg08list));

				ImGui::Combo(charenc("Aug"), &Vars.Misc.SkinAug, auglist, ARRAYSIZE(auglist));

				ImGui::Combo(charenc("SG556"), &Vars.Misc.SkinSG556, sg556list, ARRAYSIZE(sg556list));

				ImGui::Combo(charenc("AWP"), &Vars.Misc.SkinAWP, awplist, ARRAYSIZE(awplist));

				ImGui::Combo(charenc("G3SG1"), &Vars.Misc.SkinG3SG1, g3sg1list, ARRAYSIZE(g3sg1list));

				ImGui::Combo(charenc("Scar-20"), &Vars.Misc.SkinScar20, scar20list, ARRAYSIZE(scar20list));

				ImGui::Text(charenc("Pistols"));

				ImGui::Separator();

				ImGui::Combo(charenc("Glock-18"), &Vars.Misc.SkinGlock, glocklist, ARRAYSIZE(glocklist));

				ImGui::Combo(charenc("USP-S"), &Vars.Misc.SkinUsp, uspslist, ARRAYSIZE(uspslist));

				ImGui::Combo(charenc("P2000"), &Vars.Misc.Skinp2000, hkp2000list, ARRAYSIZE(hkp2000list));

				ImGui::Combo(charenc("P250"), &Vars.Misc.Skinp250, P250list, ARRAYSIZE(P250list));

				ImGui::Combo(charenc("Dual Beretas"), &Vars.Misc.SkinElite, elitelist, ARRAYSIZE(elitelist));

				ImGui::Combo(charenc("Tec-9"), &Vars.Misc.SkinTec9, P250list, ARRAYSIZE(P250list));

				ImGui::Combo(charenc("FiveSeven"), &Vars.Misc.Skinf7, f7list, ARRAYSIZE(f7list));

				ImGui::Combo(charenc("CZ-75"), &Vars.Misc.SkinCZ, CZ75Alist, ARRAYSIZE(CZ75Alist));

				ImGui::Combo(charenc("Deagle"), &Vars.Misc.SkinDeagle, deaglelist, ARRAYSIZE(deaglelist));

				ImGui::Text(charenc("Shotguns"));

				ImGui::Separator();

				ImGui::Combo(charenc("Nova"), &Vars.Misc.SkinNova, novalist, ARRAYSIZE(novalist));

				ImGui::Combo(charenc("Sawed-Off"), &Vars.Misc.SkinSawedOff, sawedofflist, ARRAYSIZE(sawedofflist));

				ImGui::Combo(charenc("Mag-7"), &Vars.Misc.SkinMAG7, mag7list, ARRAYSIZE(mag7list));

				ImGui::Combo(charenc("XM1014"), &Vars.Misc.SkinXM, xm1014list, ARRAYSIZE(xm1014list));

				ImGui::Text(charenc("Heavy"));

				ImGui::Separator();

				ImGui::Combo(charenc("Negev"), &Vars.Misc.SkinNegev, negevlist, ARRAYSIZE(negevlist));

				ImGui::Combo(charenc("M249"), &Vars.Misc.Skin249, m249list, ARRAYSIZE(m249list));

				ImGui::Text(charenc("MPs"));

				ImGui::Separator();

				ImGui::Combo(charenc("MAC-10"), &Vars.Misc.SkinMac10, mac10list, ARRAYSIZE(mac10list));

				ImGui::Combo(charenc("MP-7"), &Vars.Misc.SkinMP7, mp7list, ARRAYSIZE(mp7list));

				ImGui::Combo(charenc("MP-9"), &Vars.Misc.SkinMp9, mp9list, ARRAYSIZE(mp9list));

				ImGui::Combo(charenc("UMP-45"), &Vars.Misc.SkinUMP, umplist, ARRAYSIZE(umplist));

				ImGui::Combo(charenc("P90"), &Vars.Misc.SkinP90, p90list, ARRAYSIZE(p90list));

				ImGui::Combo(charenc("PP-Bizon"), &Vars.Misc.SkinBizon, bizonlist, ARRAYSIZE(bizonlist));

				ImGui::Text(charenc("Knife"));

				ImGui::Separator();

				ImGui::Combo(charenc("Knife"), &Vars.Misc.KnifeModel, knifelist, ARRAYSIZE(knifelist));
				ImGui::Combo(charenc("Bayonet"), &Vars.Misc.BayonetSkin, bayonetskin, ARRAYSIZE(bayonetskin));


				break;
			}

		}
		ImGui::End(); //End main window
	}

	if (Vars.Menu.Colors)
	{
		ImGui::ShowStyleEditor();
	}

	ImGui::Render();

	return oEndScene( pDevice );
}

ResetFn oReset;
long __stdcall Hooks::Reset( IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters )
{
	if( !G::d3dinit )
		return oReset( pDevice, pPresentationParameters );

	ImGui_ImplDX9_InvalidateDeviceObjects();

	auto hr = oReset( pDevice, pPresentationParameters );
	ImGui_ImplDX9_CreateDeviceObjects();


	return hr;
}

void GUI_Init( IDirect3DDevice9* pDevice ) 
{
	ImGui_ImplDX9_Init( G::Window, pDevice );
	ImGuiStyle& style = ImGui::GetStyle();

	/*
	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.22f, 0.78f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.10f, 0.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(1.00f, 0.78f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.16f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 0.78f, 0.00f, 0.73f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.85f, 0.61f, 0.00f, 0.81f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(1.00f, 0.61f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.61f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.00f, 0.61f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.00f, 0.78f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.01f, 0.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.00f, 0.79f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 0.71f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 0.63f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.79f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.79f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.63f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.49f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.40f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(1.00f, 0.53f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.40f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 0.32f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(1.00f, 0.40f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(1.00f, 0.52f, 0.04f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(1.00f, 0.52f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	style.WindowPadding = ImVec2(4, 4);
	style.WindowRounding = 2.0f;
	style.ChildWindowRounding = 2.0f;
	style.FramePadding = ImVec2(7, 4);
	style.FrameRounding = 3.0f;
	style.ItemSpacing = ImVec2(20, 5);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 0.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 1.0f;
	style.GrabMinSize = 18.0f;
	style.GrabRounding = 1.0f;
	*/

	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.412f, 0.412f, 0.412f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.10f, 0.00f);
	// style.Colors[ImGuiCol_PopupBg] = ImVec4(0.00f, 0.00f, 0.10f, 1.00f); 
	style.Colors[ImGuiCol_Border] = ImVec4(0.502f, 0.502f, 0.502f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.000f, 0.000f, 0.545f, 0.16f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 0.78f, 0.00f, 0.73f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.85f, 0.61f, 0.00f, 0.81f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.502f, 0.502f, 0.502f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.502f, 0.502f, 0.502f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.502f, 0.502f, 0.502f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.412f, 0.412f, 0.412f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.502f, 0.502f, 0.502f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.098f, 0.098f, 0.439f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.502f, 0.502f, 0.502f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.412f, 0.412f, 0.412f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.79f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.412f, 0.412f, 0.412f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.863f, 0.863f, 0.863f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.000f, 0.000f, 0.000f, 0.502f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.663f, 0.663f, 0.663f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.502f, 0.502f, 0.502f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.502f, 0.502f, 0.502f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.502f, 0.502f, 0.502f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.502f, 0.502f, 0.502f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.663f, 0.663f, 0.663f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.502f, 0.502f, 0.502f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.412f, 0.412f, 0.412f, 1.00);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.502f, 0.502f, 0.502f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.663f, 0.663f, 0.663f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.412f, 0.412f, 0.412f, 1.00);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.502f, 0.502f, 0.502f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.663f, 0.663f, 0.663f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.827f, 0.827f, 0.827f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.863f, 0.863f, 0.863f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.753f, 0.753f, 0.753f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.412, 0.412f, 0.412f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.545, 0.000f, 0.000f, 1.00f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.184, 0.310f, 0.310f, 1.00f);

	style.WindowPadding = ImVec2(4, 4);
	style.WindowRounding = 2.0f;
	style.ChildWindowRounding = 2.0f;
	style.FramePadding = ImVec2(7, 4);
	style.FrameRounding = 3.0f;
	style.ItemSpacing = ImVec2(20, 5);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 0.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 1.0f;
	style.GrabMinSize = 18.0f;
	style.GrabRounding = 1.0f;

	style.AntiAliasedLines = false;
	style.AntiAliasedShapes = false;

	G::d3dinit = true;
}
