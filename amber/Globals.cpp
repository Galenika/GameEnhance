#include "Globals.h"

int	G::selectedPlayer;
Vector G::qLastTickAngle;
Vector G::LastAngle;
bool G::initializedPlayerList;
CUserCmd* G::UserCmd;
float G::fov;

float G::RealAngles;
float G::FakeAngles;

bool G::LBYUpdate;
float G::LastLBYUpdateTime;
float G::NextLBYUpdate;
bool G::bSwitch;

float G::GetOutgoingLatency()
{
	//owned
}