#pragma once

struct Variables
{
	Variables()
	{

	}

	struct Ragebot_s
	{
		bool 	Enabled = false;
		bool 	AutoFire;
		bool 	Hold;
		int 	HoldKey;
		int 	Filter; // will hold flags for team/enemy/etc.
		bool 	Silent;
		bool	RCS;
		bool 	VisCheck;
		bool 	AutoWall;
		float	AutoWallDmg;
		int 	TargetMethod;
		int		Hitbox;
		float 	FOV;
		bool 	HitChance;
		float	HitChanceAmt;
		bool 	AutoStop;
		bool 	AutoCrouch;
		bool 	FriendlyFire;
		bool	Aimstep;
		float	AimstepAmount;
		bool	HitScanAll;
		bool	HitScanHitbox;
		bool	UntrustedCheck = true;
		struct
		{
			bool	Enabled;
			int		Pitch;
			int		Yaw;
			bool	AimAtPlayer;
			bool	DetectEdge;
			bool	PSilent;
		} Antiaim;
	} Ragebot;

	struct
	{
		struct
		{
			bool 	Enabled = true;
			int 	Key = 0;
			bool	OnKey = true;
			int 	Filter; // will hold flags for team/enemy/etc.
			bool 	RCS = true;
			float 	RCSAmount = 50;
			float	Speed = 35;
			bool 	VisCheck;
			int 	Hitbox;
			float 	FOV = 10;
			bool 	AlwaysOn = false;
			bool	FriendlyFire;
		} Aimbot;

		struct
		{
			bool	Enabled = false;
			bool	AutoFire;
			int		Key;
			bool 	HitChance;
			float	HitChanceAmt;
			bool	Dellay;
			float	DellayAmt;
			struct
			{
				bool Head;
				bool Chest;
				bool Stomach;
				bool Arms;
				bool Legs;
				bool Friendly;
			} Filter;

		} Triggerbot;

	} Legitbot;

	struct
	{
		bool 	Enabled;
		int 	Box;
		int 	BoxType;
		bool	Skeleton;
		bool	Glow;
		bool	BulletTrace;
		float   BulletTraceLength;
		bool	CrosshairOn;
		int		CrosshairType;
		int		CrosshairStyle;
		float 	TeamColor[ 3 ];			
		float 	TeamColorWall[ 3 ];
		float 	EnemyColor[ 3 ];

		bool ASUS;
		float ASUSFactor;

		float 	EnemyColorWall[ 3 ];
		struct
		{
			bool Enemies;
			bool Friendlies;
			bool Weapons;
			bool Decoy;
			bool C4;
		} Filter;

		struct
		{
			bool	Enabled;
			int 	Mode;
			bool	Wireframe;
			bool	XQZ;
			float 	TeamColor[ 3 ];
			float 	TeamColorWall[ 3 ];
			float  	EnemyColor[ 3 ];
			float 	EnemyColorWall[ 3 ];
		} Chams;

		struct
		{
			bool 	Name;
			bool 	Health;
			bool 	Weapon;
			bool	Flashed;
		} Info;

		struct
		{
			bool	Hands;
			bool	Weapon;
			bool	VisualRecoil;
			bool	Flash;
			bool    Smoke;
			bool	Sky;
			int HandsMode;
		} Removals;
	} Visuals;

	struct
	{
		bool 	Bhop = false;
		bool 	AutoStrafe = false;
		bool	ChatSpam = false;
		int		ChatSpamMode = false;
		bool	Watermark = false;
		bool	Ranks = false;
		bool	AirStuck = false;
		int		AirStuckKey = false;
		bool	AutoAccept = false;
		bool	Namespam = false;

		bool SkinsEnabled = false;
		int SkinAK;
		int SkinAWP;
		int SkinDeagle;
		int Skinp2000;
		int Skinf7;
		int SkinNova;
		int SkinNegev;
		int	SkinAug;
		int SkinMac10;
		int SkinGlock;
		int SkinGalil;
		int SkinMP7;
		int Skinp250;
		int SkinSawedOff;
		int SkinUMP;
		int SkinTec9;
		int SkinMAG7;
		int SkinFamas;
		int SkinM4A1S;
		int SkinCZ;
		int Skin249;
		int SkinM4A4;
		int SkinG3SG1;
		int SkinSG556;
		int SkinSSG08;		
		int SkinP90;
		int SkinScar20;
		int SkinMp9;

		int SkinElite;
		int SkinR8;

		int SkinBizon;
		int SkinXM;
		int SkinUsp;

		int KnifeModel;

		int KnifeIDb;
		int KnifeIDf;
		int KnifeIDd;
		int KnifeIDg;
		int KnifeIDm;
		int KnifeIDk;
		int KnifeIDh;
		int KnifeIDbutter;
		int KnifeIDfal;
		int KnifeIDdag;
		int KnifeIDbow;
		char* buf;
	} Misc;

	struct
	{
		bool	Opened = false;
		int 	Key;
		bool	Ragebot = false;
		bool	Legitbot = false;
		bool	Visual = false;
		bool	Misc = false;
		bool	Colors = false;

		int		ConfigIndex;
		bool	LoadFaceit = true;
	} Menu;
};

extern Variables Vars;