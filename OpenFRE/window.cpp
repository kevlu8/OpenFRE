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

const int MAX_BUTTONS = 16;

int curTrackLen = 0;
button buttons[MAX_BUTTONS];
bool isDrawing = false;



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
	Pos buttonPosition(200, 200);
	Pos buttonFarPoint(300, 225);
	button injectBtn(&buttonPosition, &buttonFarPoint, L"Inject !");
	injectBtn.buttonColor = 0x282828;
	injectBtn.functionClicked = &inject;
	registerButton(injectBtn);

	ShowWindow(hwnd, SW_SHOWNORMAL);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

bool isMouseInButton(_In_ button toTest, _In_ LONG mx, _In_ LONG my) {
	return 
		mx > toTest.getPosOrigin()->x and // x check start
		mx < toTest.getPosFarPoint()->x and // x check end
		my > toTest.getPosOrigin()->y and // y check start
		my < toTest.getPosFarPoint()->y; // y check end
	//---------------------------------------
}

bool paintWindow(_In_ HWND hwnd) {
	POINT cursorPos = {};
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
	if (GetCursorPos(&cursorPos) && ScreenToClient(hwnd, &cursorPos)) {
		for (button toDraw : buttons) {
			if (!isMouseInButton(toDraw, cursorPos.x, cursorPos.y)) {
				toDraw.draw(hdc);
			} else {
				toDraw.hover(hdc);
			}
		}
	}
	EndPaint(hwnd, &ps);
	ReleaseDC(hwnd, hdc);
	
	return true;
}

void invalidateAttempt(_In_ HWND hwnd) {
	POINT cursorPos = {};
	if (GetCursorPos(&cursorPos) && ScreenToClient(hwnd, &cursorPos)) {
		for (button toDraw : buttons) {
			LPRECT tdRect = toDraw.getRect();
			if (!isMouseInButton(toDraw, cursorPos.x, cursorPos.y)) {
				InvalidateRect(hwnd, tdRect, true);
			}
			else {
				InvalidateRect(hwnd, tdRect, true);
			}
		}
	}
}

void attemptClick(_In_ HWND hwnd) {
	POINT cursorPos = {};
	if (GetCursorPos(&cursorPos) && ScreenToClient(hwnd, &cursorPos)) {
		for (button toDraw : buttons) {
			LPRECT tdRect = toDraw.getRect();
			if (isMouseInButton(toDraw, cursorPos.x, cursorPos.y)) {
				toDraw.press();
			}
		}
	}
}

LRESULT CALLBACK WindowProcHomemade(_In_ HWND hwnd, _In_ UINT msg, _In_ WPARAM wp, _In_ LPARAM lp) {
	switch (msg) {
	case WM_PAINT: {
		if (!isDrawing) {
			isDrawing = true;
			paintWindow(hwnd);
			isDrawing = false;
		}
		return 0;
	}
	case WM_MOUSEMOVE: {
		if (!isDrawing) {
			isDrawing = true;
			invalidateAttempt(hwnd);
			isDrawing = false;
		}
		return 0;
	}
	case WM_LBUTTONDOWN: {
		if (!isDrawing) {
			isDrawing = true;
			attemptClick(hwnd);
			isDrawing = false;
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

void registerButton(_In_ button buttonToReg) {
	if (curTrackLen + 1 >= MAX_BUTTONS) {
		errorMsg("Buttons", "Too many buttons registred.");
	}
	buttons[curTrackLen] = buttonToReg;
	curTrackLen = curTrackLen + 1;
}

// Definition of Pos
Pos::Pos(int x, int y) {
	this->x = x;
	this->y = y;
};

// ******************************************
// DEFINITION OF BUTTON CLASS    -    START
// ******************************************

void empty() {

}

button::button(Pos* pos1, Pos* pos2, LPCWSTR text) {
	this->buttonColor = 0x000000;
	this->buttonText = text;
	this->functionClicked = &empty;
	this->origin = pos1;
	this->farPoint = pos2;
	this->colorNormal = this->buttonColor;
	this->brushNormal = CreateSolidBrush(this->colorNormal);
	this->colorHovered = RGB(GetRValue(this->buttonColor) + 70, GetGValue(this->buttonColor) + 70, GetBValue(this->buttonColor) + 70);
	this->brushHovered = CreateSolidBrush(colorHovered);
	RECT tdRect = {};
	tdRect.left = pos1->x;
	tdRect.right = pos2->x;
	tdRect.top = pos1->y;
	tdRect.bottom = pos2->y;
	this->buttonRect = tdRect;
}

button::button() : button(new Pos(0, 0), new Pos(0, 0), L"") {} // default constructor


void button::draw(_In_ HDC hdc) {
	SelectObject(hdc, this->brushNormal);
	Rectangle(hdc, this->origin->x, this->origin->y, this->farPoint->x, this->farPoint->y);
	SetBkColor(hdc, this->colorNormal);
	SetTextColor(hdc, 0xffffff);
	DrawTextW(hdc, this->buttonText, -1, &this->buttonRect, DT_CENTER || DT_NOCLIP);
}

void button::hover(_In_ HDC hdc) {
	SelectObject(hdc, this->brushHovered);
	Rectangle(hdc, this->origin->x, this->origin->y, this->farPoint->x, this->farPoint->y);
	SetBkColor(hdc, this->colorHovered);
	SetTextColor(hdc, 0xffffff);
	DrawTextW(hdc, this->buttonText, -1, &this->buttonRect, DT_CENTER || DT_NOCLIP);
}

void button::press() {
	functionClicked();
}

Pos* button::getPosOrigin() {
	return this->origin;
}

Pos* button::getPosFarPoint() {
	return this->farPoint;
}

RECT* button::getRect() {
	return &this->buttonRect;
}

// ******************************************
// DEFINITION OF BUTTON CLASS    -    END
// ******************************************
