#include "include.hpp"
#include "injector.hpp"
#include "window.hpp"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, INT nCmdShow) {
	//createWindow(hInstance);
	inject();
	return 0;
}