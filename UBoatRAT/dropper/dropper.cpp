#include<Windows.h>
#include<iphlpapi.h>
#include<stdio.h>

#pragma comment(lib,"IPHLPAPI.lib")


char DomainName[0x84];
char HostName[0x84];
char BufStr[304];
char domain[3] = {};


DWORD GetNetworkInfo() {
	DWORD result = 0;
	ULONG OutBufLen;
	FIXED_INFO *FixedInfo;


	FixedInfo = (FIXED_INFO *)GlobalAlloc(GMEM_ZEROINIT, sizeof(FIXED_INFO));	// 0x248

	OutBufLen = sizeof(FIXED_INFO);
	if (GetNetworkParams(FixedInfo, &OutBufLen) == ERROR_BUFFER_OVERFLOW) {
		GlobalFree(FixedInfo);
		FixedInfo = (FIXED_INFO *)GlobalAlloc(GMEM_ZEROINIT, OutBufLen);	
	}
	
	result = GetNetworkParams(FixedInfo, &OutBufLen);
	if (!result) {
		strncpy_s(HostName, FixedInfo->HostName, 0x84);
		strncpy_s(DomainName, FixedInfo->DomainName, 0x84);
	}
	return result;
}


DWORD antiNW() {
	CHAR UserName[256];
	DWORD nSize;	
	BOOL bRet;
	CHAR errmsg[512];


	nSize = sizeof(UserName);
	ZeroMemory(UserName, sizeof(UserName));
	ZeroMemory(DomainName, sizeof(DomainName));
	ZeroMemory(HostName, sizeof(HostName));

	GetNetworkInfo();
	bRet = GetUserNameA(UserName, &nSize);
	if (bRet == FALSE) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("GetUserNameA : %hs\n", errmsg);
		return 0;
	}

	sprintf_s(BufStr, "%s-%s-%s", HostName, DomainName, UserName);

	// not detect
	if (strcmp(DomainName, domain)) {
		return 1;
	}
	// detect
	else {
		return 0;
	}
}


DWORD antiVM() {
	HANDLE hFile;
	DWORD BytesReturned = 0;
	DWORD x = 0;
	BOOL bRet;

	unsigned char UserName[0x80];
	CHAR errmsg[512];
	CHAR device[512];

	char *VM[] = {"qemu","virtual","vmware"};

	STORAGE_PROPERTY_QUERY StoragePropertyQuery;
	STORAGE_DEVICE_DESCRIPTOR *StorageDeviceDescriptor;


	ZeroMemory(&StoragePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY));
	StoragePropertyQuery.PropertyId = StorageDeviceProperty;
	StoragePropertyQuery.QueryType = PropertyStandardQuery;

	ZeroMemory(UserName, sizeof(UserName));

	hFile = CreateFileW(L"\\\\.\\PhysicalDrive0", 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("GetModuleFileName : %hs\n", errmsg);
		return 0;
	}


	bRet = DeviceIoControl(hFile, IOCTL_STORAGE_QUERY_PROPERTY,
		&StoragePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY),
		UserName, sizeof(UserName),
		&BytesReturned, NULL);

	if (bRet == FALSE) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("DeviceIoControl : %hs\n", errmsg);
		CloseHandle(hFile);
		return 0;
	}

	
	StorageDeviceDescriptor = (STORAGE_DEVICE_DESCRIPTOR *)UserName;
	for (int index = StorageDeviceDescriptor->ProductIdOffset; UserName[index] != '\0'; index++) {
		device[x] = UserName[index];
		x++;
	}
	device[x] = '\0';

	CharLowerBuff(device, strlen(device));

	printf("%s\n", device);

	CloseHandle(hFile);

	for (int i = 0; i < 3; i++) {
		// if VM_detect
		if (strstr(device, VM[i])) {
			return 1;
		}
	}
	// not detect
	return 0;
}
