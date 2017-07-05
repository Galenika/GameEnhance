#include "Cheat.h"

#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1

#define	LIFE_ALIVE 0

#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);

FrameStageNotifyFn oFrameStageNotify;

void RecvProxy_Viewmodel(const CRecvProxyData* pData, void* pStruct, void* pOut)
{
	int iModel = pData->m_Value.m_Int;

	auto nCTDefault = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_default_ct.mdl"));
	auto nBayonet = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_bayonet.mdl"));
	auto nGut = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_gut.mdl"));
	auto nTDefault = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_default_t.mdl"));
	auto nButterfly = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_butterfly.mdl"));
	auto nFlip = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_flip.mdl"));
	auto nGunGame = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_gg.mdl"));
	auto nFalchion = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_falchion_advanced.mdl"));
	auto nM9Bayonet = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_m9_bay.mdl"));
	auto nKarambit = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_karam.mdl"));
	auto nHuntsman = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_tactical.mdl"));
	auto nDagger = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_push.mdl"));
	auto nBowie = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_survival_bowie.mdl"));

	CBaseEntity *localPlayer = static_cast<CBaseEntity*>(I::ClientEntList->GetClientEntity(I::Engine->GetLocalPlayer()));
	if (localPlayer)
	{
		if (localPlayer->GetWeapon())
		{
			if (iModel == nBayonet
				|| iModel == nButterfly
				|| iModel == nCTDefault
				|| iModel == nTDefault
				|| iModel == nFlip
				|| iModel == nGut
				|| iModel == nHuntsman
				|| iModel == nGunGame
				|| iModel == nKarambit
				|| iModel == nM9Bayonet
				|| iModel == nFalchion)
			{
				//int iKnife = StopFuckinLyin.m_pConvars->flknife;
				if (Vars.Misc.KnifeModel == 1)
				{
					iModel = nBayonet;
				}
				else if (Vars.Misc.KnifeModel == 2)
				{
					iModel = nFlip;
				}
				else if (Vars.Misc.KnifeModel == 3)
				{
					iModel = nGut;
				}
				else if (Vars.Misc.KnifeModel == 4)
				{
					iModel = nKarambit;
				}
				else if (Vars.Misc.KnifeModel == 5)
				{
					iModel = nM9Bayonet;
				}
				else if (Vars.Misc.KnifeModel == 6)
				{
					iModel = nHuntsman;
				}
				else if (Vars.Misc.KnifeModel == 7)
				{
					iModel = nButterfly;
				}
				else if (Vars.Misc.KnifeModel == 8)
				{
					iModel = nFalchion;
				}
				else if (Vars.Misc.KnifeModel == 9)
				{
					iModel = nDagger;
				}
				else if (Vars.Misc.KnifeModel == 10)
				{
					iModel = nBowie;
				}
			}
		}
	}

	*static_cast<int*>(pOut) = iModel;
}

void RecvProxy_Sequence(const CRecvProxyData* pData, void* pStruct, void* pOut)
{
	int iSequence = pData->m_Value.m_Int;

	CBaseCombatWeapon* Weapon = static_cast<CBaseCombatWeapon*>(pStruct);
	CBaseEntity *localPlayer = static_cast<CBaseEntity*>(I::ClientEntList->GetClientEntity(I::Engine->GetLocalPlayer()));
	
	if (Weapon && Weapon->GetModel())
	{
		std::string rWeapon = I::ModelInfo->GetModelName(Weapon->GetModel());

		static bool bWasOpen = false;

		if (rWeapon.find(charenc("v_knife_push.mdl")) != std::string::npos)
		{
			switch (iSequence) {
			case SEQUENCE_DEFAULT_IDLE2:
				iSequence = SEQUENCE_DAGGERS_IDLE1; break;
			case SEQUENCE_DEFAULT_LIGHT_MISS1:
			case SEQUENCE_DEFAULT_LIGHT_MISS2:
				iSequence = RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5); break;
			case SEQUENCE_DEFAULT_HEAVY_MISS1:
				iSequence = RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1); break;
			case SEQUENCE_DEFAULT_HEAVY_HIT1:
			case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
			case SEQUENCE_DEFAULT_LOOKAT01:
				iSequence += 3; break;
			case SEQUENCE_DEFAULT_DRAW:
			case SEQUENCE_DEFAULT_IDLE1:
				break;
			default:
				iSequence += 2;
			}
		}
		else if (rWeapon.find(charenc("v_knife_falchion_advanced.mdl")) != std::string::npos)
		{
			switch (iSequence) {
			case SEQUENCE_DEFAULT_IDLE2:
				iSequence = SEQUENCE_FALCHION_IDLE1; break;
			case SEQUENCE_DEFAULT_HEAVY_MISS1:
				iSequence = RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP); break;
			case SEQUENCE_DEFAULT_LOOKAT01:
				iSequence = RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02); break;
			case SEQUENCE_DEFAULT_DRAW:
			case SEQUENCE_DEFAULT_IDLE1:
				break;
			default:
				iSequence--;
			}
		}
		else if (rWeapon.find(charenc("v_knife_butterfly.mdl")) != std::string::npos)
		{
			switch (iSequence) {
			case SEQUENCE_DEFAULT_DRAW:
				iSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2); break;
			case SEQUENCE_DEFAULT_LOOKAT01:
				iSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03); break;
			default:
				iSequence++;
			}
		}
		else if (rWeapon.find(charenc("v_knife_survival_bowie.mdl")) != std::string::npos)
		{
			switch (iSequence) {
			case SEQUENCE_DEFAULT_DRAW:
			case SEQUENCE_DEFAULT_IDLE1:
				break;
			case SEQUENCE_DEFAULT_IDLE2:
				iSequence = SEQUENCE_BOWIE_IDLE1; break;
			default:
				iSequence--;
			}
		}

		*static_cast<int*>(pOut) = iSequence;
	}
}


void __stdcall Hooks::FrameStageNotify( ClientFrameStage_t stage )
{
	QAngle aim_punch_old;
	QAngle view_punch_old;

	QAngle* aim_punch = nullptr;
	QAngle* view_punch = nullptr;

	if( I::Engine->IsInGame() && stage == FRAME_RENDER_START && Vars.Visuals.Removals.VisualRecoil )
	{
		if( G::LocalPlayer && G::LocalPlayer->GetAlive() )
		{
			aim_punch = ( QAngle* )( ( DWORD )G::LocalPlayer + offsets.m_aimPunchAngle );
			view_punch = ( QAngle* )( ( DWORD )G::LocalPlayer + offsets.m_viewPunchAngle );

			aim_punch_old = *aim_punch;
			view_punch_old = *view_punch;

			*aim_punch = QAngle( 0, 0, 0 );
			*view_punch = QAngle( 0, 0, 0 );
		}
	}

	oFrameStageNotify( stage );

	if( aim_punch && view_punch && Vars.Visuals.Removals.VisualRecoil )
	{
		*aim_punch = aim_punch_old;
		*view_punch = view_punch_old;
	}

	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		auto nCTDefault = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_default_ct.mdl"));
		auto nBayonet = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_bayonet.mdl"));
		auto nGut = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_gut.mdl"));
		auto nTDefault = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_default_t.mdl"));
		auto nButterfly = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_butterfly.mdl"));
		auto nFlip = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_flip.mdl"));
		auto nGunGame = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_gg.mdl"));
		auto nFalchion = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_falchion_advanced.mdl"));
		auto nM9Bayonet = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_m9_bay.mdl"));
		auto nKarambit = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_karam.mdl"));
		auto nHuntsman = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_tactical.mdl"));
		auto nDagger = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_push.mdl"));
		auto nBowie = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_survival_bowie.mdl"));

		for (int i = 0; i < I::ClientEntList->GetHighestEntityIndex(); i++)
		{
			CBaseEntity *Entityy = I::ClientEntList->GetClientEntity(i);

			if (!Entityy)
				continue;
			if (Entityy->GetOwner() == -1)
				continue;
			CBaseEntity* localowner = I::ClientEntList->GetClientEntity(I::Engine->GetLocalPlayer());
			CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Entityy;
			ULONG hOwnerEntity = *(PULONG)((DWORD)Entityy + offsets.m_hOwnerEntity);
			CBaseEntity* pOwner = I::ClientEntList->GetClientEntityFromHandle((DWORD)hOwnerEntity);

			player_info_t LocalInfo;
			LocalInfo = localowner->GetPlayerInfo();

			int weapon = pWeapon->GetItemDefinitionIndex();

			if (Vars.Misc.SkinsEnabled)
			{ 

			if (pOwner == localowner)
			{
				std::string rWeapon = I::ModelInfo->GetModelName(Entityy->GetModel());

				if (!(rWeapon.find(charenc("models/weapons"), 0) != std::string::npos))
					continue;

				if (rWeapon.find(charenc("c4_planted"), 0) != std::string::npos)
					continue;

				if (rWeapon.find(charenc("thrown"), 0) != std::string::npos)
					continue;

				if (rWeapon.find(charenc("smokegrenade"), 0) != std::string::npos)
					continue;

				if (rWeapon.find(charenc("flashbang"), 0) != std::string::npos)
					continue;

				if (rWeapon.find(charenc("fraggrenade"), 0) != std::string::npos)
					continue;

				if (rWeapon.find(charenc("molotov"), 0) != std::string::npos)
					continue;

				if (rWeapon.find(charenc("decoy"), 0) != std::string::npos)
					continue;

				if (rWeapon.find(charenc("incendiarygrenade"), 0) != std::string::npos)
					continue;

				if (rWeapon.find(charenc("ied"), 0) != std::string::npos)
					continue;

				if (rWeapon.find(charenc("w_eq_"), 0) != std::string::npos)
					continue;

			//	U::NetVars->HookProp(charenc("DT_BaseViewModel"), charenc("m_nModelIndex"), RecvProxy_Viewmodel);
			//	U::NetVars->HookProp(charenc("DT_BaseViewModel"), charenc("m_nSequence"), RecvProxy_Sequence);

				switch (weapon)
				{ 
				case 9: // AWP
				{
					switch (Vars.Misc.SkinAWP)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 174;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 344;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 84;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 30;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 51;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 72;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 181;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 259;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 395;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 212;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 227;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 251;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 279;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 424;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 446;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 451;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 475;
						break;
					}
				}
				break;
				case 7:
				{
					switch (Vars.Misc.SkinAK)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 371;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 14;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 22;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 44;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 72;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 122;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 282;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 170;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 172;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 180;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 394;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 300;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 226;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 302;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 316;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 340;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 380;
						break;
					case 17:
						pWeapon->GetFallbackPaintKit() = 422;
						break;
					case 18:
						pWeapon->GetFallbackPaintKit() = 456;
						break;
					case 19:
						pWeapon->GetFallbackPaintKit() = 474;
						break;
					}
				}
				break;
				case 1:
				{			
					switch (Vars.Misc.SkinDeagle)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 37;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 347;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 468;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 469;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 5;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 12;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 17;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 40;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 61;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 90;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 235;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 185;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 248;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 231;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 237;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 397;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 328;
						break;
					case 17:
						pWeapon->GetFallbackPaintKit() = 273;
						break;
					case 18:
						pWeapon->GetFallbackPaintKit() = 296;
						break;
					case 19:
						pWeapon->GetFallbackPaintKit() = 351;
						break;
					case 20:
						pWeapon->GetFallbackPaintKit() = 425;
						break;
					case 21:
						pWeapon->GetFallbackPaintKit() = 470;
						break;
					}
					
				}
				break;
				case 60:
				{
					switch (Vars.Misc.SkinM4A1S)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 59;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 33;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 60;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 430;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 77;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 235;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 254;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 189;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 301;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 217;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 257;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 321;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 326;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 360;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 383;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 440;
						break;
					}
				}
				break;
				case 8:
				{
					switch (Vars.Misc.SkinAug)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 73;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 10;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 9;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 28;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 167;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 110;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 33;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 100;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 46;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 47;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 280;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 305;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 442;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 444;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 455;
						break;
					}

				}
				break;
				case 24:
				{
					switch (Vars.Misc.SkinUMP)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 37;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 5;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 15;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 17;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 436;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 70;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 93;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 169;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 175;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 193;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 392;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 281;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 333;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 362;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 441;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 488;
						break;
					}
				}
				break;
				case 17:
				{
					switch (Vars.Misc.SkinMac10)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 101;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 3;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 32;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 5;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 17;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 38;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 433;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 98;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 157;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 188;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 337;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 246;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 284;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 310;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 333;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 343;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 372;
						break;
					case 17:
						pWeapon->GetFallbackPaintKit() = 402;
						break;
					}
				}
				break;
				case 30:
				{
					switch (Vars.Misc.SkinTec9)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 101;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 2;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 5;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 463;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 17;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 36;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 439;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 159;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 168;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 235;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 179;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 248;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 206;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 216;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 242;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 272;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 289;
						break;
					case 17:
						pWeapon->GetFallbackPaintKit() = 303;
						break;
					case 18:
						pWeapon->GetFallbackPaintKit() = 374;
						break;
					case 19:
						pWeapon->GetFallbackPaintKit() = 459;
						break;
					}
				}
				break;
				case 39:
				{
					switch (Vars.Misc.SkinSG556)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 101;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 28;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 22;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 27;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 39;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 98;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 136;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 410;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 169;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 186;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 243;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 247;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 287;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 298;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 363;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 378;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 487;
						break;
					}
				}
				break;
				case 16:
				{
					switch (Vars.Misc.SkinM4A4)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 8;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 101;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 5;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 167;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 164;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 16;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 17;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 155;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 170;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 176;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 187;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 255;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 39;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 215;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 336;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 384;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 400;
						break;
					case 17:
						pWeapon->GetFallbackPaintKit() = 449;
						break;
					case 18:
						pWeapon->GetFallbackPaintKit() = 471;
						break;
					case 19:
						pWeapon->GetFallbackPaintKit() = 480;
						break;
					}
				}
				break;
				case 11:
				{
					switch (Vars.Misc.SkinG3SG1)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 8;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 6;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 27;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 46;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 72;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 74;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 147;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 235;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 170;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 195;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 229;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 294;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 465;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 464;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 382;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 438;
						break;
					}
				}
				break;
				case 27:
				{
					switch (Vars.Misc.SkinMAG7)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 462;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 34;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 32;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 100;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 39;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 431;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 99;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 171;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 177;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 198;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 291;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 385;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 473;
						break;
					}
				}
				break;
				case 10:
				{
					switch (Vars.Misc.SkinFamas)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 22;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 47;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 92;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 429;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 154;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 178;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 194;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 244;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 218;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 260;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 288;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 371;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 477;
						break;
					}
				}
				break;
				case 14:
				{
					switch (Vars.Misc.Skin249)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 5;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 22;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 75;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 202;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 4;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 266;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 41;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 452;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 472;
						break;
					}
				}
				break;
				case 63:
				{
					switch (Vars.Misc.SkinCZ)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 435;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 12;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 254;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 218;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 268;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 269;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 270;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 297;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 298;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 315;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 322;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 325;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 334;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 350;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 366;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 453;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 476;
						break;
					}
				}
				break;
				case 32:
				{
					switch (Vars.Misc.Skinp2000)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 104;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 32;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 21;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 25;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 36;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 385;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 38;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 71;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 95;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 184;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 211;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 338;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 246;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 275;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 327;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 346;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 357;
						break;
					case 17:
						pWeapon->GetFallbackPaintKit() = 389;
						break;
					case 18:
						pWeapon->GetFallbackPaintKit() = 442;
						break;
					case 19:
						pWeapon->GetFallbackPaintKit() = 443;
						break;
					}
				}
				break;
				case 29:
				{
					switch (Vars.Misc.SkinSawedOff)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 345;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 5;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 22;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 30;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 83;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 38;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 41;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 434;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 119;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 235;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 171;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 204;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 405;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 246;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 250;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 390;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 256;
						break;
					case 17:
						pWeapon->GetFallbackPaintKit() = 323;
						break;
					case 18:
						pWeapon->GetFallbackPaintKit() = 458;
						break;
					case 19:
						pWeapon->GetFallbackPaintKit() = 459;
						break;
					}
				}
				break;
				case 4:
				{
					switch (Vars.Misc.SkinGlock)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 2;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 3;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 38;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 40;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 48;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 337;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 99;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 159;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 399;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 208;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 230;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 278;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 293;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 353;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 367;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 381;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 479;
						break;
					}
				}
				break;
				case 13:
				{
					switch (Vars.Misc.SkinGalil)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 5;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 22;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 83;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 428;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 76;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 119;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 235;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 398;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 192;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 308;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 216;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 237;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 241;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 264;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 297;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 379;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 460;
						break;
					case 17:
						pWeapon->GetFallbackPaintKit() = 478;
						break;
					}
				}
				break;
				case 33:
				{
					switch (Vars.Misc.SkinMP7)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 2;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 102;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 5;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 28;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 11;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 15;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 22;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 27;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 36;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 141;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 235;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 245;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 209;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 213;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 250;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 354;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 365;
						break;
					case 17:
						pWeapon->GetFallbackPaintKit() = 423;
						break;
					case 18:
						pWeapon->GetFallbackPaintKit() = 472;
						break;
					case 19:
						pWeapon->GetFallbackPaintKit() = 481;
						break;
					}
				}
				break;
				case 36:
				{
					switch (Vars.Misc.Skinp250)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 102;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 34;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 162;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 15;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 164;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 27;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 77;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 99;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 168;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 258;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 207;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 219;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 404;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 230;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 271;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 290;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 464;
						break;
					case 17:
						pWeapon->GetFallbackPaintKit() = 358;
						break;
					case 18:
						pWeapon->GetFallbackPaintKit() = 373;
						break;
					case 19:
						pWeapon->GetFallbackPaintKit() = 388;
						break;
					case 20:
						pWeapon->GetFallbackPaintKit() = 426;
						break;
					case 21:
						pWeapon->GetFallbackPaintKit() = 466;
						break;
					case 22:
						pWeapon->GetFallbackPaintKit() = 467;
						break;
					}
				}
				break;
				case 3:
				{
					switch (Vars.Misc.Skinf7)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 3;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 27;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 44;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 46;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 78;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 141;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 151;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 254;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 248;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 210;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 223;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 252;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 265;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 274;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 464;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 352;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 377;
						break;
					case 17:
						pWeapon->GetFallbackPaintKit() = 387;
						break;
					case 18:
						pWeapon->GetFallbackPaintKit() = 427;
						break;
					}
				}
				break;
				case 35:
				{
					switch (Vars.Misc.SkinNova)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 3;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 166;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 164;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 25;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 62;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 99;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 107;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 158;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 170;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 191;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 214;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 225;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 263;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 286;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 294;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 299;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 356;
						break;
					case 17:
						pWeapon->GetFallbackPaintKit() = 350;
						break;
					case 18:
						pWeapon->GetFallbackPaintKit() = 484;
						break;
					}
				}
				break;
				case 28:
				{
					switch (Vars.Misc.SkinNegev)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 28;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 432;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 157;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 335;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 201;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 240;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 285;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 298;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 317;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 355;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 369;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 483;
						break;
					}
				}
				break;
				case 19:
				{
					switch (Vars.Misc.SkinP90)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 342;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 22;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 100;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 67;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 111;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 124;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 156;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 234;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 169;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 175;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 182;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 244;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 228;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 283;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 311;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 335;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 359;
						break;
					case 17:
						pWeapon->GetFallbackPaintKit() = 486;
						break;
					}
				}
				break;
				case 38:
				{
					switch (Vars.Misc.SkinScar20)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 12;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 165;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 100;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 46;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 70;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 116;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 157;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 196;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 232;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 391;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 298;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 312;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 406;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 453;
						break;
					}
				}
				break;
				case 34:
				{
					switch (Vars.Misc.SkinMp9)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 482;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 27;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 33;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 100;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 39;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 61;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 148;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 141;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 199;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 329;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 262;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 366;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 368;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 386;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 403;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 448;
						break;

					}
				}
				break;
				case 2:
				{
					switch (Vars.Misc.SkinElite)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 28;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 36;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 43;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 46;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 47;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 153;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 190;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 248;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 249;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 220;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 396;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 261;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 276;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 307;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 330;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 447;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 450;
						break;
					}
				}
				break;
				case 26:
				{
					switch (Vars.Misc.SkinBizon)
						{
						case 0:
							pWeapon->GetFallbackPaintKit() = 13;
							break;
						case 1:
							pWeapon->GetFallbackPaintKit() = 164;
							break;
						case 2:
							pWeapon->GetFallbackPaintKit() = 25;
							break;
						case 3:
							pWeapon->GetFallbackPaintKit() = 27;
							break;
						case 4:
							pWeapon->GetFallbackPaintKit() = 70;
							break;
						case 5:
							pWeapon->GetFallbackPaintKit() = 148;
							break;
						case 6:
							pWeapon->GetFallbackPaintKit() = 149;
							break;
						case 7:
							pWeapon->GetFallbackPaintKit() = 159;
							break;
						case 8:
							pWeapon->GetFallbackPaintKit() = 235;
							break;
						case 9:
							pWeapon->GetFallbackPaintKit() = 171;
							break;
						case 10:
							pWeapon->GetFallbackPaintKit() = 203;
							break;
						case 11:
							pWeapon->GetFallbackPaintKit() = 224;
							break;
						case 12:
							pWeapon->GetFallbackPaintKit() = 236;
							break;
						case 13:
							pWeapon->GetFallbackPaintKit() = 267;
							break;
						case 14:
							pWeapon->GetFallbackPaintKit() = 306;
							break;
						case 15:
							pWeapon->GetFallbackPaintKit() = 323;
							break;
						case 16:
							pWeapon->GetFallbackPaintKit() = 349;
							break;
						case 17:
							pWeapon->GetFallbackPaintKit() = 376;
							break;
						case 18:
							pWeapon->GetFallbackPaintKit() = 457;
							break;
						case 19:
							pWeapon->GetFallbackPaintKit() = 459;
							break;
						}
				}
				break;
				case 25:
				{
					switch (Vars.Misc.SkinXM)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 166;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 238;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 27;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 42;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 96;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 95;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 135;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 151;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 235;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 169;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 205;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 240;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 251;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 393;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 320;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 314;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 348;
						break;
					case 17:
						pWeapon->GetFallbackPaintKit() = 370;
						break;
					case 18:
						pWeapon->GetFallbackPaintKit() = 407;
						break;
					}
				}
				break;
				case 61:
				{
					switch (Vars.Misc.SkinUsp)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 59;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 25;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 60;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 235;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 183;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 339;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 217;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 221;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 236;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 277;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 290;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 313;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 318;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 332;
						break;
					case 14:
						pWeapon->GetFallbackPaintKit() = 364;
						break;
					case 15:
						pWeapon->GetFallbackPaintKit() = 454;
						break;
					case 16:
						pWeapon->GetFallbackPaintKit() = 489;
						break;
					}
				}	
				break;
				case 40:
				{
					switch (Vars.Misc.SkinSSG08)
					{
					case 0:
						pWeapon->GetFallbackPaintKit() = 59;
						break;
					case 1:
						pWeapon->GetFallbackPaintKit() = 26;
						break;
					case 2:
						pWeapon->GetFallbackPaintKit() = 38;
						break;
					case 3:
						pWeapon->GetFallbackPaintKit() = 60;
						break;
					case 4:
						pWeapon->GetFallbackPaintKit() = 96;
						break;
					case 5:
						pWeapon->GetFallbackPaintKit() = 99;
						break;
					case 6:
						pWeapon->GetFallbackPaintKit() = 157;
						break;
					case 7:
						pWeapon->GetFallbackPaintKit() = 200;
						break;
					case 8:
						pWeapon->GetFallbackPaintKit() = 222;
						break;
					case 9:
						pWeapon->GetFallbackPaintKit() = 233;
						break;
					case 10:
						pWeapon->GetFallbackPaintKit() = 253;
						break;
					case 11:
						pWeapon->GetFallbackPaintKit() = 304;
						break;
					case 12:
						pWeapon->GetFallbackPaintKit() = 319;
						break;
					case 13:
						pWeapon->GetFallbackPaintKit() = 361;
						break;
					}
				}		
				break;

				}
				
				pWeapon->GetItemIDHigh() = -1;
			}
			}
				
		}
	}	
}


/*
void __stdcall Hooks::FrameStageNotify(ClientFrameStage_t curStage)
{
	QAngle aimold;
	QAngle viewold;
	QAngle* aim = NULL;
	QAngle* view = NULL;
	if (I::Engine->IsConnected() && I::Engine->IsInGame())
	{
		if (FRAME_RENDER_START)
		{
			auto LocalPlayer = static_cast<CBaseEntity*>(I::ClientEntList->GetClientEntity(I::Engine->GetLocalPlayer()));
			if (!LocalPlayer)
				return;
			if (LocalPlayer->GetAlive())
			{
				if (Vars.Visuals.Removals.VisualRecoil)
				{

					aim = reinterpret_cast<QAngle*>(DWORD(G::LocalPlayer) + offsets.m_aimPunchAngle);
					view = reinterpret_cast<QAngle*>(DWORD(G::LocalPlayer) + offsets.m_viewPunchAngle);
					aimold = *aim;
					viewold = *view;
					*aim = QAngle(0, 0, 0);
					*view = QAngle(0, 0, 0);

				}
			}
		}

		if (FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		{
			
			if (Vars.Misc.SkinsEnabled)
			{
				auto nCTDefault = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_default_ct.mdl"));
				auto nBayonet = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_bayonet.mdl"));
				auto nGut = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_gut.mdl"));
				auto nTDefault = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_default_t.mdl"));
				auto nButterfly = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_butterfly.mdl"));
				auto nFlip = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_flip.mdl"));
				auto nGunGame = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_gg.mdl"));
				auto nFalchion = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_falchion_advanced.mdl"));
				auto nM9Bayonet = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_m9_bay.mdl"));
				auto nKarambit = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_karam.mdl"));
				auto nHuntsman = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_tactical.mdl"));
				auto nDagger = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_push.mdl"));
				auto nBowie = I::ModelInfo->GetModelIndex(charenc("models/weapons/v_knife_survival_bowie.mdl"));

				CBaseEntity *pLocal = I::ClientEntList->GetClientEntity(I::Engine->GetLocalPlayer());

				for (int i = I::ClientEntList->GetHighestEntityIndex(); i >= 0; --i)
				{
					CBaseEntity* pEntity = I::ClientEntList->GetClientEntity(i);
					if (pEntity)
					{
						ULONG hOwnerEntity = *reinterpret_cast<PULONG>(DWORD(pEntity) + 0x148);

						CBaseEntity* pOwner = I::ClientEntList->GetClientEntityFromHandle(hOwnerEntity);

						if (pOwner)
						{
							if (pOwner == pLocal)
							{

								std::string rWeapon = I::ModelInfo->GetModelName(pEntity->GetModel());

								if (!(rWeapon.find(charenc("models/weapons"), 0) != std::string::npos))
									continue;

								if (rWeapon.find(charenc("c4_planted"), 0) != std::string::npos)
									continue;

								if (rWeapon.find(charenc("thrown"), 0) != std::string::npos)
									continue;

								if (rWeapon.find(charenc("smokegrenade"), 0) != std::string::npos)
									continue;

								if (rWeapon.find(charenc("flashbang"), 0) != std::string::npos)
									continue;

								if (rWeapon.find(charenc("fraggrenade"), 0) != std::string::npos)
									continue;

								if (rWeapon.find(charenc("molotov"), 0) != std::string::npos)
									continue;

								if (rWeapon.find(charenc("decoy"), 0) != std::string::npos)
									continue;

								if (rWeapon.find(charenc("incendiarygrenade"), 0) != std::string::npos)
									continue;

								if (rWeapon.find(charenc("ied"), 0) != std::string::npos)
									continue;

								if (rWeapon.find(charenc("w_eq_"), 0) != std::string::npos)
									continue;

								CBaseCombatWeapon* pWeapon = reinterpret_cast<CBaseCombatWeapon*>(pEntity);

								ClientClass *pClass = I::Client->GetAllClasses();

							//	U::NetVars->HookProp(charenc("DT_BaseViewModel"), charenc("m_nModelIndex"), RecvProxy_Viewmodel);
							//	U::NetVars->HookProp(charenc("DT_BaseViewModel"), charenc("m_nSequence"), RecvProxy_Sequence);

								player_info_t LocalInfo;
								LocalInfo = pLocal->GetPlayerInfo();

								auto weaponID = *reinterpret_cast<int*>(reinterpret_cast<DWORD>(pWeapon) + offsets.m_iItemDefinitionIndex);

								int M41S = Vars.Misc.SkinM4A1S;
								int M4A4 = Vars.Misc.SkinM4A4;
								int AK47 = Vars.Misc.SkinAK;
								int AWP = Vars.Misc.SkinAWP;
								int GLOCK = Vars.Misc.SkinGlock;
								int USPS = Vars.Misc.SkinUsp;
								int DEAGLE = Vars.Misc.SkinDeagle;
								int FIVE7 = Vars.Misc.Skinf7;
								int AUG = Vars.Misc.SkinAug;
								int FAMAS = Vars.Misc.SkinFamas;
								int G3SG1 = Vars.Misc.SkinG3SG1;
								int Galil = Vars.Misc.SkinGalil;
								int M249 = Vars.Misc.Skin249;
								int MAC10 = Vars.Misc.SkinMac10;
								int P90 = Vars.Misc.SkinP90;
								int UMP45 = Vars.Misc.SkinUMP;
								int XM1014 = Vars.Misc.SkinXM;
								int BIZON = Vars.Misc.SkinBizon;
								int MAG7 = Vars.Misc.SkinMAG7;
								int NEGEV = Vars.Misc.SkinNegev;
								int SAWEDOFF = Vars.Misc.SkinSawedOff;
								int TEC9 = Vars.Misc.SkinTec9;
								int P2000 = Vars.Misc.Skinp2000;
								int MP7 = Vars.Misc.SkinMP7;
								int MP9 = Vars.Misc.SkinMp9;
								int NOVA = Vars.Misc.SkinNova;
								int P250 = Vars.Misc.Skinp250;
								int SCAR20 = Vars.Misc.SkinScar20;
								int SG553 = Vars.Misc.SkinSG556;
								int SSG08 = Vars.Misc.SkinSSG08;
								int Magnum = Vars.Misc.SkinR8;

								int weapon = pWeapon->GetItemDefinitionIndex();





								switch (weaponID)
								{
								case 9: // AWP
								{
									switch (Vars.Misc.SkinAWP)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 174;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 344;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 84;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 30;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 51;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 72;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 181;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 259;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 395;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 212;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 227;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 251;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 279;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 424;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 446;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 451;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 475;
										break;
									}
								}
								break;
								case 7:
								{
									switch (Vars.Misc.SkinAK)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 371;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 14;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 22;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 44;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 72;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 122;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 282;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 170;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 172;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 180;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 394;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 300;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 226;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 302;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 316;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 340;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 380;
										break;
									case 17:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 422;
										break;
									case 18:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 456;
										break;
									case 19:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 474;
										break;
									}
								}
								break;
								case 1:
								{
									switch (Vars.Misc.SkinDeagle)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 37;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 347;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 468;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 469;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 5;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 12;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 17;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 40;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 61;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 90;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 235;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 185;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 248;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 231;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 237;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 397;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 328;
										break;
									case 17:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 273;
										break;
									case 18:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 296;
										break;
									case 19:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 351;
										break;
									case 20:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 425;
										break;
									case 21:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 470;
										break;
									}

								}
								break;
								case 60:
								{
									switch (Vars.Misc.SkinM4A1S)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 59;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 33;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 60;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 430;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 77;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 235;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 254;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 189;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 301;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 217;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 257;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 321;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 326;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 360;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 383;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 440;
										break;
									}
								}
								break;
								case 8:
								{
									switch (Vars.Misc.SkinAug)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 73;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 10;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 9;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 28;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 167;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 110;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 33;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 100;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 46;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 47;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 280;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 305;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 442;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 444;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 455;
										break;
									}

								}
								break;
								case 24:
								{
									switch (Vars.Misc.SkinUMP)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 37;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 5;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 15;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 17;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 436;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 70;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 93;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 169;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 175;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 193;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 392;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 281;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 333;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 362;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 441;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 488;
										break;
									}
								}
								break;
								case 17:
								{
									switch (Vars.Misc.SkinMac10)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 101;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 3;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 32;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 5;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 17;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 38;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 433;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 98;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 157;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 188;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 337;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 246;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 284;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 310;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 333;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 343;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 372;
										break;
									case 17:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 402;
										break;
									}
								}
								break;
								case 30:
								{
									switch (Vars.Misc.SkinTec9)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 101;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 2;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 5;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 463;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 17;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 36;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 439;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 159;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 168;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 235;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 179;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 248;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 206;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 216;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 242;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 272;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 289;
										break;
									case 17:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 303;
										break;
									case 18:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 374;
										break;
									case 19:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 459;
										break;
									}
								}
								break;
								case 39:
								{
									switch (Vars.Misc.SkinSG556)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 101;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 28;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 22;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 27;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 39;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 98;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 136;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 410;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 169;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 186;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 243;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 247;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 287;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 298;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 363;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 378;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 487;
										break;
									}
								}
								break;
								case 16:
								{
									switch (Vars.Misc.SkinM4A4)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 8;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 101;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 5;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 167;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 164;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 16;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 17;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 155;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 170;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 176;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 187;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 255;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 39;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 215;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 336;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 384;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 400;
										break;
									case 17:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 449;
										break;
									case 18:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 471;
										break;
									case 19:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 480;
										break;
									}
								}
								break;
								case 11:
								{
									switch (Vars.Misc.SkinG3SG1)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 8;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 6;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 27;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 46;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 72;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 74;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 147;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 235;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 170;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 195;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 229;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 294;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 465;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 464;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 382;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 438;
										break;
									}
								}
								break;
								case 27:
								{
									switch (Vars.Misc.SkinMAG7)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 462;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 34;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 32;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 100;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 39;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 431;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 99;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 171;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 177;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 198;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 291;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 385;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 473;
										break;
									}
								}
								break;
								case 10:
								{
									switch (Vars.Misc.SkinFamas)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 22;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 47;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 92;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 429;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 154;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 178;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 194;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 244;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 218;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 260;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 288;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 371;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 477;
										break;
									}
								}
								break;
								case 14:
								{
									switch (Vars.Misc.Skin249)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 5;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 22;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 75;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 202;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 4;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 266;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 41;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 452;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 472;
										break;
									}
								}
								break;
								case 63:
								{
									switch (Vars.Misc.SkinCZ)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 435;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 12;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 254;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 218;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 268;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 269;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 270;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 297;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 298;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 315;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 322;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 325;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 334;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 350;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 366;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 453;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 476;
										break;
									}
								}
								break;
								case 32:
								{
									switch (Vars.Misc.Skinp2000)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 104;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 32;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 21;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 25;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 36;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 385;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 38;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 71;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 95;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 184;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 211;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 338;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 246;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 275;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 327;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 346;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 357;
										break;
									case 17:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 389;
										break;
									case 18:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 442;
										break;
									case 19:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 443;
										break;
									}
								}
								break;
								case 29:
								{
									switch (Vars.Misc.SkinSawedOff)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 345;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 5;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 22;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 30;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 83;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 38;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 41;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 434;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 119;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 235;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 171;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 204;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 405;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 246;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 250;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 390;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 256;
										break;
									case 17:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 323;
										break;
									case 18:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 458;
										break;
									case 19:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 459;
										break;
									}
								}
								break;
								case 4:
								{
									switch (Vars.Misc.SkinGlock)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 2;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 3;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 38;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 40;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 48;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 337;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 99;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 159;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 399;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 208;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 230;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 278;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 293;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 353;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 367;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 381;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 479;
										break;
									}
								}
								break;
								case 13:
								{
									switch (Vars.Misc.SkinGalil)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 5;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 22;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 83;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 428;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 76;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 119;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 235;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 398;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 192;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 308;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 216;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 237;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 241;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 264;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 297;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 379;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 460;
										break;
									case 17:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 478;
										break;
									}
								}
								break;
								case 33:
								{
									switch (Vars.Misc.SkinMP7)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 2;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 102;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 5;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 28;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 11;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 15;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 22;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 27;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 36;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 141;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 235;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 245;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 209;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 213;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 250;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 354;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 365;
										break;
									case 17:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 423;
										break;
									case 18:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 472;
										break;
									case 19:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 481;
										break;
									}
								}
								break;
								case 36:
								{
									switch (Vars.Misc.Skinp250)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 102;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 34;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 162;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 15;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 164;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 27;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 77;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 99;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 168;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 258;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 207;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 219;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 404;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 230;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 271;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 290;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 464;
										break;
									case 17:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 358;
										break;
									case 18:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 373;
										break;
									case 19:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 388;
										break;
									case 20:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 426;
										break;
									case 21:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 466;
										break;
									case 22:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 467;
										break;
									}
								}
								break;
								case 3:
								{
									switch (Vars.Misc.Skinf7)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 3;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 27;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 44;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 46;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 78;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 141;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 151;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 254;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 248;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 210;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 223;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 252;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 265;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 274;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 464;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 352;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 377;
										break;
									case 17:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 387;
										break;
									case 18:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 427;
										break;
									}
								}
								break;
								case 35:
								{
									switch (Vars.Misc.SkinNova)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 3;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 166;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 164;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 25;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 62;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 99;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 107;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 158;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 170;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 191;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 214;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 225;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 263;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 286;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 294;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 299;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 356;
										break;
									case 17:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 350;
										break;
									case 18:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 484;
										break;
									}
								}
								break;
								case 28:
								{
									switch (Vars.Misc.SkinNegev)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 28;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 432;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 157;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 335;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 201;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 240;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 285;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 298;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 317;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 355;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 369;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 483;
										break;
									}
								}
								break;
								case 19:
								{
									switch (Vars.Misc.SkinP90)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 342;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 22;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 100;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 67;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 111;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 124;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 156;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 234;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 169;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 175;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 182;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 244;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 228;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 283;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 311;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 335;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 359;
										break;
									case 17:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 486;
										break;
									}
								}
								break;
								case 38:
								{
									switch (Vars.Misc.SkinScar20)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 12;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 165;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 100;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 46;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 70;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 116;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 157;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 196;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 232;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 391;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 298;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 312;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 406;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 453;
										break;
									}
								}
								break;
								case 34:
								{
									switch (Vars.Misc.SkinMp9)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 482;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 27;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 33;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 100;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 39;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 61;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 148;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 141;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 199;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 329;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 262;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 366;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 368;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 386;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 403;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 448;
										break;

									}
								}
								break;
								case 2:
								{
									switch (Vars.Misc.SkinElite)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 28;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 36;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 43;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 46;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 47;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 153;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 190;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 248;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 249;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 220;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 396;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 261;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 276;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 307;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 330;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 447;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 450;
										break;
									}
								}
								break;
								case 26:
								{
									switch (Vars.Misc.SkinBizon)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 13;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 164;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 25;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 27;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 70;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 148;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 149;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 159;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 235;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 171;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 203;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 224;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 236;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 267;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 306;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 323;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 349;
										break;
									case 17:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 376;
										break;
									case 18:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 457;
										break;
									case 19:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 459;
										break;
									}
								}
								break;
								case 25:
								{
									switch (Vars.Misc.SkinXM)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 166;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 238;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 27;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 42;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 96;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 95;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 135;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 151;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 235;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 169;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 205;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 240;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 251;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 393;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 320;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 314;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 348;
										break;
									case 17:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 370;
										break;
									case 18:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 407;
										break;
									}
								}
								break;
								case 61:
								{
									switch (Vars.Misc.SkinUsp)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 59;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 25;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 60;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 235;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 183;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 339;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 217;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 221;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 236;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 277;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 290;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 313;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 318;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 332;
										break;
									case 14:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 364;
										break;
									case 15:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 454;
										break;
									case 16:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 489;
										break;
									}
								}
								break;
								case 40:
								{
									switch (Vars.Misc.SkinSSG08)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 59;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 26;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 38;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 60;
										break;
									case 4:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 96;
										break;
									case 5:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 99;
										break;
									case 6:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 157;
										break;
									case 7:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 200;
										break;
									case 8:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 222;
										break;
									case 9:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 233;
										break;
									case 10:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 253;
										break;
									case 11:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 304;
										break;
									case 12:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 319;
										break;
									case 13:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 361;
										break;
									}
								}
								break;
								case 41:
								{
									switch (Vars.Misc.SkinR8)
									{
									case 0:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 27;
										break;
									case 1:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 12;
										break;
									case 2:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 522;
										break;
									case 3:
										*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = 523;
										break;
									default:
										break;
									}

								}
								}


								if (weaponID == 59
									|| weaponID == 500
									|| weaponID == 42
									|| weaponID == 507
									|| weaponID == 506
									|| weaponID == 508
									|| weaponID == 509
									|| weaponID == 515
									|| weaponID == 516
									|| weaponID == 505
									|| weaponID == 512)
								{
									ChangeKnifeColor(pWeapon, nBayonet, nFlip, nGut, nKarambit, nM9Bayonet, nHuntsman, nButterfly, nFalchion, nDagger, nBowie);

								}
								*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_OriginalOwnerXuidHigh) = LocalInfo.xuidhigh;
								*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_OriginalOwnerXuidLow) = LocalInfo.xuidlow;
								*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iAccountID) = LocalInfo.xuidlow;
								*reinterpret_cast<float*>(DWORD(pWeapon) + offsets.m_flFallbackWear) = float(0.000000002f);
								
								*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iItemIDHigh) = -1;
							}
						}
					}
				}
			}
		}
	}

	oFrameStageNotify(curStage);
	if (aim && view && Vars.Visuals.Removals.VisualRecoil)
	{
		*aim = aimold;
		*view = viewold;
	}
}
*/