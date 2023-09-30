#include "AppStructure.h"
namespace Window
{
	class ButtonItem : public AppStructure
	{
	public:
		static std::shared_ptr<AppStructure> Create(std::string name, std::string content)
		{
			std::shared_ptr<AppStructure> node = std::shared_ptr<ButtonItem>(new ButtonItem(name), [](ButtonItem* b) { delete b; });
			((ButtonItem*)node.get())->_content = content;
			return node;
		}

		ButtonItem(std::string name = "button", std::string content = "this is a button") { _name = name; _content = content; }

		void Compute() override
		{

		}

		void PreRender() override
		{
			ImGui::Button(_content.c_str());
		}

		void PostRender() override
		{

		}
	private:
		std::string _content;
	};
}