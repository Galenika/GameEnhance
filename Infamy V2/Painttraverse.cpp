#include "Hooks.h"
#include "Globals.h"
#include "Cheats.h"
#include "HWID.h"
#include "Accessors.h"
#include "mouse.h"
#include "Framework.h"

int fAlphaToUse = 0.f;
PaintTraverseFn oPaintTraverse;
bool bMenu = false;

inline bool IsMousePressed()
{
	return (GetAsyncKeyState(0x01));
}

bool IsClicked2;
static bool once2 = false;
bool ClickedTick2;

//MenuCursor
void DrawMouse()
{
	auto ptMouse = Input->GetMousePos();
	int x, y;
	x = ptMouse.x;
	y = ptMouse.y;
	static float rainbow;
	rainbow += 0.002f;
	if (rainbow > 1.f) rainbow = 0.f;
	DrawRect(x, y, 1, 12, Color(0, 0, 0));
	DrawRect(x + 1, y, 1, 11, Color(0, 0, 0));
	DrawRect(x + 1, y + 11, 1, 1, Color(0, 0, 0));
	DrawRect(x + 2, y + 1, 1, 1, Color(0, 0, 0));
	DrawRect(x + 2, y + 10, 1, 1, Color(0, 0, 0));
	DrawRect(x + 3, y + 2, 1, 1, Color(0, 0, 0));
	DrawRect(x + 3, y + 9, 1, 1, Color(0, 0, 0));
	DrawRect(x + 4, y + 3, 1, 1, Color(0, 0, 0));
	DrawRect(x + 5, y + 4, 1, 1, Color(0, 0, 0));
	DrawRect(x + 6, y + 5, 1, 1, Color(0, 0, 0));
	DrawRect(x + 7, y + 6, 1, 1, Color(0, 0, 0));
	DrawRect(x + 8, y + 7, 1, 1, Color(0, 0, 0));
	DrawRect(x + 4, y + 8, 4, 1, Color(0, 0, 0));
	DrawRect(x + 1, y + 1, 1, 10, Color::FromHSB(rainbow, 0.60f, 0.75f));
	DrawRect(x + 2, y + 2, 1, 8, Color::FromHSB(rainbow, 0.60f, 0.75f));
	DrawRect(x + 3, y + 3, 1, 6, Color::FromHSB(rainbow, 0.60f, 0.75f));
	DrawRect(x + 4, y + 4, 1, 4, Color::FromHSB(rainbow, 0.60f, 0.75f));
	DrawRect(x + 5, y + 5, 1, 3, Color::FromHSB(rainbow, 0.60f, 0.75f));
	DrawRect(x + 6, y + 6, 1, 2, Color::FromHSB(rainbow, 0.60f, 0.75f));
	DrawRect(x + 7, y + 7, 1, 1, Color::FromHSB(rainbow, 0.60f, 0.75f));
}

void DrawMenu()
{
	//MenuLocation
	MWindow* window = new MWindow(XorStr("Infamy V2"), Vector2D(620, 250), Vector2D(675, 405));
	MCategory* Title = new MCategory(XorStr("Infamy V2"), Vector2D(window->position.x + 25, window->position.y + 25), window);
	Input->GetClicked();
	//categories
	MCategory* Rage = new MCategory(XorStr("Rage"), Vector2D(window->position.x, window->position.y + 65), window);
	MCategory* Visual = new MCategory(XorStr("Visuals"), Vector2D(window->position.x, window->position.y + 95), window);
	MCategory* Removals = new MCategory(XorStr("Removals"), Vector2D(window->position.x, window->position.y + 125), window);
	MCategory* PlayerMods = new MCategory(XorStr("Misc"), Vector2D(window->position.x, window->position.y + 155), window);
	MCategory* Aesthetics = new MCategory(XorStr("Aesthetics"), Vector2D(window->position.x, window->position.y + 185), window);

	//Boxes Per Categories
	MGroup* RageGroup = new MGroup(Rage, XorStr("Ragebot"), Vector2D(window->position.x + 200, window->position.y + 30), Vector2D(226, 360));
	MGroup* RageGroup2 = new MGroup(Rage, XorStr("Accuracy"), Vector2D(window->position.x + 440, window->position.y + 30), Vector2D(227, 153));
	MGroup* RageGroup3 = new MGroup(Rage, XorStr("Anti-Aim"), Vector2D(window->position.x + 440, window->position.y + 200), Vector2D(228, 190));
	Rage->AddChildControl(RageGroup);
	Rage->AddChildControl(RageGroup2);
	Rage->AddChildControl(RageGroup3);
	MGroup* pModsGroup = new MGroup(PlayerMods, XorStr("Misc"), Vector2D(window->position.x + 200, window->position.y + 30), Vector2D(465, 360));
	PlayerMods->AddChildControl(pModsGroup);
	MGroup* VisualGroup = new MGroup(Visual, XorStr("Visuals"), Vector2D(window->position.x + 200, window->position.y + 30), Vector2D(465, 360));
	Visual->AddChildControl(VisualGroup);
	MGroup* RemovalGroup = new MGroup(Removals, XorStr("Removals"), Vector2D(window->position.x + 200, window->position.y + 30), Vector2D(465, 360));
	Removals->AddChildControl(RemovalGroup);
	MGroup* AestheticsGroup = new MGroup(Aesthetics, XorStr("Aesthetics"), Vector2D(window->position.x + 200, window->position.y + 30), Vector2D(465, 360));
	Aesthetics->AddChildControl(AestheticsGroup);

	//RageTab
	MCheckbox* RageEnabled = new MCheckbox(RageGroup, "Enabled", &Options.Rage.Aimbot.Enabled, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 30));
	MCheckbox* AutowallToggle = new MCheckbox(RageGroup, "Autowall", &Options.Rage.Aimbot.Autowall, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 50));
	MCheckbox* AAAToggle = new MCheckbox(RageGroup, "Resolver", &Options.Rage.Aimbot.AntiAntiAim, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 70));

	MSliderFloat* fov = new MSliderFloat(RageGroup, "FOV", &Options.Rage.Aimbot.FOV, 1.0f, 180.0f, 150, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 230));

	std::vector<std::string> TargetType = { "FOV", "Distance", "Threat" };
	MCombo* Targeting = new MCombo(RageGroup, "Target Method", &Options.Rage.Aimbot.Targeting, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 210), TargetType, &Options.Menu.TargetTypeCombo);
	
	MCheckbox* Multipoint = new MCheckbox(RageGroup, "Multipoint", &Options.Rage.Aimbot.Multipoint, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 90));
	std::vector<std::string> HitScanType = { "Low", "Medium", "Extreme" };
	MCombo* HitScan = new MCombo(RageGroup, "Hitscans", &Options.Rage.Aimbot.HitScans, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 170), HitScanType, &Options.Menu.HitscanTypeCombo);
	
	std::vector<std::string> RHitboxes = { "Pelvis", "Lower Body", "Dick", "Stomach", "Chest", "Neck", "Head" };
	MCombo* Hitbox = new MCombo(RageGroup, "Hitbox", &Options.Rage.Aimbot.Hitbox, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 130), RHitboxes, &Options.Menu.RageHitboxCombo);

	MCheckbox* EnemyOnlyR = new MCheckbox(RageGroup, "Enemy Only", &Options.Rage.Aimbot.EnemyOnly, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 270));
	MCheckbox* AScope = new MCheckbox(RageGroup, "Auto Scope", &Options.Rage.Aimbot.AutoScope, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 290));
	MCheckbox* Nigger = new MCheckbox(RageGroup, "Friendly Aim", &Options.Rage.Aimbot.FriendAim, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 310));
	MCheckbox* AShoot = new MCheckbox(RageGroup, "Auto Shoot", &Options.Rage.Aimbot.AutoShoot, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 330));
	MCheckbox* SAim = new MCheckbox(RageGroup, "Silent Aim", &Options.Rage.Aimbot.SilentAim, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 350));



	MSliderFloat* MinDmg = new MSliderFloat(RageGroup2, "Minimum Damage", &Options.Rage.AutoWall.MinDamage, 1.f, 100.f, 150, Vector2D(window->position.x + 440 + 25, window->position.y + 20 + 30));
	MCheckbox* HitC = new MCheckbox(RageGroup2, "Hitchance", &Options.Rage.Aimbot.bHitchance, Vector2D(window->position.x + 440 + 25, window->position.y + 20 + 70));
	MCheckbox* HitCSniper = new MCheckbox(RageGroup2, "Hitchance Snipers", &Options.Rage.Aimbot.bHitchanceSnipers, Vector2D(window->position.x + 440 + 25, window->position.y + 20 + 90));
	MSliderFloat* HitchanceAmt = new MSliderFloat(RageGroup2, "Hitchance Amount", &Options.Rage.Aimbot.Hitchance, 1.f, 100.f, 150, Vector2D(window->position.x + 440 + 25, window->position.y + 20 + 110));
	MCheckbox* AAEnable = new MCheckbox(RageGroup3, "Anti-Aim", &Options.Rage.AntiAim.Enabled, Vector2D(window->position.x + 440 + 25, window->position.y + 190 + 30));
	//MCheckbox* AtTargets = new MCheckbox(RageGroup3, "At Targets", &Options.Rage.AntiAim.AtTargets, Vector2D(window->position.x + 440 + 25, window->position.y + 190 + 180));
	MSlider* SpinSpeed = new MSlider(RageGroup3, "Twitch/Spin-Speed", &Options.Rage.AntiAim.spinSpeed, 1, 50, 150, Vector2D(window->position.x + 440 + 25, window->position.y + 190 + 150));
	std::vector<std::string> AAYaws = { "Off", "Fake-Backwards", "Fake-Sideways", "Fake-Static", "Fake-Static 2", "Fake-Static 3", "Fake-Twitch", "Fake1", "Fake2", "Backwards" };
	MCombo* YawComboBox = new MCombo(RageGroup3, "Yaw", &Options.Rage.AntiAim.Yaw, Vector2D(window->position.x + 440 + 25, window->position.y + 190 + 110), AAYaws, &Options.Menu.AntiYawCombo);
	std::vector<std::string> AAPitchs = { "Off", "Emotion", "Down", "Fake-Down", "Up", "Fake-Up", "Normal" };
	MCombo* PitchComboBox = new MCombo(RageGroup3, "Pitch", &Options.Rage.AntiAim.Pitch, Vector2D(window->position.x + 440 + 25, window->position.y + 190 + 70), AAPitchs, &Options.Menu.AntiPitchCombo);

	Rage->AddChildControl(RageEnabled);
	Rage->AddChildControl(AutowallToggle);
	Rage->AddChildControl(AShoot);
	Rage->AddChildControl(AScope);
	Rage->AddChildControl(EnemyOnlyR);
	Rage->AddChildControl(SAim);
	Rage->AddChildControl(MinDmg);
	Rage->AddChildControl(HitC);
	Rage->AddChildControl(HitCSniper);
	Rage->AddChildControl(HitchanceAmt);
	Rage->AddChildControl(AAEnable);
	Rage->AddChildControl(SpinSpeed);
	Rage->AddChildControl(YawComboBox);
	Rage->AddChildControl(PitchComboBox);
	Rage->AddChildControl(fov);
	Rage->AddChildControl(Targeting);
	Rage->AddChildControl(HitScan);
	Rage->AddChildControl(Multipoint);
	Rage->AddChildControl(Hitbox);
	Rage->AddChildControl(AAAToggle);


	// Simple slider value fixes cuz im lazy to set them to 1 in options
	if (Options.Rage.AutoWall.MinDamage < 1 || Options.Rage.AntiAim.spinSpeed < 1)
	{
		Options.Rage.AutoWall.MinDamage = 1;
		Options.Rage.AntiAim.spinSpeed = 1;
	}

	//PlayerModsTab
	MCheckbox* BHop = new MCheckbox(pModsGroup, "Bunnyhop", &Options.PlayerMods.bhop.Enabled, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 30));
	MCheckbox* RAutoStrafe = new MCheckbox(pModsGroup, "Autostrafe", &Options.PlayerMods.AutoStrafe.REnabled, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 50));
	MCheckbox* AirStuck = new MCheckbox(pModsGroup, "Airstuck", &Options.PlayerMods.AirStuck.Enabled, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 70));
	MKey* AirStuckKey = new MKey(pModsGroup, "Airstuck Key", &Options.PlayerMods.AirStuck.Key, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 90));
	MCheckbox* AutoAccept = new MCheckbox(VisualGroup, "Auto Accept", &Options.Visuals.ESP.AutoAccept, Vector2D(window->position.x + 460 + 25, window->position.y + 30 + 140));
	MCheckbox* RevealAll = new MCheckbox(VisualGroup, "Reveal Ranks", &Options.Visuals.ESP.RankESP, Vector2D(window->position.x + 460 + 25, window->position.y + 30 + 120));

	MCheckbox* Fakelag = new MCheckbox(pModsGroup, "Fakelag", &Options.PlayerMods.FakeLag.Enabled, Vector2D(window->position.x + 460 + 25, window->position.y + 30 + 30));
	MSlider* FLSend = new MSlider (pModsGroup, "Send", &Options.PlayerMods.FakeLag.Send,  0, 15, 150, Vector2D(window->position.x + 460 + 25, window->position.y + 30 + 50));
	MSlider* FLChoke = new MSlider (pModsGroup, "Choke", &Options.PlayerMods.FakeLag.Choke,  0, 15, 150, Vector2D(window->position.x + 460 + 25, window->position.y + 30 + 80));
	
	PlayerMods->AddChildControl(BHop);
	PlayerMods->AddChildControl(RAutoStrafe);
	PlayerMods->AddChildControl(AirStuck);
	PlayerMods->AddChildControl(AirStuckKey);
	PlayerMods->AddChildControl(AutoAccept);
	PlayerMods->AddChildControl(RevealAll);
	PlayerMods->AddChildControl(Fakelag);
	PlayerMods->AddChildControl(FLSend);
	PlayerMods->AddChildControl(FLChoke);

	//VisualsTab
	MCheckbox* ESPEnabled = new MCheckbox(VisualGroup, "Enabled", &Options.Visuals.ESP.Enabled, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 30));
	MCheckbox* EnemyOnly = new MCheckbox(VisualGroup, "EnemyOnly", &Options.Visuals.ESP.EnemyOnly, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 50));
	MCheckbox* Box = new MCheckbox(VisualGroup, "Box", &Options.Visuals.ESP.Box, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 70));
	MSlider*  HealthOption = new MSlider(VisualGroup, "Health", &Options.Visuals.ESP.Health, 0, 3, 150, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 90));
	MCheckbox* Name = new MCheckbox(VisualGroup, "Name", &Options.Visuals.ESP.Name, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 130));
	MCheckbox* Weapon = new MCheckbox(VisualGroup, "Weapon", &Options.Visuals.ESP.Weapon, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 150));
	MCheckbox* Ammo = new MCheckbox(VisualGroup, "Ammo", &Options.Visuals.ESP.Ammo, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 170));
	MCheckbox* C4Timer = new MCheckbox(VisualGroup, "C4 Timer", &Options.Visuals.ESP.DrawBomb, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 190));
	MCheckbox* C4Defuser = new MCheckbox(VisualGroup, "C4 Defuser", &Options.Visuals.ESP.DrawDefuser, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 210));
	MCheckbox* PChams = new MCheckbox(VisualGroup, "Chams", &Options.Visuals.ESP.Chams, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 230));
	MCheckbox* pInfo = new MCheckbox(VisualGroup, "Player Info", &Options.Visuals.ESP.pInfo, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 250));

	MSlider* FOVView = new MSlider(VisualGroup, "Viewmodel FOV", &Options.Visuals.ESP.FOV2, 0, 40, 150, Vector2D(window->position.x + 460 + 25, window->position.y + 30 + 30));
	MSlider*  FOVS = new MSlider(VisualGroup, "FOV", &Options.Visuals.ESP.FOV, 0, 40, 150, Vector2D(window->position.x + 460 + 25, window->position.y + 30 + 60));
	MSlider*  Glow = new MSlider(VisualGroup, "Glow", &Options.Visuals.ESP.DrawGlowS, 0, 2, 150, Vector2D(window->position.x + 460 + 25, window->position.y + 30 + 90));
	
	VisualGroup->AddChildControl(ESPEnabled);
	VisualGroup->AddChildControl(EnemyOnly);
	VisualGroup->AddChildControl(Box);
	VisualGroup->AddChildControl(HealthOption);
	VisualGroup->AddChildControl(Name);
	VisualGroup->AddChildControl(Weapon);
	VisualGroup->AddChildControl(Ammo);
	VisualGroup->AddChildControl(C4Timer);
	VisualGroup->AddChildControl(C4Defuser);
	VisualGroup->AddChildControl(FOVView);
	VisualGroup->AddChildControl(FOVS);
	VisualGroup->AddChildControl(Glow);
	VisualGroup->AddChildControl(PChams);
	VisualGroup->AddChildControl(pInfo);

	//RemovalsTab
	std::vector<std::string> handType = { "Off", "Invisible", "Wireframe", "Translucent" };
	MCombo* handTypeCombo = new MCombo(RemovalGroup, "Hands", &Options.Visuals.Removals.Hands, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 30), handType, &Options.Menu.CustomHandCombo);
	MCheckbox* NWeapon = new MCheckbox(RemovalGroup, "No Weapon", &Options.Visuals.Removals.Weapon, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 50));
	MCheckbox* NoVis = new MCheckbox(RemovalGroup, "No Visual Recoil", &Options.Visuals.Removals.VisualRecoil, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 70));
	MCheckbox* NoRecoil = new MCheckbox(RemovalGroup, "No Recoil", &Options.Visuals.Removals.Recoil, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 90));
	MCheckbox* NoFlash = new MCheckbox(RemovalGroup, "No Flash", &Options.Visuals.Removals.Flash, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 110));
	MCheckbox* NoSmoke = new MCheckbox(RemovalGroup, "No Smoke", &Options.Visuals.Removals.Smoke, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 130));
	
	Removals->AddChildControl(NWeapon);
	Removals->AddChildControl(NoVis);
	Removals->AddChildControl(NoRecoil);
	Removals->AddChildControl(NoFlash);
	Removals->AddChildControl(handTypeCombo);

	// Aesthetics tab
	MCheckbox* ChatSpamRegular = new MCheckbox(AestheticsGroup, "Chat Spam", &Options.PlayerMods.ChatSpamRegular.Enabled, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 60));
	
	MCheckbox* fpsBoost = new MCheckbox(AestheticsGroup, "FPS Boost", &Options.Visuals.ESP.fpsBoost, Vector2D(window->position.x + 460 + 25, window->position.y + 30 + 60));

	MCheckbox* cumMode = new MCheckbox(AestheticsGroup, "Cum-Mode", &Options.Visuals.ESP.cumMode, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 80));

	std::vector<std::string> ClanTagType = { "Off", "Izumi.pro - slide", "Izumi.pro - build", "Erection", "Izumi.pro - static", "Invisible" };
	MCombo* ClanTags = new MCombo(AestheticsGroup, "Clantag Changer", &Options.PlayerMods.ClanTags.Enabled, Vector2D(window->position.x + 460 + 25, window->position.y + 30 + 30), ClanTagType, &Options.Menu.ClanTagCombo);

	std::vector<std::string> custNames = { "off", "Invisible", "I Z U M I", "I Z U M I . P R O", "I Z U M I Colored", "Overflow" };
	MCombo* customName = new MCombo(AestheticsGroup, "Name Changer", &Options.PlayerMods.Names.Enabled, Vector2D(window->position.x + 200 + 25, window->position.y + 30 + 30), custNames, &Options.Menu.CustomNameCombo);
	
	Aesthetics->AddChildControl(cumMode);
	Aesthetics->AddChildControl(fpsBoost);
	Aesthetics->AddChildControl(ClanTags);
	Aesthetics->AddChildControl(ChatSpamRegular);
	Aesthetics->AddChildControl(customName);

	//Configs
	MButton* Load = new MButton(XorStr("Load"), Vector2D(window->position.x + 20, window->position.y + 360), Vector2D(170, 10));
	MButton* Save = new MButton(XorStr("Save"), Vector2D(window->position.x + 20, window->position.y + 380), Vector2D(170, 10));
	std::vector<std::string> Configs = { XorStr("Default"), XorStr("Rage") };
	MComboNoParent* ConfigsType = new MComboNoParent("Selected Config", &Options.Config.Current, Vector2D(window->position.x + 20, window->position.y + 340), Configs, &Options.Menu.ConfigCombo);
	
	if (Input->Clicked(window->position.x + 20, window->position.y + 360, 170, 10) && !Options.Menu.ConfigCombo);
	if (Input->Clicked(window->position.x + 20, window->position.y + 380, 170, 10) && !Options.Menu.ConfigCombo);

	//Menu tab locations
	window->AddChildControl(Title);
	window->AddChildControl(Rage);
	window->AddChildControl(Visual);
	window->AddChildControl(Removals);
	window->AddChildControl(PlayerMods);
	window->AddChildControl(Aesthetics);
	window->AddChildControl(Load);
	window->AddChildControl(Save);
	window->AddChildControl(ConfigsType);
	
	Rage->active = Options.Menu.Ragebot;
	PlayerMods->active = Options.Menu.PlayerMods;
	Visual->active = Options.Menu.Visuals;
	Removals->active = Options.Menu.Removals;
	Aesthetics->active = Options.Menu.Aesthetics;

	// Simple check to force rage tab active on load
	if (Options.Menu.Ragebot == false && Options.Menu.PlayerMods == false && Options.Menu.Visuals == false && Options.Menu.Removals == false && Options.Menu.Aesthetics == false)
	{
		Options.Menu.Ragebot = true;
	}

	if (Input->Clicked(window->position.x + 20, window->position.y + 55, GetStringWidth(Impact, "Rage"), 30))
	{
		Options.Menu.Ragebot = true;
		Options.Menu.PlayerMods = false;
		Options.Menu.Visuals = false;
		Options.Menu.Removals = false;
		Options.Menu.Aesthetics = false;
	}
	if (Input->Clicked(window->position.x + 20, window->position.y + 85, GetStringWidth(Impact, "Visuals"), 30))
	{
		Options.Menu.PlayerMods = false;
		Options.Menu.Ragebot = false;
		Options.Menu.Visuals = true;
		Options.Menu.Removals = false;
		Options.Menu.Aesthetics = false;
	}
	if (Input->Clicked(window->position.x + 20, window->position.y + 115, GetStringWidth(Impact, "Removals"), 30))
	{
		Options.Menu.Visuals = false;
		Options.Menu.PlayerMods = false;
		Options.Menu.Ragebot = false;
		Options.Menu.Removals = true;
		Options.Menu.Aesthetics = false;
	}
	if (Input->Clicked(window->position.x + 20, window->position.y + 145, GetStringWidth(Impact, "PlayerMods"), 30))
	{
		Options.Menu.Removals = false;
		Options.Menu.Visuals = false;
		Options.Menu.PlayerMods = true;
		Options.Menu.Ragebot = false;
		Options.Menu.Aesthetics = false;
	}
	if (Input->Clicked(window->position.x + 20, window->position.y + 175, GetStringWidth(Impact, "Aesthetics"), 30))
	{
		Options.Menu.Removals = false;
		Options.Menu.Visuals = false;
		Options.Menu.PlayerMods = false;
		Options.Menu.Ragebot = false;
		Options.Menu.Aesthetics = true;
	}

	window->Draw();
}


void __fastcall Game::hkdPaintTraverse(void* ecx, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	oPaintTraverse(ecx, edx, vguiPanel, forceRepaint, allowForce);
	static unsigned int MatTopPanel;
	if (!MatTopPanel)
	{
		auto cName = VPanel->GetName(vguiPanel);
		if (!strcmp(cName, XorStr("MatSystemTopPanel")))
		{
			MatTopPanel = vguiPanel;
		}
	}
	if (MatTopPanel == vguiPanel)
	{
		static auto doOnce = false;
		if (!doOnce)
		{
			Cvar->ConsoleColorPrintf(Color(37, 195, 219,255), XorStr("Infamy V2\n"));
			Cvar->ConsoleColorPrintf(Color(37, 195, 219,255), XorStr("Build Date: %s %s\n"), __DATE__, __TIME__);
			doOnce = true;
		}
		
		//Watermark
		static float rainbow;
		rainbow += 0.002f;
		if (rainbow > 1.f) rainbow = 0.f;
		if (Options.Visuals.ESP.Watermark)
		{
			DrawString(Watermark, 8, 11, Color::FromHSB(rainbow, 0.60f, 0.75f), FONT_LEFT, XorStr("Infamy V2"));
			DrawString(Watermark, 8, 30, Color::FromHSB(rainbow, 0.60f, 0.75f), FONT_LEFT, XorStr("Build Date: %s"), __DATE__);
		}

		
		if (Engine->IsInGame() && Engine->IsConnected() && !Engine->IsTakingScreenshot())
		{
			Global::LocalPlayer = GetLocalPlayer();
			if (Options.Visuals.ESP.Enabled)
			{
				g_pESP->getScreenSize();
				g_pESP->think(Global::LocalPlayer);
			}
			Global::CurrTime = Globals->interval_per_tick * (Global::LocalPlayer->GetTickBase() + 1);
			
			//RankESP
			if (Options.Visuals.ESP.Enabled && Options.Visuals.ESP.RankESP)
			{
				using tServerRankRevealAllFn = bool(__cdecl*)(float*);
				static auto ServerRankRevealAll = reinterpret_cast<tServerRankRevealAllFn>(Memory::FindPatternIDA("client.dll", "55 8B EC 8B 0D ? ? ? ? 68"));
				static float fArray[3] = { 0,0,0 };
				ServerRankRevealAll(fArray);
			}
			
			//C4Stuff
			if (Options.Visuals.ESP.Enabled && Options.Visuals.ESP.DrawBomb)
			{
				static itemTimer bombTimer(40.f);
				if (!Global::isBombPlanted) { //if bomb isn't planted, just reset the time to now
					bombTimer.setMaxTime(40.f);
					bombTimer.startTimer();
				}
				else { //if planted, stop resetting time and draw timer
					int screenw, screenh;
					Engine->GetScreenSize(screenw, screenh);
					auto x = 16;
					auto y = screenh - 70;
					char dettime[64];
					XorCompileTime::w_sprintf_s(dettime, 64, "Bomb Explodes in: %.1f", ((int)(100 * bombTimer.getTimeRemaining()) / 100.0));
					DrawString(BombStuff, x, y, Color(37, 195, 219,255), FONT_LEFT, dettime);
				}
			}

			DrawRect(1, 1, 1, 1, Color(0, 0, 0, 1)); // Fix for menu not drawing without net_graph 1
			
		}
		static auto isDown = false;
		static auto isClicked = false;
		if (InputSystem->IsButtonDown(KEY_INSERT))
		{
			isClicked = false;
			isDown = true;
		}
		else if (!InputSystem->IsButtonDown(KEY_INSERT) && isDown)
		{
			isClicked = true;
			isDown = false;
		}
		else
		{
			isClicked = false;
			isDown = false;
		}
		if (isClicked)
		{
			Options.Menu.Opened = !Options.Menu.Opened;

			static auto cl_mouseenable = Cvar->FindVar(XorStr("cl_mouseenable"));

			cl_mouseenable->SetValue(!Options.Menu.Opened);
		}
		if (Options.Menu.Opened)
		{
			DrawMenu();
			DrawMouse();
		}
	}
}