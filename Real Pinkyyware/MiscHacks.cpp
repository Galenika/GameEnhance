/*
Syn's AyyWare Framework 2015
*/

#define _CRT_SECURE_NO_WARNINGS

#include "MiscHacks.h"
#include "Interfaces.h"
#include "RenderManager.h"

#include <time.h>

template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

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
	// Any init
}

void CMiscHacks::Draw()
{
	// Any drawing	
	// Spams
	switch (Menu::Window.MiscTab.OtherChatSpam.GetIndex())
	{
	case 0:
		// No Chat Spam
		break;
	case 1:
		// XenoHook
		SetNameXenoHook();
		break;
	case 2:
		// PinkYware
		SetNamePinkyWare();
		break;
	case 3:
		// PinkYware
		SetNameInterwebz();
		break;
	case 4:
		// PinkYware
		SetNameMemeWare();
		break;
	case 5:
		// PinkYware
		SetNameSkeet();
		break;
	case 6:
		// PinkYware
		SetNameCheatBetter();
		break;
	case 7:
		// Regular
		ChatSpamRapeGang();
		break;
	case 8:
		// EZ
		ChatSpamEZ();
		break;
	case 9:
		// Racism
		ChatSpamRacist();
		break;
	case 10:
		// Ayimware
		ChatSpamAyimware();
		break;
	case 11:
		// Report Spam
		ChatSpamReports();
		break;
	}
}
void CMiscHacks::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	// Any Move Stuff
	
	// Bhop
	switch (Menu::Window.MiscTab.OtherAutoJump.GetIndex())
	{
	case 0:
		break;
	case 1:
		AutoJump(pCmd);
		break;
	}

	// AutoStrafe
	Interfaces::Engine->GetViewAngles(AutoStrafeView);
	switch (Menu::Window.MiscTab.OtherAutoStrafe.GetIndex())
	{
	case 0:
		// Off
		break;
	case 1:
		LegitStrafe(pCmd);
		break;
	case 2:
		RageStrafe(pCmd);
		break;
	}

	//Fake Lag
	if (Menu::Window.MiscTab.FakeLagEnable.GetState())
		Fakelag(pCmd, bSendPacket);
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

void CMiscHacks::AutoJump(CUserCmd *pCmd)
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
	if (!(pLocal->GetFlags() & FL_ONGROUND))
	{
		if (pCmd->mousedx > 1 || pCmd->mousedx < -1) {
			pCmd->sidemove = pCmd->mousedx < 0.f ? -450.f : 450.f;
		}
		else {
			pCmd->forwardmove = (1800.f * 4.f) / pLocal->GetVelocity().Length2D();
			pCmd->sidemove = (pCmd->command_number % 3) == 0 ? -450.f : 450.f;
			if (pCmd->forwardmove > 450.f)
				pCmd->forwardmove = 450.f;
		}
	}

	static bool bDirection = true;


	static float move = 450; //400.f; // move = max(move, (abs(cmd->move.x) + abs(cmd->move.y)) * 0.5f);
	float s_move = move * 0.5065f;


	if ((pCmd->buttons & IN_JUMP) || !(pLocal->GetFlags() & FL_ONGROUND))
	{
		pCmd->forwardmove = move * 0.015f;
		pCmd->sidemove += (float)(((pCmd->tick_count % 2) * 2) - 1) * s_move;

		if (pCmd->mousedx)
			pCmd->sidemove = (float)clamp(pCmd->mousedx, -1, 1) * s_move;

		static float strafe = pCmd->viewangles.y;

		float rt = pCmd->viewangles.y, rotation;
		rotation = strafe - rt;

		if (rotation < FloatNegate(Interfaces::Globals->interval_per_tick))
			pCmd->sidemove = -s_move;

		if (rotation > Interfaces::Globals->interval_per_tick)
			pCmd->sidemove = s_move;

		strafe = rt;
	} 
}

Vector GetAutostrafeView()
{
	return AutoStrafeView;
}
void CMiscHacks::SetNameInterwebz()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	static bool wasSpamming = true;
	static std::string nameBackup = "INTERWEBZ";

	if (wasSpamming)
	{
		static bool useSpace = true;
		if (useSpace)
		{
			change_name ("INTERWEBZ-");
			useSpace = !useSpace;
		}
		else
		{
			change_name("-INTERWEBZ");
			useSpace = !useSpace;
		}
	}

	start_t = clock();
}

void CMiscHacks::SetNameMemeWare()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	static bool wasSpamming = true;
	static std::string nameBackup = "MEMEWARE.NET";

	if (wasSpamming)
	{
		static bool useSpace = true;
		if (useSpace)
		{
			change_name("MEMEWARE.NET-");
			useSpace = !useSpace;
		}
		else
		{
			change_name("-MEMEWARE.NET");
			useSpace = !useSpace;
		}
	}

	start_t = clock();
}

void CMiscHacks::SetNameCheatBetter()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	static bool wasSpamming = true;
	static std::string nameBackup = "PINKYWARE>YOUR CHEAT";

	if (wasSpamming)
	{
		static bool useSpace = true;
		if (useSpace)
		{
			change_name("PINKYWARE>YOUR CHEAT");
			useSpace = !useSpace;
		}
		else
		{
			change_name("PINKYWARE>YOUR CHEAT");
			useSpace = !useSpace;
		}
	}

	start_t = clock();
}

void CMiscHacks::SetNameSkeet()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	static bool wasSpamming = true;
	static std::string nameBackup = "Skeet.cc";

	if (wasSpamming)
	{
		static bool useSpace = true;
		if (useSpace)
		{
			change_name("Skeet.cc-");
			useSpace = !useSpace;
		}
		else
		{
			change_name("-Skeet.cc");
			useSpace = !useSpace;
		}
	}

	start_t = clock();
}


void CMiscHacks::ChatSpamAyimware()
{
	static int countSpamRacist = 1;
	// Don't spam it too fast so you can still do stuff
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	int speed = Menu::Window.MiscTab.ChatSpeed.GetIndex();
	if (speed == 0)
	{
		if (timeSoFar < 1.5)
			return;
	}
	else if (speed == 1)
	{
		if (timeSoFar < 1)
			return;
	}
	else
	{
		if (timeSoFar < .7)
			return;
	}

	SayInChat("Ayimware.xyz.co.uk.gov.net | Public Detected Csgo Cheat");

	start_t = clock();
}

void CMiscHacks::SetNameXenoHook()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	static bool wasSpamming = true;
	static std::string nameBackup = "XenoHook";

	if (wasSpamming)
	{
		static bool useSpace = true;
		if (useSpace)
		{
			change_name("XenoHook™");
			useSpace = !useSpace;
		}
		else
		{
			change_name("™XenoHook");
			useSpace = !useSpace;
		}
	}

	start_t = clock();
}

void CMiscHacks::SetNamePinkyWare()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	static bool wasSpamming = true;
	static std::string nameBackup = "PinkyWare";

	if (wasSpamming)
	{
		static bool useSpace = true;
		if (useSpace)
		{
			change_name("PinkyWare");
			useSpace = !useSpace;
		}
		else
		{
			change_name("PinkyWare");
			useSpace = !useSpace;
		}
	}

	start_t = clock();
}


void CMiscHacks::ChatSpamRegular()
{
	// Don't spam it too fast so you can still do stuff
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

void CMiscHacks::ChatSpamRacist()
{
	static int countSpamRacist = 1;
	// Don't spam it too fast so you can still do stuff
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	int speed = Menu::Window.MiscTab.ChatSpeed.GetIndex();
	if (speed == 0)
	{
		if (timeSoFar < 1.5)
			return;
	}
	else if (speed == 1)
	{
		if (timeSoFar < 1)
			return;
	}
	else
	{
		if (timeSoFar < .7)
			return;
	}

	std::vector< std::string > msgs;

	msgs.push_back("What did God say when he made the first black man ? \"Damn, I burnt one.\"");
	msgs.push_back("What is a Mexican's favorite sport? Cross-country");
	msgs.push_back("How do Chinese people name their babies? They throw the down the stairs and see what sound they make");
	msgs.push_back("How do you blindfold a Chinese person? Put floss over their eyes");
	msgs.push_back("Why are asprins white? Because they work!");
	msgs.push_back("What's the difference between a Jew and a boy scout? A boy scout comes home from camp");
	msgs.push_back("Why do Mexicans eat beans for dinner? So they can take bubble baths");
	msgs.push_back("What is the difference between a Mexican and a book? A book has papers");
	msgs.push_back("Why did President Obama get two terms? Because every black man gets a longer sentence");
	msgs.push_back("Why do black people hate country music? Because when they hear the word \"hoedown\" they think their sister got shot");
	msgs.push_back("78% of black men like sex in the shower. The other 22% haven't been to prison yet");
	msgs.push_back("How many Mexicans does it take to change a lightbulb? Just Juan");
	msgs.push_back("What do black men and sperm have in common? One in two million work");

	int randomIndex = rand() % msgs.size();
	SayInChat(msgs[randomIndex].c_str());
	start_t = clock();
}

void CMiscHacks::ChatSpamEZ()
{
	// EZ
	static int count = 0;
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	int speed = Menu::Window.MiscTab.ChatSpeed.GetIndex();
	if (speed == 0)
	{
		if (timeSoFar < 1.5)
			return;
	}
	else if (speed == 1)
	{
		if (timeSoFar < 1)
			return;
	}
	else
	{
		if (timeSoFar < .7)
			return;
	}

	std::vector<std::string> msgs;

	msgs.push_back("____________________");
	msgs.push_back("EEEEEEEE____ZZZZZZZZ");
	msgs.push_back("EE_______________ZZ_");
	msgs.push_back("EE______________ZZ__");
	msgs.push_back("EEEEEEEE_______ZZ___");
	msgs.push_back("EE____________ZZ____");
	msgs.push_back("EE___________ZZ_____");
	msgs.push_back("EE__________ZZ______");
	msgs.push_back("EEEEEEEE___ZZZZZZZZ_");
	msgs.push_back("____________________");

	SayInChat(msgs[count].c_str());

	if (count == 9)
		count = 0;
	else
		count += 1;
	start_t = clock();
}

void CMiscHacks::ChatSpamRapeGang()
{
	// Don't spam it too fast so you can still do stuff
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	int speed = Menu::Window.MiscTab.ChatSpeed.GetIndex();
	if (speed == 0)
	{
		if (timeSoFar < 1.5)
			return;
	}
	else if (speed == 1)
	{
		if (timeSoFar < 1)
			return;
	}
	else
	{
		if (timeSoFar < .7)
			return;
	}

	std::vector<std::string> msgs;

	msgs.push_back("JuanTap");
	msgs.push_back("Damn fam you really payed for that");
	msgs.push_back("R E F U N D");
	msgs.push_back("UFF YA");
	msgs.push_back("cya on the forums skid");

	int randomIndex = rand() % msgs.size();

	SayInChat(msgs[randomIndex].c_str());

	start_t = clock();
}

void CMiscHacks::ChatSpamReports()
{
	// Don't spam it too fast so you can still do stuff
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	int speed = Menu::Window.MiscTab.ChatSpeed.GetIndex();
	if (speed == 0)
	{
		if (timeSoFar < 1.5)
			return;
	}
	else if (speed == 1)
	{
		if (timeSoFar < 1)
			return;
	}
	else
	{
		if (timeSoFar < .7)
			return;
	}

	// Loop through all active entitys
	std::vector < std::string > Names;

	for (int i = 0; i < I::EntList->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *entity = I::EntList->GetClientEntity(i);

		player_info_t pInfo;
		// If it's a valid entity and isn't the player
		int index = Menu::Window.MiscTab.PlayerSelection.GetIndex();
		if (index == 0)
		{
			if (entity && hackManager.pLocal()->GetTeamNum() != entity->GetTeamNum())
			{
				ClientClass* cClass = (ClientClass*)entity->GetClientClass();

				// If entity is a player
				if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
				{
					if (I::Engine->GetPlayerInfo(i, &pInfo))
					{
						if (!strstr(pInfo.name, "GOTV"))
							Names.push_back(pInfo.name);
					}
				}
			}
		}
		else if (index == 1)
		{
			if (entity && hackManager.pLocal()->GetTeamNum() == entity->GetTeamNum())
			{
				ClientClass* cClass = (ClientClass*)entity->GetClientClass();

				// If entity is a player
				if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
				{
					if (I::Engine->GetPlayerInfo(i, &pInfo))
					{
						if (!strstr(pInfo.name, "GOTV"))
							Names.push_back(pInfo.name);
					}
				}
			}
		}
		else
		{
			if (entity && hackManager.pLocal()->GetTeamNum() != entity->GetTeamNum())
			{
				ClientClass* cClass = (ClientClass*)entity->GetClientClass();

				// If entity is a player
				if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
				{
					if (I::Engine->GetPlayerInfo(i, &pInfo))
					{
						if (!strstr(pInfo.name, "GOTV"))
							Names.push_back(pInfo.name);
					}
				}
			}
			if (entity && hackManager.pLocal()->GetTeamNum() == entity->GetTeamNum())
			{
				ClientClass* cClass = (ClientClass*)entity->GetClientClass();

				// If entity is a player
				if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
				{
					if (I::Engine->GetPlayerInfo(i, &pInfo))
					{
						if (!strstr(pInfo.name, "GOTV"))
							Names.push_back(pInfo.name);
					}
				}
			}
		}
	}

	int randomIndex = rand() % Names.size();
	char buffer[128];
	static unsigned long int meme = 3091961887820;
	sprintf_s(buffer, "Report for %s submitted, report id %lu.", Names[randomIndex].c_str(), meme);
	meme += 1;
	SayInChat(buffer);
	start_t = clock();
}

void CMiscHacks::Fakelag(CUserCmd *pCmd, bool &bSendPacket)
{
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
