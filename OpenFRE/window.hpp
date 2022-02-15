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

/* 
Creates a blank window.

@param hInstance The HINSTANCE in WinMain()
@return True if success, false if error
*/
bool createWindow(_In_ HINSTANCE hInstance);

/* 
Paints the window

@param hwnd	The handle to the window
@return True if success, false if error
*/
bool paintWindow(_In_ HWND hwnd);

/*
Callback for WindowProc

@param Don't worry about it.
*/
LRESULT CALLBACK WindowProcHomemade(_In_ HWND hwnd, _In_ UINT msg, _In_ WPARAM wp, _In_ LPARAM lp);

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
public:
	COLORREF buttonColor;
	HDC hdc;

	button(Pos* pos1, Pos* pos2);

	void draw();

	void hover();

	void press(void (*function)());
};