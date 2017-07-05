

#pragma once

#include "GUI.h"
#include "Controls.h"
#include "Xor.h"

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
	CCheckBox AimbotMultiPoint;
	CSlider AimbotMultiPointIndexes;
	CCheckBox AimbotSilentAim;
	CCheckBox AimbotPSilent;
	CCheckBox AimbotAutoPistol;
	CCheckBox AimbotPVSFix;
	CCheckBox AimbotAimStep;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CCheckBox AimbotChicken;

	// Target Selection Settings
	CGroupBox TargetGroup;
	CComboBox TargetSelection;
	CCheckBox TargetFriendlyFire;
	CComboBox TargetHitbox;
	CComboBox TargetHitscan;
	CSlider TargetPointScale;
	CSlider TargetMinDamage;

	// Accuracy Settings
	CGroupBox AccuracyGroup;
	CCheckBox AccuracySpread;
	CCheckBox AccuracyRecoil;
	CCheckBox AccuracyAutoWall;
	CSlider	  AccuracyMinimumDamage;
	CCheckBox AccuracyAutoStop;
	CCheckBox AccuracyPosAdjust;
	CCheckBox AccuracyAutoCrouch;
	CCheckBox AccuracyAutoScope;
	CCheckBox AccuracySpreadLimit;
	CSlider	  AccuracyMinimumSpread;
	CSlider AccuracySmart;
	CSliderFloat AccuracyPointScale;
	CComboBox AccuracyAngleFix;


	//Resolver Settings
	CGroupBox ResolverGroup;
	CCheckBox ResolverON;
	CComboBox ResolverType;
	CCheckBox LBYFix;
	CCheckBox ResolverDebug;
	CComboBox ResolverFLFix;

	//NEW Resolver Settings
	CComboBox ResolverPitch;
	CComboBox ResolverYaw;
	CComboBox ResolverStep;
	CComboBox ResolverSnap;

	// Anti-Aim Settings
	CGroupBox AntiAimGroup;
	CCheckBox AntiAimEnable;
	CComboBox AntiAimPitch;
	CComboBox AntiAimYaw;
	CComboBox AntiAimFakeYaw;
	CSliderFloat AntiAimSpeed;
	CCheckBox AntiAimLBY;
	CCheckBox AntiAimRealAngles;
	CCheckBox AntiAimDynamic;
	CCheckBox AntiAimDormant;
	CCheckBox AntiAimKnife;
	CCheckBox AntiAimLadder;
	CCheckBox AntiAimTarget;
	CComboBox AntiAimDuck;
	CComboBox AntiAimEdge;
	CCheckBox AntiAfk;
	CSlider AntiAimX;
	CSlider AntiAimY;
};

class CLegitBotTab : public CTab
{
public:
	void Setup();

	//Top Group Box
	CGroupBox ActiveGroup;
	CCheckBox LegitActivate;
	CComboBox LegitWeapType;

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

	// Main
	CGroupBox TriggerGroup;
	CCheckBox TriggerEnable;
	CCheckBox TriggerKeyPress;
	CKeyBind  TriggerKeyBind;

	// Main
	CGroupBox WeaponMainGroup;
	CSlider   WeaponMainSpeed;
	CSlider   WeaponMainFoV;
	CCheckBox WeaponMainRecoil;
	CComboBox WeaponMainHitbox;

	// Pistol
	CGroupBox WeaponPistGroup;
	CSlider   WeaponPistSpeed;
	CSlider   WeaponPistFoV;
	CCheckBox WeaponPistRecoil;
	CComboBox WeaponPistHitbox;

	// Sniper
	CGroupBox WeaponSnipGroup;
	CSlider   WeaponSnipSpeed;
	CSlider   WeaponSnipFoV;
	CCheckBox WeaponSnipRecoil;
	CComboBox WeaponSnipHitbox;

};

class CVisualTab : public CTab
{
public:
	void Setup();
	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;

	// Options Settings
	CGroupBox OptionsGroup;
	CCheckBox OptionsBox;
	CSlider OptionsFillBox;
	CCheckBox OptionsName;
	CCheckBox OptionsHealth;
	CCheckBox OptionsArmor;
	CComboBox OptionsWeapon;
	CCheckBox OptionsInfo;
	CCheckBox OptionsDefusing;
	CCheckBox OptionsDistance;
	CComboBox OptionsChams;
	CCheckBox OptionsSkeleton;
	CCheckBox OptionsAimSpot;
	CCheckBox OptionsCompRank;
	CCheckBox FiltersPlayers;
	CCheckBox FiltersEnemiesOnly;
	CCheckBox FiltersC4;
	CCheckBox FiltersWeapons;

	// Filters Settings
	CGroupBox EffectsGroup;
	CComboBox EffectMap;
	CCheckBox EffectNightMode;
	CCheckBox EffectASUS;
	CCheckBox EffectParticles;
	CCheckBox EffectNoSky;
	CCheckBox EffectNoVisualRecoil;
	CCheckBox EffectNoScope;
	CCheckBox EffectNoFlash;
	CComboBox EffectThirdPerson;
	CSlider EffectFOV;
	CSlider EffectViewmodelFOV;

	// Other Settings
	CGroupBox OtherGroup;
	CCheckBox OtherFOVCrosshair;
	CCheckBox OtherHitmarker;
	CCheckBox OtherHitmarkerDMG;
	CSlider OtherHitmarkerDuration;
	CCheckBox OtherSpreadCrosshair;
	CComboBox OtherHandFX;
	CComboBox OtherWeaponFX;
	CCheckBox OtherWatermark;
	CCheckBox OtherLBYIndicator;
	CCheckBox OtherSniperCrosshair;
	CCheckBox OtherSpectatorList;
	CCheckBox OtherMCTextures;
};
class CColorsTab : public CTab
{
public:
	void Setup();
	CGroupBox ControlGroup;
	CComboBox ControlColor;

	// CT IS VISUAL ESP
	CGroupBox CTVisESP;
	CSlider CTVisESPR; //red
	CSlider CTVisESPG; //green
	CSlider CTVisESPB; //blue

	// T IS VISUAL ESP
	CGroupBox TVisESP;
	CSlider TVisESPR; //red
	CSlider TVisESPG; //green
	CSlider TVisESPB; //blue

	// CT NOT VISIBLE ESP
	CGroupBox CTInvisESP;
	CSlider CTInvisESPR; //red
	CSlider CTInvisESPG; //green
	CSlider CTInvisESPB; //blue

	// T NOT VISIBLE ESP
	CGroupBox TInvisESP;
	CSlider TInvisESPR; //red
	CSlider TInvisESPG; //green
	CSlider TInvisESPB; //blue

	// CT IS VISUAL CHAMS
	CGroupBox CTVisChams;
	CSlider CTVisChamsR; //red
	CSlider CTVisChamsG; //green
	CSlider CTVisChamsB; //blue

	// T IS VISUAL CHAMS
	CGroupBox TVisChams;
	CSlider TVisChamsR; //red
	CSlider TVisChamsG; //green
	CSlider TVisChamsB; //blue

	// CT NOT VISIBLE CHAMS
	CGroupBox CTInvisChams;
	CSlider CTInvisChamsR; //red
	CSlider CTInvisChamsG; //green
	CSlider CTInvisChamsB; //blue

	// T NOT VISIBLE CHAMS
	CGroupBox TInvisChams;
	CSlider TInvisChamsR; //red
	CSlider TInvisChamsG; //green
	CSlider TInvisChamsB; //blue
};
class CMiscTab : public CTab
{
public:
	void Setup();

	CGroupBox ClanTagGroup;
	CCheckBox ClanTagEnable;
	CComboBox ClanTagType;
	CComboBox ClanTagChoice;
	CSlider ClanTagSpeed;

	// CircleStrafe Settings
	CGroupBox CircleStrafeGroup;
	CCheckBox CircleEnable;
	CSliderFloat CircleFactor;

	// Fake Lag Settings
	CGroupBox FakeLagGroup;
	CCheckBox FakeLagEnable;
	CSlider   FakeLagAmount;
	CCheckBox FakeLagWhileShooting;
	CComboBox FakeLagType;
	CCheckBox FakeLagInAir;
	CKeyBind SlowMotion;
	CSlider SlowMoAmount;

	// Glove Changer
	CGroupBox GloveGroup;
	CComboBox GloveModel;
	CComboBox GloveSkin;
	CButton GloveActive;

	// Knife Changer
	CGroupBox KnifeGroup;
	CComboBox KnifeModel;
	CComboBox KnifeSkin;
	CButton   KnifeApply;

	// Other Settings
	CGroupBox OtherGroup;
	CCheckBox OtherAutoJump;
	CComboBox OtherAutoStrafe;
	CCheckBox OtherSafeMode;
	CCheckBox OtherNameSpam;
	CComboBox OtherChatSpam;
	CKeyBind  OtherAirStuck;
	CCheckBox  OtherSpectators;
	CCheckBox  OtherServerLag;
	CSlider OtherFakeLagActive;
	CCheckBox OtherFakeLag;
};

class AyyWareWindow : public CWindow
{
public:
	void Setup();

	CRageBotTab RageBotTab;
	CLegitBotTab LegitBotTab;
	CVisualTab VisualsTab;
	CMiscTab MiscTab;
	CColorsTab ColorsTab;
	CButton SaveButton;
	CButton LoadButton;
	CButton UnloadButton;
};

namespace Menu
{
	void SetupMenu();
	void DoUIFrame();

	extern AyyWareWindow Window;
};