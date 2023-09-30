#pragma once
#include "AppStructure.h"
#include<functional>
#include<vector>
#include<string>
namespace Window
{
	class ListItem : public AppStructure
	{
	public:
		static std::shared_ptr<AppStructure> Create(std::string name, int fontScale, bool sameLine)
		{
			std::shared_ptr<AppStructure> node = std::shared_ptr<ListItem>(new ListItem(name,  sameLine, fontScale), [](ListItem* l) { delete l; });
			return node;
		}

		ListItem(std::string name = "text", bool sameLine = false, float fontScale = 1.0f)
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
			ImGui::BeginGroup();			
		}

		void PostRender() override
		{
			ImGui::EndGroup();
			ImGui::SetWindowFontScale(1.0f);
		}
	private:		
		float _fontScale;
	};
}