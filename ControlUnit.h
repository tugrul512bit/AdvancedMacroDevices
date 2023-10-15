#pragma once
#include"Module.h"
#include"Tech.h"
#include"Skill.h"
#include<vector>
#include<queue>
#include<cmath>
#include<map>
#include<memory>
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
		ControlUnit(int frequency, int lithography, int parallelism) :
			Module(parallelism, parallelism, lithography,/*numTransistors*/ 1, ModuleType::CONTROL_UNIT,
				/* thermalDissipationPower */ 1, frequency, /* failProbability*/ 0.0f)
		{
			_resCtr = 0;			
			_roundRobinIncoming = 0; 

			// round-robin (1 = always shortest)
			_resourceSchedulingType = 0;
		}

		static std::shared_ptr<Module> Create(int frequency, int lithography, int parallelism)
		{
			return std::make_shared<ControlUnit>(frequency, lithography, parallelism);
		}

		// Cpu object calls this only when output is free	
		// also it does only single work (unless upgraded with a skill level)
		void Compute() override
		{
			SetIdle();
			for (int i = 0; i < _parallelism; i++)
			{
				if (GetOutput(i).dataType != Design::DataType::Null)
					continue;

				// list of resources found (resource module ID to use) & their bus paths
				std::map<int, std::map<int, bool>> validOutputResourceBus;

				
				bool computed = false;
				auto opcode = Data();
				_roundRobinIncoming++;
				for (int j = 0; j < 4; j++)
				{
					if (!computed)
					{
						opcode = _input[j][i];
						if (opcode.dataType != Design::DataType::Null)
						{
							if(_roundRobinIncoming % 3 == 0)
							if (opcode.dataType == Design::DataType::MicroOpAlu)
							{
								SetBusy();
								SetOutput(Design::Data(opcode.dataType, Design::ModuleType::ALU, -1 /* filled when output is sent*/, -1, Design::ModuleType::CONTROL_UNIT, _id),i);
								computed = true;
							}

							if (_roundRobinIncoming % 3 == 1)
							if (opcode.dataType == Design::DataType::MicroOpDecode)
							{
								SetBusy();
								SetOutput(Design::Data(opcode.dataType, Design::ModuleType::DECODER, -1 /* filled when output is sent*/, -1, Design::ModuleType::CONTROL_UNIT, _id),i);
								computed = true;
							}

							if (_roundRobinIncoming % 3 == 2)
							if (opcode.dataType == Design::DataType::Result)
							{
								
								if (opcode.value == Design::ModuleType::ALU)
								{
					
									SetOutput(Design::Data(Design::DataType::MicroOpAlu, Design::ModuleType::ALU, -1 /* filled when output is sent*/, -1, Design::ModuleType::CONTROL_UNIT, _id), i);
								}
								SetBusy();
								_numCompletedOperations++;
								// todo: merge current operation to architectural state
								computed = true;
							}
						}

					}

					if (!computed)
					{
						// if not computed, put the data back
						_input[j][i] = opcode;
					}
					else
					{
						// if computed, input is cleared for new data
						_input[j][i] = Data();
						break;
					}
				}
			}
		}

		void SendOutput() override
		{
			for (int i = 0; i < _parallelism; i++)
			{
				if (GetOutput(i).dataType != Design::DataType::Null)
				{
					// resource -> bus , jumps
					std::map<int, std::map<int, int>> validOutputResourceBus;
					// check bus connections for target module
					
					for (int j = 0; j < 4; j++)
					{
						auto dConn = _directConnectedModules[j];
						if (dConn.get())
						{
							if (dConn->GetModuleType() == Design::ModuleType::BUS)
							{
								auto alus = dConn->AsPtr<Design::Bus>()->GetFarConnectionsOfType(GetOutput(i).targetModuleType);
								for (auto& a : alus)
								{									
									validOutputResourceBus[a.moduleId][j] = a.jumps;									
								}
							}
						}
					}


					// Round-Robin
					if (_resourceSchedulingType == 0)
					{
						bool sent = false;
						int resCtr = 0;
						for (auto& resource : validOutputResourceBus)
						{
							if (!sent)
							{
								int busCtr = 0;

								if (resCtr == _resCtr)
								{

									for (auto& bus : resource.second)
									{

										if (!sent && busCtr++ == _busCtr[resource.first])
										{
											for (int channel = 0; channel < _directConnectedModules[bus.first]->GetParallelism(); channel++)
											{
												if (_directConnectedModules[bus.first]->GetInput(bus.first, /*i*/ channel).dataType == Design::DataType::Null)
												{
													sent = true;

													auto dataToSend = GetOutput(i);
													dataToSend.targetModuleId = resource.first;

													_directConnectedModules[bus.first]->SetInput(dataToSend, bus.first, /*i*/ channel);
													SetOutput(Data(), i);
													_busCtr[resource.first]++;

													if (_busCtr[resource.first] >= validOutputResourceBus[resource.first].size())
													{
														_busCtr[resource.first] = 0;
													}
													break;
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
					else if (_resourceSchedulingType == 1) // shortest path (both resource & bus)
					{
						int distance = 1000000;
						int selectedBus = -1;
						int selectedResource = -1;
						for (auto& resource : validOutputResourceBus)
						{
							for (auto& bus : resource.second)
							{
								if (distance > bus.second)
								{
									distance = bus.second;
									selectedBus = bus.first;
									selectedResource = resource.first;
								}
							}													
						}

						for (int channel = 0; channel < _directConnectedModules[selectedBus]->GetParallelism(); channel++)
						{
							if (_directConnectedModules[selectedBus]->GetInput(selectedBus, /*i*/ channel).dataType == Design::DataType::Null)
							{
								auto dataToSend = GetOutput(i);
								dataToSend.targetModuleId = selectedResource;
								_directConnectedModules[selectedBus]->SetInput(dataToSend, selectedBus, /*i*/ channel);
								SetOutput(Data(), i);
								break;
							}
						}
					}
				}
			}
		}

		void SetNearestResourceScheduling()
		{
			_resourceSchedulingType = 1;
		}

		void SetRoundRobinScheduling()
		{
			_resourceSchedulingType = 0;
		}
	private:
		// 0: Round-Robing scheduling for resources & bus connections
		int _resourceSchedulingType;
		// to select resource & bus fairly against deadlock
		int _resCtr;
		std::map<int,int> _busCtr;


		// to select an input fairly against starvation
		int _roundRobinIncoming;
		std::queue<Design::Data> _opcode;
	};
}