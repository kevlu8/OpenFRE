#include "include.hpp"

/*
Grabs process ID of the specified program

@param Window Title
@param processid

@return None
*/
void getProcID(_In_ LPCSTR windowTitle, _Out_ DWORD& processID);

/*
Displays error messages in case of injection failure, then exits the program

@param Error Title
@param Error Message

@return None
*/
void errorMsg(_In_ LPCSTR errorTitle, _In_ LPCSTR errorMsg);

/*
Checks to see if a file exists

@param Name of file

@return True if file exists, false if it doesn't
*/
bool fileExists(_In_ std::string fileName);

/*
Main function, injects DLL into Roblox memory

@return True if success, false if error.
*/
bool inject();