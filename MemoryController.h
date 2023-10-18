#pragma once
#include"Module.h"
#include"Tech.h"
#include"Skill.h"
#include<vector>
#include<queue>
#include<cmath>
#include<memory>
namespace Design
{
	
	class MemoryController :public Module
	{
	public:
		MemoryController(int frequency, int lithography, int capacity, int parallelism) :
			Module(parallelism, parallelism, lithography,/*numTransistors*/ 1, ModuleType::MEMORY_CONTROL,
				/* thermalDissipationPower */ 1, frequency, /* failProbability*/ 0.0f)
		{
			for (int i = 0; i < 4; i++)
				_directConnectedModules[i] = nullptr;

			_commandFilter.push_back(DataType::InstructionMemRead);
			_commandFilter.push_back(DataType::InstructionMemWrite);

			// requires level 5 electronics to unlock
			_skillRequirements.push_back(SkillRequirement(ElectronicsSkill().GetName(), 5 + 2 * std::sqrt(lithography)));
			// requires 10 intelligence to unlock
			_statRequirements.push_back(StatRequirement("Intelligence", 10 + std::sqrt(frequency)));

			_capacity = capacity;

		}

		static std::shared_ptr<Module> Create(int frequency, int lithography, int parallelism, int capacity)
		{
			return std::make_shared<MemoryController>(frequency, lithography, capacity, parallelism);
		}

		void Compute(int clockCycleId) override
		{

		}
	private:
		int _capacity;
	};
}