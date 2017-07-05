#ifndef _CACHIVEMENTS_H_
#define _CACHIVEMENTS_H_
//===================================================================================
#include "SDK.h"
//===================================================================================
class CAchievements
{
public:

	void Think( void ); //call every frame

	bool IsReady( )
	{

		if( !g_pEngine )
			return false;

		if( !g_pEngine->GetAchievementMgr( ) )
			return false;

		if( !g_pSteamStats )
			return false;

		return true;
	}

	void UnlockAll( void )
	{
		if( !IsReady() )
			return;

		CGameID gameid( g_pEngine->GetAppID( ) );
	
		CAchievementMgr* pAchieve = dynamic_cast<CAchievementMgr*>( g_pEngine->GetAchievementMgr( ) );

		if( !pAchieve )
			return;

		for( int i = 0; i < g_pSteamStats->GetNumAchievements( gameid ); i++ )
		{
			IAchievement* pAchievement = pAchieve->GetAchievementByIndex( i );

			if( !pAchievement )
				continue;

			CBaseAchievement *pBaseAchievement = dynamic_cast<CBaseAchievement *>( pAchievement );

			if( !pBaseAchievement )
				continue;

			int id = pAchievement->GetAchievementID( );

			pBaseAchievement->SetAchieved( true );

			pAchieve->AwardAchievement( id );

			g_pSteamStats->SetAchievement( gameid, g_pSteamStats->GetAchievementName( gameid, i ) );
			g_pSteamStats->StoreStats( gameid );
		}
	}

	void LockAll( void )
	{
		if( !IsReady( ) )
			return;

		CGameID gameid( g_pEngine->GetAppID( ) );

		for( int i = 0; i < g_pSteamStats->GetNumAchievements( gameid ); i++ )
		{
			CAchievementMgr* pAchieveMgr = dynamic_cast<CAchievementMgr*>( g_pEngine->GetAchievementMgr( ) );

			if( !pAchieveMgr )
				return;

			IAchievement* pAchievement = pAchieveMgr->GetAchievementByIndex( i );

			if( !pAchievement )
				continue;

			CBaseAchievement *pBaseAchievement = dynamic_cast<CBaseAchievement *>( pAchievement );

			if( !pBaseAchievement )
				continue;

			pBaseAchievement->SetAchieved( false );
			
			pBaseAchievement->SetCount( 0 );	

			g_pSteamStats->ClearAchievement( gameid, g_pSteamStats->GetAchievementName( gameid, i ) );
			g_pSteamStats->StoreStats( gameid );

		}
	}

};
//===================================================================================
extern CAchievements gAchievements;
//===================================================================================
#endif