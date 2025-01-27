#include "WindowsMemoryChecker.h"

void Engine::WindowsMemoryChecker::PrintMemoryStatus()
{
	// DEPRECATED: CHECK IN UTILITY_LIBRARY
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);

	std::cout << "Available physical memory: " << statex.ullAvailPhys / (static_cast<unsigned long long>(1024) * 1024) << "MB\n";
	std::cout << "Available virtual memory: " << statex.ullAvailVirtual / (static_cast<unsigned long long>(1024) * 1024) << "MB\n";
}