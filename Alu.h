#pragma once
#include"Module.h"
#include"Tech.h"
#include"Skill.h"
#include<vector>
#include<queue>
#include<cmath>

namespace Design
{
	// cache bank stores cached data
	// input: memory request
	// output: data	
	class Alu :public Module
	{
	public:
		Alu(int frequency, int lithography)
		{
			_frequency = frequency;
			_lithography = lithography;
			_type = ModuleType::ALU;
		}

		void SetBusy() override { _isBusy = true; }
		void Compute() override
		{
			
			for (int i = 0; i < 4; i++)
			{
				bool workDone = false;
				bool breakLoop = false;
				auto inp = _input[i];
				if (inp.dataType != Design::DataType::Null)
				{
					if (inp.dataType == Design::DataType::MicroOpAlu)
					{
						workDone = true;
						std::cout << "alu dummy compute" << std::endl;
						int idSource = inp.sourceModuleId;
						
						for (int j = 0; j < 4; j++)
						{
							
							auto bus = _directConnectedModules[j];
							if (bus.get())
							{
								if (bus->GetModuleType() == Design::ModuleType::BUS)
								{
									std::cout << "output bus found" << std::endl;
									auto sources = bus->AsPtr<Design::Bus>()->GetFarConnectionsOfType(inp.sourceModuleType);
									for (auto & s : sources)
									{
										std::cout << "!   "<< s.moduleId <<"      "<< idSource << std::endl;
										// send result back to source
										if (s.moduleId == idSource)
										{
											std::cout << "source path found" << std::endl;
											int idx = 0;
											if (j == 0)
												idx += 2;
											if (j == 1)
												idx += 3;
											if (j == 2)
												idx += 0;
											if (j == 3)
												idx += 1;

											// todo: do not set input if input is full (same for all modules)
											bus->AsPtr<Design::Bus>()->SetInput(
												Data(
													Design::DataType::Result,
													Design::CONTROL_UNIT,
													idSource,
													-1,
													Design::ModuleType::ALU,
													_id), 
												idx);
											breakLoop=true;
											break;
										}
									}
								}
							}

							if (breakLoop)
								break;
						}

					}
				}

				if (breakLoop)
					break;
			}
		}
	private:
	};
}