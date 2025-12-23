/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 18:30:53 by marvin            #+#    #+#             */
/*   Updated: 2025/12/23 18:30:53 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_HPP
#define STRING_HPP

#include <cstdint>
#include <string>
#include <sstream>
#include <cwchar>
#include <cstring>


#include <cstdint>
#include <cctype>
#include <clocale>
#include <locale>
#include <string>
#include <iostream>

// Mapping pair type
struct MapPair {
	uint32_t from;
	uint32_t to;
};

// Upper mapping (one entry per line to satisfy line length)
static const MapPair LATIN_UPPER[] = {
    {0x00E0, 0x00C0},
    {0x00E1, 0x00C1},
    {0x00E2, 0x00C2},
    {0x00E3, 0x00C3},
    {0x00E4, 0x00C4},
    {0x00E5, 0x00C5},
    {0x00E6, 0x00C6},
    {0x00E7, 0x00C7},
    {0x00E8, 0x00C8},
    {0x00E9, 0x00C9},
    {0x00EA, 0x00CA},
    {0x00EB, 0x00CB},
    {0x00EC, 0x00CC},
    {0x00ED, 0x00CD},
    {0x00EE, 0x00CE},
    {0x00EF, 0x00CF},
    {0x00F1, 0x00D1},
    {0x00F2, 0x00D2},
    {0x00F3, 0x00D3},
    {0x00F4, 0x00D4},
    {0x00F5, 0x00D5},
    {0x00F6, 0x00D6},
    {0x00F8, 0x00D8},
    {0x00F9, 0x00D9},
    {0x00FA, 0x00DA},
    {0x00FB, 0x00DB},
    {0x00FC, 0x00DC},
    {0x00FD, 0x00DD},
    {0x00FF, 0x0178}
};
static const size_t LATIN_UPPER_N =
    sizeof(LATIN_UPPER) / sizeof(LATIN_UPPER[0]);

static const MapPair LATIN_LOWER[] = {
    {0x00C0, 0x00E0},
    {0x00C1, 0x00E1},
    {0x00C2, 0x00E2},
    {0x00C3, 0x00E3},
    {0x00C4, 0x00E4},
    {0x00C5, 0x00E5},
    {0x00C6, 0x00E6},
    {0x00C7, 0x00E7},
    {0x00C8, 0x00E8},
    {0x00C9, 0x00E9},
    {0x00CA, 0x00EA},
    {0x00CB, 0x00EB},
    {0x00CC, 0x00EC},
    {0x00CD, 0x00ED},
    {0x00CE, 0x00EE},
    {0x00CF, 0x00EF},
    {0x00D1, 0x00F1},
    {0x00D2, 0x00F2},
    {0x00D3, 0x00F3},
    {0x00D4, 0x00F4},
    {0x00D5, 0x00F5},
    {0x00D6, 0x00F6},
    {0x00D8, 0x00F8},
    {0x00D9, 0x00F9},
    {0x00DA, 0x00FA},
    {0x00DB, 0x00FB},
    {0x00DC, 0x00FC},
    {0x00DD, 0x00FD},
    {0x00DE, 0x00FE},
    {0x0178, 0x00FF}
};
static const size_t LATIN_LOWER_N =
    sizeof(LATIN_LOWER) / sizeof(LATIN_LOWER[0]);

// Declare implementation entry so inline wrappers below compile
void strcase_toggle(std::string *s, int mod);

// Inline wrappers rely on the implementation above
inline void toupperstr(std::string *s) { strcase_toggle(s, 0); }
inline void tolowerstr(std::string *s) { strcase_toggle(s, 1); }
inline void togglecasestr(std::string *s) { strcase_toggle(s, 2); }

namespace testing
{
	namespace internal
	{

		class String
		{
		public:
			// Clone a 0-terminated C string using new[]. Caller must delete[] the result.
			// Returns nullptr if input is nullptr.
			static const char *CloneCString(const char *c_str);

			// Compare two C strings (handles nullptr). nullptr != any non-null string.
			static bool CStringEquals(const char *lhs, const char *rhs);

			// Convert a wide C string to a UTF-8 std::string.
			// nullptr => "(null)". On conversion failure returns "(failed to convert from wide string)".
			static std::string ShowWideCString(const wchar_t *wide_c_str);

			// Compare two wide C strings (handles nullptr).
			static bool WideCStringEquals(const wchar_t *lhs, const wchar_t *rhs);

			// Compare two C strings, ignoring case (handles nullptr).
			static bool CaseInsensitiveCStringEquals(const char *lhs, const char *rhs);

			// Compare two wide C strings, ignoring case (handles nullptr).
			static bool CaseInsensitiveWideCStringEquals(const wchar_t *lhs, const wchar_t *rhs);

			// Return true if str ends with suffix ignoring case.
			static bool EndsWithCaseInsensitive(const std::string &str,
												const std::string &suffix);

			// Format utilities
			static std::string FormatIntWidth2(int val);
			static std::string FormatIntWidthN(int val, int width);
			static std::string FormatHexInt(int val);
			static std::string FormatHexUint32(uint32_t val);
			static std::string FormatByte(unsigned char value);

		private:
			String();
		};
		std::string StringStreamToString(::std::stringstream *stream);
	}
}

void			strcase_toggle(std::string *s, int mod);
std::wstring	ft_str_to_wstr(const std::string &s);
void			append_utf8(std::string *out, uint32_t cp);
uint32_t		decode_utf8_at(const std::string &s, size_t *idx);

#endif