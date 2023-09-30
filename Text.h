#include "AppStructure.h"
namespace Window
{
	class TextItem : public AppStructure
	{
	public:
		static std::shared_ptr<AppStructure> Create(std::string name, std::string content)
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