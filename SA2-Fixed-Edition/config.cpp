#include "pch.h"
#include "IniFile.hpp"
#include "config.h"

namespace config
{
	void read(const char* path)
	{
		const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
		const IniGroup* general = config->getGroup("");

		if (general)
		{
			
		}

		delete config;
	}
}