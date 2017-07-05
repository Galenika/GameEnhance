#pragma once
#include "SDK.h"
#include "Globals.h"
#include "Globals.h"

struct FireBulletData
{
	Vector src;
	trace_t enter_trace;
	Vector direction;
	CTraceFilter filter;
	float trace_length;
	float trace_length_remaining;
	float current_damage;
	int penetrate_count;
};

class class_autowall
{
public:
	class_autowall();
	bool AutoWall(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, Vector& point);
	float GetDamage(Vector& point);
	float GetHitgroupDamage(int iHitgroup);
	bool TraceToExit(Vector& end, trace_t& tr, float x, float y, float z, float x2, float y2, float z2, trace_t* trace);
	void ScaleDamage(int Hitgroup, CBaseEntity* pEntity, float weapon_armor_ratio, float& current_damage);
	static bool DidHitNonWorldEntity(CBaseEntity* m_pEnt);
	//bool TraceToExit(Vector& end, trace_t* enter_trace, Vector start, Vector dir, trace_t* exit_trace);

	bool SimulateFireBullet(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, FireBulletData& data);
	bool HandleBulletPenetration(WeaponInfo_t* wpn_data, FireBulletData& data);
	bool IsFuckingAutowalling;
	float bIsPointPenetrable(Vector vEnd);
private:
	CTraceFilterNoPlayer tfNoPlayers;
};

extern class_autowall g_pAutowall;

struct Hitbox
{
	Hitbox(void)
	{
		hitbox = -1;
	}

	Hitbox(int newHitBox)
	{
		hitbox = newHitBox;
	}

	int  hitbox;
	Vector points[9];
};

enum
{
	FL_HIGH = (1 << 0),
	FL_LOW = (1 << 1),
	FL_SPECIAL = (1 << 2)
};

struct BestPoint
{
	BestPoint(void)
	{
		hitbox = -1;
		point = Vector(0, 0, 0);
		index = -1;
		dmg = -1;
		flags = 0;
	}

	explicit BestPoint(int newHitBox)
	{
		hitbox = newHitBox;
		point = Vector(0, 0, 0);
		index = -1;
		dmg = -1;
		flags = 0;
	}

	Vector point;
	int  index;
	int  dmg;
	int  flags;
	int  hitbox;
};

class CFixMove
{
public:
	void Start();
	void End();
private:
	float m_oldforward, m_oldsidemove;
	QAngle m_oldangle;
};// stolen from a5

class cRage
{
public:
	void Invoke(CUserCmd*,bool&,bool);
	bool GetHitbox(CBaseEntity*, Hitbox*);
	bool GetBestPoint(CBaseEntity*, Hitbox*, BestPoint*);
	bool CustomMultipoint(CBaseEntity*, Hitbox*, BestPoint*);
	int HitScan(CBaseEntity * pEntity);
	bool BestAimPointAll(CBaseEntity*, Vector&);
	bool BestAimPointAll2(CBaseEntity*, Vector&);
	bool BestAimPoint(CBaseEntity* target, Vector& hitboxs);
	bool BestAimPointHitbox(CBaseEntity*, Vector&);
	bool IsValidEntity(int);
	float AimNearPlayer(CBaseEntity*);
	int TargetSelection(CUserCmd*);
	void AntiAim(CUserCmd*, bool&);
	bool TargetMeetsRequirements(CBaseEntity * pEntity);
	float FovToPlayer(Vector ViewOffSet, Vector View, CBaseEntity * pEntity, int aHitBox);
	void Aimbot(CUserCmd*);
	void EnginePred(CUserCmd*);
	int target;
	bool IsAimbotting, IsStopped;
};

extern cRage* g_pRage;

class cPlayer
{
public:
	void Bhop(CUserCmd*);
	void ForceThirdPerson(CUserCmd* pCmd);
	void AirStuck(CUserCmd*);
	void SpoofConvars();
	void FakeLag(CUserCmd*, bool&);
	void ChatSpamRegular();
	void ClanTagChanger();
	void NameChanger();
	void NoName();
	void IzumiSlide();
	void IzumiBuild();
	void erectTag();
	void RageAutoStrafe(CUserCmd*);
};
extern cPlayer* g_pPlayer;

class cESP
{
	struct plist_t
	{
		char* name;
		char* steamid;
		int   money;
		int   health;
		int   mmwins;
		char* mmrank;
	}players[64];

public:
	cESP();
	void think(CBaseEntity* local);
	void getScreenSize();
	bool isVisible(Vector& start, Vector& end, CBaseEntity* entity);
private:
	void drawPlayer(CBaseEntity* local, CBaseEntity* entity);
	void RunTrace(CBaseEntity* pEnt, Vector& vecStart, Vector& vecForward);
	void DrawBombDefuser(CBaseEntity* pEnt);
	void DrawGlow(CBaseEntity* pEnt);
public:
	int width;
	int height;

private:
	Color espColor;
	Color glowColor;

private:
	player_info_t info;
};

extern cESP* g_pESP;