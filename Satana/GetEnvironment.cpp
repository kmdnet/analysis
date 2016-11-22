#include<Windows.h>
#include<stdio.h>

#include "satana_header.h"

CHAR SubKey[0x512];

void GetEnvironment() {
	DWORD Ret,size;
	TCHAR Processor_Architecture[0x104], Processor_Identifier[0x104], Processor_Level[0x104], Processor_Revision[0x104];
	CHAR errmsg[256];


	Ret = ExpandEnvironmentStrings("%PROCESSOR_ARCHITECTURE%",Processor_Architecture,0x104);
	if (Ret == NULL) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("ExpandEnvironmentStrings : %hs\n", errmsg);
		return;
	}

	Ret = ExpandEnvironmentStrings("%PROCESSOR_IDENTIFIER%", Processor_Identifier, 0x104);
	if (Ret == NULL) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("ExpandEnvironmentStrings : %hs\n", errmsg);
		return;
	}

	Ret = ExpandEnvironmentStrings("%PROCESSOR_LEVEL%", Processor_Level, 0x104);
	if (Ret == NULL) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("ExpandEnvironmentStrings : %hs\n", errmsg);
		return;
	}

	Ret = ExpandEnvironmentStrings("%PROCESSOR_REVISION%", Processor_Revision, 0x104);
	if (Ret == NULL) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		printf("ExpandEnvironmentStrings : %hs\n", errmsg);
		return;
	}

	sprintf_s(SubKey, 0x512, "%s%s%s%s\n", Processor_Architecture, Processor_Identifier, Processor_Level, Processor_Revision);
	size = strlen(SubKey);

	printf("%s %d\n", SubKey,size);

	MD5(SubKey, size);

}