#include "stdafx.h"
#include "Utils.hpp"

namespace bjoernligan
{
	std::vector<std::string> explode(const std::string& string, const std::string& delimeter)
	{
		std::vector<std::string> parts;
		std::size_t oldPos = 0;
		while (true)
		{
			std::size_t newPos = string.find(delimeter, oldPos);
			if (newPos == std::string::npos)
			{
				parts.push_back(string.substr(oldPos, newPos - oldPos));
				break;
			}
			parts.push_back(string.substr(oldPos, newPos - oldPos));
			oldPos = newPos + 1;
		}

		return parts;
	}

	bool string_to_bool(const std::string& string)
	{
		if (string == "0" || string.empty() || string == "false")
			return false;
		return true;
	}
}