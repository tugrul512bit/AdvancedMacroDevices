#pragma once
#include"Module.h"
#include"Tech.h"
#include"Skill.h"
#include"Bus.h"
#include<vector>
#include<queue>
#include<cmath>
#include<memory>

namespace Design
{
	// arithmetic logic unit
	// input: computational opcode
	// input: data from memory/cache/register
	// output: memory/cache/register request
	// output: result
	class Alu :public Module
	{
	public:
		Alu(int frequency, int lithography,int parallelism):
			Module(parallelism, parallelism,lithography,/*numTransistors*/ 1, ModuleType::ALU, 
				/* thermalDissipationPower */ 1, frequency, /* failProbability*/ 0.0f)
		{

		}

		static std::shared_ptr<Module> Create(int frequency, int lithography, int parallelism)
		{
			return std::make_shared<Alu>(frequency, lithography, parallelism);
		}

		void Compute() override
		{
			SetIdle();
			
			for (int i = 0; i < _parallelism; i++)
			{
				if (GetOutput(i).dataType != Design::DataType::Null)
					continue;

				bool computed = false;
				auto inp = Data();
				for (int j = 0; j < 4; j++)
				{
					if (!computed)
					{

						inp = _input[j][i];
						if (inp.dataType != Design::DataType::Null)
						{

							if (inp.dataType == Design::DataType::MicroOpAlu)
							{
								computed = true;
								std::cout << "alu dummy compute" << std::endl;
								SetBusy();
								SetOutput(Data(
									Design::DataType::Result,
									Design::CONTROL_UNIT,
									inp.sourceModuleId,
									-1,
									Design::ModuleType::ALU,
									_id),i);

							}
						}

					}

					if (!computed)
					{
						// if not computed, put the data back
						_input[j][i] = inp;
					}
					else
					{
						// if computed, input is cleared for new data
						_input[j][i] = Data();
					}

				}
			}
		}

		void SendOutput() override
		{
			for (int i = 0; i < _parallelism; i++)
			if (GetOutput(i).dataType != Design::DataType::Null)
			{

				int idSource = GetOutput(i).targetModuleId;
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
								auto sources = bus->AsPtr<Design::Bus>()->GetFarConnectionsOfType(GetOutput(i).targetModuleType);
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

											if (bus->AsPtr<Design::Bus>()->GetInput(idx,i).dataType == Design::DataType::Null)
											{
												bus->AsPtr<Design::Bus>()->SetInput(
													GetOutput(i),
													idx,i);
												SetOutput(Data(),i);
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