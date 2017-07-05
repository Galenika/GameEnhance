// IPanel
#include "Ares.h"

const char* IPanel::GetName(unsigned int Panel) {
	typedef const char* (__thiscall* OriginalFn)(PVOID, unsigned int);
	return GetVFunc<OriginalFn>(this, 36)(this, Panel);
}