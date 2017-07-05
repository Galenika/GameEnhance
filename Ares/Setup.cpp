// Setup
#include "Ares.h"

// Interface pointers
IClientEntityList* g_pIEntList;
IClientModeShared* g_pClientMode;
IVEngineClient* g_pEngineClient;
CHLClient* g_pClient;
ISurface* g_pSurface;
IPanel* g_pPanel;

// VirtualMethodTable pointers
std::unique_ptr<CVMT> g_pClientModeHook;
std::unique_ptr<CVMT> g_pPanelHook;

// Various other pointers
CMath* g_pMath;

// Factories
CreateInterfaceFn oClientFactory = nullptr;
CreateInterfaceFn oEngineFactory = nullptr;
CreateInterfaceFn oVGUIFactory = nullptr;
CreateInterfaceFn oVGUI2Factory = nullptr;

// Fonts
unsigned long Fonts::Font;

#pragma region Hooks

// CreateMove
typedef bool(__thiscall* CreateMoveFn)(void*, float, CUserCmd*); // Function prototype.
bool __stdcall hkCreateMove(float flInputSampleTime, CUserCmd* m_pCmd) {

	g_pClientModeHook->GetOriginalFunction<CreateMoveFn>(21)(g_pClientMode, flInputSampleTime, m_pCmd); /* Call the original ClientMode::CreateMove th_off
	ensure csgo runs fine */

	if (m_pCmd->command_number == 0) {
		return false; /* Returns an increasing number representing the index of the user cmd.
        The value returned is occasionally 0 inside CreateMove,
        so it's advised to check for a non-zero value if you wish to get the correct number.
		*/
	}

	auto m_pLocal = (CBaseEntity*)g_pIEntList->GetClientEntity(g_pEngineClient->GetLocalPlayer()); /* Aquire a pointer
	to our local player */

	if (m_pCmd->buttons & IN_JUMP) { // Make sure the IN_JUMP flag is set
		if (!(m_pLocal->GetFlags() & FL_ONGROUND)) { // Make sure our flags match FL_ONGROUND
			m_pCmd->buttons &= ~IN_JUMP; // Set IN_JUMP (which is a bitshifting operation) to AND NOT using &= and ~
		}
	}

	return true; // Make sure ExtraMouseSample is called.

}

// PaintTraverse
typedef void(__thiscall* PaintTraverseFn)(void*, unsigned int, bool, bool);
void __fastcall hkPaintTraverse(void* ecx, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce) {

	g_pPanelHook->GetOriginalFunction<PaintTraverseFn>(41)(ecx, vguiPanel, forceRepaint, allowForce); /* Call
	the original painttraverse.
	*/

	static unsigned int m_iDrawPanel;
	static bool m_bCallOnce = false;
	
	if (!m_iDrawPanel)
		if (strstr(g_pPanel->GetName(vguiPanel), "MatSystemTopPanel"))
			m_iDrawPanel = vguiPanel;

	if (vguiPanel != m_iDrawPanel)
		return;

	if (!m_bCallOnce) {

		Fonts::Font = g_pSurface->CreateFont();
		g_pSurface->SetFontGlyphSet(Fonts::Font, "Calibri", 14, 700, 0, 0, FONTFLAG_OUTLINE);

		m_bCallOnce = true;

	}

	if (g_pEngineClient->IsDrawingLoadingImage() || !g_pEngineClient->IsInGame() || !g_pEngineClient->IsConnected() || g_pEngineClient->Con_IsVisible() || g_pEngineClient->IsTakingScreenshot()) return;

	Draw::DrawString(4, 4, Color(128, 0, 128, 255), Fonts::Font, "Ares  1.0.1 | www.github.com/gitaphu");
	Draw::DrawString(4, 14, Color(255, 0, 0, 255), Fonts::Font, "Public version");

}

#pragma endregion

#pragma region Helper Functions

// Dynamically grab interface pointers
void* GetInterfacePtr(const char* interfaceName, const char* ptrName, CreateInterfaceFn pInterface) {

	char szDebugString[1024];
	std::string sinterface = "";
	std::string interfaceVersion = "0";

	for (int i = 0; i <= 99; i++) {

		sinterface = interfaceName;
		sinterface += interfaceVersion;
		sinterface += std::to_string(i);

		void* funcPtr = pInterface(sinterface.c_str(), NULL);

		if ((DWORD)funcPtr != 0x0) {
			sprintf(szDebugString, "%s: 0x%x (%s%s%i)", ptrName, (DWORD)funcPtr, interfaceName, interfaceVersion.c_str(), i);
			return funcPtr;
		}
		if (i >= 99 && interfaceVersion == "0") {
			interfaceVersion = "00";
			i = 0;
		}
		else if (i >= 99 && interfaceVersion == "00") {
			sprintf(szDebugString, "%s: 0x%x (error)", ptrName, (DWORD)funcPtr);
		}

	}

	return 0;
}

#pragma endregion

#pragma region Setup

void Setup::SetupInterfaces() {

	if (g_pClient == nullptr) { // prevent repeat calling

		// Client
		oClientFactory = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(GetModuleHandleA("client.dll"), "CreateInterface"));
		g_pClient = static_cast<CHLClient*>(GetInterfacePtr("VClient", "g_pClient", oClientFactory));
		g_pIEntList = static_cast<IClientEntityList*>(GetInterfacePtr("VClientEntityList", "g_pIEntList", oClientFactory));

		// Engine
		oEngineFactory = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(GetModuleHandleA("engine.dll"), "CreateInterface"));
		g_pEngineClient = static_cast<IVEngineClient*>(GetInterfacePtr("VEngineClient", "g_pEngineClient", oEngineFactory));

		// VGUI
		oVGUIFactory = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(GetModuleHandleA("vguimatsurface.dll"), "CreateInterface"));
		oVGUI2Factory = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(GetModuleHandleA("vgui2.dll"), "CreateInterface"));
		g_pSurface = static_cast<ISurface*>(GetInterfacePtr("VGUI_Surface", "g_pSurface", oVGUIFactory));
		g_pPanel = static_cast<IPanel*>(GetInterfacePtr("VGUI_Panel", "g_pPanel", oVGUI2Factory));


	}

}

void Setup::SetupHooks() {

	if (g_pClient) {

		// Grab clientmode
		g_pClientMode = **(IClientModeShared***)((*(DWORD**)g_pClient)[10] + 0x5); // Aquire a pointer to IClientModeShared
		g_pClientModeHook = std::make_unique<CVMT>(g_pClientMode); // Initialize our clientmode hook

		g_pClientModeHook->HookFunction((void*)hkCreateMove, 21); // Replace CreateMove with our own function

	}

	if (g_pPanel) {

		g_pPanelHook = std::make_unique<CVMT>(g_pPanel);
		g_pPanelHook->HookFunction((void*)hkPaintTraverse, 41);

	}

}

#pragma endregion