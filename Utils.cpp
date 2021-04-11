#include <Windows.h>
#include <cstdint>
#include <fstream>
#include <algorithm>
#include <cctype>
#include "Utils.h"
#include <vector>
#include <regex>
#include <fmt/format.h>
#include <fmt/core.h>
#include "D2Tables.h"
#include "D2Structs.h"
#include "D2Ptrs.h"

#pragma comment(lib, "Version.Lib")

GameVersion InitGameVersion(LPCVOID pVersionResource) {
    UINT uLen;
    VS_FIXEDFILEINFO* ptFixedFileInfo;
    if (!VerQueryValue(pVersionResource, L"\\", (LPVOID*)&ptFixedFileInfo, &uLen))
        return GameVersion::NONE;

    if (uLen == 0)
        return GameVersion::ERR;

    WORD major = HIWORD(ptFixedFileInfo->dwFileVersionMS);
    WORD minor = LOWORD(ptFixedFileInfo->dwFileVersionMS);
    WORD revision = HIWORD(ptFixedFileInfo->dwFileVersionLS);
    WORD subrevision = LOWORD(ptFixedFileInfo->dwFileVersionLS);

    if (major != 1)
        return GameVersion::ERR;
    if (minor == 0 && revision == 13 && subrevision == 60) return GameVersion::V113c;
    if (minor == 14 && revision == 3 && subrevision == 71) return GameVersion::V114d;
    return GameVersion::ERR;
}

GameVersion InitGameVersion() {
    HMODULE hModule = GetModuleHandle(NULL);
    HRSRC hResInfo = FindResource(hModule, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
    if (!hResInfo) {
        return GameVersion::ERR;
    }
    HGLOBAL hResData = LoadResource(hModule, hResInfo);
    if (!hResData) {
        return GameVersion::ERR;
    }
    LPVOID pVersionResource = LockResource(hResData);
    GameVersion version = InitGameVersion(pVersionResource);
    FreeResource(hResData);
    return version;
}

GameVersion GetGameVersion() {
    if (GAME_VERSION == GameVersion::NONE) {
        GAME_VERSION = InitGameVersion();
    }
    return GAME_VERSION;
}

void  PrintGameString(std::wstring wStr, TextColor color) {
    if (GetGameVersion() == GameVersion::V114d) {
        D2CLIENT_PrintGameStringe_114d(wStr.c_str(), color);
    } else {
        D2CLIENT_PrintGameString(wStr.c_str(), color);
    }
}

Unit* FindUnit(uint32_t unitId, UnitType type) {
    Unit* unit = D2CLIENT_FindServerSideUnit(unitId, type);
    if (unit == NULL) {
        unit = D2CLIENT_FindClientSideUnit(unitId, type);
    }
    return unit;
}

uint32_t __fastcall GetDllOffset(uint32_t baseAddress, int offset) {
    DEBUG_LOG("baseAddress: {}\n", baseAddress);
    DEBUG_LOG("offset: {}\n", offset);
    DEBUG_LOG("return: {}\n", baseAddress + offset);
    if (offset < 0)
        return (uint32_t)GetProcAddress((HMODULE)baseAddress, (LPCSTR)(-offset));
    return baseAddress + offset;
}

std::wstring_view ltrim(std::wstring_view s)
{
	s.remove_prefix(std::distance(s.cbegin(), std::find_if(s.cbegin(), s.cend(),
		[](int c) {return !std::isspace(c); })));

	return s;
}

std::wstring_view rtrim(std::wstring_view s)
{
	s.remove_suffix(std::distance(s.crbegin(), std::find_if(s.crbegin(), s.crend(),
		[](int c) {return !std::isspace(c); })));

	return s;
}

std::wstring_view trim(std::wstring_view s)
{
	return ltrim(rtrim(s));
}

void replace(std::wstring& subject, const std::wstring& search, const std::wstring& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

std::vector<std::wstring> split(const std::wstring& stringToSplit, const std::wstring& regexPattern)
{
    std::vector<std::wstring> result;

    const std::wregex rgx(regexPattern);
    std::wsregex_token_iterator iter(stringToSplit.begin(), stringToSplit.end(), rgx, -1);

    for (std::wsregex_token_iterator end; iter != end; ++iter)
    {
        result.push_back(iter->str());
    }

    return result;
}

