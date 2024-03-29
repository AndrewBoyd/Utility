#pragma once
#include <string>
#include <vector>

namespace string_utils 
{
	std::string b_to_s(bool x);
	std::string concat(std::vector<std::string> const& strings, std::string delimiter);
	std::u8string concat(std::vector<std::u8string> const& strings, std::string delimiter);
	std::vector< std::string > split(std::string string, char delimiter);

	std::string u8_to_ascii(std::u8string string);
	std::u8string ascii_to_u8(std::string string);

	std::u8string to_u8(std::string string);
	std::u8string to_u8(std::u8string string);

	std::string to_ascii(std::string string);
	std::string to_ascii(std::u8string string);
}
