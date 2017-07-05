/*
Syn's AyyWare Framework 2015
*/

#pragma once

#include "Hacks.h"

Vector GetAutostrafeView();

class CMiscHacks : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
private:

	/*---------Movement_Hacks---------*/
	void Bunnyhop(CUserCmd *pCmd);
	void LegitStrafe(CUserCmd *pCmd);
	void RageStrafe(CUserCmd *pCmd);
	void Fakelag(CUserCmd *pCmd, bool &bSendPacket);

	/*---------ChatSpams---------*/
	void ChatSpamHowBad();
	void ChatSpamName();
	void ChatSpamDisperseName();
	void SpamRegular();

	/*---------ConVar_Spoofing---------*/
	void SpoofPostProcessing();
	void AmbientExposure();
	void AmbientLighting();
};



