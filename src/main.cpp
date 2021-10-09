#include "pch.h"
#include "Core.h"
#include <ctime>
#include <iostream>

void StartHeapControl();
void DumpMemoryLeaks();

int main( int argc, char *argv[] )
{
	srand(static_cast<unsigned int>(time(nullptr)));
	{
		StartHeapControl();

		try
		{
			Core core{ Window{ "Super Paper Mario - De Ridder, Jonas - 1DAE18", 1000.0f, 750.0f } };
			core.Run();
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << '\n';
			std::cout << "Press ENTER to finish the application" << '\n';
			std::cin.get();
		}
		catch (...)
		{
			std::cout << "An unrecognizable error occured\n Press ENTER to finish the application" << '\n';
			std::cin.get();
		}
		
	}
	DumpMemoryLeaks();
	return 0;
}


void StartHeapControl()
{
#if defined(DEBUG) | defined(_DEBUG)
	// Notify user if heap is corrupt
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	// Report detected leaks when the program exits
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Set a breakpoint on the specified object allocation order number
	//_CrtSetBreakAlloc(573);
#endif
}

void DumpMemoryLeaks()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif
}


