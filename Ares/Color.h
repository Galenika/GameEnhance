// Color
#pragma once

class Color {
public:

	unsigned char rgba[4];

	Color(int r, int g, int b, int a) {
		rgba[0] = (unsigned char)r;
		rgba[1] = (unsigned char)g;
		rgba[2] = (unsigned char)b;
		rgba[3] = (unsigned char)a;
	}

};