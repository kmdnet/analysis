#include<Windows.h>
#include<stdio.h>
#include<WinBase.h>

#include "satana_header.h"


void Privilege(){
	HANDLE TokenHandle;
	TOKEN_PRIVILEGES Token_Privileges;
	LUID Luid;
	BOOL bRet;
	CHAR errmsg[256];


	// 0x28
	bRet = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &TokenHandle);
	if (bRet == FALSE) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("OpenProcessToken : %hs\n", errmsg);
		return;
	}

	bRet = LookupPrivilegeValue(NULL, "SeShutdownPrivilege", &Luid);
	if (bRet == FALSE) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("LookupPrivilegeValue : %hs\n", errmsg);
		return;
	}

	Token_Privileges.PrivilegeCount = 1;
	Token_Privileges.Privileges[0].Luid = Luid;
	Token_Privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;	// 0x2


	bRet = AdjustTokenPrivileges(TokenHandle, FALSE, &Token_Privileges, 0, 0, 0);
	if (bRet == FALSE) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("AdjustTokenPrivileges : %hs\n", errmsg);
		return;
	}

	CloseHandle(TokenHandle);

}