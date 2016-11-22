#include<Windows.h>
#include<stdio.h>


typedef struct { 
	ULONG     i[2];
	ULONG     buf[4];
	unsigned char in[64];
	unsigned char digest[16];
} MD5_CTX;


typedef VOID(WINAPI *pMD5Init)(MD5_CTX *);
typedef VOID(WINAPI *pMD5Update)(MD5_CTX *, const unsigned char *, unsigned int);
typedef VOID(WINAPI *pMD5Final)(MD5_CTX *);


void MD5(CHAR *SubKey, DWORD size) {
	HMODULE hModule;
	CHAR errmsg[256];

	pMD5Init  MD5Init;
	pMD5Update MD5Update;
	pMD5Final MD5Final;
	
	MD5_CTX md5_context;


	ZeroMemory(&md5_context, sizeof(md5_context));

	hModule = LoadLibraryA("advapi32");
	if (hModule == NULL) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("LoadLibraryA : %hs\n", errmsg);
		return;
	}

	MD5Init = (pMD5Init)GetProcAddress(hModule, "MD5Init");
	if (MD5Init == NULL) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("GetProcAddress : %hs\n", errmsg);
		return;
	}

	MD5Update = (pMD5Update)GetProcAddress(hModule, "MD5Update");
	if (MD5Update == NULL) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("GetProcAddress : %hs\n", errmsg);
		return;
	}

	MD5Final = (pMD5Final)GetProcAddress(hModule, "MD5Final");
	if (MD5Final == NULL) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("GetProcAddress : %hs\n", errmsg);
		return;
	}

	MD5Init(&md5_context);
	MD5Update(&md5_context, (const unsigned char *)SubKey, size);
	MD5Final(&md5_context);

}