/*
Syn's AyyWare Framework 2015
*/

#define MakePtr( Type, dwBase, dwOffset ) ( ( Type )( DWORD( dwBase ) + (DWORD)( dwOffset ) ) )
#define M_RADPI 57.295779513082f
#define square( x ) ( x * x )

// Any init here
void CVisuals::Init()
{
	// Idk
}

// Don't really need to do anything in here
void CVisuals::Move(CUserCmd *pCmd, bool& bSendPacket)
{

}

// Main ESP Drawing loop
void CVisuals::Draw()
{
	IClientEntity* pLocal = hackManager.pLocal();
	// Crosshair
	if (pLocal->IsAlive()) {
		if (Menu::Window.VisualsTab.OtherFOVCrosshair.GetState())
			DrawFOVCrosshair();
		else if (!Menu::Window.VisualsTab.OtherFOVCrosshair.GetState())
		{

		}

		// Recoil Crosshair
		if (Menu::Window.VisualsTab.OtherSpreadCrosshair.GetState())
			DrawSpreadCrosshair();
		else if (!Menu::Window.VisualsTab.OtherSpreadCrosshair.GetState())
		{

		}
	}

	//	if (Menu::Window.VisualsTab.EffectASUS.GetState())
	//		ASUSOn();
	//	else if (!Menu::Window.VisualsTab.EffectASUS.GetState())
	//		ASUSOff();

	//if (Menu::Window.VisualsTab.EffectNoSky.GetState())
	//	NoSkyON();
	//	else if (!Menu::Window.VisualsTab.EffectNoSky.GetState())
	//	NoSkyOFF();

	if (Menu::Window.VisualsTab.EffectParticles.GetState())
		Particles();
	else if (!Menu::Window.VisualsTab.EffectParticles.GetState()) {
		static auto r_drawparticles = Interfaces::CVar->FindVar(XorStr("r_drawparticles"));
		r_drawparticles->nFlags &= FCVAR_CHEAT;

		static auto mat_postprocess_enable = Interfaces::CVar->FindVar(XorStr("mat_postprocess_enable"));
		mat_postprocess_enable->nFlags &= FCVAR_CHEAT;

		r_drawparticles->SetValue(1);
		mat_postprocess_enable->SetValue(1);
	}

	if (Menu::Window.VisualsTab.OtherSniperCrosshair.GetState() && !pLocal->IsScoped() && pLocal->IsAlive())
	{
		ConVar* crosshair = Interfaces::CVar->FindVar("weapon_debug_spread_show");
		crosshair->SetValue(3);
	}
	else if (!Menu::Window.VisualsTab.OtherSniperCrosshair.GetState() || pLocal->IsScoped() || !pLocal->IsAlive())
	{
		ConVar* crosshairs = Interfaces::CVar->FindVar("weapon_debug_spread_show");
		crosshairs->SetValue(0);
	}
}

void CVisuals::ThirdPerson()
{
	IClientEntity *pLocal = hackManager.pLocal();

	static auto ThirdPerson = false;

	if (pLocal->IsAlive()) {
		Interfaces::Engine->ClientCmd_Unrestricted("thirdperson");
		ThirdPerson = true;
	}
	else {
		Interfaces::Engine->ClientCmd_Unrestricted("firstperson");
		ThirdPerson = false;
	}
}

void CVisuals::Particles()
{
	static auto r_drawparticles = Interfaces::CVar->FindVar(XorStr("r_drawparticles"));
	r_drawparticles->nFlags &= FCVAR_CHEAT;

	static auto mat_postprocess_enable = Interfaces::CVar->FindVar(XorStr("mat_postprocess_enable"));
	mat_postprocess_enable->nFlags &= FCVAR_CHEAT;

	r_drawparticles->SetValue(0);
	mat_postprocess_enable->SetValue(0);
}

void CVisuals::ASUSOn()
{
	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		for (MaterialHandle_t i = Interfaces::MaterialSystem->FirstMaterial(); i != Interfaces::MaterialSystem->InvalidMaterial(); i = Interfaces::MaterialSystem->NextMaterial(i))
		{
			IMaterial *pMaterial = Interfaces::MaterialSystem->GetMaterial(i);

			if (!pMaterial)
				continue;

			if (strstr(pMaterial->GetTextureGroupName(), charenc("World")))
			{
				pMaterial->AlphaModulate(0.4f);
				pMaterial->SetMaterialVarFlag(MATERIAL_VAR_OPAQUETEXTURE, true);
				pMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
			}
		}
	}
}
void CVisuals::ASUSOff()
{
	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		for (MaterialHandle_t i = Interfaces::MaterialSystem->FirstMaterial(); i != Interfaces::MaterialSystem->InvalidMaterial(); i = Interfaces::MaterialSystem->NextMaterial(i))
		{
			IMaterial *pMaterial = Interfaces::MaterialSystem->GetMaterial(i);

			if (!pMaterial)
				continue;

			if (strstr(pMaterial->GetTextureGroupName(), charenc("World")))
			{
				pMaterial->AlphaModulate(1.0f);
				pMaterial->SetMaterialVarFlag(MATERIAL_VAR_OPAQUETEXTURE, false);
				pMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
			}
		}
	}
}

void CVisuals::NoSkyON()
{
	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		for (MaterialHandle_t i = Interfaces::MaterialSystem->FirstMaterial(); i != Interfaces::MaterialSystem->InvalidMaterial(); i = Interfaces::MaterialSystem->NextMaterial(i))
		{
			IMaterial *pMaterial = Interfaces::MaterialSystem->GetMaterial(i);

			if (!pMaterial)
				continue;

			if (strstr(pMaterial->GetTextureGroupName(), charenc("Sky")))
			{
				pMaterial->ColorModulate(0.0f, 0.0f, 0.0f);
			}
		}
	}
}
void CVisuals::NoSkyOFF()
{
	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		for (MaterialHandle_t i = Interfaces::MaterialSystem->FirstMaterial(); i != Interfaces::MaterialSystem->InvalidMaterial(); i = Interfaces::MaterialSystem->NextMaterial(i))
		{
			IMaterial *pMaterial = Interfaces::MaterialSystem->GetMaterial(i);

			if (!pMaterial)
				continue;

			if (strstr(pMaterial->GetTextureGroupName(), charenc("Sky")))
			{
				pMaterial->ColorModulate(1.00f, 1.00f, 1.00f);
			}
		}
	}
}

// Draw a basic crosshair
void CVisuals::DrawFOVCrosshair()
{
	int xs;
	int ys;

	float fov = Menu::Window.RageBotTab.AimbotFov.GetValue();

	Interfaces::Engine->GetScreenSize(xs, ys);
	Interfaces::Engine->GetScreenSize(xs, ys);
	xs /= 2; ys /= 2;
	Render::DrawCircle(xs, ys, fov * 6.25, fov * 6.25, Color(0, 0, 255, 255));
}

// Recoil crosshair
void CVisuals::DrawSpreadCrosshair()
{
	IClientEntity *pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	int xs;
	int ys;
	Interfaces::Engine->GetScreenSize(xs, ys);
	xs /= 2; ys /= 2;

	auto accuracy = pWeapon->GetAccuracyPenalty() * 3000.f;
	while (accuracy > 200) {
		accuracy /= 2;
	}

	Render::DrawCircle(xs, ys, accuracy, accuracy, Color(255, 0, 0, 255));
}