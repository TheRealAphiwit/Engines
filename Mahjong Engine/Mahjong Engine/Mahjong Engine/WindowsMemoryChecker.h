#pragma once
#include <Windows.h>
#include <iostream>

// What more would a memory check do in the future?
namespace Engine
{
	class WindowsMemoryChecker
	{
		// Current task
		void PrintMemoryStatus();

		// Memory profile/usage tracking
		// Leak detection
		// Memory pooling
		// memory capping (I feel like could be used for when working with less capable PC)
	};
}