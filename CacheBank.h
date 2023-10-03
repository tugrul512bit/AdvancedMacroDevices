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
	class CacheBank :public Module
	{
	public:
		CacheBank(int frequency, int lithography, int capacity, int evictionPolicy /* 0=direct, 1=associative LRU */)
		{
			_capacity = capacity;
			_lithography = lithography;
			_evictionPolicy = evictionPolicy;
		}

		void Compute() override
		{

		}
	private:

		int _capacity;
		int _lithography;
		int _evictionPolicy;
	};
}