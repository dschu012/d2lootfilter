#pragma once

#include <Windows.h>
#include <string>
#include <unordered_map>
#include "Action.h"
#include "D2Constants.h"
#include "Utils.h"

//Globals
extern const utility::string_umap<std::wstring, int32_t> ItemTypes;
extern const std::unordered_map<int32_t, std::wstring> ItemTypesLookup;

extern const utility::string_umap<std::wstring, int32_t> Rarities;
extern const std::unordered_map<int32_t, std::wstring> RaritiesLookup;

extern const utility::string_umap<std::wstring, int32_t> Qualities;
extern const std::unordered_map<int32_t, std::wstring> QualitiesLookup;

extern const utility::string_umap<std::wstring, std::wstring> CustomStats;

extern std::unordered_map<uint32_t, ActionResult> ITEM_ACTIONS;