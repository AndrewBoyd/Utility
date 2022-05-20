#pragma once
#include <inttypes.h>

namespace bit_twiddle 
{
	template <typename bitmask_t> inline
	int countBits(bitmask_t bitmask) {
		if constexpr (sizeof(bitmask) == 8) {
			return __popcnt64(bitmask);
		}
		else {
			return __popcnt(bitmask);
		}
	}
}
