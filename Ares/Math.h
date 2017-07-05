// Math
#pragma once

#include "CUserCmd.h"
#undef min
#undef max

inline float normalize(const float& f) {
	return std::remainderf(f, 360.f);
}

template<typename T>
inline T min(const T& a, const T& b) {
	return a < b ? a : b;
}

template<typename T>
inline T max(const T& a, const T& b) {
	return a > b ? a : b;
}

template <typename T>
inline T clamp(const T& val, const T& lower, const T& upper) {
	return max(lower, min(val, upper));
}

class CMath {
public:
	void ClampMovement(CUserCmd* Cmd);
}; extern CMath* g_pMath;