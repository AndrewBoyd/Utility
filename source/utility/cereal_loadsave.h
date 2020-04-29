#pragma once
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <fstream>
#include <filesystem>


template <typename value_t>
void saveToFile_Binary(std::filesystem::path const& filename, value_t const& value)
{
	auto file = std::ofstream(filename, std::ios_base::binary);
	auto archiver = cereal::BinaryOutputArchive(file);
	archiver(value);
}

template <typename value_t>
value_t loadFromFile_Binary(std::filesystem::path const& filename)
{
	auto value = value_t{};
	auto file = std::ifstream(filename, std::ios_base::binary);
	auto archiver = cereal::BinaryInputArchive(file);
	archiver(value);
	return value;
}

template <typename value_t>
void saveToFile_Json(std::filesystem::path const& filename, value_t const& value)
{
	auto file = std::ofstream(filename);
	auto archiver = cereal::JSONOutputArchive(file);
	archiver(value);
}

template <typename value_t>
value_t loadFromFile_Json(std::filesystem::path const& filename)
{
	auto value = value_t{};
	auto file = std::ifstream(filename);
	auto archiver = cereal::JSONInputArchive(file);
	archiver(value);
	return value;
}

template <typename value_t>
void saveToFile_Xml(std::filesystem::path const& filename, value_t const& value)
{
	auto file = std::ofstream(filename);
	auto archiver = cereal::XMLOutputArchive(file);
	archiver(value);
}

template <typename value_t>
value_t loadFromFile_Xml(std::filesystem::path const& filename)
{
	auto value = value_t{};
	auto file = std::ifstream(filename);
	auto archiver = cereal::XMLInputArchive(file);
	archiver(value);
	return value;
}

// This deduces the file type based on the extenstion
template <typename value_t>
void saveToFile(std::filesystem::path const& filename, value_t const& value)
{
	auto extension = filename.extension();
	if (extension == ".json")
		saveToFile_Json(filename, value);
	else if (extension == ".xml")
		saveToFile_Xml(filename, value);
	else
		saveToFile_Binary(filename, value);
}

// This deduces the file type based on the extenstion
template <typename value_t>
value_t loadFromFile(std::filesystem::path const& filename)
{
	auto extension = filename.extension();
	if (extension == ".json")
		return loadFromFile_Json<value_t>(filename);
	else if (extension == ".xml")
		return loadFromFile_Xml<value_t>(filename);
	else
		return loadFromFile_Binary<value_t>(filename);
}


