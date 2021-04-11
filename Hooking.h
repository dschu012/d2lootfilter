#pragma once
class Hooking
{
public:
	static bool Hook(void* src, void* dst, int len);
	static void TrampolineHook(void* src, void* dst, void** orig, int len);
	static void SetCall(void* address, void* function, size_t size);
	static void SetJmp(void* address, void* function, size_t size);
};

