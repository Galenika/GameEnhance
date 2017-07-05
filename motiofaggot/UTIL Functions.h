#pragma once

#include "Vector.h"
#include "MathFunctions.h"
#include "checksum_md5.h"
#include "Entities.h"
#include "offsets.h"
#include "ClientRecvProps.h"
#include "BaseClient.h"
#include "EngineClient.h"
#include "MiscClasses.h"
#include "Surface.h"
#include "Materials.h"

namespace GameUtils
{
	void NormaliseViewAngle(Vector &angle);
	void ClampViewAngle(Vector &angle);
	void CL_FixMove(CUserCmd* cmd, Vector viewangles);
	bool IsVisible(IClientEntity* pLocal, IClientEntity* pEntity, int BoneID);
	bool IsKnife(void* weapon);
	bool IsBallisticWeapon(void* weapon);
	bool IsPistol(void* weapon);
	bool IsSniper(void* weapon);
	bool IsMachinegun(void* weapon);
	bool IsShotgun(void* weapon);
	bool IsMP(void* weapon);
	int GetPlayerCompRank(IClientEntity* pEntity);
};

// Trace Line Memes
void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask,
	const IClientEntity *ignore, int collisionGroup, trace_t *ptr);

void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr);

bool IsBreakableEntity(IClientEntity* ent);

bool TraceToExit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace);

void SayInChat(const char *text);

Vector GetHitboxPosition(IClientEntity* pEntity, int Hitbox);

void ForceUpdate();