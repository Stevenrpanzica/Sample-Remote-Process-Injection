#include <windows.h>

// Function executed by the new thread
DWORD WINAPI ShowMessageBoxThread(LPVOID lpParam) {
    MessageBoxA(NULL, "Hello World", "Injected", MB_OK | MB_ICONINFORMATION);
    return 0;
}

// Entry point for the DLL
BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        // Create a new thread to safely show MessageBox
        CreateThread(NULL, 0, ShowMessageBoxThread, NULL, 0, NULL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
