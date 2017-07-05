// Setup
#pragma once

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode); // https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/tier1/interface.h#L68

namespace Fonts {
	extern unsigned long Font;
}

namespace Setup {
	void SetupInterfaces();
	void SetupHooks();
}