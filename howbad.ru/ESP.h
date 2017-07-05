/*
Syn's AyyWare Framework 2015
*/

#pragma once

#include "Hacks.h"

class CEsp : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
private:

	IClientEntity *BombCarrier;
	struct ESPBox
	{
		int x, y, w, h;
	};

	/*Drawing*/
	void DrawPlayer(IClientEntity* pEntity, player_info_t pinfo);

	/*Get-Info*/
	Color GetPlayerColor(IClientEntity* pEntity);
	bool GetBox(IClientEntity* pEntity, ESPBox &result);

	/*Draw-Hacks*/
	void SpecList();
	void DrawBox(ESPBox size, Color color);
	void DrawCornerBox(ESPBox size, Color color);
	void DrawName(player_info_t pinfo, ESPBox size);
	void DrawHealth(IClientEntity* pEntity, ESPBox size);
	void DrawArmor(IClientEntity* pEntity, CEsp::ESPBox size);
	void DrawInfo(IClientEntity* pEntity, ESPBox size);
	void DrawSkeleton(IClientEntity* pEntity);

	void DrawChicken(IClientEntity* pEntity, ClientClass* cClass);
	void DrawDrop(IClientEntity* pEntity, ClientClass* cClass);
	void DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass);
	void DrawBomb(IClientEntity* pEntity, ClientClass* cClass);
};

