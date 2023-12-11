#pragma once

#include "Utilities.h"
#include <Windows.h>

DWORD Celestial::Utilities::ScanSignature(const char* AoB, const char* Mask) {
	for (DWORD i = (DWORD)GetModuleHandle(0); i <= 0xF000000; ++i) {

		if (Compare((BYTE*)i, (BYTE*)AoB, Mask))
			return i;
	}
	return 0;
}

std::string Celestial::Utilities::ReadJobString(std::uintptr_t str_address)
{
	const auto length = *reinterpret_cast<int*>(str_address + 0x14);

	if (length >= 16u)
	{
		const auto new_name_address = *reinterpret_cast<std::uintptr_t*>(str_address);
		return read_string_of_unknown_length(new_name_address);
	}
	else
	{
		const auto name = read_string_of_unknown_length(str_address);
		return name;
	}
}

std::uintptr_t Celestial::Utilities::ret()
{
	return std::uintptr_t();
}
