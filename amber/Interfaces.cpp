

#include "Interfaces.h"
#include "Utilities.h"

//SDK Specific Definitions
typedef void* (__cdecl* CreateInterface_t)(const char*, int*);
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

//Some globals for later
CreateInterfaceFn fnEngineFactory = NULL;
CreateInterface_t EngineFactory = NULL; // These are used to store the individual
CreateInterface_t ClientFactory = NULL; //  CreateInterface functions for each game
CreateInterface_t VGUISurfaceFactory = NULL; //  dll that we need access to. Can call
CreateInterface_t VGUI2Factory = NULL; //  them to recieve pointers to game classes.
CreateInterface_t MatFactory = NULL;
CreateInterface_t PhysFactory = NULL;
CreateInterface_t StdFactory = NULL;

void Interfaces::Initialise()
{
	
	//Get function pointers to the CreateInterface function of each module
	fnEngineFactory = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(XorStr("engine.dll")), XorStr("CreateInterface"));
	EngineFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Engine, XorStr("CreateInterface"));
	ClientFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Client, XorStr("CreateInterface"));
	VGUI2Factory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VGUI2, XorStr("CreateInterface"));
	VGUISurfaceFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VGUISurface, XorStr("CreateInterface"));
	MatFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Material, XorStr("CreateInterface"));
	PhysFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VPhysics, XorStr("CreateInterface"));
	StdFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Stdlib, XorStr("CreateInterface"));

	//Get the interface names regardless of their version number by scanning for each string
	char* CHLClientInterfaceName = (char*)Utilities::Memory::FindTextPattern(XorStr("client.dll"), XorStr("VClient0"));
	char* VGUI2PanelsInterfaceName = (char*)Utilities::Memory::FindTextPattern(XorStr("vgui2.dll"), XorStr("VGUI_Panel0"));
	char* VGUISurfaceInterfaceName = (char*)Utilities::Memory::FindTextPattern(XorStr("vguimatsurface.dll"), XorStr("VGUI_Surface0"));
	char* EntityListInterfaceName = (char*)Utilities::Memory::FindTextPattern(XorStr("client.dll"), XorStr("VClientEntityList0"));
	char* EngineDebugThingInterface = (char*)Utilities::Memory::FindTextPattern(XorStr("engine.dll"), XorStr("VDebugOverlay0"));
	char* EngineClientInterfaceName = (char*)Utilities::Memory::FindTextPattern(XorStr("engine.dll"), XorStr("VEngineClient0"));
	char* ClientPredictionInterface = (char*)Utilities::Memory::FindTextPattern(XorStr("client.dll"), XorStr("VClientPrediction0"));
	char* MatSystemInterfaceName = (char*)Utilities::Memory::FindTextPattern(XorStr("materialsystem.dll"), XorStr("VMaterialSystem0"));
	char* EngineRenderViewInterface = (char*)Utilities::Memory::FindTextPattern(XorStr("engine.dll"), XorStr("VEngineRenderView0"));
	char* EngineModelRenderInterface = (char*)Utilities::Memory::FindTextPattern(XorStr("engine.dll"), XorStr("VEngineModel0"));
	char* EngineModelInfoInterface = (char*)Utilities::Memory::FindTextPattern(XorStr("engine.dll"), XorStr("VModelInfoClient0"));
	char* EngineTraceInterfaceName = (char*)Utilities::Memory::FindTextPattern(XorStr("engine.dll"), XorStr("EngineTraceClient0"));
	char* PhysPropsInterfaces = (char*)Utilities::Memory::FindTextPattern(XorStr("client.dll"), XorStr("VPhysicsSurfaceProps0"));
	char* VEngineCvarName = (char*)Utilities::Memory::FindTextPattern(XorStr("engine.dll"), XorStr("VEngineCvar00"));

	// Use the factory function pointers along with the interface versions to grab
	//  pointers to the interfaces
	Client = (IBaseClientDLL*)ClientFactory(CHLClientInterfaceName, NULL);
	Engine = (IVEngineClient*)EngineFactory(EngineClientInterfaceName, NULL);
	Panels = (IPanel*)VGUI2Factory(VGUI2PanelsInterfaceName, NULL);
	Surface = (ISurface*)VGUISurfaceFactory(VGUISurfaceInterfaceName, NULL);
	EntList = (IClientEntityList*)ClientFactory(EntityListInterfaceName, NULL);
	DebugOverlay = (IVDebugOverlay*)EngineFactory(EngineDebugThingInterface, NULL);
	Prediction = (DWORD*)ClientFactory(ClientPredictionInterface, NULL);
	MaterialSystem = (CMaterialSystem*)MatFactory(MatSystemInterfaceName, NULL);
	RenderView = (CVRenderView*)EngineFactory(EngineRenderViewInterface, NULL);
	ModelRender = (IVModelRender*)EngineFactory(EngineModelRenderInterface, NULL);
	ModelInfo = (CModelInfo*)EngineFactory(EngineModelInfoInterface, NULL);
	Trace = (IEngineTrace*)EngineFactory(EngineTraceInterfaceName, NULL);
	PhysProps = (IPhysicsSurfaceProps*)PhysFactory(PhysPropsInterfaces, NULL);
	CVar = (ICVar*)StdFactory(VEngineCvarName, NULL);

	//Engine Prediction
	Prediction2 = reinterpret_cast<IPrediction*>(Utilities::Memory::CaptureInterface(XorStr("client.dll"), XorStr("VClientPrediction")));
	GameMovement = reinterpret_cast<IGameMovement*>(Utilities::Memory::CaptureInterface(XorStr("client.dll"), XorStr("GameMovement")));
	MoveHelper = **reinterpret_cast<IMoveHelper***>(Utilities::Memory::FindPatternAlt(XorStr("client.dll"), reinterpret_cast<PBYTE>("\x8B\x0D\x00\x00\x00\x00\x6A\x02\x8B\x01\xFF\x50\x28"), "xx????xxxxxxx", 0, 0) + 0x2);
	pInput = *(CInput**)((*(DWORD**)Client)[15] + 0x1);

	GameEventManager = (IGameEventManager*)fnEngineFactory(XorStr("GAMEEVENTSMANAGER002"), NULL);

	ClientMode = **(IClientModeShared***)((*(DWORD**)Client)[10] + 0x5);
	Globals = **reinterpret_cast<CGlobalVarsBase***>((*reinterpret_cast<DWORD**>(Client))[0] + 0x1B);

	//Utilities::Log("Interfaces Loaded");
}

// Namespace to contain all the valve interfaces
namespace Interfaces
{
	IBaseClientDLL* Client;
	IVEngineClient* Engine;
	IPanel* Panels;
	IClientEntityList* EntList;
	ISurface* Surface;
	IVDebugOverlay* DebugOverlay;
	IClientModeShared *ClientMode;
	CGlobalVarsBase *Globals;
	DWORD *Prediction;
	CMaterialSystem* MaterialSystem;
	CVRenderView* RenderView;
	IVModelRender* ModelRender;
	CModelInfo* ModelInfo;
	IEngineTrace* Trace;
	IPhysicsSurfaceProps* PhysProps;
	ICVar *CVar;

	CInput* pInput;
	IPrediction *Prediction2;
	IMoveHelper* MoveHelper;
	IGameMovement* GameMovement;
	IGameEventManager*	GameEventManager;
};