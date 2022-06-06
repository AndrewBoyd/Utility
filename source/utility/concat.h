#pragma once
#include <vector>

namespace stdish
{
	template <typename value_t>
	std::vector< value_t > concat(std::vector< std::vector< value_t > > nested);





	template <typename value_t>
	std::vector< value_t > concat(std::vector< std::vector< value_t > > nested) {
		auto result = std::vector< value_t >{};
		for (auto vec : nested) {
			result.insert(result.end(), vec.begin(), vec.end());
		}
		return result;
	}
}
