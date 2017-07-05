#include "Hacks.h"
#include "Autowall.h"
class Rage2Bot : public CHack
{
public:
	
	void doAim(CUserCmd * pCmd, bool aim_active, bool aim_silent, bool aim_autoshoote, bool aim_psilent);
};