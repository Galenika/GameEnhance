#include "Menu.h"
#include "Controls.h"
#include "Hooks.h" // For the unloading pepe
#include "Interfaces.h"
#include "CRC32.h"

#define WINDOW_WIDTH 800 // Menu Width
#define WINDOW_HEIGHT 900 // Menu Height

AyyWareWindow Menu::Window;

void SaveCallbk() // Saving Config to CSGO directory
{
	GUI.SaveWindowState(&Menu::Window, "config.xml");
}

void LoadCallbk() // Loading Config from CSGO directory
{
	GUI.LoadWindowState(&Menu::Window, "config.xml");
}

void UnLoadCallbk() // Unload Button Function
{
	DoUnload = true;
}

void KnifeApplyCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	Interfaces::Engine->ClientCmd_Unrestricted("cl_fullupdate");
}

void AyyWareWindow::Setup()
{
	SetPosition(50, 50);
	SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetTitle("WarmHook - 2 Steps ahead of the game");

	RegisterTab(&RageBotTab);
	RegisterTab(&LegitBotTab);
	RegisterTab(&VisualsTab);
	RegisterTab(&MiscTab);
	RegisterTab(&SkinTab);
	RegisterTab(&GUITab);
	//RegisterTab(&Playerlist);

	RECT Client = GetClientArea();
	Client.bottom -= 29;

	RageBotTab.Setup();
	LegitBotTab.Setup();
	VisualsTab.Setup();
	MiscTab.Setup();
	SkinTab.Setup();
	GUITab.Setup();
	//Playerlist.Setup();

#pragma region Bottom Buttons
	SaveButton.SetText("Save Configuration");
	SaveButton.SetCallback(SaveCallbk);
	SaveButton.SetPosition(16, Client.bottom - 42);

	LoadButton.SetText("Load Configuration");
	LoadButton.SetCallback(LoadCallbk);
	LoadButton.SetPosition(203, Client.bottom - 42);
	
	UnloadButton.SetText("Complete Unload");
	UnloadButton.SetCallback(UnLoadCallbk);
	UnloadButton.SetPosition(396, Client.bottom - 42);

	RageBotTab.RegisterControl(&SaveButton);
	LegitBotTab.RegisterControl(&SaveButton);
	VisualsTab.RegisterControl(&SaveButton);
	MiscTab.RegisterControl(&SaveButton);
	SkinTab.RegisterControl(&SaveButton);
	GUITab.RegisterControl(&SaveButton);
	//Playerlist.RegisterControl(&SaveButton);

	RageBotTab.RegisterControl(&LoadButton);
	LegitBotTab.RegisterControl(&LoadButton);
	VisualsTab.RegisterControl(&LoadButton);
	MiscTab.RegisterControl(&LoadButton);
	SkinTab.RegisterControl(&LoadButton);
	GUITab.RegisterControl(&LoadButton);
	//Playerlist.RegisterControl(&LoadButton);

	RageBotTab.RegisterControl(&UnloadButton);
	LegitBotTab.RegisterControl(&UnloadButton);
	VisualsTab.RegisterControl(&UnloadButton);
	MiscTab.RegisterControl(&UnloadButton);
	SkinTab.RegisterControl(&UnloadButton);
	GUITab.RegisterControl(&UnloadButton);
	//Playerlist.RegisterControl(&UnloadButton);

#pragma endregion Setting up the settings buttons
}

void CRageBotTab::Setup()
{
	SetTitle("Rage");

	ActiveLabel.SetPosition(16, 16);
	ActiveLabel.SetText("Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(66, 16);
	RegisterControl(&Active);

#pragma region Aimbot

	AimbotGroup.SetPosition(16, 48);
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetSize(376, 300);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl("Enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl("Auto Fire", this, &AimbotAutoFire);

	AimbotFov.SetFileId("aim_fov");
	AimbotFov.SetBoundaries(0.f, 180.f);
	AimbotFov.SetValue(39.f);
	AimbotGroup.PlaceLabledControl("FOV Range", this, &AimbotFov);

	AimbotSilentAim.SetFileId("aim_silent");
	AimbotGroup.PlaceLabledControl("Silent Aim", this, &AimbotSilentAim);

	AimbotPerfectSilentAim.SetFileId("aim_psilent");
	AimbotGroup.PlaceLabledControl("Perfect Silent", this, &AimbotPerfectSilentAim);

	AimbotAutoPistol.SetFileId("aim_autopistol");
	AimbotGroup.PlaceLabledControl("Auto Pistol", this, &AimbotAutoPistol);

	AimbotAutoRevolver.SetFileId("aim_autorevolver");
	AimbotGroup.PlaceLabledControl("Auto Revolver", this, &AimbotAutoRevolver);

	AimbotAimStep.SetFileId("aim_aimstep");
	AimbotGroup.PlaceLabledControl("Aim Step", this, &AimbotAimStep);

	AimbotKeyPress.SetFileId("aim_usekey");
	AimbotGroup.PlaceLabledControl("On Key Press", this, &AimbotKeyPress);

	AimbotKeyBind.SetFileId("aim_key");
	AimbotGroup.PlaceLabledControl("Key", this, &AimbotKeyBind);

	AimbotStopKey.SetFileId("aim_stop");
	AimbotGroup.PlaceLabledControl("Stop Aim", this, &AimbotStopKey);
#pragma endregion Aimbot Controls Get Setup in here

#pragma region Target
	TargetGroup.SetPosition(16, 358);
	TargetGroup.SetText("Target");
	TargetGroup.SetSize(376, 168);
	RegisterControl(&TargetGroup);

	TargetSelection.SetFileId("tgt_selection");
	TargetSelection.AddItem("Closest To Crosshair");
	TargetSelection.AddItem("Distance");
	TargetSelection.AddItem("Lowest Health");
	TargetGroup.PlaceLabledControl("Selection", this, &TargetSelection);

	TargetFriendlyFire.SetFileId("tgt_friendlyfire");
	TargetGroup.PlaceLabledControl("Friendly Fire", this, &TargetFriendlyFire);

	TargetHitbox.SetFileId("tgt_hitbox");
	TargetHitbox.AddItem("Head");
	TargetHitbox.AddItem("Neck");
	TargetHitbox.AddItem("Chest");
	TargetHitbox.AddItem("Stomach");
	TargetHitbox.AddItem("Pelvis");
	TargetGroup.PlaceLabledControl("Hitbox", this, &TargetHitbox);

	TargetHitscan.SetFileId("tgt_hitscan");
	TargetHitscan.AddItem("Off"); // Hitscan Case 0
	TargetHitscan.AddItem("Head"); // Hitscan case 1
	TargetHitscan.AddItem("Upperbody"); // Hitscan Case 2
	TargetHitscan.AddItem("Everything"); // Hitscan Case 3
	TargetGroup.PlaceLabledControl("Hitscan", this, &TargetHitscan);

	TargetMultipoint.SetFileId("tgt_multipoint");
	TargetGroup.PlaceLabledControl("Multipoint", this, &TargetMultipoint);

	TargetPointscale.SetFileId("tgt_pointscale"); // Pointscale Meme
	TargetPointscale.SetBoundaries(0.f, 10.f);
	TargetPointscale.SetValue(0.f);
	TargetGroup.PlaceLabledControl("Point Scale", this, &TargetPointscale);
#pragma endregion Targetting controls 

#pragma region Accuracy
	AccuracyGroup.SetPosition(408, 48);
	AccuracyGroup.SetText("Accuracy");
	AccuracyGroup.SetSize(360, 270); //280
	RegisterControl(&AccuracyGroup);

	AccuracyRecoil.SetFileId("acc_norecoil");
	AccuracyGroup.PlaceLabledControl("Anti-Recoil", this, &AccuracyRecoil);

	AccuracyAutoWall.SetFileId("acc_awall");
	AccuracyGroup.PlaceLabledControl("Penetrate Wall", this, &AccuracyAutoWall);

	AccuracyMinimumDamage.SetFileId("acc_mindmg");
	AccuracyMinimumDamage.SetBoundaries(0.f, 99.f);
	AccuracyMinimumDamage.SetValue(0.f);
	AccuracyGroup.PlaceLabledControl("Autowall Damage", this, &AccuracyMinimumDamage);

	AccuracyAutoStop.SetFileId("acc_stop");
	AccuracyGroup.PlaceLabledControl("Auto Stop / Crouch", this, &AccuracyAutoStop);

	AccuracyAutoScope.SetFileId("acc_scope");
	AccuracyGroup.PlaceLabledControl("Auto Scope", this, &AccuracyAutoScope);

	AccuracyHitchance.SetFileId("acc_chance");
	AccuracyHitchance.SetBoundaries(0, 100);
	AccuracyHitchance.SetValue(0);
	AccuracyGroup.PlaceLabledControl("Hit Chance", this, &AccuracyHitchance);

	AccuracyPrediction.SetFileId("acc_prediction");
	AccuracyGroup.PlaceLabledControl("Movement Prediction", this, &AccuracyPrediction);
	
	AccuracyResolverPitch.SetFileId("acc_pitchresolver");
	AccuracyResolverPitch.AddItem("Off");
	AccuracyResolverPitch.AddItem("Test 1");
	// AccuracyResolverPitch.AddItem("Reserved");
	// AccuracyResolverPitch.AddItem("Reserved");
	AccuracyGroup.PlaceLabledControl("Resolver Pitch", this, &AccuracyResolverPitch);

	AccuracyResolverYaw.SetFileId("acc_yawresolver");
	AccuracyResolverYaw.AddItem("Off");
	AccuracyResolverYaw.AddItem("Bruteforce LBY 1");
	AccuracyResolverYaw.AddItem("Bruteforce LBY 2");
	AccuracyResolverYaw.AddItem("Bruteforce LBY 3");
	AccuracyResolverYaw.AddItem("Normal Bruteforce");
	AccuracyResolverYaw.AddItem("Auto 1");
	AccuracyResolverYaw.AddItem("Yaw32");
	AccuracyResolverYaw.AddItem("Yaw64");
	AccuracyGroup.PlaceLabledControl("Resolver Yaw", this, &AccuracyResolverYaw);

	// AccuracyResolver.SetFileId("acc_aaa");
	// AccuracyGroup.PlaceLabledControl("Anti Aim Resolver", this, &AccuracyResolver);

	AccuracySmart.SetFileId("acc_smart");
	AccuracySmart.SetBoundaries(0, 20);
	AccuracySmart.SetValue(0);
	AccuracyGroup.PlaceLabledControl("Smart Aim", this, &AccuracySmart);
#pragma endregion  Accuracy controls get Setup in here

#pragma region AntiAim
	AntiAimGroup.SetPosition(408, 328); //344
	AntiAimGroup.SetText("Anti Aim");
	AntiAimGroup.SetSize(360, 300);
	RegisterControl(&AntiAimGroup);

	AntiAimEnable.SetFileId("aa_enable");
	AntiAimGroup.PlaceLabledControl("Enable", this, &AntiAimEnable);

	AntiAimPitch.SetFileId("aa_x"); // Antiaim List for Pitch
	AntiAimPitch.AddItem("Disabled");
	AntiAimPitch.AddItem("Emotion");
	AntiAimPitch.AddItem("Fake Down");
	AntiAimPitch.AddItem("Fake Up");
	AntiAimPitch.AddItem("Zero Down");
	AntiAimPitch.AddItem("Zero Up");
	AntiAimGroup.PlaceLabledControl("Pitch", this, &AntiAimPitch);

	AntiAimYaw.SetFileId("aa_y"); // Antiaim List for Yaw
	AntiAimYaw.AddItem("Disabled");
	AntiAimYaw.AddItem("Backward");
	AntiAimYaw.AddItem("Breaker 1");
	AntiAimYaw.AddItem("Breaker 2");
	AntiAimYaw.AddItem("Breaker 3");
	AntiAimYaw.AddItem("Test Breaker");
	AntiAimYaw.AddItem("LBY Breaker 1");
	AntiAimYaw.AddItem("LBY Breaker 2");
	AntiAimYaw.AddItem("LowerBody");
	AntiAimYaw.AddItem("LowerBody Backwards");
	AntiAimYaw.AddItem("Fake Lower Body");
	AntiAimYaw.AddItem("FJitter");
	AntiAimYaw.AddItem("Fake Spin");
	AntiAimYaw.AddItem("Fake Backward");
	AntiAimYaw.AddItem("Fake Forward");
	AntiAimYaw.AddItem("Fake Static");
	AntiAimYaw.AddItem("TFake");
	AntiAimYaw.AddItem("TJitter");
	AntiAimGroup.PlaceLabledControl("Real Yaw", this, &AntiAimYaw);

	RealAntiAimYawSlider.SetFileId("aa_yaa");
	RealAntiAimYawSlider.SetBoundaries(0, 360);
	RealAntiAimYawSlider.SetValue(0);
	AntiAimGroup.PlaceLabledControl("Custom Yaw", this, &RealAntiAimYawSlider);

	// AntiAimOffset.SetFileId("aa_offset");
	// AntiAimOffset.SetBoundaries(0, 360);
	// AntiAimOffset.SetValue(0);
	// AntiAimGroup.PlaceLabledControl("Yaw Offset", this, &AntiAimOffset);

	AntiAimSpinspeed.SetFileId("aa_spinspeed");
	AntiAimSpinspeed.SetBoundaries(0, 20);
	AntiAimSpinspeed.SetValue(0);
	AntiAimGroup.PlaceLabledControl("Spinbot Speed", this, &AntiAimSpinspeed);

	AntiAimTicks.SetFileId("aa_ticks");
	AntiAimTicks.SetBoundaries(0, 2);
	AntiAimTicks.SetValue(1.1);
	AntiAimGroup.PlaceLabledControl("Ticks", this, &AntiAimTicks);

	AntiAimPercentage.SetFileId("aa_percentage");
	AntiAimPercentage.SetBoundaries(0, 100);
	AntiAimPercentage.SetValue(2);
	AntiAimGroup.PlaceLabledControl("Percentage", this, &AntiAimPercentage);

	AntiAimKnife.SetFileId("aa_knife");
	AntiAimGroup.PlaceLabledControl("Anti Aim on Knife", this, &AntiAimKnife);

	AntiAimTarget.SetFileId("aa_target");
	AntiAimGroup.PlaceLabledControl("Anti Aim At Target", this, &AntiAimTarget);

	// AntiAimEdge.SetFileId("aa_edge");
	// AntiAimGroup.PlaceLabledControl("Anti Aim Edge", this, &AntiAimEdge);
#pragma endregion  AntiAim controls get setup in here
}

void CLegitBotTab::Setup()
{
	SetTitle("Legit");

	ActiveLabel.SetPosition(16, 16);
	ActiveLabel.SetText("Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(66, 16);
	RegisterControl(&Active);

#pragma region Aimbot
	AimbotGroup.SetPosition(16, 48);
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetSize(240, 210);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl("Enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl("Auto Fire", this, &AimbotAutoFire);

	AimbotFriendlyFire.SetFileId("aim_friendfire");
	AimbotGroup.PlaceLabledControl("Friendly Fire", this, &AimbotFriendlyFire);

	AimbotKeyPress.SetFileId("aim_usekey");
	AimbotGroup.PlaceLabledControl("On Key Press", this, &AimbotKeyPress);

	AimbotKeyBind.SetFileId("aim_key");
	AimbotGroup.PlaceLabledControl("Key Bind", this, &AimbotKeyBind);
	
	AimbotAutoPistol.SetFileId("aim_apistol");
	AimbotGroup.PlaceLabledControl("Auto Pistol", this, &AimbotAutoPistol);

#pragma endregion Aimbot shit

#pragma region Triggerbot
	TriggerGroup.SetPosition(272, 48);
	TriggerGroup.SetText("Triggerbot");
	TriggerGroup.SetSize(240, 210);
	RegisterControl(&TriggerGroup);

	TriggerEnable.SetFileId("trig_enable");
	TriggerGroup.PlaceLabledControl("Enable", this, &TriggerEnable);

	TriggerKeyPress.SetFileId("trig_onkey");
	TriggerGroup.PlaceLabledControl("On Key Press", this, &TriggerKeyPress);

	TriggerKeyBind.SetFileId("trig_key");
	TriggerGroup.PlaceLabledControl("Key Bind", this, &TriggerKeyBind);

	TriggerDelay.SetFileId("trig_time");
	TriggerDelay.SetBoundaries(0.f, 1000.f);
	TriggerGroup.PlaceLabledControl("Delay (ms)", this, &TriggerDelay);
#pragma endregion Triggerbot stuff

#pragma region Main Weapon
	WeaponMainGroup.SetPosition(16, 274);
	WeaponMainGroup.SetText("Rifles/Other");
	WeaponMainGroup.SetSize(240, 210);
	RegisterControl(&WeaponMainGroup);

	WeaponMainSpeed.SetFileId("main_speed");
	WeaponMainSpeed.SetBoundaries(0.1f, 2.f); 
	WeaponMainSpeed.SetValue(1.0f);
	WeaponMainGroup.PlaceLabledControl("Max Speed", this, &WeaponMainSpeed);

	WeaponMainFoV.SetFileId("main_fov");
	WeaponMainFoV.SetBoundaries(0.1f, 30.f);
	WeaponMainFoV.SetValue(5.f);
	WeaponMainGroup.PlaceLabledControl("FoV", this, &WeaponMainFoV);

	WeaponMainRecoil.SetFileId("main_recoil");
	WeaponMainGroup.PlaceLabledControl("Recoil", this, &WeaponMainRecoil);

	WeaponMainPSilent.SetFileId("main_psilent");
	WeaponMainGroup.PlaceLabledControl("Perfect Silent", this, &WeaponMainPSilent);

	WeaponMainInacc.SetFileId("main_inacc");
	WeaponMainInacc.SetBoundaries(0.f, 15.f);
	WeaponMainGroup.PlaceLabledControl("Inaccuracy", this, &WeaponMainInacc);

	WeaponMainHitbox.SetFileId("main_hitbox");
	WeaponMainHitbox.AddItem("Head");
	WeaponMainHitbox.AddItem("Neck");
	WeaponMainHitbox.AddItem("Chest");
	WeaponMainHitbox.AddItem("Stomach");
	WeaponMainGroup.PlaceLabledControl("Hitbox", this, &WeaponMainHitbox);
#pragma endregion

#pragma region Pistols
	WeaponPistGroup.SetPosition(272, 274);
	WeaponPistGroup.SetText("Pistols");
	WeaponPistGroup.SetSize(240, 210);
	RegisterControl(&WeaponPistGroup);

	WeaponPistSpeed.SetFileId("pist_speed");
	WeaponPistSpeed.SetBoundaries(0.1f, 2.f);
	WeaponPistSpeed.SetValue(1.0f);
	WeaponPistGroup.PlaceLabledControl("Max Speed", this, &WeaponPistSpeed);

	WeaponPistFoV.SetFileId("pist_fov");
	WeaponPistFoV.SetBoundaries(0.1f, 30.f);
	WeaponPistFoV.SetValue(5.f);
	WeaponPistGroup.PlaceLabledControl("FoV", this, &WeaponPistFoV);

	WeaponPistRecoil.SetFileId("pist_recoil");
	WeaponPistGroup.PlaceLabledControl("Recoil", this, &WeaponPistRecoil);

	WeaponPistPSilent.SetFileId("pist_psilent");
	WeaponPistGroup.PlaceLabledControl("Perfect Silent", this, &WeaponPistPSilent);

	WeaponPistInacc.SetFileId("pist_inacc");
	WeaponPistInacc.SetBoundaries(0.f, 15.f);
	WeaponPistGroup.PlaceLabledControl("Inaccuracy", this, &WeaponPistInacc);

	WeaponPistHitbox.SetFileId("pist_hitbox");
	WeaponPistHitbox.AddItem("Head");
	WeaponPistHitbox.AddItem("Neck");
	WeaponPistHitbox.AddItem("Chest");
	WeaponPistHitbox.AddItem("Stomach");
	WeaponPistGroup.PlaceLabledControl("Hitbox", this, &WeaponPistHitbox);
#pragma endregion

#pragma region Snipers
	WeaponSnipGroup.SetPosition(528, 274);
	WeaponSnipGroup.SetText("Snipers");
	WeaponSnipGroup.SetSize(240, 210);
	RegisterControl(&WeaponSnipGroup);

	WeaponSnipSpeed.SetFileId("snip_speed");
	WeaponSnipSpeed.SetBoundaries(0.1f, 2.f);
	WeaponSnipSpeed.SetValue(1.0f);
	WeaponSnipGroup.PlaceLabledControl("Max Speed", this, &WeaponSnipSpeed);

	WeaponSnipFoV.SetFileId("snip_fov");
	WeaponSnipFoV.SetBoundaries(0.1f, 30.f);
	WeaponSnipFoV.SetValue(5.f);
	WeaponSnipGroup.PlaceLabledControl("FoV", this, &WeaponSnipFoV);

	WeaponSnipRecoil.SetFileId("snip_recoil");
	WeaponSnipGroup.PlaceLabledControl("Recoil", this, &WeaponSnipRecoil);

	WeaponSnipPSilent.SetFileId("snip_psilent");
	WeaponSnipGroup.PlaceLabledControl("Perfect Silent", this, &WeaponSnipPSilent);

	WeaponSnipInacc.SetFileId("snip_inacc");
	WeaponSnipInacc.SetBoundaries(0.f, 15.f);
	WeaponSnipGroup.PlaceLabledControl("Inaccuracy", this, &WeaponSnipInacc);

	WeaponSnipHitbox.SetFileId("snip_hitbox");
	WeaponSnipHitbox.AddItem("Head");
	WeaponSnipHitbox.AddItem("Neck");
	WeaponSnipHitbox.AddItem("Chest");
	WeaponSnipHitbox.AddItem("Stomach");
	WeaponSnipGroup.PlaceLabledControl("Hitbox", this, &WeaponSnipHitbox);
#pragma endregion
}

void CVisualTab::Setup()
{
	SetTitle("Visuals");

	ActiveLabel.SetPosition(16, 16);
	ActiveLabel.SetText("Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(66, 16);
	RegisterControl(&Active);

#pragma region Options
	OptionsGroup.SetText("Options");
	OptionsGroup.SetPosition(16, 48);
	OptionsGroup.SetSize(193, 430);
	RegisterControl(&OptionsGroup);

	OptionsBox.SetFileId("opt_box");
	OptionsGroup.PlaceLabledControl("Box", this, &OptionsBox);

	BoxDesign.SetFileId("opt_boxtype");
	BoxDesign.AddItem("Cornors");
	BoxDesign.AddItem("Whole");
	OptionsGroup.PlaceLabledControl("Box Type", this, &BoxDesign);

	OptionsName.SetFileId("opt_name");
	OptionsGroup.PlaceLabledControl("Name", this, &OptionsName);

	OptionHealthEnable.SetFileId("opt_health");
	OptionsGroup.PlaceLabledControl("Health", this, &OptionHealthEnable);

	OptionsHealth.SetFileId("opt_hp");
	OptionsHealth.AddItem("Interwebz");
	OptionsHealth.AddItem("Aimware");
	OptionsGroup.PlaceLabledControl("Health", this, &OptionsHealth);

	OptionsWeapon.SetFileId("opt_weapon");
	OptionsGroup.PlaceLabledControl("Weapon", this, &OptionsWeapon);

	OptionsInfo.SetFileId("opt_info");
	OptionsGroup.PlaceLabledControl("Info", this, &OptionsInfo);

	OptionsChams.SetFileId("opt_chams");
	OptionsChams.AddItem("Off");
	OptionsChams.AddItem("Normal");
	OptionsChams.AddItem("Flat");
	OptionsGroup.PlaceLabledControl("Chams", this, &OptionsChams);

	OptionsSkeleton.SetFileId("opt_bone");
	OptionsGroup.PlaceLabledControl("Skeleton", this, &OptionsSkeleton);

	OptionsAimSpot.SetFileId("opt_aimspot");
	OptionsGroup.PlaceLabledControl("Head Cross", this, &OptionsAimSpot);
	
	OptionsCompRank.SetFileId("opt_comprank");
	OptionsGroup.PlaceLabledControl("Player Ranks", this, &OptionsCompRank);

#pragma endregion Setting up the Options controls

#pragma region ESPColorSkeleton
	ESPColorSkeleton.SetText("ESP Color");
	ESPColorSkeleton.SetPosition(16, 488);
	ESPColorSkeleton.SetSize(193, 200);
	RegisterControl(&ESPColorSkeleton);

	RSkele.SetFileId("colormenu_rmma");
	RSkele.SetBoundaries(0, 255);
	RSkele.SetValue(255);
	ESPColorSkeleton.PlaceLabledControl("R", this, &RSkele);

	GSkele.SetFileId("colormenu_gmma");
	GSkele.SetBoundaries(0, 255);
	GSkele.SetValue(0);
	ESPColorSkeleton.PlaceLabledControl("G", this, &GSkele);

	BSkele.SetFileId("colormenu_bmma");
	BSkele.SetBoundaries(0, 255);
	BSkele.SetValue(0);
	ESPColorSkeleton.PlaceLabledControl("B", this, &BSkele);

	ASkele.SetFileId("colormenu_amma");
	ASkele.SetBoundaries(0, 255);
	ASkele.SetValue(255);
	ESPColorSkeleton.PlaceLabledControl("A", this, &ASkele);

#pragma endregion

#pragma region ESPColorBox
	ESPColorBox.SetText("CT ESP");
	ESPColorBox.SetPosition(225, 488);
	ESPColorBox.SetSize(193, 220);
	RegisterControl(&ESPColorBox);

	RBoxCT.SetFileId("colorbox_rmma");
	RBoxCT.SetBoundaries(0, 255);
	RBoxCT.SetValue(15);
	ESPColorBox.PlaceLabledControl("R CT", this, &RBoxCT);

	GBoxCT.SetFileId("colorbox_gmma");
	GBoxCT.SetBoundaries(0, 255);
	GBoxCT.SetValue(110);
	ESPColorBox.PlaceLabledControl("G CT", this, &GBoxCT);

	BBoxCT.SetFileId("colorbox_bmma");
	BBoxCT.SetBoundaries(0, 255);
	BBoxCT.SetValue(220);
	ESPColorBox.PlaceLabledControl("B CT", this, &BBoxCT);

	ABoxCT.SetFileId("colorbox_amma");
	ABoxCT.SetBoundaries(0, 255);
	ABoxCT.SetValue(255);
	ESPColorBox.PlaceLabledControl("A CT", this, &ABoxCT);

	RBoxCTV.SetFileId("colorbox_rmma");
	RBoxCTV.SetBoundaries(0, 255);
	RBoxCTV.SetValue(120);
	ESPColorBox.PlaceLabledControl("R CT Visible", this, &RBoxCTV);

	GBoxCTV.SetFileId("colorbox_gmma");
	GBoxCTV.SetBoundaries(0, 255);
	GBoxCTV.SetValue(210);
	ESPColorBox.PlaceLabledControl("G CT Visible", this, &GBoxCTV);

	BBoxCTV.SetFileId("colorbox_bmma");
	BBoxCTV.SetBoundaries(0, 255);
	BBoxCTV.SetValue(26);
	ESPColorBox.PlaceLabledControl("B CT Visible", this, &BBoxCTV);

	ABoxCTV.SetFileId("colorbox_amma");
	ABoxCTV.SetBoundaries(0, 255);
	ABoxCTV.SetValue(255);
	ESPColorBox.PlaceLabledControl("A CT Visible", this, &ABoxCTV);

#pragma endregion

#pragma region Filters
	FiltersGroup.SetText("Filters");
	FiltersGroup.SetPosition(225, 48);
	FiltersGroup.SetSize(193, 430);
	RegisterControl(&FiltersGroup);

	FiltersAll.SetFileId("ftr_all");
	FiltersGroup.PlaceLabledControl("All", this, &FiltersAll);

	FiltersPlayers.SetFileId("ftr_players");
	FiltersGroup.PlaceLabledControl("Players", this, &FiltersPlayers);

	FiltersEnemiesOnly.SetFileId("ftr_enemyonly");
	FiltersGroup.PlaceLabledControl("Enemies Only", this, &FiltersEnemiesOnly);

	FiltersWeapons.SetFileId("ftr_weaps");
	FiltersGroup.PlaceLabledControl("Weapons", this, &FiltersWeapons);

	FiltersChickens.SetFileId("ftr_chickens");
	FiltersGroup.PlaceLabledControl("Chickens", this, &FiltersChickens);

	FiltersC4.SetFileId("ftr_c4");
	FiltersGroup.PlaceLabledControl("C4", this, &FiltersC4);
#pragma endregion Setting up the Filters controls

#pragma region Other
	OtherGroup.SetText("Other");
	OtherGroup.SetPosition(434, 48);
	OtherGroup.SetSize(334, 430);
	RegisterControl(&OtherGroup);

	OtherCrosshair.SetFileId("otr_xhair");
	OtherCrosshair.AddItem("Off");
	OtherCrosshair.AddItem("Rainbow X");
	OtherCrosshair.AddItem("Rainbow Swastika");
	OtherGroup.PlaceLabledControl("Crosshair", this, &OtherCrosshair);

	OtherRecoilCrosshair.SetFileId("otr_recoilhair");
	OtherRecoilCrosshair.AddItem("Off");
	OtherRecoilCrosshair.AddItem("Recoil Position");
	OtherGroup.PlaceLabledControl("Recoil Crosshair", this, &OtherRecoilCrosshair);

	OtherRadar.SetFileId("otr_radar");
	OtherGroup.PlaceLabledControl("Radar", this, &OtherRadar);

	OtherNoVisualRecoil.SetFileId("otr_visrecoil");
	OtherGroup.PlaceLabledControl("No Visual Recoil", this, &OtherNoVisualRecoil);

	OtherNoFlash.SetFileId("otr_noflash");
	OtherGroup.PlaceLabledControl("No Flash", this, &OtherNoFlash);

	OtherNoHands.SetFileId("otr_hands");
	OtherNoHands.AddItem("Off");
	OtherNoHands.AddItem("None");
	OtherNoHands.AddItem("Transparent");
	OtherNoHands.AddItem("Chams");
	OtherNoHands.AddItem("Rainbow");
	OtherGroup.PlaceLabledControl("Hands", this, &OtherNoHands);

	OtherThirdperson.SetFileId("aa_thirdpsr");
	OtherGroup.PlaceLabledControl("Thirdperson", this, &OtherThirdperson);

	OtherViewmodelFOV.SetFileId("otr_viewfov");
	OtherViewmodelFOV.SetBoundaries(0.f, 180.f);
	OtherViewmodelFOV.SetValue(0.f);
	OtherGroup.PlaceLabledControl("Viewmodel FOV Changer", this, &OtherViewmodelFOV);

	OtherFOV.SetFileId("otr_fov");
	OtherFOV.SetBoundaries(0.f, 180.f);
	OtherFOV.SetValue(90.f);
	OtherGroup.PlaceLabledControl("Field of View Changer", this, &OtherFOV);

	CustomSky.SetFileId("otr_customsky");
	OtherGroup.PlaceLabledControl("Custom Skies", this, &CustomSky);

	OtherNoScope.SetFileId("otr_noscope");
	OtherGroup.PlaceLabledControl("Noscope", this, &OtherNoScope);

#pragma endregion Setting up the Other controls
}

void CSkinTab::Setup()
{
	SetTitle("Skins");

	SkinActive.SetPosition(16, 16);
	SkinActive.SetText("Active");
	RegisterControl(&SkinActive);

	SkinEnable.SetFileId("skin_enable");
	SkinEnable.SetPosition(66, 16);
	RegisterControl(&SkinEnable);

	SkinApply.SetText("Apply");
	SkinApply.SetCallback(KnifeApplyCallbk);
	SkinApply.SetPosition(408, 490);
	SkinApply.SetSize(360, 106);
	RegisterControl(&SkinApply);

#pragma region Knife
	KnifeGroup.SetPosition(16, 48);
	KnifeGroup.SetText("Knife");
	KnifeGroup.SetSize(376, 80);
	RegisterControl(&KnifeGroup);

	KnifeModel.SetFileId("knife_model");
	KnifeModel.AddItem("Bayonet");
	KnifeModel.AddItem("Bowie Knife");
	KnifeModel.AddItem("Butterfly Knife");
	KnifeModel.AddItem("Falchion Knife");
	KnifeModel.AddItem("Flip Knife");
	KnifeModel.AddItem("Gut Knife");
	KnifeModel.AddItem("Huntsman Knife");
	KnifeModel.AddItem("Karambit");
	KnifeModel.AddItem("M9 Bayonet");
	KnifeModel.AddItem("Shadow Daggers");
	KnifeGroup.PlaceLabledControl("Knife", this, &KnifeModel);

	KnifeSkin.SetFileId("knife_skin");
	KnifeSkin.AddItem("None");
	KnifeSkin.AddItem("Crimson Web");
	KnifeSkin.AddItem("Bone Mask");
	KnifeSkin.AddItem("Fade");
	KnifeSkin.AddItem("Night");
	KnifeSkin.AddItem("Blue Steel");
	KnifeSkin.AddItem("Stained");
	KnifeSkin.AddItem("Case Hardened");
	KnifeSkin.AddItem("Slaughter");
	KnifeSkin.AddItem("Safari Mesh");
	KnifeSkin.AddItem("Boreal Forest");
	KnifeSkin.AddItem("Ultraviolet");
	KnifeSkin.AddItem("Urban Masked");
	KnifeSkin.AddItem("Scorched");
	KnifeSkin.AddItem("Rust Coat");
	KnifeSkin.AddItem("Tiger Tooth");
	KnifeSkin.AddItem("Damascus Steel");
	KnifeSkin.AddItem("Damascus Steel");
	KnifeSkin.AddItem("Marble Fade");
	KnifeSkin.AddItem("Rust Coat");
	KnifeSkin.AddItem("Doppler Ruby");
	KnifeSkin.AddItem("Doppler Sapphire");
	KnifeSkin.AddItem("Doppler Blackpearl");
	KnifeSkin.AddItem("Doppler Phase 1");
	KnifeSkin.AddItem("Doppler Phase 2");
	KnifeSkin.AddItem("Doppler Phase 3");
	KnifeSkin.AddItem("Doppler Phase 4");
	KnifeSkin.AddItem("Gamma Doppler Phase 1");
	KnifeSkin.AddItem("Gamma Doppler Phase 2");
	KnifeSkin.AddItem("Gamma Doppler Phase 3");
	KnifeSkin.AddItem("Gamma Doppler Phase 4");
	KnifeSkin.AddItem("Gamma Doppler Emerald");
	KnifeSkin.AddItem("Lore");
	KnifeGroup.PlaceLabledControl("Skin", this, &KnifeSkin);
#pragma endregion

#pragma region Machineguns
	MachinegunsGroup.SetPosition(408, 48);
	MachinegunsGroup.SetText("Machineguns");
	MachinegunsGroup.SetSize(360, 80);
	RegisterControl(&MachinegunsGroup);

	NEGEVSkin.SetFileId("negev_skin");
	NEGEVSkin.AddItem("Anodized Navy");
	NEGEVSkin.AddItem("Man-o'-war");
	NEGEVSkin.AddItem("Palm");
	NEGEVSkin.AddItem("VariCamo");
	NEGEVSkin.AddItem("Palm");
	NEGEVSkin.AddItem("CaliCamo");
	NEGEVSkin.AddItem("Terrain");
	NEGEVSkin.AddItem("Army Sheen");
	NEGEVSkin.AddItem("Bratatat");
	NEGEVSkin.AddItem("Desert-Strike");
	NEGEVSkin.AddItem("Nuclear Waste");
	NEGEVSkin.AddItem("Loudmouth");
	NEGEVSkin.AddItem("Power Loader");
	MachinegunsGroup.PlaceLabledControl("Negev", this, &NEGEVSkin);

	M249Skin.SetFileId("m249_skin");
	M249Skin.AddItem("Contrast Spray");
	M249Skin.AddItem("Blizzard Marbleized");
	M249Skin.AddItem("Jungle DDPAT");
	M249Skin.AddItem("Gator Mesh");
	M249Skin.AddItem("Magma");
	M249Skin.AddItem("System Lock");
	M249Skin.AddItem("Shipping Forecast");
	M249Skin.AddItem("Impact Drill");
	M249Skin.AddItem("Nebula Crusader");
	M249Skin.AddItem("Spectre");
	MachinegunsGroup.PlaceLabledControl("M249", this, &M249Skin);

#pragma endregion

#pragma region Snipers
	Snipergroup.SetPosition(16, 135);
	Snipergroup.SetText("Snipers");
	Snipergroup.SetSize(376, 125);
	RegisterControl(&Snipergroup);

	AWPSkin.SetFileId("awp_skin");
	AWPSkin.AddItem("BOOM");
	AWPSkin.AddItem("Dragon Lore");
	AWPSkin.AddItem("Pink DDPAT");
	AWPSkin.AddItem("Snake Camo");
	AWPSkin.AddItem("Lightning Strike");
	AWPSkin.AddItem("Safari Mesh");
	AWPSkin.AddItem("Corticera");
	AWPSkin.AddItem("Redline");
	AWPSkin.AddItem("Man-o'-war");
	AWPSkin.AddItem("Graphite");
	AWPSkin.AddItem("Electric Hive");
	AWPSkin.AddItem("Pit Viper");
	AWPSkin.AddItem("Asiimov");
	AWPSkin.AddItem("Worm God");
	AWPSkin.AddItem("Medusa");
	AWPSkin.AddItem("Sun in Leo");
	AWPSkin.AddItem("Hyper Beast");
	AWPSkin.AddItem("Elite Build");
	Snipergroup.PlaceLabledControl("AWP", this, &AWPSkin);

	SSG08Skin.SetFileId("sgg08_skin");
	SSG08Skin.AddItem("Lichen Dashed");
	SSG08Skin.AddItem("Dark Water");
	SSG08Skin.AddItem("Blue Spruce");
	SSG08Skin.AddItem("Sand Dune");
	SSG08Skin.AddItem("Palm");
	SSG08Skin.AddItem("Mayan Dreams");
	SSG08Skin.AddItem("Blood in the Water");
	SSG08Skin.AddItem("Tropical Storm");
	SSG08Skin.AddItem("Acid Fade");
	SSG08Skin.AddItem("Slashed");
	SSG08Skin.AddItem("Detour");
	SSG08Skin.AddItem("Abyss");
	SSG08Skin.AddItem("Big Iron");
	SSG08Skin.AddItem("Necropos");
	SSG08Skin.AddItem("Ghost Crusader");
	SSG08Skin.AddItem("Dragonfire");
	Snipergroup.PlaceLabledControl("SGG 08", this, &SSG08Skin);

	SCAR20Skin.SetFileId("scar20_skin");
	SCAR20Skin.AddItem("Splash Jam");
	SCAR20Skin.AddItem("Storm");
	SCAR20Skin.AddItem("Contractor");
	SCAR20Skin.AddItem("Carbon Fiber");
	SCAR20Skin.AddItem("Sand Mesh");
	SCAR20Skin.AddItem("Palm");
	SCAR20Skin.AddItem("Emerald");
	SCAR20Skin.AddItem("Crimson Web");
	SCAR20Skin.AddItem("Cardiac");
	SCAR20Skin.AddItem("Army Sheen");
	SCAR20Skin.AddItem("Cyrex");
	SCAR20Skin.AddItem("Grotto");
	SCAR20Skin.AddItem("Emerald");
	SCAR20Skin.AddItem("Green Marine");
	SCAR20Skin.AddItem("Outbreak");
	SCAR20Skin.AddItem("Bloodsport");
	Snipergroup.PlaceLabledControl("SCAR-20", this, &SCAR20Skin);

	G3SG1Skin.SetFileId("g3sg1_skin");
	G3SG1Skin.AddItem("Desert Storm");
	G3SG1Skin.AddItem("Arctic Camo");
	G3SG1Skin.AddItem("Bone Mask");
	G3SG1Skin.AddItem("Contractor");
	G3SG1Skin.AddItem("Safari Mesh");
	G3SG1Skin.AddItem("Polar Camo");
	G3SG1Skin.AddItem("Jungle Dashed");
	G3SG1Skin.AddItem("VariCamo");
	G3SG1Skin.AddItem("Predator");
	G3SG1Skin.AddItem("Demeter");
	G3SG1Skin.AddItem("Azure Zebra");
	G3SG1Skin.AddItem("Green Apple");
	G3SG1Skin.AddItem("Orange Kimono");
	G3SG1Skin.AddItem("Neon Kimono");
	G3SG1Skin.AddItem("Murky");
	G3SG1Skin.AddItem("Chronos");
	G3SG1Skin.AddItem("Flux");
	G3SG1Skin.AddItem("The Executioner");
	G3SG1Skin.AddItem("Orange Crash");
	Snipergroup.PlaceLabledControl("G3SG1", this, &G3SG1Skin);
#pragma endregion

#pragma region Shotguns
	Shotgungroup.SetPosition(408, 135);
	Shotgungroup.SetText("Shotguns");
	Shotgungroup.SetSize(360, 125);
	RegisterControl(&Shotgungroup);

	MAG7Skin.SetFileId("mag7_skin");
	MAG7Skin.AddItem("Counter Terrace");
	MAG7Skin.AddItem("Metallic DDPAT");
	MAG7Skin.AddItem("Silver");
	MAG7Skin.AddItem("Storm");
	MAG7Skin.AddItem("Bulldozer");
	MAG7Skin.AddItem("Heat");
	MAG7Skin.AddItem("Sand Dune");
	MAG7Skin.AddItem("Irradiated Alert");
	MAG7Skin.AddItem("Memento");
	MAG7Skin.AddItem("Hazard");
	MAG7Skin.AddItem("Heaven Guard");
	MAG7Skin.AddItem("Firestarter");
	MAG7Skin.AddItem("Seabird");
	MAG7Skin.AddItem("Cobalt Core");
	MAG7Skin.AddItem("Praetorian");
	Shotgungroup.PlaceLabledControl("Mag-7", this, &MAG7Skin);

	XM1014Skin.SetFileId("xm1014_skin");
	XM1014Skin.AddItem("Blaze Orange");
	XM1014Skin.AddItem("VariCamo Blue");
	XM1014Skin.AddItem("Bone Mask");
	XM1014Skin.AddItem("Blue Steel");
	XM1014Skin.AddItem("Blue Spruce");
	XM1014Skin.AddItem("Grassland");
	XM1014Skin.AddItem("Urban Perforated");
	XM1014Skin.AddItem("Jungle");
	XM1014Skin.AddItem("VariCamo");
	XM1014Skin.AddItem("VariCamo");
	XM1014Skin.AddItem("Fallout Warning");
	XM1014Skin.AddItem("Jungle");
	XM1014Skin.AddItem("CaliCamo");
	XM1014Skin.AddItem("Pit Viper");
	XM1014Skin.AddItem("Tranquility");
	XM1014Skin.AddItem("Red Python");
	XM1014Skin.AddItem("Heaven Guard");
	XM1014Skin.AddItem("Red Leather");
	XM1014Skin.AddItem("Bone Machine");
	XM1014Skin.AddItem("Quicksilver");
	XM1014Skin.AddItem("Scumbria");
	XM1014Skin.AddItem("Teclu Burner");
	XM1014Skin.AddItem("Black Tie");
	Shotgungroup.PlaceLabledControl("XM1014", this, &XM1014Skin);

	SAWEDOFFSkin.SetFileId("sawedoff_skin");
	SAWEDOFFSkin.AddItem("First Class");
	SAWEDOFFSkin.AddItem("Forest DDPAT");
	SAWEDOFFSkin.AddItem("Contrast Spray");
	SAWEDOFFSkin.AddItem("Snake Camo");
	SAWEDOFFSkin.AddItem("Orange DDPAT");
	SAWEDOFFSkin.AddItem("Fade");
	SAWEDOFFSkin.AddItem("Copper");
	SAWEDOFFSkin.AddItem("Origami");
	SAWEDOFFSkin.AddItem("Sage Spray");
	SAWEDOFFSkin.AddItem("VariCamo");
	SAWEDOFFSkin.AddItem("Irradiated Alert");
	SAWEDOFFSkin.AddItem("Mosaico");
	SAWEDOFFSkin.AddItem("Serenity");
	SAWEDOFFSkin.AddItem("Amber Fade");
	SAWEDOFFSkin.AddItem("Full Stop");
	SAWEDOFFSkin.AddItem("Highwayman");
	SAWEDOFFSkin.AddItem("The Kraken");
	SAWEDOFFSkin.AddItem("Rust Coat");
	SAWEDOFFSkin.AddItem("Bamboo Shadow");
	SAWEDOFFSkin.AddItem("Bamboo Forest");
	SAWEDOFFSkin.AddItem("Yorick");
	SAWEDOFFSkin.AddItem("Fubar");
	SAWEDOFFSkin.AddItem("Wasteland Princess");
	Shotgungroup.PlaceLabledControl("Sawed-Off", this, &SAWEDOFFSkin);

	NOVASkin.SetFileId("nova_skin");
	NOVASkin.AddItem("Candy Apple");
	NOVASkin.AddItem("Blaze Orange");
	NOVASkin.AddItem("Modern Hunter");
	NOVASkin.AddItem("Forest Leaves");
	NOVASkin.AddItem("Bloomstick");
	NOVASkin.AddItem("Sand Dune");
	NOVASkin.AddItem("Polar Mesh");
	NOVASkin.AddItem("Walnut");
	NOVASkin.AddItem("Predator");
	NOVASkin.AddItem("Tempest");
	NOVASkin.AddItem("Graphite");
	NOVASkin.AddItem("Ghost Camo");
	NOVASkin.AddItem("Rising Skull");
	NOVASkin.AddItem("Antique");
	NOVASkin.AddItem("Green Apple");
	NOVASkin.AddItem("Caged Steel");
	NOVASkin.AddItem("Koi");
	NOVASkin.AddItem("Moon in Libra");
	NOVASkin.AddItem("Ranger");
	NOVASkin.AddItem("HyperBeast");
	Shotgungroup.PlaceLabledControl("Nova", this, &NOVASkin);
#pragma endregion

#pragma region Rifles
	Riflegroup.SetPosition(16, 270);
	Riflegroup.SetText("Rifles");
	Riflegroup.SetSize(376, 195);
	RegisterControl(&Riflegroup);

	AK47Skin.SetFileId("ak47_skin");
	AK47Skin.AddItem("First Class");
	AK47Skin.AddItem("Red Laminate");
	AK47Skin.AddItem("Case Hardened");
	AK47Skin.AddItem("Black Laminate");
	AK47Skin.AddItem("Fire Serpent");
	AK47Skin.AddItem("Cartel");
	AK47Skin.AddItem("Emerald Pinstripe");
	AK47Skin.AddItem("Blue Laminate");
	AK47Skin.AddItem("Redline");
	AK47Skin.AddItem("Vulcan");
	AK47Skin.AddItem("Jaguar");
	AK47Skin.AddItem("Jet Set");
	AK47Skin.AddItem("Wasteland Rebel");
	AK47Skin.AddItem("Elite Build");
	AK47Skin.AddItem("Hydroponic");
	AK47Skin.AddItem("Aquamarine Revenge");
	AK47Skin.AddItem("Frontside Misty");
	AK47Skin.AddItem("Point Disarray");
	AK47Skin.AddItem("Fuel Injector");
	AK47Skin.AddItem("Neon Revolution");
	Riflegroup.PlaceLabledControl("AK-47", this, &AK47Skin);

	M41SSkin.SetFileId("m4a1s_skin");
	M41SSkin.AddItem("Dark Water");
	M41SSkin.AddItem("Hyper Beast");
	M41SSkin.AddItem("Boreal Forest");
	M41SSkin.AddItem("VariCamo");
	M41SSkin.AddItem("Nitro");
	M41SSkin.AddItem("Bright Water");
	M41SSkin.AddItem("Atomic Alloy");
	M41SSkin.AddItem("Blood Tiger");
	M41SSkin.AddItem("Guardian");
	M41SSkin.AddItem("Master Piece");
	M41SSkin.AddItem("Knight");
	M41SSkin.AddItem("Cyrex");
	M41SSkin.AddItem("Basilisk");
	M41SSkin.AddItem("Icarus Fell");
	M41SSkin.AddItem("Hot Rod");
	M41SSkin.AddItem("Golden Coi");
	M41SSkin.AddItem("Chantico's Fire");
	M41SSkin.AddItem("Mecha Industries");
	M41SSkin.AddItem("Flashback");
	Riflegroup.PlaceLabledControl("M4A1-S", this, &M41SSkin);

	M4A4Skin.SetFileId("m4a4_skin");
	M4A4Skin.AddItem("Bullet Rain");
	M4A4Skin.AddItem("Zirka");
	M4A4Skin.AddItem("Asiimov");
	M4A4Skin.AddItem("Howl");
	M4A4Skin.AddItem("X-Ray");
	M4A4Skin.AddItem("Desert-Strike");
	M4A4Skin.AddItem("Griffin");
	M4A4Skin.AddItem("Dragon King");
	M4A4Skin.AddItem("Poseidon");
	M4A4Skin.AddItem("Daybreak");
	M4A4Skin.AddItem("Evil Daimyo");
	M4A4Skin.AddItem("Royal Paladin");
	M4A4Skin.AddItem("The BattleStar");
	M4A4Skin.AddItem("Desolate Space");
	M4A4Skin.AddItem("Buzz Kill");
	Riflegroup.PlaceLabledControl("M4A4", this, &M4A4Skin);

	AUGSkin.SetFileId("aug_skin");
	AUGSkin.AddItem("Bengal Tiger");
	AUGSkin.AddItem("Hot Rod");
	AUGSkin.AddItem("Chameleon");
	AUGSkin.AddItem("Torque");
	AUGSkin.AddItem("Radiation Hazard");
	AUGSkin.AddItem("Asterion");
	AUGSkin.AddItem("Daedalus");
	AUGSkin.AddItem("Akihabara Accept");
	AUGSkin.AddItem("Ricochet");
	AUGSkin.AddItem("Fleet Flock");
	AUGSkin.AddItem("Syd Mead");
	Riflegroup.PlaceLabledControl("AUG", this, &AUGSkin);

	FAMASSkin.SetFileId("famas_skin");
	FAMASSkin.AddItem("Contrast Spray");
	FAMASSkin.AddItem("Colony");
	FAMASSkin.AddItem("Cyanospatter");
	FAMASSkin.AddItem("Djinn");
	FAMASSkin.AddItem("Afterimage");
	FAMASSkin.AddItem("Doomkitty");
	FAMASSkin.AddItem("Spitfire");
	FAMASSkin.AddItem("Teardown");
	FAMASSkin.AddItem("Hexane");
	FAMASSkin.AddItem("Pulse");
	FAMASSkin.AddItem("Sergeant");
	FAMASSkin.AddItem("Styx");
	FAMASSkin.AddItem("Neural Net");
	FAMASSkin.AddItem("Survivor");
	FAMASSkin.AddItem("Valence");
	FAMASSkin.AddItem("Roll Cage");
	FAMASSkin.AddItem("Mecha Industries");
	Riflegroup.PlaceLabledControl("FAMAS", this, &FAMASSkin);

	GALILSkin.SetFileId("galil_skin");
	GALILSkin.AddItem("Forest DDPAT");
	GALILSkin.AddItem("Contrast Spray");
	GALILSkin.AddItem("Orange DDPAT");
	GALILSkin.AddItem("Eco");
	GALILSkin.AddItem("Winter Forest");
	GALILSkin.AddItem("Sage Spray");
	GALILSkin.AddItem("VariCamo");
	GALILSkin.AddItem("VariCamo");
	GALILSkin.AddItem("Chatterbox");
	GALILSkin.AddItem("Shattered");
	GALILSkin.AddItem("Kami");
	GALILSkin.AddItem("Blue Titanium");
	GALILSkin.AddItem("Urban Rubble");
	GALILSkin.AddItem("Hunting Blind");
	GALILSkin.AddItem("Sandstorm");
	GALILSkin.AddItem("Tuxedo");
	GALILSkin.AddItem("Cerberus");
	GALILSkin.AddItem("Aqua Terrace");
	GALILSkin.AddItem("Rocket Pop");
	GALILSkin.AddItem("Stone Cold");
	GALILSkin.AddItem("Firefight");
	Riflegroup.PlaceLabledControl("GALIL", this, &GALILSkin);

	SG553Skin.SetFileId("sg552_skin");
	SG553Skin.AddItem("Bulldozer");
	SG553Skin.AddItem("Ultraviolet");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Pulse");
	SG553Skin.AddItem("Army Sheen");
	SG553Skin.AddItem("Traveler");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Cyrex");
	SG553Skin.AddItem("Tiger Moth");
	SG553Skin.AddItem("Atlas");
	Riflegroup.PlaceLabledControl("SG552", this, &SG553Skin);
#pragma endregion

#pragma region MPs
	MPGroup.SetPosition(16, 475);
	MPGroup.SetText("MPs");
	MPGroup.SetSize(376, 165);
	RegisterControl(&MPGroup);

	MAC10Skin.SetFileId("mac10_skin");
	MAC10Skin.AddItem("Tornado");
	MAC10Skin.AddItem("Candy Apple");
	MAC10Skin.AddItem("Silver");
	MAC10Skin.AddItem("Forest DDPAT");
	MAC10Skin.AddItem("Urban DDPAT");
	MAC10Skin.AddItem("Fade");
	MAC10Skin.AddItem("Neon Rider");
	MAC10Skin.AddItem("Ultraviolet");
	MAC10Skin.AddItem("Palm");
	MAC10Skin.AddItem("Graven");
	MAC10Skin.AddItem("Tatter");
	MAC10Skin.AddItem("Amber Fade");
	MAC10Skin.AddItem("Heat");
	MAC10Skin.AddItem("Curse");
	MAC10Skin.AddItem("Indigo");
	MAC10Skin.AddItem("Commuter");
	MAC10Skin.AddItem("Nuclear Garden");
	MAC10Skin.AddItem("Malachite");
	MAC10Skin.AddItem("Rangeen");
	MAC10Skin.AddItem("Lapis Gator");
	MPGroup.PlaceLabledControl("MAC-10", this, &MAC10Skin);

	P90Skin.SetFileId("p90_skin");
	P90Skin.AddItem("Leather");
	P90Skin.AddItem("Virus");
	P90Skin.AddItem("Contrast Spray");
	P90Skin.AddItem("Storm");
	P90Skin.AddItem("Cold Blooded");
	P90Skin.AddItem("Glacier Mesh");
	P90Skin.AddItem("Sand Spray");
	P90Skin.AddItem("Death by Kitty");
	P90Skin.AddItem("Ash Wood");
	P90Skin.AddItem("Fallout Warning");
	P90Skin.AddItem("Scorched");
	P90Skin.AddItem("Emerald Dragon");
	P90Skin.AddItem("Teardown");
	P90Skin.AddItem("Blind Spot");
	P90Skin.AddItem("Trigon");
	P90Skin.AddItem("Desert Warfare");
	P90Skin.AddItem("Module");
	P90Skin.AddItem("Asiimov");
	P90Skin.AddItem("Elite Build");
	P90Skin.AddItem("Shapewood");
	P90Skin.AddItem("Shallow Grave");
	MPGroup.PlaceLabledControl("P90", this, &P90Skin);

	UMP45Skin.SetFileId("ump45_skin");
	UMP45Skin.AddItem("Blaze");
	UMP45Skin.AddItem("Forest DDPAT");
	UMP45Skin.AddItem("Gunsmoke");
	UMP45Skin.AddItem("Urban DDPAT");
	UMP45Skin.AddItem("Grand Prix");
	UMP45Skin.AddItem("Carbon Fiber");
	UMP45Skin.AddItem("Caramel");
	UMP45Skin.AddItem("Fallout Warning");
	UMP45Skin.AddItem("Scorched");
	UMP45Skin.AddItem("Bone Pile");
	UMP45Skin.AddItem("Delusion");
	UMP45Skin.AddItem("Corporal");
	UMP45Skin.AddItem("Indigo");
	UMP45Skin.AddItem("Labyrinth");
	UMP45Skin.AddItem("Minotaur's Labyrinth");
	UMP45Skin.AddItem("Riot");
	UMP45Skin.AddItem("Primal Saber");
	MPGroup.PlaceLabledControl("UMP-45", this, &UMP45Skin);

	BIZONSkin.SetFileId("bizon_skin");
	BIZONSkin.AddItem("Blue Streak");
	BIZONSkin.AddItem("Modern Hunter");
	BIZONSkin.AddItem("Forest Leaves");
	BIZONSkin.AddItem("Bone Mask");
	BIZONSkin.AddItem("Carbon Fiber");
	BIZONSkin.AddItem("Sand Dashed");
	BIZONSkin.AddItem("Urban Dashed");
	BIZONSkin.AddItem("Brass");
	BIZONSkin.AddItem("VariCamo");
	BIZONSkin.AddItem("Irradiated Alert");
	BIZONSkin.AddItem("Rust Coat");
	BIZONSkin.AddItem("Water Sigil");
	BIZONSkin.AddItem("Night Ops");
	BIZONSkin.AddItem("Cobalt Halftone");
	BIZONSkin.AddItem("Antique");
	BIZONSkin.AddItem("Rust Coat");
	BIZONSkin.AddItem("Osiris");
	BIZONSkin.AddItem("Chemical Green");
	BIZONSkin.AddItem("Bamboo Print");
	BIZONSkin.AddItem("Bamboo Forest");
	BIZONSkin.AddItem("Fuel Rod");
	BIZONSkin.AddItem("Photic Zone");
	BIZONSkin.AddItem("Judgement of Anubis");
	MPGroup.PlaceLabledControl("PP-Bizon", this, &BIZONSkin);

	MP7Skin.SetFileId("mp7_skin");
	MP7Skin.AddItem("Groundwater");
	MP7Skin.AddItem("Whiteout");
	MP7Skin.AddItem("Forest DDPAT");
	MP7Skin.AddItem("Anodized Navy");
	MP7Skin.AddItem("Skulls");
	MP7Skin.AddItem("Gunsmoke");
	MP7Skin.AddItem("Contrast Spray");
	MP7Skin.AddItem("Bone Mask");
	MP7Skin.AddItem("Ossified");
	MP7Skin.AddItem("Orange Peel");
	MP7Skin.AddItem("VariCamo");
	MP7Skin.AddItem("Army Recon");
	MP7Skin.AddItem("Groundwater");
	MP7Skin.AddItem("Ocean Foam");
	MP7Skin.AddItem("Full Stop");
	MP7Skin.AddItem("Urban Hazard");
	MP7Skin.AddItem("Olive Plaid");
	MP7Skin.AddItem("Armor Core");
	MP7Skin.AddItem("Asterion");
	MP7Skin.AddItem("Nemesis");
	MP7Skin.AddItem("Special Delivery");
	MP7Skin.AddItem("Impire");
	MPGroup.PlaceLabledControl("MP7", this, &MP7Skin);

	MP9Skin.SetFileId("mp9_skin");
	MP9Skin.AddItem("Ruby Poison Dart");
	MP9Skin.AddItem("Bone Mask");
	MP9Skin.AddItem("Hot Rod");
	MP9Skin.AddItem("Storm");
	MP9Skin.AddItem("Bulldozer");
	MP9Skin.AddItem("Hypnotic");
	MP9Skin.AddItem("Sand Dashed");
	MP9Skin.AddItem("Orange Peel");
	MP9Skin.AddItem("Dry Season");
	MP9Skin.AddItem("Dark Age");
	MP9Skin.AddItem("Rose Iron");
	MP9Skin.AddItem("Green Plaid");
	MP9Skin.AddItem("Setting Sun");
	MP9Skin.AddItem("Dart");
	MP9Skin.AddItem("Deadly Poison");
	MP9Skin.AddItem("Pandora's Box");
	MP9Skin.AddItem("Bioleak");
	MP9Skin.AddItem("Airlock");
	MPGroup.PlaceLabledControl("MP9", this, &MP9Skin);

#pragma endregion

#pragma region Pistols
	PistolGroup.SetPosition(408, 270);
	PistolGroup.SetText("Pistols");
	PistolGroup.SetSize(360, 215);
	RegisterControl(&PistolGroup);

	GLOCKSkin.SetFileId("glock_skin");
	GLOCKSkin.AddItem("Groundwater");
	GLOCKSkin.AddItem("Candy Apple");
	GLOCKSkin.AddItem("Fade");
	GLOCKSkin.AddItem("Night");
	GLOCKSkin.AddItem("Dragon Tattoo");
	GLOCKSkin.AddItem("Twilight Galaxy");
	GLOCKSkin.AddItem("Sand Dune");
	GLOCKSkin.AddItem("Brass");
	GLOCKSkin.AddItem("Catacombs");
	GLOCKSkin.AddItem("Sand Dune");
	GLOCKSkin.AddItem("Steel Disruption");
	GLOCKSkin.AddItem("Blue Fissure");
	GLOCKSkin.AddItem("Death Rattle");
	GLOCKSkin.AddItem("Water Elemental");
	GLOCKSkin.AddItem("Reactor");
	GLOCKSkin.AddItem("Grinder");
	GLOCKSkin.AddItem("Bunsen Burner");
	GLOCKSkin.AddItem("Wraith");
	GLOCKSkin.AddItem("Royal Legion");
	GLOCKSkin.AddItem("Weasel");
	GLOCKSkin.AddItem("Wasteland Rebel");
	PistolGroup.PlaceLabledControl("Glock", this, &GLOCKSkin);

	USPSSkin.SetFileId("usps_skin");
	USPSSkin.AddItem("Forest Leaves");
	USPSSkin.AddItem("Dark Water");
	USPSSkin.AddItem("VariCamo");
	USPSSkin.AddItem("Overgrowth");
	USPSSkin.AddItem("Caiman");
	USPSSkin.AddItem("Blood Tiger");
	USPSSkin.AddItem("Serum");
	USPSSkin.AddItem("Night Ops");
	USPSSkin.AddItem("Stainless");
	USPSSkin.AddItem("Guardian");
	USPSSkin.AddItem("Orion");
	USPSSkin.AddItem("Road Rash");
	USPSSkin.AddItem("Royal Blue");
	USPSSkin.AddItem("Business Class");
	USPSSkin.AddItem("Para Green");
	USPSSkin.AddItem("Torque");
	USPSSkin.AddItem("Kill Confirmed");
	USPSSkin.AddItem("Lead Conduit");
	USPSSkin.AddItem("Cyrex");
	PistolGroup.PlaceLabledControl("USP-S", this, &USPSSkin);

	DEAGLESkin.SetFileId("deagle_skin");
	DEAGLESkin.AddItem("Blaze");
	DEAGLESkin.AddItem("Pilot");
	DEAGLESkin.AddItem("Midnight Storm");
	DEAGLESkin.AddItem("Sunset Storm");
	DEAGLESkin.AddItem("Forest DDPAT");
	DEAGLESkin.AddItem("Crimson Web");
	DEAGLESkin.AddItem("Urban DDPAT");
	DEAGLESkin.AddItem("Night");
	DEAGLESkin.AddItem("Hypnotic");
	DEAGLESkin.AddItem("Mudder");
	DEAGLESkin.AddItem("VariCamo");
	DEAGLESkin.AddItem("Golden Koi");
	DEAGLESkin.AddItem("Cobalt Disruption");
	DEAGLESkin.AddItem("Urban Rubble");
	DEAGLESkin.AddItem("Naga");
	DEAGLESkin.AddItem("Hand Cannon");
	DEAGLESkin.AddItem("Heirloom");
	DEAGLESkin.AddItem("Meteorite");
	DEAGLESkin.AddItem("Conspiracy");
	DEAGLESkin.AddItem("Bronze Deco");
	DEAGLESkin.AddItem("Sunset Storm");
	DEAGLESkin.AddItem("Corinthian");
	DEAGLESkin.AddItem("Kumicho Dragon");
	PistolGroup.PlaceLabledControl("Deagle", this, &DEAGLESkin);


	// Case 2
	DUALSSkin.SetFileId("duals_skin");
	DUALSSkin.AddItem("Anodized Navy");
	DUALSSkin.AddItem("Ossified");
	DUALSSkin.AddItem("Stained");
	DUALSSkin.AddItem("Contractor");
	DUALSSkin.AddItem("Colony");
	DUALSSkin.AddItem("Demolition");
	DUALSSkin.AddItem("Dualing Dragons");
	DUALSSkin.AddItem("Black Limba");
	DUALSSkin.AddItem("Red Quartz");
	DUALSSkin.AddItem("Cobalt Quartz");
	DUALSSkin.AddItem("Hemoglobin");
	DUALSSkin.AddItem("Urban Shock");
	DUALSSkin.AddItem("Marina");
	DUALSSkin.AddItem("Panther");
	DUALSSkin.AddItem("Retribution");
	DUALSSkin.AddItem("Briar");
	DUALSSkin.AddItem("Duelist");
	DUALSSkin.AddItem("Moon in Libra");
	DUALSSkin.AddItem("Cartel");
	DUALSSkin.AddItem("Ventilators");
	DUALSSkin.AddItem("Royal Consorts");
	PistolGroup.PlaceLabledControl("Duals", this, &DUALSSkin);

	FIVESEVENSkin.SetFileId("fiveseven_skin");
	FIVESEVENSkin.AddItem("Candy Apple");
	FIVESEVENSkin.AddItem("Bone Mask");
	FIVESEVENSkin.AddItem("Case Hardened");
	FIVESEVENSkin.AddItem("Contractor");
	FIVESEVENSkin.AddItem("Forest Night");
	FIVESEVENSkin.AddItem("Orange Peel");
	FIVESEVENSkin.AddItem("Jungle");
	FIVESEVENSkin.AddItem("Nitro");
	FIVESEVENSkin.AddItem("Red Quartz");
	FIVESEVENSkin.AddItem("Anodized Gunmetal");
	FIVESEVENSkin.AddItem("Nightshade");
	FIVESEVENSkin.AddItem("Silver Quartz");
	FIVESEVENSkin.AddItem("Kami");
	FIVESEVENSkin.AddItem("Copper Galaxy");
	FIVESEVENSkin.AddItem("Neon Kimono");
	FIVESEVENSkin.AddItem("Fowl Play");
	FIVESEVENSkin.AddItem("Hot Shot");
	FIVESEVENSkin.AddItem("Urban Hazard");
	FIVESEVENSkin.AddItem("Monkey Business");
	FIVESEVENSkin.AddItem("Retrobution");
	FIVESEVENSkin.AddItem("Triumvirate");
	PistolGroup.PlaceLabledControl("Five-Seven", this, &FIVESEVENSkin);

	TECNINESkin.SetFileId("tec9_skin");
	TECNINESkin.AddItem("Tornado");
	TECNINESkin.AddItem("Groundwater");
	TECNINESkin.AddItem("Forest DDPAT");
	TECNINESkin.AddItem("Terrace");
	TECNINESkin.AddItem("Urban DDPAT");
	TECNINESkin.AddItem("Ossified");
	TECNINESkin.AddItem("Hades");
	TECNINESkin.AddItem("Brass");
	TECNINESkin.AddItem("VariCamo");
	TECNINESkin.AddItem("Nuclear Threat");
	TECNINESkin.AddItem("Red Quartz");
	TECNINESkin.AddItem("Tornado");
	TECNINESkin.AddItem("Blue Titanium");
	TECNINESkin.AddItem("Army Mesh");
	TECNINESkin.AddItem("Titanium Bit");
	TECNINESkin.AddItem("Sandstorm");
	TECNINESkin.AddItem("Isaac");
	TECNINESkin.AddItem("Toxic");
	TECNINESkin.AddItem("Bamboo Forest");
	TECNINESkin.AddItem("Avalanche");
	TECNINESkin.AddItem("Jambiya");
	TECNINESkin.AddItem("Re-Entry");
	TECNINESkin.AddItem("Fuel Injector");
	PistolGroup.PlaceLabledControl("Tec-9", this, &TECNINESkin);

	P2000Skin.SetFileId("p2000_skin");
	P2000Skin.AddItem("Grassland Leaves");
	P2000Skin.AddItem("Silver");
	P2000Skin.AddItem("Granite Marbleized");
	P2000Skin.AddItem("Forest Leaves");
	P2000Skin.AddItem("Ossified");
	P2000Skin.AddItem("Handgun");
	P2000Skin.AddItem("Fade");
	P2000Skin.AddItem("Scorpion");
	P2000Skin.AddItem("Grassland");
	P2000Skin.AddItem("Corticera");
	P2000Skin.AddItem("Ocean Foam");
	P2000Skin.AddItem("Pulse");
	P2000Skin.AddItem("Amber Fade");
	P2000Skin.AddItem("Red FragCam");
	P2000Skin.AddItem("Chainmail");
	P2000Skin.AddItem("Coach Class");
	P2000Skin.AddItem("Ivory");
	P2000Skin.AddItem("Fire Elemental");
	P2000Skin.AddItem("Asterion");
	P2000Skin.AddItem("Pathfinder");
	P2000Skin.AddItem("Imperial");
	P2000Skin.AddItem("Oceanic");
	P2000Skin.AddItem("Imperial Dragon");
	PistolGroup.PlaceLabledControl("P2000", this, &P2000Skin);

	P250Skin.SetFileId("p250_skin");
	P250Skin.AddItem("Whiteout");
	P250Skin.AddItem("Metallic DDPAT");
	P250Skin.AddItem("Splash");
	P250Skin.AddItem("Gunsmoke");
	P250Skin.AddItem("Modern Hunter");
	P250Skin.AddItem("Bone Mask");
	P250Skin.AddItem("Boreal Forest");
	P250Skin.AddItem("Sand Dune");
	P250Skin.AddItem("Nuclear Threat");
	P250Skin.AddItem("Mehndi");
	P250Skin.AddItem("Facets");
	P250Skin.AddItem("Hive");
	P250Skin.AddItem("Muertos");
	P250Skin.AddItem("Steel Disruption");
	P250Skin.AddItem("Undertow");
	P250Skin.AddItem("Franklin");
	P250Skin.AddItem("Neon Kimono");
	P250Skin.AddItem("Supernova");
	P250Skin.AddItem("Contamination");
	P250Skin.AddItem("Cartel");
	P250Skin.AddItem("Valence");
	P250Skin.AddItem("Crimson Kimono");
	P250Skin.AddItem("Mint Kimono");
	P250Skin.AddItem("Wing Shot");
	P250Skin.AddItem("Asiimov");
	PistolGroup.PlaceLabledControl("P250", this, &P250Skin);

#pragma endregion
}

void CMiscTab::Setup()
{
	SetTitle("Misc");

#pragma region Other
	OtherGroup.SetPosition(408, 16);
	OtherGroup.SetSize(360, 450);
	OtherGroup.SetText("Other");
	RegisterControl(&OtherGroup);

	OtherChatSpam.SetFileId("otr_spam");
	OtherChatSpam.AddItem("Off");
	OtherChatSpam.AddItem("Namestealer");
	OtherChatSpam.AddItem("Regular");
	OtherChatSpam.AddItem("Interwebz");
	OtherChatSpam.AddItem("Disperse Name");
	OtherGroup.PlaceLabledControl("Chat Spam", this, &OtherChatSpam);

	OtherClantag.SetFileId("otr_spam");
	OtherClantag.AddItem("Off");
	OtherClantag.AddItem("pasteware");
	OtherClantag.AddItem("arduino.cc");
	OtherClantag.AddItem("envorial.win");
	OtherClantag.AddItem("Blank");
	OtherClantag.AddItem("Valve");
	OtherGroup.PlaceLabledControl("Custom Clantag", this, &OtherClantag);

	OtherTeamChat.SetFileId("otr_teamchat");
	OtherGroup.PlaceLabledControl("Team Chat Only", this, &OtherTeamChat);

	OtherChatDelay.SetFileId("otr_chatdelay");
	OtherChatDelay.SetBoundaries(0.1, 3.0);
	OtherChatDelay.SetValue(0.5);
	OtherGroup.PlaceLabledControl("Spam Delay", this, &OtherChatDelay);

	OtherAirStuck.SetFileId("otr_astuck");
	OtherGroup.PlaceLabledControl("Air Stuck", this, &OtherAirStuck);

	OtherSpectators.SetFileId("otr_speclist");
	OtherGroup.PlaceLabledControl("Spectators List", this, &OtherSpectators);

	OtherSafeMode.SetFileId("otr_safemode");
	OtherSafeMode.SetState(true);
	OtherGroup.PlaceLabledControl("Anti Untrusted", this, &OtherSafeMode);

#pragma endregion other random options

#pragma region FakeLag
	FakeLagGroup.SetPosition(16, 16);
	FakeLagGroup.SetSize(360, 60);
	FakeLagGroup.SetText("Fake Lag");
	RegisterControl(&FakeLagGroup);

	FakeLagEnable.SetFileId("fakelag_enable");
	FakeLagGroup.PlaceLabledControl("Enable", this, &FakeLagEnable);

	FakeLagChoke.SetFileId("fakelag_choke");
	FakeLagChoke.SetBoundaries(0, 16);
	FakeLagChoke.SetValue(0);
	FakeLagGroup.PlaceLabledControl("Choke-Factor", this, &FakeLagChoke);
#pragma endregion fakelag shit

#pragma region Strafing
	StrafingGroup.SetPosition(16, 86);
	StrafingGroup.SetSize(360, 150);
	StrafingGroup.SetText("Other");
	RegisterControl(&StrafingGroup);

	OtherAutoJump.SetFileId("otr_autojump");
	OtherAutoJump.AddItem("Off");
	OtherAutoJump.AddItem("Normal");
	StrafingGroup.PlaceLabledControl("Auto Jump", this, &OtherAutoJump);

	OtherAutoStrafe.SetFileId("otr_strafe");
	OtherAutoStrafe.AddItem("Off");
	OtherAutoStrafe.AddItem("Legit");
	OtherAutoStrafe.AddItem("Rage");
	StrafingGroup.PlaceLabledControl("Auto Strafer", this, &OtherAutoStrafe);

	OtherStrafeSpeed.SetFileId("acc_strafespeed");
	OtherStrafeSpeed.SetBoundaries(0.f, 10.f);
	OtherStrafeSpeed.SetValue(5.f);
	StrafingGroup.PlaceLabledControl("Strafe Speed", this, &OtherStrafeSpeed);

	OtherEdgeJump.SetFileId("otr_edgejump");
	StrafingGroup.PlaceLabledControl("Edge Jump", this, &OtherEdgeJump);
#pragma endregion

#pragma region Rank
	RankGroup.SetPosition(16, 246);
	RankGroup.SetSize(360, 55);
	RankGroup.SetText("Rank");
	RegisterControl(&RankGroup);

	RankRevealer.SetFileId("otr_rankreveal");
	RankGroup.PlaceLabledControl("Reveal Rank", this, &RankRevealer);
#pragma endregion
}

void CGUITab::Setup()
{
	SetTitle("GUI");

#pragma region colormenu

	ColorMain.SetPosition(16, 16);
	ColorMain.SetSize(360, 100);
	ColorMain.SetText("Borders");
	RegisterControl(&ColorMain);

	RMainMenuA.SetFileId("colormenu_rmma");
	RMainMenuA.SetBoundaries(0, 255);
	RMainMenuA.SetValue(75);
	ColorMain.PlaceLabledControl("R", this, &RMainMenuA);

	GMainMenuA.SetFileId("colormenu_gmma");
	GMainMenuA.SetBoundaries(0, 255);
	GMainMenuA.SetValue(72);
	ColorMain.PlaceLabledControl("G", this, &GMainMenuA);

	BMainMenuA.SetFileId("colormenu_bmma");
	BMainMenuA.SetBoundaries(0, 255);
	BMainMenuA.SetValue(72);
	ColorMain.PlaceLabledControl("B", this, &BMainMenuA);

#pragma endregion

#pragma region colortext

	ColorText.SetPosition(16, 126);
	ColorText.SetSize(360, 100);
	ColorText.SetText("Text 1");
	RegisterControl(&ColorText);

	RMenuText.SetFileId("colortext_rtext");
	RMenuText.SetBoundaries(0, 255);
	RMenuText.SetValue(140);
	ColorText.PlaceLabledControl("R", this, &RMenuText);

	GMenuText.SetFileId("colortext_gtext");
	GMenuText.SetBoundaries(0, 255);
	GMenuText.SetValue(255);
	ColorText.PlaceLabledControl("G", this, &GMenuText);

	BMenuText.SetFileId("colortext_btext");
	BMenuText.SetBoundaries(0, 255);
	BMenuText.SetValue(0);
	ColorText.PlaceLabledControl("B", this, &BMenuText);

#pragma endregion

#pragma region colorinner

	ColorInner.SetPosition(16, 236);
	ColorInner.SetSize(360, 120);
	ColorInner.SetText("Background");
	RegisterControl(&ColorInner);

	RInner.SetFileId("colorinner_rinner");
	RInner.SetBoundaries(0, 255);
	RInner.SetValue(60);
	ColorInner.PlaceLabledControl("R", this, &RInner);

	GInner.SetFileId("colorinner_ginner");
	GInner.SetBoundaries(0, 255);
	GInner.SetValue(60);
	ColorInner.PlaceLabledControl("G", this, &GInner);

	BInner.SetFileId("colorinner_binner");
	BInner.SetBoundaries(0, 255);
	BInner.SetValue(60);
	ColorInner.PlaceLabledControl("B", this, &BInner);

	AInner.SetFileId("colorinner_ainner");
	AInner.SetBoundaries(0, 255);
	AInner.SetValue(255);
	ColorInner.PlaceLabledControl("A", this, &AInner);

#pragma endregion

#pragma region colortabs

	ColorTabs.SetPosition(16, 366);
	ColorTabs.SetSize(360, 120);
	ColorTabs.SetText("Tabs");
	RegisterControl(&ColorTabs);

	RTabs.SetFileId("colortabs_rinner");
	RTabs.SetBoundaries(0, 255);
	RTabs.SetValue(60);
	ColorTabs.PlaceLabledControl("R", this, &RTabs);

	GTabs.SetFileId("colortabs_ginner");
	GTabs.SetBoundaries(0, 255);
	GTabs.SetValue(60);
	ColorTabs.PlaceLabledControl("G", this, &GTabs);

	BTabs.SetFileId("colortabs_binner");
	BTabs.SetBoundaries(0, 255);
	BTabs.SetValue(60);
	ColorTabs.PlaceLabledControl("B", this, &BTabs);

#pragma endregion

};

void Menu::SetupMenu()
{
	Window.Setup();

	GUI.RegisterWindow(&Window);
	GUI.BindWindow(VK_INSERT, &Window);
}

void Menu::DoUIFrame()
{
	// General Processing

	// If the "all filter is selected tick all the others
	if (Window.VisualsTab.FiltersAll.GetState())
	{
		Window.VisualsTab.FiltersC4.SetState(true);
		Window.VisualsTab.FiltersChickens.SetState(true);
		Window.VisualsTab.FiltersPlayers.SetState(true);
		Window.VisualsTab.FiltersWeapons.SetState(true);
	}

	GUI.Update();
	GUI.Draw();

}