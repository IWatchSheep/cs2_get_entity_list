#include "memory.h"
#include <thread>

// i hide the default folders. i made three new folders: cheat, output, output\intermediates
// project property settings
/*
* configuration = all configuration, platform = win32, solution configuration = release, solution platforms = x86
*
* general: output directory, i set this to the output folder i made. intermediate directory, i set this to the intermediate directory i made.
* configuration type = application(.exe), language standard = iso c++ 20 standard
*
* advanced: character set = multi-byte
*
* linker: subsystem = Console (/SUBSYSTEM:CONSOLE)
*
*/

int main()
{
		
	auto mem = Memory("cs2.exe");
	
	std::cout << "Process id: " << mem.GetProcessId() << std::endl;

	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, NULL, mem.GetProcessId());

	const auto client = reinterpret_cast<uintptr_t>(GetModuleHandleA("client.dll"));
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x186EE00; // in offsets.hpp
	const auto localPlayer = client + dwLocalPlayerPawn;

	constexpr std::ptrdiff_t dwEntityList = 0x1A1A7E8; // in offsets.hpp
	auto baseAddress = mem.GetModuleAddress("client.dll");
	const auto entitylist = baseAddress + dwEntityList;

	constexpr std::ptrdiff_t m_hPlayerPawn = 0x80C; // in client_dll.hpp
	constexpr std::ptrdiff_t m_iHealth = 0x344; // in client_dll.hpp
	constexpr std::ptrdiff_t m_iTeamNum = 0x3E3; // in client_dll.hpp
	constexpr std::ptrdiff_t m_sSanitizedPlayerName = 0x770; // in client_dll.hpp	

	
	while (!GetAsyncKeyState(VK_DELETE))
	{
		for (int i = 0; i <= 32; i++)
		{
			uintptr_t Entity = mem.Read<uintptr_t>(baseAddress + dwEntityList);
			if (!Entity)
				continue;

			uintptr_t listEntity = mem.Read<uintptr_t>(Entity + ((8 * (i & 0x7FFF) >> 9) + 16));
			if (listEntity == 0)
				continue;

			uintptr_t entityController = mem.Read<uintptr_t>(listEntity + (120) * (i & 0x1FF));
			if (entityController == 0)
				continue;

			uintptr_t entityControllerPawn = mem.Read<uintptr_t>(entityController + m_hPlayerPawn);
			if (entityControllerPawn == 0)
				continue;

			listEntity = mem.Read<uintptr_t>(Entity + (0x8 * ((entityControllerPawn & 0x7FFF) >> 9) + 16));
			if (listEntity == 0)
				continue;

			uintptr_t entityPawn = mem.Read<uintptr_t>(listEntity + (120) * (entityControllerPawn & 0x1FF));
			if (entityPawn == 0)
				continue;

			int health = mem.Read<int>(entityPawn + m_iHealth);

			if (health <= 0 || health > 100)
				continue;

			std::cout << "INDEX PLAYER: " << i << " " << "HP: " << health << std::endl;
		}
	}
	

	// the bottom code in commented section shows playerTeam, and entityName too, 
	// i commented it out because i get the health in the for loop above
	// if i need playerTeam, and entityName, i know where to get it, in the commented code below.
	
	/*
	while (!GetAsyncKeyState(VK_DELETE))
	{
		for (int i = 0; i < 64; i++)
		{
			uintptr_t Entity = mem.Read<uintptr_t>(baseAddress + dwEntityList);

			uintptr_t listEntity = mem.Read<uintptr_t>(Entity + ((8 * (i & 0x7FFF) >> 9) + 16));

			if (listEntity == 0)
				continue;

			uintptr_t entityController = mem.Read<uintptr_t>(listEntity + (120) * (i & 0x1FF));
			if (entityController == 0)
				continue;

			uintptr_t entityControllerPawn = mem.Read<uintptr_t>(entityController + m_hPlayerPawn);
			if (entityControllerPawn == 0)
				continue;

			listEntity = mem.Read<uintptr_t>(Entity + (0x8 * ((entityControllerPawn & 0x7FFF) >> 9) + 16));
			if (listEntity == 0)
				continue;

			uintptr_t entityPawn = mem.Read<uintptr_t>(listEntity + (120) * (entityControllerPawn & 0x1FF));
			if (entityPawn == 0)
				continue;

			int playerHealth = mem.Read<int>(entityPawn + m_iHealth);
			std::cout << "playerHealth: " << playerHealth << std::endl;

			int playerTeam = mem.Read<int>(entityPawn + m_iTeamNum);
			std::cout << "playerTeam: " << playerTeam << std::endl;

			uintptr_t entityNameAddress = mem.Read<uintptr_t>(entityController + m_sSanitizedPlayerName);
			int entityName = mem.Read<char>(entityNameAddress);
			std::cout << "entityName: " << mem.Read<char>(entityNameAddress) << std::endl << std::endl;
		}
	}
	*/	
}