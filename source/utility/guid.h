#pragma once
#include <string>

namespace guid
{
	using Guid = std::string;
	Guid generateGuid();
	Guid generateGuid_NoBrackets();
}
