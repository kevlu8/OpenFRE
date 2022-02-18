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

/* 
Creates a blank window.

@param hInstance The HINSTANCE in WinMain()
@return True if success, false if error
*/
bool createWindow(_In_ HINSTANCE hInstance);

/*
Attempt to invalidate if required the window client area.
*/
void invalidateAttempt(_In_ HWND hwnd);

/* 
Paints the window

@param hwnd	The handle to the window
@param goodDc The handle to the DC.
@return True if success, false if error
*/
bool paintWindow(_In_ HWND hwnd);

/*
Callback for WindowProc

@param Don't worry about it.
*/
LRESULT CALLBACK WindowProcHomemade(_In_ HWND hwnd, _In_ UINT msg, _In_ WPARAM wp, _In_ LPARAM lp);

/*
Attempt to click on button.
*/
void attemptClick(_In_ HWND hwnd);

/*
Class for position. Includes x and y value
*/
class Pos {
public:
	int x;
	int y;

	Pos(int x, int y);
};

/*
Class for button. Clickable, and fires an event when clicked
*/
class button {
protected:
	Pos* origin;
	Pos* farPoint;

	HBRUSH brushNormal;
	HBRUSH brushHovered;

	COLORREF colorNormal;
	COLORREF colorHovered;

	LPCWSTR buttonText;

	RECT buttonRect;

public:

	std::function<void()> functionClicked;

	COLORREF buttonColor;

	button(Pos* pos1, Pos* pos2, LPCWSTR text);

	button(); // Default Constructor.

	void draw(_In_ HDC hdc);

	void hover(_In_ HDC hdc);

	void press();

	Pos* getPosOrigin();

	Pos* getPosFarPoint();

	RECT* getRect();
};

/*
Register button for further repaints.
*/
void registerButton(_In_ button buttonToReg);

/*
Checks if the mouse is in the given button.
*/
bool isMouseInButton(_In_ button toTest, _In_ LONG mx, _In_ LONG my);
