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
		Cpu(int width = 4, int height = 4)
		{
			_moduleGrid.resize(((size_t)width)*height);
			_width = width;
			_height = height;
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