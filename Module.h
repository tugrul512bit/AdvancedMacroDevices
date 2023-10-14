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
		// empty data
		Null,


		// data between RAM and decoder (may pass through cache)
		InstructionFpu,

		// dummy calc, this is only for debugging / unit-testing
		InstructionAlu,

		// real work for ALU
		// r[i] = r[j] + r[k]
		// i= parameter 1
		// j= parameter 2
		// k= parameter 3
		InstructionAluSumRegisterRegister,


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

		// dummy alu operation for unit-testing
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
		MicroOpDecode,

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
		DECODER,
		// for searching modules through bus connections
		ANY
	};

	// the data that is passing between modules
	struct Data
	{
		// the module that has sent this data
		ModuleType sourceModuleType;
		int sourceModuleId;

		// type of module to take this data
		// if input queue of module is full, it goes to another module
		ModuleType targetModuleType;
		int targetModuleId;
		DataType dataType;

		// whose context is this: id of instruction or branch prediction or thread
		int context;

		// 0: instruction, 1: branch, 2: thread
		int contextType;

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
	
		Data(DataType dataTypePrm=DataType::Null, ModuleType targetModuleTypePrm=ModuleType::ANY, int targetModuleIdPrm=-1, int valuePrm=-1, ModuleType sourceModuleTypePrm = ModuleType::ANY, int sourceModuleIdValue=-1)
		{
			sourceModuleType = sourceModuleTypePrm;
			sourceModuleId = sourceModuleIdValue;
			targetModuleType = targetModuleTypePrm;
			targetModuleId = targetModuleIdPrm;
			dataType = dataTypePrm;
			value = valuePrm;

			context = -1;
			contextType = -1;
		}
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
			_input.resize(4);
			_inputRegister.resize(4);
			_output=Data();
			_directConnectedModules.resize(4);
			for (int i = 0; i < 4; i++)
				_directConnectedModules[i] = nullptr;
			_busynessLevel = 0;
			_busynessLevelMax = 1;
			_failProbability = 0.0f;
			_lithography = 1000;
			_type = ModuleType::ANY; // just a default
			_numTransistors = 0;
			_thermalDissipationPower = 0;
			_frequency = 1; // 1 means equal frequency to outer source. 2 means 2x frequency or 2 iterations per cycle
			_numCompletedOperations = 0;
		}
		int GetCompletedOperationCount() { return _numCompletedOperations; }
		virtual void SetBusy() { _busynessLevel++; }
		virtual void SetIdle() { _busynessLevel = 0; }
		// percentage level: 100 = full, 0 = idle
		virtual int GetBusyness() { return 100 * (_busynessLevel / (float)_busynessLevelMax); }

		int GetId() { return _id; }

		virtual void ComputeOutput() {}

		virtual Data GetInput(int index)
		{
			return _inputRegister[index];
		}
		virtual void SetInput(Data input, int index) 
		{
			_inputRegister[index] = input;
		}
		virtual void ApplyInput()
		{
			for (int i = 0; i < 4; i++)
			{
				// if output & input register is empty, then its ok to compute later				
				// because if output is clogged, there is no reason to compute something that won't go anywhere
				if (GetOutput().dataType == Design::DataType::Null && _input[i].dataType == Design::DataType::Null)
				{
					_input[i] = _inputRegister[i];
					_inputRegister[i] = Design::Data();
				}
			}
		}
		virtual void SendOutput() {}
		virtual void SetOutput(Data data) { _output = data; }
		virtual Data GetOutput() { return _output; }
		virtual ModuleType GetModuleType() { return _type; }

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

		virtual void Compute(){  }

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
		int _busynessLevel; 
		int _busynessLevelMax;
		float _failProbability; // all transistors have a failure probability, hence the module failure, per clock. the more transistors the more failure chance.
		int _lithography;

		// actual frequency is controlled from outside, this is just a multiplier (that is checked for triggering compute at signal)
		int _frequency;
		int _numTransistors;
		int _thermalDissipationPower;

		// 0 index: top
		// 1 index: right
		// 2 index: bottom
		// 3 index: left
		std::vector<Design::Data> _input;
		std::vector<Design::Data> _inputRegister;

		// 1 internat output that can take any of 4 input paths
		Design::Data _output;

		std::vector<SkillRequirement> _skillRequirements;
		std::vector<StatRequirement> _statRequirements;


		// maximum 4 modules can be connected to a module. top, right, bottom, left
		std::vector<std::shared_ptr<Module>> _directConnectedModules;
		

		std::vector<DataType> _commandFilter; // takes only these kind of commands to work

		int _numCompletedOperations;

	};


}