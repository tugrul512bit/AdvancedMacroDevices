#pragma once

#include "AppStructure.h"
#include"Image.h"
#include<functional>
#include<vector>
#include<string>
namespace Window
{

	class GridItem : public AppStructure
	{
	public:
		static std::shared_ptr<AppStructure> Create(std::string name,int rows, int cols, int fontScale, bool sameLine)
		{
			std::shared_ptr<AppStructure> node = std::shared_ptr<GridItem>(new GridItem(name, rows, cols, sameLine, fontScale), [](GridItem* g) { delete g; });
			return node;
		}

		GridItem(std::string name = "text", int rows=2, int cols=2, bool sameLine = false, float fontScale = 1.0f)
		{
			_name = name;
			_sameLine = sameLine;
			_fontScale = fontScale;
			_rows = rows;
			_cols = cols;
			for (int j = 0; j < _rows; j++)
			{
				for (int i = 0; i < _cols; i++)
				{
					_cells.push_back(ImageItem::Create(std::string("grid_cell_") + std::to_string(i + j * _cols), false, Images::EmptyGridCellImage(),false));
				}
			}
		}

		void SetCell(int col, int row, std::shared_ptr<AppStructure> structure)
		{
			_cells[row*_cols + col] = structure;
		}

		void Compute() override
		{

		}

		void PreRender() override
		{
			ImGui::SetWindowFontScale(_fontScale);
			ImGui::BeginGroup();
			for (int j = 0; j < _rows; j++)
			{
				
				for (int i = 0; i < _cols; i++)
				{
					if(i>0)
						ImGui::SameLine();
				
					if (_cells[j*_cols + i].get() != nullptr)
					{
						_cells[j * _cols + i]->PreRender();
						_cells[j * _cols + i]->PostRender();
					}
					
				}
				
			}
			ImGui::EndGroup();
		}

		void PostRender() override
		{
			ImGui::SetWindowFontScale(1.0f);
		}
	private:
		float _fontScale;
		int _rows;
		int _cols;
		std::vector<std::shared_ptr<AppStructure>> _cells;
	};
}