#include "utils.hpp"

DWORD updateProgram() {
	cpr::Response response = cpr::Get(cpr::Url{ "https://api.github.com/repos/kevlu8/OpenFRE/releases/latest" });
	if (response.status_code == 200) {
		std::string latest_version = response.text;
		if (latest_version.find("\"tag_name\":\"v") != std::string::npos) {
			latest_version = latest_version.substr(latest_version.find("\"tag_name\":\"v") + 12);
			latest_version = latest_version.substr(0, latest_version.find("\""));
			if (latest_version != OPENFRE_VERSION) {
				remove("openfre-updater.exe");
				MSGBOXPARAMS params = {};
				params.cbSize = sizeof(MSGBOXPARAMS);
				params.hInstance = GetModuleHandle(NULL);
				params.hwndOwner = NULL;
				params.lpszText = "OpenFRE is out of date. Updating OpenFRE in the background...";
				params.lpszCaption = "Updating OpenFRE";
				params.dwStyle = MB_OK | MB_ICONINFORMATION;
				CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MessageBoxIndirect, &params, 0, NULL);

				// now we update the program
				URLDownloadToFile(NULL, "https://github.com/kevlu8/OpenFRE/releases/latest/download/openfre-updater.exe", "openfre-updater.exe", 0, NULL);
				SHELLEXECUTEINFO execInfo = { 0 };
				execInfo.cbSize = sizeof(SHELLEXECUTEINFO);
				execInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
				execInfo.hwnd = NULL;
				execInfo.lpVerb = "open";
				execInfo.lpFile = "openfre-updater.exe";
				execInfo.lpParameters = "";
				execInfo.lpDirectory = NULL;
				execInfo.nShow = SW_HIDE;
				execInfo.hInstApp = NULL;
				ShellExecuteEx(&execInfo);
				if (execInfo.hProcess != NULL) {
					CloseHandle(execInfo.hProcess);
				}
				return 1;
			}
			else return 0;
		}
		else return 0;
	}
	else {
		MessageBoxW(NULL, L"Failed to check for a new version of OpenFRE. Please make sure you're connected to the internet. If you are, please report this as an issue on the GitHub repository.", L"Failed to check for update", MB_OK | MB_ICONERROR);
		return 2;
	}
}