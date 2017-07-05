#ifndef _RADAR_H_
#define _RADAR_H_
//===================================================================================
#include "SDK.h"
//===================================================================================
class CRadar
{
public:
	void DrawRadarBack( void );
	void DrawRadarPoint( Vector vOriginX, Vector vOriginY, QAngle qAngle, C_BaseEntity *pBaseEntity, DWORD dwTeamColor );
};
//===================================================================================
extern CRadar gRadar;

#endif