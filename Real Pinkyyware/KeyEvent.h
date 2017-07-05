#pragma once
extern Base::Utils::CVMTHookManager g_pInKeyEventVMT;
int __stdcall hkdInKeyEvent(int eventcode, const char *pszCurrentBinding);