#include "MiscHacks.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include <time.h>

inline float bitsToFloat(unsigned long i)
{
	return *reinterpret_cast<float*>(&i);
}

inline float FloatNegate(float f)
{
	return bitsToFloat(FloatBits(f) ^ 0x80000000);
}

Vector AutoStrafeView;

void CMiscHacks::Init()
{
}

void CMiscHacks::SpoofPostProcessing()
{
	auto PostProcessing = Interfaces::CVar->FindVar("mat_postprocess_enable");
	auto PostProcessingForce = new SpoofedConvar(PostProcessing);
	PostProcessingForce->SetInt(0);
}

void CMiscHacks::AmbientLighting()
{
	float AmbientRedAmount = Menu::Window.VisualsTab.AmbientRed.GetValue();
	float AmbientGreenAmount = Menu::Window.VisualsTab.AmbientGreen.GetValue();
	float AmbientBlueAmount = Menu::Window.VisualsTab.AmbientBlue.GetValue();

	ConVar* AmbientRedCvar = Interfaces::CVar->FindVar("mat_ambient_light_r");
	*(float*)((DWORD)&AmbientRedCvar->fnChangeCallback + 0xC) = NULL;
	AmbientRedCvar->SetValue(AmbientRedAmount);

	ConVar* AmbientGreenCvar = Interfaces::CVar->FindVar("mat_ambient_light_g");
	*(float*)((DWORD)&AmbientGreenCvar->fnChangeCallback + 0xC) = NULL;
	AmbientGreenCvar->SetValue(AmbientGreenAmount);

	ConVar* AmbientBlueCvar = Interfaces::CVar->FindVar("mat_ambient_light_b");
	*(float*)((DWORD)&AmbientBlueCvar->fnChangeCallback + 0xC) = NULL;
	AmbientBlueCvar->SetValue(AmbientBlueAmount);
}

void CMiscHacks::AmbientExposure()
{
	float AmbientExposureMax = Menu::Window.VisualsTab.AmbientExposure.GetValue();
	float AmbientExposureMin = Menu::Window.VisualsTab.AmbientExposure.GetValue();

	ConVar* AmbientExposure2 = Interfaces::CVar->FindVar("mat_autoexposure_max_multiplier");
	*(float*)((DWORD)&AmbientExposure2->fnChangeCallback + 0xC) = NULL;
	AmbientExposure2->SetValue(AmbientExposureMax);

	ConVar* AmbientExposure3 = Interfaces::CVar->FindVar("mat_autoexposure_min");
	*(float*)((DWORD)&AmbientExposure3->fnChangeCallback + 0xC) = NULL;
	AmbientExposure3->SetValue(AmbientExposureMin);
}


void CMiscHacks::Draw()
{
	switch (Menu::Window.MiscTab.OtherChatSpam.GetIndex()) /*Chat-Spammers*/
	{
	case 0:
		/*Disabled*/
		break;
	case 1:
		ChatSpamName();
		break;
	case 2:
		SpamRegular();
		break;
	case 3:
		ChatSpamHowBad();
		break;
	case 4:
		ChatSpamDisperseName();
		break;
	}
}

void CMiscHacks::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) /*Auto-Disable Post-Processing*/
		SpoofPostProcessing();

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) /*Auto-Ambient-Lighting*/
		AmbientLighting();

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) /*Auto-Ambient-Lighting*/
		AmbientExposure();

	if (Menu::Window.MiscTab.FakeLagEnable.GetState())
		Fakelag(pCmd, bSendPacket);

	Interfaces::Engine->GetViewAngles(AutoStrafeView);

	switch (Menu::Window.MiscTab.OtherAutoJump.GetIndex()) /*AutoHop*/
	{
	case 0:
		/*Disabled*/
		break;
	case 1:
		Bunnyhop(pCmd);
		break;
	}

	switch (Menu::Window.MiscTab.OtherAutoStrafe.GetIndex())
	{
	case 0:
		/*Disabled*/
		break;
	case 1:
		LegitStrafe(pCmd);
		break;
	case 2:
		RageStrafe(pCmd);
		break;
	}

	switch (Menu::Window.VisualsTab.AmbientSkybox.GetIndex())
	{
	case 0:
		/*Disabled*/
		break;
	case 1:
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) {
			ConVar* NoSkybox = Interfaces::CVar->FindVar("sv_skyname"); /*No-Skybox*/
			*(float*)((DWORD)&NoSkybox->fnChangeCallback + 0xC) = NULL;
			NoSkybox->SetValue("sky_l4d_rural02_ldr");
		}
		break;

	case 2:
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) {
			ConVar* NightSkybox1 = Interfaces::CVar->FindVar("sv_skyname"); /*Night-Skybox*/
			*(float*)((DWORD)&NightSkybox1->fnChangeCallback + 0xC) = NULL;
			NightSkybox1->SetValue("sky_csgo_night02b");
		}
		break;

	case 3:
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) {
			ConVar* NightSkybox2 = Interfaces::CVar->FindVar("sv_skyname"); /*Night-Skybox*/
			*(float*)((DWORD)&NightSkybox2->fnChangeCallback + 0xC) = NULL;
			NightSkybox2->SetValue("sky_csgo_night02");
		}
		break;

	case 4:
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) {
			ConVar* SkyBoxJungle = Interfaces::CVar->FindVar("sv_skyname"); /*Night-Skybox*/
			*(float*)((DWORD)&SkyBoxJungle->fnChangeCallback + 0xC) = NULL;
			SkyBoxJungle->SetValue("jungle");
		}
		break;
	}
}

static __declspec(naked) void __cdecl Invoke_NET_SetConVar(void* pfn, const char* cvar, const char* value)
{
	__asm 
	{
		push    ebp
			mov     ebp, esp
			and     esp, 0FFFFFFF8h
			sub     esp, 44h
			push    ebx
			push    esi
			push    edi
			mov     edi, cvar
			mov     esi, value
			jmp     pfn
	}
}

void DECLSPEC_NOINLINE NET_SetConVar(const char* value, const char* cvar)
{
	static DWORD setaddr = Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x8D\x4C\x24\x1C\xE8\x00\x00\x00\x00\x56", "xxxxx????x");
	if (setaddr != 0) 
	{
		void* pvSetConVar = (char*)setaddr;
		Invoke_NET_SetConVar(pvSetConVar, cvar, value);
	}
}

void change_name(const char* name)
{
	if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected())
		NET_SetConVar(name, "name");
}

void CMiscHacks::Bunnyhop(CUserCmd *pCmd)
{
	if (pCmd->buttons & IN_JUMP && GUI.GetKeyState(VK_SPACE))
	{
		int iFlags = hackManager.pLocal()->GetFlags();
		if (!(iFlags & FL_ONGROUND))
			pCmd->buttons &= ~IN_JUMP;

		if (hackManager.pLocal()->GetVelocity().Length() <= 50)
		{
			pCmd->forwardmove = 450.f;
		}
	}
}

void CMiscHacks::LegitStrafe(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();
	if (!(pLocal->GetFlags() & FL_ONGROUND))
	{
		pCmd->forwardmove = 0.0f;

		if (pCmd->mousedx < 0)
		{
			pCmd->sidemove = -450.0f;
		}
		else if (pCmd->mousedx > 0)
		{
			pCmd->sidemove = 450.0f;
		}
	}
}

void CMiscHacks::RageStrafe(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();

	bool bKeysPressed = true;
	if (GUI.GetKeyState(0x41) || GUI.GetKeyState(0x57) || GUI.GetKeyState(0x53) || GUI.GetKeyState(0x44)) bKeysPressed = false;

	if ((GetAsyncKeyState(VK_SPACE) && !(pLocal->GetFlags() & FL_ONGROUND)) && bKeysPressed)
	{
		if (pCmd->mousedx > 1 || pCmd->mousedx < -1) {
			pCmd->sidemove = pCmd->mousedx < 0.f ? -450.f : 450.f;
		}
		else {
			pCmd->forwardmove = (1800.f * 4.f) / pLocal->GetVelocity().Length2D();
			pCmd->sidemove = (pCmd->command_number % 2) == 0 ? -450.f : 450.f;
			if (pCmd->forwardmove > 450.f)
				pCmd->forwardmove = 450.f;
		}
	}
}

Vector GetAutostrafeView()
{
	return AutoStrafeView;
}

void CMiscHacks::ChatSpamHowBad()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	static bool wasSpamming = true;


	if (wasSpamming)
	{
		static bool useSpace = true;
		if (useSpace)
		{
			change_name ("HowBad.RU-");
			useSpace = !useSpace;
		}
		else
		{
			change_name("-HowBad.RU");
			useSpace = !useSpace;
		}
	}
	start_t = clock();
}

void CMiscHacks::ChatSpamDisperseName()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	static bool wasSpamming = true;

	if (wasSpamming)
	{
		change_name("\n…e…e…e\n");
	}

	start_t = clock();
}

void CMiscHacks::ChatSpamName()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	std::vector < std::string > Names;

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *entity = Interfaces::EntList->GetClientEntity(i);

		player_info_t pInfo;
		// If it's a valid entity and isn't the player
		if (entity && hackManager.pLocal()->GetTeamNum() == entity->GetTeamNum() && entity != hackManager.pLocal())
		{
			ClientClass* cClass = (ClientClass*)entity->GetClientClass();

			// If entity is a player
			if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
			{
				if (Interfaces::Engine->GetPlayerInfo(i, &pInfo))
				{
					if (!strstr(pInfo.name, "GOTV"))
						Names.push_back(pInfo.name);
				}
			}
		}
	}

	static bool wasSpamming = true;
	//static std::string nameBackup = "INTERWEBZ.CC";

	int randomIndex = rand() % Names.size();
	char buffer[128];
	sprintf_s(buffer, "%s ", Names[randomIndex].c_str());

	if (wasSpamming)
	{
		change_name(buffer);
	}
	else
	{
		change_name ("p$i 1337");
	}

	start_t = clock();
}

void CMiscHacks::SpamRegular()
{
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < spamtime)
		return;

	static bool holzed = true;

	if (Menu::Window.MiscTab.OtherTeamChat.GetState())
	{
		SayInTeamChat("INTERWEBZ.CC OWNS ME AND ALL");
	}
	else
	{
		SayInChat("INTERWEBZ.CC OWNS ME AND ALL");
	}

	start_t = clock();
}

void CMiscHacks::Fakelag(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocal = hackManager.pLocal();
	if (pCmd->buttons & IN_ATTACK) return;
	if (pLocal->GetVelocity() == Vector(0, 0, 0)) return;

	int iChoke = Menu::Window.MiscTab.FakeLagChoke.GetValue();

	static int iFakeLag = -1;
	iFakeLag++;

	if (iFakeLag <= iChoke && iFakeLag > -1)
	{
		bSendPacket = false;
	}
	else
	{
		bSendPacket = true;
		iFakeLag = -1;
	}
}
