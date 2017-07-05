#include "Cheats.h"
cPlayer* g_pPlayer = new cPlayer;

void cPlayer::Bhop(CUserCmd* pCmd)
{
	static auto bJumped = false;
	static auto bFake = false;
	if (!bJumped && bFake)
	{
		bFake = false;
		pCmd->buttons |= IN_JUMP;
	}
	else if (pCmd->buttons & IN_JUMP)
	{
		if (Global::LocalPlayer->GetFlags() & FL_ONGROUND)
		{
			bJumped = true;
			bFake = true;
		}
		else
		{
			pCmd->buttons &= ~IN_JUMP;
			bJumped = false;
		}
	}
	else
	{
		bJumped = false;
		bFake = false;
	}
}

template<class T, class U>

T Clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

//ThirdPerson
void cPlayer::ForceThirdPerson(CUserCmd* pCmd)
{
	static auto ThirdPerson = false;

	if (Engine->IsInGame() && Engine->IsConnected())
	{
		if (Global::LocalPlayer)
		{

			if (Options.Visuals.ESP.ThirdPerson)
			{
				auto LifeState = Global::LocalPlayer->GetAlive();
				static auto oLifeState = !LifeState;

				if (oLifeState != LifeState)
				{
					if (LifeState)
						Engine->ExecuteClientCmd(XorStr("thirdperson"));
					else
						Engine->ExecuteClientCmd(XorStr("firstperson"));
				}
				oLifeState = LifeState;
				ThirdPerson = true;
			}


		}
	}
	if (!Options.Visuals.ESP.ThirdPerson && ThirdPerson)
	{
		ThirdPerson = false;
		Engine->ExecuteClientCmd(XorStr("firstperson"));
	}
}

//FakeLag
void cPlayer::FakeLag(CUserCmd* pCmd, bool& bSendPacket) 
{
	if (pCmd->buttons & IN_ATTACK) return;
	if (Global::LocalPlayer->GetVelocity() == Vector(0, 0, 0)) return;
	static int iTicks;
	if (iTicks > Options.PlayerMods.FakeLag.Choke)
		iTicks = 0;
	bSendPacket = iTicks < Options.PlayerMods.FakeLag.Send;
	iTicks++;
}

//AirStuck
void cPlayer::AirStuck(CUserCmd* pCmd)
{
	static auto bAirStucked = false;
	if (pCmd->buttons & IN_ATTACK) return;
	if (GetAsyncKeyState(Options.PlayerMods.AirStuck.Key) & 1)
		bAirStucked = !bAirStucked;
	if (bAirStucked)
		pCmd->tick_count = 0xFFFFFFF;
}

//CvarSpoof
void cPlayer::SpoofConvars()
{
	auto svcheatsspoof = new MinspecCvar(XorStr("sv_cheats"), XorStr("inf_cheeto"), 1);
	auto svcheats = Cvar->FindVar(XorStr("inf_cheeto"));
	svcheats->SetValue(1);
}


void cPlayer::ChatSpamRegular()
{
	std::vector<std::string> chatspams =
	{ "I Z U M I . P R O > ALL", "Izumi.pro | Premium 1 tap machine!", "Izumi.pro | Best way to baim!" };

	if (Options.PlayerMods.ChatSpamRegular.Enabled)
	{
		static DWORD lastspammed = 0;
		if (GetTickCount() - lastspammed > 500)
		{
			lastspammed = GetTickCount();
			auto say = "say ";
			std::string p = say + chatspams[rand() % chatspams.size()];
			Engine->ExecuteClientCmd(p.c_str());
		}
	}
}

// Name Changer
void cPlayer::NameChanger()
{
	ConVar* nameConvar = Cvar->FindVar(XorStr("name"));
	*reinterpret_cast<int*>(DWORD(&nameConvar->fnChangeCallback) + 0xC) = NULL;
	switch (Options.PlayerMods.Names.Enabled)
	{
	case 1:
		nameConvar->SetValue(XorStr("\n"));
		break;
	case 2:
		nameConvar->SetValue(XorStr("-I Z U M I-"));
		break;
	case 3:
		nameConvar->SetValue(XorStr("I Z U M I . P R O"));
		break;
	case 4:
		nameConvar->SetValue(XorStr(" \x01\x0B\x01\x0E-I Z U M I-"));
		break;
	case 5:
		nameConvar->SetValue(XorStr("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n----------------------------IZUMI----------------------------"));
		break;
	}
}

//NoName
void cPlayer::NoName()
{
	ConVar* nameConvar = Cvar->FindVar(XorStr("name"));
	*reinterpret_cast<int*>(DWORD(&nameConvar->fnChangeCallback) + 0xC) = NULL;
	nameConvar->SetValue(XorStr("\n"));
}

void SetClanTag(const char* tag, const char* name)
{
	typedef void(__fastcall* pSetClanTagFn)(const char*, const char*);
	static auto pSetClanTag = reinterpret_cast<pSetClanTagFn>(Memory::FindPatternAlt(XorStr("engine.dll"),
		reinterpret_cast<PBYTE>(XorStr("\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x30")), XorStr("xxxxxxxxx????xxxxxx"), 0, 0));

	pSetClanTag(tag, name);
}

//ClanTag
void cPlayer::ClanTagChanger()
{
	switch (Options.PlayerMods.ClanTags.Enabled)
	{
	case 1:
		IzumiSlide();
		break;
	case 2:
		IzumiBuild();
		break;
	case 3:
		erectTag();
		break;
	case 4:
		SetClanTag(XorStr("Izumi.pro"), XorStr("izumi.pro"));
		break;
	case 5:
		SetClanTag(XorStr("\n"), XorStr("izumi.pro"));
		break;
	}
}

//ClanTag
void cPlayer::IzumiSlide()
{
	static int counter = 0;
	static int motion = 0;
	int ServerTime = (float)Globals->interval_per_tick * Global::LocalPlayer->GetTickBase() * 3;

	if (counter % 48 == 0)
		motion++;
	int value = ServerTime % 17;
	switch (value) {
	case 0:SetClanTag("         ", "izumi.pro"); break;
	case 1:SetClanTag("        I", "izumi.pro"); break;
	case 2:SetClanTag("       Iz", "izumi.pro"); break;
	case 3:SetClanTag("      Izu", "izumi.pro"); break;
	case 4:SetClanTag("     Izum", "izumi.pro"); break;
	case 5:SetClanTag("    Izumi", "izumi.pro"); break;
	case 6:SetClanTag("   Izumi.", "izumi.pro"); break;
	case 7:SetClanTag("  Izumi.p", "izumi.pro"); break;
	case 8:SetClanTag(" Izumi.pr", "izumi.pro"); break;
	case 9:SetClanTag("Izumi.pro ", "izumi.pro"); break;
	case 10:SetClanTag("zumi.pro  ", "izumi.pro"); break;
	case 11:SetClanTag("umi.pro   ", "izumi.pro"); break;
	case 12:SetClanTag("mi.pro    ", "izumi.pro"); break;
	case 13:SetClanTag("i.pro     ", "izumi.pro"); break;
	case 14:SetClanTag(".pro      ", "izumi.pro"); break;
	case 15:SetClanTag("ro       ", "izumi.pro"); break;
	case 16:SetClanTag("o        ", "izumi.pro"); break;
	case 17:SetClanTag("         ", "izumi.pro"); break;
	}
	counter++;
}


void cPlayer::erectTag()
{
	static int counter = 0;
	static int motion = 0;
	int ServerTime = (float)Globals->interval_per_tick * Global::LocalPlayer->GetTickBase() * 3;

	if (counter % 48 == 0)
		motion++;
	int value = ServerTime % 13;
	switch (value) {
	case 0:SetClanTag("<=3       ", "izumi.pro"); break;
	case 1:SetClanTag("<==3      ", "izumi.pro"); break;
	case 2:SetClanTag("<===3     ", "izumi.pro"); break;
	case 3:SetClanTag("<====3    ", "izumi.pro"); break;
	case 4:SetClanTag("<=====3   ", "izumi.pro"); break;
	case 5:SetClanTag("<======3  ", "izumi.pro"); break;
	case 6:SetClanTag("<=======3 ", "izumi.pro"); break;
	case 7:SetClanTag("<========3", "izumi.pro"); break;
	case 8:SetClanTag("Im gettin big", "izumi.pro"); break;
	case 9:SetClanTag("Im gettin big", "izumi.pro"); break;
	case 10:SetClanTag("<=======3", "izumi.pro"); break;
	case 11:SetClanTag("<=====3 ", "izumi.pro"); break;
	case 12:SetClanTag("<====3  ", "izumi.pro"); break;
	case 13:SetClanTag("<===3    ", "izumi.pro"); break;
	case 14:SetClanTag("<==3      ", "izumi.pro"); break;
	case 15:SetClanTag("<=3       ", "izumi.pro"); break;
	}
	counter++;
}

void cPlayer::IzumiBuild()
{
	static int counter = 0;
	static int motion = 0;
	int ServerTime = (float)Globals->interval_per_tick * Global::LocalPlayer->GetTickBase() * 3;

	if (counter % 48 == 0)
		motion++;
	int value = ServerTime % 42;
	switch (value) {
	case 0:SetClanTag("\ ", "izumi.pro"); break;
	case 1:SetClanTag("| ", "izumi.pro"); break;
	case 2:SetClanTag("/ ", "izumi.pro"); break;
	case 3:SetClanTag("I\ ", "izumi.pro"); break;
	case 4:SetClanTag("I| ", "izumi.pro"); break;
	case 5:SetClanTag("I/ ", "izumi.pro"); break;
	case 6:SetClanTag("Iz ", "izumi.pro"); break;
	case 7:SetClanTag("Iz\ ", "izumi.pro"); break;
	case 8:SetClanTag("Iz| ", "izumi.pro"); break;
	case 9:SetClanTag("Iz/ ", "izumi.pro"); break;
	case 10:SetClanTag("Izu ", "izumi.pro"); break;
	case 11:SetClanTag("Izu\ ", "izumi.pro"); break;
	case 12:SetClanTag("Izu| ", "izumi.pro"); break;
	case 13:SetClanTag("Izu/ ", "izumi.pro"); break;
	case 14:SetClanTag("Izum ", "izumi.pro"); break;
	case 15:SetClanTag("Izum\ ", "izumi.pro"); break;
	case 16:SetClanTag("Izum| ", "izumi.pro"); break;
	case 17:SetClanTag("Izum/ ", "izumi.pro"); break;
	case 18:SetClanTag("Izumi ", "izumi.pro"); break;
	case 19:SetClanTag("Izumi\ ", "izumi.pro"); break;
	case 20:SetClanTag("Izumi| ", "izumi.pro"); break;
	case 21:SetClanTag("Izumi/ ", "izumi.pro"); break;
	case 22:SetClanTag("Izumi. ", "izumi.pro"); break;
	case 23:SetClanTag("Izumi.\ ", "izumi.pro"); break;
	case 24:SetClanTag("Izumi.| ", "izumi.pro"); break;
	case 25:SetClanTag("Izumi./ ", "izumi.pro"); break;
	case 26:SetClanTag("Izumi.p ", "izumi.pro"); break;
	case 27:SetClanTag("Izumi.p\ ", "izumi.pro"); break;
	case 28:SetClanTag("Izumi.p| ", "izumi.pro"); break;
	case 29:SetClanTag("Izumi.p/ ", "izumi.pro"); break;
	case 30:SetClanTag("Izumi.pr ", "izumi.pro"); break;
	case 31:SetClanTag("Izumi.pr\ ", "izumi.pro"); break;
	case 32:SetClanTag("Izumi.pr| ", "izumi.pro"); break;
	case 33:SetClanTag("Izumi.pr/ ", "izumi.pro"); break;
	case 34:SetClanTag("Izumi.pro ", "izumi.pro"); break;
	case 35:SetClanTag("Izumi.pr\  ", "izumi.pro"); break;
	case 36:SetClanTag("Izumi.p\    ", "izumi.pro"); break;
	case 37:SetClanTag("Izumi.\      ", "izumi.pro"); break;
	case 38:SetClanTag("Izumi\        ", "izumi.pro"); break;
	case 39:SetClanTag("Izum\          ", "izumi.pro"); break;
	case 40:SetClanTag("Izu\            ", "izumi.pro"); break;
	case 41:SetClanTag("Iz\              ", "izumi.pro"); break;
	case 42:SetClanTag("I                 ", "izumi.pro"); break;


	}
	counter++;
}


//RageAutoStraft
void cPlayer::RageAutoStrafe(CUserCmd* pCmd)
{
	if (GetAsyncKeyState(0x57)//W 
		|| GetAsyncKeyState(0x41)//A
		|| GetAsyncKeyState(0x53)//S
		|| GetAsyncKeyState(0x44))//D
		return;

	if (GetAsyncKeyState(VK_SPACE) && (Global::LocalPlayer->GetFlags() & FL_ONGROUND)
		&& Global::LocalPlayer->GetVelocity().Length() <= 60.f)
	{
		pCmd->forwardmove = 450.f;
	}
	auto Delta = 0.f;
	if (Global::LocalPlayer->GetVelocity().Length() > 60.f)
	{
		auto x = 30.f;
		auto z = x / Global::LocalPlayer->GetVelocity().Length();
		z = fabsf(z);
		auto a = x * z;
		Delta = a;
	}
	if (GetAsyncKeyState(VK_SPACE) && !(Global::LocalPlayer->GetFlags() & FL_ONGROUND))
	{
		if (pCmd->mousedx < 0)
			pCmd->sidemove = -450;
		if (pCmd->mousedx > 0)
			pCmd->sidemove = 450;
		if (pCmd->tick_count % 2)
		{
			pCmd->sidemove = -450;
		}
		else
		{
			pCmd->viewangles.y += Delta;
			pCmd->sidemove = 450;
		}
	}
}