#pragma once

#include "MiscDefinitions.h"
#include "ClientRecvProps.h"
#include "offsets.h"
#include "Vector.h"

class NET_SetConVar
{
public:
	NET_SetConVar(const char* name, const char* value) {
		typedef void(__thiscall* SetConVarConstructor_t)(void*);
		static SetConVarConstructor_t pNET_SetConVar = (SetConVarConstructor_t)(Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x83\xE9\x04\xE9\x00\x00\x00\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x56\x8B\xF1\xC7\x06\x00\x00\x00\x00\x8D\x4E\x08", "xxxx????xxxxxxxxxxxxxxxx????xxx") + 19);
		pNET_SetConVar(this);

		typedef void(__thiscall* SetConVarInit_t)(void*, const char*, const char*);
		static SetConVarInit_t pNET_SetConVarInit = (SetConVarInit_t)Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x55\x8B\xEC\x56\x8B\xF1\x57\x83\x4E\x14\x01\x83\x7E\x0C\x00", "xxxxxxxxxxxxxxx");
		pNET_SetConVarInit(this, name, value);
	}

	~NET_SetConVar() {
		typedef void(__thiscall* SetConVarConstructor_t)(void*);
		static SetConVarConstructor_t pNET_SetConVar = (SetConVarConstructor_t)(Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x56\x8B\xF1\x57\x8D\x7E\x04\xC7\x07\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxx????") + 12);
		pNET_SetConVar(this);
	}
private:
	DWORD pad[13];
};

class INetChannelInfo
{
public:
	void SendNetMsg(NET_SetConVar& convar) {
		typedef void(__thiscall* OriginalFn)(PVOID, NET_SetConVar&, int, int);
		call_vfunc<OriginalFn>(this, 42)(this, convar, 0, 0);
	}
};

// EngineClient
class IVEngineClient
{
public:
	int GetPlayerForUserID(int UserID) {
		using Original = int(__thiscall*)(PVOID, int);
		return call_vfunc<Original>(this, 9)(this, UserID);
	}
	void GetScreenSize(int& width, int& height)
	{
		typedef void(__thiscall* oGetScreenSize)(PVOID, int&, int&);
		return call_vfunc< oGetScreenSize >(this, Offsets::VMT::Engine_GetScreenSize)(this, width, height);
	}
	void ClientCmd(const char *szCmdString)
	{
		typedef void(__thiscall* oClientCmd)(PVOID, const char*);
		return call_vfunc<oClientCmd>(this, 7)(this, szCmdString);
	}
	bool GetPlayerInfo(int ent_num, player_info_t *pinfo)
	{
		typedef bool(__thiscall* oGetPlayerInfo)(PVOID, int, player_info_t*);
		return call_vfunc< oGetPlayerInfo >(this, Offsets::VMT::Engine_GetPlayerInfo)(this, ent_num, pinfo);
	}
	int GetLocalPlayer()
	{
		typedef int(__thiscall* oLocal)(PVOID);
		return call_vfunc< oLocal >(this, Offsets::VMT::Engine_GetLocalPlayer)(this);
	}
	float Time()
	{
		typedef float(__thiscall* oTime)(PVOID);
		return call_vfunc< oTime >(this, Offsets::VMT::Engine_Time)(this);
	}
	void GetViewAngles(Vector& vAngles)
	{
		typedef void(__thiscall* oSetViewAngles)(PVOID, Vector&);
		return call_vfunc< oSetViewAngles >(this, Offsets::VMT::Engine_GetViewAngles)(this, vAngles);
	}
	void SetViewAngles(Vector& vAngles)
	{
		typedef void(__thiscall* oSetViewAngles)(PVOID, Vector&);
		return call_vfunc< oSetViewAngles >(this, Offsets::VMT::Engine_SetViewAngles)(this, vAngles);
	}
	int GetMaxClients()
	{
		typedef bool(__thiscall* oGetMaxClients)(PVOID);
		return call_vfunc< oGetMaxClients >(this, Offsets::VMT::Engine_GetMaxClients)(this);
	}
	bool IsConnected()
	{
		typedef bool(__thiscall* oGetScreenSize)(PVOID);
		return call_vfunc< oGetScreenSize >(this, Offsets::VMT::Engine_IsConnected)(this);
	}
	bool IsInGame()
	{
		typedef bool(__thiscall* oLocal)(PVOID);
		return call_vfunc< oLocal >(this, Offsets::VMT::Engine_IsInGame)(this);
	}
	const matrix3x4& WorldToScreenMatrix()
	{
		typedef const matrix3x4& (__thiscall* oWorldToScreenMatrix)(PVOID);
		return call_vfunc< oWorldToScreenMatrix >(this, Offsets::VMT::Engine_WorldToScreenMatrix)(this);
	}
	void ClientCmd_Unrestricted(char  const* cmd)
	{
		typedef void(__thiscall* oClientCmdUnres)(PVOID, const char*, char);
		return call_vfunc<oClientCmdUnres>(this, Offsets::VMT::Engine_ClientCmd_Unrestricted)(this, cmd, 0);
	}
	INetChannelInfo* GetNetChannelInfo()
	{
		typedef INetChannelInfo*(__thiscall* OriginalFn)(PVOID);
		return call_vfunc< OriginalFn >(this, 78)(this);
	}

};