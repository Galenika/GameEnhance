#include <windows.h>
#include "Draw Manager.h"
#include "Player Manager.h"
#include "Font Manager.h"
#include "Color Manager.h"
#include "Control Variables.h"
#include "Cheat Menu.h"
#include "Xor String.h"

CCheatMenu gCheatMenu;

int CCheatMenu::AddItem(int nIndex, char szTitle[128], float* value, float flMin, float flMax, float flStep )
{
	strcpy( pMenu[nIndex].szTitle, szTitle );
	pMenu[nIndex].value = value;
	pMenu[nIndex].flMin = flMin;
	pMenu[nIndex].flMax = flMax;
	pMenu[nIndex].flStep = flStep;
	return ( nIndex + 1 );
}

void RGB_DWORD(DWORD &dwColor, BYTE r, BYTE g, BYTE b)
{
  dwColor = (r << 24) | (g << 16) | (b << 8);
}

DWORD dwWhite = NULL;
DWORD dwGreen = NULL;

void CCheatMenu::Initialize( void )
{
	RGB_DWORD(dwWhite, 255, 255, 255);
	RGB_DWORD(dwGreen, 0, 255, 0);

	int i = 0;
	i = AddItem(i, " - Aim Bot", &gCvars.aim_bot, 0, 1, 1);
	i = AddItem(i, " - Aim Key", &gCvars.aim_key, 0, 4, 1);
	i = AddItem(i, " - Aim FOV", &gCvars.aim_fov, 1, 360, 1);
	i = AddItem(i, " - Auto Aim", &gCvars.aim_auto_aim, 0, 1, 1);
	i = AddItem(i, " - Aim Method", &gCvars.aim_method, 1, 2, 1 );
	i = AddItem(i, " - Aim Mode", &gCvars.aim_mode, 0, 1, 1 );
	i = AddItem(i, " - Aim Lock", &gCvars.aim_lock, 0, 1, 1 );
	i = AddItem(i, " - Aim Team", &gCvars.aim_team, 0, 2, 1);
	i = AddItem(i, " - Aim Spot", &gCvars.aim_spot, 1, 5, 1 );
	i = AddItem(i, " - Aim Thru", &gCvars.aim_thru, 0, 1, 1 );
	i = AddItem(i, " - SpinBot", &gCvars.misc_spinbot, 0, 1, 1 );
	i = AddItem(i, " - Prediction", &gCvars.aim_prediction, 0, 1, 1 );
	i = AddItem(i, " - Nospread", &gCvars.aim_nospread, 0, 2, 1 );
	i = AddItem(i, " - Const Crits", &gCvars.aim_constcrits, 0, 1, 1);
	i = AddItem(i, " - Smooth Aim", &gCvars.aim_smooth, 0, 100, 1);
	i = AddItem(i, " - Trigger Bot", &gCvars.aim_triggerbot, 0, 1, 1);
	i = AddItem(i, " - Name ESP", &gCvars.esp_name, 0, 1, 1);
	i = AddItem(i, " - Health ESP", &gCvars.esp_health, 0, 1, 1);
	i = AddItem(i, " - Weapon ESP", &gCvars.esp_weapon, 0, 1, 1);
	i = AddItem(i, " - Distance ESP", &gCvars.esp_distance, 0, 1, 1);
	i = AddItem(i, " - Box ESP", &gCvars.esp_box, 0, 1, 1);
	i = AddItem(i, " - Class ESP", &gCvars.esp_class, 0, 1, 1);
	i = AddItem(i, " - Sound ESP", &gCvars.esp_sound, 0, 1, 1);
	i = AddItem(i, " - Structure ESP", &gCvars.esp_structure, 0, 2, 1);
	i = AddItem(i, " - GFX ESP", &gCvars.esp_optical, 0, 6, 1);
	i = AddItem(i, " - Radar", &gCvars.esp_radar, 0, 1, 1);
	i = AddItem(i, " - Radar X", &gCvars.esp_radar_x, 0, 10000, 10);
	i = AddItem(i, " - Radar Y", &gCvars.esp_radar_y, 0, 10000, 10);
	i = AddItem(i, " - Radar W", &gCvars.esp_radar_w, 0, 1000, 10);
	i = AddItem(i, " - Radar Altitude", &gCvars.esp_radar_altitude, 0, 1, 1);
	i = AddItem(i, " - Bunny Hop", &gCvars.misc_bunnyhop, 0, 1, 1 );
	i = AddItem(i, " - Duck Shoot", &gCvars.misc_duckshoot, 0, 1, 1 );
	i = AddItem(i, " - Duck Jump", &gCvars.misc_duckjump, 0, 1, 1 );
	i = AddItem(i, " - Auto Pistol", &gCvars.misc_autopistol, 0, 1, 1);
	i = AddItem(i, " - Enable Speed", &gCvars.misc_speed_on, 0, 1, 1);
	i = AddItem(i, " - Speed Key", &gCvars.misc_speed_key, 1, 5, 1);
	i = AddItem(i, " - Speed Variable", &gCvars.misc_speed, 1, 25, 1 );
	i = AddItem(i, " - Speed Mode", &gCvars.misc_speed_mode, 0, 1, 1);
	i = AddItem(i, " - Sniper Removal", &gCvars.misc_sniper, 0, 1, 1);
	i = AddItem(i, " - Third Person", &gCvars.misc_thirdperson, 0, 1, 1);
	i = AddItem(i, " - Clean Screenshots", &gCvars.misc_antiss, 0, 1, 1);
	i = AddItem(i, " - CrossHair", &gCvars.misc_cross, 0, 4, 1 );
	i = AddItem(i, " - Wallhack", &gCvars.misc_wallhack, 0, 1, 1);
	i = AddItem(i, " - Model Hack", &gCvars.misc_modelhack, 0, 1, 1);
	i = AddItem(i, " - Achievement Spam", &gCvars.misc_achivementspam, 0, 1, 1 );
	
	iMenuItems = i;
}

void CCheatMenu::DrawMenu( void )
{
	int x = gCvars.misc_menu_x,
		xx = x + 105,
		y = gCvars.misc_menu_y,
		w = gCvars.misc_menu_w,
		h = gFontManager.GetHUDHeight( );

	gDrawManager.DrawRect( x , y - ( h + 4 ) , w, iMenuItems * 11 + 21, gColorManager.dwGetColor( 5 ) );
	gDrawManager.OutlineRect( x, y - ( h + 4 ), w, ( h + 4 ) , gColorManager.dwGetColor( 5 ) );
	gDrawManager.OutlineRect( x, y - ( h + 4 ), w, iMenuItems * 11 + 21, gColorManager.dwGetColor( 14 ) );

	gFontManager.DrawString( true, x + 12, y -( h + 2 ), gColorManager.dwGetColor( 14 ), /*Project VDC: Darkstorm*/XorStr<0xC5,23,0x723CAF46>("\x95\xB4\xA8\xA2\xAC\xA9\xBF\xEC\x9B\x8A\x8C\xEA\xF1\x96\xB2\xA6\xBE\xA5\xA3\xB7\xAB\xB7"+0x723CAF46).s);

	for( int i = 0; i < iMenuItems; i++ )
	{
		if( i != iMenuIndex )
		{
			gFontManager.DrawString( true, x + 2, y + (11*i),  gColorManager.dwGetColor( 15 ),  pMenu[i].szTitle);
			gFontManager.DrawString( true, xx, y + (11*i), gColorManager.dwGetColor( 15 ), "%2.2f", pMenu[i].value[0] );
		}
		else
		{
			gDrawManager.DrawRect( x + 1, y + (11*i) , w - 2, h, COLORCODE(255, 255, 255, 80) );
			gFontManager.DrawString( true,  x + 2, y + (11*i), gColorManager.dwGetColor( 14 ),  pMenu[i].szTitle );
			gFontManager.DrawString( true, xx, y + (11*i), gColorManager.dwGetColor( 14 ),  "%2.2f", pMenu[i].value[0] );
		}
	}
}