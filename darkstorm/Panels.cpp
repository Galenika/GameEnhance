#include "SDK.h"
#include "Panels.h"
#include "Cheat Menu.h"
#include "Utilities.h"
#include "Font Manager.h"
#include "Draw Manager.h"
#include "Color Manager.h"
#include "File Manager.h"
#include "Control Variables.h"
#include "Model Render.h"
#include "Player ESP.h"
#include "Radar.h"
#include "Extra Functions.h"
#include "Aimbot.h"
#include "Player Manager.h"
//===================================================================================
CScreenSize gScreenSize;
//===================================================================================
CDrawLayer::CDrawLayer ( void )
{
	pDrawPanel = NULL;
}
//===================================================================================
CDrawLayer::~CDrawLayer ( void )
{
	Destroy();
}
//===================================================================================
void CDrawLayer::Create( vgui::VPANEL parent )
{
	pDrawPanel = new CDrawPanel ( parent );
}
//===================================================================================
void CDrawLayer::Destroy( void )
{
	if ( pDrawPanel )
	{
		ExitProcess( 0 );
		pDrawPanel->SetParent( (vgui::Panel *)NULL );
		delete pDrawPanel;
		pDrawPanel = NULL;
	}
}
//===================================================================================
CDrawPanel::CDrawPanel( vgui::VPANEL parent ) : BaseClass( NULL, "staticDrawOurPanel" )
{
	SetParent( parent );
	SetPos( 0, 0 );
	SetVisible( true );

	g_pEngine->GetScreenSize( gScreenSize.iScreenWidth, gScreenSize.iScreenHeight );
	
	SetSize( gScreenSize.iScreenWidth, gScreenSize.iScreenHeight );

	gFontManager.Initialize( );
	gFileManager.Initialize( gBaseAPI.GetDirectoryFile( "Darkstorm.ini" ) );
	gColorManager.Initialize( );
	gCvars.Initialize( );
	gCheatMenu.Initialize( );

	gCvars.Load( );

}
//===================================================================================
void CDrawPanel::Paint( void ) 
{
	if( g_pEngine->IsLevelMainMenuBackground( ) || g_pEngine->Con_IsVisible( ) || !g_pEngine->IsInGame( ) || !g_pEngine->IsConnected( ) )
		return;

	if( g_pEngine->IsTakingScreenshot( ) && gCvars.misc_antiss )
		return;

	if( gCvars.misc_speed_on || gCvars.misc_thirdperson || gCvars.misc_modelhack )
		UnprotectCvars( );

	//RESOLUTION FIX
	int iNewWidth, iNewHeight;
	g_pEngine->GetScreenSize( iNewWidth, iNewHeight );
	if( gScreenSize.iScreenWidth != iNewWidth || gScreenSize.iScreenHeight != iNewHeight )
	{
		g_pEngine->GetScreenSize( gScreenSize.iScreenWidth, gScreenSize.iScreenHeight );
	}

	if( gCheatMenu.bMenuActive )
		gCheatMenu.DrawMenu( );

	if( gCvars.esp_radar )
		gRadar.DrawRadarBack( );

	if( gCvars.misc_sniper )
		RemoveSniperScope( );

	if( gCvars.esp_sound )
		gPlayerESP.DrawSoundESP( );

	for( int iIndex = 0; iIndex < g_pEntList->GetHighestEntityIndex( ); iIndex++ )
	{
		gPlayerESP.DrawPlayerESP( iIndex );
		gPlayerESP.DrawWorldESP( iIndex );
	}

	gDrawManager.DrawCrosshair( gCvars.misc_cross );

}
//===================================================================================