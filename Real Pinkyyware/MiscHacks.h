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
	void AutoJump(CUserCmd *pCmd);
	void AutoJumpSMAC(CUserCmd *pCmd);
	void EdgeJump(CUserCmd *pCmd);
	void PerfectStrafe(CUserCmd *pCmd);
	void LegitStrafe(CUserCmd *pCmd);
	void RageStrafe(CUserCmd *pCmd);
	void ChangeName(const char* name);
	void ChatSpamRegular();
	void ChatSpamEZ();
	void ChatSpamRacist();
	void ChatSpamInterplebz();
	void ChatSpamAyimware();
	void ChatSpamReports();
	void ChatSpamInterplebs();
	void SetNameXenoHook();
	void SetNamePinkyWare();
	void SetNameCheatBetter();
	void SetNameMemeWare();
	void SetNameSkeet();
	void ChatSpamRapeGang();
	void ChatSpamCallout();
	void SetNameInterwebz();
	void NameStealer();
	void CCStrafe();
	void AirDuck();
	void ClampAngles();
	//void ChatSpamAchievement();
	//void NameDispersion();
	//void ChatSpamAchievement();
	void Fakelag(CUserCmd *pCmd, bool &bSendPacket);

	//std::string GetNameChange();

	/*void ChatSpam();
	void Namechanger();

	std::string GetRegularSpam();
	std::string GetCustomSpam();
	std::string GetPlayerSpam();
	std::string GetSinglePlayerSpam();

	std::string GetNameChange();
	std::string GetNameCalloutChange();*/
};
