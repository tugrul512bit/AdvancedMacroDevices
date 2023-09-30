#include "AppStructure.h"
#include<functional>
namespace Window
{
	class ButtonItem : public AppStructure
	{
	public:
		static std::shared_ptr<AppStructure> Create(std::string name, std::string content,std::function<void(void)> onClick, bool sameLine)
		{
			std::shared_ptr<AppStructure> node = std::shared_ptr<ButtonItem>(new ButtonItem(name,content,onClick,sameLine), [](ButtonItem* b) { delete b; });						
			return node;
		}

		ButtonItem(std::string name = "button", std::string content = "this is a button", std::function<void(void)> onClick = []() {}, bool sameLine=false)
		{ 
			_name = name; 
			_content = content; 
			_onClick = onClick;
			_sameLine = sameLine;
		}

		void Compute() override
		{

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
	};
}