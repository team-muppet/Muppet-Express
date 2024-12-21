#pragma once

#include <string>
#include <algorithm>


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
			// Generate something Guid like in c++
		}

		static std::string convert(const std::string& idStr) {
			return idStr;
		}
	};
}