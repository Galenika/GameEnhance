#pragma once

#include "Hacks.h"

Vector GetAutostrafeView();

class CMiscHacks : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool& bSendPacket);
	void NoFlash();
	void NameChange(CUserCmd * pcmd);
	void fovchanger();
	void NameSteal();
	void SetClanTag(const char * tag, const char * name);
private:
	void AutoJump(CUserCmd *pCmd);
	void AutoStrafe(CUserCmd *pCmd);
	void RemoveScope();
	void ChatSpamRegular();
	void ChatSpamReports();
	void ChatSpam1();
	void ChatSpam2();
	void RageStrafe(CUserCmd * pCmd);
	void CircleStrafer(CUserCmd * cmd, IClientEntity * localPly);
	void RotateMovement(CUserCmd * pCmd, float rotation);
	bool doCircularStrafe(CUserCmd * pCmd, Vector & OriginalView);
	void CheatsBypass(CUserCmd * cmd);
	void NoFlash(CUserCmd * pCmd);
	void RageAutoStrafe(CUserCmd * pCmd);
	void ChatSpam3();
	void TeleportHack(CUserCmd *pCmd);
	
};