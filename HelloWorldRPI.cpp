#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

// Find the PID of a process by its name
DWORD FindProcessId(const wchar_t* processName) {
    PROCESSENTRY32W pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;

    if (Process32FirstW(snapshot, &pe32)) {
        do {
            if (_wcsicmp(pe32.szExeFile, processName) == 0) {
                CloseHandle(snapshot);
                return pe32.th32ProcessID;
            }
        } while (Process32NextW(snapshot, &pe32));
    }

    CloseHandle(snapshot);
    return 0;
}

int wmain() {
    const wchar_t* dllPath = L"C:Users\\Win10User\\Downloads\\Sysmon\\HelloBox.dll"; // 🔁 Update this to your actual DLL path
    const wchar_t* targetProcess = L"mspaint.exe";

    DWORD pid = FindProcessId(targetProcess);
    if (pid == 0) {
        std::wcerr << L"Process not found: " << targetProcess << std::endl;
        return 1;
    }

    std::wcout << L"Found " << targetProcess << L" (PID: " << pid << L")" << std::endl;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) {
        std::cerr << "Failed to open target process. Error: " << GetLastError() << std::endl;
        return 1;
    }

    SIZE_T size = (wcslen(dllPath) + 1) * sizeof(wchar_t);
    LPVOID remoteMemory = VirtualAllocEx(hProcess, NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remoteMemory) {
        std::cerr << "VirtualAllocEx failed. Error: " << GetLastError() << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    if (!WriteProcessMemory(hProcess, remoteMemory, dllPath, size, NULL)) {
        std::cerr << "WriteProcessMemory failed. Error: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    HANDLE hThread = CreateRemoteThread(
        hProcess, NULL, 0,
        (LPTHREAD_START_ROUTINE)LoadLibraryW,
        remoteMemory, 0, NULL
    );

    if (!hThread) {
        std::cerr << "CreateRemoteThread failed. Error: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    std::cout << "DLL injected successfully." << std::endl;

    WaitForSingleObject(hThread, INFINITE);

    VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return 0;
}
