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
    std::string Image;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Pokemon, Id, Name, Image)

struct PmrPokemon {
    using IdType = int;
	using allocator_type = std::pmr::polymorphic_allocator<std::byte>;
    IdType Id;
    allocator_type _alloc;
    std::pmr::string Name;
	std::pmr::string Image;

	PmrPokemon(std::allocator_arg_t, const allocator_type& alloc, const PmrPokemon& other)
		: Id(other.Id)
		, _alloc(alloc)
		, Name(other.Name, alloc)
		, Image(other.Image, alloc)
	{
	}

	PmrPokemon(IdType id,
		std::string_view name,
		std::string_view image,
		const allocator_type& alloc)
		: Id(id)
		, _alloc(alloc)
		, Name(name, alloc)
		, Image(image, alloc)
	{
	}

    PmrPokemon(IdType id = 0,
        std::string name = "",
        std::string image = "")
        : Id(id)
        , Name(name, allocator_type{})
		, _alloc(allocator_type{}),
        Image("", allocator_type{})
    {
    }

	PmrPokemon(const PmrPokemon& other)
		: PmrPokemon(other, allocator_type{})
	{ }

	PmrPokemon(const PmrPokemon& other, allocator_type alloc)
		: _alloc(alloc)
		, Name(alloc)
		, Image(alloc)
		
	{
		operator=(other);
	}

	PmrPokemon& operator=(const PmrPokemon& other) {
		
		Id = other.Id;
		Name = other.Name;
		Image = other.Image;
		return *this;
	}

	allocator_type get_allocator() const {
		return _alloc;
	}

	// Move constructor
	PmrPokemon(PmrPokemon&& other) noexcept
		: PmrPokemon(std::move(other), other.get_allocator())
	{
	}

	PmrPokemon(PmrPokemon&& other, allocator_type alloc) noexcept
		: _alloc(alloc)
	{
		operator=(std::move(other));
	}

	// Move assignment operator
	PmrPokemon& operator=(PmrPokemon&& other) noexcept {
		if (_alloc == other._alloc) {
			std::swap(Id, other.Id);
			std::swap(Name, other.Name);
			std::swap(Image, other.Image);
		}
		else
		{
			operator=(other);
		}

		return *this;
	}
	 //Destructor
	~PmrPokemon() {
	}

};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PmrPokemon, Id, Name, Image)

Pokemon operator"" _pokemon(const char* str, std::size_t len) {
    std::string input(str);
    auto delimiter_pos = input.find(',');
    if (delimiter_pos == std::string::npos) {
        throw std::invalid_argument("Invalid format. Use 'id,name' format.");
    }

    int id = std::stoi(input.substr(0, delimiter_pos));
    std::string name = input.substr(delimiter_pos + 1);

    return Pokemon{ id, name, ""};
}

PmrPokemon operator"" _pmrPokemon(const char* str, std::size_t len) {
	std::string input(str);
	auto delimiter_pos = input.find(',');
	if (delimiter_pos == std::string::npos) {
		throw std::invalid_argument("Invalid format. Use 'id,name' format.");
	}

	int id = std::stoi(input.substr(0, delimiter_pos));
	std::string name = input.substr(delimiter_pos + 1);
	return PmrPokemon{ id, name, ""};
}

