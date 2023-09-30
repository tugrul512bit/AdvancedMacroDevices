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

	class AppWindow : public AppStructure
	{
	public:
		static std::shared_ptr<AppStructure> Create(std::string name)
		{
			std::shared_ptr<AppStructure> node = std::shared_ptr<AppWindow>(new AppWindow(name), [](AppWindow* w) { delete w; });
			return node;
		}

		AppWindow(std::string name = "item") { _name = name; }

		void PreRender() override
		{
			ImGui::Begin(_name.c_str());
		}

		void PostRender() override
		{
			ImGui::End();
		}
	};

	class TextItem : public AppStructure
	{
	public:
		static std::shared_ptr<AppStructure> Create(std::string name,std::string content)
		{
			std::shared_ptr<AppStructure> node = std::shared_ptr<TextItem>(new TextItem(name), [](TextItem* t) { delete t; });
			((TextItem*)node.get())->_content = content;
			return node;
		}

		TextItem(std::string name = "text", std::string content = "this is a text") { _name = name; _content = content; }

		void PreRender() override
		{
			ImGui::Text(_content.c_str());
		}

		void PostRender() override
		{

		}
	private:
		std::string _content;
	};

}