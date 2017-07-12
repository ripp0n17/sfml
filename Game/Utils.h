#pragma once
#include <cassert>
#include <fstream>
#include "json/json.h"

namespace Utils
{
	static Json::Value openFile(const std::string& path)
	{
		Json::Reader reader;
		Json::Value node;
		std::ifstream file;
		file.open(path);

		if (file.is_open())
		{
			if (!reader.parse(file, node, false))
			{
				assert(false);
			}
			file.close();
		}
		return node;
	}
}
