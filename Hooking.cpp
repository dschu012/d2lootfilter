#include <Windows.h>
#include <cstdint>
#include "Hooking.h"

//https://stackoverflow.com/a/54661410/597419
bool Hooking::Hook(void* src, void* dst, int len) {
    if (len < 5) return false;

    DWORD  curProtection;
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection);

    memset(src, 0x90, len);
    uintptr_t  relativeAddress = ((uintptr_t)dst - (uintptr_t)src) - 5;

    *(BYTE*)(src) = 0xE9;
    *(uintptr_t*)((uintptr_t)src + 1) = relativeAddress;

    VirtualProtect(src, len, curProtection, &curProtection);
    return true;
}

void Hooking::TrampolineHook(void* src, void* dst, void** orig, int len) {
    // Make sure the length is greater than 5
    if (len < 5) {
        return;
    }

    // Create the gateway (len + 5 for the overwritten bytes + the jmp)
    void* gateway = VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    //Write the stolen bytes into the gateway
    memcpy(gateway, src, len);

    // Get the gateway to destination addy
    uintptr_t  gatewayRelativeAddr = ((uintptr_t)src - (uintptr_t)gateway) - 5;

    // Add the jmp opcode to the end of the gateway
    *(BYTE*)((uintptr_t)gateway + len) = 0xE9;

    // Add the address to the jmp
    *(uintptr_t*)((uintptr_t)gateway + len + 1) = gatewayRelativeAddr;

    // Place the hook at the destination
    Hook(src, dst, len);

    *orig = gateway;
}

//todo: refactor combine call/jmp fncs
void Hooking::SetCall(void* address, void* function, size_t size) {
    DWORD old_protect;
    VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &old_protect);

    memset(address, 0x90, size);
    *static_cast<unsigned char*>(address) = 0xE8;
    *reinterpret_cast<int32_t*>(static_cast<char*>(address) + 1) = reinterpret_cast<int32_t>(function) - reinterpret_cast<int32_t>(address) - 5;

    VirtualProtect(address, size, old_protect, &old_protect);
}

void Hooking::SetJmp(void* address, void* function, size_t size) {
    DWORD old_protect;
    VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &old_protect);

    memset(address, 0x90, size);
    *static_cast<unsigned char*>(address) = 0xE9;
    *reinterpret_cast<int32_t*>(static_cast<char*>(address) + 1) = reinterpret_cast<int32_t>(function) - reinterpret_cast<int32_t>(address) - 5;

    VirtualProtect(address, size, old_protect, &old_protect);
}