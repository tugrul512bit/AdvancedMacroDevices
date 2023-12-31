#pragma once
#include<string>
#include<vector>
#include<memory>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <mutex>
#include<functional>
#include<queue>
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
			_canDragDrop = false;
		}

		virtual void PreRender() {}
		virtual void PostRender() {}

		virtual void Compute() {}

		void Calculate()
		{
			Compute();
			for (auto node : _childNodes)
			{
				if(node.get())
					node->Calculate();
			}
			
		}

		void Render()
		{			
			if(_changeDefaultColor)
				ImGui::PushStyleColor(_defaultType, _defaultColor);
			if (_onPreRender)
			{
				_onPreRender();
			}
			if (_visible)
			{
				if (_sameLine)
					ImGui::SameLine();
				if (!_enabled)
					ImGui::BeginDisabled();

				ImGui::PushID(_name.c_str());
				PreRender();

				if (ImGui::IsItemHovered() && _hoverPopup.get()!=nullptr)
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					if(_hoverPopup.get())
						_hoverPopup->Render();
					ImGui::PopTextWrapPos();
					ImGui::EndTooltip();
				}

				for (auto node : _childNodes)
				{
					if(node.get())
						node->Render();
				}
				
				PostRender();
				ImGui::PopID();

				if (!_enabled)
					ImGui::EndDisabled();

			}
			if (_changeDefaultColor)
				ImGui::PopStyleColor(1);
		}

		template<typename T>
		T* AsPtr()
		{
			return (T*)this;
		}

		void DeleteAllNodes()
		{
			_childNodes.clear();
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

		void AddHoverPopup(std::shared_ptr<AppStructure> structure)
		{
			_hoverPopup = structure;
		}

		void MakeDraggable()
		{
			_canDragDrop = true;
		}

		void OnPreRender(std::function<void(void)> onPreRender)
		{
			_onPreRender=onPreRender;
		}
	protected:
		ItemType _type;
		std::string _name;
		int _renderWidth;
		int _renderHeight;
		bool _visible;
		bool _enabled;
		bool _sameLine;
		bool _canDragDrop;
		std::vector<std::shared_ptr<AppStructure>> _childNodes;
		std::shared_ptr<AppStructure> _hoverPopup;
		std::function<void(void)> _onPreRender;
		bool _changeDefaultColor;
		ImVec4 _defaultColor;
		ImGuiCol_ _defaultType;
	};

}