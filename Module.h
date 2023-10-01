#pragma once
#include"Skill.h"

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
		}

		virtual void ComputeOutput() {}
		virtual void AddInput(Data input) {}
		virtual Data GetOutput() {}
	protected:
		bool _isPipelined; // pipelining overlaps execution and i/o. rest of the pipelining is modeled as "less/more latency"
		bool _isBusy; // when it is computing something, it is busy and producing heat
		float _failProbability; // all transistors have a failure probability, hence the module failure, per clock. the more transistors the more failure chance.
		std::vector<SkillRequirement> _skillRequirements;
		std::vector<StatRequirement> _statRequirements;
	};

	class Cache :public Module
	{
	public:
		Cache():Module()
		{
			_skillRequirements.push_back(SkillRequirement(ElectronicsSkill().GetName(), 5));
			_statRequirements.push_back(StatRequirement("Intelligence", 11));
		}
	private:
	};
}