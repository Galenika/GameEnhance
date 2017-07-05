
#pragma once

#include "Interfaces.h"

#include "Vector2D.h"

void Quad();


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


	//NEW FUNCTIONS
	void DrawRect(int x, int y, int w, int h, Color col);
	void DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow);
	void GradientSideways(int x, int y, int w, int h, Color color1, Color color2, int variation);
	void DrawCircle(float x, float y, float r, float segments, Color color);

	// Text functions
	namespace Fonts
	{
		extern DWORD Default;
		extern DWORD Menu;
		extern DWORD MenuBold;
		extern DWORD Tab;
		extern DWORD BITWare;
		extern DWORD ESP;
		extern DWORD Slider;

	};

	void Text(int x, int y, Color color, DWORD font, const char* text);
	void Textf(int x, int y, Color color, DWORD font, const char* fmt, ...);
	void Text(int x, int y, Color color, DWORD font, const wchar_t* text);
	RECT GetTextSize(DWORD font, const char* text);

	// Other rendering functions
	bool WorldToScreen(Vector &in, Vector &out);
	RECT GetViewport();
};

