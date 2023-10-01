#pragma once
#include"Skill.h"
#include<vector>
#include<queue>
namespace Design
{
	// for simple processors, some instructions generate many micro operations (like 1 square root = a lot of multiplications, additions, etc)
	enum DataType
	{
		// data between RAM and decoder (may pass through cache)
		InstructionFpu,
		InstructionAlu,
		InstructionMemRead,
		InstructionMemWrite,
		InstructionParameter1,
		InstructionParameter2,
		InstructionParameter3,
		InstructionParameter4,
		InstructionParameter5,
		InstructionJump,
		InstructionSync, // this is for a new & easy multi-threading model that runs on single core

		// data between anything
		DataChunk,

		// (command) data between decoder and other modules
		MicroOpFpu,
		MicroOpAlu,
		MicroOpMemRead,
		MicroOpMemWrite,
		MicroOpParameter1,
		MicroOpParameter2,
		MicroOpParameter3,
		MicroOpParameter4,
		MicroOpParameter5,
		MicroOpBranchPrediction,
		MicroOpSync
	};

	// the data that is passing between modules
	struct Data
	{
		DataType type;
		int valueInt;
		float valueFloat;
	};

	// adjacent modules (top,bot,left,right) make connections automatically.
	// output is broadcasted to 4 neighbors
	// input is made of 4 outputs of neighbors
	// if there is a bus connected, all connected modules to the bus are neighbors (unlimited number of modules)
	class Module
	{
	public:
		Module()
		{
			_isPipelined = false;
			_isBusy = false;
			_failProbability = 0.0f;
			_nanometer = 1000;
		}

		virtual void ComputeOutput() {}
		virtual void AddInput(Data input) {}
		virtual Data GetOutput() {}
	protected:
		bool _isPipelined; // pipelining overlaps execution and i/o. rest of the pipelining is modeled as "less/more latency"
		bool _isBusy; // when it is computing something, it is busy and producing heat
		float _failProbability; // all transistors have a failure probability, hence the module failure, per clock. the more transistors the more failure chance.
		int _nanometer;
		std::vector<SkillRequirement> _skillRequirements;
		std::vector<StatRequirement> _statRequirements;
		std::queue<Data> _commandQueue; // for ease of design, control-logic is exchanged with simple command queue per module
		std::queue<Data> _dataQueue;
		std::vector<DataType> _commandFilter; // takes only these kind of commands to work
	};

	class Cache :public Module
	{
	public:
		Cache(int capacity, int latencyHit, int latencyMiss, int ways, int eviction):Module()
		{
			_capacity = capacity;
			_latencyHit = latencyHit;
			_latencyMiss = latencyMiss;
			_numWays = ways;
			_evictionPolicy = eviction;

			// requirements to create a cache
			_skillRequirements.push_back(SkillRequirement(ElectronicsSkill().GetName(), 5 + capacity * 5 - latencyHit * 4 - latencyMiss + ways * 3 + (eviction==1 ? 4 : 0)));
			_statRequirements.push_back(StatRequirement("Intelligence", 11));

			// cache takes only these commands
			// read: MicroOpMemRead + MicroOpParameter1
			// write: MicroOpMemWrite + MicroOpParameter1 + MicroOpParameter2
			_commandFilter.push_back(DataType::MicroOpMemRead);
			_commandFilter.push_back(DataType::MicroOpMemWrite);
			_commandFilter.push_back(DataType::MicroOpParameter1); // address to read/write
			_commandFilter.push_back(DataType::MicroOpParameter2); // data chunk to read/write
		}
	private:
		// cache size in number of maximum "Data"
		// increases with electronics, physics skills
		int _capacity; 

		// number of cycles for cache-hit
		// decreases with electronics, algorithm, physics skills
		int _latencyHit; 

		// number of cycles for cache-miss, excluding RAM latency
		// decreases with electronics, algorithm, physics skills
		int _latencyMiss; 

		// number of parallel data lines to serve multiple clients concurrently or single client with more data
		// increases with electronics
		int _numWays; 

		// 0=direct-mapped, 1=fully associative LRU
		// unlocks LRU with algorithm & electronics
		int _evictionPolicy; 
	};
}