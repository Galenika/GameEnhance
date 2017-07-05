#include <unordered_map>
#include <set>
#include <vector>
#include <algorithm>

class Weapon_t
{
public:
	bool Enabled = false;
	float Fov = 3;
	int Bone = 8;
	bool Nearest = false;
	int NearestType = 1;
	float Smooth = 2.5f;
	bool FireDelayEnabled = false;
	bool FireDelayRepeat = false;
	float FireDelay = 0;
	int StartBullet = 0;
	int EndBullet = 0;
	int RcsX = 0;
	int RcsY = 0;
	bool pSilent = false;
	int pSilentHitchance = 100;
	int pSilentBullet = 1;
	float pSilentFov = 1.0;
	float pSilentSmooth = 0;

	bool ChangerEnabled = false;
	int ChangerSkin = 0;
	char ChangerName[32] = "";
	int ChangerStatTrak = 0;
	int ChangerSeed = 0;
	float ChangerWear = 0;
};
Weapon_t* weapons = new Weapon_t[520];

struct ColorP
{
public:
	const char* Name;
	float* Ccolor;

	ColorP(const char* name, float* color)
	{
		this->Name = name;
		this->Ccolor = color;
	}
};

struct skinInfo
{
	int seed = -1;
	int paintkit;
	std::string tagName;
};

namespace G
{
	bool MenuOpened = false;
	bool PressedKeys[256] = {};
	bool Inited = false;
	bool AllowSilent = false;
	bool ForceFullUpdate = false;
	float KillDelayTime = 0;
	bool KillDelayEnd = false;
	float FireDelayTime = 0;
	bool NextResetRadar = false;
	std::unordered_map<std::string, std::set<std::string>> weaponSkins;
	std::unordered_map<std::string, skinInfo> skinMap;
	std::unordered_map<std::string, std::string> skinNames;
	std::vector<std::string> weaponNames;
	std::vector<ColorP> ColorsForPicker;
	std::vector<CPlayer*> PlayersDrawList;
	std::vector<std::string> EventList;
	std::vector<bool> VisibledPlayers(128);
	IMaterial* chamsNormalMaterial;
	IMaterial* ignoreNormalMaterial;
	IMaterial* chamsFlatMaterial;
	IMaterial* ignoreFlatMaterial;
}

namespace Options
{
	namespace Aimbot
	{
		bool Enabled = false;
		bool DrawFov = false;
		bool Deathmatch = false;
		bool SmokeCheck = false;
		bool AutoFire = false;
		int AimType = 0;
		bool KillDelay = false;
		float KillDelayTime = 0.40f;
	}
	namespace Visuals
	{
		namespace ESP
		{
			bool Enabled = false;
			bool EnemyOnly = false;
			bool VisibleOnly = false;
			bool SmokeCheck = false;
			bool Box = false;
			int Style = 1;
			bool Name = false;
			bool Health = false;
			bool Weapon = false;
			bool WeaponAmmo = false;
		}
		namespace Chams
		{
			bool Enabled = false;
			bool EnemyOnly = false;
			bool VisibleOnly = false;
			bool DrawHead = false;
			float color[3] = { 0, 1.f, 0 };
			float coloriz[3] = { 0, 0, 0 };
			int Style = 0;
		}
		namespace Hands
		{
			bool Enabled = false;
			int Style = 1; // 0 - Disabled; 1 - Wireframe; 2 - Chams; 3 - Chams & Wireframe
			float color[3] = { 0, 0.41f, 1 };
		}
		namespace Misc
		{
			bool DropESP = false;
			bool BombTimer = false;
			int BombTimerType = 2;
			bool FovChanger = false;
			float FovChangerValue = 90;
			bool ViewmodelChanger = false;
			float ViewmodelChangerValue = 90;
		}
	}
	namespace Radar
	{
		bool Enabled = false;
		int Style = 0; // 0 - External; 1 - In-Game
		int Type = 0; // 0 - Box; 1 - Filled box; 2 - Circle; 3 - Filled circle;
		bool EnemyOnly = false;
		bool VisibleOnly = false;
		bool SmokeCheck = false;
		bool ViewCheck = false;
		int Alpha = 0;
		float Zoom = 1;
	}
	namespace SkinChanger
	{
		bool EnabledSkin = false;
		bool EnabledKnife = false;
		bool EnabledGlove = false;
		int Knife = 3; // 0 - Bayonet, 1 - Flip, 2 - Gut, 3 - Karambit, 4 - M9Bayonet, 5 - Huntsman, 6 - Falchion, 7 - Bowie, 8 - Butterfly, 9 - Daggers, 10 - Golden
		int Glove = 4; // 0 - Bloodhound; 1 - Sport; 2 - Driver; 3 - Wraps; 4 - Moto; 5 - Specialist
		int GloveSkin = 0;
	}
	namespace Misc
	{
		bool RevealRanks = false;
		bool Bunnyhop = false;
		bool SpectatorList = false;
		bool RecoilCrosshair = false;
		bool SniperCrosshair = false;
		bool HitInfo = false;
		bool NoFlash = false;
		bool KnifeBot = false;
		bool KnifeBotAuto = false;
		bool KnifeBot360 = false;
		bool Copyright = false;
		float SpeedControl = 0.1f;
		float NoFlashAlpha = 100;
		bool ClanTagAnim = false;
		int ClanTagAnimStyle = 1;
		char ClanTabAnimText[15] = "";
	};
};