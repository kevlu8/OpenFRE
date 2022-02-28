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
HANDLE pipe = NULL;
HMENU menu = NULL;
HINSTANCE curInstance = NULL;

bool createWindow(_In_ HINSTANCE hInstance) {

	curInstance = hInstance;

	pipe = CreateNamedPipeA("\\\\.\\pipe\\openfrecomms", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE, 2, 8092000, 8092000, 0, NULL);

	if (pipe == NULL) {
		MessageBoxW(NULL, L"Communication between OpenFRE and Roblox could not be established. Please report this as an issue on the GitHub repository.", L"Failed to establish pipe", MB_OK | MB_ICONERROR);
		return false;
	}

	WNDCLASS wc = {};

	menu = LoadMenuW(hInstance, MAKEINTRESOURCEW(IDR_WINDOWMENU));
	if (menu == NULL || menu == INVALID_HANDLE_VALUE) {
		return false;
	}

	wc.lpfnWndProc = WindowProcHomemade;
	wc.hInstance = hInstance;
	wc.lpszClassName = "OpenFRE Script Injector";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	RegisterClass(&wc);

	INITCOMMONCONTROLSEX structInit = {};
	structInit.dwICC = ICC_STANDARD_CLASSES;
	structInit.dwSize = sizeof(structInit);
	if (!InitCommonControlsEx(&structInit)) {
		return false;
	}

	HWND hwnd = CreateWindowExW(
		0,
		L"OpenFRE Script Injector",
		L"OpenFRE Executor",
		WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | BS_DEFPUSHBUTTON,
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 320,
		NULL,
		menu,
		hInstance,
		NULL
	);

	HMODULE dll = LoadLibraryW(L"Msftedit.dll");
	if (dll == NULL) {
		return false;
	}
	if (hwnd == NULL) {
		return false;
	}

	inputText = CreateWindowW(MSFTEDIT_CLASS, L"print(\"Hello World !\")",  WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL | ES_SUNKEN | WS_HSCROLL, 0, 0, 500, 200, hwnd, NULL, hInstance, NULL);
	
	if (inputText == NULL) {
		return false;
	}

	SendMessageW(inputText, EM_SETEVENTMASK, NULL, (LPARAM)ENM_CHANGE);

	runBtn = CreateWindowW(WC_BUTTONW, L"Run", WS_VISIBLE | WS_CHILD, 10, 210, 80, 30, hwnd, NULL, hInstance, NULL);
	exitBtn = CreateWindowW(WC_BUTTONW, L"Exit", WS_VISIBLE | WS_CHILD, 100, 210, 80, 30, hwnd, NULL, hInstance, NULL);
	injectBtn = CreateWindowW(WC_BUTTONW, L"Inject", WS_VISIBLE | WS_CHILD, 190, 210, 80, 30, hwnd, NULL, hInstance, NULL);

	if (runBtn == NULL || exitBtn == NULL || injectBtn == NULL) {
		return false;
	}

	EnableWindow(runBtn, FALSE);

	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);

	textColorization();

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

void textColorization() {
	std::map<std::wstring, unsigned int> markdownSyntax = {
								{ L"and", 0xff0000 },
								{ L"break", 0xff0000 },
								{ L"do", 0xff0000 },
								{ L"else", 0xff0000 },
								{ L"elseif", 0xff0000 },
								{ L"end", 0xff0000 },
								{ L"false", 0x0000ff },
								{ L"for", 0xff0000 },
								{ L"function", 0x0000ff },
								{ L"if", 0xff0000 },
								{ L"in", 0xff0000 },
								{ L"local", 0xff0000 },
								{ L"nil", 0xff0000 },
								{ L"not", 0xff0000 },
								{ L"or", 0xff0000 },
								{ L"repeat", 0xff0000 },
								{ L"return", 0xff0000 },
								{ L"then", 0xff0000 },
								{ L"true", 0x0000ff },
								{ L"until", 0xff0000 },
								{ L"while", 0xff0000 },
								// other
								{ L"numbers", 0x00ff00 },
	};

	int codeLength = GetWindowTextLengthW(inputText);
	LPWSTR buff = (LPWSTR)VirtualAlloc(NULL, (SIZE_T)codeLength + 1, MEM_COMMIT, PAGE_READWRITE);
	if (buff != NULL) {
		GetWindowTextW(inputText, buff, codeLength + 1);
		std::wstring wStrConv = buff;
		size_t ezPos = 0;
		CHARRANGE rangeSel = {};
		CHARRANGE rangeDef = {};
		CHARFORMAT2 cf = {};
		cf.dwMask = CFM_COLOR;
		cf.cbSize = sizeof(cf);
		SendMessageW(inputText, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
		SendMessageW(inputText, EM_EXGETSEL, NULL, (LPARAM)&rangeDef);
		for (const auto& [key, value] : markdownSyntax) {
			//MessageBoxW(NULL, &key[0], L"EE", MB_OK);
			ezPos = 0;
			while (true) {
				cf = {};
				rangeSel = {};
				ezPos = wStrConv.find(key, ezPos);
				if (ezPos == std::wstring::npos)
					break;
				rangeSel.cpMin = ezPos;
				rangeSel.cpMax = ezPos + key.size();
				SendMessageW(inputText, EM_EXSETSEL, NULL, (LPARAM)&rangeSel);
				SendMessageW(inputText, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
				cf.dwMask = CFM_COLOR;
				cf.crTextColor = (unsigned int)value;
				cf.cbSize = sizeof(cf);
				SendMessageW(inputText, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
				ezPos = ezPos + key.size();
			}
			//MessageBoxW(NULL, L"SEND3", L"EE", MB_OK);
		}
		SendMessageW(inputText, EM_EXSETSEL, NULL, (LPARAM)&rangeDef);
		VirtualFree(buff, NULL, MEM_RELEASE);
		//MessageBoxW(NULL, L"SEND2", L"EE", MB_OK);
	}
	//MessageBoxW(NULL, L"SEND", L"EE", MB_OK);
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
		switch (LOWORD(wp)) {
		case ID_FILE_EXIT: {
			DestroyWindow(hwnd);
			return 0;
		}
		case ID__ABOUT: {
			// Not implemented yet.
			ShellExecuteA(NULL, "open", "https://github.com/kevlu8/OpenFRE", NULL, NULL, SW_HIDE);
			return 0;
		}
		default:
			switch (HIWORD(wp)) {
			case BN_CLICKED: {
				HWND btn = (HWND)lp;
				if (btn == exitBtn) {
					DestroyWindow(hwnd);
				}
				if (btn == injectBtn) {
					EnableMenuItem(GetSubMenu(menu, 0), ID_FILE_EXIT, MF_DISABLED);
					EnableWindow(exitBtn, FALSE);
					EnableWindow(injectBtn, FALSE);
					HANDLE dllProc = NULL;
					bool injected = true;
					if (!inject(dllProc)) {
						MessageBoxW(hwnd, L"Failed to inject code into Roblox. Please make sure it is open.", L"Failed to inject", MB_OK | MB_ICONERROR);
						EnableWindow(injectBtn, TRUE);
					}
					else {
						if (!pipe) {
							MessageBoxW(hwnd, L"Communication between OpenFRE and Roblox could not be established. Please report this as an issue on the GitHub repository.", L"Failed to establish pipe", MB_OK | MB_ICONERROR);
							injected = false;
						}
					}
					if (injected) {
						EnableWindow(runBtn, TRUE);
					}
					else {
						EnableWindow(injectBtn, TRUE);
					}

					EnableMenuItem(GetSubMenu(menu, 0), ID_FILE_EXIT, MF_ENABLED);
					EnableWindow(exitBtn, TRUE);
				}
				if (btn == runBtn) {
					int codeLength = GetWindowTextLengthW(inputText);
					LPWSTR buff = (LPWSTR)VirtualAlloc(NULL, (SIZE_T)codeLength + 1, MEM_COMMIT, PAGE_READWRITE);
					if (buff != NULL) {
						GetWindowTextW(inputText, buff, codeLength + 1);
						DWORD out;
						void* thing = &buff;
						BOOL success = TransactNamedPipe(pipe, thing, codeLength + 1, NULL, 0, &out, NULL);
						if (!success) MessageBoxW(hwnd, buff, L"Code", MB_OK | MB_ICONINFORMATION); // currently diong this
					}
				}
				return 0;
			}
			case EN_CHANGE: {
				HWND tb = (HWND)lp;
				if (tb == inputText) {
					textColorization();
				}
				return 0;
			}
			default:
				return 0;
			}
			return 0;
		}
	}
		
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wp, lp);
	}
}

HANDLE getPipe() {
	return pipe;
}