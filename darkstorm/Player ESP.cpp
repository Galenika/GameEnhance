#include "SDK.h"
#include "Player ESP.h"
#include "Player Manager.h"
#include "Draw Manager.h"
#include "Radar.h"
#include "World Objects.h"
#include "Control Variables.h"
#include "Font Manager.h"
#include "Extra Functions.h"
#include "Sound Engine.h"
#include "Aimbot.h"
//===================================================================================
CPlayerESP gPlayerESP;
//===================================================================================
void CPlayerESP::DrawPlayerESP( int iIndex )
{
	IClientEntity* pLocalEntity = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer( ) );

	if( pLocalEntity == NULL )
		return;

	C_BaseEntity* pLocalBaseEntity = dynamic_cast< C_BaseEntity* > ( pLocalEntity );

	if( pLocalBaseEntity == NULL )
		return;

//	if( pLocalBaseEntity->m_lifeState != LIFE_ALIVE )
//		gAimbot.DropTarget( );

	player_info_s pInfo;

	IClientEntity* pEntity = g_pEntList->GetClientEntity( iIndex );

	if( pEntity == NULL )
		return;

	C_BaseEntity* pBaseEntity = dynamic_cast< C_BaseEntity* >( pEntity );

	if( pBaseEntity == NULL )
		return;

	if( iIndex == pLocalBaseEntity->index )
		return;

	if ( pBaseEntity->m_lifeState == LIFE_ALIVE && pBaseEntity->IsDormant( ) == false && g_pEngine->GetPlayerInfo( iIndex, &pInfo ) && pBaseEntity->IsPlayer( ) )
	{
		Vector vScreen, vWorldPos;

		int iTeamNum = pBaseEntity->GetTeamNumber( );

		int iTarget = gAimbot.iGetTarget( );

		if( pBaseEntity->index == iTarget )
			iTeamNum = 16;

		DWORD dwGetTeamColor = gColorManager.dwGetColor( iTeamNum );

		if( gCvars.esp_radar )
			gRadar.DrawRadarPoint( pBaseEntity->GetAbsOrigin( ), pLocalBaseEntity->GetAbsOrigin( ) , pLocalBaseEntity->GetAbsAngles( ), pBaseEntity, dwGetTeamColor );

		GetWorldSpaceCenter( pBaseEntity, vWorldPos );

		if( gDrawManager.WorldToScreen( vWorldPos, vScreen ) == false )
			return;

		float flDistance = flGetDistance( pBaseEntity->GetAbsOrigin( ), pLocalBaseEntity->GetAbsOrigin( ) );
		int iRadius = ( 300.0 * 90.0 ) / ( flDistance * 90.0 );
		
		if( gCvars.esp_box )
		{
			gDrawManager.DrawBox( pBaseEntity->EyePosition( ), RED( dwGetTeamColor ), GREEN( dwGetTeamColor ), BLUE( dwGetTeamColor ), ALPHA( dwGetTeamColor ), 4, iRadius );
		}
		if( gCvars.esp_class )
		{
			char* pszModelClass = MakePtr( char*, pBaseEntity, 0xA5B );
			gFontManager.DrawString( false, vScreen.x, vScreen.y, dwGetTeamColor, "%s", szGetTF2Class( pszModelClass ) );
			vScreen.y += gFontManager.GetESPHeight( );
		}
		if( gCvars.esp_name )
		{
			gFontManager.DrawString( false, vScreen.x, vScreen.y, dwGetTeamColor, "%s", pInfo.name );
			vScreen.y += gFontManager.GetESPHeight( );
		}
		if( gCvars.esp_distance )
		{
			gFontManager.DrawString( false, vScreen.x, vScreen.y, dwGetTeamColor, "[%.0fm]", flDistance );
			vScreen.y += gFontManager.GetESPHeight( );
		}
		if( gCvars.esp_health )
		{
			gFontManager.DrawString( false, vScreen.x, vScreen.y, dwGetTeamColor, "%i hp", ( int ) pBaseEntity->GetHealth() );
			vScreen.y += gFontManager.GetESPHeight( );
		}
		if( gCvars.esp_weapon )
		{
			C_BaseCombatWeapon *pBaseWeapon = GetBaseCombatActiveWeapon( pBaseEntity );

			if( pBaseWeapon == NULL )
				return;
			
			int iWeaponID = iGetWeaponID( pBaseWeapon );

			if( iWeaponID == NULL )
				return;

			char* pszWeapon = szWeaponIDToAlias( iWeaponID );

			if( pszWeapon == NULL )
				return;

			pszWeapon += 10;
		
			gFontManager.DrawString( false, vScreen.x, vScreen.y, dwGetTeamColor, "%s", pszWeapon );
			vScreen.y += gFontManager.GetESPHeight( );
		}
		if( gCvars.esp_optical )
		{
			Vector vMin, vMax;
			Vector vHeadScreen, vecPointOne, vecPointTwo;
			QAngle qView;
			Vector vecDir = pBaseEntity->GetAbsOrigin( );
			float flRoll = 0.0f;

			vecDir.z = pBaseEntity->EyePosition( ).z;

			if( gDrawManager.WorldToScreen( vecDir, vHeadScreen ) )
			{
				switch( (int)gCvars.esp_optical )
				{
				case 1:
					gFontManager.DrawString( true, vHeadScreen.x, vHeadScreen.y - 25, RGBA(dwGetTeamColor), "+" );
					break;
				case 2:
					g_pSurface->DrawSetColor( RGBA( dwGetTeamColor ) );
					g_pSurface->DrawOutlinedCircle( vHeadScreen.x, vHeadScreen.y, 10, 10 );
					break;
				case 3:
					g_pSurface->DrawSetColor( RGBA( dwGetTeamColor ) );
					g_pSurface->DrawOutlinedRect( vHeadScreen.x - 5, vHeadScreen.y - 5, vHeadScreen.x + 5, vHeadScreen.y + 5 );
					break;
				case 4:
					pBaseEntity->GetRenderBounds( vMin, vMax );
					g_pDebugOverlay->AddBoxOverlay( pBaseEntity->GetAbsOrigin(), vMin, vMax, pBaseEntity->GetAbsAngles(), RED( dwGetTeamColor ), GREEN( dwGetTeamColor ), BLUE( dwGetTeamColor ), 40, 0.07f );
					break;
				case 5:
					g_pSurface->DrawSetColor( RGBA( dwGetTeamColor ) );
					g_pSurface->DrawFilledRect( vHeadScreen.x - 5, vHeadScreen.y - 5, vHeadScreen.x + 5, vHeadScreen.y + 5 );;
					break;
				case 6: 
					pBaseEntity->GetRenderBounds( vMin, vMax );

					vecPointTwo = pBaseEntity->GetAbsOrigin( );
					vecPointTwo.z += vMax.z - vMin.z;

					if ( gDrawManager.WorldToScreen( vecPointTwo, vecPointOne ) )
					{
						g_pSurface->DrawSetColor( RGBA( dwGetTeamColor ) );
						g_pSurface->DrawOutlinedRect( vecPointOne.x - 25, vecPointOne.y, vecPointOne.x + 25, vecPointOne.y + vMax.y - vMin.y );
					}
					break;
				}
			}
		}
	}
}
//===================================================================================
void CPlayerESP::DrawWorldESP( int iIndex )
{
	if( gCvars.esp_structure <= 0 )
		return;

	IClientEntity* pEntity = g_pEntList->GetClientEntity( iIndex );

	if( pEntity == NULL )
		return;

	C_BaseEntity* pBaseEntity = dynamic_cast< C_BaseEntity* >( pEntity );

	if( pBaseEntity == NULL )
		return;

	if( pBaseEntity->IsDormant( ) == false )
	{
		Vector vScreen;
		int iTeamNum;

		if ( gDrawManager.WorldToScreen( pBaseEntity->GetAbsOrigin( ), vScreen ) == false )
			return;

		DWORD dwTeamColor = gColorManager.dwGetColor( pBaseEntity->GetTeamNumber( ) );

		if( !strcmp( pBaseEntity->GetClientClass( )->GetName( ), "CCaptureFlag" ) )
		{
			gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "**Capture**" );
		}

		if( !strcmp( pBaseEntity->GetClientClass( )->GetName( ), "CTFStunBall" ) )
		{
			gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "[#]" );
		}

		if( !strcmp( pBaseEntity->GetClientClass( )->GetName( ), "CTFGrenadePipebombProjectile" ) )
		{
			gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "[*]" );
		}

		if( !strcmp( pBaseEntity->GetClientClass( )->GetName( ), "CTFProjectile_Rocket" ) )
		{
			gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "[&]" );
		}
	
		if( !strcmp( pBaseEntity->GetClientClass( )->GetName( ), "CObjectSentrygun" ) )
		{
			CObject* pObject = ToBaseObject( pBaseEntity );

			if( pObject == NULL )
				return;
			
			CObjectSentryGun* pSentryGun = reinterpret_cast< CObjectSentryGun* >( pObject );

			if( pSentryGun == NULL )
				return;

			if( pObject->m_bHasSapper )
			{
				gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "**SAPPED**", pObject->m_bHasSapper );
				vScreen.y += gFontManager.GetESPHeight( );
			}

			gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "Sentry: %i", pObject->m_iUpgradeLevel );
			vScreen.y += gFontManager.GetESPHeight( );

			gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "Health: %i - %i", pObject->m_iHealth, pObject->m_iMaxHealth );
			vScreen.y += gFontManager.GetESPHeight( );

			if( gCvars.esp_structure >= 2 )
			{
				if( pObject->m_bBuilding )
				{
					gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "Building %f", pObject->m_flPercentageConstructed * 100 );
					vScreen.y += gFontManager.GetESPHeight( );
				}

				if( pObject->m_iUpgradeLevel < 3 )
				{
					gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "Upgraded: %i", pObject->m_iUpgradeMetal / 2 );
					vScreen.y += gFontManager.GetESPHeight( );
				}

				gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "%s", pSentryGun->GetStateString( ).c_str( ) );
				vScreen.y += gFontManager.GetESPHeight( );

				gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "Shells: %i", pSentryGun->m_iAmmoShells );
				vScreen.y += gFontManager.GetESPHeight( );
			}
		}
		
		if( !strcmp( pBaseEntity->GetClientClass( )->GetName( ), "CObjectTeleporter" ) )
		{
			CObject* pObject = ToBaseObject( pBaseEntity );

			if( pObject == NULL )
				return;

			CObjectTeleporter* pTeleporter = reinterpret_cast< CObjectTeleporter* >( pObject );

			if( pTeleporter == NULL )
				return;

			if( pObject->m_bHasSapper )
			{
				gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "**SAPPED**", pObject->m_bHasSapper );
				vScreen.y += gFontManager.GetESPHeight( );
			}

			gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "Teleporter: %i", pObject->m_iUpgradeLevel );
			vScreen.y += gFontManager.GetESPHeight( );

			gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "Health: %i - %i", pObject->m_iHealth, pObject->m_iMaxHealth );
			vScreen.y += gFontManager.GetESPHeight( );
			
			if( gCvars.esp_structure >= 2 )
			{
				if( pObject->m_bBuilding )
				{
					gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "Building: %.0f", pObject->m_flPercentageConstructed * 100 );
					vScreen.y += gFontManager.GetESPHeight( );
				}

				if( pObject->m_iUpgradeLevel < 3 )
				{
					gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "Upgraded: %i", pObject->m_iUpgradeMetal / 2 );
					vScreen.y += gFontManager.GetESPHeight( );
				}

				gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "%s", pTeleporter->GetStateString( ).c_str( ) );
				vScreen.y += gFontManager.GetESPHeight( );
			}
		}

		if( !strcmp( pBaseEntity->GetClientClass( )->GetName( ), "CObjectDispenser" ) )
		{
			CObject* pObject = ToBaseObject( pBaseEntity );

			if( pObject == NULL )
				return;

			CObjectDispenser* pDispenser = reinterpret_cast< CObjectDispenser* >( pObject );

			if( pDispenser == NULL )
				return;

			if( pObject->m_bHasSapper )
			{
				gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "**SAPPED**", pObject->m_bHasSapper );
				vScreen.y += gFontManager.GetESPHeight( );
			}

			gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "Dispenser: %i", pObject->m_iUpgradeLevel );
			vScreen.y += gFontManager.GetESPHeight( );

			gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "Health: %i - %i", pObject->m_iHealth, pObject->m_iMaxHealth );
			vScreen.y += gFontManager.GetESPHeight( );

			if( gCvars.esp_structure >= 2 )
			{
				if( pObject->m_bBuilding )
				{
					gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "Building: %.0f", pObject->m_flPercentageConstructed * 100 );
					vScreen.y += gFontManager.GetESPHeight( );
				}

				if( pObject->m_iUpgradeLevel < 3 )
				{
					gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "Upgraded: %i", pObject->m_iUpgradeMetal / 2 );
					vScreen.y += gFontManager.GetESPHeight( );
				}

				gFontManager.DrawString( false, vScreen.x, vScreen.y, dwTeamColor, "Ammo: %i", pDispenser->m_iAmmoMetal );
				vScreen.y += gFontManager.GetESPHeight( );
			}
		}
	}
}
//===================================================================================
void CPlayerESP::DrawSoundESP( void )
{
	Vector vScreen;

	for ( int iIndex = 0; iIndex < 64; iIndex++ )
	{
		if ( gSoundMarker[iIndex].dwTime + 1000 < timeGetTime( ) )
			continue;

		IClientEntity* pEntity = g_pEntList->GetClientEntity( gSoundMarker[iIndex].iIndex );

		if( pEntity == NULL )
			continue;

		C_BaseEntity* pBaseEntity = dynamic_cast< C_BaseEntity* >( pEntity );

		if( pBaseEntity == NULL )
			continue;

		if( gDrawManager.WorldToScreen( gSoundMarker[iIndex].vOrigin , vScreen ) )
		{
			int iTeamNum = pBaseEntity->GetTeamNumber( );
			DWORD dwGetTeamColor = gColorManager.dwGetColor( iTeamNum );

			gFontManager.DrawString( true, vScreen.x, vScreen.y, RED( dwGetTeamColor ), GREEN( dwGetTeamColor ), BLUE( dwGetTeamColor ), 255, gSoundMarker[iIndex].strSoundName.c_str( ) );
		}
	}
}