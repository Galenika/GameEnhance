/*
Syn's AyyWare Framework 2015
*/

#include "Interfaces.h"
#include "Utilities.h"
#include "IPrediction.h"

void Interfaces::Initialise()
{
	GameMovement = reinterpret_cast<IGameMovement*>(Utilities::Memory::CaptureInterface(XorStr("client.dll"), XorStr("GameMovement")));

	CVar = reinterpret_cast<ICVar*>(Utilities::Memory::CaptureInterface(XorStr("engine.dll"), XorStr("VEngineCvar")));
	printf("0x%X\n", reinterpret_cast<DWORD>(CVar));

	RenderView = reinterpret_cast<CVRenderView*>(Utilities::Memory::CaptureInterface(XorStr("engine.dll"), XorStr("VEngineRenderView")));
	printf("0x%X\n", reinterpret_cast<DWORD>(RenderView));

	ModelRender = reinterpret_cast<IVModelRender*>(Utilities::Memory::CaptureInterface(XorStr("engine.dll"), XorStr("VEngineModel")));
	printf("0x%X\n", reinterpret_cast<DWORD>(ModelRender));

	ModelInfo = reinterpret_cast<CModelInfo*>(Utilities::Memory::CaptureInterface(XorStr("engine.dll"), XorStr("VModelInfoClient")));
	printf("0x%X\n", reinterpret_cast<DWORD>(ModelInfo));

	Panels = reinterpret_cast<IPanel*>(Utilities::Memory::CaptureInterface(XorStr("vgui2.dll"), XorStr("VGUI_Panel")));
	printf("0x%X\n", reinterpret_cast<DWORD>(ModelInfo));

	Surface = reinterpret_cast<ISurface*>(Utilities::Memory::CaptureInterface(XorStr("vguimatsurface.dll"), XorStr("VGUI_Surface")));
	printf("0x%X\n", reinterpret_cast<DWORD>(Surface));

	EntList = reinterpret_cast<IClientEntityList*>(Utilities::Memory::CaptureInterface(XorStr("client.dll"), XorStr("VClientEntityList")));
	printf("0x%X\n", reinterpret_cast<DWORD>(EntList));

	DebugOverlay = reinterpret_cast<IVDebugOverlay*>(Utilities::Memory::CaptureInterface(XorStr("engine.dll"), XorStr("VDebugOverlay")));
	printf("0x%X\n", reinterpret_cast<DWORD>(DebugOverlay));

	PhysProps = reinterpret_cast<IPhysicsSurfaceProps*>(Utilities::Memory::CaptureInterface(XorStr("client.dll"), XorStr("VPhysicsSurfaceProps")));
	printf("0x%X\n", reinterpret_cast<DWORD>(PhysProps));

	MaterialSystem = reinterpret_cast<CMaterialSystem*>(Utilities::Memory::CaptureInterface(XorStr("materialsystem.dll"), XorStr("VMaterialSystem")));
	printf("0x%X\n", reinterpret_cast<DWORD>(MaterialSystem));

	Client = reinterpret_cast<IBaseClientDLL*>(Utilities::Memory::CaptureInterface(XorStr("client.dll"), XorStr("VClient")));
	printf("0x%X\n", reinterpret_cast<DWORD>(Client));

	Prediction = reinterpret_cast<IPrediction*>(Utilities::Memory::CaptureInterface(XorStr("client.dll"), XorStr("VClientPrediction")));
	printf("0x%X\n", reinterpret_cast<DWORD>(Prediction));

	Engine = reinterpret_cast<IVEngineClient*>(Utilities::Memory::CaptureInterface(XorStr("engine.dll"), XorStr("VEngineClient")));
	printf("0x%X\n", reinterpret_cast<DWORD>(Engine));

	Trace = reinterpret_cast<IEngineTrace*>(Utilities::Memory::CaptureInterface(XorStr("engine.dll"), XorStr("EngineTraceClient")));
	printf("0x%X\n", reinterpret_cast<DWORD>(Trace));

	ClientMode = **reinterpret_cast<IClientModeShared***>((*reinterpret_cast<DWORD**>(Client))[10] + 0x5);
	printf("0x%X\n", reinterpret_cast<DWORD>(ClientMode));

	Globals = **reinterpret_cast<CGlobalVarsBase***>((*reinterpret_cast<DWORD**>(Client))[0] + 0x1B);
	printf("0x%X\n", reinterpret_cast<DWORD>(Globals));
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
	IClientModeShared*		ClientMode;
	CGlobalVarsBase *Globals;
	IPrediction* Prediction;
	CMaterialSystem* MaterialSystem;
	CVRenderView* RenderView;
	IVModelRender* ModelRender;
	CModelInfo* ModelInfo;
	IEngineTrace* Trace;
	IPhysicsSurfaceProps* PhysProps;
	ICVar *CVar;
	CInput* pInput;
	IMoveHelper* Interfaces::MoveHelper;
	IGameMovement* Interfaces::GameMovement;
};