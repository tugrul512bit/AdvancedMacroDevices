#pragma once
#include<string>
#include<vector>
#include<memory>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <mutex>
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
			std::unique_lock<std::mutex> lck(_syncPoint);
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
			std::unique_lock<std::mutex> lck(_syncPoint);
			Compute();
			for (auto node : _childNodes)
			{
				if(node.get())
					node->Calculate();
			}
			
		}

		void Render()
		{			
			std::unique_lock<std::mutex> lck(_syncPoint);
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
		}

		template<typename T>
		T* AsPtr()
		{
			return (T*)this;
		}

		void DeleteAllNodes()
		{
			std::unique_lock<std::mutex> lck(_syncPoint);
			_childNodes.clear();
		}

		void AddNode(std::shared_ptr<AppStructure> node)
		{
			std::unique_lock<std::mutex> lck(_syncPoint);
			_childNodes.push_back(node);
		}

		void Enable()
		{
			std::unique_lock<std::mutex> lck(_syncPoint);
			_enabled = true;
		}
		void Disable()
		{
			std::unique_lock<std::mutex> lck(_syncPoint);
			_enabled = false;
		}

		void Hide()
		{
			std::unique_lock<std::mutex> lck(_syncPoint);
			_visible = false;
		}

		void Show()
		{
			std::unique_lock<std::mutex> lck(_syncPoint);
			_visible = true;
		}

		void AddHoverPopup(std::shared_ptr<AppStructure> structure)
		{
			std::unique_lock<std::mutex> lck(_syncPoint);
			_hoverPopup = structure;
		}

		void MakeDraggable()
		{
			std::unique_lock<std::mutex> lck(_syncPoint);
			_canDragDrop = true;
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
		std::mutex _syncPoint;
	};

}