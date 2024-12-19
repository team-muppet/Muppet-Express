#pragma once

#include <nlohmann/json.hpp>
#include <string>

struct Pokemon {
    int Id;
    std::string Name;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Pokemon, Id, Name)
