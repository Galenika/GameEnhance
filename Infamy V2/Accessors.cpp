#include "Accessors.h"
#include <vector>

cManagers* g_pManagers = new cManagers;
cpSilent* g_pSilent = new cpSilent;

void cManagers::CreateMove(CUserCmd* pCmd, bool& bSendPackets)
{
	// Call Pregame stuff here
	//call Create Move Features Here
	if (!Global::LocalPlayer) return;
	if (!Global::LocalPlayer->GetAlive()) return;
	// Testing new convar stuff
	static bool RegisterCommandOnce;

	if (!RegisterCommandOnce)
	{
		MinspecCvar* fovbypass = new MinspecCvar("viewmodel_fov", "inf_fov", 60);
		MinspecCvar* graybypass = new MinspecCvar("mat_drawgray", "boost_fps", 0);
		MinspecCvar* postbypass = new MinspecCvar("mat_postprocess_enable", "booost_fps", 1);
		RegisterCommandOnce = true;
	}
	ConVar* ViewModelFOV = Cvar->FindVar("inf_fov");
	ConVar* cum = Cvar->FindVar("boost_fps");
	ConVar* post = Cvar->FindVar("booost_fps");

	if (Options.PlayerMods.ClanTags.Enabled >= 1)
		g_pPlayer->ClanTagChanger();

	if (Options.PlayerMods.ChatSpamRegular.Enabled)
		g_pPlayer->ChatSpamRegular();

	if (Options.PlayerMods.Names.Enabled >= 1)
		g_pPlayer->NameChanger();

	if (Options.Visuals.ESP.NoName)
		g_pPlayer->NoName();

	if (Options.Visuals.ESP.Enabled && Options.Visuals.ESP.FOV2 > 1)
	{
		int newViewModel = 60 + Options.Visuals.ESP.FOV2;
		ViewModelFOV->SetValue(newViewModel);
	}


	// fps boost
	if (Options.Visuals.ESP.fpsBoost == 1)
	{
		post->SetValue(0);
	}
	else if(Options.Visuals.ESP.fpsBoost == 0)
	{
		post->SetValue(1);
	}
	

	// cum-mode
	if (Options.Visuals.ESP.cumMode == 1)
	{
		cum->SetValue(1);
	}
	else if (Options.Visuals.ESP.cumMode == 0)
	{
		cum->SetValue(0);
	}

	//auto LocalWepID = Global::LocalPlayer->GetWeaponID();
	auto LocalWep = Global::LocalPlayer->GetWeapon();
	if (LocalWep)
		g_pSilent->Setup();

	if (Options.PlayerMods.bhop.Enabled)
		g_pPlayer->Bhop(pCmd);

	if (Options.PlayerMods.AutoStrafe.REnabled)
		g_pPlayer->RageAutoStrafe(pCmd);
	

	static auto WasFiring = false;
	auto WeaponInfo = LocalWep->GetCSWpnData();
	if (LocalWep)

	if (LocalWep)
		g_pRage->Invoke(pCmd, bSendPackets, g_pSilent->bCanFire);

	if (pCmd->buttons & IN_ATTACK && !g_pRage->IsAimbotting && Options.Visuals.Removals.Recoil)
		pCmd->viewangles -= Global::LocalPlayer->GetPunch() * 2.f;

	if (Options.PlayerMods.AirStuck.Enabled)
		g_pPlayer->AirStuck(pCmd);

	if (Options.PlayerMods.FakeLag.Enabled)
		g_pPlayer->FakeLag(pCmd, bSendPackets);

	g_pSilent->Handle(bSendPackets);

	if (Options.AntiCheat.MM || Options.AntiCheat.VAC)
		this->Clamp(pCmd);
}

void cManagers::Painttraverse(HFont hFont)
{
	//call Drawn Features Here
}

void cManagers::Clamp(CUserCmd* pCmd)
{
	pCmd->viewangles.Normalize();
	if (pCmd->forwardmove > 450.f)
		pCmd->forwardmove = 450.f;
	else if (pCmd->forwardmove < -450.f)
		pCmd->forwardmove = -450.f;

	if (pCmd->sidemove > 450.f)
		pCmd->sidemove = 450.f;
	else if (pCmd->sidemove < -450.f)
		pCmd->sidemove = -450.f;

	if (pCmd->upmove > 320.f)
		pCmd->upmove = 320.f;
	else if (pCmd->upmove < -320.f)
		pCmd->upmove = -320.f;

	pCmd->viewangles.Clamp();
}

void cpSilent::Setup()
{
	bCanFire = true;
	flCurTime = Globals->interval_per_tick * (Global::LocalPlayer->GetTickBase() + 1);
	flNextPrimary = Global::LocalPlayer->GetWeapon()->GetNextPrimaryAttack();
	if (flNextPrimary > flCurTime || bOldBullet)
		bCanFire = false;
	if (!(flNextPrimary > flCurTime))
		bOldBullet = false;
}

void cpSilent::Handle(bool& bSendPackets)
{
	if (bCanFire && Global::pCmd->buttons & IN_ATTACK)
	{
		bSendPackets = false;
		iChokedticks++;
	}
	if (iChokedticks > iMaxChokedticks)
		bSendPackets = true;
	if (bSendPackets)
	{
		iChokedticks = 0;
		Global::pCmd->viewangles = Global::oCmd->viewangles;
		Global::pCmd->forwardmove = Global::oCmd->forwardmove;
		Global::pCmd->sidemove = Global::oCmd->sidemove;
	}
}

itemTimer::itemTimer() {
	maxTime = 0;
}

itemTimer::itemTimer(float _maxTime) {
	maxTime = _maxTime;
}

float itemTimer::getTimeRemaining() {
	auto time = (timeStarted - (float(clock()) / float(CLOCKS_PER_SEC))) + maxTime;
	return time ? time : 0.00001f; //ensure we don't ever return a 0
}

float itemTimer::getTimeRemainingRatio() {
	return getTimeRemaining() / getMaxTime();
}

float itemTimer::getMaxTime() {
	return maxTime ? maxTime : 1; //don't ever return 0
}

void itemTimer::startTimer() {
	timeStarted = float(float(clock()) / float(CLOCKS_PER_SEC));
}

void itemTimer::setMaxTime(float time) {
	maxTime = time;
}

FloatingText::FloatingText(CBaseEntity* attachEnt, float lifetime, int Damage)
{
	TimeCreated = Global::CurrTime;
	ExpireTime = TimeCreated + lifetime;
	pEnt = attachEnt;
	DamageAmt = Damage;
}

void FloatingText::Draw()
{
	auto head = pEnt->GetBonePosition(6);
	Vector screen;

	if (WorldToScreen(head, screen))
	{
		auto lifetime = ExpireTime - TimeCreated;
		auto pct = 1 - ((ExpireTime - Global::CurrTime) / lifetime);
		int offset = pct * 50;
		int y = screen.y - 15 - offset;
	}
}

CTraceObj::CTraceObj(Color colorTrace, unsigned int iMaxLength)
{
	m_colorTrace = colorTrace;
	m_iMaxLength = iMaxLength;
	m_timerPointLock.init();
}