#pragma once
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_internal.h"
#include "ImGUI/DX9/imgui_impl_dx9.h"

int screenWidth, screenHeight;
ImDrawData			_drawData;
ImDrawList*         _drawList;
IDirect3DTexture9*  _texture;
ImFontAtlas         _fonts;

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include "CVisuals.h"
#include "CMenu.h"

namespace CRender
{
	void __stdcall CreateObjects(IDirect3DDevice9* pDevice)
	{
		_drawList = new ImDrawList();

		auto font_path = std::string{};

		uint8_t* pixel_data;

		int width,
			height,
			bytes_per_pixel;
		
		if (!get_system_font_path(/*"Verdana (TrueType)"*//*"Segoe UI (TrueType)"*//*"Calibri"*/"AA American Captain", font_path)) return;

		auto font = _fonts.AddFontFromFileTTF(font_path.data(), 14.0f, 0, _fonts.GetGlyphRangesDefault());

		_fonts.GetTexDataAsRGBA32(&pixel_data, &width, &height, &bytes_per_pixel);
		//printf("bytes_per_pixel: %d\n", bytes_per_pixel);

		auto hr = pDevice->CreateTexture(
			width, height,
			1,
			D3DUSAGE_DYNAMIC,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&_texture,
			NULL);

		if (FAILED(hr)) return;

		D3DLOCKED_RECT tex_locked_rect;
		if (_texture->LockRect(0, &tex_locked_rect, NULL, 0) != D3D_OK)
			return;
		for (int y = 0; y < height; y++)
			memcpy((uint8_t*)tex_locked_rect.pBits + tex_locked_rect.Pitch * y, pixel_data + (width * bytes_per_pixel) * y, (width * bytes_per_pixel));
		_texture->UnlockRect(0);

		_fonts.TexID = _texture;
	}

	void __stdcall InvalidateObjects()
	{
		if (_texture) _texture->Release();
		_texture = nullptr;

		_fonts.Clear();

		if (_drawList)
			delete _drawList;
		_drawList = nullptr;
	}

	void __stdcall GUI_Init(IDirect3DDevice9* pDevice)
	{
		ImGui_ImplDX9_Init(g_hWnd, pDevice);

		

		ImColor a1 = ImColor(255, 0, 43, 0);
		ImColor mainColor = ImColor(33, 33, 33, 255);
		ImColor bodyColor = ImColor(20, 20, 20, 255);
		ImColor f = ImColor(255, 255, 255, 255);
		ImColor f1 = ImColor(255, 115, 0, 255);

		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4 mainColorHovered = ImVec4(mainColor.Value.x + 0.1f, mainColor.Value.y + 0.1f, mainColor.Value.z + 0.1f, mainColor.Value.w);
		ImVec4 mainColorActive = ImVec4(mainColor.Value.x + 0.2f, mainColor.Value.y + 0.2f, mainColor.Value.z + 0.2f, mainColor.Value.w);
		ImVec4 menubarColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w - 0.8f);
		ImVec4 frameBgColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w + .1f);
		ImVec4 tooltipBgColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w + .05f);



		style.Colors[ImGuiCol_Text] = f;
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = bodyColor;
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(.0f, .0f, .0f, .0f);
		style.Colors[ImGuiCol_PopupBg] = tooltipBgColor;
		style.Colors[ImGuiCol_Border] = mainColor;
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = frameBgColor;
		style.Colors[ImGuiCol_FrameBgHovered] = mainColorHovered;
		style.Colors[ImGuiCol_FrameBgActive] = mainColorActive;
		style.Colors[ImGuiCol_TitleBg] = f1;
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = f1;
		style.Colors[ImGuiCol_MenuBarBg] = menubarColor;
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(frameBgColor.x + .05f, frameBgColor.y + .05f, frameBgColor.z + .05f, frameBgColor.w);
		style.Colors[ImGuiCol_ScrollbarGrab] = mainColor;
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = mainColorHovered;
		style.Colors[ImGuiCol_ScrollbarGrabActive] = mainColorActive;
		style.Colors[ImGuiCol_ComboBg] = frameBgColor;
		style.Colors[ImGuiCol_CheckMark] = mainColor;
		style.Colors[ImGuiCol_SliderGrab] = mainColorHovered;
		style.Colors[ImGuiCol_SliderGrabActive] = mainColorActive;
		style.Colors[ImGuiCol_Button] = mainColor;
		style.Colors[ImGuiCol_ButtonHovered] = mainColorHovered;
		style.Colors[ImGuiCol_ButtonActive] = mainColorActive;
		style.Colors[ImGuiCol_Header] = mainColor;
		style.Colors[ImGuiCol_HeaderHovered] = mainColorHovered;
		style.Colors[ImGuiCol_HeaderActive] = mainColorActive;
		style.Colors[ImGuiCol_Column] = mainColor;
		style.Colors[ImGuiCol_ColumnHovered] = mainColorHovered;
		style.Colors[ImGuiCol_ColumnActive] = mainColorActive;
		style.Colors[ImGuiCol_ResizeGrip] = mainColor;
		style.Colors[ImGuiCol_ResizeGripHovered] = mainColorHovered;
		style.Colors[ImGuiCol_ResizeGripActive] = mainColorActive;
		style.Colors[ImGuiCol_CloseButton] = mainColor;
		style.Colors[ImGuiCol_CloseButtonHovered] = mainColorHovered;
		style.Colors[ImGuiCol_CloseButtonActive] = mainColorActive;
		style.Colors[ImGuiCol_PlotLines] = mainColor;
		style.Colors[ImGuiCol_PlotLinesHovered] = mainColorHovered;
		style.Colors[ImGuiCol_PlotHistogram] = mainColor;
		style.Colors[ImGuiCol_PlotHistogramHovered] = mainColorHovered;
		style.Colors[ImGuiCol_TextSelectedBg] = mainColorHovered;
		style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.0f, 0.0f, 0.0f, 0.75f);
		style.Colors[ImGuiCol_Tab] = a1;
		style.Colors[ImGuiCol_TabActive] = a1;
		style.Colors[ImGuiCol_TabHovered] = a1;
		style.Colors[ImGuiCol_TabSelected] = a1;
		style.Colors[ImGuiCol_TabText] = ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
		style.Colors[ImGuiCol_TabTextActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = bodyColor;
		style.Colors[ImGuiCol_CloseButton] = ImVec4(0.82f, 0.12f, 0.24f, 0.81f);
		style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.72f, 0.0f, 0.9f, 0.66f);
		style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
		
		CreateObjects(pDevice);

		G::Inited = true;
	}

	void __stdcall PreRender(IDirect3DDevice9* device)
	{
		if (!G::Inited)
			GUI_Init(device);

		ImGui_ImplDX9_NewFrame();
		_drawData.Valid = false;
		_drawList->Clear();
		_drawList->PushClipRectFullScreen();
	}

	void __stdcall PostRender(IDirect3DDevice9* deivce)
	{
		if (!_drawList->VtxBuffer.empty()) {
			_drawData.Valid = true;
			_drawData.CmdLists = &_drawList;
			_drawData.CmdListsCount = 1;
			_drawData.TotalVtxCount = _drawList->VtxBuffer.Size;
			_drawData.TotalIdxCount = _drawList->IdxBuffer.Size;
		}
		
		ImGui_ImplDX9_RenderDrawLists(&_drawData);
		ImGui::Render();
	}

	long __stdcall Present(IDirect3DDevice9* device, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion)
	{
		PreRender(device);

		// Rendering

		Interface.Engine->GetScreenSize(screenWidth, screenHeight);
		CMenu::Render();
		CVisuals::Render();

		// End rendering

		PostRender(device);

		return O::Present(device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	}

	long __stdcall Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		if (!G::Inited || H::isEjecting)
			return O::Reset(pDevice, pPresentationParameters);

		ImGui_ImplDX9_InvalidateDeviceObjects();
		InvalidateObjects();

		auto hr = O::Reset(pDevice, pPresentationParameters);

		CreateObjects(pDevice);
		ImGui_ImplDX9_CreateDeviceObjects();
		
		return hr;
	}
};