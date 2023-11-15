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
	// input-1: memory request coming from client
	// input-2: memory response coming from Memory Controller (RAM)
	// input-3: memory response coming from Cache Bank(s)
	// output-1: memory response going to client
	// output-2: memory request going to Memory Controller (RAM)
	// output-3: memory request going to Cache Bank(s)
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

			_nMappedTags = 0;
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


			SetIdle();
			for (int i = 0; i < _parallelism; i++)
			{

				if (GetOutput(i).GetDataType() != Design::DataType::Null)
					continue;

				bool computed = false;
				auto inp = Data();
				for (int j = 0; j < 4; j++)
				{
					if (!computed)
					{

						inp = _input[j][i];
						if (inp.GetDataType() != Design::DataType::Null)
						{

							if (inp.GetDataType() == Design::DataType::MicroOpMemRead)
							{
								computed = true;
								std::cout << "cache control dummy compute: clock id=" << clockCycleId << std::endl;
								SetBusy();

								// memread operation comes from a client
								// check cache tags for address
								std::cout << "cache controller received memory read request" << std::endl;
								std::cout << "finding a cache bank module..." << std::endl;

								// direct-mapped cache
								int address = inp.GetValue();

								// todo: mapping of threads will be required first --> division by zero otherwise
								int tagId = address % _nMappedTags;


								int bankId = -1;
								// output logic will send this to necessary cache bank connected
								// all cache banks will be already mapped before cpu starts working
								SetOutput(Data(
									Design::DataType::MicroOpCacheBankReadTag, // message is "read this tag"
									Design::CACHE_BANK, // a cache bank will receive this message
									bankId, // any cache bank (but selected with eviction policy. direct mapped = modulo)
									tagId,
									Design::ModuleType::CACHE_CONTROL,
									_id, clockCycleId), i);
								
							}
						}

					}

					if (!computed)
					{
						// if not computed, put the data back
						_input[j][i] = inp;
					}
					else
					{
						// if computed, input is cleared for new data
						_input[j][i] = Data();
					}

				}
			}
		}

		// one-time preparation
		void MapConnectedCacheBanksToAddresses()
		{

		}
	private:

		int _capacity;
		int _evictionPolicy;
		int _nMappedTags;
	
	};
}