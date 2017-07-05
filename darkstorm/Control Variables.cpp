#include <Windows.h>
#include <stdio.h>
#include <fstream>
#include "SDK.h"
#include "Control Variables.h"
#include "File Manager.h"
//#include "cMenu.h"
//===================================================================================
CControlVariables gCvars;
//===================================================================================
void CControlVariables::Initialize( void )
{
	misc_menu_x = 300;
	misc_menu_y = 75;
	misc_menu_w = 150;
}
//===================================================================================
void CControlVariables::Save( void )
{
	gFileManager.WriteFloat( "Cvars", "Aimbot" , aim_bot );
	gFileManager.WriteFloat( "Cvars", "Aim Key", aim_key );
	gFileManager.WriteFloat( "Cvars", "Aim FOV", aim_fov );
	gFileManager.WriteFloat( "Cvars", "Auto Aim", aim_auto_aim );
	gFileManager.WriteFloat( "Cvars", "Aim Mode", aim_mode );
	gFileManager.WriteFloat( "Cvars", "Aim Team", aim_team );
	gFileManager.WriteFloat( "Cvars", "Aim Spot", aim_spot );
	gFileManager.WriteFloat( "Cvars", "Aim Lock", aim_lock );
	gFileManager.WriteFloat( "Cvars", "Aim Method", aim_method );
	gFileManager.WriteFloat( "Cvars", "Const Crits", aim_constcrits );
	gFileManager.WriteFloat( "Cvars", "Smooth Aim", aim_smooth );
	gFileManager.WriteFloat( "Cvars", "Trigger Bot", aim_triggerbot );
	gFileManager.WriteFloat( "Cvars", "Name ESP", esp_name );
	gFileManager.WriteFloat( "Cvars", "Health ESP", esp_health );
	gFileManager.WriteFloat( "Cvars", "Weapon ESP", esp_weapon );
	gFileManager.WriteFloat( "Cvars", "Distance ESP", esp_distance );
	gFileManager.WriteFloat( "Cvars", "Box ESP", esp_box );
	gFileManager.WriteFloat( "Cvars", "Class ESP", esp_class );
	gFileManager.WriteFloat( "Cvars", "Structure ESP", esp_structure );
	gFileManager.WriteFloat( "Cvars", "GFX ESP", esp_optical );
	gFileManager.WriteFloat( "Cvars", "Radar", esp_radar );
	gFileManager.WriteFloat( "Cvars", "Radar X", esp_radar_x );
	gFileManager.WriteFloat( "Cvars", "Radar Y", esp_radar_y );
	gFileManager.WriteFloat( "Cvars", "Radar W", esp_radar_w );
	gFileManager.WriteFloat( "Cvars", "Radar Altitude", esp_radar_altitude );
	gFileManager.WriteFloat( "Cvars", "Bunny Hop", misc_bunnyhop );
	gFileManager.WriteFloat( "Cvars", "Duck Shoot", misc_duckshoot );
	gFileManager.WriteFloat( "Cvars", "Duck Jump", misc_duckjump );
	gFileManager.WriteFloat( "Cvars", "Auto Pistol", misc_autopistol );
	gFileManager.WriteFloat( "Cvars", "Enable Speed", misc_speed_on );
	gFileManager.WriteFloat( "Cvars", "Speed Key", misc_speed_key );
	gFileManager.WriteFloat( "Cvars", "Speed Variable", misc_speed );
	gFileManager.WriteFloat( "Cvars", "Speed Mode", misc_speed_mode );
	gFileManager.WriteFloat( "Cvars", "Sniper Removal", misc_sniper );
	gFileManager.WriteFloat( "Cvars", "Third Person", misc_thirdperson );
	gFileManager.WriteFloat( "Cvars", "Clean Screenshots", misc_antiss );
	gFileManager.WriteFloat( "Cvars", "Crosshair", misc_cross );
	gFileManager.WriteFloat( "Cvars", "Wallhack", misc_wallhack );
	gFileManager.WriteFloat( "Cvars", "ModelHack", misc_modelhack );
	gFileManager.WriteFloat( "Cvars", "Spinbot", misc_spinbot );
	gFileManager.WriteFloat( "Cvars", "Aim Method", aim_method );
	gFileManager.WriteFloat( "Cvars", "Nospread", aim_nospread );
	gFileManager.WriteFloat( "Cvars", "Prediction", aim_prediction );
	gFileManager.WriteFloat( "Cvars", "Achievement Spam", misc_achivementspam );
	gFileManager.WriteFloat( "Cvars", "Sound ESP", esp_sound );
}
//===================================================================================
void CControlVariables::Load( void )
{
	aim_bot =			gFileManager.ReadFloat( "Cvars", "Aimbot" , aim_bot );
	aim_key =			gFileManager.ReadFloat( "Cvars", "Aim Key", aim_key );
	aim_fov =			gFileManager.ReadFloat( "Cvars", "Aim FOV", aim_fov );
	aim_auto_aim =		gFileManager.ReadFloat( "Cvars", "Auto Aim", aim_auto_aim );
	aim_mode =			gFileManager.ReadFloat( "Cvars", "Aim Mode", aim_mode );
	aim_team =			gFileManager.ReadFloat( "Cvars", "Aim Team", aim_team );
	aim_spot =			gFileManager.ReadFloat( "Cvars", "Aim Spot", aim_spot );
	aim_lock =			gFileManager.ReadFloat( "Cvars", "Aim Lock", aim_lock );
	aim_method =		gFileManager.ReadFloat( "Cvars", "Aim Method", aim_method );
	aim_constcrits =	gFileManager.ReadFloat( "Cvars", "Const Crits", aim_constcrits );
	aim_smooth =		gFileManager.ReadFloat( "Cvars", "Smooth Aim", aim_smooth );
	aim_triggerbot =	gFileManager.ReadFloat( "Cvars", "Trigger Bot", aim_triggerbot );
	aim_nospread =		gFileManager.ReadFloat( "Cvars", "Nospread", aim_nospread );
	aim_prediction =	gFileManager.ReadFloat( "Cvars", "Prediction", aim_prediction );
	esp_name =			gFileManager.ReadFloat( "Cvars", "Name ESP", esp_name );
	esp_health =		gFileManager.ReadFloat( "Cvars", "Health ESP", esp_health );
	esp_weapon =		gFileManager.ReadFloat( "Cvars", "Weapon ESP", esp_weapon );
	esp_distance =		gFileManager.ReadFloat( "Cvars", "Distance ESP", esp_distance );
	esp_box =			gFileManager.ReadFloat( "Cvars", "Box ESP", esp_box );
	esp_class =			gFileManager.ReadFloat( "Cvars", "Class ESP", esp_class );
	esp_structure =		gFileManager.ReadFloat( "Cvars", "Structure ESP", esp_structure );
	esp_optical =		gFileManager.ReadFloat( "Cvars", "GFX ESP", esp_optical );
	esp_radar =			gFileManager.ReadFloat( "Cvars", "Radar", esp_radar );
	esp_radar_x =		gFileManager.ReadFloat( "Cvars", "Radar X", esp_radar_x );
	esp_radar_y =		gFileManager.ReadFloat( "Cvars", "Radar Y", esp_radar_y );
	esp_radar_w =		gFileManager.ReadFloat( "Cvars", "Radar W", esp_radar_w );
	esp_radar_altitude = gFileManager.ReadFloat( "Cvars", "Radar Altitude", esp_radar_altitude );
	misc_bunnyhop =		gFileManager.ReadFloat( "Cvars", "Bunny Hop", misc_bunnyhop );
	misc_duckshoot =	gFileManager.ReadFloat( "Cvars", "Duck Shoot", misc_duckshoot );
	misc_duckjump =		gFileManager.ReadFloat( "Cvars", "Duck Jump", misc_duckjump );
	misc_autopistol =	gFileManager.ReadFloat( "Cvars", "Auto Pistol", misc_autopistol );
	misc_speed_on =		gFileManager.ReadFloat( "Cvars", "Enable Speed", misc_speed_on );
	misc_speed_key =	gFileManager.ReadFloat( "Cvars", "Speed Key", misc_speed_key );
	misc_speed =		gFileManager.ReadFloat( "Cvars", "Speed Variable", misc_speed );
	misc_speed_mode =	gFileManager.ReadFloat( "Cvars", "Speed Mode", misc_speed_mode );
	misc_sniper =		gFileManager.ReadFloat( "Cvars", "Sniper Removal", misc_sniper );
	misc_thirdperson =	gFileManager.ReadFloat( "Cvars", "Third Person", misc_thirdperson );
	misc_antiss =		gFileManager.ReadFloat( "Cvars", "Clean Screenshots", misc_antiss );
	misc_cross =		gFileManager.ReadFloat( "Cvars", "Crosshair", misc_cross );
	misc_wallhack =		gFileManager.ReadFloat( "Cvars", "Wallhack", misc_wallhack );
	misc_modelhack =	gFileManager.ReadFloat( "Cvars", "ModelHack", misc_modelhack );
	misc_spinbot =		gFileManager.ReadFloat( "Cvars", "Spinbot", misc_spinbot );
	misc_achivementspam = gFileManager.ReadFloat( "Cvars", "Achievement Spam", misc_achivementspam );
	esp_sound =			gFileManager.ReadFloat( "Cvars", "Sound ESP", esp_sound );
}
//===================================================================================