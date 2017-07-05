#pragma once

#include "Gui.h"
#include "Controls.h"

class CRageBotTab : public CTab
{
public:
	void Setup();

	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;

	// Aimbot Settings
	CGroupBox AimbotGroup;

	CCheckBox AimbotEnable;
	CCheckBox AimbotAutoFire;
	CSlider	  AimbotFov;
	CCheckBox AimbotSilentAim;
	CComboBox serversided;
	CCheckBox AimbotPSilent;
	CCheckBox AimbotAutoPistol;
	CCheckBox AimbotAimStep;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CCheckBox AutoAwpBody;
	CCheckBox AntiSpawnProtection;

	// Target Selection Settings
	CGroupBox TargetGroup;

	CComboBox TargetSelection;
	CCheckBox TargetFriendlyFire;
	CComboBox TargetHitbox;
	CComboBox TargetHitscan;
	CCheckBox AccuracyAutoWall;
	CSlider	  AccuracyMinimumDamage;

	// Accuracy Settings
	CGroupBox AccuracyGroup;

	CCheckBox AccuracySpread;
	CCheckBox AccuracyRecoil;
	CCheckBox AccuracyAutoStop;
	CCheckBox AccuracyAutoCrouch;
	CCheckBox AccuracyAutoScope;
	CCheckBox AccuracySpreadLimit;
	CSlider	  AccuracyMinimumSpread;
	CCheckBox AccuracyAngleFix;
	CComboBox ResolverX;
	CCheckBox HitChance;
	CSlider HitChanceAmount;
	CCheckBox AntiAntiAim;
	CCheckBox AntiUntrusted;
	CCheckBox OtherEnginePrediction;
	CComboBox PreferBodyAim;
	CCheckBox LagFix;
	// Anti-Aim Settings
	CGroupBox AntiAimGroup;

	CCheckBox AntiAimEnable;
	CComboBox AntiAimPitch;
	CComboBox AntiAimYaw;
	CTextField AntiAimCustomPitch;
	CTextField AntiAimCustomYaw;
	CCheckBox AtTargets;
	CCheckBox EdgeAntiAim;
	CCheckBox LispAntiAim;
	CCheckBox ThirdPersonAA;
	CComboBox zAngle;
};

class CLegitBotTab : public CTab
{
public:
	void Setup();

	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;

	// Aimbot Settings
	CGroupBox AimbotGroup;

	CCheckBox AimbotEnable;
	CCheckBox AimbotAutoFire;
	CCheckBox AimbotFriendlyFire;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CCheckBox AimbotAutoPistol;
	CCheckBox AimbotChicken;
	CSlider   Innacuracy;
	CCheckBox AimbotSmokeCheck;

	// Main
	CGroupBox TriggerGroup;

	CCheckBox TriggerEnable;
	CCheckBox TriggerKeyPress;
	CKeyBind  TriggerKeyBind;
	CCheckBox TriggerHitChance;
	CSlider   TriggerHitChanceAmmount;
	CSlider   TriggerDelay;
	CSlider2   TriggerBurst;
	CSlider   TriggerBreak;
	CCheckBox TriggerRecoil;

	// Trigger Filter
	CGroupBox TriggerFilterGroup;

	CCheckBox TriggerHead;
	CCheckBox TriggerChest;
	CCheckBox TriggerStomach;
	CCheckBox TriggerArms;
	CCheckBox TriggerLegs;
	CCheckBox TriggerTeammates;

	// Rifles
	CGroupBox WeaponMainGroup;

	CSlider   WeaponMainSpeed;
	CSlider3   WeaponMainFoV;
	CSlider WeaponMainRecoil;
	CComboBox WeaponMainHitbox;
	CSlider WeaponMainAimtime;
	CSlider WeaoponMainStartAimtime;

	// Pistol
	CGroupBox WeaponPistGroup;

	CSlider   WeaponPistSpeed;
	CSlider3   WeaponPistFoV;
	CSlider WeaponPistRecoil;
	CComboBox WeaponPistHitbox;
	CSlider WeaponPistAimtime;
	CSlider WeaoponPistStartAimtime;

	// Sniper
	CGroupBox WeaponSnipGroup;

	CSlider   WeaponSnipSpeed;
	CSlider3   WeaponSnipFoV;
	CSlider WeaponSnipRecoil;
	CComboBox WeaponSnipHitbox;
	CSlider WeaponSnipAimtime;
	CSlider WeaoponSnipStartAimtime;

	// MPs
	CGroupBox WeaponMpGroup;

	CSlider   WeaponMpSpeed;
	CSlider3   WeaponMpFoV;
	CSlider WeaponMpRecoil;
	CComboBox WeaponMpHitbox;
	CSlider WeaponMpAimtime;
	CSlider WeaoponMpStartAimtime;

	// Shotguns
	CGroupBox WeaponShotgunGroup;

	CSlider   WeaponShotgunSpeed;
	CSlider3   WeaponShotgunFoV;
	CSlider WeaponShotgunRecoil;
	CComboBox WeaponShotgunHitbox;
	CSlider WeaponShotgunAimtime;
	CSlider WeaoponShotgunStartAimtime;

	// Machineguns
	CGroupBox WeaponMGGroup;

	CSlider   WeaponMGSpeed;
	CSlider3   WeaponMGFoV;
	CSlider WeaponMGRecoil;
	CComboBox WeaponMGHitbox;
	CSlider WeaponMGAimtime;
	CSlider WeaoponMGStartAimtime;
};

class CColorsTab : public CTab
{
public:
	void Setup();

	//Color's
	CGroupBox ColorsGroup;
	CSlider CTColorVisR;
	CSlider CTColorVisG;
	CSlider CTColorVisB;

	CSlider CTColorNoVisR;
	CSlider CTColorNoVisG;
	CSlider CTColorNoVisB;

	CLabel ColorSpacer;

	CSlider TColorVisR;
	CSlider TColorVisG;
	CSlider TColorVisB;

	CSlider TColorNoVisR;
	CSlider TColorNoVisG;
	CSlider TColorNoVisB;

	CGroupBox ConfigsGroup;
	CComboBox Cfgs;
	CCheckBox Load1337;
	CCheckBox Save1337;

	CSlider GlowR;
	CSlider GlowG;
	CSlider GlowB;
};

class CMiscTab : public CTab
{
public:
	void Setup();

	// Fake Lag Settings
	CCheckBox FakeLagEnable;
	CSlider2   FakeLagAmount;
	CSlider FakeLagSend;
	CComboBox FakeLagTypes;

	// Teleport
	CKeyBind  OtherAirStuck;
	CCheckBox OtherAirStuckEnabled;
	CCheckBox OtherTeleport;
	CKeyBind  OtherTeleportKey;

	// Recoil Settings
	CGroupBox RecoilGroup;
	CCheckBox RecoilControlSystem;
	CCheckBox OtherNoVisualRecoil;

	// Other Settings
	CGroupBox OtherGroup;
	CCheckBox OtherBunnyhop;
	CCheckBox OtherAutoStrafe;

	// Spammers
	CGroupBox OtherSpammers;
	CTextField2 OtherChatText;
	CCheckBox OtherNameStealer;
	CCheckBox OtherClanChanger;
	CCheckBox ChatSpamRegular;
	CCheckBox ChatSpam;

	// Other
	CGroupBox OtherOtherGroup;
	CCheckBox OtherSafeMode;
	CCheckBox OtherAASAFE;
	CComboBox AutoStrafer1;
	CComboBox ClanTag;
	CCheckBox ThirdPerson;
	CComboBox BunnyHop;
	CCheckBox pSpamerino;
	CCheckBox namespam;
	CCheckBox svcheats;
	CCheckBox Circle;
	CCheckBox Circle1;
	CCheckBox OtherAutoAccept;
	CCheckBox NoInterp;
	CComboBox NameSpam;
	CKeyBind OtherCircle;
	CCheckBox NoName;
	CCheckBox OtherCircleStrafe;
	CCheckBox ServerCrash;
		CComboBox OtherStrafeDir;;
	CTextField2  Customtag;
	CCheckBox OtherEventSpam;
	// Skinsettings
	CGroupBox SkinsettingsGroup;
	CCheckBox StatTrakEnable;
	CTextField StatTrackAmount;
	CTextField2 SkinName;
	CTextField2 KnifeName;
};

class CVisualTab : public CTab
{
public:
	void Setup();

	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;

	// Entities Settings
	CGroupBox OptionsGroup;

	CComboBox OptionsBox;
	CComboBox OptionsName;
	CComboBox OptionsHealth;
	CComboBox OptionsWeapon;
	CCheckBox OptionsInfo;
	CCheckBox OptionsCompRank;

	CComboBox OptionsChams;

	CComboBox OptionsSkeleton;
	CCheckBox OptionsAimSpot;

	CCheckBox OptionsDilights;
	CCheckBox ShowRanks;
	CComboBox OptionsGlow;
	CCheckBox OtherViewDirection;
	CCheckBox OtherNoSky;
	CCheckBox OtherNoFlash;
	CCheckBox OtherNoSmoke;
	CComboBox OtherNoHands;
	CComboBox OtherCrosshair;
	CComboBox OtherCrosshairType;
	CCheckBox OtherFovChanger;
	CSlider OtherHandFov;
	CSlider OtherHandFovSc;
	CCheckBox OtherFovChangerModel;
	CCheckBox FovChangerIgnoreScope;
	CCheckBox OtherSpectators;
	CCheckBox NoFlash;
	CSlider FlashAlpha;
	CCheckBox OtherRadar;
	CSlider2  OtherRadarSize;
	CCheckBox OtherWatermark;
	CSlider GlowZ;
	// Filters Settings
	CGroupBox FiltersGroup;
	CCheckBox FiltersAll;
	CCheckBox FiltersPlayers;
	CCheckBox FiltersEnemiesOnly;
	CCheckBox FiltersWeapons;
	CCheckBox FiltersChickens;
	CCheckBox FiltersC4;
	CCheckBox Grenades;

};

class CSkinchangerTab : public CTab
{
public:
	void Setup();

	// Knife Changer/Skin Changer
	CLabel SkinActive;
	CCheckBox SkinEnable;
	CButton   SkinApply;

	// Knife
	CGroupBox KnifeGroup;
	CComboBox KnifeModel;
	CComboBox KnifeSkin;

	// Pistols
	CGroupBox PistolGroup;
	CComboBox GLOCKSkin;
	CComboBox USPSSkin;
	CComboBox DEAGLESkin;
	CComboBox MAGNUMSkin;
	CComboBox DUALSSkin;
	CComboBox FIVESEVENSkin;
	CComboBox TECNINESkin;
	CComboBox P2000Skin;
	CComboBox P250Skin;

	// Rifles
	CGroupBox Riflegroup;
	CComboBox M41SSkin;
	CComboBox M4A4Skin;
	CComboBox AK47Skin;
	CComboBox AUGSkin;
	CComboBox FAMASSkin;
	CComboBox GALILSkin;
	CComboBox SG553Skin;
	CComboBox MAC10Skin;
	CComboBox P90Skin;
	CComboBox UMP45Skin;
	CComboBox BIZONSkin;
	CComboBox MP7Skin;
	CComboBox MP9Skin;


	// Machineguns
	CGroupBox MachinegunsGroup;
	CComboBox NEGEVSkin;
	CComboBox M249Skin;

	// Snipers
	CGroupBox Snipergroup;
	CComboBox SCAR20Skin;
	CComboBox G3SG1Skin;
	CComboBox SSG08Skin;
	CComboBox AWPSkin;

	// Shotguns
	CGroupBox Shotgungroup;
	CComboBox MAG7Skin;
	CComboBox XM1014Skin;
	CComboBox SAWEDOFFSkin;
	CComboBox NOVASkin;

};

class MainWindow : public CWindow
{
public:
	void Setup();

	CRageBotTab RageBotTab;
	CLegitBotTab LegitBotTab;
	CVisualTab VisualsTab;
	CMiscTab MiscTab;
	CColorsTab ColorsTab;
	CSkinchangerTab SkinchangerTab;

	CButton SaveButton1;
	CButton LoadButton1;
	CButton SaveButton;
	CButton LoadButton;
	CButton Load;
	CButton Save;
	CButton UnloadButton;
	CButton PanicButton;
};

class RadarWindow : public CWindowRadar
{
public:
	void Setup();

};
namespace Menu
{
	void SetupMenu();
	void DoUIFrame();

	extern MainWindow Window;
	extern RadarWindow WindowRadar;
};