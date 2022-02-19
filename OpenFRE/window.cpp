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
#include "window.hpp"

HWND runBtn = NULL;
HWND exitBtn = NULL;
HWND injectBtn = NULL;
HWND inputText = NULL;

bool createWindow(_In_ HINSTANCE hInstance) {

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProcHomemade;
	wc.hInstance = hInstance;
	wc.lpszClassName = "OpenFRE Script Injector";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	RegisterClass(&wc);
	
	HWND hwnd = CreateWindowExW(
		0,
		L"OpenFRE Script Injector",
		L"OpenFRE Executor",
		WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | BS_DEFPUSHBUTTON,
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 300,
		NULL,
		NULL,
		hInstance,
		NULL
	);


	if (hwnd == NULL) {
		return false;
	}

	inputText = CreateWindowW(L"Edit", L"print(\"Hello World !\")", WS_BORDER | WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 0, 0, 500, 200, hwnd, NULL, hInstance, NULL);
	
	if (inputText == NULL) {
		return false;
	}

	runBtn = CreateWindowW(L"Button", L"Run", WS_BORDER | WS_VISIBLE | WS_CHILD, 10, 210, 80, 30, hwnd, NULL, hInstance, NULL);
	exitBtn = CreateWindowW(L"Button", L"Exit", WS_BORDER | WS_VISIBLE | WS_CHILD, 100, 210, 80, 30, hwnd, NULL, hInstance, NULL);
	injectBtn = CreateWindowW(L"Button", L"Inject", WS_BORDER | WS_VISIBLE | WS_CHILD, 190, 210, 80, 30, hwnd, NULL, hInstance, NULL);

	if (runBtn == NULL || exitBtn == NULL || injectBtn == NULL) {
		return false;
	}

	ShowWindow(hwnd, SW_SHOWNORMAL);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

LRESULT CALLBACK WindowProcHomemade(_In_ HWND hwnd, _In_ UINT msg, _In_ WPARAM wp, _In_ LPARAM lp) {
	switch (msg) {
	case WM_PAINT: {
		PAINTSTRUCT paintStruct = {};
		HDC hdc = BeginPaint(hwnd, &paintStruct);
		FillRect(hdc, &paintStruct.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &paintStruct);
		return 0;
	}
	case WM_COMMAND: {
		switch (HIWORD(wp)) {
		case BN_CLICKED: {
			HWND btn = (HWND)lp;
			if (btn == exitBtn) {
				DestroyWindow(hwnd);
			}
			if (btn == injectBtn) {
				inject();
			}
			if (btn == runBtn) {
				int codeLength = GetWindowTextLengthW(inputText);
				LPWSTR buff = (LPWSTR)VirtualAlloc(NULL, (SIZE_T)codeLength + 1, MEM_COMMIT, PAGE_READWRITE);
				if (buff != NULL) {
					GetWindowTextW(inputText, buff, codeLength + 1);
					// implement so code-run here.
					MessageBoxW(hwnd, buff, L"Code", MB_OK | MB_ICONINFORMATION);
				}
			}
			return 0;
		}
		default:
			break;
		}
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wp, lp);
	}
}