

#include "GUI.h"

#include "RenderManager.h"

#include <algorithm>
#include "tinyxml2.h"
#include "Controls.h"
//#include "input.h"
#include "Menu.h"

CGUI GUI;
CSkinChanger *Radar = new CSkinChanger;
static float koloreklul;
bool SaveFile = false;
bool LoadFile = false;

CGUI::CGUI()
{

}

// Draws all windows 
void CGUI::Draw()
{
	int red1 = Menu::Window.ColorsTab.MenuR.GetValue();
	int green2 = Menu::Window.ColorsTab.MenuG.GetValue();
	int blue3 = Menu::Window.ColorsTab.MenuB.GetValue();
	bool ShouldDrawCursor = false;

	for (auto window : Windows)
	{
		if (window->m_bIsOpen)
		{
			ShouldDrawCursor = true;
			DrawWindow(window);

		}

	}

	if (ShouldDrawCursor)
	{
		static Vertex_t MouseVt[3];

#define UI_CURSORSIZE		12

		MouseVt[0].Init(Vector2D(Mouse.x, Mouse.y));
		MouseVt[1].Init(Vector2D(Mouse.x + UI_CURSORSIZE, Mouse.y));
		MouseVt[2].Init(Vector2D(Mouse.x, Mouse.y + UI_CURSORSIZE));

		Render::PolygonOutline(3, MouseVt, Color(red1, green2, blue3, 230), Color(0, 0, 0, 240));
	}
}

// Handle all input etc
void CGUI::Update()
{

	static int bWasntHolding = false;
	static int bGrabbing = false;
	static int iOffsetX = 0, iOffsetY = 0;
	//Key Array
	std::copy(keys, keys + 255, oldKeys);
	for (int x = 0; x < 255; x++)
	{
		//oldKeys[x] = oldKeys[x] & keys[x];
		keys[x] = (GetAsyncKeyState(x));
	}

	// Mouse Location
	POINT mp; GetCursorPos(&mp);
	Mouse.x = mp.x; Mouse.y = mp.y;

	RECT Screen = Render::GetViewport();

	// Window Binds
	for (auto& bind : WindowBinds)
	{
		if (GetKeyPress(bind.first))
		{
			bind.second->Toggle();
		}
	}

	// Stop dragging
	if (IsDragging && !GetKeyState(VK_LBUTTON))
	{
		IsDragging = false;
		DraggingWindow = nullptr;
		DraggingRadar = nullptr;

	}

	// If we are in the proccess of dragging a window
	if (IsDragging && GetKeyState(VK_LBUTTON) && !GetKeyPress(VK_LBUTTON))
	{
		if (DraggingWindow)
		{
			DraggingWindow->m_x = Mouse.x - DragOffsetX;
			DraggingWindow->m_y = Mouse.y - DragOffsetY;
		}
		if (DraggingRadar)
		{
			DraggingRadar->m_x = Mouse.x - DragOffsetX;
			Menu::Window.VisualsTab.OtherRadarXPosition.SetValue(DraggingRadar->m_x);

			DraggingRadar->m_y = Mouse.y - DragOffsetY;
			Menu::Window.VisualsTab.OtherRadarYPosition.SetValue(DraggingRadar->m_y);
		}
	}
	if (Menu::Window.VisualsTab.OtherRadar.GetState()) {
		if (GetKeyPress(VK_LBUTTON)) {
			if (IsMouseInRegion(Radar->m_x, Radar->m_y, Radar->m_x + Radar->m_iWidth, Radar->m_y + Radar->m_iHeight)) {
				if (IsMouseInRegion(Radar->m_x, Radar->m_y, Radar->m_x + Radar->m_iWidth, Radar->m_y + 28)) {
					IsDragging = true;

					DraggingWindow = nullptr;
					DraggingRadar = Radar;

					DragOffsetX = Mouse.x - Radar->m_x;
					DragOffsetY = Mouse.y - Radar->m_y;
				}
			}
		}
	}

	//bWasntHolding = Input->Hovering(x, y, width, 28) && !GetAsyncKeyState(VK_LBUTTON);

	// Process some windows
	for (auto window : Windows)
	{
		if (window->m_bIsOpen)
		{

			koloreklul += 0.005f;
			if (koloreklul > 1.f) koloreklul = 0.f;
			//	window->m_x = window->m_x - 5;
			// Used to tell the widget processing that there could be a click
			bool bCheckWidgetClicks = false;

			// If the user clicks inside the window
			if (GetKeyPress(VK_LBUTTON))
			{
				//	window->m_iHeight = window->m_iHeight - 5; For ZeusCheat Shit
				// Is it inside the client area?
				if (IsMouseInRegion(window->GetClientArea1()))
				{
					// User is selecting a new tab
					if (IsMouseInRegion(window->GetTabArea()))
					{
						bCheckWidgetClicks = true;

						int iTab = 0;
						int TabCount = window->Tabs.size();
						if (TabCount) // If there are some tabs
						{
							int TabSize = 100;
							int Dist = Mouse.y - (window->m_y + 50 + UI_WIN_TITLEHEIGHT + UI_WIN_TOPHEIGHT);
							if (Dist < (TabSize * TabCount))
							{
								while (Dist > TabSize)
								{
									if (Dist > TabSize)
									{
										iTab++;
										Dist -= TabSize;
									}
									if (iTab == (TabCount - 1))
									{
										break;
									}
								}
								window->SelectedTab = window->Tabs[iTab];

								// Loose focus on the control
								bCheckWidgetClicks = false;
								window->IsFocusingControl = false;
								window->FocusedControl = nullptr;
							}
						}

					}

					else
						bCheckWidgetClicks = true;
				}
				else if (IsMouseInRegion(window->m_x, window->m_y, window->m_x + window->m_iWidth, window->m_y + window->m_iHeight))
					
				{
					// Must be in the around the title or side of the window
					// So we assume the user is trying to drag the window
					IsDragging = true;
					//
					DraggingWindow = window;
					DraggingRadar = nullptr;

					DragOffsetX = Mouse.x - window->m_x;
					DragOffsetY = Mouse.y - window->m_y;

					// Loose focus on the control
					window->IsFocusingControl = false;
					window->FocusedControl = nullptr;
				}

				else
				{
					// Loose focus on the control
					window->IsFocusingControl = false;
					window->FocusedControl = nullptr;
				}
			}


			// Controls 
			if (window->SelectedTab != nullptr)
			{
				// Focused widget
				bool SkipWidget = false;
				CControl* SkipMe = nullptr;

				// this window is focusing on a widget??
				if (window->IsFocusingControl)
				{
					if (window->FocusedControl != nullptr)
					{
						// We've processed it once, skip it later
						SkipWidget = true;
						SkipMe = window->FocusedControl;

						POINT cAbs = window->FocusedControl->GetAbsolutePos();
						RECT controlRect = { cAbs.x, cAbs.y, window->FocusedControl->m_iWidth, window->FocusedControl->m_iHeight };
						window->FocusedControl->OnUpdate();

						if (window->FocusedControl->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
						{
							window->FocusedControl->OnClick();

							// If it gets clicked we loose focus
							window->IsFocusingControl = false;
							window->FocusedControl = nullptr;
							bCheckWidgetClicks = false;
						}
					}
				}

				// Itterate over the rest of the control
				for (auto control : window->SelectedTab->Controls)
				{
					if (control != nullptr)
					{
						if (SkipWidget && SkipMe == control)
							continue;

						POINT cAbs = control->GetAbsolutePos();
						RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
						control->OnUpdate();

						if (control->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
						{
							control->OnClick();
							bCheckWidgetClicks = false;

							// Change of focus
							if (control->Flag(UIFlags::UI_Focusable))
							{
								window->IsFocusingControl = true;
								window->FocusedControl = control;
							}
							else
							{
								window->IsFocusingControl = false;
								window->FocusedControl = nullptr;
							}

						}
					}
				}

				// We must have clicked whitespace
				if (bCheckWidgetClicks)
				{
					// Loose focus on the control
					window->IsFocusingControl = false;
					window->FocusedControl = nullptr;
				}
			}
		}
	}
}

// Returns 
bool CGUI::GetKeyPress(unsigned int key)
{
	if (keys[key] == true && oldKeys[key] == false)
		return true;
	else
		return false;
}

bool CGUI::GetKeyState(unsigned int key)
{
	return keys[key];
}

bool CGUI::IsMouseInRegion(int x, int y, int x2, int y2)
{
	if (Mouse.x > x && Mouse.y > y && Mouse.x < x2 && Mouse.y < y2)
		return true;
	else
		return false;
}

bool CGUI::IsMouseInRegion(RECT region)
{
	return IsMouseInRegion(region.left, region.top, region.left + region.right, region.top + region.bottom);
}

POINT CGUI::GetMouse()
{
	return Mouse;
}
void DrawRect(int x, int y, int w, int h, Color col)
{
	Interfaces::Surface->DrawSetColor(col);
	Interfaces::Surface->DrawFilledRect(x, y, x + w, y + h);
}


void DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow)
{
	Color colColor(0, 0, 0);

	flRainbow += flSpeed;
	if (flRainbow > 1.f) flRainbow = 0.f;

	for (int i = 0; i < width; i++)
	{
		float hue = (1.f / (float)width) * i;
		hue -= flRainbow;
		if (hue < 0.f) hue += 1.f;

		Color colRainbow = colColor.FromHSB(hue, 1.f, 1.f);
		DrawRect(x + i, y, 1, height, colRainbow);
	}
}
bool CGUI::DrawWindow(CWindow* window)
{
	int red = Menu::Window.ColorsTab.MenuR.GetValue();
	int green = Menu::Window.ColorsTab.MenuG.GetValue();
	int blue = Menu::Window.ColorsTab.MenuB.GetValue();

	int redtitle = Menu::Window.ColorsTab.MenutitleR.GetValue();
	int greentitle = Menu::Window.ColorsTab.MenutitleG.GetValue();
	int bluetitle = Menu::Window.ColorsTab.MenutitleB.GetValue();
	
	



	//Render::Outline(window->m_x, window->m_y, window->m_iWidth, window->m_iHeight, Color::FromHSB(koloreklul, 1.f, 1.f));
	//Render::GradientV(window->m_x + 2, window->m_y + 2, window->m_iWidth - 4, 26, Color::FromHSB(koloreklul, 1.f, 1.f), Color::FromHSB(koloreklul, 1.f, 1.f));
	//Render::Clear(window->m_x + 2, window->m_y + 2 + 26, window->m_iWidth - 4, window->m_iHeight - 4 - 26, Color::FromHSB(koloreklul, 1.f, 1.f));
	//Render::Outline(window->m_x + 1, window->m_y + 1, window->m_iWidth - 2, window->m_iHeight - 2, Color(red, green, blue, 255));
	//Render::Text(window->m_x + 350, window->m_y + 8, Color::FromHSB(koloreklul, 1.f, 1.f), Render::Fonts::MenuBold, window->Title.c_str());
	

	//Obramówka
	//Render::Clear(window->m_x + 8, window->m_y + 1 + 27, window->m_iWidth - 4 - 12, window->m_iHeight - 2 - 8 - 26, Color(50, 150, 225, 255));
	//Render::Clear(window->m_x, window->m_y + 20, window->m_iWidth - 6, window->m_iHeight - 7 - 23, Color(255, 150, 0, 255));
	//T³o
	Render::Clear(window->m_x + 6, window->m_y + 1 + 25, window->m_iWidth - 2 - 10, window->m_iHeight - 2 - 4 - 26, Color(red, green, blue, 255));
	Render::Clear(window->m_x + 8, window->m_y + 1 + 27, window->m_iWidth - 4 - 12, window->m_iHeight - 2 - 8 - 26, Color(50, 50, 50, 255));

	Vertex_t Verts[4];
	Verts[0].Init(Vector2D(window->m_x + 8, window->m_y + 27 + 1));//Prawo
	Verts[1].Init(Vector2D(window->m_x + 8 + window->m_iWidth - 16, window->m_y + 27 + 1)); // Gora
	Verts[2].Init(Vector2D(window->m_x + 8 + window->m_iWidth - 32, window->m_y + 27 + 20 + 1)); // Dol
	Verts[3].Init(Vector2D(window->m_x + 8 + 16, window->m_y + 27 + 20 + 1));//Lewo
	
	RECT TitleSize = Render::GetTextSize(Render::Fonts::MenuBold, window->Title.c_str());
	Render::Polygon(4, Verts, Color(red, green, blue, 255));
	Render::Text(window->m_x + 8 + (window->m_iWidth / 2) - TitleSize.right / 2, window->m_y + 30, Color(redtitle, greentitle, bluetitle, 255), Render::Fonts::MenuBold, window->Title.c_str());
	// Taby
	int TabCount = window->Tabs.size();
	if (TabCount) // If there are some tabs
	{
		for (int i = 0; i < TabCount; i++)
		{
			RECT TabArea = { window->m_x, window->m_y + 110 + (i * 100) , 100, 60 };
			CTab *tab = window->Tabs[i];
			//Jak ktoœ chce to niech u¿ywa lul. To robi takie siatki na tabach.

			//Linia Prawo
			//	Render::Clear(TabArea.left + 100, TabArea.top - 5, 1, 70, UI_COL_TABSEPERATOR);
			//Linia Poczatek
			//	Render::Clear(TabArea.left + 8, window->m_y + 48 + 52 + 5, 92, 1, UI_COL_TABSEPERATOR);
			//Linia Lewo
			//	Render::Clear(TabArea.left + 8, TabArea.top - 5, 1, 70, UI_COL_TABSEPERATOR);
			///	//Linia Konca 

			//	Render::Clear(TabArea.left + 8, window->m_y + 332 + 52, 92, 1, UI_COL_TABSEPERATOR);

			Color txtColor = UI_COL_TABSEPERATOR;

			if (window->SelectedTab == tab)
			{
				// Selected
				//Render::Clear(TabArea.left + 9, TabArea.top, 95, 70, Color(90, 90, 90, 255));
				txtColor = Color(red, green, blue, 255);
			}
			else if (IsMouseInRegion(TabArea))
			{
				// Hover
				txtColor = UI_COL_SHADOW;
			}

			Render::Text(TabArea.left + 32, TabArea.top + 8, txtColor, Render::Fonts::Tabs, tab->Title.c_str());
			RECT Icon = Render::GetTextSize(Render::Fonts::Tabs, tab->Title.c_str());
			RECT Icon2 = Render::GetTextSize(Render::Fonts::Menu, tab->Title2.c_str());
			Render::Text(TabArea.left + 28, TabArea.top + 53, txtColor, Render::Fonts::Menu, tab->Title2.c_str());
		}
	}
	//Render::Line(window->m_x + 8, window->m_y + 28, window->m_x + window->m_iWidth - 8, window->m_y + window->m_iHeight - 8, Color(255, 0, 0, 255));

	// Controls 
	if (window->SelectedTab != nullptr)
	{
		// Focused widget
		bool SkipWidget = false;
		CControl* SkipMe = nullptr;

		// this window is focusing on a widget??
		if (window->IsFocusingControl)
		{
			if (window->FocusedControl != nullptr)
			{
				// We need to draw it last, so skip it in the regular loop
				SkipWidget = true;
				SkipMe = window->FocusedControl;
			}
		}


		// Itterate over all the other controls
		for (auto control : window->SelectedTab->Controls)
		{
			if (SkipWidget && SkipMe == control)
				continue;

			if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
			{
				POINT cAbs = control->GetAbsolutePos();
				RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
				bool hover = false;
				if (IsMouseInRegion(controlRect))
				{
					hover = true;
				}
				control->Draw(hover);
			}
		}

		// Draw the skipped widget last
		if (SkipWidget)
		{
			auto control = window->FocusedControl;

			if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
			{
				POINT cAbs = control->GetAbsolutePos();
				RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
				bool hover = false;
				if (IsMouseInRegion(controlRect))
				{
					hover = true;
				}
				control->Draw(hover);
			}
		}

	}


	return true;
}
#include "Hacks.h"
bool CGUI::DrawRadar() {

	int red = Menu::Window.ColorsTab.MenuR.GetValue();
	int green = Menu::Window.ColorsTab.MenuG.GetValue();
	int blue = Menu::Window.ColorsTab.MenuB.GetValue();

	int redtitle = Menu::Window.ColorsTab.MenutitleR.GetValue();
	int greentitle = Menu::Window.ColorsTab.MenutitleG.GetValue();
	int bluetitle = Menu::Window.ColorsTab.MenutitleB.GetValue();





	//Render::Outline(window->m_x, window->m_y, window->m_iWidth, window->m_iHeight, Color::FromHSB(koloreklul, 1.f, 1.f));
	//Render::GradientV(window->m_x + 2, window->m_y + 2, window->m_iWidth - 4, 26, Color::FromHSB(koloreklul, 1.f, 1.f), Color::FromHSB(koloreklul, 1.f, 1.f));
	//Render::Clear(window->m_x + 2, window->m_y + 2 + 26, window->m_iWidth - 4, window->m_iHeight - 4 - 26, Color::FromHSB(koloreklul, 1.f, 1.f));
	//Render::Outline(window->m_x + 1, window->m_y + 1, window->m_iWidth - 2, window->m_iHeight - 2, Color(red, green, blue, 255));
	//Render::Text(window->m_x + 350, window->m_y + 8, Color::FromHSB(koloreklul, 1.f, 1.f), Render::Fonts::MenuBold, window->Title.c_str());


	//Obramówka
	//Render::Clear(window->m_x + 8, window->m_y + 1 + 27, window->m_iWidth - 4 - 12, window->m_iHeight - 2 - 8 - 26, Color(50, 150, 225, 255));
	//Render::Clear(window->m_x, window->m_y + 20, window->m_iWidth - 6, window->m_iHeight - 7 - 23, Color(255, 150, 0, 255));
	//T³o
	Render::Clear(Radar->m_x + 6, Radar->m_y + 1 + 25, Radar->m_iWidth - 2 - 10, Radar->m_iHeight - 2 - 4 - 26, Color(red, green, blue, 255));
	Render::Clear(Radar->m_x + 8, Radar->m_y + 1 + 27, Radar->m_iWidth - 4 - 12, Radar->m_iHeight - 2 - 8 - 26, Color(50, 50, 50, 255));

	Vertex_t Verts[4];
	Verts[0].Init(Vector2D(Radar->m_x + 8, Radar->m_y + 27 + 1));//Prawo
	Verts[1].Init(Vector2D(Radar->m_x + 8 + Radar->m_iWidth - 16, Radar->m_y + 27 + 1)); // Gora
	Verts[2].Init(Vector2D(Radar->m_x + 8 + Radar->m_iWidth - 32, Radar->m_y + 27 + 20 + 1)); // Dol
	Verts[3].Init(Vector2D(Radar->m_x + 8 + 16, Radar->m_y + 27 + 20 + 1));//Lewo

	RECT TitleSize = Render::GetTextSize(Render::Fonts::MenuBold, Radar->Title.c_str());
	Render::Polygon(4, Verts, Color(red, green, blue, 255));
	Render::Text(Radar->m_x + 8 + (Radar->m_iWidth / 2) - TitleSize.right / 2, Radar->m_y + 30, Color(redtitle, greentitle, bluetitle, 255), Render::Fonts::MenuBold, Radar->Title.c_str());
	// Taby
	int TabCount = Radar->Tabs.size();
	if (TabCount) // If there are some tabs
	{
		for (int i = 0; i < TabCount; i++)
		{
			RECT TabArea = { Radar->m_x, Radar->m_y + 110 + (i * 100) , 100, 60 };
			CTab *tab = Radar->Tabs[i];
			//Jak ktoœ chce to niech u¿ywa lul. To robi takie siatki na tabach.

			//Linia Prawo
			//	Render::Clear(TabArea.left + 100, TabArea.top - 5, 1, 70, UI_COL_TABSEPERATOR);
			//Linia Poczatek
			//	Render::Clear(TabArea.left + 8, window->m_y + 48 + 52 + 5, 92, 1, UI_COL_TABSEPERATOR);
			//Linia Lewo
			//	Render::Clear(TabArea.left + 8, TabArea.top - 5, 1, 70, UI_COL_TABSEPERATOR);
			///	//Linia Konca 

			//	Render::Clear(TabArea.left + 8, window->m_y + 332 + 52, 92, 1, UI_COL_TABSEPERATOR);

			Color txtColor = UI_COL_TABSEPERATOR;

			if (Radar->SelectedTab == tab)
			{
				// Selected
				//Render::Clear(TabArea.left + 9, TabArea.top, 95, 70, Color(90, 90, 90, 255));
				txtColor = Color(red, green, blue, 255);
			}
			else if (IsMouseInRegion(TabArea))
			{
				// Hover
				txtColor = UI_COL_SHADOW;
			}

			Render::Text(TabArea.left + 32, TabArea.top + 8, txtColor, Render::Fonts::Tabs, tab->Title.c_str());
			RECT Icon = Render::GetTextSize(Render::Fonts::Tabs, tab->Title.c_str());
			RECT Icon2 = Render::GetTextSize(Render::Fonts::Menu, tab->Title2.c_str());
			Render::Text(TabArea.left + 28, TabArea.top + 53, txtColor, Render::Fonts::Menu, tab->Title2.c_str());
		}
	}
	//Render::Line(window->m_x + 8, window->m_y + 28, window->m_x + window->m_iWidth - 8, window->m_y + window->m_iHeight - 8, Color(255, 0, 0, 255));

	// Controls 
	if (Radar->SelectedTab != nullptr)
	{
		// Focused widget
		bool SkipWidget = false;
		CControl* SkipMe = nullptr;

		// this window is focusing on a widget??
		if (Radar->IsFocusingControl)
		{
			if (Radar->FocusedControl != nullptr)
			{
				// We need to draw it last, so skip it in the regular loop
				SkipWidget = true;
				SkipMe = Radar->FocusedControl;
			}
		}


		// Itterate over all the other controls
		for (auto control : Radar->SelectedTab->Controls)
		{
			if (SkipWidget && SkipMe == control)
				continue;

			if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
			{
				POINT cAbs = control->GetAbsolutePos();
				RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
				bool hover = false;
				if (IsMouseInRegion(controlRect))
				{
					hover = true;
				}
				control->Draw(hover);
			}
		}

		// Draw the skipped widget last
		if (SkipWidget)
		{
			auto control = Radar->FocusedControl;

			if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
			{
				POINT cAbs = control->GetAbsolutePos();
				RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
				bool hover = false;
				if (IsMouseInRegion(controlRect))
				{
					hover = true;
				}
				control->Draw(hover);
			}
		}

	}


	return true;
}

void CGUI::RegisterWindow(CWindow* window)
{
	Windows.push_back(window);

	// Resorting to put groupboxes at the start
}

void CGUI::BindWindow(unsigned char Key, CWindow* window)
{
	if (window)
		WindowBinds[Key] = window;
	else
		WindowBinds.erase(Key);
}

void CGUI::SaveWindowState(CWindow* window, std::string Filename)
{
	// Create a whole new document and we'll just save over top of the old one
	tinyxml2::XMLDocument Doc;

	// Root Element is called "INTERX"
	tinyxml2::XMLElement *Root = Doc.NewElement("ANG3LZ.CR3W");
	Doc.LinkEndChild(Root);

	// If the window has some tabs..
	if (Root && window->Tabs.size() > 0)
	{
		for (auto Tab : window->Tabs)
		{
			// Add a new section for this tab
			tinyxml2::XMLElement *TabElement = Doc.NewElement(Tab->Title.c_str());
			Root->LinkEndChild(TabElement);

			// Now we itterate the controls this tab contains
			if (TabElement && Tab->Controls.size() > 1)
			{
				for (auto Control : Tab->Controls)
				{
					// If the control is ok to be saved
					if (Control && Control->Flag(UIFlags::UI_SaveFile) && Control->FileIdentifier.length() > 1 && Control->FileControlType)
					{
						// Create an element for the control
						tinyxml2::XMLElement *ControlElement = Doc.NewElement(Control->FileIdentifier.c_str());
						TabElement->LinkEndChild(ControlElement);

						if (!ControlElement)
						{
							return;
						}

						CCheckBox* cbx = nullptr;
						CComboBox* cbo = nullptr;
						CKeyBind* key = nullptr;
						CSlider* sld = nullptr;

						// Figure out what kind of control and data this is
						switch (Control->FileControlType)
						{
						case UIControlTypes::UIC_CheckBox:
							cbx = (CCheckBox*)Control;
							ControlElement->SetText(cbx->GetState());
							break;
						case UIControlTypes::UIC_ComboBox:
							cbo = (CComboBox*)Control;
							ControlElement->SetText(cbo->GetIndex());
							break;
						case UIControlTypes::UIC_KeyBind:
							key = (CKeyBind*)Control;
							ControlElement->SetText(key->GetKey());
							break;
						case UIControlTypes::UIC_Slider:
							sld = (CSlider*)Control;
							ControlElement->SetText(sld->GetValue());
							break;
						}
					}
				}
			}
		}
	}

	//Save the file
	if (Doc.SaveFile(Filename.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		MessageBox(NULL, "Failed To Save Config File!", "ANGELZ", MB_OK);
	}

}

void CGUI::LoadWindowState(CWindow* window, std::string Filename)
{
	// Lets load our meme
	tinyxml2::XMLDocument Doc;
	if (Doc.LoadFile(Filename.c_str()) == tinyxml2::XML_NO_ERROR)
	{
		tinyxml2::XMLElement *Root = Doc.RootElement();


		if (Root)
		{
			// If the window has some tabs..
			if (Root && window->Tabs.size() > 0)
			{
				for (auto Tab : window->Tabs)
				{
					// We find the corresponding element for this tab
					tinyxml2::XMLElement *TabElement = Root->FirstChildElement(Tab->Title.c_str());
					if (TabElement)
					{
						// Now we itterate the controls this tab contains
						if (TabElement && Tab->Controls.size() > 0)
						{
							for (auto Control : Tab->Controls)
							{
								// If the control is ok to be saved
								if (Control && Control->Flag(UIFlags::UI_SaveFile) && Control->FileIdentifier.length() > 1 && Control->FileControlType)
								{
									// Get the controls element
									tinyxml2::XMLElement *ControlElement = TabElement->FirstChildElement(Control->FileIdentifier.c_str());

									if (ControlElement)
									{
										CCheckBox* cbx = nullptr;
										CComboBox* cbo = nullptr;
										CKeyBind* key = nullptr;
										CSlider* sld = nullptr;

										// Figure out what kind of control and data this is
										switch (Control->FileControlType)
										{
										case UIControlTypes::UIC_CheckBox:
											cbx = (CCheckBox*)Control;
											cbx->SetState(ControlElement->GetText()[0] == '1' ? true : false);
											break;
										case UIControlTypes::UIC_ComboBox:
											cbo = (CComboBox*)Control;
											cbo->SelectIndex(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_KeyBind:
											key = (CKeyBind*)Control;
											key->SetKey(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_Slider:
											sld = (CSlider*)Control;
											sld->SetValue(atof(ControlElement->GetText()));
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}