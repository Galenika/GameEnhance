

#pragma once

// Includes
#include "CommonIncludes.h"
#include <time.h>
#include "NetVars.h"
#include "ClientRecvProps.h"
#include "Xor.h"

// Colors for the console
//Define extra colours
#define FOREGROUND_WHITE		    (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_YELLOW       	(FOREGROUND_RED | FOREGROUND_GREEN)
#define FOREGROUND_CYAN		        (FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_MAGENTA	        (FOREGROUND_RED | FOREGROUND_BLUE)
#define FOREGROUND_BLACK		    0

#define FOREGROUND_INTENSE_RED		(FOREGROUND_RED | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_GREEN	(FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_BLUE		(FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_WHITE	(FOREGROUND_WHITE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_YELLOW	(FOREGROUND_YELLOW | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_CYAN		(FOREGROUND_CYAN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_MAGENTA	(FOREGROUND_MAGENTA | FOREGROUND_INTENSITY)

// U Namespace
// Purpose: Contains misc functionality for memory related functionality
namespace Utilities
{
	// Debug console controls
	void OpenConsole(std::string Title);
	void CloseConsole();
	void Log(const char *fmt, ...);

	void EnableLogFile(std::string filename);

	void SetConsoleColor(WORD color);

	// Misc Shizz
	std::string GetTimeString();

	// Memory utils
	// Purpose: Provides memeory related functionality (pattern scanning ect)
	namespace Memory
	{
		typedef void* (*CreateInterface_t)(const char*, int*);

		inline void* CaptureInterface(const char* chHandle, const char* chInterfaceName)
		{
			volatile auto handlegotten = (GetModuleHandleA(chHandle) != nullptr);
			while (!GetModuleHandleA(chHandle)) Sleep(100);
			void* fnFinal = nullptr;
			auto PossibleInterfaceName = new char[strlen(chInterfaceName) + 4];
			auto TestInterface = reinterpret_cast<CreateInterface_t>(GetProcAddress(GetModuleHandleA(chHandle), XorStr("CreateInterface")));
			for (auto i = 100; i > 0; i--)
			{
				XorCompileTime::w_sprintf(PossibleInterfaceName, XorStr("%s%03i"), chInterfaceName, i);
				fnFinal = static_cast<void*>(TestInterface(PossibleInterfaceName, nullptr));

				if (fnFinal != nullptr)
					break;

			}
			delete PossibleInterfaceName;
			return fnFinal;
		}


		// Waits for a module to be available, before returning it's base address
		DWORD WaitOnModuleHandle(std::string moduleName);

		// Attempts to locate the given signature and mask in a memory range
		// Returns the address at which it has been found
		DWORD FindPattern(std::string moduleName, BYTE* Mask, char* szMask);
		uint64_t FindPatternIDA(const char* szModule, const char* szSignature);
		DWORD FindPatternV2(std::string moduleName, std::string pattern);
		DWORD FindPatternVersion2(std::string moduleName, std::string pattern);
		extern ULONG FindPatternAlt(std::string szModuleName, PBYTE pbPattern, std::string szMask, ULONG uCodeBase, ULONG uSizeOfCode);
		ULONG PatternSearch(std::string sModuleName, PBYTE pbPattern, std::string sMask, ULONG uCodeBase, ULONG uSizeOfCode);

		// Attempts to locate the given text in a memory range
		// Returns the address at which it has been found
		DWORD FindTextPattern(std::string moduleName, char* string);

		class VMTManager
		{
		private:
			DWORD	*CustomTable;
			bool	initComplete;
			DWORD	*OriginalTable;
			DWORD	*Instance;

			int		MethodCount(DWORD* InstancePointer);

		public:
			bool	Initialise(DWORD* InstancePointer); // Pass a &class

			DWORD	HookMethod(DWORD NewFunction, int Index);
			void	UnhookMethod(int Index);

			void	RestoreOriginal();
			void	RestoreCustom();

			template<typename T>
			T GetMethod(size_t nIndex)
			{
				return (T)OriginalTable[nIndex];
			}

			DWORD	GetOriginalFunction(int Index);
		};

		class CVMTHookManager
		{
		public:
			CVMTHookManager(void)
			{
				memset(this, 0, sizeof(CVMTHookManager));
			}

			CVMTHookManager(PDWORD* ppdwClassBase)
			{
				bInitialize(ppdwClassBase);
			}

			~CVMTHookManager(void)
			{
				UnHook();
			}
			bool bInitialize(PDWORD* ppdwClassBase)
			{
				m_ppdwClassBase = ppdwClassBase;
				m_pdwOldVMT = *ppdwClassBase;
				m_dwVMTSize = dwGetVMTCount(*ppdwClassBase);
				m_pdwNewVMT = new DWORD[m_dwVMTSize];
				memcpy(m_pdwNewVMT, m_pdwOldVMT, sizeof(DWORD) * m_dwVMTSize);
				*ppdwClassBase = m_pdwNewVMT;
				return true;
			}
			bool bInitialize(PDWORD** pppdwClassBase) // fix for pp
			{
				return bInitialize(*pppdwClassBase);
			}

			void UnHook(void)
			{
				if (m_ppdwClassBase)
				{
					*m_ppdwClassBase = m_pdwOldVMT;
				}
			}

			void ReHook(void)
			{
				if (m_ppdwClassBase)
				{
					*m_ppdwClassBase = m_pdwNewVMT;
				}
			}

			int iGetFuncCount(void)
			{
				return (int)m_dwVMTSize;
			}

			DWORD dwGetMethodAddress(int Index)
			{
				if (Index >= 0 && Index <= (int)m_dwVMTSize && m_pdwOldVMT != NULL)
				{
					return m_pdwOldVMT[Index];
				}
				return NULL;
			}

			PDWORD pdwGetOldVMT(void)
			{
				return m_pdwOldVMT;
			}

			DWORD dwHooKmethod(DWORD dwNewFunc, unsigned int iIndex)
			{
				if (m_pdwNewVMT && m_pdwOldVMT && iIndex <= m_dwVMTSize && iIndex >= 0)
				{
					m_pdwNewVMT[iIndex] = dwNewFunc;
					return m_pdwOldVMT[iIndex];
				}

				return NULL;
			}

		private:
			DWORD dwGetVMTCount(PDWORD pdwVMT)
			{
				DWORD dwIndex = 0;

				for (dwIndex = 0; pdwVMT[dwIndex]; dwIndex++)
				{
					if (IsBadCodePtr((FARPROC)pdwVMT[dwIndex]))
					{
						break;
					}
				}
				return dwIndex;
			}
			PDWORD*    m_ppdwClassBase;
			PDWORD    m_pdwNewVMT, m_pdwOldVMT;
			DWORD    m_dwVMTSize;
		};

		void* Hook(void* pInstance, int iIndex, void* pHookFunc);

	};
	int FindNet(const char* table, const char* varName);
};

template<class T>
inline T GetFieldValue(int offset)
{
	return *(T*)((DWORD)this + offset);
}