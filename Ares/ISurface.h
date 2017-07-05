// ISurface
#pragma once

#undef CreateFont

enum FontFlags_t {
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

enum FontDrawType_t {
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
};

class ISurface {
public:
	void DrawSetColor(Color Col);
	void DrawSetColor(int r, int g, int b, int a);
	void DrawFilledRect(int x, int y, int w, int h);
	void DrawOutlinedRect(int x, int y, int w, int h);
	void DrawSetTextFont(unsigned long font);
	void DrawSetTextColor(Color Col);
	void DrawSetTextColor(int r, int g, int b, int a);
	void DrawSetTextPos(int x, int y);
	void DrawLine(int x0, int y0, int x1, int y1);
	void DrawPrintText(const wchar_t* text, int textLen);
	unsigned long CreateFont();
	void SetFontGlyphSet(unsigned long& font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags);
	void GetTextSize(unsigned long font, const wchar_t* text, int& wide, int& tall);
}; extern ISurface* g_pSurface;