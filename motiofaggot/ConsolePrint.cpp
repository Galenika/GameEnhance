#include "ConsolePrint.h"
#include "Hacks.h"
#include <stdio.h>
#include <string.h>
#include "ESP.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "GlowManager.h"
#include <stdio.h>
static bool printed = false;

char welcometext[512];
void Read()
{
	char* pText = ("echo Logged As ");//Welcome
	char* user = "";//User

	
	//sprintf_s(welcometext, "%s%s",pText, user);
}

void Console::ConsolePrinter()
{
	if (!printed)
	{
		Read();
		Interfaces::Engine->ClientCmd_Unrestricted("clear");
		Interfaces::Engine->ClientCmd_Unrestricted("echo");
		Interfaces::CVar->ConsoleColorPrintf(Color(0, 150, 255, 255), "RedHead.Pro:");
		Interfaces::CVar->ConsoleColorPrintf(Color(255, 255, 255, 255), " Loaded !");
		Interfaces::Engine->ClientCmd_Unrestricted(welcometext);
		Interfaces::Engine->ClientCmd_Unrestricted("echo Good Luck & Have Fun");
		Interfaces::Engine->ClientCmd_Unrestricted("echo Build date : ""16.04.2017");
	//	Interfaces::Engine->ClientCmd_Unrestricted("echo Configs :");
		//Interfaces::Engine->ClientCmd_Unrestricted("echo HVHMAX");
		//Interfaces::Engine->ClientCmd_Unrestricted("echo MEXI");
		printed = true;
	}
}

