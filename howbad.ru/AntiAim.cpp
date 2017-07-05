#include "RageBot.h"
#include "RenderManager.h"
#include "Resolver.h"
#include "Autowall.h"
#include <iostream>
#include "UTIL Functions.h"


/* AntiAim-List

YAW-Backward
YAW-Static
YAW-Jitter
YAW-Spinbot
YAW-FakeJitter
YAW-FakeSpin
YAW-FakeBackward
YAW-FakeForward
YAW-FakeStatic
*/


namespace AAYaw /*AA-Yaw*/
{

	void Backward(CUserCmd* pCmd) {
		pCmd->viewangles.y -= 180.00f;
	}

	void Static(CUserCmd* pCmd) {

		static bool flip;
		static bool flip2;
		flip = !flip;
		flip2 = !flip2;

		if (flip)
		{
			if (flip2)
				pCmd->viewangles.y += 90.0f;
			else
				pCmd->viewangles.y -= 90.0f;
		}
		else
		{
			pCmd->viewangles.y += 180.0f;
		}
	}

	void Jitter(CUserCmd* pCmd) {

		if (pCmd->command_number % 3)
			pCmd->viewangles.y += 200.0f;
	}

	void Spinbot(CUserCmd* pCmd) {

		int random = rand() % 100;
		int random2 = rand() % 1000;

		static bool dir;
		static float current_y = pCmd->viewangles.y;

		if (random == 1) dir = !dir;

		if (dir)
			current_y += 100.9;
		else
			current_y -= 100.9;

		pCmd->viewangles.y = current_y;

		if (random == random2)
			pCmd->viewangles.y += random;
	}

	void FJitter(CUserCmd* pCmd, bool& bSendPacket) {

		static int jitterangle = 0;

		if (jitterangle <= 1)
		{
			pCmd->viewangles.y += 135;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			pCmd->viewangles.y += 225;
		}

		static int iChoked = -1;
		iChoked++;
		if (iChoked < 1)
		{
			bSendPacket = false;
			if (jitterangle <= 1)
			{
				pCmd->viewangles.y += 45;
				jitterangle += 1;
			}
			else if (jitterangle > 1 && jitterangle <= 3)
			{
				pCmd->viewangles.y -= 45;
				jitterangle += 1;
			}
			else
			{
				jitterangle = 0;
			}
		}
		else
		{
			bSendPacket = true;
			iChoked = -1;
		}
	}

	void FakeSpin(CUserCmd* pCmd, bool& bSendPacket) {

		int random = rand() % 100;
		int random2 = rand() % 1000;
		static bool dir;
		static float current_y = pCmd->viewangles.y;
		IClientEntity* pLocal = hackManager.pLocal();
		float server_time = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;

		if (bSendPacket) {
			pCmd->viewangles.y = (float)(fmod(server_time / 0.39f * 360.0f, 360.0f));
		}
		else
		{
			if (random == 1) dir = !dir;
			if (dir)
				current_y -= 100.9;
			else
				current_y += 100.9;
			pCmd->viewangles.y = current_y;
			if (random == random2)
				pCmd->viewangles.y -= random;
		}
	}

	void FakeBackward(CUserCmd* pCmd, bool& bSendPacket) {

		if (pCmd->command_number % 2)
		{
			bSendPacket = false;
			pCmd->viewangles.y -= 58.f;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y += 180.f;
		}
	}

	void FakeForward(CUserCmd* pCmd, bool& bSendPacket) {

		if (pCmd->command_number % 2)
		{
			bSendPacket = false;
			pCmd->viewangles.y += 125.f;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y -= 0.f;
		}
	}

	void FakeStatic(CUserCmd* pCmd, bool& bSendPacket) {

		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			static int y2 = -179;
			int spinBotSpeedFast = 360.0f / 1.618033988749895f;;

			y2 += spinBotSpeedFast;

			if (y2 >= 179)
				y2 = -179;

			pCmd->viewangles.y = y2;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y -= 180;
			ChokedPackets = -1;
		}
	}
}

bool CanFire()
{
	IClientEntity* pLocalEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalEntity)
		return false;

	CBaseCombatWeapon* entwep = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocalEntity->GetActiveWeaponHandle());

	float flServerTime = (float)pLocalEntity->GetTickBase() * Interfaces::Globals->interval_per_tick;
	float flNextPrimaryAttack = entwep->GetNextPrimaryAttack();

	std::cout << flServerTime << " " << flNextPrimaryAttack << std::endl;

	return !(flNextPrimaryAttack > flServerTime);
}

void CRageBot::aimAtPlayer(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (!pLocal || !pWeapon)
		return;

	Vector eye_position = pLocal->GetEyePosition();

	float best_dist = pWeapon->GetCSWpnData()->m_flRange;

	IClientEntity* target = nullptr;

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			if (Globals::TargetID != -1)
				target = Interfaces::EntList->GetClientEntity(Globals::TargetID);
			else
				target = pEntity;

			Vector target_position = target->GetEyePosition();

			float temp_dist = eye_position.DistTo(target_position);

			if (best_dist > temp_dist)
			{
				best_dist = temp_dist;
				CalcAngle(eye_position, target_position, pCmd->viewangles);
			}
		}
	}
}

void CRageBot::DoAntiAim(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocal = hackManager.pLocal();

	if ((pCmd->buttons & IN_USE) || pLocal->GetMoveType() == MOVETYPE_LADDER)
		return;

	if ((IsAimStepping || pCmd->buttons & IN_ATTACK) && !Menu::Window.RageBotTab.AimbotSilentAim.GetIndex())
		return;

	if (Menu::Window.RageBotTab.AntiAimAtTargets.GetState())
		aimAtPlayer(pCmd);

	CBaseCombatWeapon* CSWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (CSWeapon)
	{
		CSWeaponInfo* pWeaponInfo = CSWeapon->GetCSWpnData();
		if (!GameUtils::IsBallisticWeapon(CSWeapon))
		{
			if (Menu::Window.RageBotTab.AntiAimKnife.GetState())
			{
				if (!CanFire() || pCmd->buttons & IN_ATTACK2)
					return;
			}
			else
			{
				return;
			}
		}
	}

	switch (Menu::Window.RageBotTab.AntiAimPitch.GetIndex()) /*AA-Pitch*/
	{
	case 0:
		/*Disabled*/
		break;
	case 1:
		pCmd->viewangles.x = 89.0000;
		break;

	case 2:
		pCmd->viewangles.x = -991; /*Fake-Down*/
		break;

	case 3:
		pCmd->viewangles.x = 991; /*Fake-UP*/
		break;

	case 4:
		pCmd->viewangles.x = 1080; /*Zero-Down*/
		break;

	case 5:
		pCmd->viewangles.x = -1080; /*Zero-UP*/
		break;
	}

	switch (Menu::Window.RageBotTab.AntiAimYaw.GetIndex()) /*AA-Yaw*/
	{
	case 0:
		/*Disabled*/
		break;
	case 1:
		AAYaw::Backward(pCmd);
		break;

	case 2:
		AAYaw::Static(pCmd);
		break;

	case 3:
		AAYaw::Jitter(pCmd);
		break;

	case 4:
		AAYaw::Spinbot(pCmd);
		break;

	case 5:
		AAYaw::FJitter(pCmd, bSendPacket);
		break;

	case 6:
		AAYaw::FakeSpin(pCmd, bSendPacket);
		break;

	case 7:
		AAYaw::FakeBackward(pCmd, bSendPacket);
		break;

	case 8:
		AAYaw::FakeForward(pCmd, bSendPacket);
		break;

	case 9:
		AAYaw::FakeStatic(pCmd, bSendPacket);
		break;
	}
	static bool OffsetJitter = false;
	OffsetJitter = !OffsetJitter;

	if (pCmd->command_number % 3) {

		if (OffsetJitter)
			pCmd->viewangles.y -= Menu::Window.RageBotTab.AntiAimOffset.GetValue();
		else
			pCmd->viewangles.y += Menu::Window.RageBotTab.AntiAimOffset.GetValue();
	}
}
