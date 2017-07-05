#pragma once

namespace SSDK {
	/*class WeaponInfo_t
	{
	public:
		char pad_0x0000[99];	//0x0000
		char m_name[80];			//0x00
		char pad_0x0800[0x74D];	//0x0000 Updated (Structure)
		float m_flArmorRatio;		//0x07B0 
		float unkwn1;				//0x07B4 
		float unkwn2;				//0x07B8 
		__int32 unkwn3;				//0x07BC 
		__int32 unkwn4;				//0x07C0 
		float m_flPenetration;		//0x07C4 
		__int32 m_iDamage;			//0x07C8 
		float m_flRange;			//0x07CC 
		float m_flRangeModifier;	//0x07D0 
	};*/

	struct CHudTexture
	{
		char	szShortName[64];	//0x0000
		char	szTextureFile[64];	//0x0040
		bool	bRenderUsingFont;	//0x0080
		bool	bPrecached;			//0x0081
		char	cCharacterInFont;	//0x0082
		char    pad;		        //0x0083
		HFont	hFont;				//0x0084
		int		iTextureId;			//0x0088
		float	afTexCoords[4];		//0x008C
		int		iPosX[4];			//0x009C
	}; //Size=0x00AC

	struct WeaponInfo_t
	{
		BYTE		pad_0x0000[4];			//0x0000
		bool		bParsedScript;			//0x0004
		bool		bLoadedHudElements;		//0x0005
		char		szClassName[80];		//0x0006
		char		szPrintName[80];		//0x0056
		char		szViewModel[80];		//0x00A6
		char		szWorldModel[80];		//0x00F6
		char		szBulletType[32];		//0x0146
		char		szWorldDroppedModel[80];//0x0166
		char		szAnimationPrefix[16];	//0x01B6
		BYTE		pad_0x01C6[2];			//0x01C6
		int			iSlot;					//0x01C8
		int			iPosition;				//0x01CC
		int			iMaxClip1;				//0x01D0
		int			iMaxClip2;				//0x01D4
		int			iDefaultClip1;			//0x01D8
		int			iDefaultClip2;			//0x01DC
		int			iWeight;				//0x01E0
		int			iRumbleEffect;			//0x01E4
		bool		bAutoSwitchTo;			//0x01E8
		bool		bAutoSwitchFrom;		//0x01E9
		BYTE		pad_0x01EA[2];			//0x01EA
		int			iFlags;					//0x01EC
		char		szAmmo1[16];			//0x01F0
		char		szAmmo2[16];			//0x0200
		char		szAIAddOn[80];			//0x0210
		char		aShootSounds[17][80];	//0x0260
		char PAD_NEW[80];
		int			iAmmoType;
		int			iAmmo2Type;
		bool		bMeleeWeapon;
		bool		bBuiltRightHanded;
		bool		bAllowFlipping;
		BYTE		pad_0x07BB;
		int			iSpriteCount;
		CHudTexture* iconWeaponS;
		CHudTexture* iconWeapon;
		CHudTexture* iconAmmo;
		CHudTexture* iconAmmo2;
		CHudTexture* iconCrosshair;
		CHudTexture* iconAutoaim;
		CHudTexture* iconZoomedCrosshair;
		CHudTexture* iconZoomedAutoaim;
		CHudTexture* iconWeaponSmall;
		bool		bShowUsageHint;
		BYTE		pad_0x07E5[3];
		CSWeaponType iWeaponType;
		int			iTeam;
		int			iWeaponID;
		bool		bFullAuto;
		BYTE		pad_0x07F5[3];
		float		flHeatPerShot;
		int			iWeaponPrice;
		float		flArmorRatio;
		float		flMaxPlayerSpeed;
		float		flMaxPlayerSpeedAlt;
		int			iCrosshairMinDistance;
		int			iCrosshairDeltaDistance;
		float		flPenetration;
		int			iDamage;
		float		flRange;
		float		flRangeModifier;
		int			iBullets;
		float		flCycleTime;
		float		flCycleTimeAlt;
		char		szHeatEffect[80];
		float		aflSmokeColor[3];
		char		szMuzzleFlashEffect_1stPerson[80];
		char		szMuzzleFlashEffect_3rdPerson[80];
		char		szEjectBrassEffect[80];
		char		szTracerEffect[80];
		int			iTracerFrequency;
		float		flSpread;
		float		flSpreadAlt;
		float		flInaccuracyCrouch;
		float		flInaccuracyCrouchAlt;
		float		flInaccuracyStand;
		float		flInaccuracyStandAlt;
		float		flInaccuracyJump;
		float		flInaccuracyJumpAlt;
		float		flInaccuracyLand;
		float		flInaccuracyLandAlt;
		float		flInaccuracyLadder;
		float		flInaccuracyLadderAlt;
		float		flInaccuracyFire;
		float		flInaccuracyFireAlt;
		float		flInaccuracyMove;
		float		flInaccuracyMoveAlt;
		float		flRecoveryTimeStand;
		float		flRecoveryTimeCrouch;
		float		flInaccuracyReload;
		float		flInaccuracyAltSwitch;
		float		flRecoilAngle;
		float		flRecoilAngleAlt;
		float		flRecoilAngleVariance;
		float		flRecoilAngleVarianceAlt;
		float		flRecoilMagnitude;
		float		flRecoilMagnitudeAlt;
		float		flRecoilMagnitudeVariance;
		float		flRecoilMagnitudeVarianceAlt;
		int			iRecoilSeed;
		float		flFlinchVelocityModifierLarge;
		float		flFlinchVelocityModifierSmall;
		float		flTimeToIdle;
		float		flIdleInterval;
		float		recoilTable[2][128];
		int			iZoomLevels;
		int			iZoomFOV[3];
		float		fZoomTime[3];
		bool		bHideViewModelZoomed;
		char		szZoomInSound[80];
		char		szZoomOutSound[80];
		BYTE		pad_0x0F11[3];
		float		flBotAudibleRange;
		bool		bCanUseWithShield;
		char		szWrongTeamMsg[32];
		char		szAnimPrefix[16];
		char		szShieldViewModel[64];
		char		szAddonModel[80];
		char		szAddonLocation[80];
		char		szSilencerModel[80];
		BYTE		pad_0x1079[3];
		float		flAddonScale;
		float		flThrowVelocity;
		int			iKillAward;
		BYTE		pad_0x1088[8];
	};
}