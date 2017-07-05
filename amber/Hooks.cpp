/*
Syn's AyyWare Framework 2015
*/

#include "Hooks.h"
#include "Hacks.h"
#include "Chams.h"
#include "Menu.h"
#include "Entities.h"

#include "Interfaces.h"
#include "RenderManager.h"
#include "MiscHacks.h"
#include "ViewSetup.h"
#include "Globals.h"
#include "cx_strenc.h"
#include "LBYHook.h"
#include <unordered_map>
#include <chrono>

#include "Autowall.h"
#include "Resolver.h"
#include "CircleStrafe.h"

std::unordered_map<MaterialHandle_t, Color> worldMaterials;
std::unordered_map<MaterialHandle_t, Color> worldMaterials2;
#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))

// Funtion Typedefs
typedef void(__thiscall* DrawModelEx_)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4*);
typedef void(__thiscall* PaintTraverse_)(PVOID, unsigned int, bool, bool);
typedef bool(__thiscall* InPrediction_)(PVOID);
typedef void(__stdcall *FrameStageNotifyFn)(ClientFrameStage_t);
typedef bool(__thiscall *FireEventClientSideFn)(void*, IGameEvent*);
typedef float(__stdcall *oGetViewModelFOV)();

// Function Pointers to the originals
PaintTraverse_ oPaintTraverse;
DrawModelEx_ oDrawModelExecute;
FrameStageNotifyFn oFrameStageNotify;
FireEventClientSideFn oFireEventClientSide;

// Hook function prototypes
void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
bool __stdcall Hooked_InPrediction();
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld);
bool __stdcall CreateMoveClient_Hooked(/*void* self, int edx,*/ float frametime, CUserCmd* pCmd);
void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage);
bool __fastcall Hooked_FireEventClientSide(void *ecx, void* edx, IGameEvent* pEvent);

// VMT Managers
namespace Hooks
{
	// VMT Managers
	Utilities::Memory::CVMTHookManager VMTPanel; // Hooking drawing functions
	Utilities::Memory::VMTManager VMTClient; // Maybe CreateMove
	Utilities::Memory::VMTManager VMTClientMode; // CreateMove for functionality
	Utilities::Memory::CVMTHookManager VMTModelRender; // DrawModelEx for chams
	Utilities::Memory::CVMTHookManager VMTPrediction; // InPrediction for no vis recoil
};

// Initialise all our hooks
void Hooks::Initialise()
{
	// Panel hooks for drawing to the screen via surface functions
	VMTPanel.bInitialize((PDWORD*)Interfaces::Panels);
	oPaintTraverse = (PaintTraverse_)VMTPanel.dwHooKmethod((DWORD)PaintTraverse_Hooked, Offsets::VMT::Panel_PaintTraverse);
	VMTPanel.ReHook();

	// No Visual Recoil
	VMTPrediction.bInitialize((PDWORD*)Interfaces::Prediction);
	VMTPrediction.dwHooKmethod((DWORD)Hooked_InPrediction, 14);
	VMTPrediction.ReHook();

	// Chams
	VMTModelRender.bInitialize((PDWORD*)Interfaces::ModelRender);
	oDrawModelExecute = (DrawModelEx_)VMTModelRender.dwHooKmethod((DWORD)Hooked_DrawModelExecute, Offsets::VMT::ModelRender_DrawModelExecute);
	VMTModelRender.ReHook();

	// Setup ClientMode Hooks
	VMTClientMode.Initialise((DWORD*)Interfaces::ClientMode);
	VMTClientMode.HookMethod((DWORD)CreateMoveClient_Hooked, 24);

	VMTClient.Initialise((DWORD*)Interfaces::Client);
	oFrameStageNotify = (FrameStageNotifyFn)VMTClient.HookMethod((DWORD)&Hooked_FrameStageNotify, 36);

	oFireEventClientSide = (FireEventClientSideFn)Utilities::Memory::Hook(Interfaces::GameEventManager, 9, Hooked_FireEventClientSide);

	oOverrideView = (OverrideViewFn)Utilities::Memory::Hook(Interfaces::ClientMode, 18, Hooks::OverrideView);
}

// Undo our hooks
void Hooks::UndoHooks()
{
	VMTPanel.UnHook();
	VMTPrediction.UnHook();
	VMTModelRender.UnHook();
	VMTClientMode.RestoreOriginal();

}

void MovementCorrection(CUserCmd* pCmd)
{

}

/*==========================================================
  _____ _               _   _   _______       _____  _____
 / ____| |        /\   | \ | | |__   __|/\   / ____|/ ____|
| |    | |       /  \  |  \| |    | |  /  \ | |  __| (___
| |    | |      / /\ \ | . ` |    | | / /\ \| | |_ |\___ \
| |____| |____ / ____ \| |\  |    | |/ ____ \ |__| |____) |
 \_____|______/_/    \_\_| \_|    |_/_/    \_\_____|_____/

===========================================================*/


float speeds = Menu::Window.MiscTab.ClanTagSpeed.GetValue() * 10;
int speed = int(speeds + 0.5f);

void SetClanTag(const char* tag, const char* name)//190% paste
{
	static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(((DWORD)Utilities::Memory::FindPattern(XorStr("engine.dll"), (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x30", XorStr("xxxxxxxxx????xxxxxx"))));
	pSetClanTag(tag, name);
}

void NoClantag()
{
	SetClanTag("", "");
}

void messiahclan()
{
	static int counter = 0;

	if (speed <= 0)
	{
		speed = 18;
	}

	if (Menu::Window.MiscTab.ClanTagEnable.GetState())
	{
		if (Menu::Window.MiscTab.ClanTagType.GetIndex() == 0 && Menu::Window.MiscTab.ClanTagChoice.GetIndex() == 0)
		{
			SetClanTag(XorStr("amber"), XorStr("amber"));
		}
		else if (Menu::Window.MiscTab.ClanTagType.GetIndex() == 1 && Menu::Window.MiscTab.ClanTagChoice.GetIndex() == 0)
		{
			static int motion = 0;
			int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 2.5;

			if (counter % speed == 0)
				motion++;
			int value = ServerTime % 10;

			switch (value) {
			case 0:SetClanTag(XorStr("     "), XorStr("amber")); break;
			case 1:SetClanTag(XorStr("    a"), XorStr("amber")); break;
			case 2:SetClanTag(XorStr("   am"), XorStr("amber")); break;
			case 3:SetClanTag(XorStr("  amb"), XorStr("amber")); break;
			case 4:SetClanTag(XorStr(" ambe"), XorStr("amber")); break;
			case 5:SetClanTag(XorStr("amber"), XorStr("amber")); break;
			case 6:SetClanTag(XorStr("mber "), XorStr("amber")); break;
			case 7:SetClanTag(XorStr("ber  "), XorStr("amber")); break;
			case 8:SetClanTag(XorStr("er   "), XorStr("amber")); break;
			case 9:SetClanTag(XorStr("r    "), XorStr("amber")); break;
			}
			counter++;
		}
	}
}

void skeetclan()
{
	static int counter = 0;

	if (speed <= 0)
	{
		speed = 24;
	}

	if (Menu::Window.MiscTab.ClanTagEnable.GetState())
	{
		if (Menu::Window.MiscTab.ClanTagType.GetIndex() == 0 && Menu::Window.MiscTab.ClanTagChoice.GetIndex() == 1)
		{
			SetClanTag(XorStr("no"), XorStr("no"));
		}
		else if (Menu::Window.MiscTab.ClanTagType.GetIndex() == 1 && Menu::Window.MiscTab.ClanTagChoice.GetIndex() == 1)
		{
			static int motion = 0;
			int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 2.5;

			if (counter % speed == 0)
				motion++;

			int value = ServerTime % 17;
			switch (value) {
			case 0:SetClanTag(XorStr("no"), XorStr("no")); break;
			case 1:SetClanTag(XorStr("no"), XorStr("no")); break;
			case 2:SetClanTag(XorStr("no"), XorStr("no")); break;
			case 3:SetClanTag(XorStr("no"), XorStr("no")); break;
			case 4:SetClanTag(XorStr("no"), XorStr("no")); break;
			case 5:SetClanTag(XorStr("no"), XorStr("no")); break;
			case 6:SetClanTag(XorStr("no"), XorStr("no")); break;
			case 7:SetClanTag(XorStr("no"), XorStr("no")); break;
			case 8:SetClanTag(XorStr("no"), XorStr("no")); break;
			case 9:SetClanTag(XorStr("no "), XorStr("no")); break;
			case 10:SetClanTag(XorStr("no"), XorStr("no")); break;
			case 11:SetClanTag(XorStr("no"), XorStr("no")); break;
			case 12:SetClanTag(XorStr("no"), XorStr("no")); break;
			case 13:SetClanTag(XorStr("no"), XorStr("no")); break;
			case 14:SetClanTag(XorStr("no"), XorStr("no")); break;
			case 15:SetClanTag(XorStr("no"), XorStr("no")); break;
			case 16:SetClanTag(XorStr("no"), XorStr("no")); break;
			}
			counter++;
		}
	}
}

void pastedclan()
{
	static int counter = 0;

	if (speed <= 0)
	{
		speed = 18;
	}

	if (Menu::Window.MiscTab.ClanTagEnable.GetState())
	{
		if (Menu::Window.MiscTab.ClanTagType.GetIndex() == 0 && Menu::Window.MiscTab.ClanTagChoice.GetIndex() == 2)
		{
			SetClanTag(XorStr("pasted"), XorStr("pasted"));
		}
		else if (Menu::Window.MiscTab.ClanTagType.GetIndex() == 1 && Menu::Window.MiscTab.ClanTagChoice.GetIndex() == 2)
		{
			static int motion = 0;
			int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 2.5;

			if (counter % speed == 0)
				motion++;
			int value = ServerTime % 12;

			switch (value) {
			case 0:SetClanTag(XorStr("      "), XorStr("pasted")); break;
			case 1:SetClanTag(XorStr("     p"), XorStr("pasted")); break;
			case 2:SetClanTag(XorStr("    pa"), XorStr("pasted")); break;
			case 3:SetClanTag(XorStr("   pas"), XorStr("pasted")); break;
			case 4:SetClanTag(XorStr("  past"), XorStr("pasted")); break;
			case 5:SetClanTag(XorStr(" paste"), XorStr("pasted")); break;
			case 6:SetClanTag(XorStr("pasted"), XorStr("pasted")); break;
			case 7:SetClanTag(XorStr("asted "), XorStr("pasted")); break;
			case 8:SetClanTag(XorStr("sted  "), XorStr("pasted")); break;
			case 9:SetClanTag(XorStr("ted   "), XorStr("pasted")); break;
			case 10:SetClanTag(XorStr("ed   "), XorStr("pasted")); break;
			case 11:SetClanTag(XorStr("d    "), XorStr("pasted")); break;
			}
			counter++;
		}
	}
}

void ayyclan()
{
	static int counter = 0;

	if (speed <= 0)
	{
		speed = 24;
	}

	if (Menu::Window.MiscTab.ClanTagEnable.GetState())
	{
		if (Menu::Window.MiscTab.ClanTagType.GetIndex() == 0 && Menu::Window.MiscTab.ClanTagChoice.GetIndex() == 3)
		{
			SetClanTag(XorStr("ayyware"), XorStr("ayyware"));
		}
		else if (Menu::Window.MiscTab.ClanTagType.GetIndex() == 1 && Menu::Window.MiscTab.ClanTagChoice.GetIndex() == 3)
		{
			static int motion = 0;
			int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 2.5;

			if (counter % speed == 0)
				motion++;
			int value = ServerTime % 14;

			switch (value) {
			case 0:SetClanTag(XorStr("       "), XorStr("ayyware")); break;
			case 1:SetClanTag(XorStr("      a"), XorStr("ayyware")); break;
			case 2:SetClanTag(XorStr("     ay"), XorStr("ayyware")); break;
			case 3:SetClanTag(XorStr("    ayy"), XorStr("ayyware")); break;
			case 4:SetClanTag(XorStr("   ayyw"), XorStr("ayyware")); break;
			case 5:SetClanTag(XorStr("  ayywa"), XorStr("ayyware")); break;
			case 6:SetClanTag(XorStr(" ayywar"), XorStr("ayyware")); break;
			case 7:SetClanTag(XorStr("ayyware"), XorStr("ayyware")); break;
			case 8:SetClanTag(XorStr("yyware "), XorStr("ayyware")); break;
			case 9:SetClanTag(XorStr("yware  "), XorStr("ayyware")); break;
			case 10:SetClanTag(XorStr("ware   "), XorStr("ayyware")); break;
			case 11:SetClanTag(XorStr("are    "), XorStr("ayyware")); break;
			case 12:SetClanTag(XorStr("re     "), XorStr("ayyware")); break;
			case 13:SetClanTag(XorStr("e      "), XorStr("ayyware")); break;
			}
			counter++;
		}
	}
}

void trashHVH()
{
	static int counter = 0;

	if (speed <= 0)
	{
		speed = 32;
	}

	if (Menu::Window.MiscTab.ClanTagEnable.GetState())
	{
		if (Menu::Window.MiscTab.ClanTagType.GetIndex() == 0 && Menu::Window.MiscTab.ClanTagChoice.GetIndex() == 4)
		{
			SetClanTag(XorStr("trash.HVH"), XorStr("trash.HVH"));
		}
		else if (Menu::Window.MiscTab.ClanTagType.GetIndex() == 1 && Menu::Window.MiscTab.ClanTagChoice.GetIndex() == 4)
		{
			static int motion = 0;
			int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 2.5;

			if (counter % speed == 0)
				motion++;
			int value = ServerTime % 18;

			switch (value) {
			case 0:SetClanTag(XorStr("         "), XorStr("trash.HVH")); break;
			case 1:SetClanTag(XorStr("        t"), XorStr("trash.HVH")); break;
			case 2:SetClanTag(XorStr("       tr"), XorStr("trash.HVH")); break;
			case 3:SetClanTag(XorStr("      tra"), XorStr("trash.HVH")); break;
			case 4:SetClanTag(XorStr("     tras"), XorStr("trash.HVH")); break;
			case 5:SetClanTag(XorStr("    trash"), XorStr("trash.HVH")); break;
			case 6:SetClanTag(XorStr("   trash."), XorStr("trash.HVH")); break;
			case 7:SetClanTag(XorStr("  trash.H"), XorStr("trash.HVH")); break;
			case 8:SetClanTag(XorStr(" trash.Hv"), XorStr("trash.HVH")); break;
			case 9:SetClanTag(XorStr("trash.HvH"), XorStr("trash.HVH")); break;
			case 10:SetClanTag(XorStr("rash.HvH "), XorStr("trash.HVH")); break;
			case 11:SetClanTag(XorStr("ash.HvH  "), XorStr("trash.HVH")); break;
			case 12:SetClanTag(XorStr("sh.HvH   "), XorStr("trash.HVH")); break;
			case 13:SetClanTag(XorStr("h.HvH    "), XorStr("trash.HVH")); break;
			case 14:SetClanTag(XorStr(".HvH     "), XorStr("trash.HVH")); break;
			case 15:SetClanTag(XorStr("HvH      "), XorStr("trash.HVH")); break;
			case 16:SetClanTag(XorStr("vH       "), XorStr("trash.HVH")); break;
			case 17:SetClanTag(XorStr("H        "), XorStr("trash.HVH")); break;
			}
			counter++;
		}
	}
}
/*=========================================================*/

//---------------------------------------------------------------------------------------------------------
//                                         Hooked Functions
//---------------------------------------------------------------------------------------------------------

BYTE bMoveData[0x200];

void EnginePrediction(CUserCmd* pCmd, IClientEntity* LocalPlayer)
{
	// find this in ayyware 2.0
}

// InPrediction Hooked Function
bool __stdcall Hooked_InPrediction()
{
	bool result;
	static InPrediction_ origFunc = (InPrediction_)Hooks::VMTPrediction.dwGetMethodAddress(14);
	static DWORD *ecxVal = Interfaces::Prediction;
	result = origFunc(ecxVal);


	// If we are in the right place where the player view is calculated
	// Calculate the change in the view and get rid of it
	if (Menu::Window.VisualsTab.EffectNoVisualRecoil.GetState() && (DWORD)(_ReturnAddress()) == Offsets::Functions::dwCalcPlayerView)
	{
		IClientEntity* pLocalEntity = NULL;

		float* m_LocalViewAngles = NULL;

		__asm
		{
			MOV pLocalEntity, ESI
			MOV m_LocalViewAngles, EBX
		}

		Vector viewPunch = pLocalEntity->localPlayerExclusive()->GetViewPunchAngle();
		Vector aimPunch = pLocalEntity->localPlayerExclusive()->GetAimPunchAngle();

		m_LocalViewAngles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
		m_LocalViewAngles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
		m_LocalViewAngles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));
		return true;
	}

	return result;
}

// DrawModelExec for chams and shit
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{

	Color color;
	float flColor[3] = { 0.f };
	static IMaterial* CoveredLit = CreateMaterial(true);
	static IMaterial* Overwride = CreateMaterial(true, false, false);
	static IMaterial* OpenLit = CreateMaterial(false);
	static IMaterial* CoveredFlat = CreateMaterial(true, false);
	static IMaterial* OpenFlat = CreateMaterial(false, false);
	static IMaterial* Chrome = CreateMaterial(XorStr("$envmap env_cube"));

	const char* ModelName = Interfaces::ModelInfo->GetModelName((model_t*)pInfo.pModel);

	IClientEntity* pModelEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(pInfo.entity_index);
	IClientEntity *local = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	int invis_t_r = Menu::Window.ColorsTab.TInvisChamsR.GetValue();
	int invis_t_g = Menu::Window.ColorsTab.TInvisChamsG.GetValue();
	int invis_t_b = Menu::Window.ColorsTab.TInvisChamsB.GetValue();

	int invis_ct_r = Menu::Window.ColorsTab.CTInvisChamsR.GetValue();
	int invis_ct_g = Menu::Window.ColorsTab.CTInvisChamsG.GetValue();
	int invis_ct_b = Menu::Window.ColorsTab.CTInvisChamsB.GetValue();

	int vis_t_r = Menu::Window.ColorsTab.TVisChamsR.GetValue();
	int vis_t_g = Menu::Window.ColorsTab.TVisChamsG.GetValue();
	int vis_t_b = Menu::Window.ColorsTab.TVisChamsB.GetValue();

	int vis_ct_r = Menu::Window.ColorsTab.CTVisChamsR.GetValue();
	int vis_ct_g = Menu::Window.ColorsTab.CTVisChamsG.GetValue();
	int vis_ct_b = Menu::Window.ColorsTab.CTVisChamsB.GetValue();

	if (Menu::Window.VisualsTab.OptionsChams.GetIndex() == 1 && strstr(ModelName, XorStr("models/player")))
	{
		IClientEntity* pModelEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(pInfo.entity_index);
		IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		if (pLocal && (!Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() || pModelEntity->GetTeamNum() != pLocal->GetTeamNum()))
		{
			IMaterial *covered = 1 ? CoveredLit : CoveredFlat;
			IMaterial *open = 1 ? OpenLit : OpenFlat;
			if (pModelEntity)
			{
				if (local)
				{
					if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0)
					{
						float chams_alpha = 1.0f;

						if (pModelEntity->HasGunGameImmunity())
							chams_alpha = 0.5f;

						if (pModelEntity->GetTeamNum() == 2)
						{
							// Invisible T
							flColor[0] = invis_t_r / 255.f;
							flColor[1] = invis_t_g / 255.f;
							flColor[2] = invis_t_b / 255.f;
						}
						else
						{
							// Invisible CT
							flColor[0] = invis_ct_r / 255.f;
							flColor[1] = invis_ct_g / 255.f;
							flColor[2] = invis_ct_b / 255.f;
						}

						Interfaces::RenderView->SetColorModulation(flColor);
						Interfaces::RenderView->SetBlend(chams_alpha);
						Interfaces::ModelRender->ForcedMaterialOverride(covered);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

						if (pModelEntity->GetTeamNum() == 2)
						{
							// Visible T
							flColor[0] = vis_t_r / 255.f;
							flColor[1] = vis_t_g / 255.f;
							flColor[2] = vis_t_b / 255.f;
						}
						else
						{
							// Visible CT
							flColor[0] = vis_ct_r / 255.f;
							flColor[1] = vis_ct_g / 255.f;
							flColor[2] = vis_ct_b / 255.f;
						}

						Interfaces::RenderView->SetColorModulation(flColor);
						Interfaces::ModelRender->ForcedMaterialOverride(open);
					}
				}
			}
		}
	}

	else if (Menu::Window.VisualsTab.OptionsChams.GetIndex() == 2 && strstr(ModelName, XorStr("models/player")))
	{
		IMaterial *covered = 1 ? CoveredLit : CoveredFlat;
		IMaterial *open = 1 ? OpenLit : OpenFlat;
		IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		static int counter = 0;
		static float colors[3] = { 1.f, 0.f, 0.f };

		if (colors[counter] >= 1.0f)
		{
			colors[counter] = 1.0f;
			counter += 1;
			if (counter > 2)
				counter = 0;
		}
		else
		{
			int prev = counter - 1;
			if (prev < 0) prev = 2;
			colors[prev] -= 0.005f;
			colors[counter] += 0.005f;
		}



		static int counter2 = 0;
		static float colors2[3] = { 1.f, 0.f, 0.f };

		if (colors2[counter2] >= 1.0f)
		{
			colors2[counter2] = 1.0f;
			counter += 1;
			if (counter > 2)
				counter = 0;
		}
		else
		{
			int prev2 = counter2 - 1;
			if (prev2 < 0) prev2 = 2;
			colors2[prev2] -= 0.004f;
			colors2[counter2] += 0.004f;
		}

		if (pLocal && (!Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() ||
			pModelEntity->GetTeamNum() != pLocal->GetTeamNum()))
		{
			if (pModelEntity)
			{
				if (local)
				{
					if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0)
					{
						float chams_alpha = 0.9f;

						if (pModelEntity->HasGunGameImmunity())
							chams_alpha = 0.5f;

						Interfaces::RenderView->SetColorModulation(colors2);
						Interfaces::RenderView->SetBlend(1.0f);
						Interfaces::ModelRender->ForcedMaterialOverride(open);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

						Interfaces::RenderView->SetColorModulation(colors);
						Interfaces::RenderView->SetBlend(chams_alpha);
						Interfaces::ModelRender->ForcedMaterialOverride(covered);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
					}
				}
			}
		}
	}

	else if (Menu::Window.VisualsTab.OptionsChams.GetIndex() == 3 && strstr(ModelName, XorStr("models/player")))
	{
		IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		static IMaterial* Wireframe = CreateMaterial(true, false, true);
		IMaterial *wireframedcovered = 1 ? Wireframe : Wireframe;
		IMaterial *wireframedopen = 1 ? Wireframe : Wireframe;

		if (pLocal && (!Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() ||
			pModelEntity->GetTeamNum() != pLocal->GetTeamNum()))
		{
			if (pModelEntity)
			{
				if (local)
				{
					if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0)
					{
						float chams_alpha = 0.9f;

						if (pModelEntity->HasGunGameImmunity())
							chams_alpha = 0.5f;

						if (pModelEntity->GetTeamNum() == 2)
						{
							// Invisible T
							flColor[0] = invis_t_r / 255.f;
							flColor[1] = invis_t_g / 255.f;
							flColor[2] = invis_t_b / 255.f;
						}
						else
						{
							// Invisible CT
							flColor[0] = invis_ct_r / 255.f;
							flColor[1] = invis_ct_g / 255.f;
							flColor[2] = invis_ct_b / 255.f;
						}

						Interfaces::RenderView->SetColorModulation(flColor);
						Interfaces::RenderView->SetBlend(1.0f);
						Interfaces::ModelRender->ForcedMaterialOverride(wireframedcovered);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

						if (pModelEntity->GetTeamNum() == 2)
						{
							// Visible T
							flColor[0] = vis_t_r / 255.f;
							flColor[1] = vis_t_g / 255.f;
							flColor[2] = vis_t_b / 255.f;
						}
						else
						{
							// Visible CT
							flColor[0] = vis_ct_r / 255.f;
							flColor[1] = vis_ct_g / 255.f;
							flColor[2] = vis_ct_b / 255.f;
						}

						Interfaces::RenderView->SetColorModulation(flColor);
						Interfaces::RenderView->SetBlend(1.0f);
						Interfaces::ModelRender->ForcedMaterialOverride(wireframedopen);
					}
				}
			}
		}
	}

	else if (Menu::Window.VisualsTab.OptionsChams.GetIndex() == 4 && strstr(ModelName, XorStr("models/player")))
	{
		IMaterial *chromedcovered = 1 ? Chrome : Chrome;
		IMaterial *chromedopen = 1 ? Chrome : Chrome;
		IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		if (pLocal && (!Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() ||
			pModelEntity->GetTeamNum() != pLocal->GetTeamNum()))
		{
			if (pModelEntity)
			{
				if (local)
				{
					if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0)
					{
						float chams_alpha = 0.9f;
						if (pModelEntity->HasGunGameImmunity())
							chams_alpha = 0.5f;

						if (pModelEntity->GetTeamNum() == 2)
						{
							// Invisible T
							flColor[0] = invis_t_r / 255.f;
							flColor[1] = invis_t_g / 255.f;
							flColor[2] = invis_t_b / 255.f;
						}
						else
						{
							// Invisible CT
							flColor[0] = invis_ct_r / 255.f;
							flColor[1] = invis_ct_g / 255.f;
							flColor[2] = invis_ct_b / 255.f;
						}

						Interfaces::RenderView->SetColorModulation(flColor);
						Interfaces::RenderView->SetBlend(1.0f);
						Interfaces::ModelRender->ForcedMaterialOverride(chromedcovered);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

						if (pModelEntity->GetTeamNum() == 2)
						{
							// Visible T
							flColor[0] = vis_t_r / 255.f;
							flColor[1] = vis_t_g / 255.f;
							flColor[2] = vis_t_b / 255.f;
						}
						else
						{
							// Visible CT
							flColor[0] = vis_ct_r / 255.f;
							flColor[1] = vis_ct_g / 255.f;
							flColor[2] = vis_ct_b / 255.f;
						}

						Interfaces::RenderView->SetColorModulation(flColor);
						Interfaces::RenderView->SetBlend(chams_alpha);
						Interfaces::ModelRender->ForcedMaterialOverride(chromedopen);
					}
				}
			}
		}
	}



	else if (Menu::Window.VisualsTab.OptionsChams.GetIndex() == 5 && strstr(ModelName, XorStr("models/player")))
	{
		IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		IMaterial *covered = 1 ? CoveredLit : CoveredFlat;
		IMaterial *open = 1 ? OpenLit : OpenFlat;
		if (pLocal && (!Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() ||
			pModelEntity->GetTeamNum() != pLocal->GetTeamNum()))
		{
			if (pModelEntity)
			{
				if (local)
				{
					if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0)
					{
						float chams_alpha = 0.5f;

						if (pModelEntity->GetTeamNum() == 2)
						{
							// Invisible T
							flColor[0] = invis_t_r / 255.f;
							flColor[1] = invis_t_g / 255.f;
							flColor[2] = invis_t_b / 255.f;
						}
						else
						{
							// Invisible CT
							flColor[0] = invis_ct_r / 255.f;
							flColor[1] = invis_ct_g / 255.f;
							flColor[2] = invis_ct_b / 255.f;
						}

						Interfaces::RenderView->SetColorModulation(flColor);
						Interfaces::RenderView->SetBlend(chams_alpha);
						Interfaces::ModelRender->ForcedMaterialOverride(covered);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

						if (pModelEntity->GetTeamNum() == 2)
						{
							// Visible T
							flColor[0] = vis_t_r / 255.f;
							flColor[1] = vis_t_g / 255.f;
							flColor[2] = vis_t_b / 255.f;
						}
						else
						{
							// Visible CT
							flColor[0] = vis_ct_r / 255.f;
							flColor[1] = vis_ct_g / 255.f;
							flColor[2] = vis_ct_b / 255.f;
						}

						Interfaces::RenderView->SetColorModulation(flColor);
						Interfaces::RenderView->SetBlend(chams_alpha);
						Interfaces::ModelRender->ForcedMaterialOverride(open);
					}
				}
			}
		}
	}

	else if (Menu::Window.VisualsTab.OptionsChams.GetIndex() == 6 && strstr(ModelName, XorStr("models/player")))
	{
		IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		IMaterial *covered = 1 ? CoveredFlat : CoveredFlat;
		IMaterial *open = 1 ? OpenFlat : OpenFlat;
		if (pLocal && (!Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() ||
			pModelEntity->GetTeamNum() != pLocal->GetTeamNum()))
		{
			if (pModelEntity)
			{
				if (local)
				{
					if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0)
					{
						float chams_alpha = 1.0f;

						if (pModelEntity->GetTeamNum() == 2)
						{
							// Invisible T
							flColor[0] = invis_t_r / 255.f;
							flColor[1] = invis_t_g / 255.f;
							flColor[2] = invis_t_b / 255.f;
						}
						else
						{
							// Invisible CT
							flColor[0] = invis_ct_r / 255.f;
							flColor[1] = invis_ct_g / 255.f;
							flColor[2] = invis_ct_b / 255.f;
						}

						Interfaces::RenderView->SetColorModulation(flColor);
						Interfaces::RenderView->SetBlend(chams_alpha);
						Interfaces::ModelRender->ForcedMaterialOverride(covered);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

						if (pModelEntity->GetTeamNum() == 2)
						{
							// Visible T
							flColor[0] = vis_t_r / 255.f;
							flColor[1] = vis_t_g / 255.f;
							flColor[2] = vis_t_b / 255.f;
						}
						else
						{
							// Visible CT
							flColor[0] = vis_ct_r / 255.f;
							flColor[1] = vis_ct_g / 255.f;
							flColor[2] = vis_ct_b / 255.f;
						}

						Interfaces::RenderView->SetColorModulation(flColor);
						Interfaces::RenderView->SetBlend(chams_alpha);
						Interfaces::ModelRender->ForcedMaterialOverride(open);
					}
				}
			}
		}
	}

	else if (Menu::Window.VisualsTab.OptionsChams.GetIndex() == 7 && strstr(ModelName, XorStr("models/player")))
	{
		IMaterial *covered = 1 ? CoveredFlat : CoveredFlat;
		IMaterial *open = 1 ? OpenFlat : OpenFlat;
		IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		static int counter = 0;
		static float colors[3] = { 1.f, 0.f, 0.f };

		if (colors[counter] >= 1.0f)
		{
			colors[counter] = 1.0f;
			counter += 1;
			if (counter > 2)
				counter = 0;
		}
		else
		{
			int prev = counter - 1;
			if (prev < 0) prev = 2;
			colors[prev] -= 0.005f;
			colors[counter] += 0.005f;
		}



		static int counter2 = 0;
		static float colors2[3] = { 1.f, 0.f, 0.f };

		if (colors2[counter2] >= 1.0f)
		{
			colors2[counter2] = 1.0f;
			counter += 1;
			if (counter > 2)
				counter = 0;
		}
		else
		{
			int prev2 = counter2 - 1;
			if (prev2 < 0) prev2 = 2;
			colors2[prev2] -= 0.004f;
			colors2[counter2] += 0.004f;
		}

		if (pLocal && (!Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() ||
			pModelEntity->GetTeamNum() != pLocal->GetTeamNum()))
		{
			if (pModelEntity)
			{
				if (local)
				{
					if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0)
					{
						float chams_alpha = 0.9f;

						if (pModelEntity->HasGunGameImmunity())
							chams_alpha = 0.5f;

						Interfaces::RenderView->SetColorModulation(colors2);
						Interfaces::RenderView->SetBlend(1.0f);
						Interfaces::ModelRender->ForcedMaterialOverride(open);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

						Interfaces::RenderView->SetColorModulation(colors);
						Interfaces::RenderView->SetBlend(chams_alpha);
						Interfaces::ModelRender->ForcedMaterialOverride(covered);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
					}
				}
			}
		}
	}

	else if (Menu::Window.VisualsTab.OptionsChams.GetIndex() == 8 && strstr(ModelName, XorStr("models/player")))
	{
		static IMaterial* Wireframe = CreateMaterial(true, false, true);
		IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		static int counter = 0;
		static float colors[3] = { 1.f, 0.f, 0.f };

		if (colors[counter] >= 1.0f)
		{
			colors[counter] = 1.0f;
			counter += 1;
			if (counter > 2)
				counter = 0;
		}
		else
		{
			int prev = counter - 1;
			if (prev < 0) prev = 2;
			colors[prev] -= 0.005f;
			colors[counter] += 0.005f;
		}



		static int counter2 = 0;
		static float colors2[3] = { 1.f, 0.f, 0.f };

		if (colors2[counter2] >= 1.0f)
		{
			colors2[counter2] = 1.0f;
			counter += 1;
			if (counter > 2)
				counter = 0;
		}
		else
		{
			int prev2 = counter2 - 1;
			if (prev2 < 0) prev2 = 2;
			colors2[prev2] -= 0.004f;
			colors2[counter2] += 0.004f;
		}

		if (pLocal && (!Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() ||
			pModelEntity->GetTeamNum() != pLocal->GetTeamNum()))
		{
			if (pModelEntity)
			{
				if (local)
				{
					if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0)
					{
						float chams_alpha = 0.9f;

						if (pModelEntity->HasGunGameImmunity())
							chams_alpha = 0.5f;

						Interfaces::RenderView->SetColorModulation(colors2);
						Interfaces::RenderView->SetBlend(1.0f);
						Interfaces::ModelRender->ForcedMaterialOverride(Wireframe);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

						Interfaces::RenderView->SetColorModulation(colors);
						Interfaces::RenderView->SetBlend(chams_alpha);
						Interfaces::ModelRender->ForcedMaterialOverride(Wireframe);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
					}
				}
			}
		}
	}

	else if (Menu::Window.VisualsTab.EffectNoFlash.GetState() && strstr(ModelName, XorStr("flash")))
	{
		IMaterial* flash = Interfaces::MaterialSystem->FindMaterial(charenc("effects\\flashbang"), charenc(TEXTURE_GROUP_CLIENT_EFFECTS));
		IMaterial* flashWhite = Interfaces::MaterialSystem->FindMaterial(charenc("effects\\flashbang_white"), charenc(TEXTURE_GROUP_CLIENT_EFFECTS));
		flash->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
		flashWhite->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
		Interfaces::ModelRender->ForcedMaterialOverride(flash);
		Interfaces::ModelRender->ForcedMaterialOverride(flashWhite);
	}

	if (Menu::Window.VisualsTab.OtherHandFX.GetIndex() == 1 && strstr(ModelName, XorStr("arms")))
	{
		IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		static IMaterial* Wireframe = CreateMaterial(true, false, true);
		IMaterial *wireframedcovered = 1 ? Wireframe : Wireframe;
		IMaterial *wireframedopen = 1 ? Wireframe : Wireframe;
		if (pLocal)
		{
			if (pLocal->IsAlive())
			{
				int alpha = pLocal->HasGunGameImmunity() ? 150 : 255;

				if (pLocal->GetTeamNum() == 2)
					color.SetColor(255, 0, 127, alpha);
				else
					color.SetColor(255, 0, 127, alpha);

				ForceMaterial(color, wireframedcovered);
				oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

				if (pLocal->GetTeamNum() == 2)
					color.SetColor(255, 0, 127, alpha);
				else
					color.SetColor(255, 0, 127, alpha);
			}
			else
			{
				color.SetColor(0, 255, 0, 255);
			}

			ForceMaterial(color, wireframedopen);
		}
	}

	if (Menu::Window.VisualsTab.OtherHandFX.GetIndex() == 2 && strstr(ModelName, XorStr("arms")))
	{
		IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		IMaterial *covered = 1 ? CoveredLit : CoveredFlat;
		IMaterial *open = 1 ? OpenLit : OpenFlat;
		if (pLocal)
		{
			if (pLocal->IsAlive())
			{
				int alpha = pLocal->HasGunGameImmunity() ? 150 : 255;

				if (pLocal->GetTeamNum() == 2)
					color.SetColor(255, 0, 127, alpha);
				else
					color.SetColor(255, 0, 127, alpha);

				ForceMaterial(color, covered);
				oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

				if (pLocal->GetTeamNum() == 2)
					color.SetColor(255, 0, 127, alpha);
				else
					color.SetColor(255, 0, 127, alpha);
			}
			else
			{
				color.SetColor(0, 255, 0, 255);
			}

			ForceMaterial(color, open);
		}
	}

	if (Menu::Window.VisualsTab.OtherHandFX.GetIndex() == 3 && strstr(ModelName, XorStr("arms")))
	{
		IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		IMaterial *covered = 1 ? CoveredLit : CoveredFlat;
		IMaterial *open = 1 ? OpenLit : OpenFlat;
		if (pLocal)
		{
			if (pLocal->IsAlive())
			{
				int alpha = pLocal->HasGunGameImmunity() ? 150 : 255;

				color.SetColor(0, 0, 0, alpha);

				ForceMaterial(color, covered);
				oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

				color.SetColor(0, 0, 0, alpha);
			}
			else
			{
				color.SetColor(0, 255, 0, 255);
			}

			ForceMaterial(color, open);
		}
	}

	if (Menu::Window.VisualsTab.OtherHandFX.GetIndex() == 4 && strstr(ModelName, XorStr("arms")))
	{
		IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		static IMaterial* Wireframe = CreateMaterial(true, false, true);
		IMaterial *wireframedcovered = 1 ? Wireframe : Wireframe;
		IMaterial *wireframedopen = 1 ? Wireframe : Wireframe;
		if (pLocal)
		{
			if (pLocal->IsAlive())
			{
				int alpha = pLocal->HasGunGameImmunity() ? 150 : 255;

				if (pLocal->GetTeamNum() == 2)
					color.SetColor(0, 0, 0, alpha);
				else
					color.SetColor(0, 0, 0, alpha);

				ForceMaterial(color, wireframedcovered);
				oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

				if (pLocal->GetTeamNum() == 2)
					color.SetColor(0, 0, 0, alpha);
				else
					color.SetColor(0, 0, 0, alpha);
			}
			else
			{
				color.SetColor(0, 255, 0, 255);
			}

			ForceMaterial(color, wireframedopen);
		}
	}

	if (Menu::Window.VisualsTab.OtherHandFX.GetIndex() == 5 && strstr(ModelName, XorStr("arms")))
	{
		Interfaces::RenderView->SetBlend(0.5f);
	}

	if (Menu::Window.VisualsTab.OtherHandFX.GetIndex() == 6 && strstr(ModelName, XorStr("arms")))
	{
		static int counter = 0;
		static float colors[3] = { 1.f, 0.f, 0.f };

		if (colors[counter] >= 1.0f)
		{
			colors[counter] = 1.0f;
			counter += 1;
			if (counter > 2)
				counter = 0;
		}
		else
		{
			int prev = counter - 1;
			if (prev < 0) prev = 2;
			colors[prev] -= 0.004f;
			colors[counter] += 0.004f;
		}

		Interfaces::RenderView->SetColorModulation(colors);
		Interfaces::RenderView->SetBlend(1.0f);
		Interfaces::ModelRender->ForcedMaterialOverride(OpenLit);
	}

	if (Menu::Window.VisualsTab.OtherHandFX.GetIndex() == 7 && strstr(ModelName, XorStr("arms")))
	{
		static IMaterial* Wireframe = CreateMaterial(true, false, true);

		static int counter = 0;
		static float colors[3] = { 1.f, 0.f, 0.f };

		if (colors[counter] >= 1.0f)
		{
			colors[counter] = 1.0f;
			counter += 1;
			if (counter > 2)
				counter = 0;
		}
		else
		{
			int prev = counter - 1;
			if (prev < 0) prev = 2;
			colors[prev] -= 0.004f;
			colors[counter] += 0.004f;
		}

		Interfaces::RenderView->SetColorModulation(colors);
		Interfaces::RenderView->SetBlend(1.0f);
		Interfaces::ModelRender->ForcedMaterialOverride(Wireframe);
	}

	if (Menu::Window.VisualsTab.OtherWeaponFX.GetIndex() == 1 && strstr(ModelName, XorStr("weapon")))
	{
		if (!strstr(ModelName, XorStr("arms")))
		{
			IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
			static IMaterial* Wireframe = CreateMaterial(true, false, true);
			IMaterial *wireframedcovered = 1 ? Wireframe : Wireframe;
			IMaterial *wireframedopen = 1 ? Wireframe : Wireframe;
			if (pLocal)
			{
				if (pLocal->IsAlive())
				{
					int alpha = pLocal->HasGunGameImmunity() ? 150 : 255;

					if (pLocal->GetTeamNum() == 2)
						color.SetColor(255, 0, 127, alpha);
					else
						color.SetColor(255, 0, 127, alpha);

					ForceMaterial(color, wireframedcovered);
					oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

					if (pLocal->GetTeamNum() == 2)
						color.SetColor(255, 0, 127, alpha);
					else
						color.SetColor(255, 0, 127, alpha);
				}
				else
				{
					color.SetColor(0, 255, 0, 255);
				}

				ForceMaterial(color, wireframedopen);
			}
		}
	}

	if (Menu::Window.VisualsTab.OtherWeaponFX.GetIndex() == 2 && strstr(ModelName, XorStr("weapon")))
	{
		if (!strstr(ModelName, XorStr("arms")))
		{
			IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
			IMaterial *covered = 1 ? CoveredLit : CoveredFlat;
			IMaterial *open = 1 ? OpenLit : OpenFlat;
			if (pLocal)
			{
				if (pLocal->IsAlive())
				{
					int alpha = pLocal->HasGunGameImmunity() ? 150 : 255;

					if (pLocal->GetTeamNum() == 2)
						color.SetColor(255, 0, 127, alpha);
					else
						color.SetColor(255, 0, 127, alpha);

					ForceMaterial(color, covered);
					oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

					if (pLocal->GetTeamNum() == 2)
						color.SetColor(255, 0, 127, alpha);
					else
						color.SetColor(255, 0, 127, alpha);
				}
				else
				{
					color.SetColor(0, 255, 0, 255);
				}

				ForceMaterial(color, open);
			}
		}
	}

	if (Menu::Window.VisualsTab.OtherWeaponFX.GetIndex() == 3 && strstr(ModelName, XorStr("weapon")))
	{
		if (!strstr(ModelName, XorStr("arms")))
		{
			IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
			IMaterial *covered = 1 ? CoveredLit : CoveredFlat;
			IMaterial *open = 1 ? OpenLit : OpenFlat;
			if (pLocal)
			{
				if (pLocal->IsAlive())
				{
					int alpha = pLocal->HasGunGameImmunity() ? 150 : 255;

					color.SetColor(0, 0, 0, alpha);

					ForceMaterial(color, covered);
					oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

					color.SetColor(0, 0, 0, alpha);
				}
				else
				{
					color.SetColor(0, 255, 0, 255);
				}

				ForceMaterial(color, open);
			}
		}
	}

	if (Menu::Window.VisualsTab.OtherWeaponFX.GetIndex() == 4 && strstr(ModelName, XorStr("weapon")))
	{
		if (!strstr(ModelName, XorStr("arms")))
		{
			IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
			static IMaterial* Wireframe = CreateMaterial(true, false, true);
			IMaterial *wireframedcovered = 1 ? Wireframe : Wireframe;
			IMaterial *wireframedopen = 1 ? Wireframe : Wireframe;
			if (pLocal)
			{
				if (pLocal->IsAlive())
				{
					int alpha = pLocal->HasGunGameImmunity() ? 150 : 255;

					if (pLocal->GetTeamNum() == 2)
						color.SetColor(0, 0, 0, alpha);
					else
						color.SetColor(0, 0, 0, alpha);

					ForceMaterial(color, wireframedcovered);
					oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

					if (pLocal->GetTeamNum() == 2)
						color.SetColor(0, 0, 0, alpha);
					else
						color.SetColor(0, 0, 0, alpha);
				}
				else
				{
					color.SetColor(0, 255, 0, 255);
				}

				ForceMaterial(color, wireframedopen);
			}
		}
	}

	if (Menu::Window.VisualsTab.OtherWeaponFX.GetIndex() == 5 && strstr(ModelName, XorStr("weapon")))
	{
		if (!strstr(ModelName, XorStr("arms")))
		{
			Interfaces::RenderView->SetBlend(0.5f);
		}
	}

	if (Menu::Window.VisualsTab.OtherWeaponFX.GetIndex() == 6 && strstr(ModelName, XorStr("weapon")))
	{
		if (!strstr(ModelName, XorStr("arms")))
		{
			static IMaterial* Wireframe = CreateMaterial(true, false, true);

			static int counter = 0;
			static float colors[3] = { 1.f, 0.f, 0.f };

			if (colors[counter] >= 1.0f)
			{
				colors[counter] = 1.0f;
				counter += 1;
				if (counter > 2)
					counter = 0;
			}
			else
			{
				int prev = counter - 1;
				if (prev < 0) prev = 2;
				colors[prev] -= 0.004f;
				colors[counter] += 0.004f;
			}

			Interfaces::RenderView->SetColorModulation(colors);
			Interfaces::RenderView->SetBlend(1.0f);
			Interfaces::ModelRender->ForcedMaterialOverride(OpenLit);
		}
	}

	if (Menu::Window.VisualsTab.OtherWeaponFX.GetIndex() == 7 && strstr(ModelName, XorStr("weapon")))
	{
		if (!strstr(ModelName, XorStr("arms")))
		{
			static IMaterial* Wireframe = CreateMaterial(true, false, true);

			static int counter = 0;
			static float colors[3] = { 1.f, 0.f, 0.f };

			if (colors[counter] >= 1.0f)
			{
				colors[counter] = 1.0f;
				counter += 1;
				if (counter > 2)
					counter = 0;
			}
			else
			{
				int prev = counter - 1;
				if (prev < 0) prev = 2;
				colors[prev] -= 0.004f;
				colors[counter] += 0.004f;
			}

			Interfaces::RenderView->SetColorModulation(colors);
			Interfaces::RenderView->SetBlend(1.0f);
			Interfaces::ModelRender->ForcedMaterialOverride(Wireframe);
		}
	}

	bool DontDraw = false;

	if (!DontDraw)
		oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	Interfaces::ModelRender->ForcedMaterialOverride(NULL);

}

/*==========================================================================
 __  __          _____    ______ ______ ______ ______ _____ _______ _____
|  \/  |   /\   |  __ \  |  ____|  ____|  ____|  ____/ ____|__   __/ ____|
| \  / |  /  \  | |__) | | |__  | |__  | |__  | |__ | |       | | | (___
| |\/| | / /\ \ |  ___/  |  __| |  __| |  __| |  __|| |       | |  \___ \
| |  | |/ ____ \| |      | |____| |    | |    | |___| |____   | |  ____) |
|_|  |_/_/    \_\_|      |______|_|    |_|    |______\_____|  |_| |_____/

===========================================================================*/

void DawnMode()
{
	// u can do this urself too
}

/*=========================================================================*/

void StartLagComp(IClientEntity* pEntity, CUserCmd* pCmd)
{
	float flSimTime = *pEntity->GetSimTime();
	pCmd->tick_count = TIME_TO_TICKS(flSimTime + 0.031f);
}

long GetEpochTime()
{
	auto duration = std::chrono::system_clock::now().time_since_epoch();

	return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

void pAngleVectors(const QAngle &angles, Vector& forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float sp, sy, cp, cy;

	SinCos(DEG2RAD(angles[G::UserCmd->viewangles.y]), &sy, &cy);
	SinCos(DEG2RAD(angles[G::UserCmd->viewangles.x]), &sp, &cp);

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

static float StoredLBY = 0.0f;
static bool bLowerBodyIsUpdated;

// ClientMode CreateMove
bool __stdcall CreateMoveClient_Hooked(/*void* self, int edx,*/ float frametime, CUserCmd* pCmd)
{
	bool CircleStrafe = GetAsyncKeyState(VK_XBUTTON1) & 0x8000;
	bool Trigger = GetAsyncKeyState(VK_RBUTTON) & 0x8000;

	if (!pCmd->command_number)
		return true;

	if (Interfaces::Engine->IsConnected() || Interfaces::Engine->IsInGame())
	{
		messiahclan();
		skeetclan();
		ayyclan();
		pastedclan();
		trashHVH();
		//clantagTIME();

		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		if (pCmd->viewangles.y - pLocal->GetLowerBodyYaw() >= -35 && pCmd->viewangles.y - pLocal->GetLowerBodyYaw() <= 35)
			bLowerBodyIsUpdated = true;
		else
			bLowerBodyIsUpdated = false;

		PVOID pebp;

		__asm mov pebp, ebp;

		bool* pbSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);
		bool& bSendPacket = *pbSendPacket;

		if (GetKeyState(VK_NUMPAD2) && !(pCmd->buttons & IN_ATTACK))
		{
			static int Ticks = 0;

			if ((Ticks < 6))
				bSendPacket = false;

			if (Ticks == (6 * 2))
			{
				bSendPacket = true;

				Ticks = 0;
			}

			Ticks++;
		}

		G::UserCmd = pCmd;

		// Backup for safety
		Vector origView = pCmd->viewangles;
		Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
		Vector qAimAngles;
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);

		// Do da hacks
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && pLocal && pLocal->IsAlive()) {
			EnginePrediction(pCmd, pLocal);
			Hacks::MoveHacks(pCmd, bSendPacket);
		}

		//Movement Fix
		//GameUtils::CL_FixMove(pCmd, origView);
		qAimAngles.Init(0.0f, GetAutostrafeView().y, 0.0f);
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &aimforward, &aimright, &aimup);
		Vector vForwardNorm;		Normalize(viewforward, vForwardNorm);
		Vector vRightNorm;			Normalize(viewright, vRightNorm);
		Vector vUpNorm;				Normalize(viewup, vUpNorm);

		if (Menu::Window.VisualsTab.EffectMap.GetIndex() != 0)
			DawnMode();

		// Original shit for movement correction
		float forward = pCmd->forwardmove;
		float right = pCmd->sidemove;
		float up = pCmd->upmove;
		if (forward > 450) forward = 450;
		if (right > 450) right = 450;
		if (up > 450) up = 450;
		if (forward < -450) forward = -450;
		if (right < -450) right = -450;
		if (up < -450) up = -450;
		pCmd->forwardmove = DotProduct(forward * vForwardNorm, aimforward) + DotProduct(right * vRightNorm, aimforward) + DotProduct(up * vUpNorm, aimforward);
		pCmd->sidemove = DotProduct(forward * vForwardNorm, aimright) + DotProduct(right * vRightNorm, aimright) + DotProduct(up * vUpNorm, aimright);
		pCmd->upmove = DotProduct(forward * vForwardNorm, aimup) + DotProduct(right * vRightNorm, aimup) + DotProduct(up * vUpNorm, aimup);

		// Angle normalisation
		if (Menu::Window.MiscTab.OtherSafeMode.GetState())
		{
			GameUtils::NormaliseViewAngle(pCmd->viewangles);

			if (pCmd->viewangles.z != 0.0f)
			{
				pCmd->viewangles.z = 0.00;
			}

			if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
			{
				////Utilities::Log(XorStr("Having to re-normalise!"));
				GameUtils::NormaliseViewAngle(pCmd->viewangles);
				Beep(750, 800);
				if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
				{
					pCmd->viewangles = origView;
					pCmd->sidemove = right;
					pCmd->forwardmove = forward;
				}
			}
		
			//put LBY shit here. not providing paste tho lol
		}

		G::LastAngle = pCmd->viewangles;

		if (Menu::Window.VisualsTab.EffectThirdPerson.GetIndex() == 1)
		{
			if (bSendPacket)
				G::qLastTickAngle = pCmd->viewangles;
		}
		else if (Menu::Window.VisualsTab.EffectThirdPerson.GetIndex() == 2)
		{
			if (!pCmd->buttons &IN_ATTACK)
			{
				if (!bSendPacket)
					G::qLastTickAngle = pCmd->viewangles;
			}
		}

		//StepResolver::change = true;

	}

	return false;
}

// Paint Traverse Hooked function
void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	static unsigned int FocusOverlayPanel = 0;
	static bool FoundPanel = false;

	IClientEntity *pLocal = hackManager.pLocal();
	//CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (Menu::Window.VisualsTab.EffectNoScope.GetState() && pLocal->IsScoped() && pLocal->IsAlive() && !strcmp(XorStr("HudZoom"), Interfaces::Panels->GetName(vguiPanel)))
	{
		return;
	}

	oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);

	if (!FoundPanel)
	{
		PCHAR szPanelName = (PCHAR)Interfaces::Panels->GetName(vguiPanel);
		if (strstr(szPanelName, XorStr("MatSystemTopPanel")))
		{
			FocusOverlayPanel = vguiPanel;
			FoundPanel = true;
		}
	}
	else if (FocusOverlayPanel == vguiPanel)
	{
		if (Menu::Window.VisualsTab.EffectNoScope.GetState() && pLocal->IsScoped() && pLocal->IsAlive() && Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) {
			RECT View = Render::GetViewport();
			int MidX = View.right / 2;
			int MidY = View.bottom / 2;
			Render::Line(MidX, MidY - 3000, MidX, MidY + 3000, Color(0, 0, 0, 255));
			Render::Line(MidX - 3000, MidY, MidX + 3000, MidY, Color(0, 0, 0, 255));
		}

		if (Menu::Window.VisualsTab.OtherWatermark.GetState())
		{
			Render::Text(10, 10, Color(255, 194, 0, 220), Render::Fonts::Menu, XorStr("free source"));
			Render::Text(10, 22, Color(255, 255, 255, 220), Render::Fonts::Menu, XorStr("by axio :)"));
		}

		if (Menu::Window.RageBotTab.AntiAimLBY.GetState())
		{
			RECT scrn = Render::GetViewport();
			RECT TextSize = Render::GetTextSize(Render::Fonts::BITWare, XorStr("LBY"));

			if (pLocal->IsAlive() && Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected()) {
				if (pLocal->GetVelocity().Length2D() > 0 && G::LBYUpdate) {
					Render::Text(10, scrn.bottom - 104.5, Color(255, 0, 0, 255), Render::Fonts::BITWare, XorStr("LBY"));
				}
				else {
					Render::Text(10, scrn.bottom - 104.5, Color(0, 255, 0, 255), Render::Fonts::BITWare, XorStr("LBY"));
				}
			}
		}

		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) 
		{
			Hacks::DrawHacks();
		}

		// Update and draw the menu
		Menu::DoUIFrame();
	}
}

int RandomInt(int min, int max)
{
	return rand() % max + min;
}

bool bGlovesNeedUpdate;
void ApplyCustomGloves(IClientEntity* pLocal)
{

	if (bGlovesNeedUpdate || !pLocal->IsAlive())
	{
		DWORD* hMyWearables = (DWORD*)((size_t)pLocal + 0x2EF4);

		if (!Interfaces::EntList->GetClientEntity(hMyWearables[0] & 0xFFF))
		{
			for (ClientClass* pClass = Interfaces::Client->GetAllClasses(); pClass; pClass = pClass->m_pNext)
			{
				if (pClass->m_ClassID != (int)CSGOClassID::CEconWearable)
					continue;

				int iEntry = (Interfaces::EntList->GetHighestEntityIndex() + 1);
				int	iSerial = RandomInt(0x0, 0xFFF);

				pClass->m_pCreateFn(iEntry, iSerial);
				hMyWearables[0] = iEntry | (iSerial << 16);

				break;
			}
		}

		player_info_t LocalPlayerInfo;
		Interfaces::Engine->GetPlayerInfo(Interfaces::Engine->GetLocalPlayer(), &LocalPlayerInfo);

		CBaseCombatWeapon* glovestochange = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntity(hMyWearables[0] & 0xFFF);

		if (!glovestochange)
			return;


		switch (Menu::Window.MiscTab.GloveModel.GetIndex())
		{
		case 1:
		{
			*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5027;
			glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex(XorStr("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl")));
			//*glovestochange->ViewModelIndex() = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl");
			//*glovestochange->WorldModelIndex() = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl") + 1;
			break;
		}
		case 2:
		{
			*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5032;
			glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex(XorStr("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl")));
			break;
		}
		case 3:
		{
			*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5031;
			glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex(XorStr("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl")));
			break;
		}
		case 4:
		{
			*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5030;
			glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex(XorStr("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl")));
			break;
		}
		case 5:
		{
			*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5033;
			glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex(XorStr("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl")));
			break;
		}
		case 6:
		{
			*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5034;
			glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex(XorStr("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl")));
			break;
		}
		default:
			break;
		}

		switch (Menu::Window.MiscTab.GloveSkin.GetIndex())
		{
		case 0:
			break;
		case 1:
			*glovestochange->FallbackPaintKit() = 10007;
			break;
		case 2:
			*glovestochange->FallbackPaintKit() = 10006;
			break;
		case 3:
			*glovestochange->FallbackPaintKit() = 10039;
			break;
		case 4:
			*glovestochange->FallbackPaintKit() = 10008;
			break;
		case 5:
			*glovestochange->FallbackPaintKit() = 10021;
			break;
		case 6:
			*glovestochange->FallbackPaintKit() = 10036;
			break;
		case 7:
			*glovestochange->FallbackPaintKit() = 10009;
			break;
		case 8:
			*glovestochange->FallbackPaintKit() = 10010;
			break;
		case 9:
			*glovestochange->FallbackPaintKit() = 10016;
			break;
		case 10:
			*glovestochange->FallbackPaintKit() = 10013;
			break;
		case 11:
			*glovestochange->FallbackPaintKit() = 10040;
			break;
		case 12:
			*glovestochange->FallbackPaintKit() = 10015;
			break;
		case 13:
			*glovestochange->FallbackPaintKit() = 10037;
			break;
		case 14:
			*glovestochange->FallbackPaintKit() = 10038;
			break;
		case 15:
			*glovestochange->FallbackPaintKit() = 10018;
			break;
		case 16:
			*glovestochange->FallbackPaintKit() = 10019;
			break;
		case 17:
			*glovestochange->FallbackPaintKit() = 10026;
			break;
		case 18:
			*glovestochange->FallbackPaintKit() = 10028;
			break;
		case 19:
			*glovestochange->FallbackPaintKit() = 10027;
			break;
		case 20:
			*glovestochange->FallbackPaintKit() = 10024;
			break;
		case 21:
			*glovestochange->FallbackPaintKit() = 10033;
			break;
		case 22:
			*glovestochange->FallbackPaintKit() = 10034;
			break;
		case 23:
			*glovestochange->FallbackPaintKit() = 10035;
			break;
		case 24:
			*glovestochange->FallbackPaintKit() = 10030;
			break;
		}
		//*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = whatever item definition index you want;
		//glovestochange->SetModelIndex(desired model index e.g.GLOVE_BLOOD in your case);
		//*glovestochange->FallbackPaintKit() = 10008;
		*glovestochange->m_AttributeManager()->m_Item()->ItemIDHigh() = -1; //you need to set this to apply the custom stuff you're setting (this is probably your issue)
		*glovestochange->FallbackWear() = 0.001f;
		*glovestochange->m_AttributeManager()->m_Item()->AccountID() = LocalPlayerInfo.m_nXuidLow;


		glovestochange->PreDataUpdate(0);
		bGlovesNeedUpdate = false;
	}
}

// Hooked FSN for Resolving
void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage)
{
	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && curStage == FRAME_RENDER_START)
	{
		static auto mat_drawgray = Interfaces::CVar->FindVar(XorStr("mat_drawgray"));
		mat_drawgray->nFlags &= FCVAR_CHEAT;

		if (Menu::Window.VisualsTab.EffectMap.GetIndex() == 3) 
		{
			mat_drawgray->SetValue(1);
		}
		else 
		{
			mat_drawgray->SetValue(0);
		}

		if (Menu::Window.RageBotTab.AimbotPVSFix.GetState())
		{
			for (int i = 1; i < Interfaces::Globals->maxClients; i++)
			{
				if (i == Interfaces::Engine->GetLocalPlayer()) continue;
				IClientEntity* pCurEntity = Interfaces::EntList->GetClientEntity(i);
				if (!pCurEntity) continue;
				*(int*)((uintptr_t)pCurEntity + 0xA30) = Interfaces::Globals->framecount; //we'll skip occlusion checks now
				*(int*)((uintptr_t)pCurEntity + 0xA28) = 0;//clear occlusion flags
			}
		}


		if (Menu::Window.VisualsTab.EffectThirdPerson.GetIndex() != 0)
		{
			if (hackManager.pLocal() && hackManager.pLocal()->IsAlive())
			{
				*(QAngle*)((DWORD)hackManager.pLocal() + 0x31C8) = QAngle(G::qLastTickAngle.x, G::qLastTickAngle.y, G::qLastTickAngle.z);
			}
		}
	}

	//Resolver & Skins
	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		//380

		//Resolver
		if (Menu::Window.RageBotTab.LBYFix.GetState())
			R::LowerBodyFix();

		if (Menu::Window.RageBotTab.ResolverON.GetState())
			R::Resolve();

		float flTargetTime;

		for (int i = 0; i <= Interfaces::EntList->GetHighestEntityIndex(); i++) // CHANGE
		{
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

			if (pEntity)
			{
				if (pLocal->IsAlive())
				{
					ApplyCustomGloves(pLocal);
				}
			}
		}

		//SKINS
		int iBayonet = Interfaces::ModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_bayonet.mdl"));
		int iButterfly = Interfaces::ModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_butterfly.mdl"));
		int iFlip = Interfaces::ModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_flip.mdl"));
		int iGut = Interfaces::ModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_gut.mdl"));
		int iKarambit = Interfaces::ModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_karam.mdl"));
		int iM9Bayonet = Interfaces::ModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_m9_bay.mdl"));
		int iHuntsman = Interfaces::ModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_tactical.mdl"));
		int iFalchion = Interfaces::ModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_falchion_advanced.mdl"));
		int iDagger = Interfaces::ModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_push.mdl"));
		int iBowie = Interfaces::ModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_survival_bowie.mdl"));
		int iGunGame = Interfaces::ModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_gg.mdl"));

		for (int i = 0; i <= Interfaces::EntList->GetHighestEntityIndex(); i++) // CHANGE
		{
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

			if (pEntity)
			{
				ULONG hOwnerEntity = *(PULONG)((DWORD)pEntity + 0x148);

				IClientEntity* pOwner = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)hOwnerEntity);

				if (pOwner)
				{
					if (pOwner == pLocal)
					{
						std::string sWeapon = Interfaces::ModelInfo->GetModelName(pEntity->GetModel());

						if (!(sWeapon.find(XorStr("models/weapons"), 0) != std::string::npos))
							continue;

						if (sWeapon.find(XorStr("c4_planted"), 0) != std::string::npos)
							continue;

						if (sWeapon.find(XorStr("thrown"), 0) != std::string::npos)
							continue;

						if (sWeapon.find(XorStr("smokegrenade"), 0) != std::string::npos)
							continue;

						if (sWeapon.find(XorStr("flashbang"), 0) != std::string::npos)
							continue;

						if (sWeapon.find(XorStr("fraggrenade"), 0) != std::string::npos)
							continue;

						if (sWeapon.find(XorStr("molotov"), 0) != std::string::npos)
							continue;

						if (sWeapon.find(XorStr("decoy"), 0) != std::string::npos)
							continue;

						if (sWeapon.find(XorStr("incendiarygrenade"), 0) != std::string::npos)
							continue;

						if (sWeapon.find(XorStr("ied"), 0) != std::string::npos)
							continue;

						if (sWeapon.find(XorStr("w_eq_"), 0) != std::string::npos)
							continue;

						CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)pEntity;

						ClientClass *pClass = Interfaces::Client->GetAllClasses();

						if (Menu::Window.MiscTab.KnifeModel.GetIndex() > 0)
						{
							int Model = Menu::Window.MiscTab.KnifeModel.GetIndex();

							if (pEntity->GetClientClass()->m_ClassID == (int)CSGOClassID::CKnife)
							{
								if (Model == 1) // Bayonet
								{
									*pWeapon->ModelIndex() = iBayonet; // m_nModelIndex
									*pWeapon->ViewModelIndex() = iBayonet;
									*pWeapon->WorldModelIndex() = iBayonet + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 500;

									*pWeapon->FallbackPaintKit() = 416; //doppler sapphire
									*pWeapon->FallbackWear() = 0.000000000001;
									*pWeapon->FallbackSeed() = 852;
								}
								else if (Model == 2) // Bowie Knife
								{
									*pWeapon->ModelIndex() = iBowie; // m_nModelIndex
									*pWeapon->ViewModelIndex() = iBowie;
									*pWeapon->WorldModelIndex() = iBowie + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 514;

									*pWeapon->FallbackPaintKit() = 416; //doppler sapphire
									*pWeapon->FallbackWear() = 0.000000000001;
									*pWeapon->FallbackSeed() = 852;
								}
								else if (Model == 3) // Butterfly Knife
								{
									*pWeapon->ModelIndex() = iButterfly; // m_nModelIndex
									*pWeapon->ViewModelIndex() = iButterfly;
									*pWeapon->WorldModelIndex() = iButterfly + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 515;

									*pWeapon->FallbackPaintKit() = 416; //doppler sapphire
									*pWeapon->FallbackWear() = 0.000000000001;
									*pWeapon->FallbackSeed() = 852;
								}
								else if (Model == 4) // Falchion Knife
								{
									*pWeapon->ModelIndex() = iFalchion; // m_nModelIndex
									*pWeapon->ViewModelIndex() = iFalchion;
									*pWeapon->WorldModelIndex() = iFalchion + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 512;

									*pWeapon->FallbackPaintKit() = 416; //doppler sapphire
									*pWeapon->FallbackWear() = 0.000000000001;
									*pWeapon->FallbackSeed() = 852;
								}
								else if (Model == 5) // Flip Knife
								{
									*pWeapon->ModelIndex() = iFlip; // m_nModelIndex
									*pWeapon->ViewModelIndex() = iFlip;
									*pWeapon->WorldModelIndex() = iFlip + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 505;

									*pWeapon->FallbackPaintKit() = 416; //doppler sapphire
									*pWeapon->FallbackWear() = 0.000000000001;
									*pWeapon->FallbackSeed() = 852;
								}
								else if (Model == 6) // Gut Knife
								{
									*pWeapon->ModelIndex() = iGut; // m_nModelIndex
									*pWeapon->ViewModelIndex() = iGut;
									*pWeapon->WorldModelIndex() = iGut + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 506;

									*pWeapon->FallbackPaintKit() = 416; //doppler sapphire
									*pWeapon->FallbackWear() = 0.000000000001;
									*pWeapon->FallbackSeed() = 852;
								}
								else if (Model == 7) // Huntsman
								{
									*pWeapon->ModelIndex() = iHuntsman; // m_nModelIndex
									*pWeapon->ViewModelIndex() = iHuntsman;
									*pWeapon->WorldModelIndex() = iHuntsman + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 509;

									*pWeapon->FallbackPaintKit() = 0;
								}
								else if (Model == 8) // Karambit
								{
									*pWeapon->ModelIndex() = iKarambit; // m_nModelIndex
									*pWeapon->ViewModelIndex() = iKarambit;
									*pWeapon->WorldModelIndex() = iKarambit + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 507;

									*pWeapon->FallbackPaintKit() = 419; //doppler phase 2
									*pWeapon->FallbackWear() = 0.000000000001;
									*pWeapon->FallbackSeed() = 852;
								}
								else if (Model == 9) // M9 Bayonet
								{
									*pWeapon->ModelIndex() = iM9Bayonet; // m_nModelIndex
									*pWeapon->ViewModelIndex() = iM9Bayonet;
									*pWeapon->WorldModelIndex() = iM9Bayonet + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 508;

									*pWeapon->FallbackPaintKit() = 419; //doppler phase 2
									*pWeapon->FallbackWear() = 0.000000000001;
									*pWeapon->FallbackSeed() = 492;
								}
								else if (Model == 10) // Shadow Daggers
								{
									*pWeapon->ModelIndex() = iDagger; // m_nModelIndex
									*pWeapon->ViewModelIndex() = iDagger;
									*pWeapon->WorldModelIndex() = iDagger + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 516;

									*pWeapon->FallbackPaintKit() = 0;
								}
							}
						}
					}
				}
			}
		}
	}
	oFrameStageNotify(curStage);
}

/*=========================================================================================
_   _ ________          __  ______ _    _ _   _  _____ _______ _____ ____  _   _  _____
| \ | |  ____\ \        / / |  ____| |  | | \ | |/ ____|__   __|_   _/ __ \| \ | |/ ____|
|  \| | |__   \ \  /\  / /  | |__  | |  | |  \| | |       | |    | || |  | |  \| | (___
| . ` |  __|   \ \/  \/ /   |  __| | |  | | . ` | |       | |    | || |  | | . ` |\___ \
| |\  | |____   \  /\  /    | |    | |__| | |\  | |____   | |   _| || |__| | |\  |____) |
|_| \_|______|   \/  \/     |_|     \____/|_| \_|\_____|  |_|  |_____\____/|_| \_|_____/

=========================================================================================*/

OverrideViewFn oOverrideView;
void __stdcall Hooks::OverrideView(CViewSetup* vsView)
{
	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Interfaces::Engine->IsInGame() && !pLocal->IsScoped())//Vars::Visuals::Enabled)
	{
		vsView->fov = Menu::Window.VisualsTab.EffectFOV.GetValue();
	}

	G::fov = vsView->fov;

	if (Interfaces::Engine->IsInGame())
	{
		if (hackManager.pLocal())
		{
			static Vector vecAngles;
			Interfaces::Engine->GetViewAngles(vecAngles);

			if (Menu::Window.VisualsTab.EffectThirdPerson.GetIndex() != 0 && hackManager.pLocal()->IsAlive())
			{
				if (!Interfaces::pInput->m_fCameraInThirdPerson)
				{
					Interfaces::pInput->m_fCameraInThirdPerson = true;
					Interfaces::pInput->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, 40);
				}
			}
			else
			{
				Interfaces::pInput->m_fCameraInThirdPerson = false;
				Interfaces::pInput->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, 0);
			}
		}
	}

	oOverrideView(vsView);
}


bool __fastcall Hooked_FireEventClientSide(void* ecx, void* edx, IGameEvent* pEvent) {

	if (!pEvent)
		return oFireEventClientSide(ecx, pEvent);

	const char* szEventName = pEvent->GetName();

	if (!strcmp(szEventName, XorStr("player_death")))
	{
		int userid = pEvent->GetInt(XorStr("userid"));
		if (Interfaces::Engine->GetPlayerForUserID(userid) == StepResolver::iTargetID)
		{
			StepResolver::Shots = 0;
		}
	}

	if (!strcmp(szEventName, XorStr("player_hurt")))
	{
		int attacker = pEvent->GetInt(XorStr("attacker"));
		if (Interfaces::Engine->GetPlayerForUserID(attacker) == Interfaces::Engine->GetLocalPlayer())
		{
			StepResolver::change = false;
		}
	}

	return oFireEventClientSide(ecx, pEvent);
};

