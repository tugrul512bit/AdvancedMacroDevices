#include "AppStructure.h"
namespace Window
{
	class TextItem : public AppStructure
	{
	public:
		static std::shared_ptr<AppStructure> Create(std::string name, std::string content, int fontScale)
		{
			std::shared_ptr<AppStructure> node = std::shared_ptr<TextItem>(new TextItem(name), [](TextItem* t) { delete t; });
			((TextItem*)node.get())->_content = content;
			
			auto currentFont = ImGui::GetFont();
			((TextItem*)node.get())->_fontScale = fontScale;			
			
			return node;
		}

		TextItem(std::string name = "text", std::string content = "this is a text") { _name = name; _content = content; }

		void Compute() override
		{

		}

		void PreRender() override
		{
			ImGui::SetWindowFontScale(_fontScale);
			ImGui::Text(_content.c_str());			
			ImGui::SetWindowFontScale(1.0f);
		}

		void PostRender() override
		{

		}
	private:
		std::string _content;
		float _fontScale;
	};
}