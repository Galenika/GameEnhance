/*
Syn's AyyWare Framework 2015
*/

#include "Hooks.h"
#include "Hacks.h"
#include "Chams.h"
#include "Menu.h"
#include "Resolver.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "MiscHacks.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <future>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "AntiAntiAim.h"
#include "MiscHacks.h"
#include "Header.h"
#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))
class CViewSetup
{
public:
	int            x, x_old;
	int            y, y_old;
	int            width, width_old;
	int            height, height_old;
	bool        m_bOrtho;
	float        m_OrthoLeft;
	float        m_OrthoTop;
	float        m_OrthoRight;
	float        m_OrthoBottom;
	bool        m_bCustomViewMatrix;
	matrix3x4    m_matCustomViewMatrix;
	char        pad_0x68[0x48];
	float        fov;
	float        fovViewmodel;
	Vector        origin;
	Vector        angles;
	float        zNear;
	float        zFar;
	float        zNearViewmodel;
	float        zFarViewmodel;
	float        m_flAspectRatio;
	float        m_flNearBlurDepth;
	float        m_flNearFocusDepth;
	float        m_flFarFocusDepth;
	float        m_flFarBlurDepth;
	float        m_flNearBlurRadius;
	float        m_flFarBlurRadius;
	int            m_nDoFQuality;
	int            m_nMotionBlurMode;
	float        m_flShutterTime;
	Vector        m_vShutterOpenPosition;
	Vector        m_shutterOpenAngles;
	Vector        m_vShutterClosePosition;
	Vector        m_shutterCloseAngles;
	float        m_flOffCenterTop;
	float        m_flOffCenterBottom;
	float        m_flOffCenterLeft;
	float        m_flOffCenterRight;
	int            m_EdgeBlur;
};



// Funtion Typedefs
typedef void(__thiscall* DrawModelEx_)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4*);
typedef void(__thiscall* PaintTraverse_)(PVOID, unsigned int, bool, bool);
typedef bool(__thiscall* InPrediction_)(PVOID);
typedef void(__stdcall *FrameStageNotifyFn)(ClientFrameStage_t);
//typedef void(__thiscall *FindMdlFn)(void*, char*);


//typedef void(__stdcall *iOverRideViewFn)(CViewSetup*);
// Function Pointers to the originals
PaintTraverse_ oPaintTraverse;
DrawModelEx_ oDrawModelExecute;
//FindMdlFn oFindMDL;
FrameStageNotifyFn oFrameStageNotify;
Vector aathird;
//iOverRideViewFn oOverrideView1;
// Hook function prototypes
//void __fastcall FindMDL(void* thisptr, void* edx, char *path);
void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
bool __stdcall Hooked_InPrediction();
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld);
bool __stdcall CreateMoveClient_Hooked(/*void* self, int edx,*/ float frametime, CUserCmd* pCmd);
void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage);
//void __stdcall Hooked_OverrideView(CViewSetup* pSetup);

// VMT Managers
namespace Hooks
{
	// VMT Managers
	Utilities::Memory::CVMTHookManager VMTPanel; // Hooking drawing functions
	Utilities::Memory::CVMTHookManager VMTClient; // Maybe CreateMove
	Utilities::Memory::CVMTHookManager VMTClientMode; // CreateMove for functionality
	Utilities::Memory::CVMTHookManager VMTModelRender; // DrawModelEx for chams
	Utilities::Memory::CVMTHookManager VMTPrediction; // InPrediction for no vis recoil
										//			 	Utilities::Memory::CVMTHookManager VMTOverrideView1;
};

// Initialise all our hooks
void Hooks::Initialise()
{
	/*

	COOLDOWNKING B1G PASTA
	http://www.unknowncheats.me/forum/1016761-post11.html
	THANK YOU REACTIION

	*/

	// Panel hooks for drawing to the screen via surface functions
	VMTPanel.bInitialize((PDWORD*)Interfaces::Panels);
	oPaintTraverse = (PaintTraverse_)VMTPanel.dwHookMethod((DWORD)PaintTraverse_Hooked, Offsets::VMT::Panel_PaintTraverse);
	VMTPanel.ReHook();
	//Utilities::Log("Paint Traverse Hooked");

	// No Visual Recoil
	VMTPrediction.bInitialize((PDWORD*)Interfaces::Prediction);
	VMTPrediction.dwHookMethod((DWORD)Hooked_InPrediction, 14);
	VMTPrediction.ReHook();
	//Utilities::Log("InPrediction Hooked");

	// Chams
	VMTModelRender.bInitialize((PDWORD*)Interfaces::ModelRender);
	oDrawModelExecute = (DrawModelEx_)VMTModelRender.dwHookMethod((DWORD)Hooked_DrawModelExecute, Offsets::VMT::ModelRender_DrawModelExecute);
	VMTModelRender.ReHook();
	//Utilities::Log("DrawModelExecute Hooked");

	// Setup ClientMode Hooks
	VMTClientMode.bInitialize((PDWORD*)Interfaces::ClientMode);
	VMTClientMode.dwHookMethod((DWORD)CreateMoveClient_Hooked, 24);
	VMTClient.ReHook();
	//Utilities::Log("ClientMode CreateMove Hooked");



	// Setup client hooks
	VMTClient.bInitialize((PDWORD*)Interfaces::Client);
	oFrameStageNotify = (FrameStageNotifyFn)VMTClient.dwHookMethod((DWORD)&Hooked_FrameStageNotify, 36);
	//fov




}
// Undo our hooks
void Hooks::UndoHooks()
{
	VMTPanel.UnHook();
	VMTPrediction.UnHook();
	VMTModelRender.UnHook();
	VMTClientMode.UnHook();
	//VMTClient.RestoreOriginal();
}

void MovementCorrection(CUserCmd* pCmd)
{

}


//---------------------------------------------------------------------------------------------------------
//                                         Hooked Functions
//---------------------------------------------------------------------------------------------------------
// Paint Traverse Hooked function
void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	if (Menu::Window.MiscTab.OtherThirdperson.GetState() && !strcmp("HudZoom", Interfaces::Panels->GetName(vguiPanel)))
		return;
	oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);
	//if (Menu::Window.MiscTab.OtherThirdperson.GetState() && !strcmp("HudZoom", Interfaces::Panels->GetName(vguiPanel)))
	//	return;
	static unsigned int FocusOverlayPanel = 0;
	static bool FoundPanel = false;

	if (!FoundPanel)
	{
		PCHAR szPanelName = (PCHAR)Interfaces::Panels->GetName(vguiPanel);
		if (strstr(szPanelName, "MatSystemTopPanel"))
		{
			FocusOverlayPanel = vguiPanel;
			FoundPanel = true;
		}
	}
	else if (FocusOverlayPanel == vguiPanel)
	{
		//Render::GradientV(8, 8, 160, 18, Color(0, 0, 0, 0), Color(7, 39, 17, 255));
		if (GetKeyState(VK_F4))
		{
			int red = Menu::Window.ColorsTab.MenuR.GetValue();
			int green = Menu::Window.ColorsTab.MenuG.GetValue();
			int blue = Menu::Window.ColorsTab.MenuB.GetValue();
			Render::Text(10, 30, Color(red, green, blue, 220), Render::Fonts::Menu, "RedHead.Pro | 2017");
			if (Menu::Window.MiscTab.OtherSafeMode.GetState())
			{
				Render::Text(10, 50, Color(0, 255, 0, 255), Render::Fonts::Menu, "Secure Mode");
			}
			else
			{
				Render::Text(10, 50, Color(255, 0, 0, 255), Render::Fonts::Menu, "InSecure Mode");
			}
		}

		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
			Hacks::DrawHacks();
		//if (Menu::Window.SkinchangerTab.SkinEnable.GetState() && Menu::Window.MiscTab.OtherFakeLag.GetValue() >= 2) {
		//	GUI.DrawRadar();
		//}
		// Update and draw the menu
		Menu::DoUIFrame();
		


	}
	
}
BYTE bMoveData[512];
void StartPrediction(CUserCmd* pCmd, IClientEntity* LocalPlayer)
{
	static bool done = false;
	if (LocalPlayer->IsAlive() && Menu::Window.RageBotTab.EnginePred.GetState() && !done)
	{
		//Interfaces::CVar->FindVar("rate")->SetValue(1048576);
		//Interface->Cvar()->GetVTable<ICvar>()->FindVar( "viewmodel_fov" )->SetValue( 80 );
		Interfaces::CVar->FindVar("cl_interp")->SetValue(0.01f);
		Interfaces::CVar->FindVar("cl_cmdrate")->SetValue(66);
		Interfaces::CVar->FindVar("cl_updaterate")->SetValue(66);
		Interfaces::CVar->FindVar("cl_interp_all")->SetValue(0.0f);
		Interfaces::CVar->FindVar("cl_interp_ratio")->SetValue(1.0f);
		Interfaces::CVar->FindVar("cl_smooth")->SetValue(0.0f);
		Interfaces::CVar->FindVar("cl_smoothtime")->SetValue(0.01f);
		done = true;
	}
}
void EndPrediction(CUserCmd* pCmd, IClientEntity* LocalPlayer)
{
	
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
	if (Menu::Window.VisualsTab.OtherNoVisualRecoil.GetState() && (DWORD)(_ReturnAddress()) == Offsets::Functions::dwCalcPlayerView)
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

std::vector<const char*> smoke_materials = {
	"particle/vistasmokev1/vistasmokev1_fire",
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
};



// DrxwModelExec for chams and shit
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
	Color color;
	float flColor[3] = { 0.f };
	static IMaterial* CoveredLit = CreateMaterial(true);
	static IMaterial* OpenLit = CreateMaterial(false);
	static IMaterial* CoveredFlat = CreateMaterial(true, false);
	static IMaterial* OpenFlat = CreateMaterial(false, false);
	bool DontDraw = false;

	const char* ModelName = Interfaces::ModelInfo->GetModelName((model_t*)pInfo.pModel);
	IClientEntity* pModelEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(pInfo.entity_index);
	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());


		// Player Chams
		int ChamsStyle = Menu::Window.VisualsTab.OptionsChams.GetIndex();
		int HandsStyle = Menu::Window.VisualsTab.OtherNoHands.GetIndex();
		if (ChamsStyle != 0 && Menu::Window.VisualsTab.FiltersPlayers.GetState() && strstr(ModelName, "models/player"))
		{
			if (pLocal)
			{
				
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;

				IClientEntity* pModelEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(pInfo.entity_index);
				if (pModelEntity)
				{
					IClientEntity *local = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
					if (local)
					{
						if (Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() && pModelEntity->GetTeamNum() == local->GetTeamNum())
						{

						}
						else
						{
							if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0 /*&& pModelEntity->GetTeamNum() != local->GetTeamNum()*/)
							{
								float alpha = 1.f;

								if (pModelEntity->HasGunGameImmunity())
									alpha = 0.5f;

								if (pModelEntity->GetTeamNum() == 2)
								{
									flColor[0] = Menu::Window.ColorsTab.TColorNoVisR.GetValue() / 255.f;
									flColor[1] = Menu::Window.ColorsTab.TColorNoVisG.GetValue() / 255.f;
									flColor[2] = Menu::Window.ColorsTab.TColorNoVisB.GetValue() / 255.f;
								}
								else
								{
									flColor[0] = Menu::Window.ColorsTab.CTColorNoVisR.GetValue() / 255.f;
									flColor[1] = Menu::Window.ColorsTab.CTColorNoVisG.GetValue() / 255.f;
									flColor[2] = Menu::Window.ColorsTab.CTColorNoVisB.GetValue() / 255.f;
								}

								Interfaces::RenderView->SetColorModulation(flColor);
								Interfaces::RenderView->SetBlend(alpha);
								Interfaces::ModelRender->ForcedMaterialOverride(covered);
								oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

								if (pModelEntity->GetTeamNum() == 2)
								{
									flColor[0] = Menu::Window.ColorsTab.TColorVisR.GetValue() / 255.f;
									flColor[1] = Menu::Window.ColorsTab.TColorVisG.GetValue() / 255.f;
									flColor[2] = Menu::Window.ColorsTab.TColorVisB.GetValue() / 255.f;
								}
								else
								{
									flColor[0] = Menu::Window.ColorsTab.CTColorVisR.GetValue() / 255.f;
									flColor[1] = Menu::Window.ColorsTab.CTColorVisG.GetValue() / 255.f;
									flColor[2] = Menu::Window.ColorsTab.CTColorVisB.GetValue() / 255.f;
								}

								Interfaces::RenderView->SetColorModulation(flColor);
								Interfaces::RenderView->SetBlend(alpha);
								Interfaces::ModelRender->ForcedMaterialOverride(open);
							}
							else
							{
								color.SetColor(255, 255, 255, 255);
								ForceMaterial(color, open);
							}
					}
					}
				}
			}
		}
		else if (HandsStyle != 0 && strstr(ModelName, "arms"))
		{
			if (HandsStyle == 1)
			{
				DontDraw = true;
			}
			else if (HandsStyle == 2)
			{
				Interfaces::RenderView->SetBlend(0.3);
			}
			else if (HandsStyle == 3)
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
				if (pLocal)
				{
					if (pLocal->IsAlive())
					{
						int alpha = pLocal->HasGunGameImmunity() ? 150 : 255;

						if (pLocal->GetTeamNum() == 2)
							color.SetColor(240, 30, 255, 255);
						else
							color.SetColor(63, 72, 255, 255);

						ForceMaterial(color, covered);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

						if (pLocal->GetTeamNum() == 2)
							color.SetColor(247, 180, 255, 255);
						else
							color.SetColor(32, 180, 255, 255);
					}
					else
					{
						color.SetColor(255, 255, 255, 255);
					}

					ForceMaterial(color, open);
				}
			}
			else
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
					colors[prev] -= 0.05f;
					colors[counter] += 0.05f;
				}

				Interfaces::RenderView->SetColorModulation(colors);
				Interfaces::RenderView->SetBlend(0.3);
				Interfaces::ModelRender->ForcedMaterialOverride(OpenLit);
			}
		}
		else if (ChamsStyle != 0 && Menu::Window.VisualsTab.FiltersWeapons.GetState() && strstr(ModelName, "_dropped.mdl"))
		{
			IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
			color.SetColor(255, 255, 255, 255);
			ForceMaterial(color, covered);
		}


	if (!DontDraw)
		oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	Interfaces::ModelRender->ForcedMaterialOverride(NULL);
}

/*bool __fastcall CreateMoveClient_Hooked(void* self, int edx, float frametime, CUserCmd* pCmd)
{
void* pEbp;
__asm mov pEbp, ebp;

bool* pSendPacket = (bool*)*(DWORD*)pEbp - 0x1c;
if (*pSendPacket != 0x1)
{
pSendPacket = nullptr;
}

if (*pSendPacket)
{
pSendPacket = false;
}
}*/


CUserCmd*		m_cmd;
Vector			m_position;
Vector			m_size;
IClientEntity*	m_player;

void MsgFunc_ServerRankRevealAll()
{//Wokr on it now, i have to go for while so its yours. :D
	using tServerRankRevealAllFn = bool(__cdecl*)(int*);
	static tServerRankRevealAllFn ServerRankRevealAll = (tServerRankRevealAllFn)((PDWORD)Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x8B\x0D\x00\x00\x00\x00\x68\x00\x00\x00\x00", "xxxxx????x????"));
	int fArray[3] = { 0,0,0 };

	ServerRankRevealAll(fArray);
}


void UpdateRadar(CUserCmd* pCmd)
{
	m_cmd = pCmd;

	if (!m_cmd) {
		return;
	}

	int index = Interfaces::Engine->GetLocalPlayer();

	if (!index) {
		return;
	}

	m_player = (IClientEntity*)Interfaces::EntList->GetClientEntity(index);
}

void ThirdPerson()
{
	//static bool thirdpersonon = false;
	//if (hackManager.pLocal()->IsScoped())
	//{
	//	Interfaces::Engine->ClientCmd_Unrestricted("firstperon");
	//	thirdpersonon = false;
	//}

	//if (Menu::Window.MiscTab.OtherThirdperson.GetState() && !thirdpersonon)
	//{
	//	Interfaces::Engine->ClientCmd_Unrestricted("thirdperson");
	//	thirdpersonon = true;
	//}
	//else if (thirdpersonon && !Menu::Window.MiscTab.OtherThirdperson.GetState())
	//{
	//	Interfaces::Engine->ClientCmd_Unrestricted("firstperon");
	//	thirdpersonon = false;
	//}
	//
	////10, 11
}
void SetClanTag(const char* tag, const char* name)
{

	static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(Utilities::Memory::FindPatternV2("engine.dll", "53 56 57 8B DA 8B F9 FF 15"));
	pSetClanTag(tag, name);
}
void ClanTagChanger()
{
	static int counter;
		static int motion = 0;
		int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 2.5;

		if (counter % 48 == 0)
			motion++;
		int value = ServerTime % 23;
		switch (value) {
		case 0:SetClanTag("           ", "MotionSense"); break;
		case 1:SetClanTag("M          ", "MotionSense"); break;
		case 2:SetClanTag("MO         ", "MotionSense"); break;
		case 3:SetClanTag("MOT        ", "MotionSense"); break;
		case 4:SetClanTag("MOTI       ", "MotionSense"); break;
		case 5:SetClanTag("MOTIO      ", "MotionSense"); break;
		case 6:SetClanTag("MOTION     ", "MotionSense"); break;
		case 7:SetClanTag("MOTIONS    ", "MotionSense"); break;
		case 8:SetClanTag("MOTIONSE   ", "MotionSense"); break;
		case 9:SetClanTag("MOTIONSEN  ", "MotionSense"); break;
		case 10:SetClanTag("MOTIONSENS ", "MotionSense"); break;
		case 11:SetClanTag("MOTIONSENSE", "MotionSense"); break;
		case 12:SetClanTag(" MOTIONSENS", "MotionSense"); break;
		case 13:SetClanTag("  MOTIONSEN", "MotionSense"); break;
		case 14:SetClanTag("   MOTIONSE", "MotionSense"); break;
		case 15:SetClanTag("    MOTIONS", "MotionSense"); break;
		case 16:SetClanTag("     MOTION", "MotionSense"); break;
		case 17:SetClanTag("      MOTIO", "MotionSense"); break;
		case 18:SetClanTag("       MOTI", "MotionSense"); break;
		case 19:SetClanTag("        MOT", "MotionSense"); break;
		case 20:SetClanTag("         MO", "MotionSense"); break;
		case 21:SetClanTag("          M", "MotionSense"); break;
		case 22:SetClanTag("          ", "MotionSense"); break;
		}
		counter++;
	
}
// ClientMode CreateMove
bool __stdcall CreateMoveClient_Hooked(/*void* self, int edx,*/ float frametime, CUserCmd* pCmd)
{
	// Choke packet memes

	if (!pCmd->command_number)
		return true;

	if (Interfaces::Engine->IsConnected() || Interfaces::Engine->IsInGame())
	{

		switch(Menu::Window.MiscTab.ClanTag.GetIndex())
		{
		case 0:
			//Nothing
			break;
		case 1:
			//Clear
			SetClanTag("", "");
			break;
		case 2:
			ClanTagChanger();
			break;
		case 3:
			SetClanTag("[VALV\xE1\xB4\xB1]", "[VALV\xE1\xB4\xB1]");
			break;
		case 4:
			SetClanTag("STAINLESS '", "0:0:1337");
			break;
		case 5:
			SetClanTag("UNTRA$T3D", "HvHKings");
			break;
		case 6:
			SetClanTag("bAIMed By", "BAIM.PRO");
			break;
		case 7:
			SetClanTag("Uff Yaa $", "Uff Yaa $");
			break;
		case 8:
			SetClanTag("bloodgodz", "bloodgodz");
			break;
		}
			


		PVOID pebp;

		__asm mov pebp, ebp;

		bool* pbSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);
		bool& bSendPacket = *pbSendPacket;
		bool kokot = false;
		if (Menu::Window.MiscTab.OtherFakeLag.GetValue() > 0) {
			int iChoke = Menu::Window.MiscTab.OtherFakeLag.GetValue();//14
			static int iFakeLag = 0;

			if (iFakeLag < iChoke)
				bSendPacket = false;

			if (iFakeLag > iChoke + 3)
				iFakeLag = 0;

			iFakeLag++;

		}
		if (kokot =  true && Menu::Window.RageBotTab.AimbotFov.GetValue() > 99999)
		{
			static int Ticks = 0;

			if ((Ticks < 8))
				bSendPacket = false;

			if (Ticks == (8 * 2))
			{
				bSendPacket = true;

				Ticks = 0;
			}

			Ticks++;
		}
	/*	if (Menu::Window.MiscTab.FakeLagAmount.GetValue() > 0)
		{
		int tick;
		float factor = Menu::Window.MiscTab.FakeLagAmount.GetValue();
			if (tick < factor)
			{
				bSendPacket = false;
				tick++;
			}
			else
			{

				bSendPacket = true;
				tick = 0;
			}
		}
*/
		UpdateRadar(pCmd);

		// Backup for safety
		Vector origView = pCmd->viewangles;
		Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
		Vector qAimAngles;
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);
		// Do da hacks
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && pLocal && pLocal->IsAlive())
		{
			ThirdPerson();
			StartPrediction(pCmd, pLocal);
			Hacks::MoveHacks(pCmd, bSendPacket);
			EndPrediction(pCmd, pLocal);

		}

		if (Menu::Window.MiscTab.FakeLagAmount.GetValue() > 0 && Menu::Window.MiscTab.FakeLagAmount.GetValue() > 0) {
			int iChoke = Menu::Window.MiscTab.FakeLagAmount.GetValue();
			static int iFakeLag = 0;

			if (iFakeLag < iChoke)
				bSendPacket = false;

			if (iFakeLag > iChoke + 3)
				iFakeLag = 0;

			iFakeLag++;

		}

		//if (pCmd->buttons & IN_SCORE && Menu::Window.VisualsTab.ShowRanks.GetState())
		//{
		//	int fArray[3] = { 0,0,0 };
		//	MsgFunc_ServerRankRevealAll();
		//}


	/*	if (Menu::Window.VisualsTab.ShowRanks.GetState())
		{

		if (!(pCmd->buttons & IN_SCORE))
			return;

		float input[3] = { 0.f };
		MsgFunc_ServerRankRevealAllFn MsgFunc_ServerRankRevealAll;
		MsgFunc_ServerRankRevealAll(input);
		}
		*/
		/*bool infakelag = false;
		bool fl = false;
		int flTimer = -1;
		bool lastfltick = -1;
		int factor = Menu::Window.MiscTab.FakeLagAmount.GetValue();
		int ticks;
		if (Menu::Window.MiscTab.FakeLagEnable.GetState())
		{
			if (ticks < factor)
			{
				bSendPacket = false;
			}
			else {
					lastfltick = true;
				bSendPacket = true;
				ticks = 0;
			}
			ticks++;
		}
		*/
		//Movement Fix
		//GameUtils::CL_FixMove(pCmd, origView);
		qAimAngles.Init(0.0f, GetAutostrafeView().y, 0.0f);
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &aimforward, &aimright, &aimup);
		Vector vForwardNorm;		Normalize(viewforward, vForwardNorm);
		Vector vRightNorm;			Normalize(viewright, vRightNorm);
		Vector vUpNorm;				Normalize(viewup, vUpNorm);

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



		if (Menu::Window.RageBotTab.AntiAimPitch.GetIndex() == 11 || Menu::Window.RageBotTab.AntiAimPitch.GetIndex() == 12)
		{
			pCmd->forwardmove = -pCmd->forwardmove;
		}
			pCmd->forwardmove = DotProduct(forward * vForwardNorm, aimforward) + DotProduct(right * vRightNorm, aimforward) + DotProduct(up * vUpNorm, aimforward);

			if (Menu::Window.RageBotTab.AntiAimPitch.GetIndex() == 11 || Menu::Window.RageBotTab.AntiAimPitch.GetIndex() == 12)
			{
				pCmd->forwardmove = -pCmd->forwardmove;
			}
		pCmd->sidemove = DotProduct(forward * vForwardNorm, aimright) + DotProduct(right * vRightNorm, aimright) + DotProduct(up * vUpNorm, aimright);
		if (Menu::Window.RageBotTab.AntiAimPitch.GetIndex() == 11 || Menu::Window.RageBotTab.AntiAimPitch.GetIndex() == 12)
		{
			pCmd->forwardmove = -pCmd->forwardmove;
		}
		pCmd->upmove = DotProduct(forward * vForwardNorm, aimup) + DotProduct(right * vRightNorm, aimup) + DotProduct(up * vUpNorm, aimup);
		

		if (Menu::Window.RageBotTab.AntiAimPitch.GetIndex() == 11 || Menu::Window.RageBotTab.AntiAimPitch.GetIndex() == 12)
		{
			pCmd->forwardmove = -pCmd->forwardmove;
		}

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
				//Utilities::Log("Having to re-normalise!");
				GameUtils::NormaliseViewAngle(pCmd->viewangles);
				//Beep(750, 800);
				if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
				{
					pCmd->viewangles = origView;
					pCmd->sidemove = right;
					pCmd->forwardmove = forward;
					//
				}
				
			}
		}
		if (Menu::Window.RageBotTab.Thirdaa.GetIndex() == 1)
		{
			if (bSendPacket)
				aathird = pCmd->viewangles;
		}
		if (Menu::Window.RageBotTab.Thirdaa.GetIndex() == 2)
		{
			if (!pCmd->buttons & IN_ATTACK)
			{
				if (!bSendPacket)
					aathird = pCmd->viewangles;
			}
		}
		if (Menu::Window.RageBotTab.Thirdaa.GetIndex() == 3)
		{
				aathird = pCmd->viewangles;
		}
	}
	
	return false;
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
		

		switch (Menu::Window.GlovesChanger.GloveModel.GetIndex())
		{
		case 1:
		{
			*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5027;
			glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"));
			//*glovestochange->ViewModelIndex() = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl");
			//*glovestochange->WorldModelIndex() = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl") + 1;
			break;
		}
		case 2:
		{
			*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5032;
			glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"));
			break;
		}
		case 3:
		{
			*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5031;
			glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"));
			break;
		}
		case 4:
		{
			*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5030;
			glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"));
			break;
		}
		case 5:
		{
			*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5033;
			glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"));
			break;
		}
		case 6:
		{
			*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5034;
			glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"));
			break;
		}
		default:
			break;
		}

		switch (Menu::Window.GlovesChanger.GloveSkin.GetIndex())
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
// Hooked FrameStageNotify for removing visual recoil

void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage)
{

	if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());



		//Utilities::Log("APPLY SKIN APPLY SKIN");
//		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		for (int i = 0; i <= Interfaces::EntList->GetHighestEntityIndex(); i++) // CHANGE
		{
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

			if (pEntity)
			{

				
				
					ApplyCustomGloves(pLocal);

				ULONG hOwnerEntity = *(PULONG)((DWORD)pEntity + 0x148);

				IClientEntity* pOwner = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)hOwnerEntity);

				if (pOwner)
				{
					if (pOwner == pLocal)
					{
						std::string sWeapon = Interfaces::ModelInfo->GetModelName(pEntity->GetModel());

						if (!(sWeapon.find("models/weapons", 0) != std::string::npos))
							continue;

						if (sWeapon.find("c4_planted", 0) != std::string::npos)
							continue;

						if (sWeapon.find("thrown", 0) != std::string::npos)
							continue;

						if (sWeapon.find("smokegrenade", 0) != std::string::npos)
							continue;

						if (sWeapon.find("flashbang", 0) != std::string::npos)
							continue;

						if (sWeapon.find("fraggrenade", 0) != std::string::npos)
							continue;

						if (sWeapon.find("molotov", 0) != std::string::npos)
							continue;

						if (sWeapon.find("decoy", 0) != std::string::npos)
							continue;

						if (sWeapon.find("incendiarygrenade", 0) != std::string::npos)
							continue;

						if (sWeapon.find("ied", 0) != std::string::npos)
							continue;

						if (sWeapon.find("w_eq_", 0) != std::string::npos)
							continue;

						CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)pEntity;

						ClientClass *pClass = Interfaces::Client->GetAllClasses();

						if (Menu::Window.SkinchangerTab.SkinEnable.GetState())
						{
							int Model = Menu::Window.SkinchangerTab.KnifeModel.GetIndex();

							int M41S = Menu::Window.SkinchangerTab.M41SSkin.GetIndex();
							int M4A4 = Menu::Window.SkinchangerTab.M4A4Skin.GetIndex();
							int AK47 = Menu::Window.SkinchangerTab.AK47Skin.GetIndex();
							int AWP = Menu::Window.SkinchangerTab.AWPSkin.GetIndex();
							int GLOCK = Menu::Window.SkinchangerTab.GLOCKSkin.GetIndex();
							int USPS = Menu::Window.SkinchangerTab.USPSSkin.GetIndex();
							int DEAGLE = Menu::Window.SkinchangerTab.DEAGLESkin.GetIndex();
							int DUALS = Menu::Window.SkinchangerTab.DUALSSkin.GetIndex();
							int FIVE7 = Menu::Window.SkinchangerTab.FIVESEVENSkin.GetIndex();
							int AUG = Menu::Window.SkinchangerTab.AUGSkin.GetIndex();
							int FAMAS = Menu::Window.SkinchangerTab.FAMASSkin.GetIndex();
							int G3SG1 = Menu::Window.SkinchangerTab.G3SG1Skin.GetIndex();
							int Galil = Menu::Window.SkinchangerTab.GALILSkin.GetIndex();
							int M249 = Menu::Window.SkinchangerTab.M249Skin.GetIndex();
							int MAC10 = Menu::Window.SkinchangerTab.MAC10Skin.GetIndex();
							int P90 = Menu::Window.SkinchangerTab.P90Skin.GetIndex();
							int UMP45 = Menu::Window.SkinchangerTab.UMP45Skin.GetIndex();
							int XM1014 = Menu::Window.SkinchangerTab.XM1014Skin.GetIndex();
							int BIZON = Menu::Window.SkinchangerTab.BIZONSkin.GetIndex();
							int MAG7 = Menu::Window.SkinchangerTab.MAG7Skin.GetIndex();
							int NEGEV = Menu::Window.SkinchangerTab.NEGEVSkin.GetIndex();
							int SAWEDOFF = Menu::Window.SkinchangerTab.SAWEDOFFSkin.GetIndex();
							int TEC9 = Menu::Window.SkinchangerTab.TECNINESkin.GetIndex();
							int P2000 = Menu::Window.SkinchangerTab.P2000Skin.GetIndex();
							int MP7 = Menu::Window.SkinchangerTab.MP7Skin.GetIndex();
							int MP9 = Menu::Window.SkinchangerTab.MP9Skin.GetIndex();
							int NOVA = Menu::Window.SkinchangerTab.NOVASkin.GetIndex();
							int P250 = Menu::Window.SkinchangerTab.P250Skin.GetIndex();
							int SCAR20 = Menu::Window.SkinchangerTab.SCAR20Skin.GetIndex();
							int SG553 = Menu::Window.SkinchangerTab.SG553Skin.GetIndex();
							int SSG08 = Menu::Window.SkinchangerTab.SSG08Skin.GetIndex();
							int Magnum = Menu::Window.SkinchangerTab.DEAGLESkin.GetIndex();
							//int Duals = Menu::Window.SkinchangerTab.DUALSSkin.GetIndex();

							if (pEntity->GetClientClass()->m_ClassID != (int)CSGOClassID::CKnife)
							{
								if (Menu::Window.SkinchangerTab.SkinName.getText().length() > 1)
								{
									auto pCustomName = MakePtr(char*, pWeapon, 0x301C);
									strcpy_s(pCustomName, 32, Menu::Window.SkinchangerTab.SkinName.getText().c_str());
								}
							}
							player_info_t LocalPlayerInfo;
							Interfaces::Engine->GetPlayerInfo(Interfaces::Engine->GetLocalPlayer(), &LocalPlayerInfo);
							if (Menu::Window.SkinchangerTab.StatTrackAmount.getText().c_str() != NULL && Menu::Window.SkinchangerTab.StatTrackAmount.getText().c_str() != "")
							{
								int st = atoi(Menu::Window.SkinchangerTab.StatTrackAmount.getText().c_str());

								if (Menu::Window.SkinchangerTab.StatTrakEnable.GetState())
									*pWeapon->FallbackStatTrak() = st;
								*pWeapon->m_AttributeManager()->m_Item()->AccountID() = LocalPlayerInfo.m_nXuidLow;
							}

							int weapon = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();



							switch (weapon)
							{
							case 7: // AK47 
							{
								switch (AK47)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 341;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 14;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 44;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 172;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 180;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 394;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 300;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 226;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 282;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 302;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 316;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 340;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 380;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 422;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 456;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 474;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 490;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 506;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 524;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 600;
									break;
								default:
									break;
								}
							}
							break;
							case 16: // M4A4
							{
								switch (M4A4)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 155;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 187;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 255;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 309;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 215;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 336;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 384;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 400;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 449;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 471;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 480;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 512;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 533;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 588;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 632;
									break;
								default:
									break;
								}
							}
							break;
							case 60: // M4A1
							{
								switch (M41S)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 60;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 430;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 77;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 235;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 254;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 189;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 301;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 217;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 257;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 321;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 326;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 360;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 383;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 440;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 445;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 497;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 548;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 587;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 631;
									break;
								default:
									break;
								}
							}
							break;
							case 9: // AWP
							{
								switch (AWP)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 174;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 344;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 84;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 30;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 51;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 72;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 181;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 259;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 395;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 212;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 227;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 251;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 279;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 424;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 446;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 451;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 475;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 525;
									break;
								default:
									break;
								}
							}
							break;
							case 61: // USP
							{
								switch (USPS)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 25;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 60;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 235;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 183;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 339;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 217;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 221;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 236;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 277;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 290;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 313;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 318;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 332;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 364;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 454;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 489;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 504;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 540;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 637;
									break;
								default:
									break;
								}
							}
							break;
							case 4: // Glock
							{
								switch (GLOCK)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 2;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 3;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 38;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 40;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 48;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 437;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 99;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 159;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 399;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 208;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 230;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 278;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 293;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 353;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 367;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 381;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 479;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 495;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 532;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 607;
									break;
								case 20:
									*pWeapon->FallbackPaintKit() = 586;
									break;
								default:
									break;
								}
							}
							break;
							case 1: // Deagle
							{
								switch (DEAGLE)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 37;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 347;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 468;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 469;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 5;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 232;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 17;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 40;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 61;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 90;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 235;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 185;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 231;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 237;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 397;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 328;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 273;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 296;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 351;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 425;
									break;
								case 20:
									*pWeapon->FallbackPaintKit() = 470;
									break;
								case 21:
									*pWeapon->FallbackPaintKit() = 509;
									break;
								case 22:
									*pWeapon->FallbackPaintKit() = 527;
									break;
								default:
									break;
								}
							}
							break;
							case 3: // Five Seven
							{
								switch (FIVE7)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 3;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 27;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 44;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 46;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 78;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 141;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 151;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 254;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 248;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 210;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 223;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 252;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 265;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 274;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 464;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 352;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 377;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 387;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 427;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 510;
									break;
								case 20:
									*pWeapon->FallbackPaintKit() = 530;
									break;
								default:
									break;
								}
							}
							break;
							case 8: // AUG
							{
								switch (AUG)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 9;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 33;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 280;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 305;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 375;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 442;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 444;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 455;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 507;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 541;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 601;
									break;
								default:
									break;
								}
							}
							break;
							case 10: // Famas
							{
								switch (FAMAS)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 22;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 47;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 92;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 429;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 154;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 178;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 194;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 244;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 218;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 260;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 288;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 371;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 477;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 492;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 529;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 604;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 626;
									break;
								default:
									break;
								}
							}
							break;
							case 11: // G3SG1
							{
								switch (G3SG1)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 8;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 6;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 27;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 46;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 72;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 74;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 147;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 235;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 170;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 195;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 229;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 294;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 465;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 464;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 382;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 438;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 493;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 511;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 545;
									break;
								default:
									break;
								}
							}
							break;
							case 13: // Galil
							{
								switch (Galil)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 5;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 22;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 83;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 428;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 76;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 119;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 235;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 235;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 398;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 192;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 308;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 216;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 237;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 241;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 264;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 297;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 379;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 460;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 478;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 494;
									break;
								case 20:
									*pWeapon->FallbackPaintKit() = 546;
									break;
								default:
									break;
								}
							}
							break;
							case 14: // M249
							{
								switch (M249)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 22;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 75;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 202;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 243;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 266;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 401;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 452;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 472;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 496;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 547;
									break;
								default:
									break;
								}
							}
							break;
							case 17: // Mac 10
							{
								switch (MAC10)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 101;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 3;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 32;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 5;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 17;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 38;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 433;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 98;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 157;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 188;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 337;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 246;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 284;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 310;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 333;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 343;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 372;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 402;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 498;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 534;
									break;
								default:
									break;
								}
							}
							break;
							case 19: // P90
							{
								switch (P90)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 342;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 20;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 22;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 100;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 67;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 111;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 124;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 156;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 234;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 169;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 175;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 182;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 244;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 228;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 283;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 311;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 335;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 359;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 486;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 516;
									break;
								case 20:
									*pWeapon->FallbackPaintKit() = 636;
									break;
								default:
									break;
								}
							}
							break;
							case 24: // UMP-45
							{
								switch (UMP45)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 37;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 5;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 15;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 17;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 436;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 70;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 93;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 169;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 175;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 193;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 392;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 281;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 333;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 362;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 441;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 488;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 556;
									break;
								default:
									break;
								}
							}
							break;
							case 25: // XM1014
							{
								switch (XM1014)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 166;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 238;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 27;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 42;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 96;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 95;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 135;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 151;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 235;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 235;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 169;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 205;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 240;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 251;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 393;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 320;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 314;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 348;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 370;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 407;
									break;
								case 20:
									*pWeapon->FallbackPaintKit() = 505;
									break;
								case 21:
									*pWeapon->FallbackPaintKit() = 521;
									break;
								case 22:
									*pWeapon->FallbackPaintKit() = 557;
									break;
								default:
									break;
								}
							}
							break;
							case 26: // Bizon
							{
								switch (BIZON)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 13;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 164;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 25;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 27;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 70;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 148;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 149;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 159;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 235;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 171;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 203;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 224;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 236;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 267;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 306;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 323;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 349;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 376;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 457;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 459;
									break;
								case 20:
									*pWeapon->FallbackPaintKit() = 508;
									break;
								case 21:
									*pWeapon->FallbackPaintKit() = 526;
									break;
								case 22:
									*pWeapon->FallbackPaintKit() = 542;
									break;
								default:
									break;
								}
							}
							break;
							case 27: // Mag 7
							{
								switch (MAG7)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 462;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 34;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 32;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 100;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 39;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 431;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 99;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 171;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 177;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 198;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 291;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 385;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 473;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 499;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 535;
									break;
								default:
									break;
								}
							}
							break;
							case 28: // Negev
							{
								switch (NEGEV)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 28;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 432;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 157;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 235;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 201;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 240;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 285;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 298;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 317;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 355;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 369;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 483;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 514;
									break;
								default:
									break;
								}
							}
							break;
							case 29: // Sawed Off
							{
								switch (SAWEDOFF)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 345;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 5;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 22;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 30;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 83;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 38;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 41;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 434;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 119;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 235;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 171;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 204;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 405;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 246;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 250;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 390;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 256;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 323;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 458;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 459;
									break;
								case 20:
									*pWeapon->FallbackPaintKit() = 517;
									break;
								case 21:
									*pWeapon->FallbackPaintKit() = 552;
									break;
								case 22:
									*pWeapon->FallbackPaintKit() = 638;
									break;
								default:
									break;
								}
							}
							break;
							case 30: // Tec 9
							{
								switch (TEC9)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 101;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 2;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 5;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 463;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 17;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 36;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 439;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 159;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 235;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 179;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 248;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 206;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 216;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 242;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 272;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 289;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 303;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 374;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 459;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 520;
									break;
								case 20:
									*pWeapon->FallbackPaintKit() = 539;
									break;
								case 21:
									*pWeapon->FallbackPaintKit() = 555;
									break;
								case 22:
									*pWeapon->FallbackPaintKit() = 614;
									break;
								default:
									break;
								}
							}
							break;
							case 32: // P2000
							{
								switch (P2000)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 104;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 32;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 21;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 25;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 36;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 485;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 38;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 71;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 95;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 184;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 211;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 338;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 246;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 275;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 327;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 346;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 357;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 389;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 442;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 443;
									break;
								case 20:
									*pWeapon->FallbackPaintKit() = 515;
									break;
								case 21:
									*pWeapon->FallbackPaintKit() = 550;
									break;
								case 22:
									*pWeapon->FallbackPaintKit() = 591;
									break;
								default:
									break;
								}
							}
							break;
							case 33: // MP7
							{
								switch (MP7)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 2;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 102;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 5;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 28;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 11;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 15;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 22;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 27;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 36;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 141;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 235;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 245;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 209;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 213;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 250;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 354;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 365;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 423;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 442;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 481;
									break;
								case 20:
									*pWeapon->FallbackPaintKit() = 500;
									break;
								case 21:
									*pWeapon->FallbackPaintKit() = 536;
									break;
								default:
									break;
								}
							}
							break;
							case 34: // MP9
							{
								switch (MP9)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 482;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 27;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 33;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 100;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 39;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 61;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 148;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 141;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 199;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 329;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 262;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 366;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 368;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 386;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 403;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 448;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 549;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 609;
									break;
								default:
									break;
								}
							}
							break;
							case 35: // Nova
							{
								switch (NOVA)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 3;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 166;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 164;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 25;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 62;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 99;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 107;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 158;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 170;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 191;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 214;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 225;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 263;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 286;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 294;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 299;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 356;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 450;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 484;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 537;
									break;
								default:
									break;
								}
							}
							break;
							case 36: // P250
							{
								switch (P250)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 102;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 34;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 162;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 15;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 164;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 27;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 77;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 99;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 168;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 258;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 207;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 219;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 404;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 230;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 271;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 295;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 464;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 358;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 373;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 388;
									break;
								case 20:
									*pWeapon->FallbackPaintKit() = 426;
									break;
								case 21:
									*pWeapon->FallbackPaintKit() = 466;
									break;
								case 22:
									*pWeapon->FallbackPaintKit() = 467;
									break;
								case 23:
									*pWeapon->FallbackPaintKit() = 501;
									break;
								case 24:
									*pWeapon->FallbackPaintKit() = 551;
									break;
								default:
									break;
								}
							}
							break;
							case 38: // Scar 20
							{
								switch (SCAR20)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 165;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 100;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 46;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 70;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 116;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 157;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 196;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 232;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 391;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 298;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 312;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 406;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 453;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 502;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 518;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 597;
									break;
								default:
									break;
								}
							}
							break;
							case 39: // SG553
							{
								switch (SG553)
								{
								case 1:
									*pWeapon->FallbackPaintKit() = 39;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 98;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 410;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 347;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 287;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 298;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 363;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 378;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 487;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 519;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 553;
									break;
								default:
									break;
								}
							}
							break;
							case 40: // SSG08
							{
								switch (SSG08)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 26;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 60;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 96;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 99;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 157;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 200;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 222;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 233;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 253;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 304;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 319;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 361;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 503;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 538;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 554;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 624;
									break;
								default:
									break;
								}
							}
							break;
							case 2: // Duals
							{
								switch (DUALS)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 28;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 36;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 43;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 46;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 47;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 153;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 190;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 248;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 249;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 220;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 396;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 261;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 276;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 307;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 330;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 447;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 450;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 491;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 528;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 544;
									break;
								default:
									break;
								}
							}
							break;
							case 64: // Revolver
							{
								switch (Magnum)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 27;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 12;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 522;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 523;
									break;
								default:
									break;
								}
							}
							break;
							default:
								break;
							}



							if (pEntity->GetClientClass()->m_ClassID == (int)CSGOClassID::CKnife || pEntity->GetClientClass()->m_ClassID == (int)CSGOClassID::CEconWearable)
							{
								auto pCustomName1 = MakePtr(char*, pWeapon, 0x301C);
								if (Menu::Window.SkinchangerTab.KnifeName.getText().length() > 1)
								{
									strcpy_s(pCustomName1, 32, Menu::Window.SkinchangerTab.KnifeName.getText().c_str());
								}

								if (Model == 0) // Bayonet
								{
									auto iBayonet = *MakePtr(DWORD*, pWeapon, 0x254) = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl");
									*pWeapon->ViewModelIndex() = iBayonet;
									*pWeapon->WorldModelIndex() = iBayonet + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 500; //500

									int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

									if (Skin == 0)
									{
										*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
									}
									else if (Skin == 1)
									{
										*pWeapon->FallbackPaintKit() = 12; // Crimson Web
									}
									else if (Skin == 2)
									{
										*pWeapon->FallbackPaintKit() = 27; // Bone Mask
									}
									else if (Skin == 3)
									{
										*pWeapon->FallbackPaintKit() = 38; // Fade
									}
									else if (Skin == 4)
									{
										*pWeapon->FallbackPaintKit() = 40; // Night
									}
									else if (Skin == 5)
									{
										*pWeapon->FallbackPaintKit() = 42; // Blue Steel
									}
									else if (Skin == 6)
									{
										*pWeapon->FallbackPaintKit() = 43; // Stained
									}
									else if (Skin == 7)
									{
										*pWeapon->FallbackPaintKit() = 44; // Case Hardened
									}
									else if (Skin == 8)
									{
										*pWeapon->FallbackPaintKit() = 59; // Slaughter
									}
									else if (Skin == 9)
									{
										*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
									}
									else if (Skin == 10)
									{
										*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
									}
									else if (Skin == 11)
									{
										*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
									}
									else if (Skin == 12)
									{
										*pWeapon->FallbackPaintKit() = 143; // Urban Masked
									}
									else if (Skin == 13)
									{
										*pWeapon->FallbackPaintKit() = 175; // Scorched
									}
									else if (Skin == 14)
									{
										*pWeapon->FallbackPaintKit() = 323; // Rust Coat
									}
									else if (Skin == 15)
									{
										*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
									}
									else if (Skin == 16)
									{
										*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
									}
									else if (Skin == 17)
									{
										*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
									}
									else if (Skin == 18)
									{
										*pWeapon->FallbackPaintKit() = 413; // Marble Fade
									}
									else if (Skin == 19)
									{
										*pWeapon->FallbackPaintKit() = 414; // Rust Coat
									}
									else if (Skin == 20)
									{
										*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
									}
									else if (Skin == 21)
									{
										*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
									}
									else if (Skin == 22)
									{
										*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
									}
									else if (Skin == 23)
									{
										*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
									}
									else if (Skin == 24)
									{
										*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
									}
									else if (Skin == 25)
									{
										*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
									}
									else if (Skin == 26)
									{
										*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
									}
									else if (Skin == 27)
									{
										*pWeapon->FallbackPaintKit() = 571; // Doppler Phase 4
									}
									else if (Skin == 28)
									{
										*pWeapon->FallbackPaintKit() = 558; // Doppler Phase 4
									}
									else if (Skin == 29)
									{
										*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
									}
									else if (Skin == 30)
									{
										*pWeapon->FallbackPaintKit() = 572; // Doppler Phase 4
									}
									else if (Skin == 31)
									{
										*pWeapon->FallbackPaintKit() = 563; // Doppler Phase 4
									}
									else if (Skin == 32)
									{
										*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
									}
									else if (Skin == 33)
									{
										*pWeapon->FallbackPaintKit() = 569; // Doppler Phase 4
									}
								}
								else if (Model == 1) // Bowie Knife
								{
									auto iBowie = *MakePtr(DWORD*, pWeapon, 0x254) = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
									*pWeapon->ViewModelIndex() = iBowie;
									*pWeapon->WorldModelIndex() = iBowie + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 514;

									int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

									if (Skin == 0)
									{
										*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
									}
									else if (Skin == 1)
									{
										*pWeapon->FallbackPaintKit() = 12; // Crimson Web
									}
									else if (Skin == 2)
									{
										*pWeapon->FallbackPaintKit() = 27; // Bone Mask
									}
									else if (Skin == 3)
									{
										*pWeapon->FallbackPaintKit() = 38; // Fade
									}
									else if (Skin == 4)
									{
										*pWeapon->FallbackPaintKit() = 40; // Night
									}
									else if (Skin == 5)
									{
										*pWeapon->FallbackPaintKit() = 42; // Blue Steel
									}
									else if (Skin == 6)
									{
										*pWeapon->FallbackPaintKit() = 43; // Stained
									}
									else if (Skin == 7)
									{
										*pWeapon->FallbackPaintKit() = 44; // Case Hardened
									}
									else if (Skin == 8)
									{
										*pWeapon->FallbackPaintKit() = 59; // Slaughter
									}
									else if (Skin == 9)
									{
										*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
									}
									else if (Skin == 10)
									{
										*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
									}
									else if (Skin == 11)
									{
										*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
									}
									else if (Skin == 12)
									{
										*pWeapon->FallbackPaintKit() = 143; // Urban Masked
									}
									else if (Skin == 13)
									{
										*pWeapon->FallbackPaintKit() = 175; // Scorched
									}
									else if (Skin == 14)
									{
										*pWeapon->FallbackPaintKit() = 323; // Rust Coat
									}
									else if (Skin == 15)
									{
										*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
									}
									else if (Skin == 16)
									{
										*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
									}
									else if (Skin == 17)
									{
										*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
									}
									else if (Skin == 18)
									{
										*pWeapon->FallbackPaintKit() = 413; // Marble Fade
									}
									else if (Skin == 19)
									{
										*pWeapon->FallbackPaintKit() = 414; // Rust Coat
									}
									else if (Skin == 20)
									{
										*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
									}
									else if (Skin == 21)
									{
										*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
									}
									else if (Skin == 22)
									{
										*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
									}
									else if (Skin == 23)
									{
										*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
									}
									else if (Skin == 24)
									{
										*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
									}
									else if (Skin == 25)
									{
										*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
									}
									else if (Skin == 26)
									{
										*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
									}
									else if (Skin == 27)
									{
										*pWeapon->FallbackPaintKit() = 571; // Doppler Phase 4
									}
									else if (Skin == 28)
									{
										*pWeapon->FallbackPaintKit() = 558; // Doppler Phase 4
									}
									else if (Skin == 29)
									{
										*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
									}
									else if (Skin == 30)
									{
										*pWeapon->FallbackPaintKit() = 572; // Doppler Phase 4
									}
									else if (Skin == 31)
									{
										*pWeapon->FallbackPaintKit() = 564; // Doppler Phase 4
									}
									else if (Skin == 32)
									{
										*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
									}
									else if (Skin == 33)
									{
										*pWeapon->FallbackPaintKit() = 569; // Doppler Phase 4
									}
								}
								else if (Model == 2) // Butterfly Knife
								{
									auto iButterfly = *MakePtr(DWORD*, pWeapon, 0x254) = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
									*pWeapon->ViewModelIndex() = iButterfly;
									*pWeapon->WorldModelIndex() = iButterfly + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 515;

									int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

									if (Skin == 0)
									{
										*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
									}
									else if (Skin == 1)
									{
										*pWeapon->FallbackPaintKit() = 12; // Crimson Web
									}
									else if (Skin == 2)
									{
										*pWeapon->FallbackPaintKit() = 27; // Bone Mask
									}
									else if (Skin == 3)
									{
										*pWeapon->FallbackPaintKit() = 38; // Fade
									}
									else if (Skin == 4)
									{
										*pWeapon->FallbackPaintKit() = 40; // Night
									}
									else if (Skin == 5)
									{
										*pWeapon->FallbackPaintKit() = 42; // Blue Steel
									}
									else if (Skin == 6)
									{
										*pWeapon->FallbackPaintKit() = 43; // Stained
									}
									else if (Skin == 7)
									{
										*pWeapon->FallbackPaintKit() = 44; // Case Hardened
									}
									else if (Skin == 8)
									{
										*pWeapon->FallbackPaintKit() = 59; // Slaughter
									}
									else if (Skin == 9)
									{
										*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
									}
									else if (Skin == 10)
									{
										*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
									}
									else if (Skin == 11)
									{
										*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
									}
									else if (Skin == 12)
									{
										*pWeapon->FallbackPaintKit() = 143; // Urban Masked
									}
									else if (Skin == 13)
									{
										*pWeapon->FallbackPaintKit() = 175; // Scorched
									}
									else if (Skin == 14)
									{
										*pWeapon->FallbackPaintKit() = 323; // Rust Coat
									}
									else if (Skin == 15)
									{
										*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
									}
									else if (Skin == 16)
									{
										*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
									}
									else if (Skin == 17)
									{
										*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
									}
									else if (Skin == 18)
									{
										*pWeapon->FallbackPaintKit() = 413; // Marble Fade
									}
									else if (Skin == 19)
									{
										*pWeapon->FallbackPaintKit() = 414; // Rust Coat
									}
									else if (Skin == 20)
									{
										*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
									}
									else if (Skin == 21)
									{
										*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
									}
									else if (Skin == 22)
									{
										*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
									}
									else if (Skin == 23)
									{
										*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
									}
									else if (Skin == 24)
									{
										*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
									}
									else if (Skin == 25)
									{
										*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
									}
									else if (Skin == 26)
									{
										*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
									}
									else if (Skin == 27)
									{
										*pWeapon->FallbackPaintKit() = 571; // Doppler Phase 4
									}
									else if (Skin == 28)
									{
										*pWeapon->FallbackPaintKit() = 558; // Doppler Phase 4
									}
									else if (Skin == 29)
									{
										*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
									}
									else if (Skin == 30)
									{
										*pWeapon->FallbackPaintKit() = 572; // Doppler Phase 4
									}
									else if (Skin == 31)
									{
										*pWeapon->FallbackPaintKit() = 564; // Doppler Phase 4
									}
									else if (Skin == 32)
									{
										*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
									}
									else if (Skin == 33)
									{
										*pWeapon->FallbackPaintKit() = 569; // Doppler Phase 4
									}
								}
								else if (Model == 3) // Falchion Knife
								{
									auto iFalchion = *MakePtr(DWORD*, pWeapon, 0x254) = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
									*pWeapon->ViewModelIndex() = iFalchion;
									*pWeapon->WorldModelIndex() = iFalchion + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 512;

									int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

									if (Skin == 0)
									{
										*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
									}
									else if (Skin == 1)
									{
										*pWeapon->FallbackPaintKit() = 12; // Crimson Web
									}
									else if (Skin == 2)
									{
										*pWeapon->FallbackPaintKit() = 27; // Bone Mask
									}
									else if (Skin == 3)
									{
										*pWeapon->FallbackPaintKit() = 38; // Fade
									}
									else if (Skin == 4)
									{
										*pWeapon->FallbackPaintKit() = 40; // Night
									}
									else if (Skin == 5)
									{
										*pWeapon->FallbackPaintKit() = 42; // Blue Steel
									}
									else if (Skin == 6)
									{
										*pWeapon->FallbackPaintKit() = 43; // Stained
									}
									else if (Skin == 7)
									{
										*pWeapon->FallbackPaintKit() = 44; // Case Hardened
									}
									else if (Skin == 8)
									{
										*pWeapon->FallbackPaintKit() = 59; // Slaughter
									}
									else if (Skin == 9)
									{
										*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
									}
									else if (Skin == 10)
									{
										*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
									}
									else if (Skin == 11)
									{
										*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
									}
									else if (Skin == 12)
									{
										*pWeapon->FallbackPaintKit() = 143; // Urban Masked
									}
									else if (Skin == 13)
									{
										*pWeapon->FallbackPaintKit() = 175; // Scorched
									}
									else if (Skin == 14)
									{
										*pWeapon->FallbackPaintKit() = 323; // Rust Coat
									}
									else if (Skin == 15)
									{
										*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
									}
									else if (Skin == 16)
									{
										*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
									}
									else if (Skin == 17)
									{
										*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
									}
									else if (Skin == 18)
									{
										*pWeapon->FallbackPaintKit() = 413; // Marble Fade
									}
									else if (Skin == 19)
									{
										*pWeapon->FallbackPaintKit() = 414; // Rust Coat
									}
									else if (Skin == 20)
									{
										*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
									}
									else if (Skin == 21)
									{
										*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
									}
									else if (Skin == 22)
									{
										*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
									}
									else if (Skin == 23)
									{
										*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
									}
									else if (Skin == 24)
									{
										*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
									}
									else if (Skin == 25)
									{
										*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
									}
									else if (Skin == 26)
									{
										*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
									}
									else if (Skin == 27)
									{
										*pWeapon->FallbackPaintKit() = 571; // Doppler Phase 4
									}
									else if (Skin == 28)
									{
										*pWeapon->FallbackPaintKit() = 558; // Doppler Phase 4
									}
									else if (Skin == 29)
									{
										*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
									}
									else if (Skin == 30)
									{
										*pWeapon->FallbackPaintKit() = 572; // Doppler Phase 4
									}
									else if (Skin == 31)
									{
										*pWeapon->FallbackPaintKit() = 564; // Doppler Phase 4
									}
									else if (Skin == 32)
									{
										*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
									}
									else if (Skin == 33)
									{
										*pWeapon->FallbackPaintKit() = 569; // Doppler Phase 4
									}
								}
								else if (Model == 4) // Flip Knife
								{
									auto iFlip = *MakePtr(DWORD*, pWeapon, 0x254) = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
									*pWeapon->ViewModelIndex() = iFlip;
									*pWeapon->WorldModelIndex() = iFlip + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 505;

									int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

									if (Skin == 0)
									{
										*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
									}
									else if (Skin == 1)
									{
										*pWeapon->FallbackPaintKit() = 12; // Crimson Web
									}
									else if (Skin == 2)
									{
										*pWeapon->FallbackPaintKit() = 27; // Bone Mask
									}
									else if (Skin == 3)
									{
										*pWeapon->FallbackPaintKit() = 38; // Fade
									}
									else if (Skin == 4)
									{
										*pWeapon->FallbackPaintKit() = 40; // Night
									}
									else if (Skin == 5)
									{
										*pWeapon->FallbackPaintKit() = 42; // Blue Steel
									}
									else if (Skin == 6)
									{
										*pWeapon->FallbackPaintKit() = 43; // Stained
									}
									else if (Skin == 7)
									{
										*pWeapon->FallbackPaintKit() = 44; // Case Hardened
									}
									else if (Skin == 8)
									{
										*pWeapon->FallbackPaintKit() = 59; // Slaughter
									}
									else if (Skin == 9)
									{
										*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
									}
									else if (Skin == 10)
									{
										*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
									}
									else if (Skin == 11)
									{
										*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
									}
									else if (Skin == 12)
									{
										*pWeapon->FallbackPaintKit() = 143; // Urban Masked
									}
									else if (Skin == 13)
									{
										*pWeapon->FallbackPaintKit() = 175; // Scorched
									}
									else if (Skin == 14)
									{
										*pWeapon->FallbackPaintKit() = 323; // Rust Coat
									}
									else if (Skin == 15)
									{
										*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
									}
									else if (Skin == 16)
									{
										*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
									}
									else if (Skin == 17)
									{
										*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
									}
									else if (Skin == 18)
									{
										*pWeapon->FallbackPaintKit() = 413; // Marble Fade
									}
									else if (Skin == 19)
									{
										*pWeapon->FallbackPaintKit() = 414; // Rust Coat
									}
									else if (Skin == 20)
									{
										*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
									}
									else if (Skin == 21)
									{
										*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
									}
									else if (Skin == 22)
									{
										*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
									}
									else if (Skin == 23)
									{
										*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
									}
									else if (Skin == 24)
									{
										*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
									}
									else if (Skin == 25)
									{
										*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
									}
									else if (Skin == 26)
									{
										*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
									}
									else if (Skin == 27)
									{
										*pWeapon->FallbackPaintKit() = 571; // Doppler Phase 4
									}
									else if (Skin == 28)
									{
										*pWeapon->FallbackPaintKit() = 559; // Doppler Phase 4
									}
									else if (Skin == 29)
									{
										*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
									}
									else if (Skin == 30)
									{
										*pWeapon->FallbackPaintKit() = 572; // Doppler Phase 4
									}
									else if (Skin == 31)
									{
										*pWeapon->FallbackPaintKit() = 564; // Doppler Phase 4
									}
									else if (Skin == 32)
									{
										*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
									}
									else if (Skin == 33)
									{
										*pWeapon->FallbackPaintKit() = 569; // Doppler Phase 4
									}
								}
								else if (Model == 5) // Gut Knife
								{
									auto iGut = *MakePtr(DWORD*, pWeapon, 0x254) = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
									*pWeapon->ViewModelIndex() = iGut;
									*pWeapon->WorldModelIndex() = iGut + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 506;

									int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

									if (Skin == 0)
									{
										*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
									}
									else if (Skin == 1)
									{
										*pWeapon->FallbackPaintKit() = 12; // Crimson Web
									}
									else if (Skin == 2)
									{
										*pWeapon->FallbackPaintKit() = 27; // Bone Mask
									}
									else if (Skin == 3)
									{
										*pWeapon->FallbackPaintKit() = 38; // Fade
									}
									else if (Skin == 4)
									{
										*pWeapon->FallbackPaintKit() = 40; // Night
									}
									else if (Skin == 5)
									{
										*pWeapon->FallbackPaintKit() = 42; // Blue Steel
									}
									else if (Skin == 6)
									{
										*pWeapon->FallbackPaintKit() = 43; // Stained
									}
									else if (Skin == 7)
									{
										*pWeapon->FallbackPaintKit() = 44; // Case Hardened
									}
									else if (Skin == 8)
									{
										*pWeapon->FallbackPaintKit() = 59; // Slaughter
									}
									else if (Skin == 9)
									{
										*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
									}
									else if (Skin == 10)
									{
										*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
									}
									else if (Skin == 11)
									{
										*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
									}
									else if (Skin == 12)
									{
										*pWeapon->FallbackPaintKit() = 143; // Urban Masked
									}
									else if (Skin == 13)
									{
										*pWeapon->FallbackPaintKit() = 175; // Scorched
									}
									else if (Skin == 14)
									{
										*pWeapon->FallbackPaintKit() = 323; // Rust Coat
									}
									else if (Skin == 15)
									{
										*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
									}
									else if (Skin == 16)
									{
										*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
									}
									else if (Skin == 17)
									{
										*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
									}
									else if (Skin == 18)
									{
										*pWeapon->FallbackPaintKit() = 413; // Marble Fade
									}
									else if (Skin == 19)
									{
										*pWeapon->FallbackPaintKit() = 414; // Rust Coat
									}
									else if (Skin == 20)
									{
										*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
									}
									else if (Skin == 21)
									{
										*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
									}
									else if (Skin == 22)
									{
										*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
									}
									else if (Skin == 23)
									{
										*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
									}
									else if (Skin == 24)
									{
										*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
									}
									else if (Skin == 25)
									{
										*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
									}
									else if (Skin == 26)
									{
										*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
									}
									else if (Skin == 27)
									{
										*pWeapon->FallbackPaintKit() = 571; // Doppler Phase 4
									}
									else if (Skin == 28)
									{
										*pWeapon->FallbackPaintKit() = 560; // Doppler Phase 4
									}
									else if (Skin == 29)
									{
										*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
									}
									else if (Skin == 30)
									{
										*pWeapon->FallbackPaintKit() = 572; // Doppler Phase 4
									}
									else if (Skin == 31)
									{
										*pWeapon->FallbackPaintKit() = 564; // Doppler Phase 4
									}
									else if (Skin == 32)
									{
										*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
									}
									else if (Skin == 33)
									{
										*pWeapon->FallbackPaintKit() = 569; // Doppler Phase 4
									}
								}
								else if (Model == 6) // Huntsman Knife
								{
									auto iHuntsman = *MakePtr(DWORD*, pWeapon, 0x254) = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
									*pWeapon->ViewModelIndex() = iHuntsman;
									*pWeapon->WorldModelIndex() = iHuntsman + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 509;

									int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

									if (Skin == 0)
									{
										*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
									}
									else if (Skin == 1)
									{
										*pWeapon->FallbackPaintKit() = 12; // Crimson Web
									}
									else if (Skin == 2)
									{
										*pWeapon->FallbackPaintKit() = 27; // Bone Mask
									}
									else if (Skin == 3)
									{
										*pWeapon->FallbackPaintKit() = 38; // Fade
									}
									else if (Skin == 4)
									{
										*pWeapon->FallbackPaintKit() = 40; // Night
									}
									else if (Skin == 5)
									{
										*pWeapon->FallbackPaintKit() = 42; // Blue Steel
									}
									else if (Skin == 6)
									{
										*pWeapon->FallbackPaintKit() = 43; // Stained
									}
									else if (Skin == 7)
									{
										*pWeapon->FallbackPaintKit() = 44; // Case Hardened
									}
									else if (Skin == 8)
									{
										*pWeapon->FallbackPaintKit() = 59; // Slaughter
									}
									else if (Skin == 9)
									{
										*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
									}
									else if (Skin == 10)
									{
										*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
									}
									else if (Skin == 11)
									{
										*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
									}
									else if (Skin == 12)
									{
										*pWeapon->FallbackPaintKit() = 143; // Urban Masked
									}
									else if (Skin == 13)
									{
										*pWeapon->FallbackPaintKit() = 175; // Scorched
									}
									else if (Skin == 14)
									{
										*pWeapon->FallbackPaintKit() = 323; // Rust Coat
									}
									else if (Skin == 15)
									{
										*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
									}
									else if (Skin == 16)
									{
										*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
									}
									else if (Skin == 17)
									{
										*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
									}
									else if (Skin == 18)
									{
										*pWeapon->FallbackPaintKit() = 413; // Marble Fade
									}
									else if (Skin == 19)
									{
										*pWeapon->FallbackPaintKit() = 414; // Rust Coat
									}
									else if (Skin == 20)
									{
										*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
									}
									else if (Skin == 21)
									{
										*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
									}
									else if (Skin == 22)
									{
										*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
									}
									else if (Skin == 23)
									{
										*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
									}
									else if (Skin == 24)
									{
										*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
									}
									else if (Skin == 25)
									{
										*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
									}
									else if (Skin == 26)
									{
										*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
									}
									else if (Skin == 27)
									{
										*pWeapon->FallbackPaintKit() = 571; // Doppler Phase 4
									}
									else if (Skin == 28)
									{
										*pWeapon->FallbackPaintKit() = 558; // Doppler Phase 4
									}
									else if (Skin == 29)
									{
										*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
									}
									else if (Skin == 30)
									{
										*pWeapon->FallbackPaintKit() = 572; // Doppler Phase 4
									}
									else if (Skin == 31)
									{
										*pWeapon->FallbackPaintKit() = 564; // Doppler Phase 4
									}
									else if (Skin == 32)
									{
										*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
									}
									else if (Skin == 33)
									{
										*pWeapon->FallbackPaintKit() = 569; // Doppler Phase 4
									}
								}
								else if (Model == 7) // Karambit
								{
									auto iKarambit = *MakePtr(DWORD*, pWeapon, 0x254) = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
									*pWeapon->ViewModelIndex() = iKarambit;
									*pWeapon->WorldModelIndex() = iKarambit + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 507;

									int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

									if (Skin == 0)
									{
										*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
									}
									else if (Skin == 1)
									{
										*pWeapon->FallbackPaintKit() = 12; // Crimson Web
									}
									else if (Skin == 2)
									{
										*pWeapon->FallbackPaintKit() = 27; // Bone Mask
									}
									else if (Skin == 3)
									{
										*pWeapon->FallbackPaintKit() = 38; // Fade
									}
									else if (Skin == 4)
									{
										*pWeapon->FallbackPaintKit() = 40; // Night
									}
									else if (Skin == 5)
									{
										*pWeapon->FallbackPaintKit() = 42; // Blue Steel
									}
									else if (Skin == 6)
									{
										*pWeapon->FallbackPaintKit() = 43; // Stained
									}
									else if (Skin == 7)
									{
										*pWeapon->FallbackPaintKit() = 44; // Case Hardened
									}
									else if (Skin == 8)
									{
										*pWeapon->FallbackPaintKit() = 59; // Slaughter
									}
									else if (Skin == 9)
									{
										*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
									}
									else if (Skin == 10)
									{
										*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
									}
									else if (Skin == 11)
									{
										*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
									}
									else if (Skin == 12)
									{
										*pWeapon->FallbackPaintKit() = 143; // Urban Masked
									}
									else if (Skin == 13)
									{
										*pWeapon->FallbackPaintKit() = 175; // Scorched
									}
									else if (Skin == 14)
									{
										*pWeapon->FallbackPaintKit() = 323; // Rust Coat
									}
									else if (Skin == 15)
									{
										*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
									}
									else if (Skin == 16)
									{
										*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
									}
									else if (Skin == 17)
									{
										*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
									}
									else if (Skin == 18)
									{
										*pWeapon->FallbackPaintKit() = 413; // Marble Fade
									}
									else if (Skin == 19)
									{
										*pWeapon->FallbackPaintKit() = 414; // Rust Coat
									}
									else if (Skin == 20)
									{
										*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
									}
									else if (Skin == 21)
									{
										*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
									}
									else if (Skin == 22)
									{
										*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
									}
									else if (Skin == 23)
									{
										*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
									}
									else if (Skin == 24)
									{
										*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
									}
									else if (Skin == 25)
									{
										*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
									}
									else if (Skin == 26)
									{
										*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
									}
									else if (Skin == 27)
									{
										*pWeapon->FallbackPaintKit() = 571; // Doppler Phase 4
									}
									else if (Skin == 28)
									{
										*pWeapon->FallbackPaintKit() = 561; // Doppler Phase 4
									}
									else if (Skin == 29)
									{
										*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
									}
									else if (Skin == 30)
									{
										*pWeapon->FallbackPaintKit() = 572; // Doppler Phase 4
									}
									else if (Skin == 31)
									{
										*pWeapon->FallbackPaintKit() = 564; // Doppler Phase 4
									}
									else if (Skin == 32)
									{
										*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
									}
									else if (Skin == 33)
									{
										*pWeapon->FallbackPaintKit() = 569; // Doppler Phase 4
									}
								}
								else if (Model == 8) // M9 Bayonet
								{
									auto iM9Bayonet = *MakePtr(DWORD*, pWeapon, 0x254) = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
									*pWeapon->ViewModelIndex() = iM9Bayonet;
									*pWeapon->WorldModelIndex() = iM9Bayonet + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 508;

									int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

									if (Skin == 0)
									{
										*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
									}
									else if (Skin == 1)
									{
										*pWeapon->FallbackPaintKit() = 12; // Crimson Web
									}
									else if (Skin == 2)
									{
										*pWeapon->FallbackPaintKit() = 27; // Bone Mask
									}
									else if (Skin == 3)
									{
										*pWeapon->FallbackPaintKit() = 38; // Fade
									}
									else if (Skin == 4)
									{
										*pWeapon->FallbackPaintKit() = 40; // Night
									}
									else if (Skin == 5)
									{
										*pWeapon->FallbackPaintKit() = 42; // Blue Steel
									}
									else if (Skin == 6)
									{
										*pWeapon->FallbackPaintKit() = 43; // Stained
									}
									else if (Skin == 7)
									{
										*pWeapon->FallbackPaintKit() = 44; // Case Hardened
									}
									else if (Skin == 8)
									{
										*pWeapon->FallbackPaintKit() = 59; // Slaughter
									}
									else if (Skin == 9)
									{
										*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
									}
									else if (Skin == 10)
									{
										*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
									}
									else if (Skin == 11)
									{
										*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
									}
									else if (Skin == 12)
									{
										*pWeapon->FallbackPaintKit() = 143; // Urban Masked
									}
									else if (Skin == 13)
									{
										*pWeapon->FallbackPaintKit() = 175; // Scorched
									}
									else if (Skin == 14)
									{
										*pWeapon->FallbackPaintKit() = 323; // Rust Coat
									}
									else if (Skin == 15)
									{
										*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
									}
									else if (Skin == 16)
									{
										*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
									}
									else if (Skin == 17)
									{
										*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
									}
									else if (Skin == 18)
									{
										*pWeapon->FallbackPaintKit() = 413; // Marble Fade
									}
									else if (Skin == 19)
									{
										*pWeapon->FallbackPaintKit() = 414; // Rust Coat
									}
									else if (Skin == 20)
									{
										*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
									}
									else if (Skin == 21)
									{
										*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
									}
									else if (Skin == 22)
									{
										*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
									}
									else if (Skin == 23)
									{
										*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
									}
									else if (Skin == 24)
									{
										*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
									}
									else if (Skin == 25)
									{
										*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
									}
									else if (Skin == 26)
									{
										*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
									}
									else if (Skin == 27)
									{
										*pWeapon->FallbackPaintKit() = 571; // Doppler Phase 4
									}
									else if (Skin == 28)
									{
										*pWeapon->FallbackPaintKit() = 562; // Doppler Phase 4
									}
									else if (Skin == 29)
									{
										*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
									}
									else if (Skin == 30)
									{
										*pWeapon->FallbackPaintKit() = 572; // Doppler Phase 4
									}
									else if (Skin == 31)
									{
										*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
									}
									else if (Skin == 32)
									{
										*pWeapon->FallbackPaintKit() = 569; // Doppler Phase 4
									}
									else if (Skin == 33)
									{
										*pWeapon->FallbackPaintKit() = 569; // Doppler Phase 4
									}
								}
								else if (Model == 9) // Shadow Daggers
								{
									auto iDagger = *MakePtr(DWORD*, pWeapon, 0x254) = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
									*pWeapon->ViewModelIndex() = iDagger;
									*pWeapon->WorldModelIndex() = iDagger + 1;

									*pWeapon->OwnerXuidLow() = 0;
									*pWeapon->OwnerXuidHigh() = 0;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 516;

									int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

									if (Skin == 0)
									{
										*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
									}
									else if (Skin == 1)
									{
										*pWeapon->FallbackPaintKit() = 12; // Crimson Web
									}
									else if (Skin == 2)
									{
										*pWeapon->FallbackPaintKit() = 27; // Bone Mask
									}
									else if (Skin == 3)
									{
										*pWeapon->FallbackPaintKit() = 38; // Fade
									}
									else if (Skin == 4)
									{
										*pWeapon->FallbackPaintKit() = 40; // Night
									}
									else if (Skin == 5)
									{
										*pWeapon->FallbackPaintKit() = 42; // Blue Steel
									}
									else if (Skin == 6)
									{
										*pWeapon->FallbackPaintKit() = 43; // Stained
									}
									else if (Skin == 7)
									{
										*pWeapon->FallbackPaintKit() = 44; // Case Hardened
									}
									else if (Skin == 8)
									{
										*pWeapon->FallbackPaintKit() = 59; // Slaughter
									}
									else if (Skin == 9)
									{
										*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
									}
									else if (Skin == 10)
									{
										*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
									}
									else if (Skin == 11)
									{
										*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
									}
									else if (Skin == 12)
									{
										*pWeapon->FallbackPaintKit() = 143; // Urban Masked
									}
									else if (Skin == 13)
									{
										*pWeapon->FallbackPaintKit() = 175; // Scorched
									}
									else if (Skin == 14)
									{
										*pWeapon->FallbackPaintKit() = 323; // Rust Coat
									}
									else if (Skin == 15)
									{
										*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
									}
									else if (Skin == 16)
									{
										*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
									}
									else if (Skin == 17)
									{
										*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
									}
									else if (Skin == 18)
									{
										*pWeapon->FallbackPaintKit() = 413; // Marble Fade
									}
									else if (Skin == 19)
									{
										*pWeapon->FallbackPaintKit() = 414; // Rust Coat
									}
									else if (Skin == 20)
									{
										*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
									}
									else if (Skin == 21)
									{
										*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
									}
									else if (Skin == 22)
									{
										*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
									}
									else if (Skin == 23)
									{
										*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
									}
									else if (Skin == 24)
									{
										*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
									}
									else if (Skin == 25)
									{
										*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
									}
									else if (Skin == 26)
									{
										*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
									}
									else if (Skin == 27)
									{
										*pWeapon->FallbackPaintKit() = 571; // Doppler Phase 4
									}
									else if (Skin == 28)
									{
										*pWeapon->FallbackPaintKit() = 561; // Doppler Phase 4
									}
									else if (Skin == 29)
									{
										*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
									}
									else if (Skin == 30)
									{
										*pWeapon->FallbackPaintKit() = 572; // Doppler Phase 4
									}
									else if (Skin == 31)
									{
										*pWeapon->FallbackPaintKit() = 564; // Doppler Phase 4
									}
									else if (Skin == 32)
									{
										*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
									}
									else if (Skin == 33)
									{
										*pWeapon->FallbackPaintKit() = 569; // Doppler Phase 4
									}
								}
							}

							*pWeapon->OwnerXuidLow() = 0;
							*pWeapon->OwnerXuidHigh() = 0;
							*pWeapon->FallbackWear() = 0.001f;
							*pWeapon->m_AttributeManager()->m_Item()->ItemIDHigh() = 1;

						}
					}
				}

			}
		}
		if (pLocal->IsAlive())
			R::Resolve();
	}
	if (curStage == FRAME_RENDER_START)
	{
		IClientEntity* pLocal1 = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && pLocal1 && pLocal1->IsAlive())
		{
			IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		//	if (*(bool*)((DWORD)Interfaces::pInput + 0xA5))
			//{
				if (Menu::Window.RageBotTab.Thirdaa.GetIndex() > 0)
				{
					*(Vector*)((DWORD)pLocal + 0x31C4 + 4) = aathird;
				}
			//}
		}
		if (Menu::Window.RageBotTab.HitBoxFix.GetState())
			for (int i = 1; i <= Interfaces::Globals->maxClients; i++)
			{
				if (i == Interfaces::Engine->GetLocalPlayer()) continue;

				IClientEntity* pCurEntity = Interfaces::EntList->GetClientEntity(i);
				if (!pCurEntity) continue;

				*(int*)((uintptr_t)pCurEntity + 0xA30) = Interfaces::Globals->framecount; //we'll skip occlusion checks now
				*(int*)((uintptr_t)pCurEntity + 0xA28) = 0;//clear occlusion flags
			}
		
		
	
	}
	oFrameStageNotify(curStage);
}




void f12aggo48489489t11891985156()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}

void f12aggot1178978978979816516151651658918918951186()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}

void f12agg18978978923165156o189819t15151987986()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12189189189aggot1thhfhfg5156741894()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot1189189drhdgdfgfd1895156489489489()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12a987987ggotsgysdgdf151564894894984984984()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggo789789798894165t17897987951513131313136()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12ag7987897got16516511564564895156131313189798()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12ag98gorgdgdgsdgt148948951589767898798797897897984151()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot897987987sdgsetgerger1515698798gerger16351687987()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12a7gdfgsdfhrthrt89789798ggot151561hrthrtf568979812313()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggo1sdgsdrgrhrthrth561651t157897987897156897()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12a7987987ggot1515678979ygdgersgrsg7854646597()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot15srhgdgdfhrt123612015678978978979789789()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot15115616hsthsrthr54564564578978978979878978971111564136()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12156165shrthrtshfhg1aggot15156417421784197219718271()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f124984894aggot1515641142thshthgfgf16792()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12418949849thshthrhrth8aggot151562713879124819421718972189()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot15156108979874218gdfgdfgrthrth971279812412()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot151894984151dfhdfhfggjg421741729296()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12agg84998fjgzjhfgbf4ot15156189198()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot148949dhtgghnb8515cf489498bbhhfgh6()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot1515489fjhfhkjdhfh489489hhtdffghh1rt5616()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}
void f12aggot151erghedjkgffgh489489r5g9dgdf98df1g56()
{
	float asfnefinweifuwnefiuwen = 123;
	asfnefinweifuwnefiuwen = 175;
	if (asfnefinweifuwnefiuwen = 175)
		asfnefinweifuwnefiuwen = 185;
	asfnefinweifuwnefiuwen = 175;

}

void hookedurmum()
{
	float urmum = 123;
	int kokot = 1561651;
	int ts;
	ts = urmum + kokot;
}