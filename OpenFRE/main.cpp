//  OpenFRE - The free Roblox executor
//  Copyright (C) 2022  kevlu8
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#include "include.hpp"
#include "injector.hpp"
#include "window.hpp"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, INT nCmdShow) {
	HANDLE wndThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)createWindow, &hInstance, NULL, NULL);
	
	if (!wndThread)
		errorMsg("Failed to create window", "We couldn't create the window. Please report this as an issue on the GitHub repository.");

	Sleep(60000);

	inject();
	return 0;
}