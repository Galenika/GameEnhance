// VirtualMethod
#include "Ares.h"

#pragma region Helper Functions

bool CVMT::HookFunction(void* pNewFunction, const std::size_t sizeFunctionIndex) {
	if (sizeFunctionIndex > this->m_sizeTotalFunctions)
		return false;

	this->m_pCurrentVFT[sizeFunctionIndex] = reinterpret_cast<std::uintptr_t>(pNewFunction);

	return true;
}

bool CVMT::UnhookFunction(const std::size_t sizeFunctionIndex) {
	if (sizeFunctionIndex > this->m_sizeTotalFunctions)
		return false;

	this->m_pCurrentVFT[sizeFunctionIndex] = this->m_iOriginalVFT[sizeFunctionIndex];

	return true;
}

std::size_t CVMT::GetTotalFunctions() {
	return this->m_sizeTotalFunctions;
}

#pragma endregion