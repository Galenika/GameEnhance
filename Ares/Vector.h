// Vector
#pragma once

// Description: 3D Vector class.
class Vector {
public:
	float x, y, z;
public:

	Vector(void) {
		x = y = z = 0.0f;
	}

	Vector(float x, float y, float z) {
		x = x, y = y, z = z;
	}

	float operator[](int i) const {
		return ((float *)this)[i];
	}

	Vector operator + (const Vector& v) const {
		return Vector(x + v.x, y + v.y, z + v.z);
	}

	Vector operator - (const Vector& v) const { 
		return Vector(x - v.x, y - v.y, z - v.z); 
	}

	float Length2D(void) const {
		return sqrtf(x* x + y* y);
	}

};