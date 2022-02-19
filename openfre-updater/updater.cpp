#include <Windows.h>
#include <TlHelp32.h>
#include <comdef.h>

int main() {
	URLDownloadToFile(NULL, L"https://github.com/kevlu8/OpenFRE/releases/latest/download/OpenFRE.dll", L"OpenFRE.dll.temp", 0, NULL);
	URLDownloadToFile(NULL, L"https://github.com/kevlu8/OpenFRE/releases/latest/download/OpenFRE.exe", L"OpenFRE.exe.temp", 0, NULL);

	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(PROCESSENTRY32);
	
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	bool procFound = false;

	if (Process32First(snap, &procEntry) == TRUE) {
		while (Process32Next(snap, &procEntry) == TRUE) {
			WCHAR* wc = procEntry.szExeFile;
			_bstr_t b(wc);
			const char* c = b;
			if (_stricmp(c, "OpenFRE.exe") == 0) {
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry.th32ProcessID);
				TerminateProcess(hProcess, 0);
				CloseHandle(hProcess);
				procFound = true;
			}
		}
	}

	remove("OpenFRE.dll");
	rename("OpenFRE.dll.temp", "OpenFRE.dll");
	remove("OpenFRE.exe");
	rename("OpenFRE.exe.temp", "OpenFRE.exe");

	CloseHandle(snap);

	ShellExecute(NULL, L"open", L"OpenFRE.exe", NULL, NULL, SW_HIDE);

	return 0;
}