#include<Windows.h>
#include<winternl.h>
#include<stdio.h>
#include<ntstatus.h>


typedef NTSTATUS(WINAPI *pNtQueryInformationProcess)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

void detect_debugger() {
	HMODULE hModule;
	PROCESSINFOCLASS ProcessInformationClass;
	pNtQueryInformationProcess NtQueryInformationProcess;
	CHAR errmsg[512];


	hModule = LoadLibraryA("ntdll");
	if (hModule == NULL) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("LoadLibraryA : %hs\n", errmsg);
		return;
	}

	NtQueryInformationProcess = (pNtQueryInformationProcess)GetProcAddress(hModule,"NtQueryInformationProcess");
	if (NtQueryInformationProcess == NULL) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("GetProcAddress : %hs\n", errmsg);
		return;
	}

	NtQueryInformationProcess(GetCurrentProcess(),ProcessDebugPort, &ProcessInformationClass,sizeof(PROCESSINFOCLASS),NULL);
	
	if (ProcessInformationClass != 0) {
		printf("Detected\n");
	}

}

