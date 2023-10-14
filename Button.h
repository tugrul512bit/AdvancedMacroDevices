#pragma once
#include "AppStructure.h"
#include<functional>
namespace Window
{
	class ButtonItem : public AppStructure
	{
	public:
		static std::shared_ptr<AppStructure> Create(std::string name, std::string content,std::function<void(void)> onClick, bool sameLine, std::function<bool(void)> isDisabled)
		{
			std::shared_ptr<AppStructure> node = std::shared_ptr<ButtonItem>(new ButtonItem(name,content,onClick,sameLine,isDisabled), [](ButtonItem* b) { delete b; });						
			return node;
		}

		ButtonItem(std::string name = "button", std::string content = "this is a button", std::function<void(void)> onClick = []() {}, bool sameLine = false, std::function<bool(void)> isDisabled = []() {return false; })
		{ 
			_name = name; 
			_content = content; 
			_onClick = onClick;
			_sameLine = sameLine;
			_isDisabled = isDisabled;		
			_defaultType = ImGuiCol_Button;			
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
			
			if (ImGui::Button(_content.c_str()))
			{
				_onClick();
			}
		}

		void PostRender() override
		{

		}
	private:
		std::string _content;
		std::function<void(void)> _onClick;
		std::function<bool(void)> _isDisabled;
	};
}