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
        MessageBox(NULL, L"DLL successfully injected", L"Injection completed", MB_ICONINFORMATION | MB_OK);

        pipe = CreateFile(L"\\\\.\\pipe\\openfrecomms", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        
        while (true) {
            if (pipe != INVALID_HANDLE_VALUE) break;

            if (GetLastError() != ERROR_PIPE_BUSY) MessageBox(NULL, L"Pipe busy", L"debug", MB_OK | MB_ICONERROR);

            Sleep(100);
        }

        // pipe is now connected
        DWORD mode = PIPE_READMODE_MESSAGE, cbRead;
        BOOL success = SetNamedPipeHandleState(pipe, &mode, NULL, NULL);
        TCHAR readBuff[BUFSIZ];
        std::string response = "";

        if (!success)
            MessageBox(NULL, L"Failed to set pipe", L"debug", MB_OK | MB_ICONINFORMATION);

        success = TransactNamedPipe(pipe, NULL, 0, readBuff, BUFSIZ * sizeof(TCHAR), &cbRead, NULL);

        if (!success && (GetLastError() != ERROR_MORE_DATA))
            MessageBox(NULL, L"Failed to transact pipe", L"debug", MB_OK | MB_ICONERROR);

        while (true) {
            if (!success) break;

            success = ReadFile(pipe, readBuff, BUFSIZ * sizeof(TCHAR), &cbRead, NULL);

            if (!success && (GetLastError() != ERROR_MORE_DATA)) break;

            response += (LPCSTR)readBuff;
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

        break;
    
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}