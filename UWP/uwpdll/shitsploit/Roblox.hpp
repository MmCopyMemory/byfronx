#pragma once

#include <string>
#include <Windows.h>

namespace Offsets {
	//UPDATE USING https://github.com/dvlq/always-updated-roblox-offsets
	constexpr uintptr_t get_scheduler = 0x7AAAA0;
	constexpr uintptr_t get_state = 0x4555F0;
	constexpr uintptr_t luavm_load = 0x554E90;
	constexpr uintptr_t task_spawn = 0x554030;
	constexpr uintptr_t print = 0xD71B20;
	constexpr int16_t top = 12;
	constexpr int16_t extra_space = 72, identity = 24;
}

namespace Types {
	using get_scheduler = uintptr_t(__cdecl*)();
	using get_state = uintptr_t(__thiscall*)(uintptr_t script_context, const uint32_t* identity, const uintptr_t* script);
	using luavm_load = int32_t(__fastcall*)(uintptr_t state, std::string* source, const char* chunk_name, int32_t env);
	using task_spawn = int32_t(__cdecl*)(uintptr_t state);
	using print = int32_t(__cdecl*)(int32_t type, const char* message, ...);
}

namespace Roblox {
	const auto bbcdeepinyou = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
	const auto get_scheduler = reinterpret_cast<Types::get_scheduler>(bbcdeepinyou + Offsets::get_scheduler);
	const auto get_state = reinterpret_cast<Types::get_state>(bbcdeepinyou + Offsets::get_state);
	const auto luavm_load = reinterpret_cast<Types::luavm_load>(bbcdeepinyou + Offsets::luavm_load);
	const auto task_spawn = reinterpret_cast<Types::task_spawn>(bbcdeepinyou + Offsets::task_spawn);
	const auto print = reinterpret_cast<Types::print>(bbcdeepinyou + Offsets::print);
	inline void set_identity(uintptr_t state, uint32_t identity) noexcept {
		*reinterpret_cast<int32_t*>(*reinterpret_cast<uintptr_t*>(state + Offsets::extra_space) + Offsets::identity) = identity;
	}
	inline void pop_stack(uintptr_t state, uint8_t amount) noexcept {
		*reinterpret_cast<uintptr_t*>(state + Offsets::top) -= 0x10 * amount;
	}
}
