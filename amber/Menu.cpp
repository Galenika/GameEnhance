

#include "Menu.h"
#include "Controls.h"
#include "Hooks.h" // for the unload meme
#include "Interfaces.h"

#define WINDOW_WIDTH 618
#define WINDOW_HEIGHT 595

AyyWareWindow Menu::Window;

void SaveCallbk()
{
	GUI.SaveWindowState(&Menu::Window, XorStr("config.xml"));
}

void LoadCallbk()
{
	GUI.LoadWindowState(&Menu::Window, XorStr("config.xml"));
}

void UnLoadCallbk()
{
	DoUnload = true;
}

void KnifeApplyCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar(XorStr("cl_fullupdate"));
	Meme->nFlags &= ~FCVAR_CHEAT;
	Interfaces::Engine->ClientCmd_Unrestricted(XorStr("cl_fullupdate"));
}

void GloveApplyCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar(XorStr("cl_fullupdate"));
	Meme->nFlags &= ~FCVAR_CHEAT;
	Interfaces::Engine->ClientCmd_Unrestricted(XorStr("cl_fullupdate"));
	bGlovesNeedUpdate = true;
}

void SpoofConvars()
{
	auto svcheats = Interfaces::CVar->FindVar(XorStr("sv_cheats"));
	auto svcheatsspoof = new SpoofedConvar(svcheats);
	svcheatsspoof->SetInt(1);
}

void AyyWareWindow::Setup()
{
	//SpoofConvars();

	SetPosition(80, 49);
	SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetTitle(XorStr("pPaste"));

	RegisterTab(&RageBotTab);
	RegisterTab(&LegitBotTab);
	RegisterTab(&VisualsTab);
	RegisterTab(&MiscTab);
	RegisterTab(&ColorsTab);
	RECT Client = GetClientArea();
	Client.bottom -= 29;

	RageBotTab.Setup();
	LegitBotTab.Setup();
	VisualsTab.Setup();
	MiscTab.Setup();
	ColorsTab.Setup();
}

void CRageBotTab::Setup()
{
	SetTitle(XorStr("R"));

#pragma region Aimbot

	AimbotGroup.SetPosition(16, 18);
	AimbotGroup.SetText(XorStr("Aimbot"));
	AimbotGroup.SetSize(276, 411);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl(XorStr("Active"), this, &AimbotEnable);

	TargetSelection.SetFileId("tgt_selection");
	TargetSelection.AddItem(XorStr("FoV"));
	TargetSelection.AddItem(XorStr("Distance"));
	TargetSelection.AddItem(XorStr("Threat"));
	AimbotGroup.PlaceLabledControl(XorStr("Target Selection"), this, &TargetSelection);

	TargetFriendlyFire.SetFileId("tgt_friendlyfire");
	AimbotGroup.PlaceLabledControl(XorStr("Friendly Fire"), this, &TargetFriendlyFire);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl(XorStr("Auto Fire"), this, &AimbotAutoFire);

	//AimbotKeyBind.SetFileId("aim_key");
	//AimbotGroup.PlaceLabledControl("Aimbot Key", this, &AimbotKeyBind);

	TargetHitbox.SetFileId("tgt_hitbox");
	TargetHitbox.AddItem(XorStr("Head"));
	TargetHitbox.AddItem(XorStr("Neck"));
	TargetHitbox.AddItem(XorStr("Chest"));
	TargetHitbox.AddItem(XorStr("Stomach"));
	TargetHitbox.AddItem(XorStr("Dick"));
	TargetHitbox.AddItem(XorStr("Lower Body"));
	TargetHitbox.AddItem(XorStr("Pelvis"));
	AimbotGroup.PlaceLabledControl(XorStr("Hitbox"), this, &TargetHitbox);

	AimbotFov.SetFileId("aim_fov");
	AimbotFov.SetBoundaries(0.f, 180.f);
	AimbotFov.SetValue(0.f);
	AimbotGroup.PlaceLabledControl(XorStr("Field of View"), this, &AimbotFov);

	AimbotMultiPoint.SetFileId("aim_multipoint");
	AimbotGroup.PlaceLabledControl(XorStr("Multi-Point"), this, &AimbotMultiPoint);

	AimbotMultiPointIndexes.SetFileId("aim_mpindexes");
	AimbotMultiPointIndexes.SetBoundaries(0.f, 7.f);
	AimbotMultiPointIndexes.SetValue(0.f);
	AimbotGroup.PlaceLabledControl(XorStr("Indices"), this, &AimbotMultiPointIndexes);

	TargetHitscan.SetFileId("tgt_hitscan");
	TargetHitscan.AddItem(XorStr("Off"));
	TargetHitscan.AddItem(XorStr("Low")); // 1
	TargetHitscan.AddItem(XorStr("Medium")); // 2
	TargetHitscan.AddItem(XorStr("High")); // 3
	AimbotGroup.PlaceLabledControl(XorStr("Hit Scan"), this, &TargetHitscan);

	AccuracyAutoWall.SetFileId("acc_awall");
	AimbotGroup.PlaceLabledControl(XorStr("Auto Wall"), this, &AccuracyAutoWall);

	AccuracyMinimumDamage.SetFileId("acc_mindmg");
	AccuracyMinimumDamage.SetBoundaries(1.f, 100.f);
	AccuracyMinimumDamage.SetValue(1.f);
	AimbotGroup.PlaceLabledControl(XorStr("AWall Min. Damage"), this, &AccuracyMinimumDamage);

	AimbotSilentAim.SetFileId("aim_silent");
	AimbotGroup.PlaceLabledControl(XorStr("Silent Aim"), this, &AimbotSilentAim);

	AimbotPSilent.SetFileId("aim_psilent");
	AimbotGroup.PlaceLabledControl(XorStr("Perfect Silent Aim"), this, &AimbotPSilent);

	AimbotPVSFix.SetFileId("aim_pvsFIX");
	AimbotGroup.PlaceLabledControl(XorStr("PVS Fix"), this, &AimbotPVSFix);

	AccuracySmart.SetFileId("acc_smartaim");
	AccuracySmart.SetBoundaries(0.f, 30.0f);
	AccuracySmart.SetValue(0.f);
	AimbotGroup.PlaceLabledControl(XorStr("Baim after X shots"), this, &AccuracySmart);

	AccuracyPointScale.SetFileId("acc_pointscale");
	AccuracyPointScale.SetBoundaries(0.f, 1.0f);
	AccuracyPointScale.SetValue(0.23f);
	AimbotGroup.PlaceLabledControl(XorStr("Point Scale"), this, &AccuracyPointScale);

	AccuracyRecoil.SetFileId("acc_recoil");
	AimbotGroup.PlaceLabledControl(XorStr("Disable Recoil"), this, &AccuracyRecoil);

	AccuracyAutoScope.SetFileId("acc_scope");
	AimbotGroup.PlaceLabledControl(XorStr("Auto Scope"), this, &AccuracyAutoScope);

	//AccuracyAutoStop.SetFileId("acc_stop");
	//AimbotGroup.PlaceLabledControl("Auto Stop & Crouch", this, &AccuracyAutoStop);

#pragma endregion Aimbot Controls Get Setup in here

#pragma region AntiAim
	AntiAimGroup.SetPosition(308, 18);
	AntiAimGroup.SetText(XorStr("Anti-Aim"));
	AntiAimGroup.SetSize(276, 190);
	RegisterControl(&AntiAimGroup);

	AntiAimPitch.SetFileId("aa_x");
	AntiAimPitch.AddItem(XorStr("Off"));
	AntiAimPitch.AddItem(XorStr("Default"));
	AntiAimPitch.AddItem(XorStr("Up"));
	AntiAimPitch.AddItem(XorStr("Down"));
	AntiAimPitch.AddItem(XorStr("Minimal"));
	AntiAimGroup.PlaceLabledControl(XorStr("Pitch"), this, &AntiAimPitch);

	AntiAimYaw.SetFileId("aa_y");
	AntiAimYaw.AddItem(XorStr("Off"));
	AntiAimYaw.AddItem(XorStr("Backwards"));
	AntiAimYaw.AddItem(XorStr("Spin"));
	AntiAimYaw.AddItem(XorStr("Sideways Right"));
	AntiAimYaw.AddItem(XorStr("Sideways Left"));
	AntiAimYaw.AddItem(XorStr("Static"));
	AntiAimYaw.AddItem(XorStr("Jitter"));
	AntiAimYaw.AddItem(XorStr("Jitter Synced"));
	AntiAimYaw.AddItem(XorStr("180 Jitter"));
	AntiAimGroup.PlaceLabledControl(XorStr("Real Yaw"), this, &AntiAimYaw);

	AntiAimFakeYaw.SetFileId("aa_fakey");
	AntiAimFakeYaw.AddItem(XorStr("Off"));
	AntiAimFakeYaw.AddItem(XorStr("Default"));
	AntiAimFakeYaw.AddItem(XorStr("Backwards"));
	AntiAimFakeYaw.AddItem(XorStr("Spin"));
	AntiAimFakeYaw.AddItem(XorStr("Sideways Right"));
	AntiAimFakeYaw.AddItem(XorStr("Sideways Left"));
	AntiAimFakeYaw.AddItem(XorStr("Static"));
	AntiAimFakeYaw.AddItem(XorStr("Jitter"));
	AntiAimFakeYaw.AddItem(XorStr("Jitter Synced"));
	AntiAimFakeYaw.AddItem(XorStr("180 Jitter"));
	AntiAimGroup.PlaceLabledControl(XorStr("Fake Yaw"), this, &AntiAimFakeYaw);

	AntiAimDormant.SetFileId("aa_dormant");
	AntiAimGroup.PlaceLabledControl(XorStr("Dormant Check"), this, &AntiAimDormant);

	AntiAimSpeed.SetFileId("aa_spinspeed");
	AntiAimSpeed.SetBoundaries(0.f, 10.f);
	AntiAimSpeed.SetValue(1.0f);
	AntiAimGroup.PlaceLabledControl(XorStr("Spinbot Speed"), this, &AntiAimSpeed);

	AntiAimDynamic.SetFileId("aa_dynamic");
	AntiAimGroup.PlaceLabledControl(XorStr("Dynamic"), this, &AntiAimDynamic);

	//AntiAimTarget.SetFileId("aa_target");
	//AntiAimGroup.PlaceLabledControl(XorStr("At Targets"), this, &AntiAimTarget);

	AntiAimLBY.SetFileId("aa_LBY");
	AntiAimGroup.PlaceLabledControl(XorStr("LBY Indicator"), this, &AntiAimLBY);

#pragma endregion  AntiAim controls get setup in here

#pragma region Resolver
	ResolverGroup.SetPosition(308, 224);
	ResolverGroup.SetText(XorStr("Resolver Options"));
	ResolverGroup.SetSize(276, 204);
	RegisterControl(&ResolverGroup);

	ResolverON.SetFileId("r_enable");
	ResolverGroup.PlaceLabledControl(XorStr("Resolver"), this, &ResolverON);

	ResolverPitch.SetFileId("r_pitchtype");
	ResolverPitch.AddItem(XorStr("Half Cycle"));
	ResolverPitch.AddItem(XorStr("Full Cycle"));
	ResolverPitch.AddItem(XorStr("Down"));
	ResolverGroup.PlaceLabledControl(XorStr("Pitch Type"), this, &ResolverPitch);

	ResolverYaw.SetFileId("r_yawtype");
	ResolverYaw.AddItem(XorStr("Full Cycle"));
	ResolverYaw.AddItem(XorStr("Cross"));
	ResolverGroup.PlaceLabledControl("Yaw Type", this, &ResolverYaw);

	ResolverStep.SetFileId("r_stepdegree");
	ResolverStep.AddItem(XorStr("30 Step"));
	ResolverStep.AddItem(XorStr("45 Step"));
	ResolverStep.AddItem(XorStr("90 Step"));
	ResolverGroup.PlaceLabledControl(XorStr("Angle Step"), this, &ResolverStep);

	ResolverSnap.SetFileId("r_snap");
	ResolverSnap.AddItem(XorStr("Relative"));
	ResolverSnap.AddItem(XorStr("Absolute"));
	ResolverSnap.AddItem(XorStr("Velocity"));
	ResolverSnap.AddItem(XorStr("At Target"));
	ResolverGroup.PlaceLabledControl(XorStr("Angle Snap"), this, &ResolverSnap);

	LBYFix.SetFileId("r_lowerbodyfix");
	ResolverGroup.PlaceLabledControl(XorStr("Lower Body Fix"), this, &LBYFix);
	
	/*
	ResolverType.SetFileId("r_resolvermode");
	ResolverType.AddItem("Off");
	ResolverType.AddItem("Basic");
	ResolverType.AddItem("Delta 1");
	ResolverType.AddItem("Delta 2");
	ResolverType.AddItem("Experimental");
	ResolverGroup.PlaceLabledControl("Resolver Mode", this, &ResolverType);

	ResolverDebug.SetFileId("r_debug");
	ResolverGroup.PlaceLabledControl("Debug", this, &ResolverDebug);
	*/

	ResolverFLFix.SetFileId("r_fakelagfix");
	ResolverFLFix.AddItem(XorStr("Off"));
	ResolverFLFix.AddItem(XorStr("Fix Interp"));
	ResolverFLFix.AddItem(XorStr("Pos Adjust"));
	ResolverFLFix.AddItem(XorStr("Normal"));
	ResolverFLFix.AddItem(XorStr("Testing"));
	ResolverFLFix.AddItem(XorStr("Back Tracking"));
	ResolverGroup.PlaceLabledControl(XorStr("Fake Lag Comp"), this, &ResolverFLFix);
}

void CLegitBotTab::Setup()
{
	SetTitle("L");

#pragma region Enabled Group
	ActiveGroup.SetText(XorStr("Legit"));
	ActiveGroup.SetPosition(16, 18);
	ActiveGroup.SetSize(568, 81);
	RegisterControl(&ActiveGroup);

	LegitActivate.SetFileId("legit_enabled");
	ActiveGroup.PlaceLabledControl(XorStr("Active"), this, &LegitActivate);

	LegitWeapType.SetFileId("legit_weapontype");
	LegitWeapType.AddItem(XorStr("Pistols"));
	LegitWeapType.AddItem(XorStr("SMG's"));
	LegitWeapType.AddItem(XorStr("Sniper Rifles"));
	LegitWeapType.AddItem(XorStr("Rifles"));
	LegitWeapType.AddItem(XorStr("Others"));
	ActiveGroup.PlaceLabledControl(XorStr("Active"), this, &LegitWeapType);

	if (Menu::Window.LegitBotTab.LegitActivate.GetState())
	{

	}
}

void CVisualTab::Setup()
{
	SetTitle(XorStr("V"));

#pragma region Options
	OptionsGroup.SetText(XorStr("Options"));
	OptionsGroup.SetPosition(16, 18);
	OptionsGroup.SetSize(276, 373);
	RegisterControl(&OptionsGroup);

	OptionsBox.SetFileId("opt_box");
	OptionsGroup.PlaceLabledControl(XorStr("Box"), this, &OptionsBox);

	OptionsName.SetFileId("opt_name");
	OptionsGroup.PlaceLabledControl(XorStr("Name"), this, &OptionsName);

	OptionsHealth.SetFileId("opt_hp");
	OptionsGroup.PlaceLabledControl(XorStr("Health"), this, &OptionsHealth);

	OptionsArmor.SetFileId("opt_armor");
	OptionsGroup.PlaceLabledControl(XorStr("Armor"), this, &OptionsArmor);

	OptionsWeapon.SetFileId("opt_weapon");
	OptionsWeapon.AddItem(XorStr("Off"));
	OptionsWeapon.AddItem(XorStr("Text"));
	OptionsWeapon.AddItem(XorStr("Weapon Icon"));
	OptionsGroup.PlaceLabledControl(XorStr("Weapon"), this, &OptionsWeapon);

	OptionsDistance.SetFileId("opt_distance");
	OptionsGroup.PlaceLabledControl(XorStr("Distance"), this, &OptionsDistance);

	//OptionsDefusing.SetFileId("opt_defusing");
	//OptionsGroup.PlaceLabledControl("defusing", this, &OptionsDefusing);

	OptionsSkeleton.SetFileId("opt_bone");
	OptionsGroup.PlaceLabledControl(XorStr("Skeleton"), this, &OptionsSkeleton);

	OptionsAimSpot.SetFileId("opt_aimspot");
	OptionsGroup.PlaceLabledControl(XorStr("Aim Point"), this, &OptionsAimSpot);

	OptionsChams.SetFileId("opt_chams");
	OptionsChams.AddItem(XorStr("Off"));
	OptionsChams.AddItem(XorStr("Normal"));
	OptionsChams.AddItem(XorStr("Rainbow"));
	OptionsChams.AddItem(XorStr("Wireframe"));
	OptionsChams.AddItem(XorStr("Chrome"));
	OptionsChams.AddItem(XorStr("Glass"));
	OptionsChams.AddItem(XorStr("Flat"));
	OptionsChams.AddItem(XorStr("Flat Rainbow"));
	OptionsChams.AddItem(XorStr("Wireframe Rainbow"));
	OptionsGroup.PlaceLabledControl(XorStr("Chams"), this, &OptionsChams);

	FiltersPlayers.SetFileId("ftr_players");
	OptionsGroup.PlaceLabledControl(XorStr("Players"), this, &FiltersPlayers);

	FiltersEnemiesOnly.SetFileId("ftr_enemyonly");
	OptionsGroup.PlaceLabledControl(XorStr("Enemies Only"), this, &FiltersEnemiesOnly);

	FiltersC4.SetFileId("ftr_c4");
	OptionsGroup.PlaceLabledControl(XorStr("Bomb"), this, &FiltersC4);

	FiltersWeapons.SetFileId("ftr_weaps");
	OptionsGroup.PlaceLabledControl(XorStr("Weapons"), this, &FiltersWeapons);

#pragma endregion Setting up the Options controls

#pragma region Filters
	EffectsGroup.SetText(XorStr("Effects & Removals"));
	EffectsGroup.SetPosition(308, 18);
	EffectsGroup.SetSize(276, 175);
	RegisterControl(&EffectsGroup);

	EffectMap.SetFileId("fx_map");
	EffectMap.AddItem(XorStr("Off"));
	EffectMap.AddItem(XorStr("Dawn Mode")); //credits to chad (creator of hades) for dawn mode
	EffectMap.AddItem(XorStr("Night Mode"));
	EffectMap.AddItem(XorStr("Draw Gray"));
	EffectsGroup.PlaceLabledControl(XorStr("Map Effects"), this, &EffectMap);

	//EffectASUS.SetFileId("fx_asus");
	//EffectsGroup.PlaceLabledControl("asus walls", this, &EffectASUS);

	//EffectNoSky.SetFileId("fx_nosky");
	//EffectsGroup.PlaceLabledControl("disable skybox", this, &EffectNoSky);

	EffectParticles.SetFileId("fx_noparticles");
	EffectsGroup.PlaceLabledControl(XorStr("Disable Particles"), this, &EffectParticles);

	EffectNoVisualRecoil.SetFileId("fx_novisrecoil");
	EffectsGroup.PlaceLabledControl(XorStr("Remove Visual Recoil"), this, &EffectNoVisualRecoil);

	EffectNoFlash.SetFileId("fx_noflash");
	EffectsGroup.PlaceLabledControl(XorStr("Remove Flash"), this, &EffectNoFlash);

	EffectNoScope.SetFileId("fx_noscope");
	EffectsGroup.PlaceLabledControl(XorStr("Remove Scope"), this, &EffectNoScope);

	EffectThirdPerson.SetFileId("fx_thirdperson");
	EffectThirdPerson.AddItem(XorStr("Off"));
	EffectThirdPerson.AddItem(XorStr("Fake Angles"));
	EffectThirdPerson.AddItem(XorStr("Real Angles"));
	EffectsGroup.PlaceLabledControl(XorStr("Force Third Person"), this, &EffectThirdPerson);

	EffectFOV.SetFileId("fx_fov");
	EffectFOV.SetBoundaries(0.f, 120.f);
	EffectFOV.SetValue(90.f);
	EffectsGroup.PlaceLabledControl(XorStr("Field of View"), this, &EffectFOV);

#pragma endregion Setting up the Filters controls

#pragma region Other
	OtherGroup.SetText(XorStr("Other"));
	OtherGroup.SetPosition(308, 209);
	OtherGroup.SetSize(276, 182);
	RegisterControl(&OtherGroup);

	OtherWatermark.SetFileId("otr_watermark");
	OtherWatermark.SetState(true);
	OtherGroup.PlaceLabledControl(XorStr("Watermark"), this, &OtherWatermark);

	OtherSpectatorList.SetFileId("otr_speclist");
	OtherGroup.PlaceLabledControl(XorStr("Spectators List"), this, &OtherSpectatorList);

	OtherHandFX.SetFileId("otr_handfx");
	OtherHandFX.AddItem(XorStr("Off"));
	OtherHandFX.AddItem(XorStr("Wireframe"));
	OtherHandFX.AddItem(XorStr("Chams"));
	OtherHandFX.AddItem(XorStr("Black"));
	OtherHandFX.AddItem(XorStr("Black Wireframe"));
	OtherHandFX.AddItem(XorStr("Transparent"));
	OtherHandFX.AddItem(XorStr("Rainbow Chams"));
	OtherHandFX.AddItem(XorStr("Rainbow Wireframe"));
	OtherGroup.PlaceLabledControl(XorStr("Hand FX"), this, &OtherHandFX);

	OtherWeaponFX.SetFileId("otr_wpnfx");
	OtherWeaponFX.AddItem(XorStr("Off"));
	OtherWeaponFX.AddItem(XorStr("Wireframe"));
	OtherWeaponFX.AddItem(XorStr("Chams"));
	OtherWeaponFX.AddItem(XorStr("Black"));
	OtherWeaponFX.AddItem(XorStr("Black Wireframe"));
	OtherWeaponFX.AddItem(XorStr("Transparent"));
	OtherWeaponFX.AddItem(XorStr("Rainbow Chams"));
	OtherWeaponFX.AddItem(XorStr("Rainbow Wireframe"));
	OtherGroup.PlaceLabledControl(XorStr("Weapon FX"), this, &OtherWeaponFX);

	OtherSniperCrosshair.SetFileId("otr_snipercrosshair");
	OtherGroup.PlaceLabledControl(XorStr("Crosshair on Snipers"), this, &OtherSniperCrosshair);

	OtherSpreadCrosshair.SetFileId("otr_spreadxhair");
	OtherGroup.PlaceLabledControl(XorStr("Spread Crosshair"), this, &OtherSpreadCrosshair);

	OtherHitmarker.SetFileId("otr_hitmark");
	OtherGroup.PlaceLabledControl(XorStr("Hitmarker"), this, &OtherHitmarker);

#pragma endregion Setting up the Other controls
}

void CMiscTab::Setup()
{
	SetTitle(XorStr("M"));

	GloveGroup.SetPosition(308, 18);
	GloveGroup.SetSize(276, 110);
	GloveGroup.SetText(XorStr("Gloves"));
	RegisterControl(&GloveGroup);

	GloveActive.SetText(XorStr("Apply Gloves"));
	GloveActive.SetCallback(GloveApplyCallbk);
	GloveActive.SetPosition(308 + 28, 90);
	GloveActive.SetSize(226, 48.3333333);
	RegisterControl(&GloveActive);

	GloveModel.SetFileId("gloves_model");
	GloveModel.AddItem(XorStr("Off"));
	GloveModel.AddItem(XorStr("Bloodhound"));
	GloveModel.AddItem(XorStr("Handwrap"));
	GloveModel.AddItem(XorStr("Driver"));
	GloveModel.AddItem(XorStr("Sport"));
	GloveModel.AddItem(XorStr("Motorcycle"));
	GloveModel.AddItem(XorStr("Specialist"));
	GloveGroup.PlaceLabledControl(XorStr("Model"), this, &GloveModel);

	GloveSkin.SetFileId("glove_skin");
	GloveSkin.AddItem(XorStr("None"));
	GloveSkin.AddItem(XorStr("Snakebite")); //10007
	GloveSkin.AddItem(XorStr("Charred"));// 10006
	GloveSkin.AddItem(XorStr("Guerrilla"));// 10039
	GloveSkin.AddItem(XorStr("Bronzed"));// 10008
	GloveSkin.AddItem(XorStr("Slaughter")); // 10021
	GloveSkin.AddItem(XorStr("Badlands"));// 10036
	GloveSkin.AddItem(XorStr("Leather"));// 10009
	GloveSkin.AddItem(XorStr("Spruce DDPAT"));// 10010
	GloveSkin.AddItem(XorStr("Crimson Weave"));// 10016
	GloveSkin.AddItem(XorStr("Lunar Weave"));// 10013
	GloveSkin.AddItem(XorStr("Diamondback"));// 10040
	GloveSkin.AddItem(XorStr("Convoy"));// 10015
	GloveSkin.AddItem(XorStr("Pandora's Box"));// 10037
	GloveSkin.AddItem(XorStr("Hedge Naze"));// 10038
	GloveSkin.AddItem(XorStr("Superconductor"));// 10018
	GloveSkin.AddItem(XorStr("Arid"));// 10019
	GloveSkin.AddItem(XorStr("Spearmint"));// 10026
	GloveSkin.AddItem(XorStr("Cool Mint"));// 10028
	GloveSkin.AddItem(XorStr("Boom"));// 10027
	GloveSkin.AddItem(XorStr("Eclipse"));// 10024
	GloveSkin.AddItem(XorStr("Crimson Kimono"));// 10033
	GloveSkin.AddItem(XorStr("Emerald Web"));// 10034
	GloveSkin.AddItem(XorStr("Foundation"));// 10035
	GloveSkin.AddItem(XorStr("Forest DDPAT"));// 10030
	GloveGroup.PlaceLabledControl("Skin", this, &GloveSkin);

#pragma region Knife
	KnifeGroup.SetPosition(308, 144);
	KnifeGroup.SetSize(276, 331);
	//KnifeGroup.SetSize(360, 548);
	KnifeGroup.SetText(XorStr("Skins"));
	RegisterControl(&KnifeGroup);

	KnifeModel.SetFileId("knife_model");
	KnifeModel.AddItem(XorStr("Off"));
	KnifeModel.AddItem(XorStr("Bayonet"));
	KnifeModel.AddItem(XorStr("Bowie Knife"));
	KnifeModel.AddItem(XorStr("Butterfly"));
	KnifeModel.AddItem(XorStr("Falchion"));
	KnifeModel.AddItem(XorStr("Flip Knife"));
	KnifeModel.AddItem(XorStr("Gut Knife"));
	KnifeModel.AddItem(XorStr("Huntsman"));
	KnifeModel.AddItem(XorStr("Karambit"));
	KnifeModel.AddItem(XorStr("M9 Bayonet"));
	KnifeModel.AddItem(XorStr("Shadow Daggers"));
	KnifeGroup.PlaceLabledControl(XorStr("Knife"), this, &KnifeModel);
#pragma endregion

#pragma region CircleStrafe
	CircleStrafeGroup.SetPosition(308, 491);
	CircleStrafeGroup.SetSize(276, 75);
	CircleStrafeGroup.SetText(XorStr("Circle Strafer"));
	RegisterControl(&CircleStrafeGroup);

	CircleEnable.SetFileId("cstrafe_active");
	CircleStrafeGroup.PlaceLabledControl(XorStr("Active (MOUSE4)"), this, &CircleEnable);

	CircleFactor.SetFileId("cstrafe_factor");
	CircleFactor.SetBoundaries(0.f, 10.f);
	CircleFactor.SetValue(4.2f);
	CircleStrafeGroup.PlaceLabledControl(XorStr("Circle Strafe Factor"), this, &CircleFactor);
#pragma endregion Circle

#pragma region Other
	OtherGroup.SetPosition(16, 18);
	OtherGroup.SetSize(276, 110);
	OtherGroup.SetText(XorStr("Other"));
	RegisterControl(&OtherGroup);

	OtherAutoJump.SetFileId("otr_autojump");
	OtherGroup.PlaceLabledControl(XorStr("Auto Jump"), this, &OtherAutoJump);

	OtherAutoStrafe.SetFileId("otr_autostrafe");
	OtherAutoStrafe.AddItem(XorStr("Off"));
	OtherAutoStrafe.AddItem(XorStr("Legit"));
	OtherAutoStrafe.AddItem(XorStr("Rage"));
	OtherGroup.PlaceLabledControl(XorStr("Auto Strafe"), this, &OtherAutoStrafe);

	OtherSafeMode.SetFileId("otr_safemode");
	OtherSafeMode.SetState(true);
	OtherGroup.PlaceLabledControl(XorStr("Anti Untrusted"), this, &OtherSafeMode);

	//OtherSpectators.SetFileId("otr_speclist");
	//OtherGroup.PlaceLabledControl("spec list", this, &OtherSpectators);
#pragma endregion other random options

#pragma region Clan Tag

	ClanTagGroup.SetPosition(16, 145);
	ClanTagGroup.SetSize(276, 135);
	ClanTagGroup.SetText(XorStr("Clan Tag"));
	RegisterControl(&ClanTagGroup);

	ClanTagEnable.SetFileId("clantag_enable");
	ClanTagGroup.PlaceLabledControl(XorStr("Active"), this, &ClanTagEnable);

	ClanTagType.SetFileId("clantag_type");
	ClanTagType.AddItem(XorStr("Static"));
	ClanTagType.AddItem(XorStr("Animated"));
	ClanTagGroup.PlaceLabledControl(XorStr("Display Type"), this, &ClanTagType);

	ClanTagChoice.SetFileId("clantag_choice");
	ClanTagChoice.AddItem(XorStr("Amber"));
	ClanTagChoice.AddItem(XorStr("Skeet"));
	ClanTagChoice.AddItem(XorStr("Pasted"));
	ClanTagChoice.AddItem(XorStr("Ayyware"));
	ClanTagChoice.AddItem(XorStr("Trash.HvH"));
	ClanTagChoice.AddItem(XorStr("Local Time"));
	ClanTagGroup.PlaceLabledControl(XorStr("Clan Tag Text"), this, &ClanTagChoice);

	ClanTagSpeed.SetFileId("clantag_animatedspeed");
	ClanTagSpeed.SetBoundaries(0.f, 10.f);
	ClanTagSpeed.SetValue(2.4f);
	ClanTagGroup.PlaceLabledControl(XorStr("Animated Speed"), this, &ClanTagSpeed);

#pragma region FakeLag

	FakeLagGroup.SetPosition(16, 298);
	FakeLagGroup.SetSize(276, 268);
	FakeLagGroup.SetText(XorStr("Fake Lag"));
	RegisterControl(&FakeLagGroup);

	FakeLagEnable.SetFileId("fakelag_enable");
	FakeLagGroup.PlaceLabledControl(XorStr("Active"), this, &FakeLagEnable);

	//FakeLagInAir.SetFileId("fakelag_inair");
	//FakeLagGroup.PlaceCheckBox("fakelag only in air", this, &FakeLagInAir);

	FakeLagWhileShooting.SetFileId("fakelag_whileshooting");
	FakeLagGroup.PlaceLabledControl(XorStr("FakeLag While Shooting"), this, &FakeLagWhileShooting);

	FakeLagType.SetFileId("fakelag_type");
	FakeLagType.AddItem(XorStr("Normal"));
	FakeLagType.AddItem(XorStr("Adaptive"));
	FakeLagType.AddItem(XorStr("Switch"));
	FakeLagType.AddItem(XorStr("Random"));
	FakeLagGroup.PlaceLabledControl(XorStr("Fakelag Type"), this, &FakeLagType);

	FakeLagAmount.SetFileId("fakelag_amount");
	FakeLagAmount.SetBoundaries(0.f, 15.f);
	FakeLagAmount.SetValue(0.f);
	FakeLagGroup.PlaceLabledControl(XorStr("Amount"), this, &FakeLagAmount);

	SlowMotion.SetFileId("slowmotion_key");
	FakeLagGroup.PlaceLabledControl(XorStr("Slow-Motion Key"), this, &SlowMotion);

#pragma endregion FakeLag
}

void CColorsTab::Setup()
{
	SetTitle(XorStr("C"));
	ControlGroup.SetText(XorStr("Controls"));
	ControlGroup.SetPosition(16, 18);
	ControlGroup.SetSize(276, 58);
	RegisterControl(&ControlGroup);

	ControlColor.SetFileId("col_controls");
	ControlColor.AddItem(XorStr("Green"));
	ControlColor.AddItem(XorStr("Blue"));
	ControlColor.AddItem(XorStr("Red"));
	ControlColor.AddItem(XorStr("Gold"));
	ControlGroup.PlaceLabledControl(XorStr("Controls Color"), this, &ControlColor);

#pragma region CT Invisible ESP

	CTInvisESP.SetText(XorStr("CT Invisible ESP"));
	CTInvisESP.SetPosition(16, 93);
	CTInvisESP.SetSize(276, 88);
	RegisterControl(&CTInvisESP);

	CTInvisESPR.SetFileId("esp_ctinvisR");
	CTInvisESPR.SetBoundaries(0.f, 255.f);
	CTInvisESPR.SetValue(25.f);
	CTInvisESP.PlaceLabledControl(XorStr("Red"), this, &CTInvisESPR);

	CTInvisESPG.SetFileId("esp_ctinvisG");
	CTInvisESPG.SetBoundaries(0.f, 255.f);
	CTInvisESPG.SetValue(255.f);
	CTInvisESP.PlaceLabledControl(XorStr("Green"), this, &CTInvisESPG);

	CTInvisESPB.SetFileId("esp_ctinvisB");
	CTInvisESPB.SetBoundaries(0.f, 255.f);
	CTInvisESPB.SetValue(25.f);
	CTInvisESP.PlaceLabledControl(XorStr("Blue"), this, &CTInvisESPB);

#pragma endregion CT Invisible ESP

#pragma region CT Visible ESP

	CTVisESP.SetText(XorStr("CT Visible ESP"));
	CTVisESP.SetPosition(16, 198);
	CTVisESP.SetSize(276, 88);
	RegisterControl(&CTVisESP);

	CTVisESPR.SetFileId("esp_ctvisR");
	CTVisESPR.SetBoundaries(0.f, 255.f);
	CTVisESPR.SetValue(25.f);
	CTVisESP.PlaceLabledControl(XorStr("Red"), this, &CTVisESPR);

	CTVisESPG.SetFileId("esp_ctvisG");
	CTVisESPG.SetBoundaries(0.f, 255.f);
	CTVisESPG.SetValue(25.f);
	CTVisESP.PlaceLabledControl(XorStr("Green"), this, &CTVisESPG);

	CTVisESPB.SetFileId("esp_ctvisB");
	CTVisESPB.SetBoundaries(0.f, 255.f);
	CTVisESPB.SetValue(255.f);
	CTVisESP.PlaceLabledControl(XorStr("Blue"), this, &CTVisESPB);

#pragma endregion CT Visible ESP

#pragma region T Invisible ESP
	
	TInvisESP.SetText(XorStr("T Invisible ESP"));
	TInvisESP.SetPosition(16, 303);
	TInvisESP.SetSize(276, 88);
	RegisterControl(&TInvisESP);

	TInvisESPR.SetFileId("esp_tinvisR");
	TInvisESPR.SetBoundaries(0.f, 255.f);
	TInvisESPR.SetValue(255.f);
	TInvisESP.PlaceLabledControl(XorStr("Red"), this, &TInvisESPR);

	TInvisESPG.SetFileId("esp_tinvisG");
	TInvisESPG.SetBoundaries(0.f, 255.f);
	TInvisESPG.SetValue(255.f);
	TInvisESP.PlaceLabledControl(XorStr("Green"), this, &TInvisESPG);

	TInvisESPB.SetFileId("esp_tinvisB");
	TInvisESPB.SetBoundaries(0.f, 255.f);
	TInvisESPB.SetValue(0.f);
	TInvisESP.PlaceLabledControl(XorStr("Blue"), this, &TInvisESPB);

#pragma endregion T Invisible ESP

#pragma region T Visible ESP

	TVisESP.SetText(XorStr("T Visible ESP"));
	TVisESP.SetPosition(16, 408);
	TVisESP.SetSize(276, 88);
	RegisterControl(&TVisESP);

	TVisESPR.SetFileId("esp_ctvisR");
	TVisESPR.SetBoundaries(0.f, 255.f);
	TVisESPR.SetValue(255.f);
	TVisESP.PlaceLabledControl(XorStr("Red"), this, &TVisESPR);

	TVisESPG.SetFileId("esp_ctvisG");
	TVisESPG.SetBoundaries(0.f, 255.f);
	TVisESPG.SetValue(25.f);
	TVisESP.PlaceLabledControl(XorStr("Green"), this, &TVisESPG);

	TVisESPB.SetFileId("esp_ctvisB");
	TVisESPB.SetBoundaries(0.f, 255.f);
	TVisESPB.SetValue(25.f);
	TVisESP.PlaceLabledControl(XorStr("Blue"), this, &TVisESPB);

#pragma endregion T Invisible 


#pragma region CT Invisible Chams

	CTInvisChams.SetText(XorStr("CT Invisible Chams"));
	CTInvisChams.SetPosition(308, 93);
	CTInvisChams.SetSize(276, 88);
	RegisterControl(&CTInvisChams);

	CTInvisChamsR.SetFileId("chams_ctinvisR");
	CTInvisChamsR.SetBoundaries(0.f, 255.f);
	CTInvisChamsR.SetValue(25.f);
	CTInvisChams.PlaceLabledControl(XorStr("Red"), this, &CTInvisChamsR);

	CTInvisChamsG.SetFileId("chams_ctinvisG");
	CTInvisChamsG.SetBoundaries(0.f, 255.f);
	CTInvisChamsG.SetValue(25.f);
	CTInvisChams.PlaceLabledControl(XorStr("Green"), this, &CTInvisChamsG);

	CTInvisChamsB.SetFileId("chams_ctinvisB");
	CTInvisChamsB.SetBoundaries(0.f, 255.f);
	CTInvisChamsB.SetValue(255.f);
	CTInvisChams.PlaceLabledControl(XorStr("Blue"), this, &CTInvisChamsB);

#pragma endregion CT Invisible Chams

#pragma region CT Visible Chams

	CTVisChams.SetText(XorStr("CT Visible Chams"));
	CTVisChams.SetPosition(308, 198);
	CTVisChams.SetSize(276, 88);
	RegisterControl(&CTVisChams);

	CTVisChamsR.SetFileId("chams_ctvisR");
	CTVisChamsR.SetBoundaries(0.f, 255.f);
	CTVisChamsR.SetValue(25.f);
	CTVisChams.PlaceLabledControl(XorStr("Red"), this, &CTVisChamsR);

	CTVisChamsG.SetFileId("chams_ctvisG");
	CTVisChamsG.SetBoundaries(0.f, 255.f);
	CTVisChamsG.SetValue(255.f);
	CTVisChams.PlaceLabledControl(XorStr("Green"), this, &CTVisChamsG);

	CTVisChamsB.SetFileId("chams_ctvisB");
	CTVisChamsB.SetBoundaries(0.f, 255.f);
	CTVisChamsB.SetValue(25.f);
	CTVisChams.PlaceLabledControl(XorStr("Blue"), this, &CTVisChamsB);

#pragma endregion CT Visible Chams

#pragma region T Invisible Chams

	TInvisChams.SetText(XorStr("T Invisible Chams"));
	TInvisChams.SetPosition(308, 303);
	TInvisChams.SetSize(276, 88);
	RegisterControl(&TInvisChams);

	TInvisChamsR.SetFileId("chams_tinvisR");
	TInvisChamsR.SetBoundaries(0.f, 255.f);
	TInvisChamsR.SetValue(255.f);
	TInvisChams.PlaceLabledControl(XorStr("Red"), this, &TInvisChamsR);

	TInvisChamsG.SetFileId("chams_tinvisG");
	TInvisChamsG.SetBoundaries(0.f, 255.f);
	TInvisChamsG.SetValue(25.f);
	TInvisChams.PlaceLabledControl(XorStr("Green"), this, &TInvisChamsG);

	TInvisChamsB.SetFileId("chams_tinvisB");
	TInvisChamsB.SetBoundaries(0.f, 255.f);
	TInvisChamsB.SetValue(25.f);
	TInvisChams.PlaceLabledControl(XorStr("Blue"), this, &TInvisChamsB);

#pragma endregion T Invisible Chams

#pragma region T Visible Chams

	TVisChams.SetText(XorStr("T Visible Chams"));
	TVisChams.SetPosition(308, 408);
	TVisChams.SetSize(276, 88);
	RegisterControl(&TVisChams);

	TVisChamsR.SetFileId("chams_ctvisR");
	TVisChamsR.SetBoundaries(0.f, 255.f);
	TVisChamsR.SetValue(255.f);
	TVisChams.PlaceLabledControl(XorStr("Red"), this, &TVisChamsR);

	TVisChamsG.SetFileId("chams_ctvisG");
	TVisChamsG.SetBoundaries(0.f, 255.f);
	TVisChamsG.SetValue(255.f);
	TVisChams.PlaceLabledControl(XorStr("Green"), this, &TVisChamsG);

	TVisChamsB.SetFileId("chams_ctvisB");
	TVisChamsB.SetBoundaries(0.f, 255.f);
	TVisChamsB.SetValue(0.f);
	TVisChams.PlaceLabledControl(XorStr("Blue"), this, &TVisChamsB);

#pragma endregion T Invisible Chams
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
	if (Menu::Window.VisualsTab.EffectParticles.GetState() || Menu::Window.VisualsTab.EffectMap.GetIndex() == 3)
		SpoofConvars();

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		Menu::Window.VisualsTab.Active.SetState(true);
	}
	else
	{
		Menu::Window.VisualsTab.EffectMap.SetIndex(0);
		Menu::Window.VisualsTab.Active.SetState(false);
	}

	if (Window.MiscTab.OtherSafeMode.GetState())
	{
		Window.RageBotTab.AccuracySpread.SetState(false);
	}
	
	

	
	GUI.Update();
	GUI.Draw();

	
}


