#ifndef _MODEL_RENDER_H_
#define _MODEL_RENDER_H_
//===================================================================================
#include <Windows.h>
#include "SDK.h"
//===================================================================================
struct CModelRender
{
	int						( __stdcall *DrawModel ) ( int flags, IClientRenderable *cliententity, ModelInstanceHandle_t instance, int entity_index, const model_t *model, Vector const& origin, QAngle const& angles, int skin, int body, int hitboxset, const matrix3x4_t *modelToWorld, const matrix3x4_t *pLightingOffset );
	void					( __stdcall *ForcedMaterialOverride) ( IMaterial *newMaterial );
	void					( __stdcall *SetViewTarget) ( const CStudioHdr *pStudioHdr, int nBodyIndex, const Vector& target);
	ModelInstanceHandle_t	( __stdcall *CreateInstance) ( IClientRenderable *pRenderable, LightCacheHandle_t *pCache );
	void					( __stdcall *DestroyInstance) ( ModelInstanceHandle_t handle );
	void					( __stdcall *SetStaticLighting) ( ModelInstanceHandle_t handle, LightCacheHandle_t* pHandle );
	LightCacheHandle_t		( __stdcall *GetStaticLighting) ( ModelInstanceHandle_t handle );
	bool					( __stdcall *ChangeInstance) ( ModelInstanceHandle_t handle, IClientRenderable *pRenderable );
	void					( __stdcall *AddDecal) ( ModelInstanceHandle_t handle, Ray_t const& ray, Vector const& decalUp, int decalIndex, int body, bool noPokeThru, int maxLODToDecal );
	void					( __stdcall *RemoveAllDecals) ( ModelInstanceHandle_t handle );
	void					( __stdcall *RemoveAllDecalsFromAllModels) ();
	matrix3x4_t				( __stdcall *DrawModelShadowSetup)( IClientRenderable *pRenderable, int body, int skin, DrawModelInfo_t *pInfo, matrix3x4_t *pCustomBoneToWorld );
	void					( __stdcall *DrawModelShadow) ( IClientRenderable *pRenderable, int body );
	void					( __stdcall *RecomputeStaticLighting) ( ModelInstanceHandle_t handle );
	void					( __stdcall *ReleaseAllStaticPropColorData) ( void );
	void					( __stdcall *RestoreAllStaticPropColorData) ( void );
	int						( __stdcall *DrawModelEx)( ModelRenderInfo_t &pInfo ) ; // 16
	int						( __stdcall *DrawModelExStaticProp)( ModelRenderInfo_t &pInfo );
	bool					( __stdcall *DrawModelSetup)( ModelRenderInfo_t &pInfo, DrawModelState_t *pState, matrix3x4_t *pCustomBoneToWorld, matrix3x4_t** ppBoneToWorldOut );
	void					( __stdcall *DrawModelExecute)( const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld); //19
	void					( __stdcall *SetupLighting)( const Vector &vecCenter );
	int						( __stdcall *DrawStaticPropArrayFast)( StaticPropRenderInfo_t *pProps, int count, bool bShadowDepth );
	void					( __stdcall *SuppressEngineLighting)( bool bSuppress );
	void					( __stdcall *SetupColorMeshes)( int nTotalVerts );
};
//===================================================================================
extern CModelRender gModelRender;
//===================================================================================
void BuildModelRenderHook( PDWORD* pdwModelRender );
//===================================================================================
#endif