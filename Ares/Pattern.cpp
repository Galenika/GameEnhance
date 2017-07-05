// Pattern
#include "Ares.h"

uint64_t Pattern::FindSignature(const char* szModule, const char* szSignature) {

	MODULEINFO modInfo;
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
	DWORD startAddress = (DWORD)modInfo.lpBaseOfDll;
	DWORD endAddress = startAddress + modInfo.SizeOfImage;
	const char* pat = szSignature;
	DWORD firstMatch = 0;
	for (DWORD pCur = startAddress; pCur < endAddress; pCur++) {
		if (!*pat) return firstMatch;
		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
			if (!firstMatch) firstMatch = pCur;
			if (!pat[2]) return firstMatch;
			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
			else pat += 2;    //one ?
		}
		else {
			pat = szSignature;
			firstMatch = 0;
		}
	}

	return NULL;

}