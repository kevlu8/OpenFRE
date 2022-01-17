#include "include.hpp"
#include "injector.hpp"
#include "window.hpp"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, INT nCmdShow) {
	HANDLE wndThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)createWindow, &hInstance, NULL, NULL);
	
	if (!wndThread)
		errorMsg("Failed to create window", "We couldn't create the window. Please report this as an issue on the GitHub repository.");

	inject();
	return 0;
}