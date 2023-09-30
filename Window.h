#pragma once
#include "AppStructure.h"
namespace Window
{
	class AppWindow : public AppStructure
	{
	public:
		static std::shared_ptr<AppStructure> Create(std::string name)
		{
			std::shared_ptr<AppStructure> node = std::shared_ptr<AppWindow>(new AppWindow(name), [](AppWindow* w) { delete w; });
			return node;
		}

		AppWindow(std::string name = "item") { _name = name; }

		void Compute() override
		{

		}

		void PreRender() override
		{
			ImGui::Begin(_name.c_str());
		}

		void PostRender() override
		{
			ImGui::End();
		}
	};

}