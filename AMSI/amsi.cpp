#include<Windows.h>
#include<amsi.h>
#include<stdio.h>

#pragma comment(lib, "amsi.lib")


int amsi_scanner(PVOID buffer, DWORD dwBufferSize, char *filename) {
	HRESULT hr;
	HAMSICONTEXT AmsiContext;
	AMSI_RESULT AmsiResult;
	HAMSISESSION AmsiSession;


	ZeroMemory(&AmsiContext, sizeof(AmsiContext));

	hr = AmsiInitialize((LPCWSTR)"TestAmsiScanner", &AmsiContext);
	if (hr != S_OK) {
		printf("AmsiInitialize \n");
		return -1;
	}

	hr = AmsiOpenSession(AmsiContext, &AmsiSession);
	if (hr != S_OK) {
		printf("AmsiOpenSession \n");
		return -1;
	}

	hr = AmsiScanBuffer(AmsiContext, buffer, dwBufferSize, (LPCWSTR)filename, AmsiSession, &AmsiResult);
	if (hr != S_OK) {
		printf("AmsiScanBuffer \n");
		return -1;
	}

	// Result
	switch (AmsiResult)
	{
	case AMSI_RESULT_CLEAN:
		printf("AMSI_RESULT_CLEAN \n");
		break;
	case AMSI_RESULT_NOT_DETECTED:
		printf("AMSI_RESULT_NOT_DETECTED \n");
		break;
	case AMSI_RESULT_BLOCKED_BY_ADMIN_START:
		printf("AMSI_RESULT_BLOCKED_BY_ADMIN_START \n");
		break;
	case AMSI_RESULT_BLOCKED_BY_ADMIN_END:
		printf("AMSI_RESULT_BLOCKED_BY_ADMIN_END \n");
		break;
	case AMSI_RESULT_DETECTED:
		printf("AMSI_RESULT_DETECTED \n");
		break;
	default:
		break;
	}

	
	return 0;
}


void amsi(char *fname) {
	HANDLE hFile;
	DWORD dwFileSize, dwBytesRead;
	BOOL bRet;
	char errmsg[512];

	PVOID buffer;

	hFile = CreateFileA(fname, GENERIC_ALL, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("CreateFileA : %hs\n", errmsg);
		return;
	}

	dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == INVALID_FILE_SIZE) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("GetFileSize : %hs\n", errmsg);
		return;
	}

	buffer = VirtualAlloc(NULL, dwFileSize, MEM_COMMIT, PAGE_READWRITE);
	if(buffer == NULL){
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("VirtualAlloc : %hs\n", errmsg);
		return;
	}

	bRet = ReadFile(hFile, buffer, dwFileSize, &dwBytesRead, NULL);
	if (bRet == FALSE) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("ReadFile : %hs\n", errmsg);
		return;
	}


	int result;
	// File
	result = amsi_scanner(buffer, dwFileSize, fname);
	if (result == -1) {
		printf("fatal...\n");
	}

	printf("EICAR \n");
	const char *eicar = "X5O!P%@AP[4\\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*";
	int len = strlen(eicar);
	printf("len : %d\n", len);
	result = amsi_scanner((PVOID)eicar, len, (char*)"eicar");
	if (result == -1) {
		printf("fatal...\n");
	}

}