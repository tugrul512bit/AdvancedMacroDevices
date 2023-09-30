#pragma once
#include<string>
#include"Character.h"
namespace States
{
	const static int versionMajor = 0;
	const static int versionMinor = 0;
	const static int versionFix = 1;

	std::string GetVersion()
	{
		return 
			std::string("v") + 
			std::to_string(versionMajor) + 
			std::string(".") + 
			std::to_string(versionMinor) + 
			std::to_string(versionFix);
	}
}