#include "injector.hpp"

void getProcID(_In_ LPCSTR windowTitle, _Out_ DWORD& processID) {
	GetWindowThreadProcessId(FindWindow(NULL, windowTitle), &processID);
}

void errorMsg(_In_ LPCSTR errorTitle, _In_ LPCSTR errorMsg) {
	MessageBox(NULL, errorMsg, errorTitle, MB_ICONERROR | MB_OK);
	exit(-1);
}

bool fileExists(_In_ std::string fileName) {
	struct stat buffer;
	return (stat(fileName.c_str(), &buffer) == 0);
}

bool inject() {
	DWORD procID = NULL;
	char dllPath[MAX_PATH];
	const char* dllName = "OpenFRE.dll";
	const char* windowTitle = /*L"Roblox"*/ "Untitled - Paint";

	if (!fileExists(dllName))
		errorMsg("DLL not found!", "OpenFRE.dll was not found. Please disable your antivirus or add OpenFRE to your exclusions.");

	if (!GetFullPathName(dllName, MAX_PATH, dllPath, nullptr))
		errorMsg("Failed to check full path of DLL", "Couldn't access the full path to OpenFRE.dll. Check to see if there is corruption in folder names.");

	getProcID(windowTitle, procID);
	if (procID == NULL)
		errorMsg("Roblox not found.", "Couldn't find Roblox. Are you sure it's open?");

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procID);

	if (!hProcess)
		errorMsg("Couldn't access Roblox", "Failed to open a handle to the process. Please disable your antivirus or add OpenFRE to your exclusions.");

	void* allocatedMemory = VirtualAllocEx(hProcess, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	if (!allocatedMemory)
		errorMsg("Couldn't allocate memory", "Failed to access Roblox's memory. Please disable your antivirus or add OpenFRE to your exclusions.");

	if (!WriteProcessMemory(hProcess, allocatedMemory, dllPath, MAX_PATH, nullptr))
		errorMsg("Couldn't write to memory", "Failed to write to Roblox's memory. Please disable your antivirus or add OpenFRE to your exclusions.");

	HANDLE hThread = CreateRemoteThread(hProcess, /*nullptr*/NULL, NULL, LPTHREAD_START_ROUTINE(LoadLibraryA), allocatedMemory, NULL, /*nullptr*/NULL);

	if (!hThread)
		errorMsg("Couldn't create thread", "Failed to create a remote thread within Roblox.");

	CloseHandle(hProcess);
	VirtualFreeEx(hProcess, allocatedMemory, NULL, MEM_RELEASE);
	MessageBox(NULL, "Successfully injected DLL into Roblox.", "Injected", MB_ICONINFORMATION | MB_OK);
	return true;
}