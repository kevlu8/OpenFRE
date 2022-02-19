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

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        MessageBox(NULL, L"DLL successfully injected", L"Injection completed", MB_ICONINFORMATION | MB_OK);
        
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