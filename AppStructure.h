#pragma once
#include<string>
#include<vector>
#include<memory>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
namespace Window
{
	enum ItemType
	{
		WINDOW,
		BUTTON,
		DROPDOWNLIST,
		SLIDER,
		TEXT
	};

	class AppStructure
	{
	public:
		AppStructure(ItemType type = ItemType::WINDOW, std::string name = "item")
		{
			_type = type;
			_name = name;
		}

		virtual void PreRender() {}
		virtual void PostRender() {}

		void Render()
		{
			PreRender();

			for (auto node : _childNodes)
			{
				node->Render();
			}

			PostRender();
		}

		void AddNode(std::shared_ptr<AppStructure> node)
		{
			_childNodes.push_back(node);
		}


	protected:
		ItemType _type;
		std::string _name;
		std::vector<std::shared_ptr<AppStructure>> _childNodes;
	};

}