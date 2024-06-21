#pragma once

#include <Windows.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include "D2Structs.h"
#include "D2Tables.h"
#include <filesystem>
#include <cwctype>
#include <unordered_map>

#ifdef _DEBUG
#define DEBUG_LOG(f) std::wprintf(f.c_str()); //PrintGameString(f, TextColor::YELLOW);
#else
#define DEBUG_LOG(f) ;
#endif

#define INFO_LOG(f) PrintGameString(f, TextColor::YELLOW);
#define WARN_LOG(f) PrintGameString(f, TextColor::ORANGE);
#define ERROR_LOG(f) PrintGameString(f, TextColor::RED);


//Hooking
uint32_t __fastcall GetDllOffset(uint32_t baseAddress, int offset);

D2Version GetGameVersion();
void PrintGameString(const std::wstring& wStr, TextColor color);

Unit* FindUnitFromTable(uint32_t unitId, UnitType type);
Unit* FindUnit(uint32_t unitId, UnitType type);

//String funcs
std::wstring_view ltrim(std::wstring_view s);
std::wstring_view rtrim(std::wstring_view s);
std::wstring_view trim(std::wstring_view s);
std::wstring ltrim_copy(std::wstring_view s);
std::wstring rtrim_copy(std::wstring_view s);
std::wstring trim_copy(std::wstring_view s);

void replace(std::wstring& subject, std::wstring_view search, std::wstring_view replace);
std::vector<std::wstring> split(std::wstring_view stringToSplit, std::wstring_view regexPattern);

//Utility D2 Methods
ItemsTxt* GetItemsTxt(Unit* pUnit);
std::wstring GetItemCode(Unit* pUnit);
int32_t GetQualityLevel(Unit* pItem);

// Heterogeneous lookup for std::unordered_map
namespace utility {
	namespace detail {
		template<typename T>
		struct char_type_helper {
			using type = std::remove_cv_t<std::remove_pointer_t<T>>;
		};

		template<typename CharT>
		struct char_type_helper<std::basic_string<CharT>> {
			using type = CharT;
		};

		template<typename CharT>
		struct char_type_helper<std::basic_string_view<CharT>> {
			using type = CharT;
		};

		template<typename T>
		using char_type_helper_t = char_type_helper<T>::type;

		template<typename T>
		struct string_equal {
			using is_transparent = void;

			bool operator()(std::basic_string_view<char_type_helper_t<T>> lhs,
				std::basic_string_view<char_type_helper_t<T>> rhs) const noexcept {
				return lhs == rhs;
			}
		};

		template<typename T>
		struct string_hash {
			using is_transparent = void;

			auto operator()(std::basic_string_view<char_type_helper_t<T>> str) const noexcept {
				return std::hash<std::basic_string_view<char_type_helper_t<T>>>{}(str);
			}
		};

		template<typename T = void>
		struct string_iequal {
			using is_transparent = void;

			auto operator()(std::wstring_view lhs, std::wstring_view rhs) const noexcept {
				return std::ranges::equal(lhs, rhs, [](wchar_t a, wchar_t b) {
					return std::towlower(a) == std::towlower(b);
					});
			}

		};

		constexpr size_t FNV_Offset_Basis = 2166136261u;
		constexpr size_t FNV_Prime = 16777619u;

		template<typename T = void>
		struct string_ihash {
			using is_transparent = void;

			size_t operator()(std::wstring_view str) const noexcept {
				static_assert(sizeof(decltype(str)::traits_type::char_type) == 2);

				size_t hash{ FNV_Offset_Basis };
				for (auto ch : str) {
					ch = std::towlower(ch);
					hash ^= static_cast<size_t>(ch & 0xFF);
					hash *= FNV_Prime;
					hash ^= static_cast<size_t>(ch >> 8);
					hash *= FNV_Prime;
				}
				return hash;
			}
		};
	} // namespace detail

	template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
	struct unordered_string_map_helper;

	template<typename CharT, typename T, typename Hash, typename KeyEqual, typename Allocator>
	struct unordered_string_map_helper<std::basic_string<CharT>, T, Hash, KeyEqual, Allocator> {
		using type = std::unordered_map<std::basic_string<CharT>, T, Hash, KeyEqual, Allocator>;
	};

	template<typename Key, typename T, typename Allocator = std::allocator<std::pair<const Key, T>>>
	using string_umap = unordered_string_map_helper<Key, T, detail::string_hash<Key>, detail::string_equal<Key>, Allocator>::type;

	template<typename Key, typename T, typename Allocator = std::allocator<std::pair<const Key, T>>>
	using string_umap_icase = unordered_string_map_helper<Key, T, detail::string_ihash<Key>, detail::string_iequal<Key>, Allocator>::type;
} // namespace utility
