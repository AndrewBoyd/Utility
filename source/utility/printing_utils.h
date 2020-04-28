#pragma once
#include "reflectaga.h"
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <concepts>

namespace printing_utils
{
	template <typename value_t>
	concept StringStreamable_t = requires(std::ostream & stream, value_t const& value) { stream << value; };

	template <typename value_t>
	concept PrimitivePrintable = StringStreamable_t<value_t> && !reflectaga::Reflectaga_t<value_t>;

	template <typename value_t>
	concept RecursivePrintable = reflectaga::Reflectaga_t<value_t>;

	template <RecursivePrintable value_t>
	void printValue(std::ostream& s, value_t const& value);


	template <PrimitivePrintable value_t>
	void printValue(std::ostream& stream, value_t const& value)
	{
		stream << value;
	}

	template <typename value_t>
	void printValue(std::ostream& stream, std::vector<value_t> const& vector)
	{
		stream << "[ ";
		for (auto it = vector.begin(); it != vector.end(); it++)
		{
			if (it != vector.begin()) stream << ", ";
			printValue(stream, *it);
		}
		stream << " ]";
	}

	void printValue(std::ostream& stream, std::string const& string)
	{
		stream << fmt::format(R"("{}")", string);
	}

	template <RecursivePrintable value_t>
	void printValue(std::ostream& stream, value_t const& value)
	{
		auto action = [&stream](auto const& v, const char* name)
		{
			stream << fmt::format(R"("{}": )", name);
			printValue(stream, v);
			stream << "\n";
		};
		stream << "{\n";
		reflectAction(action, const_cast<value_t&>(value));
		stream << "}";
	}
}

template <reflectaga::Reflectaga_t value_t>
std::ostream & operator<<(std::ostream & stream, value_t const & value)
{
	printing_utils::printValue(stream, value);
	return stream;
}

