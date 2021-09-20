#pragma once

#include <Windows.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include "D2Structs.h"
#include "D2Tables.h"
#include "mINI.h"
#include <filesystem>

#ifdef _DEBUG
#define DEBUG_LOG(f) std::wprintf(f); PrintGameString(f, TextColor::YELLOW);
#else
#define DEBUG_LOG(f) ;
#endif

#define INFO_LOG(f) PrintGameString(f, TextColor::YELLOW);
#define WARN_LOG(f) PrintGameString(f, TextColor::ORANGE);
#define ERROR_LOG(f) PrintGameString(f, TextColor::RED);


//Hooking
uint32_t __fastcall GetDllOffset(uint32_t baseAddress, int offset);

D2Version GetGameVersion();
void PrintGameString(std::wstring wStr, TextColor color);

Unit* FindUnitFromTable(uint32_t unitId, UnitType type);
Unit* FindUnit(uint32_t unitId, UnitType type);

//String funcs
std::wstring_view ltrim(std::wstring_view s);
std::wstring_view rtrim(std::wstring_view s);
std::wstring_view trim(std::wstring_view s);
std::wstring ltrim_copy(std::wstring s);
std::wstring rtrim_copy(std::wstring s);
std::wstring trim_copy(std::wstring s);

void replace(std::wstring& subject, const std::wstring& search, const std::wstring& replace);
std::vector<std::wstring> split(const std::wstring& stringToSplit, const std::wstring& regexPattern);

//Utility D2 Methods
ItemsTxt GetItemsTxt(Unit* pUnit);
std::wstring GetItemCode(Unit* pUnit);
int32_t GetQualityLevel(Unit* pItem);
