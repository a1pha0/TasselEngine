#include "Tassel/Core/Base.h"
#include "Tassel/Core/Layer.h"

namespace Tassel
{
	LayerStack::LayerStack()
		: m_LayerInsertIndex(0)
	{
	}

	LayerStack::~LayerStack()
	{
		if (!m_Layers.empty())
		{
			for (Layer* layer : m_Layers)
			{
				delete layer;
				layer = nullptr;
			}
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		layer->OnAttach();
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			layer->OnDetach();
			m_Layers.erase(it);
			m_LayerInsertIndex--; // TODO: Why?
		}
	}

	void LayerStack::PushOverLayer(Layer* overlayer)
	{
		overlayer->OnAttach();
		m_Layers.emplace_back(overlayer);
	}

	void LayerStack::PopOverLayer(Layer* overlayer)
	{
		std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), overlayer);
		if (it != m_Layers.end())
		{
			overlayer->OnDetach();
			m_Layers.erase(it);
		}
	}
}