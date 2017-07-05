// CHLClient
#include "Ares.h"

ClientClass* CHLClient::GetAllClasses() {
	typedef ClientClass*(__thiscall* OriginalFn)(void*);
	return GetVFunc<OriginalFn>(this, 8)(this);
}