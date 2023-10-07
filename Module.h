#pragma once
#include"Skill.h"
#include<vector>
#include<queue>
#include<memory>

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
		BUS,
		CONTROL_UNIT,
		CACHE_CONTROL,
		MEMORY_CONTROL,
		ALU,
		FPU,
		CACHE_BANK,
		BRANCH_PREDICTOR,

		// for searching modules through bus connections
		ANY
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

	static int GetUniqueId()
	{
		static int id=0;
		return id++;
	}

	// adjacent modules (top,bot,left,right) make connections automatically.
	// output is broadcasted to 4 neighbors
	// input is made of 4 outputs of neighbors
	// if there is a bus connected, all connected modules to the bus are neighbors (unlimited number of modules)
	class Module
	{
	public:
		Module()
		{			
			_id = GetUniqueId();
			_directConnectedModules.resize(4);
			for (int i = 0; i < 4; i++)
				_directConnectedModules[i] = nullptr;
			_isBusy = false;
			_failProbability = 0.0f;
			_lithography = 1000;
			_type = ModuleType::ANY; // just a default
			_numTransistors = 0;
			_thermalDissipationPower = 0;
			_frequency = 1; // 1 means equal frequency to outer source. 2 means 2x frequency or 2 iterations per cycle
		}
		
		int GetId() { return _id; }

		virtual void ComputeOutput() {}
		virtual void AddInput(Data input) {}
		virtual Data GetOutput() { return Data(); }
		virtual ModuleType GetModuleType() { return _type; }
		virtual bool GetBusyness() { return _isBusy; }
		std::vector<Module*> GetConnectedModulesExceptThis(Module* source = nullptr)
		{
			std::vector<Module*> result;
			for (int i = 0; i < 4; i++)
			{
				auto ptr = _directConnectedModules[i].get();
				if (ptr)
				{
					if (ptr != source)
						result.push_back(ptr);
				}
			}
			return result;
		}

		virtual void Compute(){	}

		template<typename T>
		T* AsPtr()
		{
			return (T*)this;
		}

		void Connect(std::shared_ptr<Module> conn, int index)
		{
			_directConnectedModules[index]=conn;
		}
	protected:		
		int _id;
		ModuleType _type;
		bool _isBusy; 
		float _failProbability; // all transistors have a failure probability, hence the module failure, per clock. the more transistors the more failure chance.
		int _lithography;

		// actual frequency is controlled from outside, this is just a multiplier (that is checked for triggering compute at signal)
		int _frequency;
		int _numTransistors;
		int _thermalDissipationPower;


		std::vector<SkillRequirement> _skillRequirements;
		std::vector<StatRequirement> _statRequirements;


		// maximum 4 modules can be connected to a module. top, right, bottom, left
		std::vector<std::shared_ptr<Module>> _directConnectedModules;
		

		std::vector<DataType> _commandFilter; // takes only these kind of commands to work
	};


}