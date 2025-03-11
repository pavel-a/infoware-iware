// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef _WIN32


#include "infoware/cpu.hpp"
#include "infoware/detail/scope.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


std::uint64_t iware::cpu::frequency() noexcept {
	HKEY hkey;
	if(ERROR_SUCCESS != RegOpenKeyExA(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)", 0, KEY_READ, &hkey)) {
		// Fallback to QueryPerformanceFrequency is not good on modern systems. Better return 0 on error.
		return 0;
	}
	iware::detail::quickscope_wrapper hkey_closer{[&] { RegCloseKey(hkey); }};

	DWORD freq_mhz;
	DWORD freq_mhz_len = sizeof(freq_mhz);
	if(ERROR_SUCCESS != RegQueryValueExA(hkey, "~MHz", nullptr, nullptr, static_cast<LPBYTE>(static_cast<void*>(&freq_mhz)), &freq_mhz_len))
		return 0;

	return static_cast<std::uint64_t>freq_mhz * 1'000'000U;
}


#endif
