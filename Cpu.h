#pragma once

#include<vector>
#include"Module.h"
#include"Tech.h"

namespace Design
{
	class Cpu
	{
	public:
		Cpu(int width = 4, int height = 4)
		{
			_moduleGrid.resize(width*height);
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
					// auto output = curModule.ComputeOutput();
					// auto neighbors = GetAdjacentModules(i + j*_width);
					/*
					for (auto& n : neighbors)
					{
						n.Input(output);
						auto out = n.ComputeOutput();
						curModule.Input(out);
					}
					*/
				}
			}
		}
	private:
		int _width;
		int _height;
		std::vector<Module> _moduleGrid;
	};
}