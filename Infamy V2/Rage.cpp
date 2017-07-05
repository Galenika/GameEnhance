#include "Cheats.h"

//Eevee~~ <3

cRage* g_pRage = new cRage;

void cRage::Invoke(CUserCmd* pCmd, bool& bSendThisPacket, bool bCanFire)
{
	CFixMove fixmove;
	fixmove.Start();

	if (Options.Rage.AntiAim.Enabled && !(pCmd->buttons & IN_ATTACK))
		this->AntiAim(pCmd, bSendThisPacket);
	if (Options.Rage.Aimbot.Enabled)
	{
		this->EnginePred(pCmd);

		if (bCanFire)
			this->Aimbot(pCmd);
	}
	fixmove.End();
}      

bool cRage::GetHitbox(CBaseEntity* target, Hitbox* hitbox)
{
	matrix3x4a_t matrix[MAXSTUDIOBONES];

	if (!target->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
		return false;

	studiohdr_t *hdr = ModelInfo->GetStudioModel(target->GetModel());

	if (!hdr) {

		return false;
	}
	mstudiohitboxset_t *hitboxSet = hdr->pHitboxSet(target->GetHitboxSet());
	mstudiobbox_t *untransformedBox = hitboxSet->pHitbox(hitbox->hitbox);

	Vector bbmin = untransformedBox->bbmin;
	Vector bbmax = untransformedBox->bbmax;
	if (untransformedBox->radius != -1.f)
	{
		bbmin -= Vector(untransformedBox->radius* 1, untransformedBox->radius* 1, untransformedBox->radius* 1);
		bbmax += Vector(untransformedBox->radius* 1, untransformedBox->radius* 1, untransformedBox->radius* 1);
	}

	Vector points[] = { ((bbmin + bbmax) * .5f),
		Vector(bbmin.x, bbmin.y, bbmin.z),
		Vector(bbmin.x, bbmax.y, bbmin.z),
		Vector(bbmax.x, bbmax.y, bbmin.z),
		Vector(bbmax.x, bbmin.y, bbmin.z),
		Vector(bbmax.x, bbmax.y, bbmax.z),
		Vector(bbmin.x, bbmax.y, bbmax.z),
		Vector(bbmin.x, bbmin.y, bbmax.z),
		Vector(bbmax.x, bbmin.y, bbmax.z) };

	for (int index = 0; index < 8; index++)
	{
		VectorTransform(points[index], matrix[untransformedBox->bone], hitbox->points[index]);
	}
	return true;
}


bool cRage::GetBestPoint(CBaseEntity* target, Hitbox* hitbox, BestPoint* point)
{
	for (int index = 0; index <= 8; ++index)
	{
		int temp_damage = g_pAutowall.GetDamage(hitbox->points[index]);
		if ((point->dmg < temp_damage))
		{
			point->dmg = temp_damage;
			point->point = hitbox->points[index];
			point->index = index;
		}
	}
	return (point->dmg > Options.Rage.AutoWall.MinDamage);
}

bool cRage::CustomMultipoint(CBaseEntity* target, Hitbox* hitbox, BestPoint* point)
{
	auto center = hitbox->points[0];

	if (hitbox->hitbox == HITBOX_HEAD)
	{
		auto high = ((hitbox->points[3] + hitbox->points[5]) * .5f);

		auto pitch = target->GetEyeAngles().x;
		if ((pitch > 0.f) && (pitch < 88.9f))
		{
			auto height = (((high - hitbox->points[0]) / 3) * 4);
			auto new_height = (hitbox->points[0] + (height * (pitch / 89.f)));

			hitbox->points[0] = new_height;
			point->flags |= FL_HIGH;
		}
		else if ((pitch < 292.5f) && (pitch > 271.f))
		{
			hitbox->points[0] -= Vector(0.f, 0.f, 1.f);
			point->flags |= FL_LOW;
		}
	}
	if (Options.Rage.Aimbot.Hitscan.Point > 0)
	{


		for (auto index = 0; index < Options.Rage.Aimbot.Hitscan.Point; index++)
		{
			int temp_damage = g_pAutowall.bIsPointPenetrable(hitbox->points[index]);

			if ((point->dmg < temp_damage))
			{
				point->dmg = temp_damage;
				point->point = hitbox->points[index];
				point->index = index;
			}
		}
	}
	else
	{
		point->dmg = g_pAutowall.bIsPointPenetrable(hitbox->points[0]);
		point->point = hitbox->points[0];
		point->index = 0;
	}
	return (point->dmg >= Options.Rage.AutoWall.MinDamage);
}

int hitboxes[8] =
{
	HITBOX_PELVIS,
	HITBOX_CHEST,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_HEAD
};

int Allhitboxs[20] =
{
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_LOWER_NECK,
	HITBOX_PELVIS,
	HITBOX_BODY,
	HITBOX_THORAX,
	HITBOX_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_HAND,
	HITBOX_LEFT_HAND,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM,
};

int hitboxes2[17] =
{
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_LOWER_NECK,
	HITBOX_UPPER_CHEST,
	HITBOX_CHEST,
	HITBOX_BODY,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_LEFT_FOREARM,
	HITBOX_PELVIS,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_FOOT,
	HITBOX_RIGHT_FOOT,
};

enum class CSGOHitboxID
{
	Head = 0,
	Neck,
	NeckLower,
	Pelvis,
	Stomach,
	LowerChest,
	Chest,
	UpperChest,
	RightThigh,
	LeftThigh,
	RightShin,
	LeftShin,
	RightFoot,
	LeftFoot,
	RightHand,
	LeftHand,
	RightUpperArm,
	RightLowerArm,
	LeftUpperArm,
	LeftLowerArm,
	Max,
};


int cRage::HitScan(CBaseEntity* pEntity)
{
	CBaseEntity* pLocal = pEntity;
	std::vector<int> HitBoxesToScan;

	// Get the hitboxes to scan
#pragma region GetHitboxesToScan
	int HitScanMode = Options.Rage.Aimbot.Multipoint;

		if (HitScanMode == 0)
		{
			// No Hitscan, just a single hitbox
			switch (Options.Rage.Aimbot.Multipoint)
			{
			case 0:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				break;
			case 1:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				break;
			case 2:
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				break;
			case 3:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				break;
			case 4:
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
				break;
			}
		}
		else
		{
			switch (HitScanMode)
			{
			case 1:
				// Low
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				break;
			case 2:
				// Normal
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
				break;
			case 3:
				// High
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
				break;
			case 4:
				// Extreme
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
				break;
			}
		}
#pragma endregion Get the list of shit to scan
}

bool cRage::BestAimPointAll(CBaseEntity* target, Vector& hitboxs)
{
	auto highest_damage = 0;
	for (auto i = 0; i < 20; i++)
	{
		BestPoint aim_point;
			if (i == HITBOX_HEAD)
				continue;

		Hitbox hitbox(Allhitboxs[i]);
		GetHitbox(target, &hitbox);

		if (!GetBestPoint(target, &hitbox, &aim_point)) {
			continue;
		}

		if (aim_point.dmg > highest_damage) {
			highest_damage = aim_point.dmg;
			hitboxs = aim_point.point;
		}
	}
	return(highest_damage > Options.Rage.AutoWall.MinDamage);

}

bool cRage::BestAimPointAll2(CBaseEntity* target, Vector& hitboxs)
{
	auto highest_damage = 0;
	for (auto i = 0; i < 17; i++)
	{
		BestPoint aim_point;
		if (i == HITBOX_HEAD)
			continue;
		Hitbox hitbox(hitboxes2[i]);
		GetHitbox(target, &hitbox);

		if (!GetBestPoint(target, &hitbox, &aim_point)) {
			continue;
		}

		if (aim_point.dmg > highest_damage) {
			highest_damage = aim_point.dmg;
			hitboxs = aim_point.point;
		}
	}
	return(highest_damage > Options.Rage.AutoWall.MinDamage);

}

bool cRage::BestAimPoint(CBaseEntity* target, Vector& hitboxs)
{
	BestPoint aim_point;

	for (auto i = 0; i < 20; i++)
	{
		if (i == HITBOX_HEAD)
			continue;

		Hitbox hitbox(Allhitboxs[i]);
		GetHitbox(target, &hitbox);

		if (!GetBestPoint(target, &hitbox, &aim_point))
			continue;
	}

	if (aim_point.dmg > Options.Rage.AutoWall.MinDamage)
	{
		hitboxs = aim_point.point;
		return true;
	}


	return false;
}

bool cRage::BestAimPointHitbox(CBaseEntity* target, Vector& hitbox)
{
	BestPoint aim_point;

	auto hitboxnum = 0;

	switch (Options.Rage.Aimbot.Hitbox)
	{
	case 0:
		hitboxnum = 6;
		break;
	case 1:
		hitboxnum = 5;
		break;
	case 2:
		hitboxnum = 4;
		break;
	case 3:
		hitboxnum = 3;
		break;
	case 4:
		hitboxnum = 2;
		break;
	case 5:
		hitboxnum = 1;
		break;
	case 6:
		hitboxnum = 0;
		break;
	default:
		hitboxnum = 0;
		break;
	}
		if (hitboxnum == HITBOX_HEAD)
			hitboxnum == HITBOX_NECK;
	Hitbox hitboxx(hitboxnum);
	GetHitbox(target, &hitboxx);

	if (!GetBestPoint(target, &hitboxx, &aim_point))
		return false;

	if (aim_point.dmg > Options.Rage.AutoWall.MinDamage)
	{
		hitbox = aim_point.point;
		return true;
	}

	return false;
}

bool cRage::IsValidEntity(int iEnt)
{
	CBaseEntity* pEnt = nullptr; player_info_t pTemp;
		
	if ((pEnt = ClientEntList->GetClientEntity(iEnt)))
		if (Engine->GetPlayerInfo(iEnt,&pTemp))
			if (!(pEnt == Global::LocalPlayer))
			{
				auto vecTemp = pEnt->GetBonePosition(Options.Rage.Aimbot.Hitbox);
				if (!(Options.Rage.Aimbot.EnemyOnly)
					|| pEnt->GetTeam() != Global::LocalPlayer->GetTeam())
					if (!pEnt->GetDormant())
						if (pEnt->GetHealth() > 0)
							if (!pEnt->GetImmune())
								if (Options.Rage.Aimbot.HitScans == 0 &&
									Options.Rage.AutoWall.MinDamage <=
									g_pAutowall.bIsPointPenetrable(vecTemp)
									|| Options.Rage.Aimbot.HitScans >= 1)
									return true;
			}
	return false;
}

enum eTargeting
{
	TARGETBY_FOV,
	TARGETBY_DISTANCE,
	TARGETBY_THREAT
};

float cRage::AimNearPlayer(CBaseEntity* pEnt)
{
	auto src = pEnt->GetEyePosition();
	auto dst = Global::LocalPlayer->GetEyePosition();
	auto forward = src - dst;
	QAngle Product;
	VectorAngles(forward, Product);
	Product.Normalize();
	static auto flMaxDif = 180.f;
	auto fldif = Product.y;
	auto flPercent = 1.f - abs(fldif / flMaxDif);
	return flPercent;
}


int cRage::TargetSelection(CUserCmd* pCmd)
{
	auto flFOV = Options.Rage.Aimbot.FOV;
	auto flDistance = 8192.f;
	auto flThreatLevel = 0.f;
	auto iBestTarget = -1;

	for(auto i = 0; i <= ClientEntList->GetHighestEntityIndex();i++)
	{
		if (!this->IsValidEntity(i))
			continue;
		auto pEnt = ClientEntList->GetClientEntity(i);
		auto vecHitbox = pEnt->GetBonePosition(Options.Rage.Aimbot.Hitbox);

		if ( Options.Rage.Aimbot.Targeting ==  0) //FOV
		{
			QAngle TempTargetAbs;
			CalcAngle(Global::LocalPlayer->GetEyePosition(), vecHitbox, TempTargetAbs);
			auto flTempFOV = GetFov(pCmd->viewangles, TempTargetAbs);
			if(flTempFOV < flFOV)
			{
				Vector TempHitbox;
				if (Options.Rage.Aimbot.HitScans == 0
					|| Options.Rage.Aimbot.HitScans == 1 && BestAimPoint(pEnt, TempHitbox)
					|| Options.Rage.Aimbot.HitScans == 2 && BestAimPointHitbox(pEnt, TempHitbox)
					|| Options.Rage.Aimbot.Multipoint == 1 && BestAimPointHitbox(pEnt, TempHitbox)
					|| Options.Rage.Aimbot.HitScans == 3 && BestAimPointAll2(pEnt, TempHitbox))
				{

					flFOV = flTempFOV;
					iBestTarget = i;
				}
			}
		}

		if (Options.Rage.Aimbot.Targeting == 1) //Distance
		{
			QAngle TempTargetAbs;
			CalcAngle(Global::LocalPlayer->GetEyePosition(), vecHitbox, TempTargetAbs);
			auto flTempFOVs = GetFov(pCmd->viewangles, TempTargetAbs);
			auto flTempDistance = VectorDistance(Global::LocalPlayer->GetOrigin(), pEnt->GetOrigin());
			if (flTempDistance < flDistance && flTempFOVs < flFOV)
			{
				Vector TempHitbox;
				if (Options.Rage.Aimbot.HitScans == 0
					|| Options.Rage.Aimbot.HitScans == 1 && BestAimPoint(pEnt, TempHitbox)
					|| Options.Rage.Aimbot.HitScans == 2 && BestAimPointHitbox(pEnt, TempHitbox)
					|| Options.Rage.Aimbot.Multipoint == 1 && BestAimPointHitbox(pEnt, TempHitbox)
					|| Options.Rage.Aimbot.HitScans == 3 && BestAimPointAll2(pEnt, TempHitbox))
				{
					flFOV = flTempFOVs;
					flDistance = flTempDistance;
					iBestTarget = i;
				}
			}
		}

		if ( Options.Rage.Aimbot.Targeting ==  2) //Threat
		{
			auto flTempdistance = VectorDistance(Global::LocalPlayer->GetOrigin(), pEnt->GetOrigin());
			auto flTempHealths = pEnt->GetHealth();
			auto flTempThreat = flTempHealths / flTempdistance;
			flTempThreat += static_cast<int>(this->AimNearPlayer(pEnt));
			if (pEnt->IsTargetingLocal())
				flTempThreat += 100;
			if(flTempThreat > flThreatLevel)
			{
				Vector TempHitbox;
				if (Options.Rage.Aimbot.HitScans == 0
					|| Options.Rage.Aimbot.HitScans == 1 && BestAimPoint(pEnt, TempHitbox)
					|| Options.Rage.Aimbot.HitScans == 2 && BestAimPointHitbox(pEnt, TempHitbox)
					|| Options.Rage.Aimbot.Multipoint == 1 && BestAimPointHitbox(pEnt, TempHitbox)
					|| Options.Rage.Aimbot.HitScans == 3 && BestAimPointAll2(pEnt, TempHitbox))
				{
					flThreatLevel = flTempThreat;
					iBestTarget = i;
				}
			}
		}
	}
	return iBestTarget;
}

void cRage::EnginePred(CUserCmd* pCmd)
{
	auto MoveData = *reinterpret_cast<void**>(reinterpret_cast<DWORD>(g_pMovement) + 0x8);
	if (!g_pMoveHelper)return;
	if (!Engine->IsInGame() && !Engine->IsConnected()) return;
	if (!MoveData) return;
	//Set Your Old Frametime
	auto oldFrame = Globals->frametime;
	Globals->frametime = Globals->interval_per_tick;
	//Set Host to local Player
	g_pMoveHelper->SetHost(Global::LocalPlayer);
	Prediction->SetupMove(Global::LocalPlayer, pCmd, nullptr,MoveData);
	g_pMovement->ProcessMovement(Global::LocalPlayer, MoveData);
	Prediction->FinishMove(Global::LocalPlayer, pCmd, MoveData);
	//restore original
	g_pMoveHelper->SetHost(nullptr);
	Globals->frametime = oldFrame;
}

bool bullettime() {
	if (!Global::LocalPlayer) return false;
	float flServerTime = (float)Global::LocalPlayer->GetTickBase() * Globals->interval_per_tick;
	float flNextPrimaryAttack = Global::LocalPlayer->GetWeapon()->GetNextPrimaryAttack();

	bool BulletTime = true;

	if (flNextPrimaryAttack > flServerTime) BulletTime = false;

	return BulletTime;
}

enum AAPitch
{
	PITCH_NONE,
	PITCH_EMOTION,
	PITCH_DOWN,
	PITCH_FAKEDOWN,
	PITCH_UP,
	PITCH_FAKEUP,
	PITCH_NORMAL,
};

enum AAYAW
{
	YAW_NONE,
	YAW_FBACKWARD,
	YAW_FSIDEWAYS,
	YAW_FSTATIC,
	YAW_FSTATIC2,
	YAW_FSTATIC3,
	YAW_TWTICH,
	YAW_FAKE1,
	YAW_FAKE2,
	YAW_BACKWARD
};

void cRage::AntiAim(CUserCmd* pCmd, bool& bSendThisPacket)
{
	auto pWeapon = Global::LocalPlayer->GetWeapon();
	static bool flip = false;
	if (pCmd->buttons & IN_USE) return;
	if (pWeapon->GetWeaponType() == WEPCLASS_INVALID) return;
	if (Global::LocalPlayer->GetMoveType() == MOVETYPE_LADDER) return;
	
	switch (Options.Rage.AntiAim.Pitch)
	{
	case PITCH_EMOTION:
		pCmd->viewangles.x = 89.000000f;
		break;
	case PITCH_DOWN:
		pCmd->viewangles.x = 179.000000f;
		break;
	case PITCH_FAKEDOWN:
	{
		if (flip)
		{
			pCmd->viewangles.x = -179.000000f;
			bSendThisPacket = false;
		}
		else
		{
			pCmd->viewangles.x = 179.000000f;
			bSendThisPacket = true;
		}
	}
	case PITCH_UP:
		pCmd->viewangles.x = -179.000000f;
		break;
	case PITCH_FAKEUP:
	{
		if (flip)
		{
			pCmd->viewangles.x = -271.000000f;
			bSendThisPacket = false;
		}
		else
		{
			pCmd->viewangles.x = 271.000000f;
			bSendThisPacket = true;
		}
	}
		break;
	case PITCH_NORMAL:
		pCmd->viewangles.x = 180.000000f;
		break;
	}
	int spinvalue = rand() % 100;
	int lispYaw = 0;
	int lispPitch = 0;
	int spinSpeed = Options.Rage.AntiAim.spinSpeed;
	static bool nigger = false;
	static bool flip2 = false;
	static auto jitter = -1;
	static auto fStatic = -1;
	static auto pJitter = 0;
	static auto pJitter2 = 0;
	static auto jitterangle = 0;
	float AACurAdjust = (Globals->curtime * 5000.0);
	float AAAdjustment = (Globals->curtime * 5000.0);
	float cCurTime = Globals->curtime * 1000.0;
	float cCurTime1 = Globals->curtime * 5000.0;
	float temp;
	float cCurTime2 = Globals->curtime * 5000.0;
	auto OriginalAA = pCmd->viewangles;
	static auto iChoked = -1;
	static auto jit = -1;
	switch (Options.Rage.AntiAim.Yaw)
	{
		if (flip)
		{
			pCmd->viewangles.y += 180.000000f;
			bSendThisPacket = false;
		}
		else
		{
			pCmd->viewangles.y -= 0.000000f;
			bSendThisPacket = true;
		}
		break;
	case YAW_FSIDEWAYS:
	{
		if (flip)
		{
			pCmd->viewangles.y += 90.000000f;
			bSendThisPacket = false;
		}
		else
		{
			pCmd->viewangles.y -= 90.000000f;
			bSendThisPacket = true;
		}
	}
		break;
	case YAW_FSTATIC:
		flip + !flip;
		flip2 = !flip2;
		{
			if (flip)
			{
				bSendThisPacket = false;

				if (flip2)
					pCmd->viewangles.y += 90.000000f;

				else
					pCmd->viewangles.y -= 90.000000f;
			}
			else
			{
				bSendThisPacket = true;
				pCmd->viewangles.y += 180.000000f;
			}
		}
		break;
	case YAW_FSTATIC2:
		flip + !flip;
		flip2 = !flip2;
		{
			if (flip)
			{
				bSendThisPacket = false;

				if (flip2)
				{
					pCmd->viewangles.y += 90.000000f;
				}
				else
					pCmd->viewangles.y -= 90.000000f;
			}
			else
			{
				bSendThisPacket = true;
				if (flip2)
				pCmd->viewangles.y -= 162.000000f;
				else if (!flip2)
				pCmd->viewangles.y -= 180.000000f;


			}
		}
		break;
	case YAW_FSTATIC3:
		flip + !flip;
		flip2 = !flip2;
		{
			if (flip)
			{
				bSendThisPacket = false;

				if (flip2)
				{
					pCmd->viewangles.y += 90.000000f;
				}
				else
					pCmd->viewangles.y -= 90.000000f;
			}
			else
			{
				bSendThisPacket = true;
				if (flip2)
					pCmd->viewangles.y += 90.000000f;
				else if (!flip2)
					pCmd->viewangles.y -= 180.000000f;


			}
		}
		break;
	case YAW_FAKE1:
		if (pJitter <= 1)
		{
			pCmd->viewangles.x = 88.000000;
			pCmd->viewangles.y += 135.000000;
		}
		else if (pJitter > 1 && pJitter <= 3)
		{
			pCmd->viewangles.x = 88.000000;
			pCmd->viewangles.y += 225.000000;
		}
		if (pCmd->tick_count % 3)
		{
			bSendThisPacket = false;
			if (pJitter <= 1)
			{
				pCmd->viewangles.y += 45.000000;
				pJitter += 1;
			}
			else if (pJitter > 1 && pJitter <= 3)
			{
				pCmd->viewangles.y -= 45.000000;
				pJitter += 1;
			}
			else
			{
				pJitter = 0;
			}
		}
		else
		{
			bSendThisPacket = true;
		}
		break;
	case YAW_FAKE2:
		if (jitterangle <= 1)
		{
			pCmd->viewangles.x = 88.000000;
			pCmd->viewangles.y += 90.000000;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			pCmd->viewangles.x = 88.000000;
			pCmd->viewangles.y -= 90.000000;
		}
		if (pCmd->tick_count % 3)
		{
			bSendThisPacket = false;
			if (jitterangle <= 1)
			{
				pCmd->viewangles.y += 179.000000;
				jitterangle += 1;
			}
			else if (jitterangle > 1 && jitterangle <= 3)
			{
				pCmd->viewangles.y -= 179.000000;
				jitterangle += 1;
			}
			else
			{
				jitterangle = 0;
			}
		}
		else
		{
			bSendThisPacket = true;
		}
		break;
	case YAW_TWTICH:
		if (pCmd->tick_count % 4)
		{
			bSendThisPacket = false;
			pCmd->viewangles.y -= spinSpeed - 100.000000;
		}
		else
		{
			bSendThisPacket = true;
			int random = rand() % 100;

			if (random < 98)
				pCmd->viewangles.y -= 180;

			if (random < 15)
			{
				float change = -70 + (rand() % (int)(140 + 1));
				pCmd->viewangles.y += change;
			}
			if (random == 69)
			{
				float change = -90 + (rand() % (int)(180 + 1));
				pCmd->viewangles.y += change;
			}
		}
		break;
	case YAW_BACKWARD:
		pCmd->viewangles.y -= 180.000000;
		break;
	}
	if (!Options.Rage.AntiAim.Yaw == 9 || !Options.Rage.AntiAim.Yaw == 10 || !Options.Rage.AntiAim.Pitch == 2 || !Options.Rage.AntiAim.Pitch == 3);
}

// At Targets

bool Hitchance()
{
	auto hitchance = 75.f + (Options.Rage.Aimbot.Hitchance / 4);
	if (Options.Rage.Aimbot.bHitchance || Options.Rage.Aimbot.bHitchanceSnipers && Global::LocalPlayer->GetWeapon()->GetWeaponType() == WEPCLASS_SNIPER)
	if ((1.0f - Global::LocalPlayer->GetWeapon()->GetAccuracyPenalty()) * 100.f >= hitchance)
		return true;
	if (!Options.Rage.Aimbot.bHitchance && !Options.Rage.Aimbot.bHitchanceSnipers)
		return true;
	if (Options.Rage.Aimbot.bHitchanceSnipers &&  Global::LocalPlayer->GetWeapon()->GetWeaponType() != WEPCLASS_SNIPER)
		return true;
	return false;
}


enum class CSGOClassID
{
	CAK47 = 1,
	CBaseAnimating = 2,
	CBaseAnimatingOverlay = 3,
	CBaseAttributableItem = 4,
	CBaseButton = 5,
	CBaseCombatCharacter = 6,
	CBaseCombatWeapon = 7,
	CBaseCSGrenade = 8,
	CBaseCSGrenadeProjectile = 9,
	CBaseDoor = 10,
	CBaseEntity = 11,
	CBaseFlex = 12,
	CBaseGrenade = 13,
	CBaseParticleEntity = 14,
	CBasePlayer = 15,
	CBasePropDoor = 16,
	CBaseTeamObjectiveResource = 17,
	CBaseTempEntity = 18,
	CBaseToggle = 19,
	CBaseTrigger = 20,
	CBaseViewModel = 21,
	CBaseVPhysicsTrigger = 22,
	CBaseWeaponWorldModel = 23,
	CBeam = 24,
	CBeamSpotlight = 25,
	CBoneFollower = 26,
	CBreakableProp = 27,
	CBreakableSurface = 28,
	CC4 = 29,
	CCascadeLight = 30,
	CChicken = 31,
	CColorCorrection = 32,
	CColorCorrectionVolume = 33,
	CCSGameRulesProxy = 34,
	CCSPlayer = 35,
	CCSPlayerResource = 36,
	CCSRagdoll = 37,
	CCSTeam = 38,
	CDEagle = 39,
	CDecoyGrenade = 40,
	CDecoyProjectile = 41,
	CDynamicLight = 42,
	CDynamicProp = 43,
	CEconEntity = 44,
	CEconWearable = 45,
	CEmbers = 46,
	CEntityDissolve = 47,
	CEntityFlame = 48,
	CEntityFreezing = 49,
	CEntityParticleTrail = 50,
	CEnvAmbientLight = 51,
	CEnvDetailController = 52,
	CEnvDOFController = 53,
	CEnvParticleScript = 54,
	CEnvProjectedTexture = 55,
	CEnvQuadraticBeam = 56,
	CEnvScreenEffect = 57,
	CEnvScreenOverlay = 58,
	CEnvTonemapController = 59,
	CEnvWind = 60,
	CFEPlayerDecal = 61,
	CFireCrackerBlast = 62,
	CFireSmoke = 63,
	CFireTrail = 64,
	CFish = 65,
	CFlashbang = 66,
	CFogController = 67,
	CFootstepControl = 68,
	CFunc_Dust = 69,
	CFunc_LOD = 70,
	CFuncAreaPortalWindow = 71,
	CFuncBrush = 72,
	CFuncConveyor = 73,
	CFuncLadder = 74,
	CFuncMonitor = 75,
	CFuncMoveLinear = 76,
	CFuncOccluder = 77,
	CFuncReflectiveGlass = 78,
	CFuncRotating = 79,
	CFuncSmokeVolume = 80,
	CFuncTrackTrain = 81,
	CGameRulesProxy = 82,
	CHandleTest = 83,
	CHEGrenade = 84,
	CHostage = 85,
	CHostageCarriableProp = 86,
	CIncendiaryGrenade = 87,
	CInferno = 88,
	CInfoLadderDismount = 89,
	CInfoOverlayAccessor = 90,
	CItem_Healthshot = 91,
	CKnife = 92,
	CKnifeGG = 93,
	CLightGlow = 94,
	CMaterialModifyControl = 95,
	CMolotovGrenade = 96,
	CMolotovProjectile = 97,
	CMovieDisplay = 98,
	CParticleFire = 99,
	CParticlePerformanceMonitor = 100,
	CParticleSystem = 101,
	CPhysBox = 102,
	CPhysBoxMultiplayer = 103,
	CPhysicsProp = 104,
	CPhysicsPropMultiplayer = 105,
	CPhysMagnet = 106,
	CPlantedC4 = 107,
	CPlasma = 108,
	CPlayerResource = 109,
	CPointCamera = 110,
	CPointCommentaryNode = 111,
	CPoseController = 112,
	CPostProcessController = 113,
	CPrecipitation = 114,
	CPrecipitationBlocker = 115,
	CPredictedViewModel = 116,
	CProp_Hallucination = 117,
	CPropDoorRotating = 118,
	CPropJeep = 119,
	CPropVehicleDriveable = 120,
	CRagdollManager = 121,
	CRagdollProp = 122,
	CRagdollPropAttached = 123,
	CRopeKeyframe = 124,
	CSCAR17 = 125,
	CSceneEntity = 126,
	CSensorGrenade = 127,
	CSensorGrenadeProjectile = 128,
	CShadowControl = 129,
	CSlideshowDisplay = 130,
	CSmokeGrenade = 131,
	CSmokeGrenadeProjectile = 132,
	CSmokeStack = 133,
	CSpatialEntity = 134,
	CSpotlightEnd = 135,
	CSprite = 136,
	CSpriteOriented = 137,
	CSpriteTrail = 138,
	CStatueProp = 139,
	CSteamJet = 140,
	CSun = 141,
	CSunlightShadowControl = 142,
	CTeam = 143,
	CTeamplayRoundBasedRulesProxy = 144,
	CTEArmorRicochet = 145,
	CTEBaseBeam = 146,
	CTEBeamEntPoint = 147,
	CTEBeamEnts = 148,
	CTEBeamFollow = 149,
	CTEBeamLaser = 150,
	CTEBeamPoints = 151,
	CTEBeamRing = 152,
	CTEBeamRingPoint = 153,
	CTEBeamSpline = 154,
	CTEBloodSprite = 155,
	CTEBloodStream = 156,
	CTEBreakModel = 157,
	CTEBSPDecal = 158,
	CTEBubbles = 159,
	CTEBubbleTrail = 160,
	CTEClientProjectile = 161,
	CTEDecal = 162,
	CTEDust = 163,
	CTEDynamicLight = 164,
	CTEEffectDispatch = 165,
	CTEEnergySplash = 166,
	CTEExplosion = 167,
	CTEFireBullets = 168,
	CTEFizz = 169,
	CTEFootprintDecal = 170,
	CTEFoundryHelpers = 171,
	CTEGaussExplosion = 172,
	CTEGlowSprite = 173,
	CTEImpact = 174,
	CTEKillPlayerAttachments = 175,
	CTELargeFunnel = 176,
	CTEMetalSparks = 177,
	CTEMuzzleFlash = 178,
	CTEParticleSystem = 179,
	CTEPhysicsProp = 180,
	CTEPlantBomb = 181,
	CTEPlayerAnimEvent = 182,
	CTEPlayerDecal = 183,
	CTEProjectedDecal = 184,
	CTERadioIcon = 185,
	CTEShatterSurface = 186,
	CTEShowLine = 187,
	CTesla = 188,
	CTESmoke = 189,
	CTESparks = 190,
	CTESprite = 191,
	CTESpriteSpray = 192,
	CTest_ProxyToggle_Networkable = 193,
	CTestTraceline = 194,
	CTEWorldDecal = 195,
	CTriggerPlayerMovement = 196,
	CTriggerSoundOperator = 197,
	CVGuiScreen = 198,
	CVoteController = 199,
	CWaterBullet = 200,
	CWaterLODControl = 201,
	CWeaponAug = 202,
	CWeaponAWP = 203,
	CWeaponBaseItem = 204,
	CWeaponBizon = 205,
	CWeaponCSBase = 206,
	CWeaponCSBaseGun = 207,
	CWeaponCycler = 208,
	CWeaponElite = 209,
	CWeaponFamas = 210,
	CWeaponFiveSeven = 211,
	CWeaponG3SG1 = 212,
	CWeaponGalil = 213,
	CWeaponGalilAR = 214,
	CWeaponGlock = 215,
	CWeaponHKP2000 = 216,
	CWeaponM249 = 217,
	CWeaponM3 = 218,
	CWeaponM4A1 = 219,
	CWeaponMAC10 = 220,
	CWeaponMag7 = 221,
	CWeaponMP5Navy = 222,
	CWeaponMP7 = 223,
	CWeaponMP9 = 224,
	CWeaponNegev = 225,
	CWeaponNOVA = 226,
	CWeaponP228 = 227,
	CWeaponP250 = 228,
	CWeaponP90 = 229,
	CWeaponSawedoff = 230,
	CWeaponSCAR20 = 231,
	CWeaponScout = 232,
	CWeaponSG550 = 233,
	CWeaponSG552 = 234,
	CWeaponSG556 = 235,
	CWeaponSSG08 = 236,
	CWeaponTaser = 237,
	CWeaponTec9 = 238,
	CWeaponTMP = 239,
	CWeaponUMP45 = 240,
	CWeaponUSP = 241,
	CWeaponXM1014 = 242,
	CWorld = 243,
	DustTrail = 244,
	MovieExplosion = 245,
	ParticleSmokeGrenade = 246,
	RocketTrail = 247,
	SmokeTrail = 248,
	SporeExplosion = 249,
	SporeTrail = 250
};


bool cRage::TargetMeetsRequirements(CBaseEntity* pEntity)
{
	// Is a valid player


	if (pEntity && pEntity->GetDormant() == false && pEntity->GetAlive() && pEntity->GetIndex() != pEntity->GetIndex())
	{
		// Entity Type checks
		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			// Team Check
			if (pEntity->GetTeam() != pEntity->GetTeam() || Options.Rage.Aimbot.FriendAim)
			{
				// Spawn Check
				if (!pEntity->GetImmune())
				{
					return true;
				}
			}
		}
	}

	// They must have failed a requirement
	return false;
}

FORCEINLINE void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

void AngleVectors(const Vector &angles, Vector *forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float	sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward->x = cp*cy;
	forward->y = cp*sy;
	forward->z = -sp;
}

void Normalize(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0) {
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}





void cRage::Aimbot(CUserCmd* pCmd)
{
	/* Declarations */
	this->IsAimbotting = false;

	float server_time = Global::LocalPlayer->GetTickBase() * Globals->interval_per_tick;

	CBaseCombatWeapon* weapon = Global::LocalPlayer->GetWeapon();
	QAngle angle = pCmd->viewangles;

	auto iBestTarget = this->TargetSelection(pCmd);
	auto pWeapon = Global::LocalPlayer->GetWeapon();
	auto iClip = *reinterpret_cast<int*>(reinterpret_cast<DWORD>(pWeapon) + offsets.m_iClip1);

	if (!(iClip > 0)) 
		return;

	if (pWeapon->GetWeaponType() == WEPCLASS_KNIFE) 
		return;

	if(iBestTarget != -1)
	{
		auto BestTarget = ClientEntList->GetClientEntity(iBestTarget);

		/* Hitbox Selection */
		Vector hitbox;
		if (Options.Rage.Aimbot.HitScans == 0)
			hitbox = BestTarget->GetBonePosition(Options.Rage.Aimbot.Hitbox);
		if (Options.Rage.Aimbot.HitScans == 1)
			BestAimPointAll(BestTarget, hitbox);
		if (Options.Rage.Aimbot.HitScans == 2)
			BestAimPointHitbox(BestTarget, hitbox);
		if (Options.Rage.Aimbot.HitScans == 3)
			BestAimPoint(BestTarget, hitbox);
		if (Options.Rage.Aimbot.HitScans == 4)

		if (Options.Rage.Aimbot.AAA.bone[iBestTarget] != 0)
			hitbox = BestTarget->GetBonePosition(Options.Rage.Aimbot.AAA.bone[iBestTarget] - 1);
		//=================================

		bool IsLocked;
		int TargetID;
		int HitBox;
		Vector AimPoint;
		CBaseEntity* pTarget = nullptr;

		CBaseEntity* pLocal;


		if (hitbox != Vector(0, 0, 0))
		{
			BestTarget->GetPredicted(hitbox);

			QAngle qTarget;
			CalcAngle(Global::LocalPlayer->GetEyePosition(), hitbox, qTarget);

			auto switchback = pCmd->viewangles;
			qTarget - pCmd->viewangles;
			qTarget.Normalize();

			if(Options.Rage.Aimbot.AutoScope)
				if (!Global::LocalPlayer->IsScoped() && pWeapon->IsScopedWeapon())
					pCmd->buttons |= IN_ATTACK2;
			
			if(Options.Rage.Aimbot.AutoScope && pWeapon->IsScopedWeapon() 
				&& Global::LocalPlayer->IsScoped() || !pWeapon->IsScopedWeapon() 
				|| !Options.Rage.Aimbot.AutoScope)
			{

				if(Hitchance())
				{
					if (!Options.Rage.Aimbot.AutoShoot && pCmd->buttons & IN_ATTACK || Options.Rage.Aimbot.AutoShoot)
					{
						pCmd->viewangles = qTarget;
						if (!Options.Rage.Aimbot.SilentAim)
							Engine->SetViewAngles(pCmd->viewangles);
						pCmd->viewangles -= Global::LocalPlayer->GetPunch() * 2.00f;
						pCmd->viewangles.Normalize();
						this->IsAimbotting = true;
					}
					target = iBestTarget;
					//AimStep(BestTarget, qTarget, pCmd); // Test
					//pCmd->viewangles = qTarget;
					if (Options.Rage.Aimbot.AutoShoot)
					{
						float nextPrimaryAttack = Global::LocalPlayer->GetWeapon()->GetNextPrimaryAttack();
						float tick = Global::LocalPlayer->GetTickBase() * Globals->interval_per_tick;
						if (nextPrimaryAttack > tick)
						{
							if (Global::LocalPlayer->GetWeapon()->GetItemDefinitionIndex() == WEAPON_REVOLVER)
								pCmd->buttons &= ~IN_ATTACK2;
							else
								pCmd->buttons &= ~IN_ATTACK;
						}
						else
						{
							if (Global::LocalPlayer->GetWeapon()->GetItemDefinitionIndex() == WEAPON_REVOLVER)
								pCmd->buttons |= IN_ATTACK2;
							else
								pCmd->buttons |= IN_ATTACK;
						}
					}
				}
			}
			else
			{
				if (!Global::LocalPlayer->IsScoped() && pWeapon->IsScopedWeapon())
					pCmd->buttons |= IN_ATTACK2;
			}
		}
	}

}

void CFixMove::Start()
{
	m_oldangle = Global::pCmd->viewangles;
	m_oldforward = Global::pCmd->forwardmove;
	m_oldsidemove = Global::pCmd->sidemove;
}

void CFixMove::End()
{
	auto yaw_delta = Global::pCmd->viewangles.y - m_oldangle.y;
	float f1;
	float f2;

	if (m_oldangle.y < 0.f)
		f1 = 360.0f + m_oldangle.y;
	else
		f1 = m_oldangle.y;

	if (Global::pCmd->viewangles.y < 0.0f)
		f2 = 360.0f + Global::pCmd->viewangles.y;
	else
		f2 = Global::pCmd->viewangles.y;

	if (f2 < f1)
		yaw_delta = abs(f2 - f1);
	else
		yaw_delta = 360.0f - abs(f1 - f2);
	yaw_delta = 360.0f - yaw_delta;

	Global::pCmd->forwardmove = cos(DEG2RAD(yaw_delta)) * m_oldforward + cos(DEG2RAD(yaw_delta + 90.f)) * m_oldsidemove;
	Global::pCmd->sidemove = sin(DEG2RAD(yaw_delta)) * m_oldforward + sin(DEG2RAD(yaw_delta + 90.f)) * m_oldsidemove;
}

