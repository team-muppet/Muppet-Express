#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <nlohmann/json.hpp>
#include <string>

struct Pokemon {
    using IdType = int;

    IdType Id;
    std::string Name;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Pokemon, Id, Name)


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

