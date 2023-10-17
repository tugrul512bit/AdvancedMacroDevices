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
	// cache controller receives memory read/write requests, then serves data from cache banks or RAM
	// input: memory request
	// output: data (cache hit)
	// output2: memory request (cache miss)
	// input2: memory response (from RAM or ALU or FPU or whatever that is being cached)
	// output3: coherency update response (write)
	// input3: coherency update request
	/*
		Algorithm
		- if request is write: send coherency update to all linked cache controllers
		- if request is sync: wait until in-flight coherency updates reaches zero (cc1->coherency->cc2->acknowledge->cc1)
		- if request is read: check if any linked cache bank contains the address (direct mapped or LRU)
		-   if found -> cache hit -> get data from bank -> response
		-   if not found -> cache miss -> get from RAM -> response
		-		if full, evict one

		No limit for number of cache banks per controller. But longer path = higher latency for some data
	*/
	class CacheController :public Module
	{
	public:
		CacheController(int frequency, int lithography, int capacity, int evictionPolicy /* 0=direct, 1=associative LRU */, int parallelism) :
			Module(parallelism, parallelism, lithography,/*numTransistors*/ 1, ModuleType::CACHE_CONTROL,
				/* thermalDissipationPower */ 1, frequency, /* failProbability*/ 0.0f)
		{
			for (int i = 0; i < 4; i++)
				_directConnectedModules[i] = nullptr;

			// this module can only take memory requests, nothing else, but anything can be giving them, even another cache
			_commandFilter.push_back(DataType::InstructionMemRead);
			_commandFilter.push_back(DataType::InstructionMemWrite);

			// requires level 5 electronics to unlock
			_skillRequirements.push_back(SkillRequirement(ElectronicsSkill().GetName(), 5 + 2 * std::sqrt(lithography)));
			// requires 10 intelligence to unlock
			_statRequirements.push_back(StatRequirement("Intelligence", 10 + std::sqrt(frequency)));


			
			_capacity = capacity;

			_evictionPolicy = evictionPolicy;

		}

		static std::shared_ptr<Module> Create(int frequency, int lithography, int parallelism, int capacity, int evictionPolicy)
		{
			return std::make_shared<CacheController>(frequency, lithography, capacity, evictionPolicy, parallelism);
		}

		void Compute(int clockCycleId) override
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
		int _evictionPolicy;

	
	};
}