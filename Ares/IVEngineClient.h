// IVEngineClient
#pragma once

typedef struct player_info_s {
	int64_t __pad0;
	union {
		int64_t xuid;
		struct {
			int xuidlow;
			int xuidhigh;
		};
	};
	char name[128];
	int userid;
	char guid[33];
	unsigned int friendsid;
	char friendsname[128];
	bool fakeplayer;
	bool ishltv;
	unsigned int customfiles[4];
	unsigned char filesdownloaded;
} player_info_t;

class IVEngineClient {
public:
	int GetLocalPlayer(void);
	bool IsInGame(void);
	bool IsConnected(void);
	bool IsTakingScreenshot(void);
	void GetScreenSize(int& width, int& height);
	bool GetPlayerInfo(int ent, player_info_t* pinfo);
	bool Con_IsVisible(void);
	float Time(void);
	void GetViewAngles(Vector& vAngle);
	void SetViewAngles(Vector& vAngle);
	int GetMaxClients(void);
	bool IsDrawingLoadingImage(void);
	const matrix3x4& WorldToScreenMatrix(void);
	DWORD* GetNetChannelInfo(void);
	void ClientCmd_Unrestricted(const char* chCommandString);
}; extern IVEngineClient* g_pEngineClient;