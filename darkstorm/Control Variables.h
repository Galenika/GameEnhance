#ifndef _CONTROL_VARIABLES_H_
#define _CONTROL_VARIABLES_H_
//===================================================================================
class CControlVariables
{
public:

	void Initialize( void );
	void Load( void );
	void Save( void );

	float aim_bot;
	float aim_auto_aim;
	float aim_auto_shoot;
	float aim_key;
	float aim_recoil;
	float aim_fov;
	float aim_mode;
	float aim_thru;
	float aim_shoot;
	float aim_spot;
	float aim_team;
	float aim_smooth;
	float aim_triggerbot;
	float aim_draw;
	float aim_constcrits;
	float aim_lock;
	float aim_prediction;
	float aim_method;
	float aim_nospread;

	float esp_name;
	float esp_distance;
//	float esp_entities;
	float esp_weapon;
	float esp_optical;
	float esp_health;
	float esp_sound;
	float esp_box;
	float esp_class;
	float esp_structure;

	float esp_radar;
	float esp_radar_x;
	float esp_radar_y;
	float esp_radar_w;
	float esp_radar_range;
	float esp_radar_altitude;

//	float esp_style;

//	float wall_entity_hack;

	float misc_menu_x;
	float misc_menu_y;
	float misc_menu_w;

	float misc_wallhack;
	float misc_cross;
	float misc_autopistol;
	float misc_bunnyhop;
	float misc_speed_on;
	float misc_speed;
	float misc_speed_key;
	float misc_speed_mode;
	float misc_spinbot;
	float misc_thirdperson;
	float misc_duckjump;
	float misc_duckshoot;
	float misc_sniper;
	float misc_antiss;
	float misc_block;
	float misc_modelhack;
	float misc_achivementspam;
};
//===================================================================================
extern CControlVariables gCvars;
//===================================================================================
#endif