#pragma once

#include <string>
#include <nlohmann/json.hpp>

struct Person {
    using IdType = std::string;

    IdType Id;
    std::string FirstName;
    std::string LastName;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Person, Id, FirstName, LastName)
