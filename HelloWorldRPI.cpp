// HelloWorldRPI.cpp
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

// Find PID by process name
DWORD FindProcessId(const wchar_t* processName) {
    PROCESSENTRY32W pe32{};
    pe32.dwSize = sizeof(pe32);
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
    const wchar_t* dllPath = L"C:\\Users\\Win10User\\Downloads\\Sysmon\\HelloBoxDLL.dll"; // ✅ Confirm path is correct
    const wchar_t* targetProcess = L"mspaint.exe";

    DWORD attrib = GetFileAttributesW(dllPath);
    if (attrib == INVALID_FILE_ATTRIBUTES || (attrib & FILE_ATTRIBUTE_DIRECTORY)) {
        std::wcerr << L"[!] DLL not found at: " << dllPath << std::endl;
        return 1;
    }

    DWORD pid = FindProcessId(targetProcess);
    if (pid == 0) {
        std::wcerr << L"[!] Target process not found: " << targetProcess << std::endl;
        return 1;
    }

    std::wcout << L"[+] Found " << targetProcess << L" (PID: " << pid << L")" << std::endl;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) {
        std::wcerr << L"[!] Failed to open process. Error: " << GetLastError() << std::endl;
        return 1;
    }

    SIZE_T size = (wcslen(dllPath) + 1) * sizeof(wchar_t);
    LPVOID remoteMemory = VirtualAllocEx(hProcess, nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remoteMemory) {
        std::wcerr << L"[!] VirtualAllocEx failed. Error: " << GetLastError() << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    if (!WriteProcessMemory(hProcess, remoteMemory, dllPath, size, nullptr)) {
        std::wcerr << L"[!] WriteProcessMemory failed. Error: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)LoadLibraryW, remoteMemory, 0, nullptr);
    if (!hThread) {
        std::wcerr << L"[!] CreateRemoteThread failed. Error: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    std::wcout << L"[+] DLL injected. Waiting for remote thread to complete..." << std::endl;
    WaitForSingleObject(hThread, INFINITE);

    DWORD remoteExitCode = 0;
    if (GetExitCodeThread(hThread, &remoteExitCode)) {
        std::wcout << L"[+] Remote thread exit code: 0x" << std::hex << remoteExitCode << std::endl;
    } else {
        std::wcerr << L"[!] GetExitCodeThread failed. Error: " << GetLastError() << std::endl;
    }

    VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    std::wcout << L"[+] Done. Press Enter to exit..." << std::endl;
    std::wcin.get();
    return 0;
}
