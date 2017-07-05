#include "SDK.h"
#include "Sound Engine.h"
#include "VMT Hook.h"
#include "Control Variables.h"
//===================================================================================
CSoundEngine gEngineSound;
//===================================================================================
SoundMarker_t gSoundMarker[64];
//===================================================================================
CVMTHookManager* m_pEngineSoundHook = NULL;
//===================================================================================
void __stdcall Hooked_EmitSound( IRecipientFilter &filter, int iEntIndex, int iChannel, const char *pSample, float flVolume, float flAttenuation, int iFlags, int iPitch, const Vector *pOrigin, const Vector *pDirection, CUtlVector<Vector>*pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity )
{
	if( gCvars.esp_sound )
	{
		switch( iChannel )
		{
			case CHAN_BODY:
			if( strstr( pSample, "footsteps" ) )
			{
				if ( iEntIndex != g_pEngine->GetLocalPlayer( ) )
				{
					SoundMarker_t& TempSound_s  =  gSoundMarker[iEntIndex]; 	
					TempSound_s.dwTime			= timeGetTime( ); 		
					TempSound_s.vOrigin			= *pOrigin; 		
					TempSound_s.iIndex			= iEntIndex; 		
					TempSound_s.strSoundName	= "step"; 		 
				}	
			}
			break;
		}
	}

	gEngineSound.EmitSound( filter, iEntIndex, iChannel, pSample, flVolume, flAttenuation, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity );
}
//===================================================================================
void BuildEngineSoundHook( PDWORD* pdwEngineSound )
{
	RtlCopyMemory( (void*)&gEngineSound, (void*)*pdwEngineSound , sizeof( CSoundEngine ) );

	m_pEngineSoundHook = new CVMTHookManager( (PDWORD*)pdwEngineSound );
	m_pEngineSoundHook->dwHookMethod( (DWORD)Hooked_EmitSound, 4 );
}
//===================================================================================