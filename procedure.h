#ifndef _GAME_HACK_
#define _GAME_HACK_

#include <stdint.h>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>


DWORD GetProcId(const wchar_t* procName);

uintptr_t GetModuleBaseAddress(DWORD dwPid, const char* moduleName);

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);

#endif