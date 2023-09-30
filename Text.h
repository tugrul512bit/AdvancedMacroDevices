#include "AppStructure.h"
#include<functional>
namespace Window
{
	class TextItem : public AppStructure
	{
	public:
		static std::shared_ptr<AppStructure> Create(std::string name, std::function<std::string(void)> content, int fontScale, bool sameLine)
		{
			std::shared_ptr<AppStructure> node = std::shared_ptr<TextItem>(new TextItem(name,content,sameLine,fontScale), [](TextItem* t) { delete t; });			
			return node;
		}

		TextItem(std::string name = "text", std::function<std::string(void)> content = []() {return ""; }, bool sameLine = false, float fontScale = 1.0f)
		{ 
			_name = name; 
			_content = content; 
			_sameLine = sameLine; 
			_fontScale = fontScale;
		}

		void Compute() override
		{

		}

		void PreRender() override
		{
			ImGui::SetWindowFontScale(_fontScale);
			ImGui::Text(_content().c_str());
			ImGui::SetWindowFontScale(1.0f);
		}

		void PostRender() override
		{

		}
	private:
		std::function<std::string(void)> _content;
		float _fontScale;
	};
}