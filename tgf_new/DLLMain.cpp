#include "Cheat.h"

#include <mmsystem.h>
#include <gdiplus.h>
#include <WinInet.h>
#include <TlHelp32.h>

void CheatThread()
{
	U::Setup();

	// loop for functions/features that are not needed in a hooked function
	while( !G::PressedKeys[ VK_END ] )
	{
		if( I::Engine->IsInGame() )
		{
			if( Vars.Misc.ChatSpam )
				E::Misc->Chatspam();

			if( Vars.Misc.Ranks && G::PressedKeys[ VK_TAB ] )
				U::ServerRankRevealAll();
		}

		Sleep( 1 );
	}

	E::Misc->Panic();
}

inline bool AntiDebugg3()
{
	__try
	{
		__asm __emit 0xF3 // 0xF3 0x64 disassembles as PREFIX REP:
		__asm __emit 0x64
		__asm __emit 0xF1 // One byte INT 1
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}

DWORD GetProcessIdByName(LPCTSTR name)
{
	PROCESSENTRY32 pe32;
	HANDLE snapshot = NULL;
	DWORD pid = 0;

	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot != INVALID_HANDLE_VALUE) {
		pe32.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(snapshot, &pe32)) {
			do {
				std::string sName = pe32.szExeFile;
				std::transform(sName.begin(), sName.end(), sName.begin(), ::tolower);

				if (!lstrcmp(sName.c_str(), name)) {
					pid = pe32.th32ProcessID;
					break;
				}
			} while (Process32Next(snapshot, &pe32));
		}

		CloseHandle(snapshot);
	}

	return pid;
}

inline BOOL CanOpenCsrss()
{
	HANDLE hCsrss = 0;

	hCsrss = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetProcessIdByName(charenc("csrss.exe")));

	if (hCsrss != NULL)
	{
		CloseHandle(hCsrss);
		return TRUE;
	}
	else
		return FALSE;

}

inline void CheckProcess()
{
	std::vector<std::string> m_processList;
	m_processList.push_back(charenc("ollydbg.exe"));
	m_processList.push_back(charenc("wireshark.exe"));
	m_processList.push_back(charenc("lordpe.exe"));
	m_processList.push_back(charenc("hookshark.exe"));
	m_processList.push_back(charenc("idag.exe"));

	for (unsigned int ax = 0; ax < m_processList.size(); ax++)
	{
		std::string sProcess = m_processList.at(ax);
		if (GetProcessIdByName(sProcess.c_str()) != 0)
		{
			ExitProcess(0);
		}
	}
}

class Polymorph {
protected:
	int width, height;
public:
	Polymorph(int a = 0, int b = 0)
	{
		width = a;
		height = b;
	}
	int area()
	{
		return 0;
	}
};

class Rectangley : public Polymorph {
public:
	Rectangley(int a = 0, int b = 0) :Polymorph(a, b) { }
	int area()
	{
		return (width * height);
	}
};

class Triangle : public Polymorph {
public:
	Triangle(int a = 0, int b = 0) :Polymorph(a, b) { }
	int area()
	{
		return (width * height / 2);
	}
};

class Virtual_Holder
{
public:
	Virtual_Holder() {  }
	virtual void test_fnc(void *) = 0;
};

class Definition_Holder : public Virtual_Holder
{
public:
	Definition_Holder() {  }
	void test_fnc(void * text)
	{
		std::cout << (char *)text << std::endl;
	}
};

// Cryptt
std::string encrypt(std::string msg, std::string key)
{
	std::string tmp(key);
	while (key.size() < msg.size())
		key += tmp;

	for (std::string::size_type i = 0; i < msg.size(); ++i)
		msg[i] ^= key[i];
	return msg;
}

std::string decrypt(std::string msg, std::string key)
{
	return encrypt(msg, key); // lol
}

// DBG
int AntiDebugg2()
{
	if (IsDebuggerPresent())
	{
		exit(-1);
		ExitProcess(0);
	}
	return 0;
}

// Oly DBG
void AntiDebugg()
{
	HANDLE hWinDbg = FindWindow(TEXT("WinDbgFrameClass"), NULL);

	if (hWinDbg)
		ExitProcess(0);

	HANDLE hOlly = FindWindow(TEXT("OLLYDBG"), NULL);

	if (hOlly)
	{
		MessageBox(NULL, "Error (0x1)", "elect-cheats.com", MB_OK);
		Sleep(1000);
		ExitProcess(0);
	}
}

// Thread Hide
inline bool ThreadHiding(HANDLE hThread)
{
	typedef NTSTATUS(NTAPI *pNtSetInformationThread)
		(HANDLE, UINT, PVOID, ULONG);
	NTSTATUS Status;

	pNtSetInformationThread NtSIT = (pNtSetInformationThread)
		GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")),
			"NtSetInformationThread");

	if (NtSIT == NULL)
		return false;

	if (hThread == NULL)
		Status = NtSIT(GetCurrentThread(),
			0x11,
			0, 0);

	else

		Status = NtSIT(hThread, 0x11, 0, 0);

	if (Status != 0x00000000)
		return false;

	else

		return true;
}

class HardwareID {
	__forceinline unsigned long crc32(const char* s)
	{
		unsigned long* crctable = new unsigned long[256];
		unsigned long x;
		for (int i = 0; i < 256; i++)
		{
			x = i;
			for (int j = 8; j > 0; j--)
			{
				if (x & 1)
					x = (x >> 1) ^ 0xEDB88320;
				else
					x >>= 1;
			}
			crctable[i] = x;
		}
		register unsigned long dwCrc32 = 0xFFFFFFFF;
		while (*s) {
			dwCrc32 = ((dwCrc32) >> 8) ^ crctable[*s ^ ((dwCrc32)& 0xFF)];
			s++;
		}
		return ~dwCrc32;
	}
public:
	bool HWIDCheck(unsigned long hwid) {
		unsigned long buffer = 255;
		GetVolumeInformationA("C:\\", 0, 0, &buffer, 0, 0, 0, 0);
		char s[255];
		_ultoa_s(buffer, s, 16);
		if (!crc32(s) == hwid)
		{
			exit(-1);
			ExitProcess(0);
		}
		if (crc32(s) == hwid) return true;
		return false;
	}
};

HardwareID* hwid = new HardwareID();

inline bool NtGlobals()
{
	DWORD NtGlobalFlags;
	__asm
	{
		mov eax, fs:[30h]
		mov eax, [eax + 68h]
			mov NtGlobalFlags, eax
	}

	if (NtGlobalFlags & 0x70)
		return true;

	return false;
}

void ExitProcessHidden(UINT uExitCode)
{
	ExitProcess(0);
	Sleep(3000);
}

void LoopChecks()
{
	if (GetProcessIdByName(charenc("idag.exe")) != 0)
	{
		MessageBox(NULL, "idafree50 debug", "elect-cheats.com", MB_OK);
		ExitProcess(0);
		int* p = 0;
		*p = 0;
		ExitProcessHidden(0);
	}

	if (GetProcessIdByName(charenc("idafree50.exe")) != 0)
	{
		MessageBox(NULL, "idafree50 debug", "elect-cheats.com", MB_OK);
		ExitProcess(0);
		int* p = 0;
		*p = 0;
		ExitProcessHidden(0);
	}

	if (GetProcessIdByName(charenc("idafree.exe")) != 0)
	{
		MessageBox(NULL, "idafree debug", "elect-cheats.com", MB_OK);
		ExitProcess(0);
		int* p = 0;
		*p = 0;
		ExitProcessHidden(0);
	}

	if (GetProcessIdByName(charenc("ollydbg.exe")) != 0)
	{
		MessageBox(NULL, "ollydbg debug", "elect-cheats.com", MB_OK);
		ExitProcess(0);

		int* p = 0;
		*p = 0;
		ExitProcessHidden(0);
	}

	if (GetProcessIdByName(charenc("wireshark.exe")) != 0)
	{
		MessageBox(NULL, "wireshark debug", "elect-cheats.com", MB_OK);
		ExitProcess(0);
		int* p = 0;
		*p = 0;
		ExitProcessHidden(0);
	}

	if (GetProcessIdByName(charenc("lordpe.exe")) != 0)
	{
		MessageBox(NULL, "lordpe debug", "elect-cheats.com", MB_OK);
		ExitProcess(0);
		int* p = 0;
		*p = 0;
		ExitProcessHidden(0);
	}

	if (GetProcessIdByName(charenc("hookshark.exe")) != 0)
	{
		MessageBox(NULL, "hookshark debug", "elect-cheats.com", MB_OK);
		ExitProcess(0);
		int* p = 0;
		*p = 0;
		ExitProcessHidden(0);
	}

	if (GetProcessIdByName(charenc("idag.exe")) != 0)
	{
		MessageBox(NULL, "idag debug", "elect-cheats.com", MB_OK);
		ExitProcess(0);
		int* p = 0;
		*p = 0;
		ExitProcessHidden(0);
	}

	if (GetProcessIdByName(charenc("TeamViewer.exe")) != 0)
	{
		MessageBox(NULL, "Close TeamViewer", "elect-cheats.com", MB_OK);
		ExitProcess(0);
		int* p = 0;
		*p = 0;
		ExitProcessHidden(0);
	}


	if (AntiDebugg3())
	{
		int* p = 0;
		*p = 0;
		ExitProcess(0);
		ExitProcessHidden(0);
	}

	if (NtGlobals())
	{
		int* p = 0;
		*p = 0;
		ExitProcess(0);
		ExitProcessHidden(0);
	}
}

DWORD WINAPI DllMain( HMODULE hDll, DWORD dwReason, LPVOID lpReserved )
{
	switch( dwReason )
	{
	case DLL_PROCESS_ATTACH:
	{
		while( !( G::Window = FindWindowA( charenc( "Valve001" ), NULL ) ) ) Sleep( 200 );

		Hooks::oldWindowProc = ( WNDPROC )SetWindowLongPtr( G::Window, GWL_WNDPROC, ( LONG_PTR )Hooks::WndProc );

		CreateThread( NULL, NULL, ( LPTHREAD_START_ROUTINE )CheatThread, NULL, NULL, NULL );
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)LoopChecks, NULL, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)AntiDebugg, NULL, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadHiding, NULL, NULL, NULL);

		G::Dll = hDll;

		return TRUE;
	}
	case DLL_PROCESS_DETACH:
	{
		return TRUE;
	}

	return FALSE;
	}
}