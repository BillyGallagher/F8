#include "stdafx.h"
#include "mem.h"

void mem::PatchMemory(BYTE* dest, BYTE* src, unsigned int size, HANDLE process)
{
	DWORD oldProtect;
	VirtualProtectEx(process, dest, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	WriteProcessMemory(process, dest, src, size, nullptr);
	VirtualProtectEx(process, dest, size, oldProtect, &oldProtect);
}

void mem::NoopMemory(BYTE* dest, unsigned int size, HANDLE process)
{
	BYTE* noops = new BYTE[size];
	memset(noops, 0x90, size);

	PatchMemory(dest, noops, size, process);
	delete[] noops;
}
