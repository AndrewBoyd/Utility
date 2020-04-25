#pragma once
#include <nlohmann/json.hpp>
#include <filesystem>
#include <string>
#include <mutex>

namespace global_config 
{
	std::filesystem::path appdataPath();

	void init(std::filesystem::path config_path);

	nlohmann::json& get();
	std::mutex& lock();
}
