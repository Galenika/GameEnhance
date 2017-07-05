#pragma once

#include "Hacks.h"

// All misc visuals (Crosshairs etc) go in this class

class CVisuals : public CHack
{
public:
	void Init();
	void Draw();
	void NoScopeBorder();
	void Move(CUserCmd *pCmd, bool& bSendPacket);
private:
	void DrawGlow();
	void DrawHitmarker();
	void DrawCrosshair();
	void DrawRecoilCrosshair();
	void DrawCenterRadar();
};