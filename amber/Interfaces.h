/*
Syn's AyyWare Framework
*/

#pragma once

// Namespace to contain all the valve interfaces
namespace Interfaces
{
	class IClientModeShared
	{
	public:
	};
	// Gets handles to all the interfaces needed
	void Initialise();

	extern IBaseClientDLL* Client;
	extern IVEngineClient* Engine;
	extern IPanel* Panels;
	extern IClientEntityList* EntList;
	extern ISurface* Surface;
	extern IVDebugOverlay* DebugOverlay;
	extern IClientModeShared *ClientMode;
	extern CGlobalVarsBase *Globals;
	extern DWORD *Prediction;
	extern CMaterialSystem* MaterialSystem;
	extern CVRenderView* RenderView;
	extern IVModelRender* ModelRender;
	extern CModelInfo* ModelInfo;
	extern IEngineTrace* Trace;
	extern IPhysicsSurfaceProps* PhysProps;
	extern ICVar *CVar;

	extern CInput* pInput;
	extern IPrediction *Prediction2;
	extern IMoveHelper *MoveHelper;
	extern IGameMovement *GameMovement;
	extern IGameEventManager*	GameEventManager;
};