#pragma once

#include "messy_header.h"

#include "reflectaga.h"
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <concepts>
#include <map>

namespace printing_utils
{
	template <typename value_t>
	concept StringStreamable_t = requires(std::ostream & stream, value_t const& value) { stream << value; };

	template <typename value_t>
	concept RecursivePrintable_t = reflectaga::Reflectaga_t<value_t> && !StringStreamable_t<value_t>;
}

template <typename value_t>
std::ostream& operator<<(std::ostream& stream, std::vector<value_t> const& vector)
{
	stream << "[ ";
	for (auto it = vector.begin(); it != vector.end(); it++)
	{
		if (it != vector.begin()) stream << ", ";
		stream << *it;
	}
	stream << " ]";
	return stream;
}

template <typename key_t, typename value_t>
std::ostream& operator<<(std::ostream& stream, std::map<key_t, value_t> const& map)
{
	stream << "{";
	for (auto it = map.begin(); it != map.end(); it++)
	{
		if (it != map.begin()) stream << ",";
		stream << "\n" << fmt::format(R"({}: {})", it->first, it->second);
	}
	stream << "\n}";
	return stream;
}

template <printing_utils::RecursivePrintable_t value_t>
std::ostream& operator<<(std::ostream& stream, value_t const& value)
{
	auto action = [&stream](auto const& v, const char* name)
	{
		stream << fmt::format(R"("{}": )", name);
		stream << v;
		stream << "\n";
	};
	stream << "{\n";
	reflectAction(action, const_cast<value_t&>(value));
	stream << "}";
	return stream;
}
