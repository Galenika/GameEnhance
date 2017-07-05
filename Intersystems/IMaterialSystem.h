#pragma once
#include "Includes.h"
#include "IMaterial.h"
#include "KeyValues.h"

typedef unsigned short MaterialHandle_t;

class IMaterialSystem
{
public:
	IMaterial* CreateMaterial(const char *pMaterialName, KeyValues *pVMTKeyValues)
	{
		typedef IMaterial*(__thiscall* CreateMaterialFn)(void*, const char *pMaterialName, KeyValues *pVMTKeyValues);
		return CallVFunc<CreateMaterialFn>(this, 83)(this, pMaterialName, pVMTKeyValues);
	}

	IMaterial* FindMaterial(const char* pMaterialName, const char* pTextureGroupName = "Model textures", bool complain = true, const char* pComplainPrefix = NULL)
	{
		typedef IMaterial* (__thiscall* FindMaterialFn)(void*, const char*, const char*, bool, const char*);
		return CallVFunc<FindMaterialFn>(this, 84)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}

	MaterialHandle_t FirstMaterial()
	{
		typedef MaterialHandle_t(__thiscall* FirstMaterialFn)(void*);
		return CallVFunc<FirstMaterialFn>(this, 86)(this);
	}

	MaterialHandle_t NextMaterial(MaterialHandle_t h)
	{
		typedef MaterialHandle_t(__thiscall* NextMaterialFn)(void*, MaterialHandle_t);
		return CallVFunc<NextMaterialFn>(this, 87)(this, h);
	}

	MaterialHandle_t InvalidMaterial()
	{
		typedef MaterialHandle_t(__thiscall* InvalidMaterialFn)(void*);
		return CallVFunc<InvalidMaterialFn>(this, 88)(this);
	}

	IMaterial* GetMaterial(MaterialHandle_t h)
	{
		typedef IMaterial*(__thiscall* GetMaterialFn)(void*, MaterialHandle_t);
		return CallVFunc<GetMaterialFn>(this, 89)(this, h);
	}
};