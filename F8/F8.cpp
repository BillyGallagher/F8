#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"

int main()
{
	DWORD procId = GetProcId(L"fate.exe");
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"fate.exe");

	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	uintptr_t dynamicPtrBaseAddr = moduleBase + 0x748900; 

	std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

	// Resolve renown pointer chain
	std::vector<unsigned int> renownLvlOffsets = { 0x80, 0x390 };
	uintptr_t renownLvlAddr = FindDMAAddress(hProcess, dynamicPtrBaseAddr, renownLvlOffsets);

	std::cout << "renownLvlAddr = " << "0x" << std::hex << renownLvlAddr << std::endl;

	int renownLvlValue = 0;
	ReadProcessMemory(hProcess, (BYTE*)renownLvlAddr, &renownLvlValue, sizeof(renownLvlValue), nullptr);
	std::cout << "Current renownLvl = " << std::dec << renownLvlValue << std::endl;

	int newRenownLvl = 10;
	WriteProcessMemory(hProcess, (BYTE*)renownLvlAddr, &newRenownLvl, sizeof(newRenownLvl), nullptr);

	ReadProcessMemory(hProcess, (BYTE*)renownLvlAddr, &renownLvlValue, sizeof(renownLvlValue), nullptr);
	std::cout << "New renownLvl = " << std::dec << renownLvlValue << std::endl;

	getchar();
	return 0;
}
