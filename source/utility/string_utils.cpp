#include "string_utils.h"
#include <sstream>

namespace string_utils 
{
	std::string b_to_s(bool x)
	{
		if (x) return "true";
		return "false";
	}

	std::string concat(std::vector<std::string> const& strings, std::string delimiter)
	{
		auto result = std::stringstream();
		for (auto string_it = strings.begin(); string_it != strings.end(); string_it++)
		{
			if (string_it != strings.begin()) result << delimiter;
			result << *string_it;
		}
		return result.str();
	}

	std::vector<std::string> split(std::string string, char delimiter)
	{
		auto result = std::vector<std::string>();
		auto stream = std::stringstream(string);
		auto buffer = std::string{};

		while (std::getline(stream, buffer, delimiter)) 
		{
			result.push_back(buffer);
		}

		return result;
	}


	std::string u8_to_ascii(std::u8string string)
	{
		auto view = std::u8string_view(string);
		auto casted = reinterpret_cast<std::string_view&>(view);
		return std::string(casted);
	}

	std::u8string ascii_to_u8(std::string string)
	{
		auto view = std::string_view(string);
		auto casted = reinterpret_cast<std::u8string_view&>(view);
		return std::u8string(casted);
	}

}
