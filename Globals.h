#pragma once

#include <Windows.h>
#include <map>
#include <string>
#include "D2Constants.h"

extern GameVersion GAME_VERSION;

extern bool TXT_DATA_LOADED;
extern std::map<std::wstring, uint16_t> ITEM_TYPE_LOOKUP_TABLE;
extern std::map<std::wstring, uint32_t> ITEM_NAME_LOOKUP_TABLE;
extern std::map<std::wstring, uint32_t> ITEM_CODE_LOOKUP_TABLE;
extern std::map<std::wstring, uint32_t> RUNE_NAME_LOOKUP_TABLE;
extern std::map<std::wstring, std::wstring> STAT_LOOKUP_TABLE;
extern std::map<std::wstring, std::wstring> CLASS_SKILL_LOOKUP_TABLE;
extern std::map<std::wstring, std::wstring> TAB_SKILL_LOOKUP_TABLE;
extern std::map<std::wstring, std::wstring> SKILL_LOOKUP_TABLE;