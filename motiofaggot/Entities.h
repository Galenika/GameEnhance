#pragma once

#include "MiscDefinitions.h"
#include "ClientRecvProps.h"
#include "Vector.h"
#include "MathFunctions.h"
#include "offsets.h"
#include "Header.h"
#include <unordered_map>
#include <map>
#define TEAM_CS_T 2
#define TEAM_CS_CT 3

#define BONE_USED_BY_HITBOX			0x00000100
typedef unsigned long long VPANEL;
typedef unsigned long long HFont;

class IClientRenderable;
class IClientNetworkable;
class IClientUnknown;
class IClientThinkable;
class IClientEntity;
class CSWeaponInfo;
typedef CSWeaponInfo& (__thiscall* GetCSWpnDataFn)(void*);

enum class ObserverMode_t : int
{
	OBS_MODE_NONE = 0,
	OBS_MODE_DEATHCAM = 1,
	OBS_MODE_FREEZECAM = 2,
	OBS_MODE_FIXED = 3,
	OBS_MODE_IN_EYE = 4,
	OBS_MODE_CHASE = 5,
	OBS_MODE_ROAMING = 6

};

enum CSWeaponType
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};




class CHudTexture
{
public:
	char type[64]; //0x0000
	char subtype[64]; //0x0040
	char unknowndata00[2]; //0x0080
	char charinFont; //0x0082
	char unknowndata01[1]; //0x0083
};//Size=0x00AC

class CSWeaponInfo
{

public:
	char _0x0000[2048];
	__int32 ammo_type_maybe; //0x0800
	char pad_0x0804[52];
	__int32 weapon_type; //0x0838
	__int32 weapon_team; //0x083C
	__int32 weapon_id; //0x0840
	char _0x0844[4];

	float heat_ps; //0x0848
	__int32 price; //0x084C
	float m_flArmorRatio; //0x0850
	float player_speed; //0x0854
	float player_speed_alt; //0x0858
	__int32 crosshair_min_distance; //0x085C
	__int32 crosshair_delta; //0x0860
	float m_flPenetration; //0x0864
	__int32 m_iDamage; //0x0868
	float m_flRange; //0x086C
	float m_flRangeModifier; //0x0870
	__int32 bullets; //0x0874
	float cycle_time; //0x0878
	float cycle_time_alt; //0x087C
	char _0x0880[416];
	float spread; //0x0A20
	float spread_alt; //0x0A24

};//Size=0x1040

enum class CSGOClassID
{
	CTestTraceline = 194,
	CTEWorldDecal = 195,
	CTESpriteSpray = 192,
	CTESprite = 191,
	CTESparks = 190,
	CTESmoke = 189,
	CTEShowLine = 187,
	CTEProjectedDecal = 184,
	CFEPlayerDecal = 61,
	CTEPlayerDecal = 183,
	CTEPhysicsProp = 180,
	CTEParticleSystem = 179,
	CTEMuzzleFlash = 178,
	CTELargeFunnel = 176,
	CTEKillPlayerAttachments = 175,
	CTEImpact = 174,
	CTEGlowSprite = 173,
	CTEShatterSurface = 186,
	CTEFootprintDecal = 170,
	CTEFizz = 169,
	CTEExplosion = 167,
	CTEEnergySplash = 166,
	CTEEffectDispatch = 165,
	CTEDynamicLight = 164,
	CTEDecal = 162,
	CTEClientProjectile = 161,
	CTEBubbleTrail = 160,
	CTEBubbles = 159,
	CTEBSPDecal = 158,
	CTEBreakModel = 157,
	CTEBloodStream = 156,
	CTEBloodSprite = 155,
	CTEBeamSpline = 154,
	CTEBeamRingPoint = 153,
	CTEBeamRing = 152,
	CTEBeamPoints = 151,
	CTEBeamLaser = 150,
	CTEBeamFollow = 149,
	CTEBeamEnts = 148,
	CTEBeamEntPoint = 147,
	CTEBaseBeam = 146,
	CTEArmorRicochet = 145,
	CTEMetalSparks = 177,
	CSteamJet = 140,
	CSmokeStack = 133,
	DustTrail = 244,
	CFireTrail = 64,
	SporeTrail = 250,
	SporeExplosion = 249,
	RocketTrail = 247,
	SmokeTrail = 248,
	CPropVehicleDriveable = 120,
	ParticleSmokeGrenade = 246,
	CParticleFire = 99,
	MovieExplosion = 245,
	CTEGaussExplosion = 172,
	CEnvQuadraticBeam = 56,
	CEmbers = 46,
	CEnvWind = 60,
	CPrecipitation = 114,
	CPrecipitationBlocker = 115,
	CBaseTempEntity = 18,
	NextBotCombatCharacter = 0,
	CEconWearable = 45,
	CBaseAttributableItem = 4,
	CEconEntity = 44,
	CWeaponXM1014 = 242,
	CWeaponTaser = 237,
	CSmokeGrenade = 131,
	CWeaponSG552 = 234,
	CSensorGrenade = 127,
	CWeaponSawedoff = 230,
	CWeaponNOVA = 226,
	CIncendiaryGrenade = 87,
	CMolotovGrenade = 96,
	CWeaponM3 = 218,
	CKnifeGG = 93,
	CKnife = 92,
	CHEGrenade = 84,
	CFlashbang = 66,
	CWeaponElite = 209,
	CDecoyGrenade = 40,
	CDEagle = 39,
	CWeaponUSP = 241,
	CWeaponM249 = 217,
	CWeaponUMP45 = 240,
	CWeaponTMP = 239,
	CWeaponTec9 = 238,
	CWeaponSSG08 = 236,
	CWeaponSG556 = 235,
	CWeaponSG550 = 233,
	CWeaponScout = 232,
	CWeaponSCAR20 = 231,
	CSCAR17 = 125,
	CWeaponP90 = 229,
	CWeaponP250 = 228,
	CWeaponP228 = 227,
	CWeaponNegev = 225,
	CWeaponMP9 = 224,
	CWeaponMP7 = 223,
	CWeaponMP5Navy = 222,
	CWeaponMag7 = 221,
	CWeaponMAC10 = 220,
	CWeaponM4A1 = 219,
	CWeaponHKP2000 = 216,
	CWeaponGlock = 215,
	CWeaponGalilAR = 214,
	CWeaponGalil = 213,
	CWeaponG3SG1 = 212,
	CWeaponFiveSeven = 211,
	CWeaponFamas = 210,
	CWeaponBizon = 205,
	CWeaponAWP = 203,
	CWeaponAug = 202,
	CAK47 = 1,
	CWeaponCSBaseGun = 207,
	CWeaponCSBase = 206,
	CC4 = 29,
	CWeaponBaseItem = 204,
	CBaseCSGrenade = 8,
	CSmokeGrenadeProjectile = 132,
	CSensorGrenadeProjectile = 128,
	CMolotovProjectile = 97,
	CItem_Healthshot = 91,
	CDecoyProjectile = 41,
	CFireCrackerBlast = 62,
	CInferno = 88,
	CChicken = 31,
	CFootstepControl = 68,
	CCSGameRulesProxy = 34,
	CWeaponCubemap = 0,
	CWeaponCycler = 208,
	CTEPlantBomb = 181,
	CTEFireBullets = 168,
	CTERadioIcon = 185,
	CPlantedC4 = 107,
	CCSTeam = 38,
	CCSPlayerResource = 36,
	CCSPlayer = 35,
	CCSRagdoll = 37,
	CTEPlayerAnimEvent = 182,
	CHostage = 85,
	CHostageCarriableProp = 86,
	CBaseCSGrenadeProjectile = 9,
	CHandleTest = 83,
	CTeamplayRoundBasedRulesProxy = 144,
	CSpriteTrail = 138,
	CSpriteOriented = 137,
	CSprite = 136,
	CRagdollPropAttached = 123,
	CRagdollProp = 122,
	CPredictedViewModel = 116,
	CPoseController = 112,
	CGameRulesProxy = 82,
	CInfoLadderDismount = 89,
	CFuncLadder = 74,
	CTEFoundryHelpers = 171,
	CEnvDetailController = 52,
	CWorld = 243,
	CWaterLODControl = 201,
	CWaterBullet = 200,
	CVoteController = 199,
	CVGuiScreen = 198,
	CPropJeep = 119,
	CPropVehicleChoreoGeneric = 0,
	CTriggerSoundOperator = 197,
	CBaseVPhysicsTrigger = 22,
	CTriggerPlayerMovement = 196,
	CBaseTrigger = 20,
	CTest_ProxyToggle_Networkable = 193,
	CTesla = 188,
	CBaseTeamObjectiveResource = 17,
	CTeam = 143,
	CSunlightShadowControl = 142,
	CSun = 141,
	CParticlePerformanceMonitor = 100,
	CSpotlightEnd = 135,
	CSpatialEntity = 134,
	CSlideshowDisplay = 130,
	CShadowControl = 129,
	CSceneEntity = 126,
	CRopeKeyframe = 124,
	CRagdollManager = 121,
	CPhysicsPropMultiplayer = 105,
	CPhysBoxMultiplayer = 103,
	CPropDoorRotating = 118,
	CBasePropDoor = 16,
	CDynamicProp = 43,
	CProp_Hallucination = 117,
	CPostProcessController = 113,
	CPointCommentaryNode = 111,
	CPointCamera = 110,
	CPlayerResource = 109,
	CPlasma = 108,
	CPhysMagnet = 106,
	CPhysicsProp = 104,
	CStatueProp = 139,
	CPhysBox = 102,
	CParticleSystem = 101,
	CMovieDisplay = 98,
	CMaterialModifyControl = 95,
	CLightGlow = 94,
	CInfoOverlayAccessor = 90,
	CFuncTrackTrain = 81,
	CFuncSmokeVolume = 80,
	CFuncRotating = 79,
	CFuncReflectiveGlass = 78,
	CFuncOccluder = 77,
	CFuncMoveLinear = 76,
	CFuncMonitor = 75,
	CFunc_LOD = 70,
	CTEDust = 163,
	CFunc_Dust = 69,
	CFuncConveyor = 73,
	CFuncBrush = 72,
	CBreakableSurface = 28,
	CFuncAreaPortalWindow = 71,
	CFish = 65,
	CFireSmoke = 63,
	CEnvTonemapController = 59,
	CEnvScreenEffect = 57,
	CEnvScreenOverlay = 58,
	CEnvProjectedTexture = 55,
	CEnvParticleScript = 54,
	CFogController = 67,
	CEnvDOFController = 53,
	CCascadeLight = 30,
	CEnvAmbientLight = 51,
	CEntityParticleTrail = 50,
	CEntityFreezing = 49,
	CEntityFlame = 48,
	CEntityDissolve = 47,
	CDynamicLight = 42,
	CColorCorrectionVolume = 33,
	CColorCorrection = 32,
	CBreakableProp = 27,
	CBeamSpotlight = 25,
	CBaseButton = 5,
	CBaseToggle = 19,
	CBasePlayer = 15,
	CBaseFlex = 12,
	CBaseEntity = 11,
	CBaseDoor = 10,
	CBaseCombatCharacter = 6,
	CBaseAnimatingOverlay = 3,
	CBoneFollower = 26,
	CBaseAnimating = 2,
	CAI_BaseNPC = 0,
	CBeam = 24,
	CBaseViewModel = 21,
	CBaseParticleEntity = 14,
	CBaseGrenade = 13,
	CBaseCombatWeapon = 7,
	CBaseWeaponWorldModel = 23,
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
	LeftLowerArm
};

enum class CSPlayerBones : int {
	pelvis = 0,
	spine_0,
	spine_1,
	spine_2,
	spine_3,
	neck_0,
	head_0,
	clavicle_L,
	arm_upper_L,
	arm_lower_L,
	hand_L,
	finger_middle_meta_L,
	finger_middle_0_L,
	finger_middle_1_L,
	finger_middle_2_L,
	finger_pinky_meta_L,
	finger_pinky_0_L,
	finger_pinky_1_L,
	finger_pinky_2_L,
	finger_index_meta_L,
	finger_index_0_L,
	finger_index_1_L,
	finger_index_2_L,
	finger_thumb_0_L,
	finger_thumb_1_L,
	finger_thumb_2_L,
	finger_ring_meta_L,
	finger_ring_0_L,
	finger_ring_1_L,
	finger_ring_2_L,
	weapon_hand_L,
	arm_lower_L_TWIST,
	arm_lower_L_TWIST1,
	arm_upper_L_TWIST,
	arm_upper_L_TWIST1,
	clavicle_R,
	arm_upper_R,
	arm_lower_R,
	hand_R,
	finger_middle_meta_R,
	finger_middle_0_R,
	finger_middle_1_R,
	finger_middle_2_R,
	finger_pinky_meta_R,
	finger_pinky_0_R,
	finger_pinky_1_R,
	finger_pinky_2_R,
	finger_index_meta_R,
	finger_index_0_R,
	finger_index_1_R,
	finger_index_2_R,
	finger_thumb_0_R,
	finger_thumb_1_R,
	finger_thumb_2_R,
	finger_ring_meta_R,
	finger_ring_0_R,
	finger_ring_1_R,
	finger_ring_2_R,
	weapon_hand_R,
	arm_lower_R_TWIST,
	arm_lower_R_TWIST1,
	arm_upper_R_TWIST,
	arm_upper_R_TWIST1,
	leg_upper_L,
	leg_lower_L,
	ankle_L,
	ball_L,
	leg_upper_L_TWIST,
	leg_upper_L_TWIST1,
	leg_upper_R,
	leg_lower_R,
	ankle_R,
	ball_R,
	leg_upper_R_TWIST,
	leg_upper_R_TWIST1,
	weapon_bone,
	lh_ik_driver,
	lean_root,
	lfoot_lock,
	rfoot_lock,
	primary_jiggle_jnt,
	primary_smg_jiggle_jnt
};



class ScriptCreatedItem
{
public:
	CPNETVAR_FUNC(int*, ItemDefinitionIndex, 0xE67AB3B8); //m_iItemDefinitionIndex
	CPNETVAR_FUNC(int*, AccountID, 0x24abbea8); //m_iAccountID
	CPNETVAR_FUNC(int*, ItemIDHigh, 0x714778A); //m_iItemIDHigh
	CPNETVAR_FUNC(int*, ItemIDLow, 0x3A3DFC74); //m_iItemIDLow
	CPNETVAR_FUNC(char**, szCustomName, 0x13285ad9); //m_szCustomName
	CPNETVAR_FUNC(int*, EntityQuality, 0x110be6fe); //m_iEntityQuality
};

class AttributeContainer
{
public:
	CPNETVAR_FUNC(ScriptCreatedItem*, m_Item, 0x7E029CE5);
};


class CBaseCombatWeapon
{
public:
	CNETVAR_FUNC(float, GetNextPrimaryAttack, 0xDB7B106E); //m_flNextPrimaryAttack
	CNETVAR_FUNC(int, GetAmmoInClip, 0x97B6F70C); //m_iClip1
	CNETVAR_FUNC(HANDLE, GetOwnerHandle, 0xC32DF98D); //m_hOwner
	CNETVAR_FUNC(Vector, GetOrigin, 0x1231CE10); //m_vecOrigin
	CPNETVAR_FUNC(int*, FallbackPaintKit, 0xADE4C870); // m_nFallbackPaintKit
	CPNETVAR_FUNC(int*, FallbackSeed, 0xC2D0683D); // m_nFallbackSeed
	CPNETVAR_FUNC(float*, FallbackWear, 0xA263576C); //m_flFallbackWear
	CPNETVAR_FUNC(int*, FallbackStatTrak, 0x1ED78768); //m_nFallbackStatTrak
	CPNETVAR_FUNC(int*, OwnerXuidLow, 0xAD8D897F);
	CPNETVAR_FUNC(int*, OwnerXuidHigh, 0x90511E77);
	CPNETVAR_FUNC(int*, ViewModelIndex, 0x7F7C89C1);
	CPNETVAR_FUNC(int*, ModelIndex, 0x27016F83);
	CPNETVAR_FUNC(int*, WorldModelIndex, 0x4D8AD9F3);
	CPNETVAR_FUNC(char*, szCustomName, 0x0);

	CPNETVAR_FUNC(AttributeContainer*, m_AttributeManager, 0xCFFCE089);

	float GetInaccuracy()
	{
		typedef float(__thiscall* oInaccuracy)(PVOID);
		return call_vfunc< oInaccuracy >(this, 484)(this);
	}

	void SetModelIndex(int modelIndex) {
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		return call_vfunc<OriginalFn>(this, 75)(this, modelIndex);
	}

	float GetSpread()
	{
		typedef float(__thiscall* oWeaponSpread)(PVOID);
		return call_vfunc< oWeaponSpread >(this, 485)(this);
	}

	void UpdateAccuracyPenalty()
	{
		typedef void(__thiscall* oUpdateAccuracy)(PVOID);
		return call_vfunc<oUpdateAccuracy>(this, 486)(this);
	}

	int GetAmmo()
	{
		return *(int*)((uintptr_t)this + 0x3204);
	}

	float GetInaccuracy1()
	{
		typedef float(__thiscall* oGetSpread)(PVOID);
		return call_vfunc< oGetSpread>(this, Offsets::VMT::Weapon_GetSpread)(this);
	}

	float GetAccuracyPenalty()
	{
		return *(float*)((DWORD)this + 0x32B0);
	}

	float GetSpread1()
	{
		typedef float(__thiscall* oGetInac)(PVOID);
		return call_vfunc< oGetInac>(this, Offsets::VMT::Weapon_GetSpread + 1)(this);
	}

	void UpdateAccuracyPenalty1()
	{
		typedef void(__thiscall* oUpdateAccuracyPenalty)(PVOID);
		return call_vfunc< oUpdateAccuracyPenalty>(this, Offsets::VMT::Weapon_GetSpread + 2)(this);
	}

	CSWeaponInfo GetCSWpnData(CBaseCombatWeapon * pWeapon, int iIndex)
	{
		return ((GetCSWpnDataFn)((*(DWORD**)pWeapon)[iIndex]))(pWeapon);
	}

	int GetAmmoInClip2() {
		return *(int*)((DWORD)this + 0x000031F4);
	}
	int GetWeaponID2() {
		return *(int*)((DWORD)this + 0x00002F88);
	}

	void PreDataUpdate(int updateType)
	{
		PVOID pNetworkable = (PVOID)((DWORD)(this) + 0x8);
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		return call_vfunc<OriginalFn>(pNetworkable, 6)(pNetworkable, updateType);
	}

	/*CSWeaponInfo* GetCSWpnData()
	{
	static DWORD GetCSWpnDataAddr = Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x81\xEC\x00\x00\x00\x00\xB8\x00\x00\x00\x00\x57", "xxxxx????x????x");
	if (GetCSWpnDataAddr)
	{
	CSWeaponInfo* retData;
	__asm
	{
	mov ecx, this
	call GetCSWpnDataAddr
	mov retData, eax
	}
	return retData;
	}
	else
	{
	return nullptr;
	}
	}*/
};

class CCSBomb
{
public:
	CNETVAR_FUNC(HANDLE, GetOwnerHandle, 0xC32DF98D); //m_hOwner
	CNETVAR_FUNC(float, GetC4BlowTime, 0xB5E0CA1C); //m_flC4Blow
	CNETVAR_FUNC(float, GetC4DefuseCountDown, 0xB959B4A6); //m_flDefuseCountDown
};

class CLocalPlayerExclusive
{
public:
	CNETVAR_FUNC(Vector, GetViewPunchAngle, 0x68F014C0);//m_viewPunchAngle
	CNETVAR_FUNC(Vector, GetAimPunchAngle, 0xBF25C290);//m_aimPunchAngle
	CNETVAR_FUNC(Vector, GetAimPunchAngleVel, 0x8425E045);//m_aimPunchAngleVel
};

class CollisionProperty
{
public:
	CNETVAR_FUNC(Vector, GetMins, 0xF6F78BAB);//m_vecMins
	CNETVAR_FUNC(Vector, GetMaxs, 0xE47C6FC4);//m_vecMaxs
	CNETVAR_FUNC(unsigned char, GetSolidType, 0xB86722A1);//m_nSolidType
	CNETVAR_FUNC(unsigned short, GetSolidFlags, 0x63BB24C1);//m_usSolidFlags
	CNETVAR_FUNC(int, GetSurroundsType, 0xB677A0BB); //m_nSurroundType

	bool IsSolid()
	{
		return (GetSolidType() != SOLID_NONE) && ((GetSolidFlags() & FSOLID_NOT_SOLID) == 0);
	}
};

class IClientRenderable
{
public:
	//virtual void*					GetIClientUnknown() = 0;
	virtual Vector const&			GetRenderOrigin(void) = 0;
	virtual Vector const&			GetRenderAngles(void) = 0;
	virtual bool					ShouldDraw(void) = 0;
	virtual bool					IsTransparent(void) = 0;
	virtual bool					UsesPowerOfTwoFrameBufferTexture() = 0;
	virtual bool					UsesFullFrameBufferTexture() = 0;
	virtual void					GetShadowHandle() const = 0;
	virtual void*					RenderHandle() = 0;
	virtual const model_t*				GetModel() const = 0;
	virtual int						DrawModel(int flags) = 0;
	virtual int						GetBody() = 0;
	virtual void					ComputeFxBlend() = 0;

	bool SetupBones(matrix3x4 *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		typedef bool(__thiscall* oSetupBones)(PVOID, matrix3x4*, int, int, float);
		return call_vfunc< oSetupBones>(this, 13)(this, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}
};

class IClientNetworkable
{
public:
	virtual IClientUnknown*	GetIClientUnknown() = 0;
	virtual void			Release() = 0;
	virtual ClientClass*	GetClientClass() = 0;// FOR NETVARS FIND YOURSELF ClientClass* stuffs
	virtual void			NotifyShouldTransmit( /* ShouldTransmitState_t state*/) = 0;
	virtual void			OnPreDataChanged( /*DataUpdateType_t updateType*/) = 0;
	virtual void			OnDataChanged( /*DataUpdateType_t updateType*/) = 0;
	virtual void			PreDataUpdate( /*DataUpdateType_t updateType*/) = 0;
	virtual void			PostDataUpdate( /*DataUpdateType_t updateType*/) = 0;
	virtual void			unknown();
	virtual bool			IsDormant(void) = 0;
	virtual int				GetIndex(void) const = 0;
	virtual void			ReceiveMessage(int classID /*, bf_read &msg*/) = 0;
	virtual void*			GetDataTableBasePtr() = 0;
	virtual void			SetDestroyedOnRecreateEntities(void) = 0;
};

class IClientUnknown
{
public:
	virtual void*		GetCollideable() = 0;
	virtual IClientNetworkable*	GetClientNetworkable() = 0;
	virtual IClientRenderable*	GetClientRenderable() = 0;
	virtual IClientEntity*		GetIClientEntity() = 0;
	virtual IClientEntity*		GetBaseEntity() = 0;
	virtual IClientThinkable*	GetClientThinkable() = 0;
};



class IClientThinkable
{
public:
	virtual IClientUnknown*		GetIClientUnknown() = 0;
	virtual void				ClientThink() = 0;
	virtual void*				GetThinkHandle() = 0;
	virtual void				SetThinkHandle(void* hThink) = 0;
	virtual void				Release() = 0;
};

class __declspec (novtable)IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	virtual void			Release(void) = 0;
	virtual void			blahblahpad(void) = 0;
	virtual Vector&	GetAbsOrigin(void) const = 0;//in broken place use GetOrigin Below
	virtual const Vector&	GetAbsAngles(void) const = 0;

	//---                 NetVars                  ---//

	CPNETVAR_FUNC(CLocalPlayerExclusive*, localPlayerExclusive, 0x7177BC3E);// m_Local
	CPNETVAR_FUNC(CollisionProperty*, collisionProperty, 0xE477CBD0);//m_Collision

	CNETVAR_FUNC(int, GetFlags, 0xE456D580); //m_fFlags
	CNETVAR_FUNC(int*, GetPointerFlags, 0xE456D580); //m_fFlags
	CNETVAR_FUNC(Vector, GetOrigin, 0x1231CE10); //m_vecOrigin
	CNETVAR_FUNC(Vector, GetRotation, 0x6BEA197A); //m_angRotation
	CNETVAR_FUNC(int, GetTeamNum, 0xC08B6C6E); //m_iTeamNum
	CNETVAR_FUNC(int, GetMaxHealth, 0xC52E1C28); //m_iMaxHealth
	CNETVAR_FUNC(int, GetHealth, 0xA93054E3); //m_iHealth
	CNETVAR_FUNC(unsigned char, GetLifeState, 0xD795CCFC); //m_lifeState
	CNETVAR_FUNC(HANDLE, GetActiveWeaponHandle, 0xB4FECDA3); //m_hActiveWeapon
	CNETVAR_FUNC(int, GetTickBase, 0xD472B079); //m_nTickBase
	CNETVAR_FUNC(Vector, GetViewOffset, 0xA9F74931); //m_vecViewOffset[0]
	CNETVAR_FUNC(Vector, GetVelocity, 0x40C1CA24); //m_vecVelocity[0]
	CNETVAR_FUNC(bool, HasGunGameImmunity, 0x6AD6FA0D); //m_bGunGameImmunity
	CNETVAR_FUNC(bool, IsDefusing, 0xA2C14106); //m_bIsDefusing
	CNETVAR_FUNC(int, ArmorValue, 0x3898634); //m_ArmorValue
	CNETVAR_FUNC(bool, HasHelmet, 0x7B97F18A); //m_bHasHelmet
	CNETVAR_FUNC(bool, IsScoped, 0x61B9C22C); //m_bIsScoped
	CNETVAR_FUNC(bool, HasDefuser, 0x32D0F325); //m_bHasDefuser
	CNETVAR_FUNC(int, GetMoney, 0xF4B3E183); //m_iAccount
	CNETVAR_FUNC(HANDLE, GetObserverTargetHandle, 0x8660FD83); //m_hObserverTarget
	CNETVAR_FUNC(float, GetLowerBodyYaw, 0xE6996CCF); //m_flLowerBodyYawTarget  // ----------------------------------------------//

	bool IsAlive()
	{
		return (GetLifeState() == LIFE_ALIVE && GetHealth() > 0);
	}

	QAngle* GetEyeAnglesPointer()
	{
		return reinterpret_cast<QAngle*>((DWORD)this + (DWORD)0xAA08);
	}

	Vector GetVecVelocity() {
		return *(Vector*)((DWORD)this + 0x110);
	}

	Vector GetBonePos(int i)
	{
		matrix3x4 boneMatrix[128];
		if (this->SetupBones(boneMatrix, 128, BONE_USED_BY_HITBOX, GetTickCount64()))
		{
			return Vector(boneMatrix[i][0][3], boneMatrix[i][1][3], boneMatrix[i][2][3]);
		}
		return Vector(0, 0, 0);
	} 
	ObserverMode_t* GetObserverMode()
	{
		return (ObserverMode_t*)((uintptr_t)this + 0x336C);
	}

	float* GetFlashMaxAlpha()
	{
		return (float*)((uintptr_t)this + 0xA304);
	}

	void* GetObserverTarget()
	{
		return (void*)((uintptr_t)this + 0x3380);
	}

	bool IsPlayer()
	{
		typedef bool(__thiscall *IsPlayer_t)(PVOID);
		return ((IsPlayer_t)(*(PDWORD)(*(PDWORD)(this) + 0x260)))(this);
	}

	Vector GetHeadPos() {
		return this->GetBonePos(6);
	}

	Vector GetOrigin2() {
		return *(Vector*)((DWORD)this + 0x00000134);
	}
	Vector GetViewAngles2() {
		return *(Vector*)((DWORD)this + 0x00000104);
	}

	Vector GetAbsOrigin2() {
		__asm {
			MOV ECX, this
			MOV EAX, DWORD PTR DS : [ECX]
			CALL DWORD PTR DS : [EAX + 0x28]
		}
	}
	Vector GetAbsAngles2() {
		__asm {
			MOV ECX, this;
			MOV EAX, DWORD PTR DS : [ECX];
			CALL DWORD PTR DS : [EAX + 0x2C]
		}
	}


	Vector GetEyePosition() {
		Vector Origin = *(Vector*)((DWORD)this + 0x00000134);
		Vector View = *(Vector*)((DWORD)this + 0x00000104);
		return(Origin + View);
	}
	Vector GetAimPunch() {
		return *(Vector*)((DWORD)this + 0x00003018);
	}
	bool IsImmune() {
		return *(bool*)((DWORD)this + 0x000038A0);
	}
	ClientClass *GetClientClass2() {
		PVOID Networkable = (PVOID)((DWORD)(this) + 0x8);
		using Original = ClientClass*(__thiscall*)(PVOID);
		return call_vfunc<Original>(Networkable, 2)(Networkable);
	}
	HANDLE GetWeaponHandle() {
		return *(HANDLE*)((DWORD)this + 0x00002EE8);
	}

	void SetModelIndex(int modelIndex) {
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		return call_vfunc<OriginalFn>(this, 75)(this, modelIndex);
	}

};