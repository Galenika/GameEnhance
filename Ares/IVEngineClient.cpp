// IVEngineClient
#include "Ares.h"

int IVEngineClient::GetLocalPlayer(void) {
	typedef int(__thiscall* OriginalFn)(PVOID);
	return GetVFunc<OriginalFn>(this, 12)(this);
}

bool IVEngineClient::IsInGame(void) {
	typedef bool(__thiscall* OriginalFn)(PVOID);
	return GetVFunc<OriginalFn>(this, 26)(this);
}

bool IVEngineClient::IsConnected(void) {
	typedef bool(__thiscall* OriginalFn)(PVOID);
	return GetVFunc<OriginalFn>(this, 27)(this);
}

bool IVEngineClient::IsTakingScreenshot(void) {
	typedef bool(__thiscall* OriginalFn)(PVOID);
	return GetVFunc<OriginalFn>(this, 85)(this);
}

void IVEngineClient::GetScreenSize(int& width, int& height) {
	typedef void(__thiscall* OriginalFn)(PVOID, int&, int&);
	return GetVFunc<OriginalFn>(this, 5)(this, width, height);
}

bool IVEngineClient::GetPlayerInfo(int ent, player_info_t* pinfo) {
	typedef bool(__thiscall* OriginalFn)(PVOID, int, player_info_t*);
	return GetVFunc<OriginalFn>(this, 8)(this, ent, pinfo);
}

bool IVEngineClient::Con_IsVisible(void) {
	typedef bool(__thiscall* OriginalFn)(PVOID);
	return GetVFunc<OriginalFn>(this, 11)(this);
}

float IVEngineClient::Time(void) {
	typedef float(__thiscall* OriginalFn)(PVOID);
	return GetVFunc<OriginalFn>(this, 14)(this);
}

void IVEngineClient::GetViewAngles(Vector& vAngle) {
	typedef void(__thiscall* OriginalFn)(PVOID, Vector& vAngle);
	return GetVFunc<OriginalFn>(this, 19)(this, vAngle);
}

void IVEngineClient::SetViewAngles(Vector& vAngle) {
	typedef void(__thiscall* OriginalFn)(PVOID, Vector& vAngle);
	return GetVFunc<OriginalFn>(this, 20)(this, vAngle);
}

int IVEngineClient::GetMaxClients(void) {
	typedef int(__thiscall* OriginalFn)(PVOID);
	return GetVFunc<OriginalFn>(this, 21)(this);
}

bool IVEngineClient::IsDrawingLoadingImage(void) {
	typedef bool(__thiscall* OriginalFn)(PVOID);
	return GetVFunc<OriginalFn>(this, 28)(this);
}

const matrix3x4& IVEngineClient::WorldToScreenMatrix(void) {
	typedef const matrix3x4& (__thiscall* OriginalFn)(PVOID);
	return GetVFunc<OriginalFn>(this, 36)(this);
}

DWORD* IVEngineClient::GetNetChannelInfo(void) {
	typedef DWORD* (__thiscall* OriginalFn)(PVOID);
	return GetVFunc<OriginalFn>(this, 72)(this);
}

void IVEngineClient::ClientCmd_Unrestricted(const char* chCommandString) {
	typedef void(__thiscall* OriginalFn)(PVOID, const char*);
	return GetVFunc<OriginalFn>(this, 106)(this, chCommandString);
}