#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <nlohmann/json.hpp>
#include <string>
#include <memory_resource>

struct Pokemon {
    using IdType = int;

    IdType Id;
    std::string Name;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Pokemon, Id, Name)

struct PmrPokemon {
    using IdType = int;
	//using allocator_type = std::pmr::polymorphic_allocator<std::byte>;
    IdType Id;
    std::pmr::string Name;

    // Constructor that accepts an ID, a name, and optionally a memory_resource
    PmrPokemon(IdType id = 0,
        std::string_view name = "",
        std::pmr::memory_resource* mr = std::pmr::get_default_resource())
        : Id(id)
        , Name(name, mr) // Construct pmr::string with the provided resource
    {
    }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PmrPokemon, Id, Name)

Pokemon operator"" _pokemon(const char* str, std::size_t len) {
    std::string input(str);
    auto delimiter_pos = input.find(',');
    if (delimiter_pos == std::string::npos) {
        throw std::invalid_argument("Invalid format. Use 'id_name' format.");
    }

    // Extract the ID and Name
    int id = std::stoi(input.substr(0, delimiter_pos));
    std::string name = input.substr(delimiter_pos + 1);

    return Pokemon{ id, name };
}

PmrPokemon operator"" _pmrPokemon(const char* str, std::size_t len) {
	std::string input(str);
	auto delimiter_pos = input.find(',');
	if (delimiter_pos == std::string::npos) {
		throw std::invalid_argument("Invalid format. Use 'id_name' format.");
	}
	// Extract the ID and Name
	int id = std::stoi(input.substr(0, delimiter_pos));
	std::string name = input.substr(delimiter_pos + 1);
	return PmrPokemon{ id, name };
}

