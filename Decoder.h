#pragma once
#include"Module.h"
#include"Tech.h"
#include"Skill.h"
#include"Bus.h"
#include<vector>
#include<queue>
#include<cmath>
#include<iostream>

namespace Design
{
	// cache bank stores cached data
	// input: memory request
	// output: data	
	class Decoder :public Module
	{
	public:
		Decoder(int frequency, int lithography)
		{
			_frequency = frequency;
			_lithography = lithography;
			_type = ModuleType::DECODER;
		}


		void Compute() override
		{
			SetIdle();
			if (GetOutput().dataType != Design::DataType::Null)
				return;

			bool computed = false;
			auto inp = Data();
			for (int i = 0; i < 4; i++)
			{
				if (!computed)
				{

					inp = _input[i];
					if (inp.dataType != Design::DataType::Null)
					{

						if (inp.dataType == Design::DataType::MicroOpDecode)
						{
							computed = true;
							std::cout << "decode dummy compute" << std::endl;
							SetBusy();
							SetOutput(Data(
								Design::DataType::Result,
								Design::CONTROL_UNIT,
								inp.sourceModuleId,
								Design::ModuleType::ALU,
								Design::ModuleType::DECODER,
								_id));

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

		void SendOutput() override
		{
			if (GetOutput().dataType != Design::DataType::Null)
			{

				int idSource = GetOutput().targetModuleId;
				bool sent = false;
				for (int j = 0; j < 4; j++)
				{
					if (!sent)
					{
						auto bus = _directConnectedModules[j];
						if (bus.get())
						{
							if (bus->GetModuleType() == Design::ModuleType::BUS)
							{
								std::cout << "output bus found" << std::endl;
								auto sources = bus->AsPtr<Design::Bus>()->GetFarConnectionsOfType(GetOutput().targetModuleType);
								for (auto& s : sources)
								{
									if (!sent)
									{

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
													GetOutput(),
													idx);
												SetOutput(Data());
												sent = true;
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
	private:
	};
}