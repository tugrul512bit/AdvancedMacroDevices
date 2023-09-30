#include "AppStructure.h"
#include<functional>
namespace Window
{
	class ButtonItem : public AppStructure
	{
	public:
		static std::shared_ptr<AppStructure> Create(std::string name, std::string content,std::function<void(void)> onClick)
		{
			std::shared_ptr<AppStructure> node = std::shared_ptr<ButtonItem>(new ButtonItem(name), [](ButtonItem* b) { delete b; });
			((ButtonItem*)node.get())->_content = content;
			((ButtonItem*)node.get())->_onClick = onClick;
			return node;
		}

		ButtonItem(std::string name = "button", std::string content = "this is a button") { _name = name; _content = content; }

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