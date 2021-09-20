#include <Windows.h>
#include <cstdint>
#include <fstream>
#include <algorithm>
#include <cctype>
#include "Utils.h"
#include <vector>
#include <regex>
#include "D2Tables.h"
#include "D2Structs.h"
#include "D2Ptrs.h"

#pragma comment(lib, "Version.Lib")

#define D2SE_SETUP_FILE "D2SE_SETUP.ini"

D2Version InitGameVersion(LPCVOID pVersionResource) {
    UINT uLen;
    VS_FIXEDFILEINFO* ptFixedFileInfo;
    if (!VerQueryValue(pVersionResource, L"\\", (LPVOID*)&ptFixedFileInfo, &uLen))
        return D2Version::NONE;

    if (uLen == 0)
        return D2Version::ERR;

    WORD major = HIWORD(ptFixedFileInfo->dwFileVersionMS);
    WORD minor = LOWORD(ptFixedFileInfo->dwFileVersionMS);
    WORD revision = HIWORD(ptFixedFileInfo->dwFileVersionLS);
    WORD subrevision = LOWORD(ptFixedFileInfo->dwFileVersionLS);

    if (major != 1)
        return D2Version::ERR;
    if (minor == 0 && revision == 13 && subrevision == 60) return D2Version::V113c;
    if (minor == 14 && revision == 3 && subrevision == 71) return D2Version::V114d;
    if (minor == 0 && revision == 10 && subrevision == 39) return D2Version::V110f;
    return D2Version::ERR;
}

D2Version InitGameVersion() {
    std::string core;
    mINI::INIFile file(D2SE_SETUP_FILE);
    mINI::INIStructure coredata;
    bool exists = std::filesystem::exists(D2SE_SETUP_FILE);

    if (!exists) {
        HMODULE hModule = GetModuleHandle(NULL);
        HRSRC hResInfo = FindResource(hModule, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
        if (!hResInfo) {
            return D2Version::ERR;
        }
        HGLOBAL hResData = LoadResource(hModule, hResInfo);
        if (!hResData) {
            return D2Version::ERR;
        }
        LPVOID pVersionResource = LockResource(hResData);
        D2Version version = InitGameVersion(pVersionResource);
        FreeResource(hResData);
        return version;
    }
    else {
        file.read(coredata);
        core = coredata["Protected"]["D2Core"];
        if (core == "1.10f") return D2Version::V110f;
        if (core == "1.13c") return D2Version::V113c;
        return D2Version::ERR;
    }
}

D2Version GetGameVersion() {
    if (GameVersion == D2Version::NONE) {
        GameVersion = InitGameVersion();
    }
    return GameVersion;
}

void  PrintGameString(std::wstring wStr, TextColor color) {
    if (GetGameVersion() == D2Version::V114d || GetGameVersion() == D2Version::V110f) {
        D2CLIENT_PrintGameStringe_114d(wStr.c_str(), color);
    } else {
        D2CLIENT_PrintGameString(wStr.c_str(), color);
    }
}

Unit* FindUnitFromTable(uint32_t unitId, UnitType type) {
    UnitHashTable serverSideUnitTable = D2CLIENT_ServerSideUnitHashTables[static_cast<int32_t>(type)];
    Unit* unit = serverSideUnitTable.table[unitId];
    if (unit == NULL) {
        UnitHashTable clientSideUnitTable = D2CLIENT_ClientSideUnitHashTables[static_cast<int32_t>(type)];
        unit = clientSideUnitTable.table[unitId];
    }
    return unit;
}

Unit* FindUnit(uint32_t unitId, UnitType type) {
    Unit* unit = D2CLIENT_FindServerSideUnit(unitId, type);
    if (unit == NULL) {
        unit = D2CLIENT_FindClientSideUnit(unitId, type);
    }
    return unit;
}

uint32_t __fastcall GetDllOffset(uint32_t baseAddress, int offset) {
    DEBUG_LOG(L"baseAddress: {}\n", baseAddress);
    DEBUG_LOG(L"offset: {}\n", offset);
    DEBUG_LOG(L"return: {}\n", baseAddress + offset);
    if (offset < 0)
        return (uint32_t)GetProcAddress((HMODULE)baseAddress, (LPCSTR)(-offset));
    return baseAddress + offset;
}

std::wstring_view ltrim(std::wstring_view s) {
	s.remove_prefix(std::distance(s.cbegin(), std::find_if(s.cbegin(), s.cend(),
		[](wchar_t c) {return !iswspace(c); })));

	return s;
}

std::wstring_view rtrim(std::wstring_view s) {
	s.remove_suffix(std::distance(s.crbegin(), std::find_if(s.crbegin(), s.crend(),
		[](wchar_t c) {return !iswspace(c); })));

	return s;
}

std::wstring_view trim(std::wstring_view s) {
	return ltrim(rtrim(s));
}

std::wstring ltrim_copy(std::wstring s) {
    ltrim(s);
    return std::wstring(s);
}

// trim from end (copying)
std::wstring rtrim_copy(std::wstring s) {
    rtrim(s);
    return std::wstring(s);
}

// trim from both ends (copying)
std::wstring trim_copy(std::wstring s) {
    trim(s);
    return std::wstring(s);
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

ItemsTxt GetItemsTxt(Unit* pUnit) {
    return D2COMMON_ItemDataTbl->pItemsTxt[pUnit->dwLineId];
}

std::wstring GetItemCode(Unit* pUnit) {
    ItemsTxt txt = GetItemsTxt(pUnit);
    std::wstring wCode = std::wstring(4, L' ');
    mbstowcs(&wCode[0], txt.szCode, 4);
    wCode = trim(wCode);
    return wCode;
}

int32_t GetQualityLevel(Unit* pItem) {
    ItemsTxt txt = GetItemsTxt(pItem);
    int32_t quality = -1;
    if (txt.dwCode == txt.dwUltraCode) {
        quality = 2;
    }
    else if (txt.dwCode == txt.dwUberCode) {
        quality = 1;
    }
    else if (txt.dwCode == txt.dwNormCode) {
        quality = 0;
    }
    return quality;
}

