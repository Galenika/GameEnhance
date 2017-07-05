#include "Cheats.h"
#include "Accessors.h"
#include <iostream>
#include <algorithm>

cESP* g_pESP = new cESP;

cESP::cESP()
{
	width = 0;
	height = 0;

	espColor = Color(0, 0, 0, 0);
	glowColor = Color(0, 0, 0, 0);
}

template <typename T> T CLAMP(const T& value, const T& low, const T& high)
{
	return value < low ? low : (value > high ? high : value);
}

void cESP::think(CBaseEntity* local)
{
	if (!Options.Visuals.ESP.Enabled)
		return;

	for (auto i = 1; i <= ClientEntList->GetHighestEntityIndex(); i++)
	{
		if (i == local->GetIndex())
			continue;

		auto entity = ClientEntList->GetClientEntity(i);
		if (!entity
			|| entity->GetDormant())
			continue;

		if (entity->GetClientClass()->m_ClassID != 35
			|| !entity->GetAlive()
			|| !Engine->GetPlayerInfo(i, &info)
			|| entity->GetHealth() == 0)
			continue;

		if (Options.Visuals.ESP.EnemyOnly)
			if (entity->GetTeam() == Global::LocalPlayer->GetTeam())
				continue;

		drawPlayer(local, entity);
	}
}

void cESP::getScreenSize()
{
	Engine->GetScreenSize(width, height);
}

bool cESP::isVisible(Vector& start, Vector& end, CBaseEntity* entity)
{
	trace_t trace;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = ClientEntList->GetClientEntity(Engine->GetLocalPlayer());

	ray.Init(start, end);
	EngineTrace->TraceRay(ray, 0x4600400B, &filter, &trace);

	return (trace.m_pEnt == entity || trace.fraction > 0.99f);
}

void cESP::drawPlayer(CBaseEntity* local, CBaseEntity* entity)
{
	auto& trans = *reinterpret_cast<matrix3x4a_t*>(DWORD(entity) + offsets.m_rgflCoordinateFrame);

	auto collision = static_cast<ICollideable*>(entity->GetCollideable());
	if (!collision)
		return;

	auto min = collision->OBBMins();
	auto max = collision->OBBMaxs();

	Vector pointList[] = {
		Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z)
	};

	Vector transformed[8];

	for (auto i = 0; i < 8; i++)
		VectorTransform(pointList[i], trans, transformed[i]);

	Vector flb, brt, blb, frt, frb, brb, blt, flt;

	if (!WorldToScreen(transformed[3], flb)
		|| !WorldToScreen(transformed[0], blb)
		|| !WorldToScreen(transformed[2], frb)
		|| !WorldToScreen(transformed[6], blt)
		|| !WorldToScreen(transformed[5], brt)
		|| !WorldToScreen(transformed[4], frt)
		|| !WorldToScreen(transformed[1], brb)
		|| !WorldToScreen(transformed[7], flt))
		return;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	auto left = flb.x;
	auto top = flb.y;
	auto right = flb.x;
	auto bottom = flb.y;

	for (auto i = 0; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (top < arr[i].y)
			top = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (bottom > arr[i].y)
			bottom = arr[i].y;
	}

	auto x = left;
	auto y = bottom;
	auto w = right - left;
	auto h = top - bottom;
	auto IsVisible = isVisible(local->GetEyePosition(), entity->GetEyePosition(), entity);

	if (IsVisible)
	{
		if (entity->GetTeam() == 2)
			espColor = Color(int(Options.Visuals.ESP.VisibleColor[0]), int(Options.Visuals.ESP.VisibleColor[1]), int(Options.Visuals.ESP.VisibleColor[2]));
		else if (entity->GetTeam() == 3)
			espColor = Color(int(Options.Visuals.ESP.VisibleColor[0]), int(Options.Visuals.ESP.VisibleColor[1]), int(Options.Visuals.ESP.VisibleColor[2]));
	}
	else
	{
		if (entity->GetTeam() == 2)
			espColor = Color(int(Options.Visuals.ESP.NonVisibleColor[0]), int(Options.Visuals.ESP.NonVisibleColor[1]), int(Options.Visuals.ESP.NonVisibleColor[2]));
		else if (entity->GetTeam() == 3)
			espColor = Color(int(Options.Visuals.ESP.NonVisibleColor[0]), int(Options.Visuals.ESP.NonVisibleColor[1]), int(Options.Visuals.ESP.NonVisibleColor[2]));
	}

	if (Options.Visuals.ESP.Box)
	{
		int thirdHeight = floor(h / 3);
		int thirdWidth = floor(w / 3);

		int innerWidth = 1;

		// Top left outline
		DrawRect(x - 1, y - 1, innerWidth + 2, thirdHeight + 2, Color::Black());
		DrawRect(x - 1, y - 1, thirdWidth + 2, innerWidth + 2, Color::Black());

		// top left -> right
		DrawRect(x, y, thirdWidth, innerWidth, Color(45, 184, 239, 255));

		// top left -> down
		DrawRect(x, y, innerWidth, thirdHeight, Color(45, 184, 239, 255));

		// top right outline
		DrawRect((x + w) - thirdWidth - 1, y - 1, thirdWidth + 2, innerWidth + 2, Color::Black());
		DrawRect(x + w - 1, y - 1, innerWidth + 2, thirdHeight + 2, Color::Black());

		// top right -> left
		DrawRect((x + w) - thirdWidth, y, thirdWidth, innerWidth, Color(45, 184, 239, 255));

		// top right -> down
		DrawRect(x + w, y, innerWidth, thirdHeight, Color(45, 184, 239, 255));

		// bottom left outline
		DrawRect(x - 1, (y + h) - thirdHeight - 1, innerWidth + 2, thirdHeight + 2, Color::Black());
		DrawRect(x - 1, y + h - 1, thirdWidth + 2, innerWidth + 2, Color::Black());

		// bottom left -> up
		DrawRect(x, (y + h) - thirdHeight, innerWidth, thirdHeight, Color(45, 184, 239, 255));

		// bottom left -> right
		DrawRect(x, y + h, thirdWidth, innerWidth, Color(45, 184, 239, 255));

		// bottom right -> outline
		DrawRect((x + w) - thirdWidth - 1, (y + h) - 1, thirdWidth + 1 + 2, innerWidth + 2, Color::Black());
		DrawRect((x + w) - 1, (y + h) - thirdHeight - 1, innerWidth + 2, thirdHeight + 2, Color::Black());

		// bottom right -> left
		DrawRect((x + w) - thirdWidth, y + h, thirdWidth + 1, innerWidth, Color(45, 184, 239, 255));

		// bottom right -> up
		DrawRect(x + w, (y + h) - thirdHeight, innerWidth, thirdHeight, Color(45, 184, 239, 255));
	}

	if (Options.Visuals.ESP.Health == 2)
		drawBar(x - 4, y + h / 2 - 1, 4, h + 1, Color(255.0f - entity->GetHealth() * 2.55f, entity->GetHealth() * 2.55f, 0), entity->GetHealth());

	if (Options.Visuals.ESP.Name)
		DrawStringN(ESP, true, x + w / 2, y - 16, Color(255, 255, 255), info.name);

	auto place = 0;


	// Player Info

	if (Options.Visuals.ESP.pInfo)
	{
		float Distance = entity->GetDistance();

		DrawStringN(ESP, false, x + w + 5, y - 4 + (place++ * 11), Color(255, 255, 255), XorStr("Distance: %i"), entity->GetDistance());

		if (entity->IsScoped())
			DrawStringN(ESP, false, x + w + 5, y - 4 + (place++ * 11), Color(255, 255, 255), "*S*");

		if (entity->IsFlashed())
			DrawStringN(ESP, false, x + w + 5, y - 4 + (place++ * 11), Color(255, 255, 255), "*F*");

		if (entity->HasDefuser())
			DrawStringN(ESP, false, x + w + 5, y - 4 + (place++ * 11), Color(255, 255, 255), "D");
		
		if (entity->HasHelmet())
			DrawStringN(ESP, false, x + w + 5, y - 4 + (place++ * 11), Color(255, 255, 255), "H");
	}

	if (Options.Visuals.ESP.Weapon)
	{
		auto wep = entity->GetWeapon();
		if (wep)
		{
			std::string WeaponName = wep->GetWeaponName();
			std::string Ammo;

			if (wep->IsReloading())
				Ammo = "[RELOADING]";
			else
				Ammo = std::to_string(entity->GetAmmo(wep));

			if (Options.Visuals.ESP.Ammo) {
				WeaponName += XorStr("[");
				WeaponName += Ammo;
				WeaponName += XorStr("]");
			}
		}
	}



	if (Options.Visuals.ESP.Health == 1)
		DrawStringN(ESP, false, x + w + 5, y - 4 + (place++ * 11), Color(255, 255, 255), XorStr("%i HP"), entity->GetHealth());

	if (Options.Visuals.ESP.Health == 3) 
	{
		auto m_iHealth = entity->GetHealth();
		if (m_iHealth >= 1)
		{
			auto healthWidth = 5;
			auto healthHeight = m_iHealth * h / 100;
			CLAMP<int>(healthHeight, 1, 100);

			DrawOutlinedRect(x - (healthWidth + 2), y - 1, healthWidth, h + 2, Color::Black());
			DrawRect(x - (healthWidth + 2) + 1, y, healthWidth - 2, healthHeight, Color::Green());
		}
		DrawStringN(ESP, false, x + w + 5, y - 4 + (place++ * 11), Color(255, 255, 255), XorStr("%i HP"), entity->GetHealth());
	}

	if (Options.Visuals.ESP.DrawDefuser && entity->IsDefusing())
		DrawStringN(ESP, false, x + w + 5, y - 4 + (place++ * 11), Color(45, 184, 239,255), XorStr("Defusing"));

	if (Options.Visuals.ESP.DrawGlowS >= 1)
		DrawGlow(entity);
}

void cESP::RunTrace(CBaseEntity* pEnt, Vector& vecStart, Vector& vecForward)
{
	Vector traceEndPos;
	auto pWeapon = pEnt->GetWeapon();
	if (!pWeapon) return;

	auto sWeaponInfo = pWeapon->GetCSWpnData();

	auto vTraceEnd = vecStart + (vecForward * sWeaponInfo->flRange);

	Ray_t ray;
	trace_t tr;
	CTraceFilter filter;
	filter.pSkip = pEnt;

	ray.Init(vecStart, vTraceEnd);
	EngineTrace->TraceRay(ray, 0x4600400B, &filter, &tr);

	traceEndPos = tr.endpos;

	DebugOverlay->AddLineOverlay(tr.startpos, traceEndPos, 0, 255, 0, false, Globals->interval_per_tick);
	DebugOverlay->AddBoxOverlay(traceEndPos - Vector(0, 0, 0), Vector(-2, -2, -2), Vector(2, 2, 2), pEnt->GetEyeAngles(), 0, 255, 0, 50, Globals->interval_per_tick);
}

void cESP::DrawBombDefuser(CBaseEntity * pEnt)
{
}

void cESP::DrawGlow(CBaseEntity* pEnt)
{

	Color glowColor;
	static GlowObjectPointer_t getGlowObjectPointer = reinterpret_cast<GlowObjectPointer_t>(Memory::FindPatternAlt(XorStr("client.dll"), (PBYTE)XorStr("\xA1\x00\x00\x00\x00\xA8\x01\x75\x4E\x33"), XorStr("x????xxxx"), 0, 0));
	static void* glowObjectPointer = getGlowObjectPointer();

	auto glowIndex = *reinterpret_cast<UINT32*>(uintptr_t(pEnt) + 0xA308 + 0x18);
	auto glowObjectArray = *static_cast<CGlowObjectManager::GlowObjectDefinition_t**>(glowObjectPointer);
	auto glowObject = &glowObjectArray[glowIndex];

	switch (Options.Visuals.ESP.DrawGlowS)
	{
	case 1:
		if (pEnt->GetTeam() == 2)
			glowColor = Color(45, 184, 239,255);
		else if (pEnt->GetTeam() == 3)
			glowColor = Color(45, 184, 239,255);
		break;
	case 2:
		if (pEnt->GetTeam() == 2)
			glowColor = Color(int(Options.Visuals.Glow.TeamColor[0]), int(Options.Visuals.Glow.TeamColor[1]), int(Options.Visuals.Glow.TeamColor[2]), 153);
		else if (pEnt->GetTeam() == 3)
			glowColor = Color(int(Options.Visuals.Glow.TeamColor[0]), int(Options.Visuals.Glow.TeamColor[1]), int(Options.Visuals.Glow.TeamColor[2]), 153);
		break;
	default: break;
	}

	glowObject->glowColor = Vector((1.f / 255.f) * glowColor.r(), (1.f / 255.f) * glowColor.g(), (1.f / 255.f) * glowColor.b());
	glowObject->glowAlpha = (1.f / 255.f) * glowColor.a();
	glowObject->shouldGlow = true;
	glowObject->isUnknown = false;
}