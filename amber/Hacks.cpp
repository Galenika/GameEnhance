/*
Syn's Payhake Framework for Insanity & Razor
*/

#include "Hacks.h"
#include "Interfaces.h"
#include "RenderManager.h"

#include "ESP.h"
#include "Visuals.h"
#include "RageBot.h"
#include "MiscHacks.h"
#include "LegitBot.h"

CEsp Esp;
CVisuals Visuals;
CMiscHacks MiscHacks;
CRageBot RageBot;
CLegitBot LegitBot;

// Initialise and register ALL hackmanager hacks in here nigga
void Hacks::SetupHacks()
{
	Esp.Init();
	Visuals.Init();
	MiscHacks.Init();
	RageBot.Init();
	LegitBot.Init();

	hackManager.RegisterHack(&Esp);
	hackManager.RegisterHack(&Visuals);
	hackManager.RegisterHack(&MiscHacks);
	hackManager.RegisterHack(&RageBot);
	hackManager.RegisterHack(&LegitBot);

	//--------------------------------
	hackManager.Ready();
}

void SpecList()
{
	IClientEntity *pLocal = hackManager.pLocal();

	RECT scrn = Render::GetViewport();
	int ayy = 0;

	// Loop through all active entitys
	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		player_info_t pinfo;

		// The entity isn't some laggy peice of shit or something
		if (pEntity &&  pEntity != pLocal)
		{
			if (Interfaces::Engine->GetPlayerInfo(i, &pinfo) && !pEntity->IsAlive() && !pEntity->IsDormant())
			{
				HANDLE obs = pEntity->GetObserverTargetHandle();

				if (obs)
				{
					IClientEntity *pTarget = Interfaces::EntList->GetClientEntityFromHandle(obs);
					player_info_t pinfo2;
					if (pTarget)
					{
						if (Interfaces::Engine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
						{
							char buf[255]; sprintf_s(buf, "%s => %s", pinfo.name, pinfo2.name);
							RECT TextSize = Render::GetTextSize(Render::Fonts::ESP, buf);
							Render::Text(scrn.right - TextSize.right - 4, (scrn.bottom / 2) + (16 * ayy), pTarget->GetIndex() == pLocal->GetIndex() ? Color(240, 70, 80, 255) : Color(255, 255, 255, 255), Render::Fonts::ESP, buf);
							ayy++;
						}
					}
				}
			}
		}
	}
}

// Only gets called in game, use a seperate draw UI call for menus in the hook
void Hacks::DrawHacks()
{
	// Spectator List
	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) {
		if (Menu::Window.VisualsTab.OtherSpectatorList.GetState())
			SpecList();
	}

	// Check the master visuals switch, just to be sure
	if (!Menu::Window.VisualsTab.Active.GetState())
		return;

	hackManager.Draw();
	//--------------------------------

}

// Game Cmd Changes
void Hacks::MoveHacks(CUserCmd *pCmd, bool& bSendPacket)
{
	Vector origView = pCmd->viewangles;
	hackManager.Move(pCmd, bSendPacket);
	// ------------------------------

	// Put it in here so it's applied AFTER the aimbot
	int AirStuckKey = Menu::Window.MiscTab.OtherAirStuck.GetKey();
	int SlowMoKey = Menu::Window.MiscTab.SlowMotion.GetKey();

	IClientEntity* pLocal = hackManager.pLocal();

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) {
		int iFlags = pLocal->GetFlags();
		if (pCmd->buttons & IN_JUMP && GUI.GetKeyState(VK_SPACE))
		{
			if ((pLocal->GetFlags() & (int)iFlags & FL_ONGROUND))
			{
				return;
			}
		}
		else {

			if (AirStuckKey > 0 && GUI.GetKeyState(AirStuckKey))
			{
				if (!(pCmd->buttons & IN_ATTACK))
					pCmd->tick_count = 16777216;
			}

			if (SlowMoKey > 0 && GUI.GetKeyState(SlowMoKey))
			{
				//owned
			}
		}
	}
	else {
		return;
	}
}

//---------------------------------------------------------------------//
HackManager hackManager;

// Register a new hack
void HackManager::RegisterHack(CHack* hake)
{
	Hacks.push_back(hake);
	hake->Init();
}

// Draw all the hakes
void HackManager::Draw()
{
	if (!IsReady)
		return;

	// Grab the local player for drawing related hacks
	pLocalInstance = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalInstance) return;

	for (auto &hack : Hacks)
	{
		hack->Draw();
	}
}

// Handle all the move hakes
void HackManager::Move(CUserCmd *pCmd, bool& bSendPacket)
{
	if (!IsReady)
		return;

	// Grab the local player for move related hacks
	pLocalInstance = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalInstance) return;

	for (auto &hack : Hacks)
	{
		hack->Move(pCmd, bSendPacket);
	}
}

//---------------------------------------------------------------------//
// Other Utils and shit

// Saves hacks needing to call a bunch of virtuals to get the instance
// Saves on cycles and file size. Recycle your plastic kids
IClientEntity* HackManager::pLocal()
{
	return pLocalInstance;
}

// Makes sure none of the hacks are called in their 
// hooks until they are completely ready for use
void HackManager::Ready()
{
	IsReady = true;
}