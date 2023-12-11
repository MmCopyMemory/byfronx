#pragma once

#include "Utilities.h"
#include <Windows.h>
#include <string>

namespace Celestial
{
	class Utilities
	{
	public:
		DWORD ScanSignature(const char* AoB, const char* Mask);

		std::string ReadJobString(std::uintptr_t str_address);

		std::uintptr_t ret();

	private:

		bool Compare(const BYTE* pData, const BYTE* bMask, const char* szMask)
		{
			for (; *szMask; ++szMask, ++pData, ++bMask)
				if (*szMask == 'x' && *pData != *bMask) return 0;
			return (*szMask) == NULL;
		}

		std::string read_string_of_unknown_length(std::uintptr_t address)
		{
			std::string res;
			char character = 0;
			int char_size = sizeof(character);
			int offset = 0;

			res.reserve(204); // 4 * 51 = 204

			while (offset < 200)
			{
				character = *reinterpret_cast<char*>(address + offset);

				if (character == 0) break;

				offset += char_size;

				res.push_back(character);
			}

			return res;
		}

	};
}