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
			_visible = true;
			_enabled = true;
			_sameLine = false;
		}

		virtual void PreRender() {}
		virtual void PostRender() {}

		virtual void Compute() 
		{
		
		}

		void Calculate()
		{
			for (auto node : _childNodes)
			{
				node->Compute();
			}
			Compute();
		}

		void Render()
		{			
			if (_visible)
			{
				if (_sameLine)
					ImGui::SameLine();
				if (!_enabled)
					ImGui::BeginDisabled();

				PreRender();

				for (auto node : _childNodes)
				{
					node->Render();
				}
				
				PostRender();

				if (!_enabled)
					ImGui::EndDisabled();
			}
		}

		void AddNode(std::shared_ptr<AppStructure> node)
		{
			_childNodes.push_back(node);
		}

		void Enable()
		{
			_enabled = true;
		}
		void Disable()
		{
			_enabled = false;
		}

		void Hide()
		{
			_visible = false;
		}

		void Show()
		{
			_visible = true;
		}

	protected:
		ItemType _type;
		std::string _name;
		int _renderWidth;
		int _renderHeight;
		bool _visible;
		bool _enabled;
		bool _sameLine;
		std::vector<std::shared_ptr<AppStructure>> _childNodes;
	};

}