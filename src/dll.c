#include <Windows.h>
#include <io.h>
#include <stdio.h>


void WINAPI ThreadMain(HMODULE hModule) {
	MessageBox(0, "DLL INJECTED :)", "evil.dll", 0);
	AllocConsole();
	
	FILE* fileOut;
	FILE* fileIn;

	freopen_s(&fileOut, "CONOUT$", "w", stdout);
	freopen_s(&fileIn, "CONIN$", "r", stdin);
	
	printf("DLL injected !\n");
	system("pause");

	fclose(fileOut);
	fclose(fileIn);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		CloseHandle(CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadMain, hModule, NULL, 0));
	}
}