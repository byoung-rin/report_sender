// report_sender.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <TlHelp32.h>
using namespace std;

BOOL ProcessAllKill(char* szProcessName)
{
	HANDLE hndl = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	DWORD dwsma = GetLastError();
	HANDLE hHandle;

	DWORD dwExitCode = 0;

	PROCESSENTRY32  procEntry = { 0 };
	procEntry.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hndl, &procEntry);
	while (1)
	{
		if (!strcmp(procEntry.szExeFile, szProcessName))
		{

			hHandle = ::OpenProcess(PROCESS_ALL_ACCESS, 0, procEntry.th32ProcessID);

			if (::GetExitCodeProcess(hHandle, &dwExitCode))
			{
				if (!::TerminateProcess(hHandle, dwExitCode))
				{
					return FALSE;
				}
			}
		}
		if (!Process32Next(hndl, &procEntry))
		{
			return TRUE;
		}
	}


	return TRUE;
}

int main(int argc, char* argv[])
{
	int i;
	for (i=1; i<argc; i++)
	{
		std::ifstream readFile;
		printf("경로[%d]=%s\n", i, argv[i]);
		readFile.open(argv[i]);
		if (readFile.is_open())
		{
			string str;
			while (getline(readFile, str))
			{
				printf("URL=%s\n", str.c_str());
				ShellExecute(NULL, "open", "chrome.exe", str.c_str(), NULL, SW_SHOW);
			}
			readFile.close();
		}
	}
	printf("i=%d", i);
	Sleep(3000*i);
	ProcessAllKill("chrome.exe");
	ShellExecute(NULL, "open", "chrome.exe", NULL, NULL, SW_SHOW);
	
    return 0;
}

