#ifndef _EXTRA_FUNCTIONS_H_
#define _EXTRA_FUNCTIONS_H_
//===================================================================================
#include "SDK.h"
//===================================================================================
typedef struct CSafeUserCmd_t 
{ 
	CUserCmd pCommand; 
	DWORD dwUnk0; 
} CSafeUserCmd;  
//===================================================================================
typedef struct CVerifyFuncs_t
{
	void *SaveUserCmd0;     
	void *SaveUserCmd1;     
} CVerifyFuncs;
//===================================================================================
unsigned short UTIL_GetAchievementEventMask( void );
CSafeUserCmd* GetSafeUserCmd( int sequence_number );
C_BaseCombatWeapon* GetBaseCombatActiveWeapon ( C_BaseEntity* pEntity );
bool bIsCriticalShot( C_BaseCombatWeapon* pBaseWeapon );
int iGetWeaponID( C_BaseCombatWeapon* pBaseWeapon );
float flGetWeaponSpread( C_BaseCombatWeapon* pBaseWeapon );
float flGetCritTime( int iWeaponID );
char* szWeaponIDToAlias( int iWeaponID );
bool bIsBlacklistCrit( int iWeaponID );
bool bIsWeaponSpread( int iWeaponID );
bool bIsSpeedKey( int iSpeedKey );
void Spinbot( CUserCmd* pCommand );
bool bTraceToPlayer( void );
void GetWorldSpaceCenter( C_BaseEntity* pBaseEntity, Vector& vWorldSpaceCenter );
float flGetDistance( Vector vOrigin, Vector vLocalOrigin );
QAngle qGetPunchAngle( void );
const char* szGetTF2Class( char* szModelClass );
void RemoveSniperScope( void );
void UnprotectCvars( void );
void LowerCase( char *szString );
//===================================================================================
extern CVerifyFuncs gVerifyFuncs;
//===================================================================================
#endif