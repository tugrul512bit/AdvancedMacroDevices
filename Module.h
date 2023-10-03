#pragma once
#include"Skill.h"
#include<vector>
#include<queue>
namespace Design
{
	// work flow of "build"
	// prepare module-module connections (top,right,bottom,left)
	// prepare module relations (cache controller -  cache banks, control unit - alus, etc)

	// work flow of "run"
	// push to outputs
	// push to inputs
	// compute

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
		MicroOpSync,

		// result (which has a target module type or id to go)
		Result
	};

	enum FpuInstructionType
	{
		Add,
		Mul,
		Sub,
		Div
	};

	enum ModuleType
	{
		// bus
		Connection,

		// alu, fpu
		Computation,

		// ram, cache bank
		Storage,

		// control unit, cache controller
		Control
	};

	// the data that is passing between modules
	struct Data
	{
		// type of module to take this data
		// if input queue of module is full, it goes to another module
		ModuleType targetModuleType;
		DataType type;

		// can be instruction or parameter
		/*
			fpu:
				0=add
				1=mul
				2=sub
				3=div
				4=sqrt
				5=pow
				6=exp
				7=log
				8=mod
				9=less than
				10=greater than
				11=equal
				12=fma

		*/
		int value; 
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
			for (int i = 0; i < 4; i++)
				connectedModules[i] = nullptr;
			_isBusy = false;
			_failProbability = 0.0f;
			_nanometer = 1000;
			_type = ModuleType::Connection;
		}
		
		virtual void ComputeOutput() {}
		virtual void AddInput(Data input) {}
		virtual Data GetOutput() {}
		virtual int GetModuleType() {}

		std::vector<Module*> GetConnectedModules(Module* source = nullptr)
		{
			std::vector<Module*> result;
			for (int i = 0; i < 4; i++)
			{
				if (connectedModules[i])
				{
					if (connectedModules[i] != source)
						result.push_back(connectedModules[i]);
				}
			}
			return result;
		}

		virtual void Compute(){	}
	protected:		
		ModuleType _type;
		bool _isBusy; // when it is computing something, it is busy and producing heat
		float _failProbability; // all transistors have a failure probability, hence the module failure, per clock. the more transistors the more failure chance.
		int _nanometer;
		int _numTransistors;
		
		int _thermalDissipationPower;
		std::vector<SkillRequirement> _skillRequirements;
		std::vector<StatRequirement> _statRequirements;
		std::queue<Data> _inputQueue;
		std::queue<Data> _outputQueue;
		std::vector<DataType> _commandFilter; // takes only these kind of commands to work
		Module* connectedModules[4];
	};


}