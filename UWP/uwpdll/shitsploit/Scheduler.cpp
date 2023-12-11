#include <string>
#include <stdexcept>

#include "Scheduler.hpp"
#include "Roblox.hpp"

uintptr_t Scheduler::get_job(const char* job_name) noexcept {
	static auto scheduler = uintptr_t();
	if (!scheduler)
		scheduler = Roblox::get_scheduler();
	auto jobs_start = *reinterpret_cast<uintptr_t*>(scheduler + 0x134);
	const auto jobs_end = *reinterpret_cast<uintptr_t*>(scheduler + 0x138);
	auto result = uintptr_t();
	for (; jobs_start < jobs_end; jobs_start += 8u) {
		const auto job = *reinterpret_cast<uintptr_t*>(jobs_start);
		if (*reinterpret_cast<std::string*>(job + 0x80) == job_name)
			result = job;
	}
	return result;
}

uintptr_t Scheduler::get_script_context() {
	if (const auto job = Scheduler::get_job("WaitingHybridScriptsJob"))
		return *reinterpret_cast<uintptr_t*>(job + 0x1A8);
	else
		throw std::runtime_error("Couldn't find WaitingHybridScriptsJob");
}
