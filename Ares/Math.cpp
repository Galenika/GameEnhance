// Math
#include "Ares.h"

void CMath::ClampMovement(CUserCmd* Cmd) {

	// forwardmove clamping, 450 / -450
	if (Cmd->forwardmove > 450.f) Cmd->forwardmove = 450.f;
	else if (Cmd->forwardmove < -450.f) Cmd->forwardmove = -450.f;

	// sizemove clamping, 450 / -450
	if (Cmd->sidemove > 450.f) Cmd->sidemove = 450.f;
	else if (Cmd->sidemove < -450.f) Cmd->sidemove = -450.f;

	// upmove clamping, 320 / -320
	if (Cmd->upmove > 320.f) Cmd->upmove = 320.f;
	else if (Cmd->upmove < -320.f) Cmd->upmove = -320.f;

}