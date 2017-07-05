#ifndef _SOUND_ENGINE_H_
#define _SOUND_ENGINE_H_
//===================================================================================
typedef struct SoundMarker_t
{
	std::string strSoundName;
	std::string strPlayerName;
	int			iTeam;
	int			iIndex;
	Vector		vOrigin;
	DWORD		dwTime;
};
extern SoundMarker_t gSoundMarker[64];
//===================================================================================
struct CSoundEngine
{
	bool(__stdcall *PrecacheSound)(const char *pSample,bool preload);
	bool(__stdcall *IsSoundPrecached)(const char *pSample);
	void(__stdcall *PrefetchSound)(const char *pSample);
	float(__stdcall *GetSoundDuration)(const char *pSample);
	void(__stdcall *EmitSound)(IRecipientFilter& filter,int iEntIndex,int iChannel,const char *pSample,float flVolume,float flAttenuation,int iFlags,int iPitch,const Vector *pOrigin,const Vector *pDirection,CUtlVector< Vector >* pUtlVecOrigins,bool bUpdatePositions,float soundtime,int speakerentity);
	void(__stdcall *EmitSound2)(IRecipientFilter& filter,int iEntIndex,int iChannel,const char *pSample,float flVolume,soundlevel_t iSoundlevel,int iFlags,int iPitch,const Vector *pOrigin,const Vector *pDirection,CUtlVector<Vector>* pUtlVecOrigins ,bool bUpdatePositions ,float soundtime,int speakerentity);
	void(__stdcall *EmitSentenceByIndex)(IRecipientFilter& filter,int iEntIndex,int iChannel,int iSentenceIndex,float flVolume,soundlevel_t iSoundlevel,int iFlags,int iPitch,const Vector *pOrigin,const Vector *pDirection,CUtlVector<Vector>* pUtlVecOrigins,bool bUpdatePositions,float soundtime,int speakerentity);
	void(__stdcall *StopSound)(int iEntIndex,int iChannel,const char *pSample);
	void(__stdcall *StopAllSounds)(bool bClearBuffers);
	void(__stdcall *SetRoomType)(IRecipientFilter& filter,int roomType);
	void(__stdcall *SetPlayerDSP)(IRecipientFilter& filter,int dspType,bool fastReset);
	void(__stdcall *EmitAmbientSound)(const char *pSample,float flVolume,int iPitch,int flags,float soundtime);
	float(__stdcall *GetDistGainFromSoundLevel)(soundlevel_t soundlevel,float dist);
};
//===================================================================================
extern CSoundEngine gEngineSound;
//===================================================================================
extern void __stdcall Hooked_EmitSound(IRecipientFilter&,int,int,const char*,float,float,int,int,const Vector*,const Vector*,CUtlVector<Vector>*,bool,float,int);
void BuildEngineSoundHook( PDWORD* pdwEngineSound );
//===================================================================================
#endif