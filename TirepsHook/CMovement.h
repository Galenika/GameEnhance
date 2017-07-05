#pragma once
#include "CLegitBot.h"
#include "CKnifeBot.h"

namespace CMovement
{

	void RunBhop(CPlayer* me, CUserCmd* cmd)
	{
		if (cmd->buttons & IN_JUMP && !(me->GetFlags() & FL_ONGROUND))
			cmd->buttons &= ~IN_JUMP;
	}
	
	void RunAntiAFK(CUserCmd* cmd)
	{
		static bool Jitter;
		Jitter = !Jitter;
		if (Jitter)
			cmd->sidemove += 450;
		else
			cmd->sidemove = -450;

		if (!Jitter)
			cmd->forwardmove = -450;
		else
			cmd->forwardmove = +450;

		cmd->buttons += IN_MOVELEFT;
	}

	void RunZeusTrigger(CPlayer* me, CUserCmd* cmd)
	{
		/*trace_t tr;
		Ray_t ray;
		CTraceFilter filter;

		filter.pSkip = me;

		Vector3 ang = Vector3();
		AngleVectors(ang, Vector3());
		ray.Init(me->GetEyePosition(), ang);

		Interface.EngineTrace->TraceRay(ray, 0x46004003, &filter, &tr);*/
	}

	void RunVisibleCheck(CPlayer* me)
	{
		G::VisibledPlayers.clear();

		Vector3 LocalPosition = me->GetEyePosition();
		for (int i = 0; i < Interface.Engine->GetMaxClients(); i++) {
			CPlayer* pBaseEntity = Interface.EntityList->GetClientEntity<CPlayer>(i);

			if (!pBaseEntity)
				continue;
			if (!pBaseEntity->IsValid())
				continue;
			if (pBaseEntity == me)
				continue;

			G::VisibledPlayers[i] = U::IsVisible(LocalPosition, pBaseEntity->GetEyePosition(), me, pBaseEntity, false);
		}
	}

	bool __stdcall CreateMove(float frametime, CUserCmd* cmd)
	{
		O::CreateMove(frametime, cmd);

		if (H::isEjecting)
			return false;

		uintptr_t* fp;
		__asm mov fp, ebp;
		bool* bSendPacket = (bool*)(*fp - 0x1C);

		if (!cmd || !cmd->command_number)
			return false;

		CPlayer* me = Interface.EntityList->GetClientEntity<CPlayer>(Interface.Engine->GetLocalPlayer());

		RunVisibleCheck(me);

		if (!me->IsValid())
			return false;

		G::GlobalCmd_r = cmd;
		G::GlobalLocalPlayer_r = me;

		if (Options::Misc::Bunnyhop) RunBhop(me, cmd);
		if (Options::Misc::AntiAFK) RunAntiAFK(cmd);
		if (true/*Options::Misc::ZeusTrigger*/) RunZeusTrigger(me, cmd);
		if (Options::Legitbot::Enabled) CLegitBot::Run(cmd, bSendPacket);
		if (Options::Misc::KnifeBot) CKnifeBot::Run(cmd, bSendPacket);
		if (Options::Misc::AirStuck && GetAsyncKeyState(0xA4)) cmd->tick_count = 16777216;

		U::ClampAngles(cmd->viewangles);
		U::AngleNormalize(cmd->viewangles);
		return false;
	}
}
