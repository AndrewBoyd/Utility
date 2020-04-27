#include "global_config.h"
#include <filesystem>
#include <fstream>


namespace {
	struct
	{
		bool initialised;
		std::mutex mutex;
		nlohmann::json global_conf;
	} private__;
}

namespace global_config
{
	std::filesystem::path appdataPath()
	{
		// TODO: This is not cross-platform
		auto result = std::string{};
		char* appdata = nullptr;
		auto length = size_t{};

		auto error = _dupenv_s(&appdata, &length, "APPDATA");
		if (error) throw std::exception("Error finding APPDATA");

		result = appdata;
		free(appdata);
		return result;
	}

	void init(std::filesystem::path config_path)
	{
		auto lock = std::unique_lock(private__.mutex);
		
		if (private__.initialised)
			throw std::exception("global config has already been initialised");

		private__.global_conf = nlohmann::json::parse( std::ifstream(config_path) );
		private__.initialised = true;
	}

	nlohmann::json& get()
	{
		if (!private__.initialised) 
			throw std::exception("global config has not been initialised");
		
		return private__.global_conf;
	}
	
	std::mutex& lock()
	{
		return private__.mutex;
	}
}
