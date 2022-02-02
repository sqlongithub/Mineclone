#include <string>
#include <iostream>

#include "Mineclone/Application.h"

/*
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

	Application application("", 1920, 1080);

}