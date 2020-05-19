#include "procedure.h"
#include <stdio.h>
#include <tchar.h>
#include <SDKDDKVer.h>

DWORD GetProcId(const wchar_t* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_wcsicmp((wchar_t *)procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));

		}
	}
	CloseHandle(hSnap);
	return procId;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const char *moduleName){

    uintptr_t moduleBase = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procId);

    if(hSnap != INVALID_HANDLE_VALUE){
        MODULEENTRY32 moduleEntry;
        moduleEntry.dwSize = sizeof(moduleEntry);        
        do{
            if(Module32First(hSnap, &moduleEntry)){

                if(!strcmp((char *)moduleEntry.szModule, moduleName)){
                    moduleBase = (uintptr_t )moduleEntry.modBaseAddr;
                    break;
                }
            }

        }while(Module32Next(hSnap, &moduleEntry));
    }
    CloseHandle(hSnap);
    return moduleBase;

}

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}