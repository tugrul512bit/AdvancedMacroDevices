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
	// cache bank stores cached data
	// input: memory request
	// output: data	
	class CacheBank :public Module
	{
	public:
		CacheBank(int frequency, int lithography, int capacity, int evictionPolicy /* 0=direct, 1=associative LRU */, int parallelism) :
			Module(parallelism, parallelism, lithography,/*numTransistors*/ 1, ModuleType::CACHE_BANK,
				/* thermalDissipationPower */ 1, frequency, /* failProbability*/ 0.0f)
		{
			_capacity = capacity;
			_evictionPolicy = evictionPolicy;
		}

		static std::shared_ptr<Module> Create(int frequency, int lithography, int parallelism, int capacity, int evictionPolicy)
		{
			return std::make_shared<CacheBank>(frequency, lithography, capacity, evictionPolicy, parallelism);
		}

		void Compute(int clockCycleId) override
		{

		}
	private:

		int _capacity;
		int _evictionPolicy;
	};
}