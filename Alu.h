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
	class Alu :public Module
	{
	public:
		Alu(int frequency, int lithography)
		{
			_frequency = frequency;
			_lithography = lithography;
			_type = ModuleType::ALU;
		}

		void SetBusy() override { _isBusy = true; }
		void Compute() override
		{
			for (int i = 0; i < 4; i++)
			{
				if (_input[i].dataType != Design::DataType::Null)
				{
					std::cout << "!!" << std::endl;
				}
			}
		}
	private:

	};
}