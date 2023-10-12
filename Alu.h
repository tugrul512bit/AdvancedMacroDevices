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


		void Compute() override
		{
			bool computed = false;
			auto inp = Data();
			for (int i = 0; i < 4; i++)
			{
				if (!computed)
				{
					bool breakLoop = false;
					inp = _input[i];
					if (inp.dataType != Design::DataType::Null)
					{
						if (inp.dataType == Design::DataType::MicroOpAlu)
						{
							
							std::cout << "alu dummy compute" << std::endl;
							int idSource = inp.sourceModuleId;

							for (int j = 0; j < 4; j++)
							{
								if (!computed)
								{
									auto bus = _directConnectedModules[j];
									if (bus.get())
									{
										if (bus->GetModuleType() == Design::ModuleType::BUS)
										{
											std::cout << "output bus found" << std::endl;
											auto sources = bus->AsPtr<Design::Bus>()->GetFarConnectionsOfType(inp.sourceModuleType);
											for (auto& s : sources)
											{
												if (!computed)
												{
													std::cout << "!   " << s.moduleId << "      " << idSource << std::endl;
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

														if (bus->AsPtr<Design::Bus>()->GetInput(idx).dataType == Design::DataType::Null)
														{
															bus->AsPtr<Design::Bus>()->SetInput(
																Data(
																	Design::DataType::Result,
																	Design::CONTROL_UNIT,
																	idSource,
																	-1,
																	Design::ModuleType::ALU,
																	_id),
																idx);
															computed = true;
														}
													}
												}
											}
										}
									}
								}
							
							}

						}
					}

				}

				if (!computed)
				{
					// if not computed, put the data back
					_input[i] = inp;
				}
				else
				{
					// if computed, input is cleared for new data
					_input[i] = Data();
				}
				
			}

		}
	private:
	};
}