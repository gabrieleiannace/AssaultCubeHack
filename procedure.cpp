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

//Funzione che restituisce un puntatore manipolabile (posso fare tutte le operazioni che faccio con gli interi)
uintptr_t GetModuleBaseAddress(DWORD dwPid, const char* moduleName) {
    uintptr_t dwBase = 0;
    do {
        //Un handle può essere qualsiasi cosa, da un indice intero a un puntatore a una risorsa nello spazio del kernel
        //CreateToolhelp32Snapshot acquisisce un'istantena (snapshot) dei processi specificati (dwPid)
        //Il primo parametro sono dei FLAG: TH32CS_SNAPMODULE -> Include tutti i moduli del processo
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE, dwPid);

        if (hSnapshot == INVALID_HANDLE_VALUE) {
        //il continue mi fa saltare direttamente alla prossima esecuzione del ciclo while.
         continue;
        }

        //MODULEENTRY32 è il descrittore di un modulo (una struct con dentro alcuni valori come .dwsize)
        MODULEENTRY32 ModuleEntry32;
        //Info: Before calling the Module32First function, set this member to sizeof(MODULEENTRY32). If you do not initialize dwSize, Module32First fails.
        ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

        //Posso finalmente chiamare Module32First -> mi fornisce il descrittore di un modulo (&ModuleEntry32 è parametro di output)
        if (Module32First(hSnapshot, &ModuleEntry32)) {
            do {
                //szModule è il nome del modulo, faccio uno string compare.
                if (!strcmp(ModuleEntry32.szModule, (LPSTR)moduleName)) {
                    //.modBaseAdd = The base address of the module in the context of the owning process
                    dwBase = (DWORD)ModuleEntry32.modBaseAddr;
                    break;
                }
            //Condizione del while: "finchè c'è un next module"
            } while (Module32Next(hSnapshot, &ModuleEntry32));
        }

        CloseHandle(hSnapshot);
    //Finchè il valore dwBase (ModuleBasAddress) non muta, in sostanza il programma è sempre in cerca del modulo specificato!
    } while (!dwBase);
    return dwBase;
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