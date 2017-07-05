#pragma once

#include "Hacks.h"

class CEsp : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool& bSendPacket);
private:

	// Other shit
	IClientEntity *BombCarrier;

	struct ESPBox
	{
		int x, y, w, h;
	};

	// Draw a player
	void DrawPlayer(IClientEntity* pEntity, player_info_t pinfo);

	// Get player info
	Color GetPlayerColor(IClientEntity* pEntity);
	bool GetBox(IClientEntity* pEntity, ESPBox &result);

	// Draw shit about player
	void DrawBox(ESPBox size, Color color);
	void Barrel(CEsp::ESPBox size, Color color, IClientEntity * pEntity);
	
	void DrawBox1(ESPBox size, Color color);
	void DrawBox2(ESPBox size, Color color);
	void DrawBoxAWRainbow(CEsp::ESPBox size, Color color);
	void DrawName(player_info_t pinfo, ESPBox size);
	void DrawHealth(IClientEntity* pEntity, ESPBox size);
	void DrawHealth1(IClientEntity* pEntity, ESPBox size);
	void DrawInfo(IClientEntity* pEntity, ESPBox size);
	void DrawCross(IClientEntity* pEntity);
	void DrawSkeleton(IClientEntity* pEntity);
	void DrawSkeletonRainbow(IClientEntity* pEntity);
	void DrawNameRainbow(player_info_t pinfo, ESPBox size);
	void DrawChicken(IClientEntity* pEntity, ClientClass* cClass);
	void DrawMolotov(IClientEntity* pEntity, ClientClass* cClass);
	void DrawInc(IClientEntity* pEntity, ClientClass* cClass);
	void DrawDecoy(IClientEntity* pEntity, ClientClass* cClass);
	void DrawFlash(IClientEntity* pEntity, ClientClass* cClass);
	void DrawHE(IClientEntity* pEntity, ClientClass* cClass);
	void DrawSmoke(IClientEntity* pEntity, ClientClass* cClass);
	void DrawDrop(IClientEntity* pEntity, ClientClass* cClass);
	void DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass);
	void DrawBomb(IClientEntity* pEntity, ClientClass* cClass);
	void DrawGlow();
	void DrawBoxRainbow(ESPBox size, Color color);
	  void DrawGlowRainbow();
	  void DrawGrenades(IClientEntity* pEntity);
		
};

