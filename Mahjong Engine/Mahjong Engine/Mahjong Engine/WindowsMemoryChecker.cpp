#include "WindowsMemoryChecker.h"

void Engine::WindowsMemoryChecker::PrintMemoryStatus()
{
	#pragma region MEMORYSTATUSEX Explaination
	//MEMORYSTATUSEX reflects the state of memory at the time of the call. 
	// It also reflects the size of the paging file at that time. 
	// The operating system can enlarge the paging file up to the maximum size set by the 
	// administrator. The physical memory sizes returned include the memory from all nodes.
#pragma endregion
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);

	std::cout << "Available physical memory: " << statex.ullAvailPhys / (static_cast<unsigned long long>(1024) * 1024) << "MB\n";
	std::cout << "Available virtual memory: " << statex.ullAvailVirtual / (static_cast<unsigned long long>(1024) * 1024) << "MB\n";
}