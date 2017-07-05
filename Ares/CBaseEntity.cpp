// CBaseEntity
#include "Ares.h"

int CBaseEntity::GetFlags() {
	return *(int*)((uintptr_t)this + 0x37C); // returns m_fFlags
}

Vector CBaseEntity::GetVelocity() {
	return *(Vector*)((uintptr_t)this + 0x120); // returns m_vecVelocity
}