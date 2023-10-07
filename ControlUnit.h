#pragma once
#include"Module.h"
#include"Tech.h"
#include"Skill.h"
#include<vector>
#include<queue>
#include<cmath>

namespace Design
{
	// cache bank stores cached data
	// input: memory request
	// output: data	
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