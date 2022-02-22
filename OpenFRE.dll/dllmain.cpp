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
#include "pch.h"
#include <lua.hpp>
#include <string>

BOOL APIENTRY DllMain( HMODULE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	HANDLE pipe;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		MSGBOXPARAMS msg = { 0 };
		msg.cbSize = sizeof(MSGBOXPARAMS);
		msg.hwndOwner = NULL;
		msg.hInstance = GetModuleHandle(NULL);
		msg.lpszText = L"DLL successfully injected";
		msg.lpszCaption = L"Injection completed";
		msg.dwStyle = MB_OK | MB_ICONINFORMATION;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MessageBoxIndirect, &msg, NULL, 0);

		while (true) {
			DWORD mode = PIPE_READMODE_MESSAGE, cbRead;
			TCHAR readBuff[BUFSIZ];
			std::string response = "";
			BOOL success = CallNamedPipe(L"\\\\.\\pipe\\openfrecomms", NULL, 0, readBuff, BUFSIZ * sizeof(TCHAR), &cbRead, 20000);

			if (success || GetLastError() == ERROR_MORE_DATA) {
				response += (LPCSTR)readBuff;

				if (!success) MessageBox(NULL, L"data lost", L"debug", MB_OK | MB_ICONERROR);
			}

			MessageBoxA(NULL, response.c_str(), "res", MB_OK | MB_ICONINFORMATION);

			lua_State* luaState;
			luaState = luaL_newstate();

			static const luaL_Reg lualibs[] = {
				{ "base", luaopen_base },
				{ NULL, NULL }
			};

			const luaL_Reg* lib = lualibs;
			while (lib->func != NULL) {
				lib->func(luaState);
				lua_settop(luaState, 0);
				lib++;
			}

			luaL_dostring(luaState, "print(\"Hello, world!\")");

			lua_close(luaState);

			Sleep(100);
		}

		break;
	
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}