#include "ESP.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "GlowManager.h"



void CEsp::Init()
{
	BombCarrier = nullptr;
}

// Yeah dude we're defo gunna do some sick moves for the esp yeah
void CEsp::Move(CUserCmd *pCmd, bool& bSendPacket) {}

// Main ESP Drawing loop
void CEsp::Draw()
{
	IClientEntity *pLocal = hackManager.pLocal();

	// Loop through all active entitys
	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		player_info_t pinfo;

		// The entity isn't some laggy peice of shit or something
		if (pEntity && !pEntity->IsDormant())
		{
			// Is it a player?!
			if (Menu::Window.VisualsTab.FiltersPlayers.GetState() && Interfaces::Engine->GetPlayerInfo(i, &pinfo) && pEntity->IsAlive())
			{
				DrawPlayer(pEntity, pinfo);
			}

			// ~ Other ESP's here (items and shit) ~ //
			ClientClass* cClass = (ClientClass*)pEntity->GetClientClass();

			// Dropped weapons
			if (Menu::Window.VisualsTab.FiltersWeapons.GetState() && cClass->m_ClassID != (int)CSGOClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)CSGOClassID::CDEagle || cClass->m_ClassID == (int)CSGOClassID::CAK47)))
			{
				DrawDrop(pEntity, cClass);
			}

			// If entity is the bomb
			if (Menu::Window.VisualsTab.FiltersC4.GetState())
			{
				if (cClass->m_ClassID == (int)CSGOClassID::CPlantedC4)
					DrawBombPlanted(pEntity, cClass);

				if (cClass->m_ClassID == (int)CSGOClassID::CC4)
					DrawBomb(pEntity, cClass);
			}

			// If entity is a chicken
			if (Menu::Window.VisualsTab.FiltersChickens.GetState())
			{
				if (cClass->m_ClassID == (int)CSGOClassID::CChicken)
					DrawChicken(pEntity, cClass);
			}

		/*	if (Menu::Window.VisualsTab.Grenades.GetState())
				CEsp::DrawGrenades(pEntity);
				*/
		}
	}


}

//  Yeah m8
void CEsp::DrawPlayer(IClientEntity* pEntity, player_info_t pinfo)
{
	ESPBox Box;
	Color color;

	// Show own team false? well gtfo teammate
	if (Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() && (pEntity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
		return;

	if (GetBox(pEntity, Box))
	{
		color = GetPlayerColor(pEntity);

		if (Menu::Window.VisualsTab.OptionsBox.GetIndex() > 0)
			DrawBox(Box, color);

		if (Menu::Window.VisualsTab.OptionsHealth.GetIndex() > 0)
			DrawHealth(pEntity, Box);

		if (Menu::Window.VisualsTab.HasC4.GetState() || Menu::Window.VisualsTab.OptionsWeapon.GetIndex() > 0 || Menu::Window.VisualsTab.HasDefuser.GetState() || Menu::Window.VisualsTab.IsScoped.GetState() || Menu::Window.VisualsTab.IsDefusing.GetState())
			DrawInfo(pEntity, Box);

		//if (Menu::Window.VisualsTab.OptionsAimSpot.GetState())
		//	DrawCross(pEntity);

		if (Menu::Window.VisualsTab.OptionsSkeleton.GetState())
		{
			//DrawSkeleton(pEntity);
			Barrel(Box, color, pEntity);
		}




		if (Menu::Window.VisualsTab.OptionsName.GetState())
		{
			DrawName(pinfo, Box);
		}

	}
}



// Gets the 2D bounding box for the entity
// Returns false on failure nigga don't fail me
bool CEsp::GetBox(IClientEntity* pEntity, CEsp::ESPBox &result)
{
	// Variables
	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	// Get the locations
	vOrigin = pEntity->GetOrigin();
	min = pEntity->collisionProperty()->GetMins() + vOrigin;
	max = pEntity->collisionProperty()->GetMaxs() + vOrigin;

	// Points of a 3d bounding box
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	// Get screen positions
	if (!Render::WorldToScreen(points[3], flb) || !Render::WorldToScreen(points[5], brt)
		|| !Render::WorldToScreen(points[0], blb) || !Render::WorldToScreen(points[4], frt)
		|| !Render::WorldToScreen(points[2], frb) || !Render::WorldToScreen(points[1], brb)
		|| !Render::WorldToScreen(points[6], blt) || !Render::WorldToScreen(points[7], flt))
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	// Width / height
	result.x = left;
	result.y = top;
	result.w = right - left;
	result.h = bottom - top;

	return true;
}

void CEsp::DrawNameRainbow(player_info_t pinfo, CEsp::ESPBox size)
{

	static float rainbow;
	rainbow += 0.005f;
	if (rainbow > 1.f) rainbow = 0.f;

	RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.m_szPlayerName);
	Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 16,
		Color::FromHSB(rainbow, 1.f, 1.f), Render::Fonts::ESP, pinfo.m_szPlayerName);
}

void CEsp::DrawSkeletonRainbow(IClientEntity* pEntity)
{

	static float rainbow;
	rainbow += 0.005f;
	if (rainbow > 1.f) rainbow = 0.f;

	studiohdr_t* pStudioHdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);

			if (Render::WorldToScreen(vParent, sParent) && Render::WorldToScreen(vChild, sChild))
			{
				Render::Line(sParent[0], sParent[1], sChild[0], sChild[1], Color::FromHSB(rainbow, 1.f, 1.f));
			}
		}
	}
}

// Get an entities color depending on team and vis ect
Color CEsp::GetPlayerColor(IClientEntity* pEntity)
{
	int TeamNum = pEntity->GetTeamNum();
	bool IsVis = GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Head) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Neck);

	Color color;

	if (TeamNum == TEAM_CS_T)
	{
		if (IsVis)
			color = Color(Menu::Window.ColorsTab.TColorVisR.GetValue(), Menu::Window.ColorsTab.TColorVisG.GetValue(), Menu::Window.ColorsTab.TColorVisB.GetValue(), 255);
		else
			color = Color(Menu::Window.ColorsTab.TColorNoVisR.GetValue(), Menu::Window.ColorsTab.TColorNoVisG.GetValue(), Menu::Window.ColorsTab.TColorNoVisB.GetValue(), 255);
	}
	else
	{
		if (IsVis)
			color = Color(Menu::Window.ColorsTab.CTColorVisR.GetValue(), Menu::Window.ColorsTab.CTColorVisG.GetValue(), Menu::Window.ColorsTab.CTColorVisB.GetValue(), 255);
		else
			color = Color(Menu::Window.ColorsTab.CTColorNoVisR.GetValue(), Menu::Window.ColorsTab.CTColorNoVisG.GetValue(), Menu::Window.ColorsTab.CTColorNoVisB.GetValue(), 255);
	}


	return color;
}

// 2D  Esp box
void CEsp::DrawBox(CEsp::ESPBox size, Color color)
{
	int VertLine = (((float)size.w) * (1.0f));
	int HorzLine = (((float)size.h) * (1.0f));
#define BoxType Menu::Window.VisualsTab.OptionsBox.GetIndex()
	if (BoxType == 1) // Normal
	{
		Render::Clear(size.x, size.y, VertLine, 1, color);
		Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
		Render::Clear(size.x, size.y + size.h, VertLine, 1, color);
		Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);

		Render::Clear(size.x, size.y, 1, HorzLine, color);
		Render::Clear(size.x, size.y + size.h - HorzLine, 1, HorzLine, color);
		Render::Clear(size.x + size.w, size.y, 1, HorzLine, color);
		Render::Clear(size.x + size.w, size.y + size.h - HorzLine, 1, HorzLine, color);
	}
	if (BoxType == 2) // Bold
	{
		Render::Outline(size.x, size.y, size.w, size.h, color);
		Render::Outline(size.x - 1, size.y - 1, size.w + 2, size.h + 2, Color(10, 10, 10, 150));
		Render::Outline(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(10, 10, 10, 150));
	}
	if (BoxType == 3) // Corners
	{
		float lineW = (size.w / 5);
		float lineH = (size.h / 6);
		float lineT = 1;
		float W = size.w;
		float H = size.h;
#define X size.x
#define Y size.y
#define Surface Interfaces::Surface
		//outline
		Surface->DrawSetColor(Color(0, 0, 0, 255));
		Surface->DrawLine(X - lineT, Y - lineT, X + lineW, Y - lineT); //top left
		Surface->DrawLine(X - lineT, Y - lineT, X - lineT, Y + lineH);
		Surface->DrawLine(X - lineT, Y + size.h - lineH, X - lineT, Y + size.h + lineT); //bot left
		Surface->DrawLine(X - lineT, Y + size.h + lineT, X + lineW, Y + size.h + lineT);
		Surface->DrawLine(X + size.w - lineW, Y - lineT, X + size.w + lineT, Y - lineT); // top right
		Surface->DrawLine(X + size.w + lineT, Y - lineT, X + W + lineT, Y + lineH);
		Surface->DrawLine(X + W + lineT, Y + size.h - lineH, X + W + lineT, Y + H + lineT); // bot right
		Surface->DrawLine(X + W - lineW, Y + H + lineT, X + W + lineT, Y + H + lineT);

		//inline
		Surface->DrawSetColor(color);
		Surface->DrawLine(X, Y, X, Y + lineH);//top left
		Surface->DrawLine(X, Y, X + lineW, Y);
		Surface->DrawLine(X + W - lineW, Y, X + W, Y); //top right
		Surface->DrawLine(X + W, Y, X + W, Y + lineH);
		Surface->DrawLine(X, Y + H - lineH, X, Y + H); //bot left
		Surface->DrawLine(X, Y + H, X + lineW, Y + H);
		Surface->DrawLine(X + W - lineW, Y + H, X + W, Y + H);//bot right
		Surface->DrawLine(X + W, Y + H - lineH, X + W, Y + H);
	}
}
void CEsp::Barrel(CEsp::ESPBox size, Color color, IClientEntity* pEntity)
{
	Vector src3D, src;
	src3D = pEntity->GetOrigin() - Vector(0, 0, 0);

	if (!Render::WorldToScreen(src3D, src))
		return;

	int ScreenWidth, ScreenHeight;
	Interfaces::Engine->GetScreenSize(ScreenWidth, ScreenHeight);

	int x = (int)(ScreenWidth * 0.5f);
	int y = 0;


		y = ScreenHeight;
		
	Render::Line((int)(src.x), (int)(src.y), x, y, Color(0,255,0,255));
}
// 2D  Esp box
void CEsp::DrawBox1(CEsp::ESPBox size, Color color)
{
	//if (PlayerBoxes->GetStringIndex() == 1)
	//{
	//	// Full Box
	//	renderMan->Outline(size.x, size.y, size.w, size.h, color);
	//	renderMan->Outline(size.x - 1, size.y - 1, size.w + 2, size.h + 2, COLORCODE(10, 10, 10, 150));
	//	renderMan->Outline(size.x + 1, size.y + 1, size.w - 2, size.h - 2, COLORCODE(10, 10, 10, 150));
	//}
	//else
	//{
	// Corner Box
	int VertLine = (((float)size.w) * (0.20f));
	int HorzLine = (((float)size.h) * (0.20f));

	Render::Clear(size.x, size.y - 1, VertLine, 1, Color(30, 30, 30, 100));
	Render::Clear(size.x + size.w - VertLine, size.y - 1, VertLine, 1, Color(30, 30, 30, 100));
	Render::Clear(size.x, size.y + size.h - 1, VertLine, 1, Color(30, 30, 30, 100));
	Render::Clear(size.x + size.w - VertLine, size.y + size.h - 1, VertLine, 1, Color(30, 30, 30, 100));
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(30, 30, 30, 100));

	Render::Clear(size.x - 1, size.y, 5, HorzLine, Color(30, 30, 30, 100));
	Render::Clear(size.x - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(30, 30, 30, 100));
	Render::Clear(size.x + size.w - 5, size.y, 1, HorzLine, Color(30, 30, 30, 100));
	Render::Clear(size.x + size.w - 5, size.y + size.h - HorzLine, 1, HorzLine, Color(30, 30, 30, 100));
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(30, 30, 30, 100));

	Render::Clear(size.x, size.y, VertLine, 1, Color(30, 30, 30, 100));
	Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, Color(30, 30, 30, 100));
	Render::Clear(size.x, size.y + size.h, VertLine, 1, Color(30, 30, 30, 100));
	Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, Color(30, 30, 30, 100));
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(30, 30, 30, 100));

	Render::Clear(size.x, size.y, 1, HorzLine, Color(30, 30, 30, 100));
	Render::Clear(size.x, size.y + size.h - HorzLine, 1, HorzLine, Color(30, 30, 30, 100));
	Render::Clear(size.x + size.w, size.y, 1, HorzLine, Color(30, 30, 30, 100));
	Render::Clear(size.x + size.w, size.y + size.h - HorzLine, 1, HorzLine, Color(30, 30, 30, 100));
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(30, 30, 30, 100));
	//}
}

void CEsp::DrawBox2(CEsp::ESPBox size, Color color)
{
	Render::Outline(size.x, size.y, size.w, size.h, color);
	Render::Outline(size.x - 1, size.y - 1, size.w + 2, size.h + 2, Color(10, 10, 10, 150));
	Render::Outline(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(10, 10, 10, 150));
}

void CEsp::DrawBoxAWRainbow(CEsp::ESPBox size, Color color)
{
	static float rainbow;
	rainbow += 0.005f;
	if (rainbow > 1.f) rainbow = 0.f;
	Render::Outline(size.x, size.y, size.w, size.h, Color::FromHSB(rainbow, 1.f, 1.f));
	Render::Outline(size.x - 1, size.y - 1, size.w + 2, size.h + 2, Color::FromHSB(rainbow, 1.f, 1.f));
	Render::Outline(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color::FromHSB(rainbow, 1.f, 1.f));
}
// Unicode Conversions
static wchar_t* CharToWideChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs_s(NULL, wa, size / 4, text, size);
	return wa;
}

// Player name
void CEsp::DrawName(player_info_t pinfo, CEsp::ESPBox size)
{
	RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.m_szPlayerName);
	Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 16,
		Color(255, 255, 255, 255), Render::Fonts::ESP, pinfo.m_szPlayerName);
}
void DrawOutlinedRect(int x, int y, int w, int h, Color col)
{
	Surface->DrawSetColor(col);
	Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void DrawOutlinedCircle(int x, int y, int r, Color col)
{
	Surface->DrawSetColor(col);
	Surface->DrawOutlinedCircle(x, y, r, 1);
}

void DrawLine(int x0, int y0, int x1, int y1, Color col)
{
	Surface->DrawSetColor(col);
	Surface->DrawLine(x0, y0, x1, y1);
}
// Draw a health bar. For Tf2 when a bar is bigger than max health a second bar is displayed
void CEsp::DrawHealth(IClientEntity* pEntity, CEsp::ESPBox size)
{
	ESPBox HealthBar = size;
	HealthBar.y += (HealthBar.h + 6);
	HealthBar.h = 4;

	float HealthValue = pEntity->GetHealth();
	float HealthPerc = HealthValue / 100.f;
	float Width = (size.w * HealthPerc);
	HealthBar.w = Width;
#define HealthType Menu::Window.VisualsTab.OptionsHealth.GetIndex()
	if (HealthType == 1) //Text
	{
		int HPEnemy = 100;
		HPEnemy = pEntity->GetHealth();
		char nameBuffer[512];
		sprintf_s(nameBuffer, "[%d HP]", HPEnemy);

		Render::Text(HealthBar.x, HealthBar.y, Color(255, 255, 255, 255), Render::Fonts::ESP, nameBuffer);

	}

	if (HealthType == 2) //Left
	{
		float h = (size.h);
		float offset = (h / 4.f) + 5;
		float w = h / 64.f;
		float health = pEntity->GetHealth();
		UINT hp = h - (UINT)((h * health) / 100); // Percentage

		int Red = 255 - (health*2.55);
		int Green = health*2.55;

		DrawOutlinedRect((size.x - 6) - 1, size.y - 1, 3, h + 2, Color(0, 0, 0, 255));

		DrawLine((size.x - 6), size.y + hp, (size.x - 6), size.y + h, Color(Red, Green, 0, 255));
	}

	if (HealthType == 3) //Right
	{
		float h = (size.h);
		float offset = (h / 4.f) + 5;
		float w = h / 64.f;
		float health = pEntity->GetHealth();
		UINT hp = h - (UINT)((h * health) / 100); // Percentage

		int Red = 255 - (health*2.55);
		int Green = health*2.55;

		DrawOutlinedRect((size.x + size.w + 6), size.y - 1, 3, h + 2, Color(0, 0, 0, 255));

		DrawLine((size.x + size.w + 6) + 1, size.y + hp, (size.x +size.w + 6), size.y + h, Color(Red, Green, 0, 255));
	}

	if (HealthType == 4) //Under(iWebz, KiduaHook Paste) Credits: Kidua
	{
		float h = (size.h);
		float offset = (h / 4.f) + 5;
		float w = h / 64.f;
		float health = pEntity->GetHealth();
		UINT hp = h - (UINT)((h * health) / 100); // Percentage

		int Red = 255 - (health*2.55);
		int Green = health*2.55;


		Vertex_t Verts[4];
		Verts[0].Init(Vector2D(HealthBar.x, HealthBar.y));
		Verts[1].Init(Vector2D(HealthBar.x + size.w + 5, HealthBar.y));
		Verts[2].Init(Vector2D(HealthBar.x + size.w, HealthBar.y + 5));
		Verts[3].Init(Vector2D(HealthBar.x - 5, HealthBar.y + 5));

		Render::PolygonOutline(4, Verts, Color(10, 10, 10, 255), Color(255, 255, 255, 170));

		Vertex_t Verts2[4];
		Verts2[0].Init(Vector2D(HealthBar.x + 1, HealthBar.y + 1));
		Verts2[1].Init(Vector2D(HealthBar.x + HealthBar.w + 4, HealthBar.y + 1));
		Verts2[2].Init(Vector2D(HealthBar.x + HealthBar.w, HealthBar.y + 5));
		Verts2[3].Init(Vector2D(HealthBar.x - 4, HealthBar.y + 5));


		Render::Polygon(4, Verts2, Color(Red, Green, 0, 255));

		Verts2[0].Init(Vector2D(HealthBar.x + 1, HealthBar.y + 1));
		Verts2[1].Init(Vector2D(HealthBar.x + HealthBar.w + 2, HealthBar.y + 1));
		Verts2[2].Init(Vector2D(HealthBar.x + HealthBar.w, HealthBar.y + 2));
		Verts2[3].Init(Vector2D(HealthBar.x - 2, HealthBar.y + 2));

		Render::Polygon(4, Verts2, Color(255, 255, 255, 40));


	}

}

void CEsp::DrawHealth1(IClientEntity* pEntity, CEsp::ESPBox size)
{
	ESPBox HealthBar = size;
	HealthBar.y += (HealthBar.h + 6);
	HealthBar.h = 4;

	float HealthValue = pEntity->GetHealth();
	float HealthPerc = HealthValue / 100.f;
	float Width = (size.w * HealthPerc);
	HealthBar.w = Width;

	int health = pEntity->GetHealth();

	if (health > 100)
		health = 100;

	int r = 255 - health * 2.55;
	int g = health * 2.55;

	int healthBar = size.h / 100 * health;
	int healthBarDelta = size.h - healthBar;

	//int iClampedHealth = pEntity->GetHealth();
	//  if (iClampedHealth >= 100)
	//  iClampedHealth = 100;

	Render::Outline(size.x - 4, size.y + 1, 1, size.h * 0.01 * health, Color(r, g, 0, 255));
	Render::Outline(size.x - 5, size.y - 1, 3, size.h + 2, Color(0, 0, 0, 150));
}

// Cleans the internal class name up to something human readable and nice
std::string CleanItemName(std::string name)
{
	std::string Name = name;
	// Tidy up the weapon Name
	if (Name[0] == 'C')
		Name.erase(Name.begin());

	// Remove the word Weapon
	auto startOfWeap = Name.find("Weapon");
	if (startOfWeap != std::string::npos)
		Name.erase(Name.begin() + startOfWeap, Name.begin() + startOfWeap + 6);

	return Name;
}

// Anything else: weapons, class state? idk
void CEsp::DrawInfo(IClientEntity* pEntity, CEsp::ESPBox size)
{
	std::vector<std::string> Info;

	// Player Weapon ESP
	IClientEntity* pWeapon = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)pEntity->GetActiveWeaponHandle());
	if (Menu::Window.VisualsTab.OptionsWeapon.GetIndex() == 1 && pWeapon)
	{
		ClientClass* cClass = (ClientClass*)pWeapon->GetClientClass();
		if (cClass)
		{
			// Draw it
			Info.push_back(CleanItemName(cClass->m_pNetworkName));
		}
	}
	if (Menu::Window.VisualsTab.OptionsWeapon.GetIndex() == 2 && pWeapon)
	{
		CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle((HANDLE)hackManager.pLocal()->GetActiveWeaponHandle());
		CSWeaponInfo WeaponInfo = weapon->GetCSWpnData(weapon, 456);
		if (&WeaponInfo == NULL)
			return;
	//	char Icon = WeaponInfo.iconWeapon->charinFont;
	//	std::string meme = std::to_string(Icon);
			// Draw it
		//	Info.push_back(meme);
	}

	// Bomb Carrier
	if (Menu::Window.VisualsTab.HasC4.GetState() && pEntity == BombCarrier)
	{
		Info.push_back("C4 Carrier");
	}
	// Is Scoped
	if (Menu::Window.VisualsTab.IsScoped.GetState() && pEntity->IsScoped())
	{
		Info.push_back("Is Scoped");
	}
	// Has Defuser
	if (Menu::Window.VisualsTab.HasDefuser.GetState() && pEntity->HasDefuser())
	{
		Info.push_back("Has Defuser");
	}
	// Defusing
	if (Menu::Window.VisualsTab.IsDefusing.GetState() && pEntity->IsDefusing())
	{
		Info.push_back("Is Defusing");
	}

	// Comp Rank
	if (Menu::Window.VisualsTab.OptionsCompRank.GetState())
	{
		int CompRank = GameUtils::GetPlayerCompRank(pEntity);
		static const char *Ranks[] =
		{
			"",
			"Silver I",
			"Silver II",
			"Silver III",
			"Silver IV",
			"Silver Elite",
			"Silver Elite Master",

			"Gold Nova I",
			"Gold Nova II",
			"Gold Nova III",
			"Gold Nova Master",
			"Master Guardian I",
			"Master Guardian II",

			"Master Guardian Elite",
			"Distinguished Master Guardian",
			"Legendary Eagle",
			"Legendary Eagle Master",
			"Supreme Master First Class",
			"Global Elite"
		};
		if (CompRank >= 0 && CompRank <= 18)
			Info.push_back(Ranks[CompRank]);
	}

	static RECT Size = Render::GetTextSize(Render::Fonts::ESP, "Text");
	int i = 0;
	for (auto Text : Info)
	{
		Render::Text(size.x + size.w + 3, size.y + (i*(Size.bottom + 1)), Color(255, 255, 255, 255), Render::Fonts::ESP, Text.c_str());
		i++;
	}
}

// Little cross on their heads
void CEsp::DrawCross(IClientEntity* pEntity)
{
	Vector cross = pEntity->GetHeadPos(), screen;
	static int Scale = 2;
	if (Render::WorldToScreen(cross, screen))
	{
		Render::Clear(screen.x - Scale, screen.y - (Scale * 2), (Scale * 2), (Scale * 4), Color(20, 20, 20, 160));
		Render::Clear(screen.x - (Scale * 2), screen.y - Scale, (Scale * 4), (Scale * 2), Color(20, 20, 20, 160));
		Render::Clear(screen.x - Scale - 1, screen.y - (Scale * 2) - 1, (Scale * 2) - 2, (Scale * 4) - 2, Color(250, 250, 250, 160));
		Render::Clear(screen.x - (Scale * 2) - 1, screen.y - Scale - 1, (Scale * 4) - 2, (Scale * 2) - 2, Color(250, 250, 250, 160));
	}
}

// Draws a dropped CS:GO Item
void CEsp::DrawDrop(IClientEntity* pEntity, ClientClass* cClass)
{
	Vector Box;
	CBaseCombatWeapon* Weapon = (CBaseCombatWeapon*)pEntity;
	IClientEntity* plr = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)Weapon->GetOwnerHandle());
	if (!plr && Render::WorldToScreen(Weapon->GetOrigin(), Box))
	{
		if (Menu::Window.VisualsTab.OptionsBox.GetIndex() > 0)
		{
			Render::Outline(Box.x - 2, Box.y - 2, 4, 4, Color(255, 255, 255, 255));
			Render::Outline(Box.x - 3, Box.y - 3, 6, 6, Color(10, 10, 10, 150));
		}

		if (Menu::Window.VisualsTab.OptionsName.GetState())
		{
			std::string ItemName = CleanItemName(cClass->m_pNetworkName);
			RECT TextSize = Render::GetTextSize(Render::Fonts::ESP, ItemName.c_str());
			Render::Text(Box.x - (TextSize.right / 2), Box.y - 16, Color(255, 255, 255, 255), Render::Fonts::MenuBold, ItemName.c_str());
		}
	}
}

// Draws a chicken
void CEsp::DrawChicken(IClientEntity* pEntity, ClientClass* cClass)
{
	ESPBox Box;

	if (GetBox(pEntity, Box))
	{
		player_info_t pinfo; strcpy_s(pinfo.m_szPlayerName, "Chicken");
		if (Menu::Window.VisualsTab.OptionsBox.GetIndex() > 0)
			DrawBox(Box, Color(255, 255, 255, 255));

		if (Menu::Window.VisualsTab.OptionsName.GetState())
			DrawName(pinfo, Box);
	}
}


/*void CEsp::DrawGrenades(IClientEntity* pEntity)
{


	Vector vGrenadePos2D = Vector(0.f, 0.f, 0.f);
	Vector vGrenadePos3D = Vector(0.f, 0.f, 0.f);
	float fGrenadeModelSize = 0.0f;
	Color colGrenadeColor = Color(0, 0, 0, 0);
	const model_t *model = pEntity->GetModel();

	if (pEntity->GetClientClass())
	{
		if (model)
		{
			studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(model);
			if (hdr)
			{
				std::string hdrName = hdr->name;
				if (hdrName.find("thrown") != std::string::npos)
				{
					vGrenadePos3D = pEntity->GetOrigin();
					fGrenadeModelSize = hdr->hull_max.DistTo(hdr->hull_min);

					if (!Render::WorldToScreen(vGrenadePos3D, vGrenadePos2D))
						return;

					if (hdrName.find("flash") != std::string::npos)
					{
						//Warning color
						colGrenadeColor = Color(0, 0, 200, 255);
					}
					else if (hdrName.find("incendiarygrenade") != std::string::npos || hdrName.find("molotov") != std::string::npos || hdrName.find("fraggrenade") != std::string::npos)
					{
						//Dangerous color
						colGrenadeColor = Color(200, 0, 0, 255);
					}

					else if (hdrName.find("smoke") != std::string::npos || hdrName.find("decoy") != std::string::npos)
					{
						//Peaceful color
						colGrenadeColor = Color(0, 200, 0, 255);
					}
					D::DrawOutlinedCircle(static_cast<int>(vGrenadePos2D.x), static_cast<int>(vGrenadePos2D.y), static_cast<int>(fGrenadeModelSize), colGrenadeColor);
				}
			}
		}
	}
}
*/
// Draw the planted bomb and timer
void CEsp::DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass)
{
	// Null it out incase bomb has been dropped or planted
	BombCarrier = nullptr;

	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	CCSBomb* Bomb = (CCSBomb*)pEntity;

	if (Render::WorldToScreen(vOrig, vScreen))
	{
		float flBlow = Bomb->GetC4BlowTime();
		float TimeRemaining = flBlow - (Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase());
		char buffer[64];
		sprintf_s(buffer, "Bomb Timer: %.1f", TimeRemaining);
		///
		if(TimeRemaining >= 0.0f)
		Render::Text(vScreen.x, vScreen.y, Color(208, 215, 25, 255), Render::Fonts::MenuBold, buffer);
	}
}

// Draw the bomb if it's dropped, or store the player who's carrying 
void CEsp::DrawBomb(IClientEntity* pEntity, ClientClass* cClass)
{
	// Null it out incase bomb has been dropped or planted
	BombCarrier = nullptr;
	CBaseCombatWeapon *BombWeapon = (CBaseCombatWeapon *)pEntity;
	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	bool adopted = true;
	HANDLE parent = BombWeapon->GetOwnerHandle();
	if (parent || (vOrig.x == 0 && vOrig.y == 0 && vOrig.z == 0))
	{
		IClientEntity* pParentEnt = (Interfaces::EntList->GetClientEntityFromHandle(parent));
		if (pParentEnt && pParentEnt->IsAlive())
		{
			BombCarrier = pParentEnt;
			adopted = false;
		}
	}

	if (adopted)
	{
		if (Render::WorldToScreen(vOrig, vScreen))
		{
			Render::Text(vScreen.x, vScreen.y, Color(208, 215, 25, 255), Render::Fonts::ESP, "Bomb");
		}
	}
}

void DrawBoneArray(int* boneNumbers, int amount, IClientEntity* pEntity, Color color)
{
	Vector LastBoneScreen;
	for (int i = 0; i < amount; i++)
	{
		Vector Bone = pEntity->GetBonePos(boneNumbers[i]);
		Vector BoneScreen;

		if (Render::WorldToScreen(Bone, BoneScreen))
		{
			if (i>0)
			{
				Render::Line(LastBoneScreen.x, LastBoneScreen.y, BoneScreen.x, BoneScreen.y, color);
			}
		}
		LastBoneScreen = BoneScreen;
	}
}


void CEsp::DrawSkeleton(IClientEntity* pEntity)
{


	studiohdr_t* pStudioHdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);

			if (Render::WorldToScreen(vParent, sParent) && Render::WorldToScreen(vChild, sChild))
			{
				Render::Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(255, 255, 255, 255));
			}
		}
	}
}

void CEsp::DrawGlow()
{
	//int GlowR = Menu::Window.ColorsTab.GlowR.GetValue();
	//int GlowG = Menu::Window.ColorsTab.GlowG.GetValue();
	//int GlowB = Menu::Window.ColorsTab.GlowB.GetValue();
	//int GlowZ = Menu::Window.VisualsTab.GlowZ.GetValue();

	//CGlowObjectManager* GlowObjectManager = (CGlowObjectManager*)GlowManager;

	//for (int i = 0; i < GlowObjectManager->size; ++i)
	//{
	//	CGlowObjectManager::GlowObjectDefinition_t* glowEntity = &GlowObjectManager->m_GlowObjectDefinitions[i];
	//	IClientEntity* Entity = glowEntity->getEntity();

	//	if (glowEntity->IsEmpty() || !Entity)
	//		continue;

	//	switch (Entity->GetClientClass()->m_ClassID)
	//	{

	//	case 1:
	//		if (Menu::Window.VisualsTab.FiltersWeapons.GetState() /*&& Entity->GetClientClass()->m_ClassID == (int)CSGOClassID::CAK47*/)
	//		{
	//			
	//			if (!Menu::Window.VisualsTab.FiltersWeapons.GetState())
	//				break;

	//				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
	//		}

	//	case 29:
	//		if (Menu::Window.VisualsTab.FiltersC4.GetState()/* && Entity->GetClientClass()->m_ClassID == (int)CSGOClassID::CC4*/)
	//		{
	//			if (!Menu::Window.VisualsTab.FiltersC4.GetState())
	//				break;

	//			glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
	//		}

	//	case 35:
	//		if (!Menu::Window.VisualsTab.FiltersPlayers.GetState() && !(Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
	//			break;
	//		if (Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() && (Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
	//			break;

	//			if (GameUtils::IsVisible(hackManager.pLocal(), Entity, 0))
	//			{
	//				glowEntity->set((Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()) ? Color(Menu::Window.ColorsTab.TColorVisR.GetValue(), Menu::Window.ColorsTab.TColorVisG.GetValue(), Menu::Window.ColorsTab.TColorVisB.GetValue(), 255) : Color(Menu::Window.ColorsTab.CTColorVisR.GetValue(), Menu::Window.ColorsTab.CTColorVisG.GetValue(), Menu::Window.ColorsTab.CTColorVisB.GetValue(), 255));
	//			}

	//			else
	//			{
	//				glowEntity->set((Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()) ? Color(Menu::Window.ColorsTab.TColorNoVisR.GetValue(), Menu::Window.ColorsTab.TColorNoVisG.GetValue(), Menu::Window.ColorsTab.TColorNoVisB.GetValue(), 255) : Color(Menu::Window.ColorsTab.CTColorNoVisR.GetValue(), Menu::Window.ColorsTab.CTColorNoVisG.GetValue(), Menu::Window.ColorsTab.CTColorNoVisB.GetValue(), 255));
	//			}
	//		

	//	case 39:
	//		if (Menu::Window.VisualsTab.FiltersC4.GetState() /*&& Entity->GetClientClass()->m_ClassID == (int)CSGOClassID::CPlantedC4*/)
	//		{
	//			if (Menu::Window.VisualsTab.FiltersC4.GetState())
	//				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
	//		}
	//	default:
	//		if (Menu::Window.VisualsTab.FiltersWeapons.GetState())
	//		{
	//			if (Menu::Window.VisualsTab.FiltersWeapons.GetState())
	//				break;

	//			if (strstr(Entity->GetClientClass()->m_pNetworkName, "CWeapon"))
	//				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
	//		}
	//	}
	//}
}

void CEsp::DrawGlowRainbow()
{
	//static float rainbow;
	//rainbow += 0.005f;
	//if (rainbow > 1.f) rainbow = 0.f;
	//int GlowZ = Menu::Window.VisualsTab.GlowZ.GetValue();

	//CGlowObjectManager* GlowObjectManager = (CGlowObjectManager*)GlowManager;

	//for (int i = 0; i < GlowObjectManager->size; ++i)
	//{
	//	CGlowObjectManager::GlowObjectDefinition_t* glowEntity = &GlowObjectManager->m_GlowObjectDefinitions[i];
	//	IClientEntity* Entity = glowEntity->getEntity();

	//	if (glowEntity->IsEmpty() || !Entity)
	//		continue;

	//	switch (Entity->GetClientClass()->m_ClassID)
	//	{

	//	case 1:
	//		if (Menu::Window.VisualsTab.FiltersWeapons.GetState() /*&& Entity->GetClientClass()->m_ClassID == (int)CSGOClassID::CAK47*/)
	//		{
	//			if (Menu::Window.VisualsTab.FiltersWeapons.GetState())
	//				glowEntity->set(Color::FromHSB(rainbow, 1.f, 1.f));
	//		}

	//	case 29:
	//		if (Menu::Window.VisualsTab.FiltersC4.GetState()/* && Entity->GetClientClass()->m_ClassID == (int)CSGOClassID::CC4*/)
	//		{
	//			glowEntity->set(Color::FromHSB(rainbow, 1.f, 1.f));
	//		}

	//	case 35:
	//		if (Menu::Window.VisualsTab.OptionsGlow.GetIndex() == 2 /*&& Entity->GetClientClass()->m_ClassID == (int)CSGOClassID::CCSPlayer*/)
	//		{
	//			if (!Menu::Window.VisualsTab.FiltersPlayers.GetState() && !(Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
	//				break;
	//			if (Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() && (Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
	//				break;

	//			if (GameUtils::IsVisible(hackManager.pLocal(), Entity, 0))
	//			{
	//				glowEntity->set((Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()) ? Color(Menu::Window.ColorsTab.TColorVisR.GetValue(), Menu::Window.ColorsTab.TColorVisG.GetValue(), Menu::Window.ColorsTab.TColorVisB.GetValue(), 255) : Color(Menu::Window.ColorsTab.CTColorVisR.GetValue(), Menu::Window.ColorsTab.CTColorVisG.GetValue(), Menu::Window.ColorsTab.CTColorVisB.GetValue(), 255));
	//			}

	//			else
	//			{
	//				glowEntity->set((Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()) ? Color(Menu::Window.ColorsTab.TColorNoVisR.GetValue(), Menu::Window.ColorsTab.TColorNoVisG.GetValue(), Menu::Window.ColorsTab.TColorNoVisB.GetValue(), 255) : Color(Menu::Window.ColorsTab.CTColorNoVisR.GetValue(), Menu::Window.ColorsTab.CTColorNoVisG.GetValue(), Menu::Window.ColorsTab.CTColorNoVisB.GetValue(), 255));
	//			}
	//		}

	//	case 39:
	//		if (Menu::Window.VisualsTab.FiltersC4.GetState() /*&& Entity->GetClientClass()->m_ClassID == (int)CSGOClassID::CPlantedC4*/)
	//		{
	//			if (Menu::Window.VisualsTab.FiltersC4.GetState())
	//				glowEntity->set(Color::FromHSB(rainbow, 1.f, 1.f));
	//		}

	//	default:
	//		if (Menu::Window.VisualsTab.FiltersWeapons.GetState())
	//		{
	//			if (strstr(Entity->GetClientClass()->m_pNetworkName, "CWeapon"))
	//				glowEntity->set(Color::FromHSB(rainbow, 1.f, 1.f));
	//		}
	//	}
	//}
}
