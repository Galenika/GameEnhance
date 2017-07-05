#pragma once

#include "GUI.h"

#include "Interfaces.h"

void CWindow::SetPosition(int x, int y)
{
	m_x = x; m_y = y;
}

void CWindow::SetSize(int w, int h)
{
	m_iWidth = w;
	m_iHeight = h;
}

void CWindow::SetTitle(std::string title)
{
	Title = title;
}

void CWindow::RegisterTab(CTab* Tab)
{
	if (Tabs.size() == 0)
	{
		SelectedTab = Tab;
	}
	Tab->parent = this;
	Tabs.push_back(Tab);
}

RECT CWindow::GetClientArea()
{
	RECT client;
	client.left = m_x + 8;
	client.top = m_y + 1 + 27;
	client.right = m_iWidth - 4 - 12;
	client.bottom = m_iHeight - 2 - 8 - 26;
	return client;
}

RECT CWindow::GetTabArea()
{
	RECT client;
	client.left = m_x;
	client.top = m_y + 100;
	client.right = 100;
	client.bottom = m_iHeight - UI_WIN_TOPHEIGHT - UI_WIN_TITLEHEIGHT;
	return client;
}

RECT CWindow::GetClientArea1()
{
	RECT client;
	client.left = m_x + 8;
	client.top = m_y + 1 + 36;
	client.right = m_iWidth - 4 - 12;
	client.bottom = m_iHeight - 2 - 8 - 26 + 500;
	return client;
}

RECT CWindow::GetTabArea1()
{
	RECT client;
	client.left = m_x + 8;
	client.top = m_y + 1 + 27;
	client.right = m_iWidth - 4 - 12;
	client.bottom = 29;
	return client;
}

void CWindow::Open()
{
	m_bIsOpen = true;
}

void CWindow::Close()
{
	m_bIsOpen = false;
}

void CWindow::Toggle()
{
	m_bIsOpen = !m_bIsOpen;
	if (m_bIsOpen)
	{
		
		//GameInput->EnableInput(false);
		Interfaces::InputSystem->EnableInput(false);
		Interfaces::Engine->ClientCmd_Unrestricted("cl_mouseenable 0");
	}
	else
	{
		
		
	Interfaces::InputSystem->EnableInput(true);
		Interfaces::Engine->ClientCmd_Unrestricted("cl_mouseenable 1");
	}

	/*static ConVar* mouselock = Interfaces::CVar->FindVar("cl_mouseenable");
	m_bIsOpen = !m_bIsOpen;
	mouselock->SetValue(!m_bIsOpen);
	Interfaces::InputSys->EnableInput(!m_bIsOpen);
	if (m_bIsOpen) Interfaces::InputSys->ResetInputState();*/
}

CControl* CWindow::GetFocus()
{
	return FocusedControl;
}

// TABS ---------------------------------------------------------------------------------------------------

void CTab::SetTitle(std::string name)
{
	Title = name;
}
void CTab::SetTitle2(std::string name)
{
	Title2 = name;
}

void CTab::RegisterControl(CControl* control)
{
	control->parent = parent;
	Controls.push_back(control);
}

//pSkinChanger Window
CControl* CSkinChanger::GetFocus()
{
	return FocusedControl;
}

void CSkinChanger::SetPosition(int x, int y)
{
	m_x = x; m_y = y;
}

void CSkinChanger::SetSize(int w, int h)
{
	m_iWidth = w;
	m_iHeight = h;
}

void CSkinChanger::SetTitle(std::string title)
{
	Title = title;
}

void CSkinChanger::RegisterTab(CTab* Tab)
{
	if (Tabs.size() == 0)
	{
		SelectedTab = Tab;
	}
	Tab->parent1 = this;
	Tabs.push_back(Tab);
}

RECT CSkinChanger::GetClientArea()
{
	RECT client;
	client.left = m_x + 8;
	client.top = m_y + 1 + 27;
	client.right = m_iWidth - 4 - 12;
	client.bottom = m_iHeight - 2 - 8 - 26;
	return client;
}

RECT CSkinChanger::GetTabArea()
{
	RECT client;
	client.left = m_x;
	client.top = m_y + 100;
	client.right = 100;
	client.bottom = m_iHeight - UI_WIN_TOPHEIGHT - UI_WIN_TITLEHEIGHT;
	return client;
}

RECT CSkinChanger::GetClientArea1()
{
	RECT client;
	client.left = m_x + 8;
	client.top = m_y + 1 + 36;
	client.right = m_iWidth - 4 - 12;
	client.bottom = m_iHeight - 2 - 8 - 26 + 500;
	return client;
}

RECT CSkinChanger::GetTabArea1()
{
	RECT client;
	client.left = m_x + 8;
	client.top = m_y + 1 + 27;
	client.right = m_iWidth - 4 - 12;
	client.bottom = 29;
	return client;
}

void CSkinChanger::Open()
{
	m_bIsOpen = true;
}

void CSkinChanger::Close()
{
	m_bIsOpen = false;
}

void CSkinChanger::Toggle()
{
	m_bIsOpen = !m_bIsOpen;
	if (m_bIsOpen)
	{

		//GameInput->EnableInput(false);
		Interfaces::InputSystem->EnableInput(false);
		Interfaces::Engine->ClientCmd_Unrestricted("cl_mouseenable 0");
	}
	else
	{


		Interfaces::InputSystem->EnableInput(true);
		Interfaces::Engine->ClientCmd_Unrestricted("cl_mouseenable 1");
	}

	/*static ConVar* mouselock = Interfaces::CVar->FindVar("cl_mouseenable");
	m_bIsOpen = !m_bIsOpen;
	mouselock->SetValue(!m_bIsOpen);
	Interfaces::InputSys->EnableInput(!m_bIsOpen);
	if (m_bIsOpen) Interfaces::InputSys->ResetInputState();*/
}

