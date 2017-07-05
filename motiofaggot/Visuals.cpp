#include "Visuals.h"
#include "Interfaces.h"
#include "RenderManager.h"
bool IsActive = false;
// Any init here
void CVisuals::Init()
{
	// Idk
}

// Don't really need to do anything in here
void CVisuals::Move(CUserCmd *pCmd, bool& bSendPacket) {}

// Main ESP Drawing loop
void CVisuals::Draw()
{
	if (!Menu::Window.VisualsTab.OtherCrosshairType.GetState())
		Interfaces::Engine->ClientCmd_Unrestricted("crosshair 1");

	NoScopeBorder();

	if (!hackManager.pLocal())
		return;

	// Crosshair
	if (Menu::Window.VisualsTab.OtherCrosshairType.GetState())
	{
		Interfaces::Engine->ClientCmd_Unrestricted("crosshair 0");

		if (Menu::Window.VisualsTab.OtherCrosshairType.GetState())
		{
			DrawCrosshair();
		}

	}
}

void CVisuals::NoScopeBorder()
{
	if (Menu::Window.MiscTab.OtherThirdperson.GetState())
	{
		if (hackManager.pLocal()->IsScoped() && hackManager.pLocal()->IsAlive())
		{
			int Width;
			int Height;
			Interfaces::Engine->GetScreenSize(Width, Height);

			Color cColor = Color(0, 0, 0, 255);
			Render::Line(Width / 2, 0, Width / 2, Height, cColor);
			Render::Line(0, Height / 2, Width, Height / 2, cColor);


		}
		else
		{

		}
	}
}


// Draw a basic crosshair
void CVisuals::DrawCrosshair()
{
	if (Menu::Window.VisualsTab.OtherCrosshair.GetState())
	{
		DrawRecoilCrosshair();
	}
	else
	{
		// Crosshair
		RECT View = Render::GetViewport();
		int MidX = View.right / 2;
		int MidY = View.bottom / 2;

		Render::Line(MidX - 10, MidY, MidX + 10, MidY, Color(255, 0, 0, 255));
		Render::Line(MidX, MidY - 10, MidX, MidY + 10, Color(255, 0, 0, 255));
	}
}

// Recoil crosshair
void CVisuals::DrawRecoilCrosshair()
{
	IClientEntity *pLocal = hackManager.pLocal();

	// Get the view with the recoil
	Vector ViewAngles;
	Interfaces::Engine->GetViewAngles(ViewAngles);
	ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	// Build a ray going fowards at that angle
	Vector fowardVec;
	AngleVectors(ViewAngles, &fowardVec);
	fowardVec *= 10000;

	// Get ray start / end
	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = start + fowardVec, endScreen;

	if (Render::WorldToScreen(end, endScreen) && pLocal->IsAlive())
	{
		RECT View = Render::GetViewport();
#define drX endScreen.x
#define drY endScreen.y
		Color color = Color(255, 0, 0, 255);
		if (Menu::Window.VisualsTab.OtherCrosshair.GetState())
		{
			Render::Line(endScreen.x, endScreen.y, endScreen.x, endScreen.y - 5, color);
			Render::Line(drX, drY - 5, drX + 5, drY - 5, color);

			Render::Line(drX, drY, drX + 5, drY, color);
			Render::Line(drX + 5, drY, drX + 5, drY + 5, color);

			Render::Line(drX, drY, drX, drY + 5, color);
			Render::Line(drX, drY + 5, drX - 5, drY + 5, color);

			Render::Line(drX, drY, drX - 5, drY, color);
			Render::Line(drX - 5, drY, drX - 5, drY - 5, color);
		}


		//	Render::Line(endScreen.x - 10, endScreen.y, endScreen.x + 10, endScreen.y, Color(255, 0, 0, 255));
		//Render::Line(endScreen.x, endScreen.y - 10, endScreen.x, endScreen.y + 10, Color(255, 0, 0, 255));
	}
}
