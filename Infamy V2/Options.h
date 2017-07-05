#pragma once

class cOptions
{
public:
	struct
	{
		struct
		{
			float 	TeamColor[3];
		}Glow;
		struct
		{
			int		Hands;
			bool	Weapon;
			bool	VisualRecoil;
			bool	Flash;
			bool	Smoke;
			bool    Recoil;
		} Removals;
		struct
		{
			int	customName;
		} Aesthetics;
		struct
		{
			bool DrawGray;
		} ConVar;
		struct
		{
			bool    Enabled = 1;
			bool    EnemyOnly;
			bool    D3D;
			int     Mode;
			bool    Box = 1;
			bool    pInfo;
			int    Health = 2;
			bool    Name = 1;
			bool    Weapon = 1;
			bool	AutoAccept;
			bool	Glow = 1;
			int     IsVisible;
			int		FOV = 40;
			int     FOV2 = 20;
			bool    ThirdPerson;
			float	VisibleColor[3];
			float	NonVisibleColor[3];
			bool RankESP;
			bool fpsBoost;
			bool cumMode;
			bool DrawBomb;
			bool DrawDefuser;
			bool DrawGlow;
			bool BombTimer;
			bool FakeLag;
			bool Chams;
			bool IzumiName;
			bool NoName;
			bool Watermark = true;
			int DrawGlowS;
			bool Ammo;

		}ESP;
		struct
		{
			int Enabled;
			char* buf;
			std::string buff;
		}Hands;
	}Visuals;
	struct
	{
		struct
		{
			float MinDamage = 65.0f;
			int AutowallType;
		}AutoWall;
		struct
		{
			// Begin new menu structure
			bool Enabled;
			int Hitbox;
			int Targeting;
			int HitScans;
			bool AutoShoot = 1;
			bool AutoScope = 1;
			bool FriendAim = 1;
			bool bHitchanceSnipers;
			bool bHitchance;
			float Hitchance;
			float FOV = 180.0f;
			bool EnemyOnly = true;
			bool PsuedoSpread;
			int Resolver = 1;
			bool AntiAntiAim;
			bool Multipoint;
			bool SilentAim = 1;
			bool Autowall = 1;
			struct
			{
				int ResolverType;
				QAngle oAngle[64];
				QAngle CAngle[64];
				bool Enemyonly = true;
				int bone[64];
				int Lisp[64];
			}AAA;
			struct
			{
				int Point;
				bool Head;
				bool Neck;
				bool LowerNeck;
				bool Pelvis;
				bool Body;
				bool Thorax;
				bool Chest;
				bool UpperChest;
				bool RightThigh;
				bool LeftThigh;
				bool RightCalf;
				bool LeftCalf;
				bool RightFoot;
				bool LeftFoot;
				bool RightHand;
				bool LeftHand;
				bool RightUpperArm;
				bool RightForeArm;
				bool LeftUpperArm;
				bool LeftForeArm;
				bool Max;
			}Hitscan;
		}Aimbot;
		struct
		{
			bool Enabled;
			int Yaw;
			int Pitch;
			int spinSpeed;
			bool AtTargets;
			float SpinAmt;
		}AntiAim;
	}Rage;
	struct
	{
		struct
		{
			bool Enabled;
			int Send;
			int Choke;
		}FakeLag;
		struct
		{
			bool Enabled;
		}ChatSpamRegular;
		struct
		{
			int Key;
			bool Enabled;
		}AirStuck;
		struct
		{
			bool Enabled;
		}bhop;
		struct
		{
			bool REnabled;
			bool Silent;
		}AutoStrafe;
		struct
		{
			int Enabled;
			char* buf;
			std::string buff;
		}ClanTags;
		struct
		{
			int Enabled;
			char* buf;
			std::string buff;
		}Names;
		int PanicKey;
		int Flash;
	}PlayerMods;
	struct
	{
		bool Opened = false;
		bool Configs;
		bool Ragebot;
		bool Visuals;
		bool PlayerMods;
		bool Removals;
		bool Aesthetics;
		bool Title;
		bool Console;
		struct
		{
			bool MoreInfo;
		}pList;
		bool HitScans;
		bool active = false;

		//  PlayerList Comboboxes
		bool AntiPitchCombo;
		bool ConfigCombo;
		bool AntiYawCombo;
		bool HitscanTypeCombo;
		bool TargetTypeCombo;
		bool ResolverTypeCombo;
		bool RageHitboxCombo;
		bool CustomNameCombo;
		bool ClanTagCombo;
		bool CustomHandCombo;
		bool ShouldDraw;
	}Menu;
	struct
	{
		bool MM = true;
		bool VAC = true;
	}AntiCheat;
	struct
	{
		int Current;
	}Config;
	struct
	{
		bool Uninject;
	}Cheat;
	struct
	{
		bool Enabled;
		char Message[255] = "";
	}IRC;
};
extern cOptions Options;