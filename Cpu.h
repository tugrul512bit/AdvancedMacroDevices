#pragma once

#include<vector>
#include"Module.h"
#include"Tech.h"
#include"Grid.h"
#include"Bus.h"
#include"Alu.h"
#include"ControlUnit.h"
namespace Design
{
	class Cpu
	{
	public:
		Cpu(int width = 4, int height = 4, int frequency = 1)
		{
			_moduleGrid.resize(((size_t)width)*height);
			_width = width;
			_height = height;
			_frequency = frequency;
		}

		void ConnectModules()
		{
			for (int j = 0; j < _height; j++)
			{
				for (int i = 0; i < _width; i++)
				{
					auto curModule = _moduleGrid[i + j * _width];
					if (curModule.get())
					{
						
						for (int y = -1; y <= 1; y ++)
						for (int x = -1; x <= 1; x ++)
						{							
							if((y == 0 && x!=0) || y!=0 && x==0)
							if (x + i >= 0 && x + i < _width && y + j >= 0 && y + j < _height)
							{
								int index = x + i + (y + j) * _width;
								auto neighborModule = _moduleGrid[index];
								if (neighborModule.get())
								{
									if (neighborModule->GetModuleType() == Design::ModuleType::BUS ||
										curModule->GetModuleType() == Design::ModuleType::BUS)
									{
										int idx = 0;
										if (x == -1)
											idx += 3;

										if (x == 1)
											idx += 1;

										if (y == -1)
											idx += 0;

										if (y == 1)
											idx += 2;
										curModule->Connect(neighborModule, idx);
										
									}
								}
							}
						}
					}
				}
			}
		}

		// builds connection tree of modules that are connected to buses
		void PrepareBusPaths()
		{

			for (int j = 0; j < _height; j++)
			{
				for (int i = 0; i < _width; i++)
				{
					auto curModule = _moduleGrid[i + j*_width];
					if (curModule.get())
					{
						if (curModule->GetModuleType() == ModuleType::BUS)
						{
							curModule->AsPtr<Design::Bus>()->ComputePaths();
							
						}

					}
				}
			}
			
		}

		void Compute()
		{
			for (int j = 0; j < _height; j++)
			{
				for (int i = 0; i < _width; i++)
				{
					// auto & curModule = _moduleGrid[i + j*_width];
					// curModule.ComputeOutput();
				}
			}

			for (int j = 0; j < _height; j++)
			{
				for (int i = 0; i < _width; i++)
				{
					// auto & curModule = _moduleGrid[i + j*_width];
					// auto output = curModule.GetOutput();
					// auto neighborList = GetNeighbors(i+j*_width);
					// for(auto & n:neighborList)
					// { n.AddInput(output); }
				}
			}
		}


		template<typename TypeModule>
		void SetCell(int col, int row, int frequency, int lithography)
		{ 
			_moduleGrid[col + row * _width] = (std::shared_ptr<Design::Module>) std::make_shared<TypeModule>(frequency,lithography); // copies
		}

		template<typename TypeModule>
		TypeModule* GetCell(int col, int row)
		{
			return _moduleGrid[col + row * _width].get()->AsPtr<TypeModule>();
		}

		std::shared_ptr<Window::AppStructure> GetGridView()
		{
			auto result = Window::GridItem::Create("cpu grid view", _height, _width, 1, false);
			for (int j = 0; j < _height; j++)
			{
				for (int i = 0; i < _width; i++)
				{
					if (_moduleGrid[i + j * _width].get())
					{				
						if (_moduleGrid[i + j * _width]->GetModuleType() == ModuleType::BUS)
						{
							
							result->AsPtr<Window::GridItem>()->SetCell(i, j, Window::ImageItem::Create(std::string("bus_img_") + std::to_string(i + j * _width), false, Window::Images::BusImage()));
						}
						else if (_moduleGrid[i + j * _width]->GetModuleType() == ModuleType::ALU)
						{

							result->AsPtr<Window::GridItem>()->SetCell(i, j, Window::ImageItem::Create(std::string("alu_img_") + std::to_string(i + j * _width), false, Window::Images::AluImage()));
						}
						else if (_moduleGrid[i + j * _width]->GetModuleType() == ModuleType::CONTROL_UNIT)
						{

							result->AsPtr<Window::GridItem>()->SetCell(i, j, Window::ImageItem::Create(std::string("control_unit_img_") + std::to_string(i + j * _width), false, Window::Images::ControlUnitImage()));
						}
					}
				}
			}
			return result;
		}

	private:
		int _width;
		int _height;
		int _frequency;
		std::vector<std::shared_ptr<Module>> _moduleGrid;
	};
}