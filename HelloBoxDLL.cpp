// HelloBoxDLL.cpp
#include <Windows.h>

BOOL APIENTRY DllMain(HMODULE, DWORD ul_reason_for_call, LPVOID) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        MessageBoxW(NULL, L"Hello from HelloBoxDLL!", L"HelloBox", MB_OK | MB_ICONINFORMATION);
    }
    return TRUE;
}
