#pragma once

#include <string>
#include <algorithm>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


namespace MuppetExpress {
	template <typename IdType>
	struct IdTraits;

	template <>
	struct IdTraits<int> {
		int counter = 0;

		int generateId() {
			return ++counter;
		}

		int convert(const std::string& idStr) {
			return std::stoi(idStr);
		}
	};

	template <>
	struct IdTraits<std::string> {
		std::string generateId() {
			return boost::uuids::to_string(boost::uuids::random_generator()());
		}

		std::string convert(const std::string& idStr) {
			return idStr;
		}
	};
}