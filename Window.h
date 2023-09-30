#pragma once
#include "AppStructure.h"
namespace Window
{
	class AppWindow : public AppStructure
	{
	public:
		static std::shared_ptr<AppStructure> Create(std::string name, std::string title)
		{
			std::shared_ptr<AppStructure> node = std::shared_ptr<AppWindow>(new AppWindow(name,title), [](AppWindow* w) { delete w; });
			return node;
		}

		AppWindow(std::string name = "item", std::string title = "title") { _name = name; _title = title; }

		void Compute() override
		{

		}

		void PreRender() override
		{
			ImGui::Begin(_title.c_str());
		}

		void PostRender() override
		{
			ImGui::End();
		}
	private:
		std::string _title;
	};

}