/////////////////////////////////////////
// zMenu 1.0, Customized for Mutiny.pw //
///////// Made by toast/zzzzz ///////////
/////////////////////////////////////////

#pragma once
#include <vector>
#include "SourceEngine\SDK.hpp"
#include "DrawTools.h"

namespace Menu
{
	extern bool ShouldDraw;
	class CBaseRenderable;
	class CWindow;
	class CTab;
	void InitializeMenu();
	void Render();
	void DestroyObjects();
};