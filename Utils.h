#pragma once

#include <Windows.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include "D2Structs.h"

#include <fmt/format.h>
#include <fmt/core.h>
#ifdef _DEBUG
#define DEBUG_LOG(fmt, ...) fmt::print(fmt, __VA_ARGS___); 
#else
#define DEBUG_LOG(fmt, ...) ;
#endif


//Hooking
uint32_t __fastcall GetDllOffset(uint32_t baseAddress, int offset);

GameVersion GetGameVersion();
void PrintGameString(std::wstring wStr, TextColor color);
Unit* FindUnit(uint32_t unitId, UnitType type);

//String funcs
std::wstring_view ltrim(std::wstring_view s);
std::wstring_view rtrim(std::wstring_view s);
std::wstring_view trim(std::wstring_view s);
void replace(std::wstring& subject, const std::wstring& search, const std::wstring& replace);
std::vector<std::wstring> split(const std::wstring& stringToSplit, const std::wstring& regexPattern);
