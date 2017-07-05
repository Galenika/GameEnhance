#include "MiscHacks.h"
#include "Interfaces.h"
#include "RenderManager.h"

#include <time.h>

Vector AutoStrafeView;

void CMiscHacks::Init()
{
	// Any init
}

void CMiscHacks::Draw()
{
	// Any drawing	
}


void Invisible()
{
	char* NewName = "\n\xAD\xAD\xAD";
	static bool ay = false;
	static int tick = 0;
	ConVar* Name = Interfaces::CVar->FindVar("name");
	if (Name)
	{
		if (tick == 14)
		{
			if (ay)
			{
				NewName = "\n\xAD\xAD\xAD";
				ay = false;
			}
			else
			{
				NewName = "\n\xAD\xAD\xAD";
				ay = true;
			}

			int lastcallback = *(int*)((DWORD)&Name->fnChangeCallback + 0xC);
			*(int*)((DWORD)&Name->fnChangeCallback + 0xC) = 0;
			Name->SetValue(NewName);
			//*(int*)((DWORD)&Name->fnChangeCallback + 0xC) = lastcallback;
		}

		tick++;
		if (tick == 20)
		{
			tick = 0;
		}
	}
}
void CMiscHacks::Move(CUserCmd *pCmd, bool& bSendPacket)
{
	static ConVar* nameConvar = Interfaces::CVar->FindVar("name");
	static int Times = 250;

	if (Menu::Window.MiscTab.OtherAimware.GetIndex() == 1) {
		NameChange(pCmd);
	}
	if (Menu::Window.MiscTab.OtherAimware.GetIndex() == 2) {
		/*std::string text = "";
		static bool useSpace = false;
		text = useSpace ? "™AIMWARE.net " : "™AIMWARE.net";
		NET_SetConVar noob("name", text.c_str());
		Interfaces::Engine->GetNetChannelInfo()->SendNetMsg(noob);
		useSpace = !useSpace;*/
	}
	if (Menu::Window.MiscTab.OtherAimware.GetIndex() == 3) {
		/*std::string text = "";
		static bool useSpace = false;
		text = useSpace ? "-INTERWEBZ" : "INTERWEBZ-";
		NET_SetConVar noob("name", text.c_str());
		Interfaces::Engine->GetNetChannelInfo()->SendNetMsg(noob);
		useSpace = !useSpace;*/
	}
	if (Menu::Window.MiscTab.OtherAimware.GetIndex() == 2) {
		NameSteal();
	}
	if (Menu::Window.MiscTab.OtherClowns.GetState()) 
	{
		Invisible();

	}
	// Any Move Stuff
	if (Menu::Window.MiscTab.OtherBypass.GetState())
		CheatsBypass(pCmd);

	// Bhop
	if (Menu::Window.MiscTab.OtherBunnyhop.GetState())
		AutoJump(pCmd);
	if (Menu::Window.VisualsTab.ViewChanger.GetState())
	{
		fovchanger();
	}
	//if (Menu::Window.MiscTab.OtherTeleportHack.GetState())
	//	TeleportHack(pCmd);

	Interfaces::Engine->GetViewAngles(AutoStrafeView);

	NoFlash();

//	NoFlash(pCmd);



	if (Menu::Window.MiscTab.OtherCircleStrafe.GetState() && Menu::Window.MiscTab.OtherCircleStrafeKey.GetKey() > 0)
	{
		int CircleKey = Menu::Window.MiscTab.OtherCircleStrafeKey.GetKey();
		if (GUI.GetKeyState(CircleKey))
		{
			doCircularStrafe(pCmd, AutoStrafeView);
		}
	}

	// Strafe

	if (Menu::Window.MiscTab.OtherAutoStrafe.GetIndex() == 1)
		AutoStrafe(pCmd);

	if (Menu::Window.MiscTab.OtherAutoStrafe.GetIndex() == 2)
		RageStrafe(pCmd);


	// Spams
	switch (Menu::Window.MiscTab.OtherChatSpam.GetIndex())
	{
	case 0:
		// No Chat Spam
		break;
	case 1:
		// Regular
		ChatSpam2();
		break;
	case 2:
		// Report Spam
		ChatSpamRegular();
		break;
	case 3:
		// Report Spam
		ChatSpam1();
		break;
	case 4:
		// Report Spam
		ChatSpam2();
		break;
	case 5:
		// Report Spam
		ChatSpam3();
		break;
	}
	//std::string CustomclanTag = Menu::Window.MiscTab.ClanTagCustom.getText();
	//char Me[512];
	//sprintf_s(Me, "[%s]", CustomclanTag);

	//int HP = hackManager.pLocal()->GetHealth();
	//char Me2[512];
	//sprintf_s(Me2, "[%d HP]", HP);

}

void CMiscHacks::NoFlash()
{
	IClientEntity* localplayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

			if (Menu::Window.VisualsTab.OtherNoFlash.GetState() && Menu::Window.VisualsTab.OtherNoFlashValue.GetValue() > 0)
			{
			
				*localplayer->GetFlashMaxAlpha() = 255.0f - Menu::Window.VisualsTab.OtherNoFlashValue.GetValue();
		    }
			else
			{
				*localplayer->GetFlashMaxAlpha() = 255.0f;
			}
}

void CMiscHacks::NameChange(CUserCmd* pcmd)
{
	for (int changes = 0; changes <= Menu::Window.MiscTab.NameTimes.GetValue(); changes++)
	{
		char* NewName = "°RedHead.Pro\xE2\x84\xA2";
		static bool ay = false;
		static int tick = 0;
		ConVar* Name = Interfaces::CVar->FindVar("name");
		if (Name)
		{
			if (tick == 14)
			{
				if (ay)
				{
					NewName = "°RedHead.Pro\xE2\x84\xA2";
					ay = false;
				}
				else
				{
					NewName = "öRedHead.Pro\xE2\x84\xA2";
					ay = true;
				}

				int lastcallback = *(int*)((DWORD)&Name->fnChangeCallback + 0xC);
				*(int*)((DWORD)&Name->fnChangeCallback + 0xC) = 0;
				Name->SetValue(NewName);
				//*(int*)((DWORD)&Name->fnChangeCallback + 0xC) = lastcallback;
			}

			tick++;
			if (tick == 20)
			{
				tick = 0;
			}
		}
	}
}

void CMiscHacks::CheatsBypass(CUserCmd* pCmd)
{
	ConVar* sv_cheats = Interfaces::CVar->FindVar("sv_cheats");
	SpoofedConvar* sv_cheats_bypass = new SpoofedConvar(sv_cheats);
	sv_cheats_bypass->SetInt(1337);
}
// pModelFov Changer

void CMiscHacks::fovchanger()
{
	int value = Menu::Window.VisualsTab.FovChanger.GetValue();

	ConVar* fov = Interfaces::CVar->FindVar("viewmodel_fov");
	int lastcallback = *(int*)((DWORD)&fov->fnChangeCallback + 0xC);
	*(int*)((DWORD)&fov->fnChangeCallback + 0xC) = 0;
	char valu0[512];
	sprintf_s(valu0, "%d", value);
	if (hackManager.pLocal()->IsScoped())
	{
		fov->SetValue(65);
	}
	else
	{
		fov->SetValue(valu0);
	}
}
void CMiscHacks::NameSteal()
{

	// Loop through all active entitys
	std::vector < std::string > Names;

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *entity = Interfaces::EntList->GetClientEntity(i);

		player_info_t pInfo;
		// If it's a valid entity and isn't the player
		if (entity)
		{
			ClientClass* cClass = (ClientClass*)entity->GetClientClass();

			// If entity is a player
			if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
			{
				if (Interfaces::Engine->GetPlayerInfo(i, &pInfo))
				{
					if (!strstr(pInfo.m_szPlayerName, "GOTV"))
						Names.push_back(pInfo.m_szPlayerName);
				}
			}
		}
	}

	int randomIndex = rand() % Names.size();
	char buffer[128];
	//static unsigned long int meme = 3091961887844204720;
	sprintf_s(buffer, "%s ", Names[randomIndex].c_str());
	NET_SetConVar namewchyh("name", buffer);
	Interfaces::Engine->GetNetChannelInfo()->SendNetMsg(namewchyh);

}
void CMiscHacks::SetClanTag(const char* tag, const char* name)
{

	static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15", "xxxxxxxxx"));
	pSetClanTag(tag, name);
}
//void CMiscHacks::TeleportHack(CUserCmd *pCmd)
//{
//	if (GetAsyncKeyState(VK_F6))   //Klawisz do teleportowania
//	{
//		*(DWORD*)(&pCmd->viewangles.z) = 1e+37;
//	}
//}

void CMiscHacks::AutoJump(CUserCmd *pCmd)
{
	if (pCmd->buttons & IN_JUMP && GUI.GetKeyState(VK_SPACE))
	{
		int iFlags = hackManager.pLocal()->GetFlags();
		if (!(iFlags & FL_ONGROUND))
			pCmd->buttons &= ~IN_JUMP;

		if (hackManager.pLocal()->GetVelocity().Length() <= 45)
		{
			if (pCmd->viewangles.x > 178 || pCmd->viewangles.x < -178)
			{
				//pCmd->forwardmove = -450.f;
			}
			else
			{
				//pCmd->forwardmove = 450.f;
			}
		}
	}
}

void CMiscHacks::AutoStrafe(CUserCmd *pCmd)
{
	int Circle = Menu::Window.MiscTab.OtherCircleStrafeKey.GetKey();
	if (!GUI.GetKeyState(Circle))
	{

		IClientEntity* pLocal = hackManager.pLocal();
		static bool bDirection = true;

		bool bKeysPressed = true;

		if (GUI.GetKeyState(0x41) || GUI.GetKeyState(0x57) || GUI.GetKeyState(0x53) || GUI.GetKeyState(0x44))
			bKeysPressed = false;
		//if (pCmd->buttons & IN_ATTACK)
		//	bKeysPressed = false;

		float flYawBhop = 0.f;
		if (pLocal->GetVelocity().Length() > 45.f)
		{
			float x = 30.f, y = pLocal->GetVelocity().Length(), z = 0.f, a = 0.f;

			z = x / y;
			z = fabsf(z);

			a = x * z;

			flYawBhop = a;
		}

		if ((GetAsyncKeyState(VK_SPACE) && !(pLocal->GetFlags() & FL_ONGROUND)) && bKeysPressed)
		{

			if (bDirection)
			{
				AutoStrafeView -= flYawBhop;
				GameUtils::NormaliseViewAngle(AutoStrafeView);
				pCmd->sidemove = -450;
				bDirection = false;
			}
			else
			{
				AutoStrafeView += flYawBhop;
				GameUtils::NormaliseViewAngle(AutoStrafeView);
				pCmd->sidemove = 430;
				bDirection = true;
			}

			if (pCmd->mousedx < 0)
			{
				if (pCmd->viewangles.x > 178 || pCmd->viewangles.x < -178)
				{
					//pCmd->forwardmove = -22;
				}
				else
				{
					//pCmd->forwardmove = 22;
				}
				pCmd->sidemove = -450;
			}

			if (pCmd->mousedx > 0)
			{
				if (pCmd->viewangles.x > 178 || pCmd->viewangles.x < -178)
				{
					//pCmd->forwardmove = -22;
				}
				else
				{
					//pCmd->forwardmove = +22;
				}
				pCmd->sidemove = 450;
			}
		}
	}
}

Vector GetAutostrafeView()
{
	return AutoStrafeView;
}
void CMiscHacks::ChatSpam3()
{

}

#include <random>
__forceinline int RandomInt(int Min, int Max) {
	std::random_device RandomDevice;
	std::mt19937 MT(RandomDevice());
	std::uniform_int_distribution<int>Compartment(Min, Max);
	return Compartment(MT);
}

std::vector<std::string> chatspamss =
{ "We are unityhacks. We are legion. We do not forgive. We do not forget. Expect us!", " www.unityhacks.com - #1 for a reason", "Get rekt - by unityhacks.com", "www.unityhacks.com - best featuers without lack of security", "www.unityhacks.com","www.unityhacks.com - we provide you with skill", "www.unityhacks.com - your premium hacks provider" };

void CMiscHacks::ChatSpamReports()
{
	static DWORD lastspammed = 0;
	if (GetTickCount() - lastspammed > 800)
	{
		lastspammed = GetTickCount();
		auto say = "say ";
		std::string p = say + chatspamss[rand() % chatspamss.size()];
		Interfaces::Engine->ClientCmd_Unrestricted(p.c_str());
	}
}

std::vector<std::string> chatspam =
{ "AIMWARE.NET | Premium Rage/Legit CSGO Cheat" };



void CMiscHacks::ChatSpamRegular()
{
	static DWORD lastspammed = 0;
	if (GetTickCount() - lastspammed > 800)
	{
		lastspammed = GetTickCount();
		auto say = "say ";
		std::string p = say + chatspam[rand() % chatspam.size()];
		Interfaces::Engine->ClientCmd_Unrestricted(p.c_str());
	}

}

std::vector<std::string> chatspams =
{ "INTERWEBZ.CC OWNS ME AND ALL" };


void CMiscHacks::ChatSpam1()
{
	static DWORD lastspammed = 0;
	if (GetTickCount() - lastspammed > 800)
	{
		lastspammed = GetTickCount();
		auto say = "say ";
		std::string p = say + chatspams[rand() % chatspams.size()];
		Interfaces::Engine->ClientCmd_Unrestricted(p.c_str());
	}
}
std::vector<std::string> chatspamsss =
{ "RedHead.Pro > YOU", "RedHead.Pro > ALL", "RedHead.Pro PREMIUM CHEATS", "Never UNTR4$T3D for RedHead.Pro", "Be good with RedHead.Pro", "RedHeadPro.pe.hu - Sales Soon" };

void CMiscHacks::ChatSpam2()
{
	static DWORD lastspammed = 0;
	if (GetTickCount() - lastspammed > 800)
	{
		lastspammed = GetTickCount();
		auto say = "say ";
		std::string p = say + chatspamsss[rand() % chatspamsss.size()];
		Interfaces::Engine->ClientCmd_Unrestricted(p.c_str());
	}
}
void CMiscHacks::RageStrafe(CUserCmd *pCmd)
{
	int Circle = Menu::Window.MiscTab.OtherCircleStrafeKey.GetKey();
	if (!GUI.GetKeyState(Circle))
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

		/*	IClientEntity* pLocal = hackManager.pLocal();
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
		} */
	}
}


void CMiscHacks::CircleStrafer(CUserCmd* cmd, IClientEntity* localPly) {
	//static bool CircleDone;
	//static bool JustStarted;
	//static float angle = 0.f;
	//static int inv = -1;

	//CircleDone = false;

	//if (JustStarted) {
	//	JustStarted = false;
	//	angle = cmd->viewangles.y;
	//}

	//Ray_t left_trace;
	//Ray_t right_trace;
	//Ray_t normal_trace;
	//CTraceFilter *Filter = Filter(localPly);
	//Vector EyePos = localPly->GetAbsAngles() + localPly->GetAbsOrigin();

	//Vector vleft = Vector(10.f, angle - 50.f, 0.f), vright = Vector(10.f, angle + 50.f, 0.f), vnormal = Vector(10.f, angle, 0.f), left, right, normal;
	//g_pTools->AngleVectors(vright, &right, 0, 0);
	//g_pTools->AngleVectors(vleft, &left, 0, 0);
	//g_pTools->AngleVectors(vnormal, &normal, 0, 0);

	//left_trace.Init(EyePos, EyePos + left * 50.f);
	//right_trace.Init(EyePos, EyePos + right * 50.f);
	//normal_trace.Init(EyePos, EyePos + normal * 50.f);

	//trace_t result_left, result_right, result;

	//RayTrace(left_trace, MASK_SOLID, &Filter, &result_left);
	//g_pEngineTrace->TraceRay(right_trace, MASK_SOLID, &Filter, &result_right);
	//g_pEngineTrace->TraceRay(normal_trace, MASK_SOLID, &Filter, &result);
	//static int amount;
	//amount = (2 * ClientVars::m_iMisc_CircleStrafeRetrackSpeed) - g_pGlobalVarsBase->interval_per_tick;
	//if ((result.fraction != 1.f && !result.m_pEnt) || (result_right.fraction != 1.f && !result_right.m_pEnt)) {
	//	inv = -1;
	//	angle -= 30.f;
	//	cmd->sidemove = -250.f;
	//}
	//else if (result_left.fraction != 1.f && !result_left.m_pEnt) {
	//	inv = 1;
	//	angle += 30.f;
	//	cmd->sidemove = 250.f;
	//}
	//else {
	//	float flAdd = (200 / localPly->GetVelocity().Length2D()) * amount;
	//	if (flAdd > 5.f) flAdd = 5.f;
	//	angle += flAdd * (float)inv;
	//	cmd->sidemove = 250.f * (float)inv;
	//}

	//NormalizeAngle(Global::strafeang.y);

	//Vector noob(0.f, angle - cmd->viewangles.y, 0.f);
	//g_pTools->NormalizeVector(noob);

	//if (noob.y > 170.f && noob.y < 180.f)
	//{
	//	CircleDone = true;
	//}
	//Global::strafeang.y = angle; //set your movement fix to this angle
}

int CircleFactor = 0;
void CMiscHacks::RotateMovement(CUserCmd* pCmd, float rotation)
{
	rotation = DEG2RAD(rotation);

	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);

	float new_forwardmove = (cos_rot * pCmd->forwardmove) - (sin_rot * pCmd->sidemove);
	float new_sidemove = (sin_rot * pCmd->forwardmove) + (cos_rot * pCmd->sidemove);

	pCmd->forwardmove = new_forwardmove;
	pCmd->sidemove = -new_sidemove;

}


bool CMiscHacks::doCircularStrafe(CUserCmd* pCmd, Vector& OriginalView) {




	IClientEntity *pLocalEntity = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!pLocalEntity)
		return false;

	if (!pLocalEntity->IsAlive())
		return false;

	CircleFactor++;
	if (CircleFactor > 361)
		CircleFactor = 0;

	int GetItDoubled = 3.0 * CircleFactor - Interfaces::Globals->interval_per_tick;

	Vector StoredViewAngles = pCmd->viewangles;

	if ((pCmd->buttons & IN_JUMP) || !(pLocalEntity->GetFlags() & FL_ONGROUND))
	{
		pCmd->viewangles = OriginalView;
		pCmd->forwardmove = 450.f;
		RotateMovement(pCmd, GetItDoubled);
	}

	return true;
}




void CMiscHacks::RageAutoStrafe(CUserCmd* pCmd)
{
	if (GetAsyncKeyState(0x57)//W 
		|| GetAsyncKeyState(0x41)//A
		|| GetAsyncKeyState(0x53)//S
		|| GetAsyncKeyState(0x44))//D
		return;

	if (GetAsyncKeyState(VK_SPACE) && (hackManager.pLocal()->GetFlags() & FL_ONGROUND)
		&& hackManager.pLocal()->GetVelocity().Length() <= 60.f)
	{
		pCmd->forwardmove = 450.f;
	}
	auto Delta = 0.f;
	if (hackManager.pLocal()->GetVelocity().Length() > 60.f)
	{
		auto x = 30.f;
		auto z = x / hackManager.pLocal()->GetVelocity().Length();
		z = fabsf(z);
		auto a = x * z;
		Delta = a;
	}
	if (GetAsyncKeyState(VK_SPACE) && !(hackManager.pLocal()->GetFlags() & FL_ONGROUND))
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