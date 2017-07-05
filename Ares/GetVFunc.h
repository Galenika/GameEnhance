// GetVFunc
#pragma once

inline const void** GetVTable(const void* inst, size_t offset = 0) {
	return *reinterpret_cast<const void***>((size_t)inst + offset);
}

template<typename Fn>
inline Fn GetVFunc(const void* inst, size_t index, size_t offset = 0) {
	return reinterpret_cast<Fn>(GetVTable(inst, offset)[index]);
}
