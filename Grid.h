#pragma once

#include "AppStructure.h"
#include<functional>
#include<vector>
#include<string>
namespace Window
{
	template<int COLS=2,int ROWS=2>
	class GridItem : public AppStructure
	{
	public:
		static std::shared_ptr<AppStructure> Create(std::string name, int fontScale, bool sameLine)
		{
			std::shared_ptr<AppStructure> node = std::shared_ptr<GridItem>(new GridItem(name, sameLine, fontScale), [](GridItem* g) { delete g; });
			return node;
		}

		GridItem(std::string name = "text", bool sameLine = false, float fontScale = 1.0f)
		{
			_name = name;
			_sameLine = sameLine;
			_fontScale = fontScale;
		}

		void Compute() override
		{

		}

		void PreRender() override
		{
			ImGui::SetWindowFontScale(_fontScale);
			for (int j = 0; j < _rows; j++)
			{
				ImGui::BeginGroup();
				for (int j = 0; j < _columns; j++)
				{
					ImGui::SameLine();
					ImGui::BeginGroup();
					_cells[j][i]->PreRender();
					_cells[j][i]->PostRender();
					ImGui::EndGroup();
				}
			}
		}

		void PostRender() override
		{
			ImGui::SetWindowFontScale(1.0f);
		}
	private:
		float _fontScale;
		std::shared_ptr<AppStructure> _cells[ROWS][COLS];
	};
}