#pragma once
#include"Skill.h"
#include<vector>
#include<queue>
#include<memory>

namespace Design
{
	/* todo: make data transmission priority - based, for evading deadlocks.oldest context / id must go first
	*  for all modules i/o
	*  example:
	*	for(all awaiting outputs to send)
	*		pick oldest first
	*/

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

		MicroOpCacheBankReadTag,
		MicroOpCacheBankWriteDataToTag,
		MicroOpCacheBankInvalidateTag,

		// result (which has a target module type or id to go)
		Result
	};

	enum FpuInstructionType
	{
		Add,
		Mul,
		Sub,
		Div,
		Sqrt,
		Exp,
		Pow,
		FMA
	};

	enum ModuleType
	{
		BUS,
		CONTROL_UNIT,
		CACHE_CONTROL,
		CACHE_CONTROL_CACHE,	// cache can be cached (by a closer & smaller cache)
		CACHE_CONTROL_CONTROL,	// context data (thread variables, register states, etc) can be cached
		CACHE_CONTROL_RAM,		// data can be cached
		CACHE_CONTROL_DECODER, // instructions (between RAM and DECODER) can be cached
		CACHE_CONTROL_ALU, // there's no harm in caching integer operations if ALU is as slow as 100 cycles per integer-division
		CACHE_CONTROL_FPU, // even fp operations can be cached (i.e. 16 bit fp power/exp/log/etc --> 128kB cache required)
		MEMORY_CONTROL,
		ALU,
		FPU,
		FPU_CLOSEST,
		FPU_WIDEST,
		FPU_WIDTH2,
		FPU_WIDTH4,
		FPU_WIDTH8,
		FPU_WIDTH16,
		FPU_WIDTH32,
		FPU_WIDTH64,
		FPU_WIDTH128,
		FPU_WIDTH256,
		FPU_SPECIAL_FUNCTION,	// any fpu that can compute square root / exponential / power / logarithm / erf / sin / cos / etc
		FPU_SQRT,				// any fpu that has square-root capability
		FPU_DIV,
		FPU_MUL,
		FPU_ADD,				// also works as FPU_SUB. just negate an input to do subtraction
		FPU_EXP,
		FPU_RND,				// FPU with random number generation
		CACHE_BANK,
		BRANCH_PREDICTOR,
		DECODER,
		// for searching modules through bus connections
		ANY,
		ANY_MEMORY_READ_CAPABLE,
		ANY_MEMORY_WRITE_CAPABLE,
		ANY_COMPUTE_CAPABLE,
		ANY_WITH_EMPTY_SLOT,
		ANY_EMPTY,
		ANY_EMPTY_ROUND_ROBIN_BUS_PATH,
		ANY_EMPTY_ROUND_ROBIN_RESOURCE
	};

	// the data that is passing between modules
	class Data
	{
	public:
		Data(DataType dataTypePrm = DataType::Null, ModuleType targetModuleTypePrm = ModuleType::ANY,
			int targetModuleIdPrm = -1, int valuePrm = -1, ModuleType sourceModuleTypePrm = ModuleType::ANY,
			int sourceModuleIdValue = -1, int clockCycleId=-1)
		{
			_sourceModuleType = sourceModuleTypePrm;
			_sourceModuleId = sourceModuleIdValue;
			_targetModuleType = targetModuleTypePrm;
			_targetModuleId = targetModuleIdPrm;
			_dataType = dataTypePrm;
			_value = valuePrm;

			_context = -1;
			_contextType = -1;
			_localId = -1;
			_clockCycleId = clockCycleId;
		}

		int GetLocalId()
		{
			return _localId;
		}

		void SetLocalId(int newLocalId)
		{
			_localId = newLocalId;
		}

		int GetClockCycleId()
		{
			return _clockCycleId;
		}

		DataType GetDataType()
		{
			return _dataType;
		}

		void SetTargetModuleId(int newTargetModuleId)
		{
			_targetModuleId = newTargetModuleId;
		}

		int GetValue()
		{
			return _value;
		}

		void SetDataType(DataType dType)
		{
			_dataType = dType;
		}

		ModuleType GetTargetModuleType()
		{
			return _targetModuleType;
		}

		int GetTargetModuleId()
		{
			return _targetModuleId;
		}

		int GetSourceModuleId()
		{
			return _sourceModuleId;
		}
	private:
		// how many cycles passed since creation of this data
		int _clockCycleId;

		// the module that has sent this data
		ModuleType _sourceModuleType;
		int _sourceModuleId;

		// type of module to take this data
		// if input queue of module is full, it goes to another module
		ModuleType _targetModuleType;
		int _targetModuleId;
		DataType _dataType;

		// whose context is this: id of instruction or branch prediction or thread
		int _context;

		// 0: instruction, 1: branch, 2: thread
		int _contextType;

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
		int _value; 	

		// every module assigns a local id to its currently stored data objects for detection/tracking of deadlock
		int _localId;
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
		Module(int parallelism,int busynessLevelMax, int lithography, 
			int numTransistors, ModuleType moduleType,int thermalDissipationPower,
			int frequency,float failProbability)
		{		
			// derived classes such as ALU have to change this value accordingly in constructor
			_parallelism = parallelism;

			_id = GetUniqueId();			
			for (int j = 0; j < 4; j++)
			{
				_input[j].resize(_parallelism);
				for (int i = 0; i < _parallelism; i++)
					_input[j][i] = Data();
			}
			for (int j = 0; j < 4; j++)
			{
				_inputRegister[j].resize(_parallelism);
				for (int i = 0; i < _parallelism; i++)
					_inputRegister[j][i] = Data();
			}
			_output.resize(_parallelism);
			for(int i=0;i< _parallelism;i++)
				_output[i] = Data();

			_directConnectedModules.resize(4);
			for (int i = 0; i < 4; i++)
				_directConnectedModules[i] = nullptr;
			_busynessLevel = 0;
			_busynessLevelMax = busynessLevelMax;
			_failProbability = failProbability;
			_lithography = lithography;
			_type = moduleType; // just a default
			_numTransistors = numTransistors;
			_thermalDissipationPower = thermalDissipationPower;
			_frequency = frequency; // 1 means equal frequency to outer source. 2 means 2x frequency or 2 iterations per cycle
			_numCompletedOperations = 0;
			_numStartedOperations = 0;
		}
		int GetCompletedOperationCount() { return _numCompletedOperations; }
		virtual void SetBusy() { _busynessLevel++; }
		virtual void SetIdle() { _busynessLevel = 0; }
		// percentage level: 100 = full, 0 = idle
		virtual int GetBusyness() { return 100 * (_busynessLevel / (float)_busynessLevelMax); }

		int GetId() { return _id; }

		virtual void ComputeOutput() {}

		virtual Data GetInput(int index, int channel)
		{
			return _inputRegister[index][channel];
		}
		virtual void SetInput(Data input, int index, int channel) 
		{
			// when data leaves a module, localId is reset
			input.SetLocalId(-1);
			_inputRegister[index][channel] = input;
		}
		virtual void ApplyInput()
		{
			for (int j = 0; j < 4; j++)
			{
				for (int i = 0; i < _parallelism; i++)
				{
					// if output & input register is empty, then its ok to compute later				
					// because if output is clogged, there is no reason to compute something that won't go anywhere
					if (GetOutput(i).GetDataType() == Design::DataType::Null && _input[j][i].GetDataType() == Design::DataType::Null)
					{
						_input[j][i] = _inputRegister[j][i];
						_inputRegister[j][i] = Design::Data();
					}
				}
			}
		}
		virtual void SendOutput() {}
		virtual void SetOutput(Data data, int channel) { _output[channel] = data; }
		virtual Data GetOutput(int channel) { return _output[channel]; }
		virtual ModuleType GetModuleType() { return _type; }
		virtual bool CheckDeadlock() { return false; }
		virtual int GetParallelism() { return _parallelism; }
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

		// clock cycle id is used for giving instructions and opcodes a relative age value for priority checks
		virtual void Compute(int clockCycleId){  }

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

		// N inputs per direction, N operations per cycle
		int _parallelism;

		// array index
		// 0 index: top
		// 1 index: right
		// 2 index: bottom
		// 3 index: left
		// vector index: channels
		std::vector<Design::Data> _input[4];
		std::vector<Design::Data> _inputRegister[4];

		// N internal outputs that can take any of 4 input paths
		std::vector<Design::Data> _output;

		std::vector<SkillRequirement> _skillRequirements;
		std::vector<StatRequirement> _statRequirements;


		// maximum 4 modules can be connected to a module. top, right, bottom, left
		std::vector<std::shared_ptr<Module>> _directConnectedModules;
		

		std::vector<DataType> _commandFilter; // takes only these kind of commands to work

		int _numCompletedOperations;
		int _numStartedOperations;
	};


}