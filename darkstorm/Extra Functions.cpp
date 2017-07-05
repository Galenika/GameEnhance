#include "SDK.h"
#include "Extra Functions.h"
#include "Weapon List.h"
#include "Control Variables.h"
//===================================================================================
CVerifyFuncs gVerifyFuncs;
//===================================================================================
IMaterial* m_pSniperScopeUpperLeft = NULL;
IMaterial* m_pSniperScopeUpperRight = NULL;
IMaterial* m_pSniperScopeLowerLeft = NULL;
IMaterial* m_pSniperScopeLowerRight = NULL;
//===================================================================================
unsigned short UTIL_GetAchievementEventMask( void )
{
	CRC32_t mapCRC;
	CRC32_Init( &mapCRC );

	char lowercase[ 256 ];

	Q_FileBase( g_pEngine->GetLevelName( ), lowercase, sizeof( lowercase ) );
	Q_strlower( lowercase );

	CRC32_ProcessBuffer( &mapCRC, lowercase, Q_strlen( lowercase ) );
	CRC32_Final( &mapCRC );

	return ( mapCRC & 0xFFFF );
}
//===================================================================================
int	C_BasePlayer::GetUserID( void )
{
	player_info_t pi;

	if ( !g_pEngine->GetPlayerInfo( g_pEngine->GetLocalPlayer( ), &pi ) )
		return -1;

	return pi.userID;
}
//===================================================================================
CSafeUserCmd *GetSafeUserCmd( int sequence_number ) 
{ 
	CSafeUserCmd *pUserCmdBackup;  
	__asm 
	{ 
		MOV EDI, [g_pInput]  
		MOV EAX, DWORD PTR DS:[EDI+0xB8] 
		MOV pUserCmdBackup, EAX 

	} 

	CSafeUserCmd *p = &pUserCmdBackup[sequence_number % MULTIPLAYER_BACKUP];
	return p; 
}
//===================================================================================
C_BaseCombatWeapon * GetBaseCombatActiveWeapon ( C_BaseEntity* pEntity )
{
	C_BaseCombatWeapon* hResult = NULL;
	__asm
	{
		MOV  EDI, pEntity;
		MOV  EAX, DWORD PTR DS :[EDI];
		MOV  ECX, EDI;
		CALL DWORD PTR DS :[EAX+0x300]
		MOV  hResult, EAX;
	}
	return hResult;
}
//===================================================================================
bool bIsCriticalShot( C_BaseCombatWeapon* pWeapon )
{
	bool bIsCritShot;
	_asm
	{
		MOV ESI, pWeapon;
		MOV EDX, DWORD PTR DS:[ESI];
		MOV EAX, DWORD PTR DS:[EDX+0x528];
		MOV ECX, ESI;
		CALL EAX;
		MOV bIsCritShot, AL;
	}
	return bIsCritShot;
}
//===================================================================================
int iGetWeaponID( C_BaseCombatWeapon* pWeapon )
{
	int iWeaponID;
	_asm
	{
		MOV ESI, pWeapon;
		MOV EDX, DWORD PTR DS:[ESI];
		MOV EAX, DWORD PTR DS:[EDX+0x51C];
		MOV ECX, ESI;
		CALL EAX;
		MOV iWeaponID,EAX;
	}
	return iWeaponID;
}
//===================================================================================
float flGetWeaponSpread( C_BaseCombatWeapon* pWeapon )
{
	float hResult = 0.0f;
	__asm
	{
		MOV ESI, pWeapon;
		MOV EDX, DWORD PTR DS:[ESI];
		MOV EAX, DWORD PTR DS:[EDX+0x580];
		MOV ECX, ESI;
		CALL EAX;
		FSTP hResult
	}
	return hResult;
}
//===================================================================================
char* szWeaponIDToAlias( int iWeaponID )
{
	char* szWeaponAlias = NULL;

	_asm
	{
		PUSH iWeaponID;
		MOV EAX, dwWeaponIDToAlias;
		CALL EAX;
		ADD ESP, 4;
		MOV szWeaponAlias, EAX;
	}

	return szWeaponAlias;
}
//===================================================================================
float flGetCritTime( int iWeaponID )
{
	switch ( iWeaponID )
	{
	case WEAPONLIST_SCATTERGUN:
	case WEAPONLIST_ROCKETLAUNCHER:
	case WEAPONLIST_SOLDIERSHOTGUN:
	case WEAPONLIST_PYROSHOTGUN:
	case WEAPONLIST_PIPEBOMBLAUNCHER:
	case WEAPONLIST_STICKYBOMBLAUNCHER:
	case WEAPONLIST_HEAVYSHOTGUN:
	case WEAPONLIST_ENGINEERSHOTGUN:
	case WEAPONLIST_SPYPISTOL:
		return 0.05;
		break;
	case WEAPONLIST_SCOUTPISTOL:
	case WEAPONLIST_MINIGUN:
	case WEAPONLIST_ENGINEERPISTOL:
	case WEAPONLIST_NEEDLEGUN:
	case WEAPONLIST_SMG:
		return 0.02631579;
		break;
	case WEAPONLIST_BAT:
	case WEAPONLIST_SHOVEL:
	case WEAPONLIST_AXE:
	case WEAPONLIST_BOTTLE:
	case WEAPONLIST_FISTS:
	case WEAPONLIST_WRENCH:
	case WEAPONLIST_BONESAW:
	case WEAPONLIST_KUKRI:
		return 0.15;
		break;
	}

	return 0.0f;
}
//===================================================================================
bool bIsBlacklistCrit( int iWeaponID )
{
	switch(iWeaponID)
	{
		case WEAPONLIST_MINIGUN:
		case WEAPONLIST_BUILDER:
		case WEAPONLIST_DESTRUCTOR:
		case WEAPONLIST_NEEDLEGUN:
		//case WEAPONLIST_BLUTSLAUGER:
		case WEAPONLIST_MEDIGUN:
		case WEAPONLIST_SAPPER:
		case WEAPONLIST_KNIFE:
		case WEAPONLIST_DISGUISE:
		case WEAPONLIST_RIFLE:
		case WEAPONLIST_FLAMETHROWER:
			return true;
	}

	return false;
}
//===================================================================================
bool bIsWeaponSpread( int iWeaponID )
{
	switch(iWeaponID)
	{
		case WEAPONLIST_MINIGUN:
		case WEAPONLIST_ENGINEERPISTOL:
		case WEAPONLIST_SCOUTPISTOL:
		case WEAPONLIST_SPYPISTOL:
		case WEAPONLIST_SMG:
			return true;
	}

	return false;
}

bool bIsSpeedKey( int iSpeedKey )
{
	static bool bKey = false;

	switch( iSpeedKey )
	{
	case 0:
		bKey = false;
		break;

	case 1:
		if(GetAsyncKeyState(VK_LBUTTON))
			bKey = true;
		else
			bKey = false;
		break;

	case 2:
		if(GetAsyncKeyState(VK_RBUTTON))
			bKey = true;
		else
			bKey = false;
		break;

	case 3:
		if(GetAsyncKeyState(VK_MBUTTON))
			bKey = true;
		else
			bKey = false;
		break;

	case 4:
		if(GetAsyncKeyState(VK_LSHIFT))
			bKey = true;
		else
			bKey = false;
		break;

	case 5:
		if(GetAsyncKeyState( 0x46 ))
			bKey = true;
		else
			bKey = false;
		break;
	}

	return bKey;
}
//===================================================================================
void Normalize(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();

	if(flLen == 0)
	{
		vOut.Init(0, 0, 1);
		return;
	}

	flLen = 1 / flLen;

	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}
//===================================================================================
void Spinbot( CUserCmd* pCommand )
{
	if( pCommand->buttons & IN_ATTACK )
		return;

	Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
	QAngle qAimAngles;

	float forward = pCommand->forwardmove;
	float right = pCommand->sidemove;
	float up = pCommand->upmove;

	qAimAngles.Init( 0.0f, pCommand->viewangles.y, 0.0f );
	AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);

	float fTime = g_pEngine->Time( );
	pCommand->viewangles.y = (vec_t)( fmod(fTime / 0.1f * 360.0f, 360.0f));
	qAimAngles.Init(0.0f, pCommand->viewangles.y, 0.0f);

	AngleVectors(qAimAngles, &aimforward, &aimright, &aimup);

	Vector vForwardNorm;		Normalize ( viewforward, vForwardNorm );
	Vector vRightNorm;			Normalize( viewright, vRightNorm );
	Vector vUpNorm;				Normalize( viewup, vUpNorm );

	pCommand->forwardmove = DotProduct(forward * vForwardNorm, aimforward) + DotProduct(right * vRightNorm, aimforward) + DotProduct(up * vUpNorm, aimforward);
	pCommand->sidemove = DotProduct(forward * vForwardNorm, aimright) + DotProduct(right * vRightNorm, aimright) + DotProduct(up * vUpNorm, aimright);
	pCommand->upmove = DotProduct(forward * vForwardNorm, aimup) + DotProduct(right * vRightNorm, aimup) + DotProduct(up * vUpNorm, aimup);
}
//===================================================================================
bool bTraceToPlayer( void )
{
	trace_t pTrace;
	Ray_t pRay;
	player_info_t pInfo;

	int iLocalIndex = g_pEngine->GetLocalPlayer();

	if ( iLocalIndex <= 0 )
		return false;

	IClientEntity* pEntity = g_pEntList->GetClientEntity( iLocalIndex );

	if ( !pEntity )
		return false;

	C_BaseEntity* pBaseEntity = pEntity->GetBaseEntity( );

	if ( !pBaseEntity )
		return false;

	Vector vDirection;

	AngleVectors( pBaseEntity->GetAbsAngles( ), &vDirection );

	vDirection = vDirection * 8192 + pBaseEntity->EyePosition( );
	Vector vLocalPosition = pBaseEntity->EyePosition( );

	pRay.Init( vLocalPosition, vDirection );

	g_pEngineTrace->TraceRay( pRay, ( CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_DEBRIS|CONTENTS_HITBOX ), NULL, &pTrace);

	if ( pTrace.allsolid )
		return false;

	if ( pTrace.m_pEnt )
	{
		if ( g_pEngine->GetPlayerInfo( pTrace.m_pEnt->index, &pInfo ) == false )
			return false;

		return pTrace.m_pEnt->m_iTeamNum != pBaseEntity->m_iTeamNum;
	}

	return false;
}
//===================================================================================
void GetWorldSpaceCenter( C_BaseEntity* pBaseEnt, Vector& vWorldSpaceCenter )
{
	if ( pBaseEnt )
	{
		Vector vMin, vMax;
		pBaseEnt->GetRenderBounds( vMin, vMax );
		vWorldSpaceCenter = pBaseEnt->GetAbsOrigin( );
		vWorldSpaceCenter.z += (vMin.z + vMax.z) / 2.0f;
	}
}
//===================================================================================
float flGetDistance( Vector vOrigin, Vector vLocalOrigin )
{
	Vector vDelta = vOrigin - vLocalOrigin;

	float m_fDistance = FastSqrt( vDelta.Length( ) );

	if( m_fDistance < 1.0f )
		return 1.0f;

	return m_fDistance;
}
//===================================================================================
QAngle qGetPunchAngle( void )
{
	C_BaseEntity* pBaseEntity = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer( ) )->GetBaseEntity( );
	CSDKPlayer *pSDKPlayer = ToSDKPlayer( pBaseEntity );
	QAngle pPunchAngle = pSDKPlayer->m_Local.m_vecPunchAngle.Get( );
	return pPunchAngle;
}
//===================================================================================
const char* szGetTF2Class( char* szModelClass )
{
	if( strstr( szModelClass, "scout" ) )
		return "Scout";
	else if( strstr( szModelClass, "soldier" ) )
		return "Soldier";
	else if( strstr( szModelClass, "demo" ) )
		return "Demoman";
	else if( strstr( szModelClass, "heavy" ) )
		return "Heavy";
	else if( strstr( szModelClass, "engineer" ) )
		return "Engineer";
	else if( strstr( szModelClass, "medic" ) )
		return "Medic";
	else if( strstr( szModelClass, "sniper" ) )
		return "Sniper";
	else if( strstr( szModelClass, "spy" ) )
		return "Spy";
	else
		return "Pyro"; //default
}
//===================================================================================
void RemoveSniperScope( void )
{
	if ( g_pMatSystem == NULL )
		return;
 
	m_pSniperScopeUpperLeft     =    g_pMatSystem->FindMaterial( "HUD/scope_sniper_ul", "ClientEffect textures" ); 
	m_pSniperScopeUpperRight    =    g_pMatSystem->FindMaterial( "HUD/scope_sniper_ur", "ClientEffect textures" ); 
	m_pSniperScopeLowerLeft     =    g_pMatSystem->FindMaterial( "HUD/scope_sniper_ll", "ClientEffect textures" ); 
	m_pSniperScopeLowerRight    =    g_pMatSystem->FindMaterial( "HUD/scope_sniper_lr", "ClientEffect textures" ); 

	if ( m_pSniperScopeUpperLeft ) 
		m_pSniperScopeUpperLeft->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, gCvars.misc_sniper ); 

	if ( m_pSniperScopeUpperRight ) 
		m_pSniperScopeUpperRight->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, gCvars.misc_sniper ); 

	if ( m_pSniperScopeLowerLeft ) 
		m_pSniperScopeLowerLeft->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, gCvars.misc_sniper ); 

	if ( m_pSniperScopeLowerRight ) 
		m_pSniperScopeLowerRight->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, gCvars.misc_sniper ); 

}
//===================================================================================
void UnprotectCvars( void )
{
	ConCommand *pVar = (ConCommand*)g_pCvar->GetCommands( );
	
	ConVar *pConsistency = g_pCvar->FindVar( "sv_consistency" );
	ConVar *pCheats = g_pCvar->FindVar( "sv_cheats" );

	while( pVar )
	{
		if( pVar->IsFlagSet( FCVAR_CHEAT ) )
			pVar->m_nFlags &= ~FCVAR_CHEAT;

		if( pVar->IsFlagSet( FCVAR_REPLICATED ) )
			pVar->m_nFlags &= ~FCVAR_REPLICATED;

		if( pVar->IsFlagSet( FCVAR_PROTECTED ) )
			pVar->m_nFlags &= ~FCVAR_PROTECTED;

		if( pVar->IsFlagSet( FCVAR_SPONLY ) )
			pVar->m_nFlags &= ~FCVAR_SPONLY;

		pVar = (ConCommand*)pVar->GetNext( );
	}

	pConsistency->SetValue( 0 );
	pCheats->SetValue( 1 );
}
//===================================================================================
double dblGetPlatRealTime( void )
{
	bool bIsBench = Plat_IsInBenchmarkMode( );

	Plat_SetBenchmarkMode( false );

	double dblCurTime = Plat_FloatTime( );

	if( bIsBench )
		Plat_SetBenchmarkMode( true );

	return dblCurTime;
}
//===================================================================================