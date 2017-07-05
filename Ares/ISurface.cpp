// ISurface
#include "Ares.h"

void ISurface::DrawSetColor(Color Col) {
	typedef void(__thiscall* OriginalFn)(PVOID, Color);
	GetVFunc<OriginalFn>(this, 10)(this, Col);
}

void ISurface::DrawSetColor(int r, int g, int b, int a) {
	typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
	GetVFunc<OriginalFn>(this, 11)(this, r, g, b, a);
}

void ISurface::DrawFilledRect(int x, int y, int w, int h) {
	typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
	GetVFunc<OriginalFn>(this, 12)(this, x, y, w, h);
}

void ISurface::DrawOutlinedRect(int x, int y, int w, int h) {
	typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
	GetVFunc<OriginalFn>(this, 14)(this, x, y, w, h);
}

void ISurface::DrawSetTextFont(unsigned long font) {
	typedef void(__thiscall* OriginalFn)(PVOID, unsigned long);
	GetVFunc<OriginalFn>(this, 17)(this, font);
}

void ISurface::DrawSetTextColor(Color Col) {
	typedef void(__thiscall* OriginalFn)(PVOID, Color);
	GetVFunc<OriginalFn>(this, 18)(this, Col);
}

void ISurface::DrawSetTextColor(int r, int g, int b, int a) {
	typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
	GetVFunc<OriginalFn>(this, 19)(this, r, g, b, a);
}

void ISurface::DrawSetTextPos(int x, int y) {
	typedef void(__thiscall* OriginalFn)(PVOID, int, int);
	GetVFunc<OriginalFn>(this, 20)(this, x, y);
}

void ISurface::DrawLine(int x0, int y0, int x1, int y1) {
	typedef void(__thiscall* OriginalFn)(void*, int, int, int, int);
	GetVFunc<OriginalFn>(this, 15)(this, x0, y0, x1, y1);
}

void ISurface::DrawPrintText(const wchar_t* text, int textLen) {
	typedef void(__thiscall* OriginalFn)(PVOID, const wchar_t*, int, int);
	return GetVFunc<OriginalFn>(this, 22)(this, text, textLen, 0);
}

unsigned long ISurface::CreateFont() {
	typedef unsigned int(__thiscall* OriginalFn)(PVOID);
	return GetVFunc<OriginalFn>(this, 66)(this);
}

void ISurface::SetFontGlyphSet(unsigned long& font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags) {
	typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const char*, int, int, int, int, int, int, int);
	GetVFunc<OriginalFn>(this, 67)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
}

void ISurface::GetTextSize(unsigned long font, const wchar_t* text, int& wide, int& tall) {
	typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const wchar_t *, int&, int&);
	GetVFunc<OriginalFn>(this, 75)(this, font, text, wide, tall);
}