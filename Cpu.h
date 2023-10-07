#pragma once

#include<vector>
#include"Module.h"
#include"Tech.h"
#include"Grid.h"
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


		void SetCell(int col, int row, Module module)
		{ 
			_moduleGrid[col + row * _width] = module; // copies
		}

		std::shared_ptr<Window::AppStructure> GetGridView()
		{
			auto result = Window::GridItem::Create("cpu grid view", _height, _width, 1, false);
			for (int j = 0; j < _height; j++)
			{
				for (int i = 0; i < _width; i++)
				{
					if (_moduleGrid[i + j * _width].GetModuleType() == ModuleType::BUS)
					{
						result->AsPtr<Window::GridItem>()->SetCell(i, j, Window::ImageItem::Create(std::string("bus_img_") + std::to_string(i + j * _width), false, Window::Images::BusImage()));
					}
				}
			}
			return result;
		}

	private:
		int _width;
		int _height;
		std::vector<Module> _moduleGrid;
	};
}