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
		static int generateId() {
			static int counter = 0;
			return ++counter;
		}

		static int convert(const std::string& idStr) {
			return std::stoi(idStr);
		}
	};

	template <>
	struct IdTraits<std::string> {
		static std::string generateId() {
			return boost::uuids::to_string(boost::uuids::random_generator()());
		}

		static std::string convert(const std::string& idStr) {
			return idStr;
		}
	};
}