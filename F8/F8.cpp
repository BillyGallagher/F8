#include "stdafx.h"
#include "proc.h"
#include <map>
#include <functional>

void DoMenu();
void ClearScreen();

void ExitF8();
void StartRenownLevelMenu();
void StartTeleportMenu();

void SetRenownLevel(int newRenownLvl);
void TeleportPlayer(float x, float y);

uintptr_t worldPtr;
uintptr_t cameraPtr;
HANDLE process;

int main()
{
	DWORD procId = GetProcId(L"fate.exe");
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"fate.exe");

	process = 0;
	process = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	worldPtr = moduleBase + 0x748900; 
	cameraPtr = moduleBase + 0x65E0A8;

	std::cout << "Fate.exe loaded." << std::endl;
	std::cout << "World pointer at: " << "0x" << std::hex << worldPtr << std::endl;
	std::cout << "Camera pointer at: " << "0x" << std::hex << worldPtr << std::endl;



	std::vector<unsigned int> cameraZoomOffsets = { 0x28C, 0x6B0 };
	uintptr_t zoomAddr = FindDMAAddress(process, cameraPtr, cameraZoomOffsets);


	// 250 is half black. fog?
	// 10 clips into player, hitbox makes it difficult to walk
	// 5 is just black
	// zooming out in dungeons doesn't help much due to limited visibility
	float newZoomValue = 200;
	std::cout << "Press any key to begin." << std::endl;
	getchar();

	for (;;)
	{
		WriteProcessMemory(process, (BYTE*)zoomAddr, &newZoomValue, sizeof(newZoomValue), nullptr);
	}


	//DoMenu();

	getchar();
	return 0;
}

void DoMenu()
{
	std::map<std::string, std::function<void()>> menu;
	menu["1"] = &StartRenownLevelMenu;
	menu["2"] = &StartTeleportMenu;
	menu["0"] = &ExitF8;


	ClearScreen();
	for (;;) {
		std::cout << "Select an option (by number):" << std::endl;
		std::cout << "1. Set Renown" << std::endl;
		std::cout << "2. Teleport" << std::endl;
		std::cout << "0. Exit" << std::endl;

		std::string choice;
		std::cin >> choice;

		if (menu.find(choice) == menu.end()) { 
			ClearScreen();
			continue;
		}

		menu[choice]();
		std::cout << std::endl;
	}
}

void SetRenownLevel(int newRenownLvl) 
{
	std::vector<unsigned int> renownLvlOffsets = { 0x80, 0x390 };
	uintptr_t renownLvlAddr = FindDMAAddress(process, worldPtr, renownLvlOffsets);

	int renownLvlValue = 0;
	ReadProcessMemory(process, (BYTE*)renownLvlAddr, &renownLvlValue, sizeof(renownLvlValue), nullptr);
	WriteProcessMemory(process, (BYTE*)renownLvlAddr, &newRenownLvl, sizeof(newRenownLvl), nullptr);
	ReadProcessMemory(process, (BYTE*)renownLvlAddr, &renownLvlValue, sizeof(renownLvlValue), nullptr);
	std::cout << "Set renown level to: " << std::dec << renownLvlValue << std::endl;
}

void TeleportPlayer(float x, float y)
{
	std::vector<unsigned int> xOffsets = { 0x80, 0xE8 };
	uintptr_t xAddr = FindDMAAddress(process, worldPtr, xOffsets);

	std::vector<unsigned int> yOffsets = { 0x80, 0xF0 };
	uintptr_t yAddr = FindDMAAddress(process, worldPtr, yOffsets);

	float xValue = 0, yValue = 0;
	ReadProcessMemory(process, (BYTE*)xAddr, &xValue, sizeof(xValue), nullptr);
	ReadProcessMemory(process, (BYTE*)yAddr, &yValue, sizeof(yValue), nullptr);
	std::cout << "Teleporting from: " << std::dec << xValue << ", " << yValue << std::endl;

	WriteProcessMemory(process, (BYTE*)xAddr, &x, sizeof(x), nullptr);
	WriteProcessMemory(process, (BYTE*)yAddr, &y, sizeof(y), nullptr);

	ReadProcessMemory(process, (BYTE*)xAddr, &xValue, sizeof(xValue), nullptr);
	ReadProcessMemory(process, (BYTE*)yAddr, &yValue, sizeof(yValue), nullptr);
	std::cout << "Teleported to: " << std::dec << xValue << ", " << yValue << std::endl;
}

void StartRenownLevelMenu()
{
	int renownLvl;
	std::cout << "Enter desired renown level:" << std::endl;
	std::cin >> renownLvl;

	SetRenownLevel(renownLvl);
}

void StartTeleportMenu()
{
	float x, y;
	std::cout << "Enter value for X coordinate:" << std::endl;
	std::cin >> x;
	std::cout << "Enter value for Y coordinate:" << std::endl;
	std::cin >> y;

	TeleportPlayer(x, y);
}

void ClearScreen()
{
	int n;
	for (n = 0; n < 10; n++) {
		printf("\n\n\n\n\n\n\n\n\n\n");
	}
}

void ExitF8()
{
	exit(0);
}
