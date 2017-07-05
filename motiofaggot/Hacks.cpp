#include "Hacks.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include <algorithm>

#include "ESP.h"
#include "Visuals.h"
#include "RageBot.h"
#include "MiscHacks.h"
#include "LegitBot.h"

CEsp Esp;
CVisuals Visuals;
CMiscHacks MiscHacks;
CRageBot RageBot;
//CLegitBot LegitBot;


float clip(float n, float lower, float upper) {
	return (std::max)(lower, (std::min)(n, upper));
}


// Initialise and register ALL hackmanager hacks in here nigga
void Hacks::SetupHacks()
{
	Esp.Init();
	Visuals.Init();
	MiscHacks.Init();
	RageBot.Init();
	//LegitBot.Init();

	hackManager.RegisterHack(&Esp);
	hackManager.RegisterHack(&Visuals);
	hackManager.RegisterHack(&MiscHacks);
	hackManager.RegisterHack(&RageBot);
//	hackManager.RegisterHack(&LegitBot);

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
							char buf[255]; sprintf_s(buf, "%s => %s", pinfo.m_szPlayerName, pinfo2.m_szPlayerName);
							RECT TextSize = Render::GetTextSize(Render::Fonts::ESP, buf);
							Render::Clear(scrn.right - 260, (scrn.bottom / 2) + (16 * ayy), 260, 16, Color(0, 0, 0, 255));
							Render::Text(scrn.right - TextSize.right - 4, (scrn.bottom / 2) + (16 * ayy), pTarget->GetIndex() == pLocal->GetIndex() ? Color(200, 0, 0, 240) : Color(0, 200, 0, 240), Render::Fonts::ESP, buf);
							ayy++;
						}
					}
				}
			}
		}
	}

	Render::Outline(scrn.right - 261, (scrn.bottom / 2) - 1, 262, (16 * ayy) + 2, Color(209, 209, 45, 255));
	Render::Outline(scrn.right - 260, (scrn.bottom / 2), 260, (16 * ayy), Color(2209, 209, 45, 255));
}

void DiLight()
{
	IClientEntity *pLocal = hackManager.pLocal();

	RECT scrn = Render::GetViewport();

	// Loop through all active entitys
	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		player_info_t pinfo;

		// The entity isn't some laggy peice of shit or something
		if (pEntity &&  pEntity != pLocal)
		{
			if (Interfaces::Engine->GetPlayerInfo(i, &pinfo) && pEntity->IsAlive() && !pEntity->IsDormant())
			{
				dlight_t* pElight = Interfaces::Dilight->CL_AllocElight(i);
				pElight->origin = pEntity->GetOrigin() + Vector(0.0f, 0.0f, 35.0f);
				pElight->radius = 2000.0f;
				pElight->color.b = Menu::Window.ColorsTab.GlowB.GetValue();
				pElight->color.g = Menu::Window.ColorsTab.GlowG.GetValue();
				pElight->color.r = Menu::Window.ColorsTab.GlowR.GetValue();
				pElight->die = Interfaces::Globals->curtime + 0.05f;
				pElight->decay = pElight->radius / 5.0f;
				pElight->key = i;

				dlight_t* pDlight = Interfaces::Dilight->CL_AllocDlight(i);
				pDlight->origin = pEntity->GetOrigin();
				pDlight->radius = 2000.0f;
				pDlight->color.b = Menu::Window.ColorsTab.GlowB.GetValue();
				pDlight->color.g = Menu::Window.ColorsTab.GlowG.GetValue();
				pDlight->color.r = Menu::Window.ColorsTab.GlowR.GetValue();
				pDlight->die = Interfaces::Globals->curtime + 0.05f;
				pDlight->decay = pDlight->radius; // / 5.0f;
				pDlight->key = i;
			}
		}
	}
}

// Only gets called in game, use a seperate draw UI call for menus in the hook
void Hacks::DrawHacks()
{
	// Check the master visuals switch, just to be sure
//	if (!Menu::Window.VisualsTab.Active.GetState())
//		return;

	//// Spectator List
	//if (Menu::Window.VisualsTab.OtherSpectators.GetState())
	//	SpecList();

	// Dilight
//	if (Menu::Window.VisualsTab.OptionsDilights.GetState())
//		DiLight();

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
	if (AirStuckKey > 0 && GUI.GetKeyState(AirStuckKey))
	{
		if (pCmd->buttons & IN_ATTACK || pCmd->buttons & IN_ATTACK2)
			return;

			pCmd->tick_count = INT_MAX;
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

void f12aggot4849848911891985156()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}

void f12ag489489489got11151651658918918951186()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}

void f12aggo489489189819t15151987986()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12189148948989189aggot15156741894()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot789789711891891mklmlkmlkmlk895156489489489()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12a987987ggot1515648948ststdter94984984984()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot17897987951513131serterteter313136()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12ag7987897got151561313ertertadfd13189798()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12ag98got1489489515azhaertgdxfdfg89767898798797897897984151()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot89798798715156gergsdgsfed9879816351687987()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12a789789798ggot151561dnfgkjdfgnsdjkfgnsdfjk568979812313()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggo1561651t15789798789715sdgopkgporksopkdof6897()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12a7987987aysfsdfdrzhrthdggot15156789797854646597()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot1512361201567897ysdtgegdfbdfgdr8978979789789()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot151156165ermvklmrlksmrklsmrkl4564564578978978979878978971111564136()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f121561651aggot151564174217sdfmsklfmsdklfmerlkzmelkgms84197219718271()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f124984894agdgfgjfghfgbfggot151564114216792()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f124189498498aggofhfhrtufgft151562713879124819421718972189()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12adhfhdfbfgjuzjkggot15156108979874218971279812412()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot1518dfhmdfklghmslkdvmdflkgmdkfl94984151421741729296()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12agg849984ot15156dgmdfklgmdklfgmdflg189198()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot1489498515cf489dhfhdhd498bbhhfgh6()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot1515489489489hsdhfghfghfghtdffghh1rt5616()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot151erghe489489gdgdfgdfr5g9dgdf98df1g56()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}