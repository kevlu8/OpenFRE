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
#include "utils.hpp"

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
) {
	HANDLE update = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)updateProgram, NULL, NULL, NULL);
	DWORD exitCode = 3;
	while (exitCode != 1 && exitCode != 2 && exitCode != 0) {
		GetExitCodeThread(update, &exitCode);
		Sleep(100);
	}

	if (exitCode == 1) {
		exit(0);
	}
	
	bool result = createWindow(hInstance);

	if (!result)
		MessageBoxW(NULL, L"We couldn't create the window. Please report this as an issue on the GitHub repository.", L"Failed to create window", MB_OK | MB_ICONERROR);

	return 0;
}
