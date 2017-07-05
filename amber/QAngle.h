#pragma once

/*
#include "SDK.h"

// 3D ANGLE
class QAngle
{
public:
	// Members
	float x, y, z;

	// Construction/destruction:
	QAngle(void);
	QAngle(float X, float Y, float Z);
	QAngle(float* clr);
	QAngle(Vector A);

	// Initialization
	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
	// TODO (Ilya): Should there be an init that takes a single float for consistency?

	// Got any nasty NAN's?
	bool IsValid() const;
	void Invalidate();

	// array access...
	float operator[](int i) const;
	float& operator[](int i);

	// Base address...
	float* Base();
	float const* Base() const;

	// Initialization methods
	void Random(float minVal, float maxVal);
	void Zero(); ///< zero out a vector

	QAngle& Normalized()
	{
		while (x > 89.0f)
			x -= 180.f;
		while (x < -89.0f)
			x += 180.f;
		while (y > 180.f)
			y -= 360.f;
		while (y < -180.f)
			y += 360.f;
		return *this;
	}

	// equality
	bool operator==(const Vector& v) const;
	bool operator!=(const Vector& v) const;

	// arithmetic operations
	QAngle& operator+=(const Vector& v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	QAngle& operator-=(const Vector& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	QAngle& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;
		return *this;
	}

	QAngle& operator*=(const Vector& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	QAngle& operator/=(const Vector& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	// this ought to be an opcode.
	QAngle&	operator+=(float fl)
	{
		x += fl;
		y += fl;
		z += fl;
		return *this;
	}

	// this ought to be an opcode.
	QAngle&	operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}
	QAngle&	operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		z -= fl;
		return *this;
	}

	// arithmetic operations
	QAngle& operator+=(const QAngle& v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	QAngle& operator-=(const QAngle& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	QAngle& operator*=(const QAngle& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	QAngle& operator/=(const QAngle& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	bool operator==(const QAngle& v)
	{
		return (x == v.x && y == v.y && z == v.z);
	}

	bool operator!=(const QAngle& v)
	{
		return !(x == v.x && y == v.y && z == v.z);
	}

	Vector Direction()
	{
		return Vector(cos(y * M_PI / 180.0f) * cos(x * M_PI / 180.0f), sin(y * M_PI / 180.0f) * cos(x * M_PI / 180.0f), sin(-x * M_PI / 180.0f)).Normalized();
	}

	Vector Forward()
	{
		return Direction();
	}

	Vector Up()
	{
		return Vector(cos(y * M_PI / 180.0f) * cos(x * M_PI / 180.0f), sin(y * M_PI / 180.0f) * cos(x * M_PI / 180.0f), sin(-(x - 90.0f) * M_PI / 180.0f)).Normalized();
	}

	Vector Right()
	{
		return Vector(cos(y * M_PI / 180.0f) * cos(x * M_PI / 180.0f), sin((y + 90.0f)* M_PI / 180.0f) * cos(x * M_PI / 180.0f), sin(-x * M_PI / 180.0f)).Normalized();
	}

	// negate the vector components
	void	Negate();

	// Get the vector's magnitude squared.
	float LengthSqr(void) const
	{
		return (x*x + y*y + z*z);
	}

	// return true if this vector is (0,0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance &&
			z > -tolerance && z < tolerance);
	}

	// Copy
	void	CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual vector equation (because it's done per-component
	// rather than per-vector).
	void	MulAdd(const Vector& a, const Vector& b, float scalar);

	// assignment
	QAngle& operator=(const QAngle &vOther);

	// copy constructors
	//	Vector(const Vector &vOther);

	// arithmetic operations
	QAngle	operator-(void) const;
	QAngle	operator+(const Vector& v) const;
	QAngle	operator-(const Vector& v) const;
	QAngle	operator*(const Vector& v) const;
	QAngle	operator/(const Vector& v) const;
	QAngle	operator*(float fl) const;
	QAngle	operator/(float fl) const;
	QAngle	operator+(const QAngle& v) const;
	QAngle	operator-(const QAngle& v) const;
	QAngle	operator*(const QAngle& v) const;
	QAngle	operator/(const QAngle& v) const;
};
*/