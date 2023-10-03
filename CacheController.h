#pragma once
#include"Module.h"
#include"Tech.h"
#include"Skill.h"
#include<vector>
#include<queue>
#include<cmath>

namespace Design
{
	// cache controller receives memory read/write requests, then serves data from cache banks or RAM
	// input: memory request
	// output: data (cache hit)
	// output2: memory request (cache miss)
	class CacheController :public Module
	{
	public:
		CacheController(int frequency, int lithography, int capacity, int evictionPolicy /* 0=direct, 1=associative LRU */)
		{
			for (int i = 0; i < 4; i++)
				connectedModules[i] = nullptr;

			// this module can only take memory requests, nothing else, but anything can be giving them, even another cache
			_commandFilter.push_back(DataType::InstructionMemRead);
			_commandFilter.push_back(DataType::InstructionMemWrite);

			// requires level 5 electronics to unlock
			_skillRequirements.push_back(SkillRequirement(ElectronicsSkill().GetName(), 5 + 2 * std::sqrt(lithography)));
			// requires 10 intelligence to unlock
			_statRequirements.push_back(StatRequirement("Intelligence", 10 + std::sqrt(frequency)));

			// this is a control unit, can manage other modules (such as memory banks of cache through a bus / multiple buses)
			_type = ModuleType::Control;

			
			_numTransistors = std::pow(capacity,1.5f)+300 + std::pow(frequency,1.1f)*100;
			_thermalDissipationPower = (evictionPolicy > 0 ? 2.0f : 1.0f) * _numTransistors * frequency * 3.5f / lithography;
			_capacity = capacity;
			_lithography = lithography;
			_evictionPolicy = evictionPolicy;
		}

		void Compute() override
		{
			// check command queue
			// if command found
			//	check if cache banks have it
			//		return data to output queue
			//	else
			//		pass command to output queue (RAM/other cache)
		}
	private:

		int _capacity;
		int _lithography;
		int _evictionPolicy;

		// finds all connected cache banks and maps them to memory requests 
		//	for direct-mapped cache: first bank serves x % k == 0, second bank serves x % k == 1, ...
		std::vector<Module*> FollowBusThenMapBanks()
		{
			struct ModuleWithSource
			{
				Module* target;
				Module* source;
			};
			std::queue<ModuleWithSource> q;
			std::vector<Module*> banks;
			for (int i = 0; i < 4; i++)
			{
				if (connectedModules[i])
				{
					if (connectedModules[i]->GetModuleType() == ModuleType::Connection || connectedModules[i]->GetModuleType() == ModuleType::Storage)
					{
						q.push({ connectedModules[i],(Module*)this });
					}
				}
			}

			// find all connected storage modules through connections
			while (!q.empty())
			{
				ModuleWithSource m = q.front();
				q.pop();

				if (m.target->GetModuleType() == ModuleType::Storage)
				{
					banks.push_back(m.target);
				}

				if (m.target->GetModuleType() == ModuleType::Connection)
				{
					std::vector<Module*> conn = m.target->GetConnectedModules(m.source);
					for (auto& c : conn)
					{
						q.push({ c,m.target });
					}
				}

			}

			// then map banks to memory address modulus
			return banks;
		}
	};
}