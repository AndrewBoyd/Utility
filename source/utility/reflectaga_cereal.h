#pragma once
#include "messy_header.h"

#include "reflectaga.h"
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>

template<typename archive_t, reflectaga::Reflectaga_t reflectable_t >
void serialize(archive_t& archive, reflectable_t& value)
{
	auto action = [&archive](auto& v, const char* name)
	{
		archive(::cereal::make_nvp(name, v));
	};
	reflectAction(action, value);
}
