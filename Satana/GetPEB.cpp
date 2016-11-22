#include<Windows.h>
#include<winternl.h>
#include<stdio.h>


typedef PPEB(WINAPI *pRtlGetCurrentPeb)(void);


void GetPEB() {
	PPEB pPeb;	// PEB
	pRtlGetCurrentPeb RtlGetCurrentPeb;
	HMODULE hModule;
	CHAR errmsg[256];
	
	VOID *Ldr, *Flink, *FullDllName, *DllBase;


	hModule = LoadLibraryA("ntdll");
	if (hModule == NULL) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("LoadLibraryA : %hs\n", errmsg);
		return;
	}

	RtlGetCurrentPeb = (pRtlGetCurrentPeb)GetProcAddress(hModule, "RtlGetCurrentPeb");
	if (RtlGetCurrentPeb == NULL) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("GetProcAddress : %hs\n", errmsg);
		return;
	}

	pPeb = (PPEB)RtlGetCurrentPeb();			// RtlGetCurrentPeb()
	Ldr = *(VOID **)(((unsigned char *)pPeb + 0x0C));
	Flink = *(VOID **)(((unsigned char *)Ldr + 0x0C));

	FullDllName = *((void **)((unsigned char *)Flink + 0x28));
	DllBase = *((void **)((unsigned char *)Flink + 0x18));

	wprintf(L"FullDllName : %s\n", FullDllName);
	printf("DllBase : 0x%p\n", DllBase);	

}


