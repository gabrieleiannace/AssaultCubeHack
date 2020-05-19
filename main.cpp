#include "procedure.h"
#include <iostream>
/*
int main(int argc, char *argv[]){
   
   //Get the procID
   //Devo convertire "ac_client.exe" poichè è un array di char.
   //Convert to unicode con la L macro.
   DWORD procId = GetProcId(L"ac_client.exe");
   

   //Get moduleBaseAddress
   uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");


   //Handle to Process.
   HANDLE hProcess = 0;
   hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, procId);

    uintptr_t dynamicBaseAddress = moduleBase + 0x10F4F4;

    //Puntatore alle munizioni
    std::vector<unsigned int>ammoOffset {0x374, 0x14, 0x0};
    uintptr_t ammoAddress = FindDMAAddy(hProcess, dynamicBaseAddress, ammoOffset);

    //Read ammoAddress
    int ammoValue = 0;
    ReadProcessMemory(hProcess, (BYTE*)ammoAddress, &ammoValue, sizeof(ammoValue), 0);
    std::cout<<"Current ammo: "<< std::dec << ammoValue << "!\n";

    //Write ammoValue
    ammoValue = 100;
    WriteProcessMemory(hProcess, (BYTE*)ammoAddress, &ammoValue, sizeof(ammoValue), 0);

    //Read it againt for test if it works
    ReadProcessMemory(hProcess, (BYTE*)ammoAddress, &ammoValue, sizeof(ammoValue), 0);
    std::cout<<"Current ammo: "<< std::dec << ammoValue << "!\n";

    /*
    00F1A440 = Value = 16

    00F11424 -> 00F1A440 + 0x00 = 00F1A440

    00F1A664 -> 00F11410 + 0x14 = 00F11424

    ac_client.exe+10F4F4 -> 00F1A2F0 + 0x374 = 00F1A664
    
    */

   /*
    getchar();
    return 0;
}*/

int main()
{
    HWND GameHWND = FindWindow(0, "AssaultCube");
    DWORD procId = 0;
    GetWindowThreadProcessId(GameHWND, &procId);

    std::cout<<"ProcID "<< procId << std::endl;

    //Getmodulebaseaddress
    uintptr_t moduleBase = GetModuleBaseAddress(procId, "ac_client.exe");

    std::cout<<"Modulebase "<< moduleBase<< std::endl;

    //Get Handle to Process
    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, procId);

    //Resolve base address of the pointer chain
    uintptr_t dynamicPtrBaseAddr = moduleBase + 0x10f4f4;

    std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

    //Resolve our ammo pointer chain
    std::vector<unsigned int> ammoOffsets = { 0x374, 0x14, 0x0 };
    uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);

    std::cout << "ammoAddr = " << "0x" << std::hex << ammoAddr << std::endl;

    //Read Ammo value
    int ammoValue = 0;

    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "Curent ammo = " << std::dec << ammoValue << std::endl;

    //Write to it
    int newAmmo = 1337;
    WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

    //Read out again
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);

    std::cout << "New ammo = " << std::dec << ammoValue << std::endl;

    getchar();

    return 0;
}