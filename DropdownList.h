#pragma once
#include "AppStructure.h"
#include<functional>
namespace Window
{
	class DropdownListItem : public AppStructure
	{
	public:
		static std::shared_ptr<AppStructure> Create(std::string name, std::string content, std::function<void(void)> onClick, bool sameLine, std::function<bool(void)> isDisabled)
		{
			std::shared_ptr<AppStructure> node = std::shared_ptr<DropdownListItem>(new DropdownListItem(name, content, onClick, sameLine, isDisabled), [](DropdownListItem* b) { delete b; });
			return node;
		}

		DropdownListItem(std::string name = "dropdown list", std::string content = "this is a dropdown list", std::function<void(void)> onClick = []() {}, bool sameLine = false, std::function<bool(void)> isDisabled = []() {return false; })
		{
			_name = name;
			_content = content;
			_onClick = onClick;
			_sameLine = sameLine;
			_isDisabled = isDisabled;
			_defaultType = ImGuiCol_DragDropTarget;
		}

		void SetDefaultColor(ImVec4 defaultColor)
		{
			_changeDefaultColor = true;
			_defaultColor = defaultColor;
		}

		void Compute() override
		{
			_enabled = !_isDisabled();
		}

		void PreRender() override
		{

			if (ImGui::BeginListBox(_content.c_str()))
			{
				_onClick();
			}
		}

		void PostRender() override
		{
			ImGui::EndListBox();
		}
	private:
		std::string _content;
		std::function<void(void)> _onClick;
		std::function<bool(void)> _isDisabled;
	};
}