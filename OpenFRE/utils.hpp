#pragma once
#include "include.hpp"
#include "injector.hpp"

/*
Updates the program
@return 0 if no update needed
@return 1 if program is about to be killed and updated
*/
DWORD updateProgram();