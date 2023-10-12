#pragma once
#include"Module.h"
#include"Tech.h"
#include"Skill.h"
#include<vector>
#include<queue>
#include<cmath>
#include<map>
namespace Design
{

	// controls other modules
	// uses several algorithms to evade dead-lock	
	// 1) Banker's Algorithm to limit the resource usage per instruction such as ALU,FPU,RAM
	// 2) Round-Robin scheduling (ordering on resources) to add fairness on both resource usage and data-path usage
	// 3) Re-routing of bus data when a resource input is clogged for a long time (this is implicit to hide some realism for more fun)
	// players can still manage to get a dead-lock buy combining multiple control units and shared resources
	class ControlUnit :public Module
	{
	public:
		ControlUnit(int frequency, int lithography)
		{
			_frequency = frequency;
			_lithography = lithography;
			_type = ModuleType::CONTROL_UNIT;
			_resCtr = 0;			
			_roundRobinIncoming = 0; 
		}

		// Cpu object calls this only when output is free	
		// also it does only single work (unless upgraded with a skill level)
		void Compute() override
		{
			// list of resources found (resource module ID to use) & their bus paths
			std::map<int,std::map<int,bool>> validOutputResourceBus;
			bool workStart = false;
			bool workEnd = false;

			bool computed = false;
			auto opcode = Data();
			for (int i = 0; i < 4; i++)
			{
				if (!computed)
				{
					opcode = _input[i];					
					if (opcode.dataType != Design::DataType::Null)
					{
						if (opcode.dataType == Design::DataType::MicroOpAlu)
						{
							workStart = true;
							// check bus connections for an ALU
							for (int j = 0; j < 4; j++)
							{
								auto dConn = _directConnectedModules[j];
								if (dConn.get())
								{
									if (dConn->GetModuleType() == Design::ModuleType::BUS)
									{
										auto alus = dConn->AsPtr<Design::Bus>()->GetFarConnectionsOfType(Design::ModuleType::ALU);
										for (auto& a : alus)
										{
											validOutputResourceBus[a.moduleId][j] = true;
										}
									}
								}
							}
						}

						if (opcode.dataType == Design::DataType::Result)
						{
							_numCompletedOperations++; 
							workEnd = true;
						}

						if (workEnd)
						{
							// do something to retire an instruction/opcode
							computed = true;
						}

						int resCtr = 0;

						if (workStart)
						{

							for (auto& resource : validOutputResourceBus)
							{
								if (!computed)
								{
									int busCtr = 0;

									if (resCtr == _resCtr)
									{

										for (auto& bus : resource.second)
										{

											if (!computed && busCtr++ == _busCtr[resource.first])
											{

												if (_directConnectedModules[bus.first]->GetInput(bus.first).dataType == Design::DataType::Null)
												{
													computed = true;
													_directConnectedModules[bus.first]->SetInput(Design::Data(opcode.dataType, Design::ModuleType::ALU, resource.first, -1, Design::ModuleType::CONTROL_UNIT, _id), bus.first);
													_busCtr[resource.first]++;
													if (_busCtr[resource.first] >= validOutputResourceBus[resource.first].size())
													{
														_busCtr[resource.first] = 0;
													}
												}

											}
										}

										_resCtr++;
										if (_resCtr >= validOutputResourceBus.size())
										{
											_resCtr = 0;
										}

									}

									resCtr++;
								}
							}

						}
					}

				
				}

				if (!computed)
				{
					// if not computed, put the data back
					_input[i] = opcode;
				}
				else
				{
					// if computed, input is cleared for new data
					_input[i] = Data();
				}
			}
									
		}
	private:
		// to select resource & bus fairly against deadlock
		int _resCtr;
		std::map<int,int> _busCtr;


		// to select an input fairly against starvation
		int _roundRobinIncoming;
		std::queue<Design::Data> _opcode;
	};
}