#include <iostream>

#include "Mineclone/Application.h"
#include "Mineclone/Log.h"

/* Programming guidelines, collapse.
 Folder structure is the same as namespace i.e Mineclone/Application.h
 is in the folder Mineclone/

 Include sorting:
	STL and std headers
 
	3rd-party libraries

	User-defined headers

 Do not use "using namespace std::" 
 Always use #pragma once
*/

// The main entry point. Builds and sets up an Application object.
// Keep the main function as minimalistic as possible.

int main() {
	using namespace Mineclone;

#ifdef _DEBUG
	std::cout << "Debug Mode, all loggers are in debug mode unless else specified.\n";
#endif

	Application application("Mineclone", 1920, 1080);
}