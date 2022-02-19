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
#define OPENFRE_VERSION 0.1.0
#include "include.hpp"
#include "injector.hpp"
#include "window.hpp"

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
) {
	// Check for a new version of OpenFRE on GitHub under the "releases" tab
	cpr::Response response = cpr::Get(cpr::Url{"https://api.github.com/repos/kevlu8/OpenFRE/releases/latest"});
	if (response.status_code == 200) {
		std::string latest_version = response.text;
		if (latest_version.find("\"tag_name\":\"v") != std::string::npos) {
			latest_version = latest_version.substr(latest_version.find("\"tag_name\":\"v") + 12);
			latest_version = latest_version.substr(0, latest_version.find("\""));
			if (latest_version != OPENFRE_VERSION) {
				MESSAGEBOXPARAMS params = {};
				params.cbSize = sizeof(MESSAGEBOXPARAMS);
				params.hInstance = GetModuleHandle(NULL);
				params.hwndOwner = NULL;
				params.lpszText = "OpenFRE is out of date. Current version: " + OPENFRE_VERSION + "\nLatest version: " + latest_version + "\n\nUpdating OpenFRE in the background...";
				params.lpszCaption = "Updating OpenFRE";
				params.dwStyle = MB_OK | MB_ICONINFORMATION;
				CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MessageBoxIndirect, &params, 0, NULL);
				auto updateThread = []() {
					
				}
			}
		}
	} else {
		errorMsg("Failed to check for a new version of OpenFRE. Please make sure you're connected to the internet. If you are, please report this as an issue on the GitHub repository.");
	}
	
	bool result = createWindow(hInstance);

	if (!result)
		errorMsg("Failed to create window", "We couldn't create the window. Please report this as an issue on the GitHub repository.");

	return 0;
}
