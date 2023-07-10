#include <stdio.h>
#include <Windows.h>
#include <TlHelp32.h>

int getProcessIdByName(const char* procName) {
	int PID = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == NULL) {
		return 0;
	}

	PROCESSENTRY32 processEntry32;
	processEntry32.dwSize = sizeof(processEntry32);

	Process32First(hSnap, &processEntry32);

	while (Process32Next(hSnap, &processEntry32)) {
		if (strcmp(procName, processEntry32.szExeFile) == 0) {
			PID = processEntry32.th32ProcessID;
			break;
		}
	}

	CloseHandle(hSnap);
	return PID;
}

int main(const int argc, char** argv)
{
    char* dll_name;
    char* process_target;
    char full_dllp[MAX_PATH];

    if (argc != 3)
    {
        printf("Inject.exe <DLL> <TARGET>\n");
        return -1;
    }

    dll_name = argv[1];
    process_target = argv[2];

    int PID = getProcessIdByName(process_target);
    if (PID == 0)
    {
        printf("Failed to get process ID.\n");
        return -1;
    }

    DWORD FullDllPath = GetFullPathNameA(dll_name, MAX_PATH, full_dllp, 0);
    if (FullDllPath == 0)
    {
        printf("Failed to get full path of dll.\n");
        return -1;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
    if (hProcess == NULL)
    {
        printf("Failed to open target process.\n");
        return -1;
    }


    LPVOID AllocPATH = VirtualAllocEx(hProcess, 0, lstrlenA(full_dllp) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (AllocPATH == NULL)
    {
        printf("Failed to allocate memory.\n");
        return -1;
    }

    if (WriteProcessMemory(hProcess, AllocPATH, full_dllp, lstrlenA(full_dllp) + 1, 0) == 0)
    {
        printf("Failed to write memory.\n");
        return -1;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"), AllocPATH, 0, NULL);
    if (hThread == NULL)
    {
        printf("Failed to create thread.\n");
        return -1;
    }

    printf("DLL Injected...\n");
    system("pause");

    CloseHandle(hThread);
    CloseHandle(hProcess);

    VirtualFreeEx(hProcess, AllocPATH, lstrlenA(full_dllp), MEM_RELEASE);

    return 0;
}