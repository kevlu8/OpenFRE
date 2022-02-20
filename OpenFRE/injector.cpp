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
#include "injector.hpp"

void getProcID(_In_ LPCSTR windowTitle, _Out_ DWORD& processID) {
	GetWindowThreadProcessId(FindWindow(NULL, windowTitle), &processID);
}

bool fileExists(_In_ std::string fileName) {
	struct stat buffer;
	return (stat(fileName.c_str(), &buffer) == 0);
}

bool inject(_Out_ HANDLE &hThread) {
	DWORD procID = NULL;
	char dllPath[MAX_PATH];
	const char* dllName = "OpenFRE.dll";
	const char* windowTitle = "Roblox";

	if (!fileExists(dllName))
		return false;

	if (!GetFullPathName(dllName, MAX_PATH, dllPath, nullptr))
		return false;

	getProcID(windowTitle, procID);
	if (procID == NULL)
		return false;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procID);

	if (!hProcess)
		return false;

	void* allocatedMemory = VirtualAllocEx(hProcess, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	if (!allocatedMemory)
		return false;

	if (!WriteProcessMemory(hProcess, allocatedMemory, dllPath, MAX_PATH, nullptr))
		return false;

	hThread = CreateRemoteThread(hProcess, NULL, NULL, LPTHREAD_START_ROUTINE(LoadLibraryA), allocatedMemory, NULL, NULL);

	if (!hThread)
		return false;

	CloseHandle(hProcess);
	VirtualFreeEx(hProcess, allocatedMemory, NULL, MEM_RELEASE);
	return true;
}