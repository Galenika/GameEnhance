#include "SDK.h"
#include "Achievements.h"
#include "Extra Functions.h"
//===================================================================================
CAchievements gAchievements;
//===================================================================================
void CAchievements::Think( void )
{
	C_BasePlayer* pBasePlayer = dynamic_cast<C_BasePlayer*>( g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer( ) ) );

	if( !pBasePlayer )
		return;

	if( GetAsyncKeyState( VK_F10 ) & 1 )
	{
		UnlockAll( );
	}
	if( GetAsyncKeyState( VK_F11 ) & 1 )
	{
		LockAll( );
	}
	if( GetAsyncKeyState( VK_F12 ) & 1 )
	{
		char cmd[256];
		int iPlayerID = pBasePlayer->GetUserID( );
		unsigned short mask = UTIL_GetAchievementEventMask( );
		int achievement_id = 0x663;
		Q_snprintf( cmd, sizeof( cmd ), "achievement_earned %d %d", achievement_id ^ mask, ( iPlayerID ^ achievement_id ) ^ mask);
		g_pEngine->ClientCmd_Unrestricted( cmd );
	}
}
//===================================================================================