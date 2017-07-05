
#include "MiscHacks.h"
#include "RenderManager.h"

Vector AutoStrafeView;

void CMiscHacks::Init()
{
	
	// Any init
}

void CMiscHacks::Draw()
{
	// Any drawing	
}
void CMiscHacks::OpenConsole(CUserCmd *pCmd)
{
	//static bool rekt;
	//if (rekt)
	//{
	//	Interfaces::Engine->ClientCmd_Unrestricted("openconsole");
	//	Interfaces::Engine->ClientCmd_Unrestricted("echo BITWare Loaded");
	//	Interfaces::Engine->ClientCmd_Unrestricted("echo Build Compiled At: " __DATE__);
	//	rekt = !rekt;
	//}
}



void CMiscHacks::Move(CUserCmd *pCmd, bool& bSendPacket)
{
	// Any Move Stuff
	OpenConsole(pCmd);
	// Bhop
	if (Menu::Window.MiscTab.OtherAutoJump.GetState())
		AutoJump(pCmd);

	// Strafe
	Interfaces::Engine->GetViewAngles(AutoStrafeView);
	if (Menu::Window.MiscTab.OtherAutoStrafe.GetIndex() > 0 && !CircleStrafe::IsActive)
		AutoStrafe(pCmd);

}

void CMiscHacks::AutoJump(CUserCmd *pCmd)
{
	if (pCmd->buttons & IN_JUMP && GUI.GetKeyState(VK_SPACE))
	{
		int iFlags = hackManager.pLocal()->GetFlags();
		if (!(iFlags & FL_ONGROUND))
			pCmd->buttons &= ~IN_JUMP;

		if (hackManager.pLocal()->GetVelocity().Length() <= 150)
		{


				//pCmd->forwardmove = 450.f;

			}

		}
	}


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

void CMiscHacks::AutoStrafe(CUserCmd *pCmd)
{
	IClientEntity* pTarget = nullptr;
	IClientEntity* pLocal = hackManager.pLocal();

	if (pLocal->IsAlive() == false || hackManager.pLocal()->GetMoveType() == MOVETYPE_LADDER || hackManager.pLocal()->GetMoveType() == MOVETYPE_OBSERVER || hackManager.pLocal()->GetMoveType() == MOVETYPE_NOCLIP) {
		return;
	}
	else {
		IClientEntity* pLocal = hackManager.pLocal();
		static bool bDirection = true;


		static float move = 450; //400.f; // move = max(move, (abs(cmd->move.x) + abs(cmd->move.y)) * 0.5f);
		float s_move = move * 0.5065f;


		if ((pCmd->buttons & IN_JUMP) || !(pLocal->GetFlags() & FL_ONGROUND))
		{
			if (Menu::Window.MiscTab.OtherAutoStrafe.GetIndex() == 0) {
				return;
			}

			//LEGIT AUTOSTRAFE
			if (Menu::Window.MiscTab.OtherAutoStrafe.GetIndex() == 1) {
				static float OldY;

				float Difference = pCmd->viewangles.y - OldY; //AngleNormalize(pCmd->viewangles.y - OldY);

				if (Difference > 0.0f)
				{
					pCmd->sidemove = -400;
				}
				if (Difference < 0.0f)
				{
					pCmd->sidemove = 400;
				}

				OldY = pCmd->viewangles.y;
			}
			//FULL/RAGE AUTOSTRAFE
			if (Menu::Window.MiscTab.OtherAutoStrafe.GetIndex() == 2) {

				int iFlags = pLocal->GetFlags();
				if (!(pLocal->GetFlags() & (int)iFlags & FL_ONGROUND))
				{
					if (pCmd->mousedx > 1 || pCmd->mousedx < -1) {
						pCmd->sidemove = pCmd->mousedx < 0.0f ? -pLocal->GetMaxSpeed() : pLocal->GetMaxSpeed();
					}
					else {
						pCmd->forwardmove = 4200.0f / pLocal->GetVelocity().Length();
						pCmd->sidemove = (pCmd->command_number % 2) == 0 ? -pLocal->GetMaxSpeed() : pLocal->GetMaxSpeed();
						if (pCmd->forwardmove > pLocal->GetMaxSpeed())
							pCmd->forwardmove = pLocal->GetMaxSpeed();
					}
				}
				else if (pCmd->buttons & IN_JUMP) {
					pCmd->forwardmove = pLocal->GetMaxSpeed();
				}
			}
		}
	}
}


Vector GetAutostrafeView()
{
	return AutoStrafeView;
}

void CMiscHacks::ChatSpamReports()
{
	// Don't spam it too fast so you can still do stuff
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.5)
		return;

	// Loop through all active entitys
	std::vector < std::string > Names;

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *entity = Interfaces::EntList->GetClientEntity(i);

		player_info_t pInfo;
		// If it's a valid entity and isn't the player
		if (entity && hackManager.pLocal()->GetTeamNum() != entity->GetTeamNum())
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

	int randomIndex = rand() % Names.size();
	char buffer[128];
	static unsigned long int meme = 3091961887844204720;
	sprintf_s(buffer, "%s ", Names[randomIndex].c_str(), meme);
	meme += 1;
	SayInChat(buffer);
	start_t = clock();
}

void CMiscHacks::ChatSpamRegular()
{
	// Don't spam it too fast so you can still do stuff
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 1.5)
		return;

	SayInChat("BITWare Premium Private Build " __DATE__);

	start_t = clock();
}
