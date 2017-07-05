#include "SDK.h"
#include "Client.h"
#include "Panels.h"
#include "Utilities.h"
#include "Achievements.h"
#include "Memory Tools.h"
#include "Extra Functions.h"
#include "Control Variables.h"
#include "Weapon List.h"
#include "Cheat Menu.h"
#include "Aimbot.h"
//===================================================================================
CClient gClient;
static CDrawLayer gDrawLayer;
//===================================================================================
void ( *SetPredictionRandomSeed )( const CUserCmd *pCommand );

LARGE_INTEGER* g_lpSpeedpointer = NULL;
LARGE_INTEGER g_RealFrequency;

DWORD dwLastReal = NULL;
DWORD dwLastFake = NULL;
DWORD dwLastTicks = NULL;
//===================================================================================
int __stdcall Hooked_Init( CreateInterfaceFn appSysFactory, CreateInterfaceFn physicsFactory, CGlobalVarsBase* pGlobals )
{
	g_AppSysFactory = appSysFactory;
	gBaseAPI.LogToFile( "g_AppSysFactory: [0x%.8X]", (DWORD)g_AppSysFactory );
	XASSERT( g_AppSysFactory );

	g_pGlobals = pGlobals;
	gBaseAPI.LogToFile( "g_pGlobals: [0x%.8X]", (DWORD)g_pGlobals );
	XASSERT( g_pGlobals );

	g_pCvar = ( ICvar* ) g_AppSysFactory( CVAR_INTERFACE_VERSION, NULL );
	gBaseAPI.LogToFile( "g_pCvar: [0x%.8X]", (DWORD)g_pCvar );
	XASSERT( g_pCvar );

	
	return gClient.Init( appSysFactory, physicsFactory, pGlobals );
}
//===================================================================================
void __stdcall Hooked_HudUpdate( bool bActive )
{
	static bool bOnce = true;

	if( bOnce && g_pEngine )
	{
		g_pEngine->ExecuteClientCmd( "clear" );
		g_pEngine->ExecuteClientCmd( "toggleconsole" );

		MathLib_Init( 2.2f, 2.2f, 0.0f, 2.0f, true, true, true, true );

		ConnectTier1Libraries( &g_AppSysFactory, 1 );
		ConnectTier2Libraries( &g_AppSysFactory, 1 );
		ConnectTier3Libraries( &g_AppSysFactory, 1 );

		vgui::VGui_InitInterfacesList( "TF2Base.dll", &g_AppSysFactory, 1 );

		vgui::VPANEL vpParent = g_pEngineVGUI->GetPanel( PANEL_CLIENTDLL );
		gDrawLayer.Create( vpParent );

		bOnce = false;
	}

	gClient.HudUpdate( bActive );
}
//===================================================================================
void __stdcall Hooked_CreateMove( int sequence_number, float input_sample_frametime, bool active )
{
	gClient.CreateMove( sequence_number, input_sample_frametime, active );

	if( g_pInput == NULL )
	{
		gAimbot.AimAtTarget( NULL );
	}
	else
	{
		if( g_pEngine->IsLevelMainMenuBackground( ) || g_pEngine->IsDrawingLoadingImage( ) || g_pEngine->IsInGame( ) == false )
			return;

		CUserCmd* pCommand = g_pInput->GetUserCmd( sequence_number ); 

		if( pCommand )
		{
			C_BaseEntity* pBaseEntity = dynamic_cast< C_BaseEntity* >( g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer( ) ) );

			if( pBaseEntity == NULL )
				return;

			C_BaseCombatWeapon* pBaseWeapon = GetBaseCombatActiveWeapon( pBaseEntity );

			if( pBaseWeapon == NULL )
				return;

			gAimbot.FindTarget( );
			gAimbot.AimAtTarget( pCommand );

			if( gCvars.misc_achivementspam )
			{
				gAchievements.Think( );
			}

			if ( gCvars.aim_triggerbot && bTraceToPlayer( ) )
			{
				pCommand->buttons |= IN_ATTACK;
			}

			if( gCvars.misc_duckjump && pCommand->buttons & IN_JUMP )
			{
				pCommand->buttons |= IN_DUCK;
			}

			if( gCvars.misc_bunnyhop && pCommand->buttons & IN_JUMP )
			{
				int iFlags = pBaseEntity->GetFlags( );

				if( !(iFlags & FL_ONGROUND) )
					pCommand->buttons &= ~IN_JUMP;
			}

			if ( gCvars.misc_duckshoot && pCommand->buttons & IN_ATTACK )
			{
				pCommand->buttons |= IN_DUCK;
			}

			if( gCvars.misc_autopistol && pCommand->buttons & IN_ATTACK && ( iGetWeaponID( pBaseWeapon ) == WEAPONLIST_SCOUTPISTOL || iGetWeaponID( pBaseWeapon ) == WEAPONLIST_ENGINEERPISTOL || iGetWeaponID( pBaseWeapon ) == WEAPONLIST_SPYPISTOL ) )
			{
				static bool bInAttack = false;
				if ( bInAttack )
					pCommand->buttons |=  IN_ATTACK;
				else
					pCommand->buttons &= ~IN_ATTACK;

				bInAttack = !bInAttack;
			}

			if( g_pCvar )
			{
				static bool bThird = false;
				if ( gCvars.misc_thirdperson && !bThird)
				{
					ConVar* pThirdCamYaw = g_pCvar->FindVar( "cam_idealyaw" );
					g_pEngine->ClientCmd_Unrestricted( "thirdperson" );

					if ( pThirdCamYaw )
					{
						pThirdCamYaw->SetValue( 0 );
					}

					bThird = true;
				}
				else if( !gCvars.misc_thirdperson && bThird )
				{
					g_pEngine->ClientCmd_Unrestricted( "firstperson" );
					bThird = false;
				}
			}

			if( gCvars.aim_nospread == 1 && pCommand->buttons & IN_ATTACK && bIsWeaponSpread( iGetWeaponID( pBaseWeapon ) ) )
			{
				Vector vForward, vRight, vUp;
				QAngle qSpreadAngle;

				float flSpread = flGetWeaponSpread( pBaseWeapon );
				
				RandomSeed( pCommand->random_seed & 255 );

				float x = RandomFloat( -0.5, 0.5 ) + RandomFloat( -0.5, 0.5 );
				float y = RandomFloat( -0.5, 0.5 ) + RandomFloat( -0.5, 0.5 );

/*				if ( pCommand->viewangles[0] > 180 )
					pCommand->viewangles[0] -= 360;
				else if ( pCommand->viewangles[0] < -180 )
					pCommand->viewangles[0] += 360;

				if ( pCommand->viewangles[1] > 180 )	
					pCommand->viewangles[1] -= 360;
				else if ( pCommand->viewangles[1] < -180 )
					pCommand->viewangles[1] += 360;
*/
				AngleVectors( pCommand->viewangles, &vForward, &vRight, &vUp );

				Vector vShot = vForward + ( vRight * x * flSpread ) + ( vUp * y * flSpread ); 

				vShot.NormalizeInPlace( ); 

				vForward = vForward + ( vForward - vShot ); 

				VectorAngles( vForward, qSpreadAngle ); 

				//ToDo: Clamp viewangles > 180 || < 0 
				pCommand->viewangles = qSpreadAngle; 

			}

			if( gCvars.aim_nospread == 2 )
			{
				//strange "almost" nospread trick
				if( pCommand->buttons & IN_ATTACK )
				{
					pCommand->random_seed = 150;
					pCommand->buttons |= IN_ATTACK;
				}
			}


			if( gCvars.aim_constcrits && !bIsBlacklistCrit( iGetWeaponID( pBaseWeapon ) ) )
			{
				static bool bWaitFire = false;

				int iSeed = pCommand->random_seed;
				int iResult = 0;     

				SetPredictionRandomSeed( pCommand );

				RandomSeed( iSeed );

				//	int	iRandomInt = RandomInt ( 0, 0x270F );
				//	if ( iRandomInt <= 10000 * GetCritTime( iWeaponID ) )
				//		log_to_console("bWillCrit");
				//	else
				//		log_to_console("bWontCrit");

				if( pBaseWeapon ) 
				{
					__asm
					{
						mov ecx, pBaseWeapon;
						mov eax, [ecx+0x16B4]; //save persistent seed
						push eax;
						mov eax, [ecx];
						mov eax, [eax+0x528]; //IsShotCritical
						call eax;
						mov iResult, eax; // Save value at AL
						pop eax;
						mov ecx, pBaseWeapon;
						mov [ecx+0x16B4], eax; //restore persistent seed
					}     
				}
				else
					iResult = 0;


				if( pCommand->buttons & IN_ATTACK )
				{
					pCommand->buttons &= ~IN_ATTACK;
					bWaitFire = true;
				}
				if( bWaitFire && (BYTE)iResult )
				{
					pCommand->buttons |= IN_ATTACK;
					bWaitFire = false;    
				}
			}

			if( gCvars.misc_spinbot )
			{
				Spinbot( pCommand );
			}

			if( gCvars.misc_speed_on && g_pCvar )
			{
				ConVar* pSpeed = g_pCvar->FindVar( "host_timescale" );

				if( pSpeed )
				{
					if( gCvars.misc_speed > 1 && bIsSpeedKey( gCvars.misc_speed_key ) )
					{
						pSpeed->SetValue( gCvars.misc_speed );
					}
					else
					{
						pSpeed->SetValue( 1.0f );
					}
				}
			}

/*			//double dblCurTime = dblGetPlatRealTime( );

			if ( gCvars.misc_speed > 1 && gCvars.misc_speed_on )
			{
				if ( g_lpSpeedpointer == NULL )
				{
					g_lpSpeedpointer = *( LARGE_INTEGER** ) ( (DWORD) Plat_FloatTime + 0x59 );// 0x59 might have changed but I doubt it

					g_RealFrequency.HighPart = g_lpSpeedpointer->HighPart;
					g_RealFrequency.LowPart = g_lpSpeedpointer->LowPart;
				}
				else
				{
					g_lpSpeedpointer->LowPart = g_RealFrequency.LowPart / gCvars.misc_speed;
				}
			}
			else
			{
				g_lpSpeedpointer->LowPart = ( g_lpSpeedpointer->LowPart - g_RealFrequency.HighPart ) * gCvars.misc_speed;
			}
*/			
		}

		

		CSafeUserCmd* pSafeCommand = GetSafeUserCmd( sequence_number );

		__asm
		{
			push pCommand;
			mov ecx, pSafeCommand;
			call gVerifyFuncs.SaveUserCmd0;
			mov ecx, pCommand;
			call gVerifyFuncs.SaveUserCmd1;
		}
	}
}
//===================================================================================
int __stdcall Hooked_IN_KeyEvent ( int eventcode, int keynum, const char *pszCurrentBinding )
{
	if(eventcode == 1)
	{
		if( keynum == 72 ) //insert
		{
			gCheatMenu.bMenuActive = !gCheatMenu.bMenuActive;
			gCvars.Save( );
		}

		if(gCheatMenu.bMenuActive)
		{
			if(keynum == 88 || keynum == 112) // Up
			{
				if( gCheatMenu.iMenuIndex > 0 ) gCheatMenu.iMenuIndex--;
				else gCheatMenu.iMenuIndex = gCheatMenu.iMenuItems - 1;
				return 0;
			}
			else if(keynum == 90 || keynum == 113 ) // Down
			{
				if( gCheatMenu.iMenuIndex < gCheatMenu.iMenuItems - 1 ) gCheatMenu.iMenuIndex++;
				else gCheatMenu.iMenuIndex = 0;
				return 0;
			}
			else if(keynum == 89 || keynum == 107 ) // Left
			{
				if( gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value )
				{
					gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] -= gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flStep;
					if( gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] < gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flMin )
						gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] = gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flMax;
				}
				return 0;
			}
			else if(keynum == 91 || keynum == 108 ) // Right
			{
				if( gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value )
				{
					gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] += gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flStep;
					if( gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] > gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flMax )
						gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] = gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flMin;
				}
				return 0;
			}
		}

		if( g_pEngine->IsInGame( ) && gCheatMenu.bMenuActive == false ) 
		{
			if( gCvars.misc_speed_on && keynum == 107 && eventcode && !gCvars.misc_speed_mode )
				return 0;
			if( gCvars.misc_speed_on && keynum == 108 && eventcode && !gCvars.misc_speed_mode )
				return 0;
			if( gCvars.misc_speed_on && keynum == 109 && eventcode && !gCvars.misc_speed_mode )
				return 0;
			if( gCvars.misc_speed_on && keynum == 16 && eventcode && !gCvars.misc_speed_mode )
				return 0;
			if( gCvars.misc_speed_on && keynum == 79 && eventcode && !gCvars.misc_speed_mode )
				return 0;
		}
	}
	return gClient.IN_KeyEvent( eventcode, keynum, pszCurrentBinding );
}
//===================================================================================