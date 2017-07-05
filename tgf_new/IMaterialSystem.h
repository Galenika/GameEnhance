#pragma once

typedef unsigned short MaterialHandle_t;

class IMaterialSystem
{
public:
	IMaterial* CreateMaterial( bool flat, bool ignorez, bool wireframed );
	IMaterial* FindMaterial( char const* pMaterialName, const char *pTextureGroupName, bool complain = true, const char *pComplainPrefix = NULL );

	MaterialHandle_t FirstMaterial()
	{
		typedef MaterialHandle_t(__thiscall* FirstMaterialFn)(void*);
		return U::GetVFunc<FirstMaterialFn>(this, 86)(this);
	}

	MaterialHandle_t NextMaterial(MaterialHandle_t h)
	{
		typedef MaterialHandle_t(__thiscall* NextMaterialFn)(void*, MaterialHandle_t);
		return U::GetVFunc<NextMaterialFn>(this, 87)(this, h);
	}

	MaterialHandle_t InvalidMaterial()
	{
		typedef MaterialHandle_t(__thiscall* InvalidMaterialFn)(void*);
		return U::GetVFunc<InvalidMaterialFn>(this, 88)(this);
	}

	IMaterial* GetMaterial(MaterialHandle_t h)
	{
		typedef IMaterial*(__thiscall* GetMaterialFn)(void*, MaterialHandle_t);
		return U::GetVFunc<GetMaterialFn>(this, 89)(this, h);
	}
};