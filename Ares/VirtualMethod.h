// VirtualMethod
#pragma once

// VirtualMethodTable
/*
Purpose: A virtual method table is an array of pointers to virtual functions. The pointer to the VMT, to the MS C++ Compiler, is placed in the first object
of a class, or the first 4 bytes. - https://www.unknowncheats.me/forum/unreal-3-engine/69951-tutorial-vtable-hooking-vmt-hooking-unreal-engine-c.html
- https://en.wikipedia.org/wiki/Virtual_method_table
*/
class CVMT {
private:
	std::uintptr_t** m_iBaseClass = nullptr;
	std::unique_ptr<std::uintptr_t[]> m_pCurrentVFT = nullptr;
	std::uintptr_t* m_iOriginalVFT = nullptr;
	std::size_t m_sizeTotalFunctions = 0;
public:

	CVMT(void) = default;

	CVMT(void* pBaseclass) {
		this->m_iBaseClass = static_cast<std::uintptr_t**>(pBaseclass);

		while (static_cast<std::uintptr_t*>(*this->m_iBaseClass)[this->m_sizeTotalFunctions])
			++this->m_sizeTotalFunctions;

		const std::size_t m_sizeTableSize = this->m_sizeTotalFunctions * sizeof(std::uintptr_t);

		this->m_iOriginalVFT = *this->m_iBaseClass;
		this->m_pCurrentVFT = std::make_unique<std::uintptr_t[]>(this->m_sizeTotalFunctions);

		std::memcpy(this->m_pCurrentVFT.get(), this->m_iOriginalVFT, m_sizeTableSize);

		*this->m_iBaseClass = this->m_pCurrentVFT.get();
	};

	~CVMT() {
		*this->m_iBaseClass = this->m_iOriginalVFT;
	};

	template <typename Function> inline const Function GetOriginalFunction(std::size_t function_index) {
		return reinterpret_cast<Function>(this->m_iOriginalVFT[function_index]);
	}

	bool HookFunction(void* pNewFunction, const std::size_t sizeFunctionIndex);
	bool UnhookFunction(const std::size_t sizeFunctionIndex);
	std::size_t GetTotalFunctions();

}; extern std::unique_ptr<CVMT> g_pClientModeHook; extern std::unique_ptr<CVMT> g_pPanelHook;