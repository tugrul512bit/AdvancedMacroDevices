#pragma once
namespace Design
{
	class Tech
	{
	public:
	private:
	};

	// required for dispatch-unit (reorder buffer)
	class OutOfOrderExecution :public Tech
	{

	};

	// required for branch-predictor 
	class SpeculativeExecution :public Tech
	{

	};

	// required for SIMD 2x,4x,8x,...
	class DataParallelism : public Tech
	{

	};

	// required for out-of-order-execution, speculative-execution, VLIW
	class InstructionLevelParallelism : public Tech
	{

	};

	// required for speculative-execution, multiple-threads per core, virtualization
	class Context : public Tech
	{

	};
}