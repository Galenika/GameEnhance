
#pragma once

#include "Interfaces.h"

#include "Vector2D.h"

void Quad();

enum FontRenderFlag_t
{
	FONT_LEFT = 0,
	FONT_RIGHT = 1,
	FONT_CENTER = 2
};


namespace Render
{
	void Initialise();

	// Normal Drawing functions
	void Clear(int x, int y, int w, int h, Color color);
	void Outline(int x, int y, int w, int h, Color color);
	void Line(int x, int y, int x2, int y2, Color color);
	void PolyLine(int *x, int *y, int count, Color color);
	void Polygon(int count, Vertex_t* Vertexs, Color color);
	void PolygonOutline(int count, Vertex_t* Vertexs, Color color, Color colorLine);
	void PolyLine(int count, Vertex_t* Vertexs, Color colorLine);

	// Gradient Functions
	void GradientV(int x, int y, int w, int h, Color c1, Color c2);
	void GradientH(int x, int y, int w, int h, Color c1, Color c2);

	// Text functions
	namespace Fonts
	{
		extern DWORD Default;
		extern DWORD Menu;
		extern DWORD MenuBold;
		extern DWORD INTERX;
		extern DWORD skrrtmenufont;
		extern DWORD ESP;
		extern DWORD Verdana;
		extern DWORD Courier;
		//extern DWORD kokonut;
		extern DWORD Tabs;

	};

	void Text(int x, int y, Color color, DWORD font, const char* text);
	void Textf(int x, int y, Color color, DWORD font, const char* fmt, ...);
	void Text(int x, int y, Color color, DWORD font, const wchar_t* text);
	RECT GetTextSize(DWORD font, const char* text);

	// Other rendering functions
	bool WorldToScreen(Vector &in, Vector &out);
	RECT GetViewport();
};

namespace D
{

	INT getWidht(unsigned long font, const char* input);
	std::wstring stringToWide(const std::string& text);
	extern void DrawPolygonRainbow(int count, Vertex_t* Vertexs, float flSpeed, float &flRainbow);
	extern void Text(int x, int y, Color color, DWORD font, const char* text);
	extern void Textf(int x, int y, Color color, DWORD font, const char* fmt, ...);
	extern void Text(int x, int y, Color color, DWORD font, const wchar_t* text);
	RECT GetTextSize(DWORD font, const char* text);
	extern void SetupFonts();
	extern void DrawString(unsigned long font, int x, int y, Color color, DWORD alignment, const char* msg, ...);
	extern void DrawStringUnicode(unsigned long font, int x, int y, Color color, bool bCenter, const wchar_t* msg, ...);
	extern void DrawRect(int x, int y, int w, int h, Color col);
	extern void DrawRectRainbow(int x, int y, int w, int h, float flSpeed, float &flRainbow);
	extern void DrawRectGradientVertical(int x, int y, int w, int h, Color color1, Color color2);
	extern void DrawRectGradientHorizontal(int x, int y, int w, int h, Color color1, Color color2);
	extern void DrawPixel(int x, int y, Color col);
	extern void DrawOutlinedRect(int x, int y, int w, int h, Color col);
	extern void DrawOutlinedCircle(int x, int y, int r, Color col);
	extern void DrawLine(int x0, int y0, int x1, int y1, Color col);
	extern void DrawCorner(int iX, int iY, int iWidth, int iHeight, bool bRight, bool bDown, Color colDraw);
	extern void DrawRoundedBox(int x, int y, int w, int h, int r, int v, Color col);
	extern void DrawPolygon(int count, Vertex_t* Vertexs, Color color);
	extern bool ScreenTransform(const Vector &point, Vector &screen);
	extern bool WorldToScreen(const Vector &origin, Vector &screen);
	extern int GetStringWidth(unsigned long font, const char* msg, ...);
	extern void Draw3DBox(Vector* boxVectors, Color color);
	extern void DrawCircle(float x, float y, float r, float s, Color color);
	extern void DrawRectOut(float x, float y, float w, float h, Color color, Color out);
	extern void DrawBorderBox(float x, float y, float w, float h, float t, Color color);
	extern void BorderBox(int x, int y, int w, int h, int thickness, Color color);
	extern void BorderBoxOutlined(int x, int y, int w, int h, int thickness, Color color, Color outlined);
	extern void CornerBox(int x, int y, int w, int h, Color color, bool outlined);
	extern void PrintIcons(int x, int y, int r, int g, int b, int a, DWORD dwAlign, const char* szText, ...);

	// Gradient Functions
	extern void GradientV(int x, int y, int w, int h, Color c1, Color c2);
	extern void GradientH(int x, int y, int w, int h, Color c1, Color c2);

	bool WorldToScreen(Vector &in, Vector &out);
	RECT GetViewport();

	extern void DrawColorSpectrum(int x, int y, int w, int h);
	Color GetColorFromPenPosition(Vector stx);
	Vector GetMouse(int x, int y);


}

