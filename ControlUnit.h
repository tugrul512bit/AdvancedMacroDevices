#pragma once
#include"Module.h"
#include"Tech.h"
#include"Skill.h"
#include<vector>
#include<queue>
#include<cmath>

namespace Design
{

	// controls other modules
	class ControlUnit :public Module
	{
	public:
		ControlUnit(int frequency, int lithography)
		{
			_frequency = frequency;
			_lithography = lithography;
			_type = ModuleType::CONTROL_UNIT;
		}

		void Compute() override
		{

		}
	private:

	};
}