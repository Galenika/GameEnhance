// Draw
#include "Ares.h"

void Draw::DrawString(int x, int y, Color Col, unsigned long Font, const char* pszText) {

	if (pszText == NULL)
		return;

	wchar_t szString[1024] = { '\0' };
	wsprintfW(szString, L"%S", pszText);
	g_pSurface->DrawSetTextPos(x, y);
	g_pSurface->DrawSetTextFont(Font);
	g_pSurface->DrawSetTextColor(Col);
	g_pSurface->DrawPrintText(szString, wcslen(szString));

}