#pragma once
#include"Module.h"
#include"Tech.h"
#include"Skill.h"
#include<vector>
#include<queue>
#include<cmath>

namespace Design
{
	// opens an independent context (registers, cache, etc) and starts computing as if that branch is taken
	// if branch is not taken, it discards whole context (releases resources)
	// if branch is taken, it converts context to the current context in 1-k cycles
	class BranchPredictor :public Module
	{
	public:
		// capacity: number of branches to remember
		// branchPolicy: 0=by last branch, 1=by average of N branches, 2=by polynomial fit
		BranchPredictor(int frequency, int lithography, int capacity, int branchPolicy)
		{
			_capacity = capacity;
			_lithography = lithography;	
			
		}

		void Compute() override
		{

		}
	private:

		int _capacity;
		int _lithography;
	};
}