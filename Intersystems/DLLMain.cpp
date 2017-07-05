#include "SDK.h"
#include "Hooks.h"
#include "Utils.h"
#include "NetVars.h"
#include "THook.h"
#include "vthook.h"
#include "xorstr.h"
#include "Config.h"
#include "ItemDefinitions.h"
#include "MathLib.h"
#include "RainbowColor.h"

#include "Parser.h"

#include "CEvents.h"
#include "CMovement.h"
#include "CChams.h"
#include "CRender.h"
#include "CFrameStageNotify.h"
#include "CProxy.h"

typedef void(__cdecl* ConsoleMsgFn)(const char* msg, va_list);
void ConsoleMsg(const char* msg, ...)
{
	if (msg == nullptr)
		return;

	ConsoleMsgFn fn = (ConsoleMsgFn)GetProcAddress(GetModuleHandle(XorStr("tier0.dll")), XorStr("Msg"));
	char buffer[989];
	va_list list;
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	fn(buffer, list);
}

void Msg(const char* c)
{
	printf(c);
	printf("\n");
}

DWORD FindSig(DWORD dwAddress, DWORD dwLength, const char* szPattern)
{
	if (!dwAddress || !dwLength || !szPattern)
		return 0;

	const char* pat = szPattern;
	DWORD firstMatch = NULL;

	for (DWORD pCur = dwAddress; pCur < dwLength; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == GetByte(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else pat += 2;
		}
		else
		{
			pat = szPattern;
			firstMatch = 0;
		}
	}

	return 0;
}

DWORD FindSignature(const char* szModuleName, const char* PatternName, char* szPattern)
{
	HMODULE hModule = GetModuleHandleA(szModuleName);
	PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS pNTHeaders = (PIMAGE_NT_HEADERS)(((DWORD)hModule) + pDOSHeader->e_lfanew);

	DWORD ret = FindSig(((DWORD)hModule) + pNTHeaders->OptionalHeader.BaseOfCode, ((DWORD)hModule) + pNTHeaders->OptionalHeader.SizeOfCode, szPattern);;

	std::cout << PatternName << " (" << szModuleName << ") - 0x" <<  (DWORD*)ret << std::endl;

	return ret;
}

typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);
template <typename T>
T* CaptureInterface(std::string strModule, std::string strInterface)
{
	CreateInterfaceFn nCreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(strModule.c_str()), "CreateInterface");
	T* tRet = (T*)nCreateInterface(strInterface.c_str(), NULL);
	std::cout << strInterface.c_str() << XorStr("(") << strModule << XorStr("): 0x") << tRet << std::endl;

	return tRet;
}

void __stdcall hkOverrideView(CViewSetup* vsView)
{
	CPlayer* LocalPlayer = Interface.EntityList->GetClientEntity<CPlayer>(Interface.Engine->GetLocalPlayer());
	if (LocalPlayer && Interface.Engine->IsInGame())
	{
		if (!LocalPlayer->IsScoped())
			if (Options::Visuals::Misc::FovChanger) vsView->fov = Options::Visuals::Misc::FovChangerValue;
	}

	O::OverrideView(vsView);
}

float __stdcall hkGetViewModelFOV()
{
	if (Options::Visuals::Misc::ViewmodelChanger)
		return Options::Visuals::Misc::ViewmodelChangerValue;
	else
		return O::GetViewModelFOV();
}

int ParseSkins()
{
	valve_parser::Document doc;
	auto r = doc.Load(".\\csgo\\scripts\\items\\items_game.txt", valve_parser::ENCODING::UTF8);
	if (!r)
		return 1;

	valve_parser::Document english;
	r = english.Load(".\\csgo\\resource\\csgo_english.txt", valve_parser::ENCODING::UTF16_LE);
	if (!r)
		return 2;

	auto weaponSkinCombo = doc.BreadthFirstSearch("weapon_icons");
	if (!weaponSkinCombo || !weaponSkinCombo->ToObject())
		return 3;

	auto skinDataVec = doc.BreadthFirstSearchMultiple("paint_kits");
	if (!skinDataVec.size())
		return 4;

	auto PaintKitNames = english.BreadthFirstSearch("Tokens");
	if (!PaintKitNames || !PaintKitNames->ToObject())
		return 5;

	//std::unordered_map<std::string, std::set<std::string>> G::weaponSkins;
	//std::unordered_map<std::string, skinInfo> G::skinMap;
	//std::unordered_map<std::string, std::string> G::skinNames;

	std::vector<std::string> weaponNames = {
		"deagle",
		"elite",
		"fiveseven",
		"glock",
		"ak47",
		"aug",
		"awp",
		"famas",
		"g3sg1",
		"galilar",
		"m249",
		"m4a1_silencer", //needs to be before m4a1 else silencer doesnt get filtered out :D
		"m4a1",
		"mac10",
		"p90",
		"ump45",
		"xm1014",
		"bizon",
		"mag7",
		"negev",
		"sawedoff",
		"tec9",
		"hkp2000",
		"mp7",
		"mp9",
		"nova",
		"p250",
		"scar20",
		"sg556",
		"ssg08",
		"usp_silencer",
		"cz75a",
		"revolver",
		"knife_m9_bayonet", //needs to be before bayonet else knife_m9_bayonet doesnt get filtered out :D
		"bayonet",
		"knife_flip",
		"knife_gut",
		"knife_karambit",
		"knife_tactical",
		"knife_falchion",
		"knife_survival_bowie",
		"knife_butterfly",
		"knife_push"
	};

	//populate G::weaponSkins
	for (auto child : weaponSkinCombo->children)
	{
		if (child->ToObject())
		{
			for (auto weapon : weaponNames)
			{
				auto skinName = child->ToObject()->GetKeyByName("icon_path")->ToKeyValue()->Value.toString();
				auto pos = skinName.find(weapon);
				//filter out the skinname
				if (pos != std::string::npos)
				{
					auto pos2 = skinName.find_last_of('_');
					G::weaponSkins[weapon].insert(
						skinName.substr(pos + weapon.length() + 1,
							pos2 - pos - weapon.length() - 1)
					);
					break;
				}
			}
		}
	}

	//populate skinData
	for (auto skinData : skinDataVec)
	{
		if (skinData->ToObject())
		{
			for (auto skin : skinData->children)
			{
				if (skin->ToObject())
				{
					skinInfo si;
					si.paintkit = skin->ToObject()->name.toInt();

					auto skinName = skin->ToObject()->GetKeyByName("name")->ToKeyValue()->Value.toString();
					auto tagNode = skin->ToObject()->GetKeyByName("description_tag");
					if (tagNode)
					{
						std::string tag = tagNode->ToKeyValue()->Value.toString();
						tag = tag.substr(1, std::string::npos); //skip #
						std::transform(tag.begin(), tag.end(), tag.begin(), towlower);
						si.tagName = tag;
					}

					auto keyVal = skin->ToObject()->GetKeyByName("seed");
					if (keyVal != nullptr)
						si.seed = keyVal->ToKeyValue()->Value.toInt();

					G::skinMap[skinName] = si;
				}
			}
		}
	}

	//populate G::skinNames
	for (auto child : PaintKitNames->children)
	{
		if (child->ToKeyValue())
		{
			std::string key = child->ToKeyValue()->Key.toString();
			std::transform(key.begin(), key.end(), key.begin(), towlower);
			if (key.find("paintkit") != std::string::npos &&
				key.find("tag") != std::string::npos)
			{
				G::skinNames[key] = child->ToKeyValue()->Value.toString();
			}
		}
	}

	//dump skins
	/*auto f = fopen("dump.txt", "wb");
	if (f)
	{
		for (auto weapon : weaponNames)
		{
			fwrite(weapon.c_str(), sizeof(char), weapon.length(), f);
			fwrite(std::string("\n").c_str(), sizeof(char), std::string("\n").length(), f);
			for (auto skin : G::weaponSkins[weapon])
			{
				fwrite(std::string("\t").c_str(), sizeof(char), std::string("\t").length(), f);
				fwrite(skin.c_str(), sizeof(char), skin.length(), f);
				fwrite(std::string(" | ").c_str(), sizeof(char), std::string(" | ").length(), f);
				fwrite(std::to_string(G::skinMap[skin].paintkit).c_str(), sizeof(char), std::to_string(G::skinMap[skin].paintkit).length(), f);
				fwrite(std::string(" | ").c_str(), sizeof(char), std::string(" | ").length(), f);
				fwrite(G::skinNames[G::skinMap[skin].tagName].c_str(), sizeof(char), G::skinNames[G::skinMap[skin].tagName].length(), f);
				fwrite(std::string("\n").c_str(), sizeof(char), std::string("\n").length(), f);
			}
		}
		fclose(f);
	}*/

	return 0;
}

int Init(HMODULE hModule)
{
	while (!(g_hWnd = FindWindowA(XorStr("Valve001"), NULL)))
		Sleep(200);


	Interface.Client = CaptureInterface<IBaseClientDLL>(XorStr("client.dll"), XorStr("VClient018")); //
	Interface.EntityList = CaptureInterface<IClientEntityList>(XorStr("client.dll"), XorStr("VClientEntityList003"));
	Interface.Engine = CaptureInterface<IVEngineClient>(XorStr("engine.dll"), XorStr("VEngineClient014"));
	Interface.EngineTrace = CaptureInterface<IEngineTrace>(XorStr("engine.dll"), XorStr("EngineTraceClient004"));
	Interface.ModelInfo = CaptureInterface<IVModelInfoClient>(XorStr("engine.dll"), XorStr("VModelInfoClient004"));
	Interface.ModelRender = CaptureInterface<IVModelRender>(XorStr("engine.dll"), XorStr("VEngineModel016"));
	Interface.RenderView = CaptureInterface<IVRenderView>(XorStr("engine.dll"), XorStr("VEngineRenderView014")); //
	Interface.GameEventsManager = CaptureInterface<IGameEventManager2>(XorStr("engine.dll"), XorStr("GAMEEVENTSMANAGER002"));
	Interface.DebugOverlay = CaptureInterface<IVDebugOverlay>(XorStr("engine.dll"), XorStr("VDebugOverlay004"));
	Interface.Input = CaptureInterface<IInputSystem>(XorStr("inputsystem.dll"), XorStr("InputSystemVersion001"));
	Interface.CVar = CaptureInterface<ICVar>(XorStr("vstdlib.dll"), XorStr("VEngineCvar007"));
	Interface.PlayerInfoManager = CaptureInterface<IPlayerInfoManager>(XorStr("server.dll"), XorStr("PlayerInfoManager002"));
	Interface.MatSystem = CaptureInterface<IMaterialSystem>(XorStr("materialsystem.dll"), XorStr("VMaterialSystem080"));
	Interface.ClientMode = **(IClientMode***)((*(DWORD**)Interface.Client)[10] + 0x5);
	Interface.Globals = **(CGlobalVars***)((*(DWORD**)Interface.Client)[0] + 0x1B);

	// check for version
	//if (std::string(Interface.Engine->GetProductVersionString()) != XorStr("1.35.6.8"))
	//{
	//	MessageBox(0, XorStr("CS:GO was updated."), "", 0);
	//	return 0;
	//}

	Resolver.NetVars = std::shared_ptr<CNetVars>(new CNetVars());
	Resolver.NetVars->GetOffset(XorStr("DT_BaseEntity"),						XorStr("m_CollisionGroup"),			&NetVars.m_rgflCoordinateFrame, -0x30);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseEntity"),						XorStr("m_Collision"),					&NetVars.m_Collision);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseEntity"),						XorStr("m_vecOrigin"),				&NetVars.m_vecOrigin);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseEntity"),						XorStr("m_iTeamNum"),				&NetVars.m_iTeamNum);
	Resolver.NetVars->GetOffset(XorStr("DT_BasePlayer"),						XorStr("m_aimPunchAngle"),		&NetVars.m_vecPunchAngles);
	Resolver.NetVars->GetOffset(XorStr("DT_BasePlayer"),						XorStr("m_viewPunchAngle"),		&NetVars.m_vecViewPunchAngles);
	Resolver.NetVars->GetOffset(XorStr("DT_BasePlayer"),						XorStr("m_vecViewOffset[0]"),		&NetVars.m_vecViewOffset);
	Resolver.NetVars->GetOffset(XorStr("DT_BasePlayer"),						XorStr("m_vecVelocity[0]"),			&NetVars.m_vecVelocity);
	Resolver.NetVars->GetOffset(XorStr("DT_BasePlayer"),						XorStr("m_lifeState"),					&NetVars.m_lifeState);
	Resolver.NetVars->GetOffset(XorStr("DT_BasePlayer"),						XorStr("m_fFlags"),						&NetVars.m_fFlags);
	Resolver.NetVars->GetOffset(XorStr("DT_BasePlayer"),						XorStr("m_iHealth"),					&NetVars.m_iHealth);
	Resolver.NetVars->GetOffset(XorStr("DT_BasePlayer"),						XorStr("m_hActiveWeapon"),		&NetVars.m_hActiveWeapon);
	Resolver.NetVars->GetOffset(XorStr("DT_BasePlayer"),						XorStr("m_hMyWeapons"),			&NetVars.m_hMyWeapons); // не верный оффсет почему то :c
	Resolver.NetVars->GetOffset(XorStr("DT_BasePlayer"),						XorStr("m_hViewModel[0]"),		&NetVars.m_hViewModel);
	Resolver.NetVars->GetOffset(XorStr("DT_BasePlayer"),						XorStr("m_nTickBase"),				&NetVars.m_nTickBase);
	Resolver.NetVars->GetOffset(XorStr("DT_BasePlayer"),						XorStr("m_hObserverTarget"),		&NetVars.m_hObserverTarget);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseAttributableItem"),		XorStr("m_iAccountID"),				&NetVars.m_iAccountID);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseAttributableItem"),		XorStr("m_iItemDefinitionIndex"),	&NetVars.m_iItemDefinitionIndex);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseAttributableItem"),		XorStr("m_iItemIDHigh"),			&NetVars.m_iItemIDHigh);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseAttributableItem"),		XorStr("m_iEntityQuality"),			&NetVars.m_iEntityQuality);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseAttributableItem"),		XorStr("m_szCustomName"),		&NetVars.m_szCustomName);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseAttributableItem"),		XorStr("m_nFallbackPaintKit"),		&NetVars.m_nFallbackPaintKit);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseAttributableItem"),		XorStr("m_flFallbackWear"),			&NetVars.m_flFallbackWear);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseAttributableItem"),		XorStr("m_nFallbackSeed"),			&NetVars.m_nFallbackSeed);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseAttributableItem"),		XorStr("m_nFallbackStatTrak"),	&NetVars.m_nFallbackStatTrak);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseAttributableItem"),		XorStr("m_OriginalOwnerXuidLow"),	&NetVars.m_OriginalOwnerXuidLow);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseAttributableItem"),		XorStr("m_OriginalOwnerXuidHigh"),	&NetVars.m_OriginalOwnerXuidHigh);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseViewModel"),				XorStr("m_nModelIndex"),			&NetVars.m_nModelIndex);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseViewModel"),				XorStr("m_hOwner"),					&NetVars.m_hOwner);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseViewModel"),				XorStr("m_hWeapon"),				&NetVars.m_hWeapon);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseCombatWeapon"),			XorStr("m_flNextPrimaryAttack"),	&NetVars.m_flNextPrimaryAttack);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseCombatWeapon"),		XorStr("m_flNextSecondaryAttack"),	&NetVars.m_flNextSecondaryAttack);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseCombatWeapon"),			XorStr("m_iClip1"),						&NetVars.m_iClip1);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseCombatWeapon"),	 XorStr("m_iPrimaryReserveAmmoCount"), &NetVars.m_iClip2);
	Resolver.NetVars->GetOffset(XorStr("DT_CSPlayer"),							XorStr("m_bIsScoped"),				&NetVars.m_bIsScoped);
	Resolver.NetVars->GetOffset(XorStr("DT_CSPlayer"),							XorStr("m_iShotsFired"),				&NetVars.m_iShotsFired);
	Resolver.NetVars->GetOffset(XorStr("DT_CSPlayer"),							XorStr("m_flFlashMaxAlpha"),		&NetVars.m_flFlashMaxAlpha);
	Resolver.NetVars->GetOffset(XorStr("DT_CSPlayer"),							XorStr("m_hMyWearables"),			&NetVars.m_hMyWearables);
	Resolver.NetVars->GetOffset(XorStr("DT_PlantedC4"),						XorStr("m_flC4Blow"),					&NetVars.m_flC4Blow);
	Resolver.NetVars->GetOffset(XorStr("DT_PlantedC4"),						XorStr("m_flTimerLength"),			&NetVars.m_flTimerLength);
	Resolver.NetVars->GetOffset(XorStr("DT_BaseAnimating"),					XorStr("m_nHitboxSet"),				&NetVars.m_nHitboxSet);

	DWORD dwPresent = FindSignature(XorStr("gameoverlayrenderer.dll"), XorStr("Present"), XorStr("FF 15 ? ? ? ? 8B F8 85 DB 74 1F")) + 0x2;
	DWORD dwReset = FindSignature(XorStr("gameoverlayrenderer.dll"), XorStr("Reset"), XorStr("FF 15 ? ? ? ? 8B F8 85 FF 78 18")) + 0x2;
	auto dwDevice = **(DWORD**)(FindSignature(XorStr("shaderapidx9.dll"), XorStr("Device"), XorStr("A1 ?? ?? ?? ?? 50 8B 08 FF 51 0C")) + 1);
	Offsets.RevealRanks = FindSignature(XorStr("client.dll"), XorStr("RevealRanks"), XorStr("8B EC 8B 0D ? ? ? ? 68")) - 1;
	Offsets.LoadFromBufferEx = FindSignature(XorStr("client.dll"), XorStr("LoadFromBuffer"), XorStr("55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04"));
	Offsets.InitKeyValuesEx = FindSignature(XorStr("client.dll"), XorStr("InitKeyValues"), XorStr("55 8B EC 51 33 C0 C7 45"));
	Offsets.LineGoesThroughSmoke = FindSignature(XorStr("client.dll"), XorStr("LineGoesThroughSmoke"), XorStr("55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0"));
	Offsets.FullUpdate = FindSignature(XorStr("engine.dll"), XorStr("FullUpdate"), XorStr("A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85"));

	Offsets.worldToScreen =							  (DWORD)&Interface.Engine->WorldToScreenMatrix() + 0x40;

	G::chamsNormalMaterial = U::CreateMat(XorStr("mat_0"), false, false);
	G::ignoreNormalMaterial = U::CreateMat(XorStr("mat_1"), false, true);
	G::chamsFlatMaterial = U::CreateMat(XorStr("mat_2"), true, false);
	G::ignoreFlatMaterial = U::CreateMat(XorStr("mat_3"), true, true);


	//H::DirectX =								new CVMTHookManager((PDWORD*)dwDevice);
	H::ClientDLL =								new CVMTHookManager((PDWORD*)Interface.Client);
	H::ClientMode =								new CVMTHookManager((PDWORD*)Interface.ClientMode);
	H::ModelRender =							new CVMTHookManager((PDWORD*)Interface.ModelRender);
	H::GameEventManager =						new CVMTHookManager((PDWORD*)Interface.GameEventsManager);

	O::Present =									(PresentFn)((new CVMTHookManager((PDWORD*)dwPresent))->dwHookMethod((DWORD)&CRender::Present, 0));
	O::Reset =										(ResetFn)((new CVMTHookManager((PDWORD*)dwReset))->dwHookMethod((DWORD)&CRender::Reset, 0));
	//O::EndScene =									(EndSceneFn)H::DirectX->dwHookMethod((DWORD)&CRender::EndScene, 42);
	//O::Reset2 =										(ResetFn)H::DirectX->dwHookMethod((DWORD)&CRender::Reset2, 16);
	O::CreateMove =							(CreateMoveFn)H::ClientMode->dwHookMethod((DWORD)&CMovement::CreateMove, 24);
	O::OverrideView =							(OverrideViewFn)H::ClientMode->dwHookMethod((DWORD)&hkOverrideView, 18);
	O::GetViewModelFOV =					(GetViewModelFOVFn)H::ClientMode->dwHookMethod((DWORD)&hkGetViewModelFOV, 35);
	O::FrameStageNotify =					(FrameStageNotifyFn)H::ClientDLL->dwHookMethod((DWORD)&CFrameStageNotify::Run, 36);
	O::FireEventClientSide =					(FireEventClientSideFn)H::GameEventManager->dwHookMethod((DWORD)&CEvents::Run, 9);
	O::DrawModelExecute =					(DrawModelExecuteFn)H::ModelRender->dwHookMethod((DWORD)&CChams::Render, 21);
	

	for (ClientClass* pClass = Interface.Client->GetAllClasses(); pClass; pClass = pClass->pNextClass) {
		if (!strcmp(pClass->szName, XorStr("CBaseViewModel"))) {
			RecvTable* pClassTable = pClass->rtTable;

			for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++) {
				RecvProp* pProp = &pClassTable->m_pProps[nIndex];

				if (!pProp)
					continue;

				if (!strcmp(pProp->m_pVarName, XorStr("m_nSequence")))
				{
					H::Sequence = std::make_unique<RecvPropHook>(pProp);
					H::Sequence->SetProxyFunction((RecvVarProxyFn)&CProxy::Sequence);
				}
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////

	/*ConsoleMsg("Parsing NetVars...\n");
	for (ClientClass* pClass = Interface.Client->GetAllClasses(); pClass; pClass = pClass->pNextClass)
	{
		ConsoleMsg("%s:\n", pClass->szName);
		RecvTable* pClassTable = pClass->rtTable;
		for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++) {
			RecvProp* pProp = &pClassTable->m_pProps[nIndex];
			if (!pProp)
				continue;

			ConsoleMsg(" |__%s -> 0x%d\n", pProp->m_pVarName, (DWORD*)pProp->m_Offset);
		}
	}*/

	/////////////////////////////////////////////////////////////////////////////////////////////////////


	player_hurt_listener* hurtListener = new player_hurt_listener();
	Interface.GameEventsManager->AddListener(hurtListener, XorStr("player_hurt"), false);
	g_pOldWindowProc = (WNDPROC)SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, (LONG_PTR)CMenu::WndProc);

	Config::SetupPath();
	Config::Read();

	// Colors
	static float color1[3] = { 0, 1, 0 };
	static float color2[3] = { 1, 0, 0 };
	static float color3[3] = { 0, 1, 0 };
	static float color4[3] = { 0, 0, 0 };
	static float color5[3] = { 0, 1, 0 };
	static float color6[3] = { 1, 0, 0 };
	static float color7[3] = { 0, 0, 1 };
	static float color8[3] = { 0, 1, 0 };
	static float color9[3] = { 0, 0, 1 };
	static float color10[3] = { 0, 0, 0 };
	static float color11[3] = { 0, 0, 0 };
	static float color12[3] = { 0, 0, 0 };
	static float color13[3] = { 0, 0, 0 };
	static float color14[3] = { 0, 0, 0 };
	static float color15[3] = { 0, 0, 0 };
	static float color16[3] = { 0, 0, 0 };
	static float color17[3] = { 0, 0, 0 };
	static float color18[3] = { 0, 0, 0 };
	static float color19[3] = { 0, 0, 0 };
	static float color20[3] = { 0, 0, 0 };
	static float color21[3] = { 0, 0, 0 };
	static float color22[3] = { 0, 0, 0 };
	static float color23[3] = { 0, 0, 0 };
	static float color24[3] = { 0, 0, 0 };
	static float color25[3] = { 0, 0, 0 };
	static float color26[3] = { 0, 0, 0 };
	static float color27[3] = { 0, 0, 0 };
	static float color28[3] = { 0, 0, 0 };
	static float color29[3] = { 0, 0, 0 };
	static float color30[3] = { 0, 0, 0 };
	static float color31[3] = { 0, 0, 0 };
	G::ColorsForPicker.push_back(ColorP("Chams Visible", Options::Visuals::Chams::color));
	G::ColorsForPicker.push_back(ColorP("Chams Invisible", Options::Visuals::Chams::coloriz));
	int psr = ParseSkins();
	if (psr != 0)
	{
		printf("Failed: %d\n", psr);
		return 0;
	}

	return 1;
}

void MainThread(HMODULE hModule)
{
	if (Init(hModule) == 0)
		return;

	while (true)
	{
		if (Interface.Engine->IsInGame())
		{
			if (Options::Misc::RevealRanks && GetAsyncKeyState(VK_TAB))
						CVisuals::RevealRanks();

			CPlayer* pLocal = Interface.EntityList->GetClientEntity<CPlayer>(Interface.Engine->GetLocalPlayer());
			for (int i = 0; i < Interface.Engine->GetMaxClients(); i++)
			{
				CPlayer* pBasePlayer = Interface.EntityList->GetClientEntity<CPlayer>(i);
				if (!pBasePlayer)
					continue;

				if (Options::Radar::Enabled && Options::Radar::Style == 1)
					*(bool*)((DWORD)pBasePlayer + 0x939) = true;
			}
		}

		Sleep(200);
	}
}

bool __stdcall DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		thismodule = hModule;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, NULL, NULL, NULL);
	}
	if (dwReason == DLL_PROCESS_DETACH)
	{
		H::ClientDLL->UnHook();
		H::ClientMode->UnHook();
		H::ModelRender->UnHook();
		H::GameEventManager->UnHook();
	}

	return true;
}