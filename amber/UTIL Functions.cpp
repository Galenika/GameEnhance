

ServerRankRevealAllFn GameUtils::ServerRankRevealAllEx;
IsReadyFn GameUtils::IsReady;

DWORD GameUtils::FindPattern1(std::string moduleName, std::string pattern)
{
	const char* pat = pattern.c_str();
	DWORD firstMatch = 0;
	DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
	MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else
				pat += 2;    //one ?
		}
		else
		{
			pat = pattern.c_str();
			firstMatch = 0;
		}
	}
	return NULL;
}

void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, const IClientEntity *ignore, int collisionGroup, trace_t *ptr)
{
	typedef int(__fastcall* UTIL_TraceLine_t)(const Vector&, const Vector&, unsigned int, const IClientEntity*, int, trace_t*);
	static UTIL_TraceLine_t TraceLine = (UTIL_TraceLine_t)Utilities::Memory::FindPatternV2("client.dll", "55 8B EC 83 E4 F0 83 EC 7C 56 52");
	TraceLine(vecAbsStart, vecAbsEnd, mask, ignore, collisionGroup, ptr);
}


void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
{
	static DWORD dwAddress = Utilities::Memory::FindPattern("client.dll", (BYTE*)"\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF0\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x81\xEC\x00\x00\x00\x00\x8B\x43\x10", "xxxxxxxxxxxxxxxxxxxxxxxx????xxx");

	if (!dwAddress)
		return;

	_asm
	{
		MOV		EAX, filter
		LEA		ECX, tr
		PUSH	ECX
		PUSH	EAX
		PUSH	mask
		LEA		EDX, vecAbsEnd
		LEA		ECX, vecAbsStart
		CALL	dwAddress
		ADD		ESP, 0xC
	}
}

bool IsBreakableEntity(IClientEntity* ent)
{
	typedef bool(__thiscall* IsBreakbaleEntity_t)(IClientEntity*);
	IsBreakbaleEntity_t IsBreakbaleEntityFn = (IsBreakbaleEntity_t)Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x51\x56\x8B\xF1\x85\xF6\x74\x68", "xxxxxxxxxxx");
	if (IsBreakbaleEntityFn)
		return IsBreakbaleEntityFn(ent);
	else
		return false;
}

bool TraceToExit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace)
{
	typedef bool(__fastcall* TraceToExitFn)(Vector&, trace_t&, float, float, float, float, float, float, trace_t*);
	static TraceToExitFn TraceToExit = (TraceToExitFn)Utilities::Memory::FindPatternV2("client.dll", "55 8B EC 83 EC 30 F3 0F 10 75");

	if (!TraceToExit)
		return false;

	return TraceToExit(end, tr, start.x, start.y, start.z, vEnd.x, vEnd.y, vEnd.z, trace);
}

/*unsigned short GameUtils::UTIL_GetAchievementEventMask()
{
static uintptr_t GetAchievementEventMask_func = Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x81\xEC\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x68\x00\x00\x00\x00", "xxxxx????xx????x????");
typedef unsigned short(__fastcall* UTIL_GetAchievementEventMask_t)();
static UTIL_GetAchievementEventMask_t pUTIL_GetAchievementEventMask =
reinterpret_cast<UTIL_GetAchievementEventMask_t>(GetAchievementEventMask_func);
return pUTIL_GetAchievementEventMask();
}*/

void GameUtils::NormaliseViewAngle(Vector &angle)
{
	if (!Menu::Window.MiscTab.OtherSafeMode.GetState())
	{
		return;
	}
	else
	{
		while (angle.y <= -180) angle.y += 360;
		while (angle.y > 180) angle.y -= 360;
		while (angle.x <= -180) angle.x += 360;
		while (angle.x > 180) angle.x -= 360;


		if (angle.x > 89) angle.x = 89;
		if (angle.x < -89) angle.x = -89;
		if (angle.y < -180) angle.y = -179.999;
		if (angle.y > 180) angle.y = 179.999;

		angle.z = 0;
	}
}

void GameUtils::CL_FixMove(CUserCmd* pCmd, Vector viewangles)
{
	/*pCmd->forwardmove = DotProduct(forward * vForwardNorm, aimforward) + DotProduct(right * vRightNorm, aimforward) + DotProduct(up * vUpNorm, aimforward);
	pCmd->sidemove = DotProduct(forward * vForwardNorm, aimright) + DotProduct(right * vRightNorm, aimright) + DotProduct(up * vUpNorm, aimright);
	pCmd->upmove = DotProduct(forward * vForwardNorm, aimup) + DotProduct(right * vRightNorm, aimup) + DotProduct(up * vUpNorm, aimup);*/
}

char shit[16];
trace_t Trace;
char shit2[16];
IClientEntity* entCopy;

bool GameUtils::IsVisible(IClientEntity* pLocal, IClientEntity* pEntity, int BoneID)
{
	if (BoneID < 0) return false;

	entCopy = pEntity;
	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = GetHitboxPosition(pEntity, BoneID);//pEntity->GetBonePos(BoneID);
	char shit3[32];

	//Interfaces::Trace->TraceRay(Ray,MASK_SOLID, NULL/*&filter*/, &Trace);
	UTIL_TraceLine(start, end, MASK_SOLID, pLocal, 0, &Trace);

	if (Trace.m_pEnt == entCopy)
	{
		return true;
	}

	if (Trace.fraction == 1.0f)
	{
		return true;
	}

	return false;

}

void GameUtils::ClampViewAngle(Vector &angle)
{

	CHECK_VALID(*angle);
	if (angle.x < -89.0f)
		angle.x = -89.0f;

	if (angle.x >  89.0f)
		angle.x = 89.0f;

	while (angle.y < -180.0f)
		angle.y += 360.0f;

	while (angle.y >  180.0f)
		angle.y -= 360.0f;

	angle.z = 0.0f;

}

bool GameUtils::IsBallisticWeapon(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)CSGOClassID::CHEGrenade || pWeaponClass->m_ClassID == (int)CSGOClassID::CDecoyGrenade || pWeaponClass->m_ClassID == (int)CSGOClassID::CIncendiaryGrenade || pWeaponClass->m_ClassID == (int)CSGOClassID::CSmokeGrenade || pWeaponClass->m_ClassID == (int)CSGOClassID::CC4 || pWeaponClass->m_ClassID == (int)CSGOClassID::CMolotovGrenade || pWeaponClass->m_ClassID == (int)CSGOClassID::CMolotovProjectile)
		return false;
	else
		return true;
}

bool GameUtils::IsKnife(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_KNIFE_BAYONET,WEAPON_KNIFE_BUTTERFLY,WEAPON_KNIFE_BOWIE,WEAPON_KNIFE_CT,WEAPON_KNIFE_FALCHION,WEAPON_KNIFE_FLIP,WEAPON_KNIFE_GUT,WEAPON_KNIFE_KARAMBIT,WEAPON_KNIFE_M9_BAYONET,WEAPON_KNIFE_PUSH,WEAPON_KNIFE_T,WEAPON_KNIFE_TACTICAL };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsPistol(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_DEAGLE,WEAPON_CZ75A,WEAPON_ELITE,WEAPON_USP_SILENCER,WEAPON_P250,WEAPON_HKP2000, WEAPON_TEC9,WEAPON_REVOLVER,WEAPON_FIVESEVEN,WEAPON_GLOCK };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsSniper(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_AWP,WEAPON_SSG08,WEAPON_G3SG1,WEAPON_SCAR20 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsScopedWeapon(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_AWP,WEAPON_SSG08,WEAPON_G3SG1,WEAPON_SCAR20, WEAPON_AUG, WEAPON_SG556 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

void SayInChat(const char *text)
{
	char buffer[250];
	sprintf_s(buffer, "say \"%s\"", text);
	Interfaces::Engine->ClientCmd_Unrestricted(buffer);
}

//Vector MultipointVectors[] = { Vector(0,0,0), Vector(0,0,1.5),Vector(0,0,3),Vector(0,0,4), Vector(0,0,-2), Vector(0,0,-4), Vector(0,0,4.8), Vector(0,0,5), Vector(0,0,5.4), Vector(0,3,0), Vector(3,0,0),Vector(-3,0,0),Vector(0,-3,0), Vector(0,2,4.2), Vector(0,-2,4.2), Vector(2,0,4.2), Vector(-2,0,4.2),  Vector(3.8,0,0), Vector(-3.8,0,0),Vector(0,3.6,0), Vector(0,-3.6,0),  Vector(0,1.2,3.2), Vector(0,0.6,1.4), Vector(0,3.1,5.1), Vector(0,0,6.2), Vector(0,2.5,0), Vector(2.1,2.1,2.1) };

Vector GetHitboxPosition(IClientEntity* pEntity, int Hitbox)
{
	matrix3x4 matrix[128];

	if (!pEntity->SetupBones(matrix, 128, 0x00000100, GetTickCount64()))
		return Vector(0, 0, 0);

	studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());
	mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

	mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

	if (!hitbox)
		return Vector(0, 0, 0);

	Vector vMin, vMax, vCenter, sCenter;
	VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
	VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
	vCenter = (vMin + vMax) *0.5f;

	IClientEntity* pLocal = hackManager.pLocal();

	if (Menu::Window.RageBotTab.ResolverFLFix.GetIndex() == 3) {
		if ((pLocal->GetVelocity().Length2D() * 0.0078125f) > 64.0f) {
			vCenter += (pLocal->GetAbsOrigin() - pLocal->GetVecOrigin());
		}
	}

	float pointscale = Menu::Window.RageBotTab.TargetPointScale.GetValue();
	vCenter.z += pointscale * 3;

	float HighestDamage = 0;

	return vCenter;
}

Vector GetHitBoxPosV2(IClientEntity* pEntity, int Hitbox)
{
	matrix3x4 matrix[128];
	Vector vMin, vMax, vCenter, sCenter;
	if (pEntity->SetupBones(matrix, 128, 0x100, 0.f))
	{
		studiohdr_t * hdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());
		mstudiohitboxset_t * set = hdr->GetHitboxSet(0);

		mstudiobbox_t * hitbox = set->GetHitbox(Hitbox);

		// Points to MultiPoint
		Vector Point[] =
		{
			Vector(hitbox->bbmin.x, hitbox->bbmin.y, hitbox->bbmin.z),
			Vector(hitbox->bbmin.x, hitbox->bbmax.y, hitbox->bbmin.z),
			Vector(hitbox->bbmax.x, hitbox->bbmax.y, hitbox->bbmin.z),
			Vector(hitbox->bbmax.x, hitbox->bbmin.y, hitbox->bbmin.z),
			Vector(hitbox->bbmax.x, hitbox->bbmax.y, hitbox->bbmax.z),
			Vector(hitbox->bbmin.x, hitbox->bbmax.y, hitbox->bbmax.z),
			Vector(hitbox->bbmin.x, hitbox->bbmin.y, hitbox->bbmax.z),
			Vector(hitbox->bbmax.x, hitbox->bbmin.y, hitbox->bbmax.z)
		};

		if (hitbox)
		{
			// Just Hitscan
			VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
			VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);

			vCenter = ((vMin + vMax) * 0.5f);

			return vCenter;

			// Multipoint
			if (Menu::Window.RageBotTab.AimbotMultiPoint.GetState())
			{
				int iCount = 7;
				for (int i = 0; i <= iCount; i++)
				{
					Vector vTargetPos;
					switch (i)
					{
					case 0:
					default:
						vTargetPos = vCenter; break;
					case 1:
						vTargetPos = (Point[7] + Point[1]) * Menu::Window.RageBotTab.AccuracyPointScale.GetValue(); break;
					case 2:
						vTargetPos = (Point[3] + Point[4]) * Menu::Window.RageBotTab.AccuracyPointScale.GetValue(); break;
					case 3:
						vTargetPos = (Point[4] + Point[0]) * Menu::Window.RageBotTab.AccuracyPointScale.GetValue(); break;
					case 4:
						vTargetPos = (Point[2] + Point[7]) * Menu::Window.RageBotTab.AccuracyPointScale.GetValue(); break;
					case 5:
						vTargetPos = (Point[6] + Point[7]) * Menu::Window.RageBotTab.AccuracyPointScale.GetValue(); break;
					case 6:
						vTargetPos = (Point[5] + Point[3]) * Menu::Window.RageBotTab.AccuracyPointScale.GetValue(); break;
					case 7:
						vTargetPos = (Point[1] + Point[2]) * Menu::Window.RageBotTab.AccuracyPointScale.GetValue(); break;
					}
					return vTargetPos;
				}
			}
		}
	}

	return Vector(0, 0, 0);
}

int GameUtils::GetPlayerCompRank(IClientEntity* pEntity)
{
	static DWORD m_iCompetitiveRanking = NetVar.GetNetVar(0x75671F7F);
	static DWORD GameResources = *(DWORD*)(Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x85\xFF\x0F\x84\x00\x00\x00\x00\x81\xC7", "xx????xxxx????xx") + 0x2);

	return *(int*)((DWORD)GameResources + m_iCompetitiveRanking + (int)pEntity->GetIndex() * 4);
}

void ForceUpdate()
{
	// Shh
	static DWORD clientstateaddr = Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x8A\xF9\xF3\x0F\x11\x45\xF8\x83\xBF\xE8\x00\x00\x00\x02", "xx????xxxxxxxxxxxxxx");
	static uintptr_t pEngineBase = (uintptr_t)GetModuleHandleA("engine.dll");

	static uintptr_t pClientState = **(uintptr_t**)(Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x8A\xF9", "xx????xx") + 2);

	static uintptr_t dwAddr1 = Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\xE8\x00\x00\x00\x00\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\xC7\x87\x00\x00\x00\x00\x00\x00\x00\x00", "x????x????x????xx????????");

	//E8 call is being used here
	static uintptr_t dwRelAddr = *(uintptr_t*)(dwAddr1 + 1);
	static uintptr_t sub_B5E60 = ((dwAddr1 + 5) + dwRelAddr);

	__asm
	{
		pushad
		mov edi, pClientState
		lea ecx, dword ptr[edi + 0x8]
		call sub_B5E60
		mov dword ptr[edi + 0x154], 0xFFFFFFFF
		popad
	}
}