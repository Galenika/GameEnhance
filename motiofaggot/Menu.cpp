#include "Menu.h"
#include "Controls.h"
#include "Hooks.h" // for the unload meme
#include "Interfaces.h"
#include "Gui.h"

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 690

MainWindow Menu::Window;


void SaveCustom()
{
	std::string savename = Menu::Window.MiscTab.SettingsName.getText();
	GUI.SaveWindowState(&Menu::Window, (savename + ".cfg"));
}


void LoadCustom()
{
	std::string loadname = Menu::Window.MiscTab.SettingsName.getText();
	GUI.LoadWindowState(&Menu::Window, (loadname + ".cfg"));
}




void UnLoadCallbk()
{
	//if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected()) {
	//	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ChangeName, NULL, NULL, NULL);
	//}
}

void SaveLegitCallbk()
{
	GUI.SaveWindowState(&Menu::Window, "legit.cfg");
}

void LoadLegitCallbk()
{
	GUI.LoadWindowState(&Menu::Window, "legit.cfg");
}

void SaveRageCallbk()
{
	GUI.SaveWindowState(&Menu::Window, "rage.cfg");
}

void LoadRageCallbk()
{
	GUI.LoadWindowState(&Menu::Window, "rage.cfg");
}

void PanicCallbk()
{
	DoUnload = true;
}

void KnifeApplyCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	Interfaces::Engine->ClientCmd_Unrestricted("cl_fullupdate");
	//bGlovesNeedUpdate = true;
}
void GloveApplyCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	Interfaces::Engine->ClientCmd_Unrestricted("cl_fullupdate");
	bGlovesNeedUpdate = true;
}

void MainWindow::Setup()
{
	//SetPosition(50, 50);
	SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetTitle("REDHEAD.PRO");

	
	RegisterTab(&RageBotTab);
	RegisterTab(&LegitBotTab);
	RegisterTab(&VisualsTab);
	RegisterTab(&MiscTab);
	RegisterTab(&SkinchangerTab);
	RegisterTab(&GlovesChanger);
	RegisterTab(&ColorsTab);


	RECT Client = GetClientArea();
	Client.bottom -= 15;

	LegitBotTab.Setup();
	RageBotTab.Setup();
	VisualsTab.Setup();
	MiscTab.Setup();
	SkinchangerTab.Setup();
	GlovesChanger.Setup();
	ColorsTab.Setup();


#pragma region Bottom Buttons
	//UnloadButton.SetText("Name Exploit");
	///UnloadButton.SetCallback(UnLoadCallbk);
	//UnloadButton.SetPosition(16, Client.bottom - 85);
	
	//SaveButton.SetText("Save Legit");
	//SaveButton.SetCallback(SaveLegitCallbk);
	//SaveButton.SetPosition(16, Client.bottom - 50);

	//LoadButton.SetText("Load Legit");
	//LoadButton.SetCallback(LoadLegitCallbk);
	//LoadButton.SetPosition(203, Client.bottom - 50);

	PanicButton.SetText("Panic Button (DEV)");
	PanicButton.SetCallback(PanicCallbk);
	PanicButton.SetPosition(16 + 100, Client.bottom - 100);

	//LoadButton1.SetText("Load Rage");
	//LoadButton1.SetCallback(LoadRageCallbk);
	//LoadButton1.SetPosition(577, Client.bottom - 50);
	
	//PanicButton.SetText("Panic Key");
	//PanicButton.SetCallback(PanicCallbk);
	//PanicButton.SetPosition(203, Client.bottom - 85);
	
	
	//LegitBotTab.RegisterControl(&UnloadButton);
	//RageBotTab.RegisterControl(&UnloadButton);
	//VisualsTab.RegisterControl(&UnloadButton);
	//MiscTab.RegisterControl(&UnloadButton);
	//SkinchangerTab.RegisterControl(&UnloadButton);
	//ColorsTab.RegisterControl(&UnloadButton);

	//LegitBotTab.RegisterControl(&SaveButton);
	//RageBotTab.RegisterControl(&SaveButton);
	//VisualsTab.RegisterControl(&SaveButton);
	//MiscTab.RegisterControl(&SaveButton);
	//SkinchangerTab.RegisterControl(&SaveButton);
	//ColorsTab.RegisterControl(&SaveButton);

	//LegitBotTab.RegisterControl(&LoadButton);
	//RageBotTab.RegisterControl(&LoadButton);
	//VisualsTab.RegisterControl(&LoadButton);
	//MiscTab.RegisterControl(&LoadButton);
	//SkinchangerTab.RegisterControl(&LoadButton);
	//ColorsTab.RegisterControl(&LoadButton);
	
	//LegitBotTab.RegisterControl(&SaveButton1);
	//RageBotTab.RegisterControl(&SaveButton1);
	//VisualsTab.RegisterControl(&SaveButton1);
	//MiscTab.RegisterControl(&SaveButton1);
	//SkinchangerTab.RegisterControl(&SaveButton1);
	//ColorsTab.RegisterControl(&SaveButton1);
	
	//LegitBotTab.RegisterControl(&LoadButton1);
	//RageBotTab.RegisterControl(&LoadButton1);
	//VisualsTab.RegisterControl(&LoadButton1);
	//MiscTab.RegisterControl(&LoadButton1);
	//SkinchangerTab.RegisterControl(&LoadButton1);
	//ColorsTab.RegisterControl(&LoadButton1);

	//LegitBotTab.RegisterControl(&PanicButton);
	//RageBotTab.RegisterControl(&PanicButton);
	//VisualsTab.RegisterControl(&PanicButton);
	MiscTab.RegisterControl(&PanicButton);
	//SkinchangerTab.RegisterControl(&PanicButton);
	//ColorsTab.RegisterControl(&PanicButton);
	
	//RADAR
	//Radar->SetPosition(Menu::Window.VisualsTab.OtherRadarXPosition.GetValue(), Menu::Window.VisualsTab.OtherRadarYPosition.GetValue());
//	Radar->SetSize(600, 600);
	//Radar->SetTitle("Skins");

	// Save and Load

#pragma endregion Setting up the settings buttons
}

void CRageBotTab::Setup()
{
	SetTitle("RAGEBOT");
	SetTitle2("RAGEBOT");

//	ActiveLabel.SetPosition(100, 16);
//	ActiveLabel.SetText("Active");
//	RegisterControl(&ActiveLabel);

//	Active.SetFileId("active");
//	Active.SetPosition(150, 16);
//	RegisterControl(&Active);

#pragma region Aimbot

	AimbotGroup.SetPosition(100, 20);
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetSize(376, 300); // 280
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl("Enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl("Auto Fire", this, &AimbotAutoFire);

	AimbotFov.SetFileId("aim_fov");
	AimbotFov.SetBoundaries(0, 360);
	AimbotFov.SetValue(0.f);
	AimbotGroup.PlaceLabledControl2("Aim FoV", this, &AimbotFov);

	AimbotPSilent.SetFileId("aim_psilent");
	AimbotPSilent.AddItem("Normal");
	AimbotPSilent.AddItem("Silent");
	AimbotPSilent.AddItem("p$ilent");
	AimbotGroup.PlaceLabledControl2("Aim Mode", this, &AimbotPSilent);

	AimbotAutoPistol.SetFileId("aim_autopistol");
	AimbotGroup.PlaceLabledControl("Auto Pistol", this, &AimbotAutoPistol);

	AccuracySpreadLimit.SetFileId("aim_aimstep");
	AimbotGroup.PlaceLabledControl("Spread Limit", this, &AccuracySpreadLimit);

	AimbotKeyPress.SetFileId("aim_usekey");
	AimbotGroup.PlaceLabledControl("Aim On Key", this, &AimbotKeyPress);

	AimbotKeyBind.SetFileId("aim_key");
	AimbotGroup.PlaceLabledControl2("Key Bind", this, &AimbotKeyBind);
	
	//AntiSpawnProtection.SetFileId("aim_antispawnprotection");
	//AimbotGroup.PlaceLabledControl("Spawn Protection", this, &AntiSpawnProtection);


#pragma endregion Aimbot Controls Get Setup in here

#pragma region Target
	TargetGroup.SetPosition(100, 370);
	TargetGroup.SetText("Target");
	TargetGroup.SetSize(376, 200);
	RegisterControl(&TargetGroup);

	TargetSelection.SetFileId("tgt_selection");
	TargetSelection.AddItem("Crosshair");
	TargetSelection.AddItem("Distance");
	TargetSelection.AddItem("Next Shoot");
	TargetGroup.PlaceLabledControl2("Selection Type", this, &TargetSelection);

	TargetHitbox.SetFileId("tgt_hitbox");
	TargetHitbox.AddItem("Head");
	TargetHitbox.AddItem("Neck");
	TargetHitbox.AddItem("Chest");
	TargetHitbox.AddItem("Stomach");
	TargetHitbox.AddItem("Smart");
	TargetGroup.PlaceLabledControl2("Hitbox", this, &TargetHitbox);

	TargetHitscan.SetFileId("tgt_hitscan");
	TargetHitscan.AddItem("Off"); //0
	TargetHitscan.AddItem("Low"); // 1
	TargetHitscan.AddItem("Medium"); // 2
	TargetHitscan.AddItem("Full Body"); // 3
	TargetGroup.PlaceLabledControl2("Multi HitBox", this, &TargetHitscan);


	TargetVisibleParts.SetFileId("acc_multipoint");
	TargetGroup.PlaceLabledControl("MultiPoint", this, &MultiPoint);

	TargetVisibleParts.SetFileId("acc_htiboxfix");
	TargetGroup.PlaceLabledControl("HitBox Fix", this, &HitBoxFix);

	PointScale.SetFileId("acc_psx");
	PointScale.SetBoundaries(0.01f, 1.f); PointScale.SetValue(0.50f);
	TargetGroup.PlaceLabledControl2("Aim Height", this, &PointScale);

	AccuracyMinimumDamage.SetFileId("acc_mindmg");
	AccuracyMinimumDamage.SetBoundaries(0.f, 100.f);
	AccuracyMinimumDamage.SetValue(0.f);
	TargetGroup.PlaceLabledControl2("Min Damage", this, &AccuracyMinimumDamage);


#pragma endregion Targetting controls 

#pragma region Accuracy


	AccuracyGroup.SetPosition(492, 280); //408 ,444   ; 408 48
	AccuracyGroup.SetText("Accuracy");
	AccuracyGroup.SetSize(360, 290);
	RegisterControl(&AccuracyGroup);
	AimType.SetFileId("typee");
	AimType.AddItem("Normal");
	AimType.AddItem("Beta");
	AccuracyGroup.PlaceLabledControl2("AimType", this, &AimType);
	AccuracyAutoWall.SetFileId("acc_awall");
	AccuracyGroup.PlaceLabledControl("Auto Wall", this, &AccuracyAutoWall);

	AccuracyRecoil.SetFileId("acc_norecoil");
	AccuracyGroup.PlaceLabledControl("No Recoil", this, &AccuracyRecoil);

	PerfectAccuracy.SetFileId("acc_paccuracy");
	AccuracyGroup.PlaceLabledControl("Velocity Adjustment", this, &PerfectAccuracy);

	EnginePred.SetFileId("rage_prediction");
	AccuracyGroup.PlaceLabledControl("Engine Prediction", this, &EnginePred);

	//Postion.SetFileId("acc_nspread");
	//AccuracyGroup.PlaceLabledControl("NoSpread Mode", this, &Postion);

	AccuracyAngleFix.SetFileId("acc_aaax");
	AccuracyGroup.PlaceLabledControl("Resolver [X]", this, &AccuracyAngleFix);

	AccuracyAngleFixY.SetFileId("acc_aaay");
	AccuracyAngleFixY.AddItem("OFF");//0
	AccuracyAngleFixY.AddItem("Basic");//1
	AccuracyAngleFixY.AddItem("Rotate");//2
	AccuracyAngleFixY.AddItem("Store Angle");//3
	AccuracyAngleFixY.AddItem("Brute");//4
	AccuracyGroup.PlaceLabledControl2("Resolver [Y]", this, &AccuracyAngleFixY);
	
	AccuracyAutoCrouch.SetFileId("acc_crouch");
	AccuracyGroup.PlaceLabledControl("Auto Crouch", this, &AccuracyAutoCrouch);

	AccuracyAutoScope.SetFileId("acc_scope");
	AccuracyGroup.PlaceLabledControl("Auto Scope", this, &AccuracyAutoScope);

	AccuracyAutoStop.SetFileId("acc_stop");
	AccuracyAutoStop.AddItem("OFF");
	AccuracyAutoStop.AddItem("Accuracy Stop");
	AccuracyAutoStop.AddItem("Full Stop");
	AccuracyGroup.PlaceLabledControl2("Auto Stop", this, &AccuracyAutoStop);

	//AccuracyAutoScope.SetFileId("acc_scope");
	//A/ccuracyGroup.PlaceLabledControl("Auto Scope", this, &AccuracyAutoStop);

	//AccuracySpreadLimit.SetFileId("acc_spreadlimon");
	//AccuracyGroup.PlaceLabledControl("Hitchance", this, &AimbotAimStep);

	//AccuracyMinimumSpread.SetFileId("acc_spreadlim");
	///AccuracyMinimumSpread.SetBoundaries(0.f, 5.0f);
	//AccuracyMinimumSpread.SetValue(1.5f);
	//AccuracyGroup.PlaceLabledControl("Hitchance Amount", this, &AccuracyMinimumSpread);





#pragma endregion  Accuracy controls get Setup in here

#pragma region AntiAim
	AntiAimGroup.SetPosition(492, 48);
	AntiAimGroup.SetText("HvH");
	AntiAimGroup.SetSize(360, 200);
	RegisterControl(&AntiAimGroup);

	AntiAimEnable.SetFileId("aa_enable");
	AntiAimGroup.PlaceLabledControl("Enable", this, &AntiAimEnable);

	AntiAimPitch.SetFileId("aa_x");
	AntiAimPitch.AddItem("None"); //0
	AntiAimPitch.AddItem("Emotion"); //1
	AntiAimPitch.AddItem("UP"); //2
	AntiAimPitch.AddItem("Jitter"); //3
	AntiAimPitch.AddItem("Fake UP (!)"); //6
	AntiAimPitch.AddItem("Fake UP Inversed (!)"); //7
	AntiAimPitch.AddItem("Silent Down (!)"); //9
	AntiAimPitch.AddItem("Silent UP (!)"); //10
	AntiAimPitch.AddItem("Zero"); //8
	AntiAimPitch.AddItem("Emotion - Zero"); //8
	AntiAimPitch.AddItem("Emotion - UP"); //8
	AntiAimPitch.AddItem("Fake Down"); //8
	AntiAimPitch.AddItem("Down"); //8

	AntiAimGroup.PlaceLabledControl2("Pitch", this, &AntiAimPitch);

	AntiAimYaw.SetFileId("aa_y");
	AntiAimYaw.AddItem("None");
	AntiAimYaw.AddItem("Backward");
	AntiAimYaw.AddItem("Fake Backward (Fake)");
	AntiAimYaw.AddItem("Fake Backward (Dynamic)");
	AntiAimYaw.AddItem("Sideways");
	AntiAimYaw.AddItem("Fake Jitter (Fake)");
	AntiAimYaw.AddItem("Fake Jitter (Dynamic)");
	AntiAimYaw.AddItem("Slow Spinbot");
	AntiAimYaw.AddItem("Fast Spinbot");
	AntiAimYaw.AddItem("Fast Spinbot (Fake)");
	AntiAimYaw.AddItem("Zero");
	AntiAimYaw.AddItem("Fake Zero");
	AntiAimYaw.AddItem("Slow Spinbot Left");
	AntiAimYaw.AddItem("Slow Spinbot Right");
	AntiAimYaw.AddItem("Fake Random");
	
	//AntiAimYaw.AddItem("Slow Spinbot");
	//AntiAimYaw.AddItem("Fast Spinbot");


	AntiAimGroup.PlaceLabledControl2("Yaw", this, &AntiAimYaw);
//	AntiAimRoll.SetFileId("aa_z");
//	AntiAimRoll.AddItem("None");
//	AntiAimRoll.AddItem("Left");
	//AntiAimRoll.AddItem("Right");

	EdgeAntiAim.SetFileId("aa_random");
	RealRandom.AddItem("Forward");
	RealRandom.AddItem("Backward");
	RealRandom.AddItem("Left");
	RealRandom.AddItem("Right");
	AntiAimGroup.PlaceLabledControl2("Real Random", this, &RealRandom);

	AtTargets.SetFileId("aa_attargets");
	AntiAimGroup.PlaceLabledControl("At Targets", this, &AtTargets);

	EdgeAntiAim.SetFileId("aa_edge");
	AntiAimGroup.PlaceLabledControl("Edge Anti-Aim", this, &EdgeAntiAim);



	Thirdaa.SetFileId("aa_thirdpersonaa");
	Thirdaa.AddItem("OFF");
	Thirdaa.AddItem("Normal AA");
	Thirdaa.AddItem("Fake AA");
	Thirdaa.AddItem("ALL");
	AntiAimGroup.PlaceLabledControl2("ThirdPerson Anti-Aim", this, &Thirdaa);
#pragma endregion  AntiAim controls get setup in here
}

void CLegitBotTab::Setup()
{
	SetTitle("LEGITBOT");
	SetTitle2("LEGITBOT");

	
}
// Hier bin ich
void CVisualTab::Setup()
{
	SetTitle("VISUALS");
	SetTitle2("VISUALS");
	//ActiveLabel.SetPosition(100, 16);
	//ActiveLabel.SetText("Active");
	//RegisterControl(&ActiveLabel);

//	Active.SetFileId("active");
//	Active.SetPosition(150, 16);
	//RegisterControl(&Active);

#pragma region Options
	OptionsGroup.SetText("ESP");
	OptionsGroup.SetPosition(100, 48);
	OptionsGroup.SetSize(193, 310);
	RegisterControl(&OptionsGroup);

	OptionsBox.SetFileId("opt_box");
	OptionsBox.AddItem("OFF");
	OptionsBox.AddItem("Normal");
	OptionsBox.AddItem("Bold");
	OptionsBox.AddItem("Corners");

	OptionsGroup.PlaceLabledControl2("Box Type", this, &OptionsBox);

	OptionsName.SetFileId("opt_name");
	OptionsGroup.PlaceLabledControl("Name", this, &OptionsName);

	OptionsHealth.SetFileId("opt_hp");
	OptionsHealth.AddItem("OFF");
	OptionsHealth.AddItem("Text");
	OptionsHealth.AddItem("Left Bar");
	OptionsHealth.AddItem("Right Bar");
	OptionsHealth.AddItem("Interwebz");
	OptionsGroup.PlaceLabledControl2("Health", this, &OptionsHealth);

	OptionsChams.SetFileId("opt_chams");
	OptionsChams.AddItem("Off");
	OptionsChams.AddItem("Normal");
	OptionsChams.AddItem("Flat");
	OptionsGroup.PlaceLabledControl2("Chams", this, &OptionsChams);


	OptionsWeapon.SetFileId("opt_weapon");
	OptionsWeapon.AddItem("Off");
	OptionsWeapon.AddItem("Text");
	OptionsWeapon.AddItem("Icons (WIP)");
	OptionsGroup.PlaceLabledControl2("Weapon", this, &OptionsWeapon);

	OptionsSkeleton.SetFileId("opt_bone");
	OptionsGroup.PlaceLabledControl("Skeleton", this, &OptionsSkeleton);

	OptionsGlow.SetState(false);

	HasC4.SetFileId("opt_c4carrier");
	OptionsGroup.PlaceLabledControl("Has C4", this, &HasC4);

	IsDefusing.SetFileId("opt_defusing");
	OptionsGroup.PlaceLabledControl("Is Defusing", this, &IsDefusing);

	HasDefuser.SetFileId("opt_HasDef");
	OptionsGroup.PlaceLabledControl("Has Defuser", this, &HasDefuser);

	IsScoped.SetFileId("opt_scoped");
	OptionsGroup.PlaceLabledControl("Is Scoped", this, &IsScoped);

	//OptionsChams.SetFileId("opt_chams");
	//OptionsChams.AddItem("Off");
	//OptionsChams.AddItem("Normal");
	//OptionsChams.AddItem("Flat");
	//OptionsGroup.PlaceLabledControl("Chams", this, &OptionsChams);



	//OptionsAimSpot.SetFileId("opt_aimspot");
	//OptionsGroup.PlaceLabledControl("Aim Spot", this, &OptionsAimSpot);

	//OptionsDilights.SetFileId("opt_dilights");
	//OptionsGroup.PlaceLabledControl("Dilights", this, &OptionsDilights);



	//GlowZ.SetFileId("GlowZ");
	//GlowZ.SetBoundaries(0.f, 255.f);
	GlowZ.SetValue(244.f);
	//OptionsGroup.PlaceLabledControl("Glow Opacity", this, &GlowZ);


#pragma endregion Setting up the Options controls

#pragma region Filters
	FiltersGroup.SetText("Filters");
	FiltersGroup.SetPosition(309, 48);
	FiltersGroup.SetSize(193, 310);
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


#pragma region VisualMiscs
	VisualMiscGroup.SetText("Others");
	VisualMiscGroup.SetPosition(518, 48);
	VisualMiscGroup.SetSize(334, 310);
	RegisterControl(&VisualMiscGroup);

	OtherCrosshairType.SetFileId("otr_crosstype");

	VisualMiscGroup.PlaceLabledControl("Custom Crosshair", this, &OtherCrosshairType);//1

	OtherCrosshair.SetFileId("otr_cross");
	VisualMiscGroup.PlaceLabledControl("Recoil Crosshair", this, &OtherCrosshair);//1

	OtherNoVisualRecoil.SetFileId("otr_visrecoil");
	VisualMiscGroup.PlaceLabledControl("No Visual Recoil", this, &OtherNoVisualRecoil);

	OtherNoHands.SetFileId("otr_hands");
	OtherNoHands.AddItem("Off");
	OtherNoHands.AddItem("None");
	OtherNoHands.AddItem("Chams");
	OtherNoHands.AddItem("Wireframe");
	VisualMiscGroup.PlaceLabledControl2("Hands", this, &OtherNoHands);

	OtherRadar.SetFileId("otr_radar");
	VisualMiscGroup.PlaceLabledControl("In-Game RadarHack", this, &OtherRadar);

	OtherNoFlash.SetFileId("otr_noflash");
	VisualMiscGroup.PlaceLabledControl("Anty Flash", this, &OtherNoFlash);

	OtherNoFlashValue.SetFileId("otr_noflashvalue");
	OtherNoFlashValue.SetBoundaries(0, 255);
	OtherNoFlashValue.SetValue(255);
	VisualMiscGroup.PlaceLabledControl2("Remove Value", this, &OtherNoFlashValue);

	ViewChanger.SetFileId("otr_fovchanger");
	VisualMiscGroup.PlaceLabledControl("Viewmodel Changer", this, &ViewChanger);

	FovChanger.SetFileId("otr_fovchangerv");
	FovChanger.SetBoundaries(0.f, 180.f);
	FovChanger.SetValue(90.0f);
	VisualMiscGroup.PlaceLabledControl2("Value", this, &FovChanger);

//	OtherRadarEnemyOnly.SetFileId("otr_radar_enemy_only");
//	VisualMiscGroup.PlaceLabledControl("Radar Enemy Only", this, &OtherRadarEnemyOnly);

//	OtherRadarVisibleOnly.SetFileId("otr_radar_visible_only");
//	VisualMiscGroup.PlaceLabledControl("Radar Visible Only", this, &OtherRadarVisibleOnly);

//	OtherRadarXPosition.SetFileId("otr_radar_position_x");
//	OtherRadarYPosition.SetFileId("otr_radar_position_y");

//	OtherRadarScale.SetFileId("otr_radar_scale");
//	OtherRadarScale.SetBoundaries(0, 10000);
//	OtherRadarScale.SetValue(10000);
//	VisualMiscGroup.PlaceLabledControl("Radar Range", this, &OtherRadarScale);

#pragma endregion Setting up the VisualMiscs controls
}

void CMiscTab::Setup()
{
	SetTitle("MISC");
	SetTitle2("MISC");
#pragma region Other
	AntiUtgroup.SetText("Restrictions");
	AntiUtgroup.SetPosition(100, 48);
	AntiUtgroup.SetSize(376, 60);
	RegisterControl(&AntiUtgroup);


	AutoStrafeGroup.SetText("BunnyHop");
	AutoStrafeGroup.SetPosition(492, 48);
	AutoStrafeGroup.SetSize(350, 140);
	RegisterControl(&AutoStrafeGroup);
	OtherSafeMode.SetFileId("otr_safemode");
	OtherSafeMode.SetState(true);
	AntiUtgroup.PlaceLabledControl("Anti Untrusted", this, &OtherSafeMode);


	OtherBunnyhop.SetFileId("otr_autojump");
	AutoStrafeGroup.PlaceLabledControl("Auto Jump", this, &OtherBunnyhop);

	OtherAutoStrafe.SetFileId("otr_autostrafe");
	OtherAutoStrafe.AddItem("OFF");
	OtherAutoStrafe.AddItem("Normal");
	OtherAutoStrafe.AddItem("Rage");
	AutoStrafeGroup.PlaceLabledControl2("Auto Strafe", this, &OtherAutoStrafe);

	OtherCircleStrafe.SetFileId("otr_circlestrafe");
	AutoStrafeGroup.PlaceLabledControl("Circle Strafe", this, &OtherCircleStrafe);

	OtherCircleStrafeKey.SetFileId("otr_circlestrafeKey");
	AutoStrafeGroup.PlaceLabledControl2("Circle Strafe Key", this, &OtherCircleStrafeKey);

	ChatSpammgr.SetText("Chat Settings");
	ChatSpammgr.SetPosition(100, 120);
	ChatSpammgr.SetSize(376, 68);
	RegisterControl(&ChatSpammgr);
	
	OtherChatSpam.SetFileId("otr_spam");
	OtherChatSpam.AddItem("OFF");//0
	OtherChatSpam.AddItem("MotionSense");//1

	ChatSpammgr.PlaceLabledControl2("Chat Spam", this, &OtherChatSpam);


#pragma endregion Setting up the Other controls



#pragma region Fakelag
	Settings.SetText("Settings");
	Settings.SetPosition(492, 299);
	Settings.SetSize(350, 160);
	RegisterControl(&Settings);
	Settings.PlaceLabledControl2("Config Name", this, &SettingsName);
	SettingsSave.SetPosition(468 + 84, 360);
	SettingsSave.SetCallback(SaveCustom);
	SettingsSave.SetText("Save Config");
	SettingsLoad.SetPosition(468 + 84, 390);
	SettingsLoad.SetCallback(LoadCustom);
	SettingsLoad.SetText("Load Config");
	RegisterControl(&SettingsSave);
	RegisterControl(&SettingsLoad);


	FakeLagGroup.SetText("ClanTag Manager");
	FakeLagGroup.SetPosition(492, 200);
	FakeLagGroup.SetSize(350, 85);
	RegisterControl(&FakeLagGroup);
	ClanTag.SetFileId("otr_clantag");
	ClanTag.AddItem("OFF");
	ClanTag.AddItem("None");
	ClanTag.AddItem("MotionSense");
	ClanTag.AddItem("Valve");
	ClanTag.AddItem("STAINLESS");
	ClanTag.AddItem("UNTRA$TED");
	ClanTag.AddItem("pBAIM");
	ClanTag.AddItem("Uff Yaa $");
	ClanTag.AddItem("Bloodgodz");
	FakeLagGroup.PlaceLabledControl2("Clan Tag", this, &ClanTag);


	NameChangerGruup.SetText("NameChanger");
	NameChangerGruup.SetPosition(100, 200);
	NameChangerGruup.SetSize(376, 85);
	RegisterControl(&NameChangerGruup);

	OtherAimware.SetFileId("otr_namechanger");
	OtherAimware.AddItem("OFF");
	OtherAimware.AddItem("MotionSense");
	OtherAimware.AddItem("Stealer");

	NameChangerGruup.PlaceLabledControl2("Name Changer", this, &OtherAimware);

	NameTimes.SetFileId("otr_times");
	NameTimes.SetBoundaries(0, 5);
	NameTimes.SetValue(4);
	NameChangerGruup.PlaceLabledControl2("Change Times", this, &NameTimes);

	OtherClowns.SetFileId("otr_silent");
	NameChangerGruup.PlaceLabledControl("Silent (NameChanger FIX)", this, &OtherClowns);

	RestricGroup.SetText("Other");
	RestricGroup.SetPosition(100, 298);
	RestricGroup.SetSize(376, 160);
	RegisterControl(&RestricGroup);
	
#pragma endregion Fakelagend


#pragma end region


    OtherAirStuck.SetFileId("otr_astuck");
	RestricGroup.PlaceLabledControl2("Air Stuck", this, &OtherAirStuck);

	OtherBypass.SetFileId("otr_bypass");
	RestricGroup.PlaceLabledControl("sv_cheats 1", this, &OtherBypass);

	//OtherTeleportHack.SetFileId("otr_teleporthack");
	//OtherGroup.PlaceLabledControl("Teleport", this, &OtherTeleportHack);

//	OtherSpectators.SetFileId("otr_speclist");
//	OtherGroup.PlaceLabledControl("Spectators List", this, &OtherSpectators);

	OtherThirdperson.SetFileId("otr_thirdperson");
	RestricGroup.PlaceLabledControl("Remove Scope", this, &OtherThirdperson);//

	OtherFakeLag.SetFileId("otr_fakelag");
	RestricGroup.PlaceLabledControl2("FakeLag", this, &OtherFakeLag);
	OtherFakeLag.SetBoundaries(0, 20);
	OtherFakeLag.SetValue(0);

}

void CColorsTab::Setup()
{
	SetTitle("COLORS");
	SetTitle2("COLORS");
//
#pragma region Visual Colors
	ColorsGroup.SetPosition(100, 48);
	ColorsGroup.SetText("Visual Colors");
	ColorsGroup.SetSize(360, 500);
	RegisterControl(&ColorsGroup);

	CTColorVisR.SetFileId("ct_color_vis_r");
	CTColorVisR.SetBoundaries(0.f, 255.f);
	CTColorVisR.SetValue(120.f);
	ColorsGroup.PlaceLabledControl2("CT Visible R", this, &CTColorVisR);

	CTColorVisG.SetFileId("ct_color_vis_g");
	CTColorVisG.SetBoundaries(0.f, 255.f);
	CTColorVisG.SetValue(210.f);
	ColorsGroup.PlaceLabledControl2("CT Visible G", this, &CTColorVisG);

	CTColorVisB.SetFileId("ct_color_vis_b");
	CTColorVisB.SetBoundaries(0.f, 255.f);
	CTColorVisB.SetValue(26.f);
	ColorsGroup.PlaceLabledControl2("CT Visible B", this, &CTColorVisB);

	CTColorNoVisR.SetFileId("ct_color_no_vis_r");
	CTColorNoVisR.SetBoundaries(0.f, 255.f);
	CTColorNoVisR.SetValue(15.f);
	ColorsGroup.PlaceLabledControl2("CT Not Visible R", this, &CTColorNoVisR);

	CTColorNoVisG.SetFileId("ct_color_no_vis_g");
	CTColorNoVisG.SetBoundaries(0.f, 255.f);
	CTColorNoVisG.SetValue(110.f);
	ColorsGroup.PlaceLabledControl2("CT Not Visible G", this, &CTColorNoVisG);

	CTColorNoVisB.SetFileId("ct_color_no_vis_b");
	CTColorNoVisB.SetBoundaries(0.f, 255.f);
	CTColorNoVisB.SetValue(220.f);
	ColorsGroup.PlaceLabledControl2("CT Not Visible B", this, &CTColorNoVisB);

	TColorVisR.SetFileId("t_color_vis_r");
	TColorVisR.SetBoundaries(0.f, 255.f);
	TColorVisR.SetValue(235.f);
	ColorsGroup.PlaceLabledControl2("T Visible R", this, &TColorVisR);

	TColorVisG.SetFileId("t_color_vis_g");
	TColorVisG.SetBoundaries(0.f, 255.f);
	TColorVisG.SetValue(200.f);
	ColorsGroup.PlaceLabledControl2("T Visible G", this, &TColorVisG);

	TColorVisB.SetFileId("t_color_vis_b");
	TColorVisB.SetBoundaries(0.f, 255.f);
	TColorVisB.SetValue(0.f);
	ColorsGroup.PlaceLabledControl2("T Visible B", this, &TColorVisB);


	TColorNoVisR.SetFileId("t_color_no_vis_r");
	TColorNoVisR.SetBoundaries(0.f, 255.f);
	TColorNoVisR.SetValue(235.f);
	ColorsGroup.PlaceLabledControl2("T Not Visible R", this, &TColorNoVisR);

	TColorNoVisG.SetFileId("t_color_no_vis_g");
	TColorNoVisG.SetBoundaries(0.f, 255.f);
	TColorNoVisG.SetValue(50.f);
	ColorsGroup.PlaceLabledControl2("T Not Visible G", this, &TColorNoVisG);

	TColorNoVisB.SetFileId("t_color_no_vis_b");
	TColorNoVisB.SetBoundaries(0.f, 255.f);
	TColorNoVisB.SetValue(0.f);
	ColorsGroup.PlaceLabledControl2("T Not Visible B", this, &TColorNoVisB);

	MenuR.SetFileId("MenuR");
	MenuR.SetBoundaries(0.f, 255.f);
	MenuR.SetValue(0.f);
	ColorsGroup.PlaceLabledControl2("Menu R", this, &MenuR);

	MenuG.SetFileId("MenuG");
	MenuG.SetBoundaries(0.f, 255.f);
	MenuG.SetValue(150.f);
	ColorsGroup.PlaceLabledControl2("Menu G", this, &MenuG);

	MenuB.SetFileId("MenuB");
	MenuB.SetBoundaries(0.f, 255.f);
	MenuB.SetValue(255.f);
	ColorsGroup.PlaceLabledControl2("Menu B", this, &MenuB);

	MenutitleR.SetFileId("MenuRTitle");
	MenutitleR.SetBoundaries(0.f, 255.f);
	MenutitleR.SetValue(255.f);
	ColorsGroup.PlaceLabledControl2("Menu Title R", this, &MenutitleR);

	MenutitleG.SetFileId("MenuGitle");
	MenutitleG.SetBoundaries(0.f, 255.f);
	MenutitleG.SetValue(255.f);
	ColorsGroup.PlaceLabledControl2("Menu Title G", this, &MenutitleG);

	MenutitleB.SetFileId("MenuBitle");
	MenutitleB.SetBoundaries(0.f, 255.f);
	MenutitleB.SetValue(255.f);
	ColorsGroup.PlaceLabledControl2("Menu Title B", this, &MenutitleB);




#pragma endregion Visual Colors
}
void CGloveschanger::Setup()
{
	SetTitle2("GLOVES");
		SetTitle("GLOVES");
		GloveActive.SetPosition(100, 16);
		GloveActive.SetText("Active");
		RegisterControl(&GloveActive);

		GloveEnable.SetFileId("glove_enable");
		GloveEnable.SetPosition(150, 16);
		RegisterControl(&GloveEnable);

		GloveApply.SetText("Apply Changes");
		GloveApply.SetCallback(GloveApplyCallbk);
		GloveApply.SetPosition(408 + 84, 10);
		GloveApply.SetSize(360, 106);
		RegisterControl(&GloveApply);

#pragma region Knife
		GloveGroup.SetPosition(16 + 84, 48);
		GloveGroup.SetText("Gloves");
		GloveGroup.SetSize(376, 80);
		RegisterControl(&GloveGroup);

		GloveModel.SetFileId("glove_model");
		GloveModel.AddItem("OFF");
		GloveModel.AddItem("BloodHound");
		GloveModel.AddItem("HandWrap");
		GloveModel.AddItem("Driver");
		GloveModel.AddItem("Sport");
		GloveModel.AddItem("Motorcycle");
		GloveModel.AddItem("Specialist");
		GloveGroup.PlaceLabledControl2("Glove Model", this, &GloveModel);

		GloveSkin.SetFileId("glove_skin");
		GloveSkin.AddItem("None");
		GloveSkin.AddItem("BloodHound - Snakebite"); //10007
		GloveSkin.AddItem("BloodHound - Charred");// 10006
		GloveSkin.AddItem("BloodHound - Guerrilla");// 10039
		GloveSkin.AddItem("BloodHound - Bronzed");// 10008
		GloveSkin.AddItem("HandWrap - Slaughter"); // 10021
		GloveSkin.AddItem("HandWrap - Badlands");// 10036
		GloveSkin.AddItem("HandWrap - Leather");// 10009
		GloveSkin.AddItem("HandWrap - Spruce DDPAT");// 10010
		GloveSkin.AddItem("Driver - Crimson Weave");// 10016
		GloveSkin.AddItem("Driver - Lunar Weave");// 10013
		GloveSkin.AddItem("Driver - Diamondback");// 10040
		GloveSkin.AddItem("Driver - Convoy");// 10015
		GloveSkin.AddItem("Sport - Pandora's Box");// 10037
		GloveSkin.AddItem("Sport - Hedge Maze");// 10038
		GloveSkin.AddItem("Sport - Superconductor");// 10018
		GloveSkin.AddItem("Sport - Arid");// 10019
		GloveSkin.AddItem("Motorcycle - Spearmint");// 10026
		GloveSkin.AddItem("Motorcycle - Cool Mint");// 10028
		GloveSkin.AddItem("Motorcycle - Boom");// 10027
		GloveSkin.AddItem("Motorcycle - Eclipse");// 10024
		GloveSkin.AddItem("Specialist - Crimson Kimono");// 10033
		GloveSkin.AddItem("Specialist - Emerald Web");// 10034
		GloveSkin.AddItem("Specialist - Foundation");// 10035
		GloveSkin.AddItem("Specialist - Forest DDPAT");// 10030
		GloveGroup.PlaceLabledControl2("Skin", this, &GloveSkin);
#pragma endregion
}

void CSkinchangerTab::Setup()
{
	SetTitle("SKINS");
	SetTitle2("SKINS");
	SkinActive.SetPosition(100, 16);
	SkinActive.SetText("Active");
	RegisterControl(&SkinActive);

	SkinEnable.SetFileId("skin_enable");
	SkinEnable.SetPosition(150, 16);
	RegisterControl(&SkinEnable);

	SkinApply.SetText("Apply Changes");
	SkinApply.SetCallback(KnifeApplyCallbk);
	SkinApply.SetPosition(408 + 84, 10);
	SkinApply.SetSize(360, 106);
	RegisterControl(&SkinApply);

#pragma region Knife
	KnifeGroup.SetPosition(16 + 84, 48);
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
	KnifeGroup.PlaceLabledControl2("Knife", this, &KnifeModel);

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
	KnifeGroup.PlaceLabledControl2("Skin", this, &KnifeSkin);
#pragma endregion

#pragma region Machineguns
	MachinegunsGroup.SetPosition(408 + 84, 48);
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
	MachinegunsGroup.PlaceLabledControl2("Negev", this, &NEGEVSkin);

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
	MachinegunsGroup.PlaceLabledControl2("M249", this, &M249Skin);

#pragma endregion

#pragma region Snipers
	Snipergroup.SetPosition(16 + 84, 135);
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
	Snipergroup.PlaceLabledControl2("AWP", this, &AWPSkin);

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
	Snipergroup.PlaceLabledControl2("SGG 08", this, &SSG08Skin);

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
	Snipergroup.PlaceLabledControl2("SCAR-20", this, &SCAR20Skin);

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
	Snipergroup.PlaceLabledControl2("G3SG1", this, &G3SG1Skin);
#pragma endregion

#pragma region Shotguns
	Shotgungroup.SetPosition(408 + 84, 135);
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
	Shotgungroup.PlaceLabledControl2("Mag-7", this, &MAG7Skin);

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
	Shotgungroup.PlaceLabledControl2("XM1014", this, &XM1014Skin);

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
	Shotgungroup.PlaceLabledControl2("Sawed-Off", this, &SAWEDOFFSkin);

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
	Shotgungroup.PlaceLabledControl2("Nova", this, &NOVASkin);
#pragma endregion

#pragma region Rifles
	Riflegroup.SetPosition(16 + 84, 270);
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
	Riflegroup.PlaceLabledControl2("AK-47", this, &AK47Skin);

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
	Riflegroup.PlaceLabledControl2("M4A1-S", this, &M41SSkin);

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
	Riflegroup.PlaceLabledControl2("M4A4", this, &M4A4Skin);

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
	Riflegroup.PlaceLabledControl2("AUG", this, &AUGSkin);

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
	Riflegroup.PlaceLabledControl2("FAMAS", this, &FAMASSkin);

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
	Riflegroup.PlaceLabledControl2("GALIL", this, &GALILSkin);

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
	Riflegroup.PlaceLabledControl2("SG553", this, &SG553Skin);
#pragma endregion

#pragma region MPs
	MPGroup.SetPosition(16 + 84, 475);
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
	MPGroup.PlaceLabledControl2("MAC-10", this, &MAC10Skin);

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
	MPGroup.PlaceLabledControl2("P90", this, &P90Skin);

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
	MPGroup.PlaceLabledControl2("UMP-45", this, &UMP45Skin);

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
	MPGroup.PlaceLabledControl2("PP-Bizon", this, &BIZONSkin);

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
	MPGroup.PlaceLabledControl2("MP7", this, &MP7Skin);

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
	MPGroup.PlaceLabledControl2("MP9", this, &MP9Skin);

#pragma endregion

#pragma region Pistols
	PistolGroup.SetPosition(408 + 84, 270);
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
	PistolGroup.PlaceLabledControl2("Glock", this, &GLOCKSkin);

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
	PistolGroup.PlaceLabledControl2("USP-S", this, &USPSSkin);

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
	PistolGroup.PlaceLabledControl2("Deagle", this, &DEAGLESkin);

	DUALSSkin.SetFileId("duals_skin");
	DUALSSkin.AddItem("Anodized Navy");
	DUALSSkin.AddItem("Ossified");
	DUALSSkin.AddItem("Stained");
	DUALSSkin.AddItem("Contractor");
	DUALSSkin.AddItem("Colony");
	DUALSSkin.AddItem("Demolition");
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
	DUALSSkin.AddItem("Dualing Dragons");
	DUALSSkin.AddItem("Cartel");
	DUALSSkin.AddItem("Ventilators");
	PistolGroup.PlaceLabledControl2("Duals", this, &DUALSSkin);

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
	PistolGroup.PlaceLabledControl2("Five-Seven", this, &FIVESEVENSkin);

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
	PistolGroup.PlaceLabledControl2("Tec-9", this, &TECNINESkin);

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
	PistolGroup.PlaceLabledControl2("P2000", this, &P2000Skin);

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
	PistolGroup.PlaceLabledControl2("P250", this, &P250Skin);

#pragma endregion

#pragma region Skinsettings
	//SkinsettingsGroup.SetPosition(408, 500);
	//SkinsettingsGroup.SetText("Settings");
	//SkinsettingsGroup.SetSize(360, 140);
	//RegisterControl(&SkinsettingsGroup);

	//StatTrakEnable.SetFileId("skin_stattrack");
	//SkinsettingsGroup.PlaceLabledControl("Stat Track", this, &StatTrakEnable);

	//StatTrackAmount.SetFileId("skin_stamount");
	//SkinsettingsGroup.PlaceLabledControl("Value", this, &StatTrackAmount);

	//KnifeName.SetFileId("knife_name");
	//SkinsettingsGroup.PlaceLabledControl("Knife Name", this, &KnifeName);

	//SkinName.SetFileId("skin_name");
	//SkinsettingsGroup.PlaceLabledControl("Skin Name", this, &SkinName);

#pragma endregion

#pragma endregion other random options
}

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
		Window.VisualsTab.FiltersEnemiesOnly.SetState(false);
		Window.VisualsTab.FiltersChickens.SetState(true);
		Window.VisualsTab.FiltersPlayers.SetState(true);
		Window.VisualsTab.FiltersWeapons.SetState(true);
	}

	GUI.Update();
	GUI.Draw();


}