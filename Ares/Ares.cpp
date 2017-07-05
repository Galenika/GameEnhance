/*
    _              
   /_\  _ _ ___ ___
  / _ \| '_/ -_|_-<
 /_/ \_\_| \___/__/
      
*/
#include "Ares.h"

#pragma region Setup
 
// Description: Intialize everything in this function. (Grabbing interface pointers, hooking, etc).
void Ares(PVOID pParam) {

	// Since we're creating our own thread and not using a game thread, sleep should work just fine here.

	while (FindWindowA("Valve001", 0) == 0)
		Sleep(100);

	while (GetModuleHandleA("client.dll") == 0 || GetModuleHandleA("engine.dll") == 0)
		Sleep(100);

	Setup::SetupInterfaces();
	Setup::SetupHooks();
	
	_endthread(); // Given to terminate.

}

// Entry
BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,  // handle to DLL module.
	DWORD fdwReason,     // reason for calling function.
	LPVOID lpReserved)  // reserved.
{
	// Perform actions based on the reason for calling.
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		_beginthread(Ares, 0, NULL);
		break;
	}
	return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

#pragma endregion