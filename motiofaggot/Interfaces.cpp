#include "Interfaces.h"
#include "Utilities.h"

//SDK Specific Definitions
typedef void* (__cdecl* CreateInterface_t)(const char*, int*);
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

//Some globals for later
CreateInterface_t EngineFactory = NULL; // These are used to store the individual
CreateInterface_t ClientFactory = NULL; //  CreateInterface functions for each game
CreateInterface_t VGUISurfaceFactory = NULL; //  dll that we need access to. Can call
CreateInterface_t VGUI2Factory = NULL; //  them to recieve pointers to game classes.
CreateInterface_t MatFactory = NULL;
CreateInterface_t PhysFactory = NULL;
CreateInterface_t StdFactory = NULL;
CreateInterface_t InputFactory = NULL;

void Interfaces::Initialise()
{

	//Get function pointers to the CreateInterface function of each module
	EngineFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Engine, "CreateInterface");
	ClientFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Client, "CreateInterface");
	VGUI2Factory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VGUI2, "CreateInterface");
	VGUISurfaceFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VGUISurface, "CreateInterface");
	MatFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Material, "CreateInterface");
	PhysFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VPhysics, "CreateInterface");
	StdFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Stdlib, "CreateInterface");
	InputFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::InputLib, "CreateInterface");

	//Get the interface names regardless of their version number by scanning for each string
	char* CHLClientInterfaceName = (char*)Utilities::Memory::FindTextPattern("client.dll", "VClient0");
	char* VGUI2PanelsInterfaceName = (char*)Utilities::Memory::FindTextPattern("vgui2.dll", "VGUI_Panel0");
	char* VGUISurfaceInterfaceName = (char*)Utilities::Memory::FindTextPattern("vguimatsurface.dll", "VGUI_Surface0");
	char* EntityListInterfaceName = (char*)Utilities::Memory::FindTextPattern("client.dll", "VClientEntityList0");
	char* EngineDebugThingInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VDebugOverlay0");
	char* EngineClientInterfaceName = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineClient0");
	char* ClientPredictionInterface = (char*)Utilities::Memory::FindTextPattern("client.dll", "VClientPrediction0");
	char* MatSystemInterfaceName = (char*)Utilities::Memory::FindTextPattern("materialsystem.dll", "VMaterialSystem0");
	char* EngineRenderViewInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineRenderView0");
	char* EngineModelRenderInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineModel0");
	char* EngineModelInfoInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VModelInfoClient0");
	char* EngineTraceInterfaceName = (char*)Utilities::Memory::FindTextPattern("engine.dll", "EngineTraceClient0");
	char* PhysPropsInterfaces = (char*)Utilities::Memory::FindTextPattern("client.dll", "VPhysicsSurfaceProps0");
	char* VEngineCvarName = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineCvar00");
	char* VEngineEffect = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineEffects001");
	char* SInputSystem = (char*)Utilities::Memory::FindTextPattern("inputsystem.dll", "InputSystemVersion001");

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
	Dilight = (IVEffects*)EngineFactory(VEngineEffect, NULL);
	InputSystem = (IInputSystem*)InputFactory(SInputSystem, NULL);

	// Get ClientMode Pointer
	DWORD* ppClientMode;
	ppClientMode = nullptr; // before "scripts/vgui_screens.txt"
	DWORD p = Utilities::Memory::FindPatternV2("client.dll", "8B 0D ? ? ? ? 8B 01 5D FF");
	if (p)
	{
		ppClientMode = **(DWORD***)(p + 2);
		ClientMode = ppClientMode;
	}

	DWORD* ClientVMT = (DWORD*)*(DWORD*)Client;

	// Search through the first entry of the Client VTable
	// The initializer contains a pointer to the 'GlobalsVariables' Table
	PDWORD pdwClient = (PDWORD)*(PDWORD)Client;
	DWORD dwInitAddr = (DWORD)(pdwClient[0]);
	for (DWORD dwIter = 0; dwIter <= 0xFF; dwIter++)
	{
		if (*(PBYTE)(dwInitAddr + dwIter - 1) == 0x08 && *(PBYTE)(dwInitAddr + dwIter) == 0xA3)
		{
			Globals = (CGlobalVarsBase*)*(PDWORD)*(PDWORD)(dwInitAddr + dwIter + 1);
			break;
		}
	}
	Globals = **reinterpret_cast< CGlobalVarsBase*** >((*reinterpret_cast< DWORD** >(Client))[0] + 0x1B);
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
	IInputSystem* InputSystem;
	DWORD *ClientMode;
	CGlobalVarsBase *Globals;
	DWORD *Prediction;
	CMaterialSystem* MaterialSystem;
	CVRenderView* RenderView;
	IVModelRender* ModelRender;
	CModelInfo* ModelInfo;
	IEngineTrace* Trace;
	IPhysicsSurfaceProps* PhysProps;
	ICVar *CVar;
	IVEffects* Dilight;
	IMoveHelper* Interfaces::MoveHelper;
	IPrediction *Interfaces::Prediction1;
	IGameMovement* Interfaces::GameMovement;
	CInput* pInput;
};

void f12aggot115616511891985156()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}

void f12ag6156156got11151651658918918951186()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}

void f12aggo165165165189819t15151987986()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12115616515689189189aggot15156741894()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot15616516511891891895156489489489()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12a98561651567987ggot151156165156564894894984984984()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot17897987978978978951513131313136()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12ag79877897987897got15156131313189798()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12ag78978978998got148948951589767898798797897897984151()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12a789798798ggot897987987151569879816351687987()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12a789789798ggot1578979871561568979812313()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggo15789798761651t157897987897156897()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12a798156157987ggot15156789797854646597()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot1515151515123612015678978978979789789()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggo155118998t1511561654564564578978978979878978971111564136()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f1215616515645641aggot15156417421784197219718271()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12498489445689415aggot151564114216792()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f1241894984879871261698aggot151562713879124819421718972189()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggo156456456t15156108979874218971279812412()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12ag8979871561651got151894984151421741729296()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12agg48498489849984ot15156189198()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12ag489489498got1489498515cf489498bbhhfgh6()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot1515489484894894989489hhtdffghh1rt5616()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12a798798789ggot151erghe489489r5g9dgdf98df1g56()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}