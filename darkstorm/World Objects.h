#ifndef _WORLD_OBJECTS_H_
#define _WORLD_OBJECTS_H_
//===================================================================================
#include "SDK.h"
//===================================================================================
class CObject : public C_BaseEntity
{
public:
	unsigned char    ucUnknown001[ 0x92C ];        //0x4A0 - 0xDCC
	int                m_iUpgradeLevel;            //0xDCC - 0xDD0
	int                m_iUnknown002;                //0xDD0 - 0xDD4
	int                m_iUpgradeMetal;            //0xDD4 - 0xDD8
	int                m_iUnknown003;                //0xDD8 - 0xDDC
	int                m_fObjectFlags;                //0xDDC - 0xDE0
	unsigned char    ucUnknown004[ 0xC ];        //0xDE0 - 0xDEC
	C_BaseEntity*    m_hBuilder;                    //0xDEC - 0xDF0
	unsigned char    ucUnknown005[ 0x8 ];        //0xDF0 - 0xDF8
	bool            m_bHasSapper;                //0xDF8 - 0xDF9
	unsigned char    ucUnknown006[ 0x3 ];        //0xDF9 - 0xDFC
	int                m_iObjectType;                //0xDFC - 0xE00
	int                m_iHealth;                    //0xE00 - 0xE04
	int                m_iMaxHealth;                //0xE04 - 0xE08
	unsigned char    ucUnknown007;                //0xE08 - 0xE09
	bool            m_bBuilding;                //0xE09 - 0xE0A
	unsigned char    ucUnknown008;                //0xE0A - 0xE0B
	bool            m_bPlacing;                    //0xE0B - 0xE0C
	bool            m_bDisabled;                //0xE0C - 0xE0D
	unsigned char    ucUnknown009[ 0x3 ];        //0xE0D - 0xE10
	float            m_flPercentageConstructed;    //0xE10 - 0xE14
	C_BaseEntity*    m_hBuiltOnEntity;            //0xE14 - 0xE18
	Vector            m_vecBuildMaxs;                //0xE18 - 0xE24 (sizeof(float)*3)
	Vector            m_vecBuildMins;                //0xE24 - 0xE30 (sizeof(float)*3)
	int                m_iDesiredBuildRotations;    //0xE30 - 0xE34
	unsigned char    ucUnknown010[ 0x8 ];        //0xE34 - 0xE3C
	bool            m_bServerOverridePlacement; //0xE3C - 0xE3D
};
//===================================================================================
class CObjectDispenser : public CObject
{
public:
	unsigned char    ucUnknown011[ 0x20 ];        //0xE40 - 0xE60
	int                m_iAmmoMetal;                //0xE60 - 0xE64
};
//===================================================================================
class CObjectSentryGun : public CObject
{
public:
	unsigned char    ucUnknown011[ 0x8 ];        //0xE40 - 0xE48
	int                m_iState;                    //0xE48 - 0xE4C
	int                m_iAmmoShells;                //0xE4C - 0xE50
	int                m_iUnknown012;                //0xE50 - 0xE54
	int                m_iAmmoRockets;                //0xE54 - 0xE58
	int                m_iKills;                    //0xE58 - 0xE5C

	inline string GetStateString( )
	{
		switch( this->m_iState )
		{
		case 1:
			{
				return "Idle";
			}
		case 2:
			{
				return "Attacking";
			}
		case 3:
			{
				return "Upgrading";
			}
		}

		return "Unknown";
	}
};
//===================================================================================
class CObjectTeleporter : public CObject
{
public:
	unsigned char    ucUnknown011[ 0x8 ];        //0xE40 - 0xE48
	int                m_iState;                    //0xE48 - 0xE4C
	int                m_iUnknown012;                //0xE4C - 0xE50
	float            m_flRechargeTime;            //0xE50 - 0xE54
	int                m_iTimesUsed;                //0xE54 - 0xE58
	float            m_flYawToExit;                //0xE58 - 0xE5C

	inline string GetStateString( )
	{
		switch( this->m_iState )
		{
		case 1:
			{
				return "Idle";
			}
		case 2:
			{
				return "Active";
			}
		case 4:
			{
				return "Teleporting";
			}
		case 6:
			{
				return "Charging";
			}
		}

		return "Unknown";
	}
};
//===================================================================================
CObject *ToBaseObject( C_BaseEntity *pBaseEntity )
{
	return reinterpret_cast< CObject *>( pBaseEntity );
}
//===================================================================================
#endif