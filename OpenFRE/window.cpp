#include "window.hpp"

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

	ShowWindow(hwnd, SW_SHOWNORMAL);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

bool paintWindow(_In_ HWND hwnd) {
	Pos buttonPosition(200, 200);
	Pos buttonFarPoint(250, 225);
	button inject(&buttonPosition, &buttonFarPoint);
	inject.buttonColor = { 0x282828 };
	inject.hdc = GetDC(hwnd);
	inject.draw();
	return true;
}

LRESULT CALLBACK WindowProcHomemade(_In_ HWND hwnd, _In_ UINT msg, _In_ WPARAM wp, _In_ LPARAM lp) {
	switch (msg) {
	case WM_PAINT:
		paintWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wp, lp);
	}
}

LRESULT CALLBACK SubProcHomemade(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_MOUSEMOVE: {
		TRACKMOUSEEVENT ev = {};
		ev.cbSize = sizeof(TRACKMOUSEEVENT);
		ev.dwFlags = TME_HOVER | TME_LEAVE;
		ev.hwndTrack = hwnd;
		ev.dwHoverTime = HOVER_DEFAULT;
		TrackMouseEvent(&ev);
	}
	case WM_MOUSEHOVER: {
		HDC hdc = GetDC(hwnd);
		SetDCBrushColor(hdc, RGB(20, 20, 20));

		Rectangle(hdc, 200, 200, 250, 225);
	}
	case WM_MOUSELEAVE: {
		HDC hdc = GetDC(hwnd);
		SetDCBrushColor(hdc, RGB(0, 255, 0));

		SelectObject(hdc, GetStockObject(DC_BRUSH));

		Rectangle(hdc, 200, 200, 250, 225);
		TRACKMOUSEEVENT ev = {};
		ev.cbSize = sizeof(TRACKMOUSEEVENT);
		ev.dwFlags = TME_HOVER | TME_LEAVE | TME_CANCEL;
		ev.hwndTrack = hwnd;
		ev.dwHoverTime = HOVER_DEFAULT;
		TrackMouseEvent(&ev);
	}
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

Pos::Pos(int x, int y) {
	this->x = x;
	this->y = y;
};

button::button(Pos* pos1, Pos* pos2) {
	this->origin = pos1;
	this->farPoint = pos2;
}

void button::draw() {
	SelectObject(button::hdc, CreateSolidBrush(button::buttonColor));
	Rectangle(hdc, this->origin->x, this->origin->y, this->farPoint->x, farPoint->y);
}

void button::hover() {
	Rectangle(hdc, this->origin->x, this->origin->y, this->farPoint->x, farPoint->y);
}

void button::press(void (*function)()) {
	function();
}