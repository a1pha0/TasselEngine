#pragma once
#include "Tassel/Core/Base.h"
#include "Tassel/Events/Event.h"

namespace Tassel
{
	class TASSEL_API Layer
	{
	public:
		Layer(const std::string& name = "DefaultLayer") : m_DebugName(name) {}
		virtual ~Layer() {}

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnEvent(Event& event) {}

		virtual void OnImGuiRender() {}

		inline const std::string& GetName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};


	class TASSEL_API LayerStack
	{
	public:
		LayerStack();
		virtual ~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		void PushOverLayer(Layer* overlayer);
		void PopOverLayer(Layer* overlayer);

		std::vector<Layer*>::iterator Begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator End() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		uint32_t m_LayerInsertIndex;
	};
}
