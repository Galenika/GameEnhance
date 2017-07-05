#include "SDK.h"
#include "VMT Hook.h"
#include "Model Render.h"
#include "Control Variables.h"
#include "Color Manager.h"
#include "Player Manager.h"
#include "Extra Functions.h"
#include "Aimbot.h"
//===================================================================================
CModelRender gModelRender;
//===================================================================================
CVMTHookManager *m_pModelRenderHook = NULL;
//===================================================================================
IMaterial *m_pMaterial = NULL;
//===================================================================================
bool C_BaseEntity::SetupBones( matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime )
{
	_asm JMP DWORD PTR [ ECX + 0x3C ];
}
//===================================================================================
/*
void GetHitspot( const DrawModelState_t &state, C_BaseEntity* m_pEnt )
{
	C_BaseAnimating* m_pAnim = m_pEnt->GetBaseAnimating( );

	if( !m_pAnim )
		return;

	mstudiohitboxset_t *set = state.m_pStudioHdr->pHitboxSet( m_pAnim->m_nHitboxSet );

	if ( !set )
		return;

	if( !state.m_pStudioHdr || !m_pEnt )
		return;

	matrix3x4_t pmatrix[MAXSTUDIOBONES];

	for ( int i = 0; i < set->numhitboxes; i++ )
	{
		mstudiobbox_t *pbox = set->pHitbox( i );

		m_pEnt->SetupBones( pmatrix, 128, BONE_USED_BY_HITBOX, m_pGlobals->curtime );

		if( strstr( state.m_pStudioHdr->pBone( pbox->bone )->pszName(), "head" ) > 0  && cvars.aim_spot == 1 )
			gPlayers[m_pEnt->index].SetHitbox( pbox->bone );
		if( strstr( state.m_pStudioHdr->pBone( pbox->bone )->pszName(), "spine_3" ) > 0  && cvars.aim_spot == 2 )
			gPlayers[m_pEnt->index].SetHitbox( pbox->bone );
		if( strstr( state.m_pStudioHdr->pBone( pbox->bone )->pszName(), "spine_2" ) > 0  && cvars.aim_spot == 3 )
			gPlayers[m_pEnt->index].SetHitbox( pbox->bone );
		if( strstr( state.m_pStudioHdr->pBone( pbox->bone )->pszName(), "spine_1" ) > 0  && cvars.aim_spot == 4 )
			gPlayers[m_pEnt->index].SetHitbox( pbox->bone );
		if( strstr( state.m_pStudioHdr->pBone( pbox->bone )->pszName(), "spine_0" ) > 0  && cvars.aim_spot == 5 )
			gPlayers[m_pEnt->index].SetHitbox( pbox->bone );
	}
	
}
*/
//===================================================================================
void SetupEntityBones( const DrawModelState_t &pState, C_BaseEntity* pBaseEntity, DWORD dwTeamColor )
{
	C_BaseAnimating* pBaseAnimating = pBaseEntity->GetBaseAnimating( );

	if( !pBaseAnimating )
		return;

	mstudiohitboxset_t *pSet = pState.m_pStudioHdr->pHitboxSet( pBaseAnimating->m_nHitboxSet );

	if ( !pSet )
		return;

	if( !pState.m_pStudioHdr || !pBaseEntity )
		return;

	matrix3x4_t pMatrix[MAXSTUDIOBONES];

	Vector vPosition;
	QAngle qAngles;

	Color clrWhite, clrTeamColor, clrTeamColorDim;

	clrWhite.SetColor( 255, 255, 255, 80 );
	clrTeamColor.SetColor( RED( dwTeamColor ), GREEN( dwTeamColor ), BLUE( dwTeamColor ), 80 );
	clrTeamColorDim.SetColor( RED( dwTeamColor ), GREEN( dwTeamColor ), BLUE( dwTeamColor ), 40 );

	if( pBaseEntity->SetupBones( pMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, g_pGlobals->realtime ) == false )
		return;

	for ( int iIndex = 0; iIndex < pSet->numhitboxes; iIndex++ )
	{
		mstudiobbox_t *pBox = pSet->pHitbox( iIndex );
				
		if( gCvars.misc_modelhack )
		{
			if( strstr( pState.m_pStudioHdr->pBone( pBox->bone )->pszName( ), "head" ) > 0 )
			{
				//Ignores Z-depth test
				g_pDebugOverlay->AddBoxOverlay2( vPosition, pBox->bbmin, pBox->bbmax, qAngles, clrWhite, clrTeamColor, g_pGlobals->frametime / 2 ); 
			}

			g_pDebugOverlay->AddBoxOverlay2( vPosition, pBox->bbmin, pBox->bbmax, qAngles, clrTeamColorDim, clrTeamColor, g_pGlobals->frametime / 2 ); 
		}

		//Setup hitboxes here!
		if( strstr( pState.m_pStudioHdr->pBone( pBox->bone )->pszName(), "head" ) > 0  && gCvars.aim_spot == 1 )
			gPlayers[pBaseEntity->index].SetHitbox( pBox->bone );
		if( strstr( pState.m_pStudioHdr->pBone( pBox->bone )->pszName(), "spine_3" ) > 0  && gCvars.aim_spot == 2 )
			gPlayers[pBaseEntity->index].SetHitbox( pBox->bone );
		if( strstr( pState.m_pStudioHdr->pBone( pBox->bone )->pszName(), "spine_2" ) > 0  && gCvars.aim_spot == 3 )
			gPlayers[pBaseEntity->index].SetHitbox( pBox->bone );
		if( strstr( pState.m_pStudioHdr->pBone( pBox->bone )->pszName(), "spine_1" ) > 0  && gCvars.aim_spot == 4 )
			gPlayers[pBaseEntity->index].SetHitbox( pBox->bone );
		if( strstr( pState.m_pStudioHdr->pBone( pBox->bone )->pszName(), "spine_0" ) > 0  && gCvars.aim_spot == 5 )
			gPlayers[pBaseEntity->index].SetHitbox( pBox->bone );
	}
}
//===================================================================================
void __stdcall new_DrawModelExecute( const DrawModelState_t &pState, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld )
{
	_asm pushad;

	const char* pszModelName = g_pModelInfo->GetModelName( pInfo.pModel );

	if( ( strstr( pszModelName, "models/player" ) ) && g_pEngine->IsInGame( ) )
	{
		g_pModelInfo->GetModelMaterials( pInfo.pModel, 1, &m_pMaterial );

		IClientUnknown* pUnknown = pState.m_pRenderable->GetIClientUnknown( );

		if( pUnknown )
		{
			C_BaseEntity* pBaseEntity = pUnknown->GetBaseEntity( );

			if( pBaseEntity->m_lifeState == LIFE_ALIVE  && !( pInfo.entity_index == g_pEngine->GetLocalPlayer( ) ) && ( pBaseEntity->IsDormant( ) == false ) && pBaseEntity->IsPlayer( ) )
			{
				DWORD dwTeamColor = gColorManager.dwGetColor( pBaseEntity->GetTeamNumber( ) );
				SetupEntityBones( pState, pBaseEntity, dwTeamColor );
			}
		}
		
		if ( m_pMaterial && gCvars.misc_wallhack )
		{
			m_pMaterial->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );
			m_pMaterial->SetMaterialVarFlag( MATERIAL_VAR_ZNEARER, true );
			m_pMaterial->SetMaterialVarFlag( MATERIAL_VAR_FLAT, true );
		}
	}

	_asm popad;

	gModelRender.DrawModelExecute( pState, pInfo, pCustomBoneToWorld );

	_asm pushad;

	if ( m_pMaterial && gCvars.misc_wallhack )
	{
		m_pMaterial->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );
		m_pMaterial->SetMaterialVarFlag( MATERIAL_VAR_ZNEARER, false );
		m_pMaterial->SetMaterialVarFlag( MATERIAL_VAR_FLAT, false); 
	}

	_asm popad;
}
//===================================================================================
void BuildModelRenderHook( PDWORD* pdwModelRender )
{
	RtlCopyMemory( (void*)&gModelRender, (void*)*pdwModelRender, sizeof( CModelRender ) );

	m_pModelRenderHook = new CVMTHookManager( (PDWORD*)pdwModelRender );
	m_pModelRenderHook->dwHookMethod( (DWORD)new_DrawModelExecute, 19 );
	
}
//===================================================================================