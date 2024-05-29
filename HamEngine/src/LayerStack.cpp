#include "Ham/Core/LayerStack.h"

namespace Ham
{

    LayerStack::~LayerStack() {}

    void LayerStack::PushLayer(Layer *layer)
    {
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
        m_LayerInsertIndex++;
    }

    void LayerStack::PushLayerUnique(Layer *layer)
    {
        if (std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer) == m_Layers.begin() + m_LayerInsertIndex)
        {
            m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
            m_LayerInsertIndex++;
        }
    }

    void LayerStack::PushOverlay(Layer *overlay)
    {
        m_Layers.emplace_back(overlay);
    }

    void LayerStack::PushOverlayUnique(Layer *overlay)
    {
        if (std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay) == m_Layers.end())
        {
            m_Layers.emplace_back(overlay);
        }
    }

    void LayerStack::PopLayer(Layer *layer)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
        if (it != m_Layers.begin() + m_LayerInsertIndex)
        {
            m_Layers.erase(it);
            m_LayerInsertIndex--;
        }
    }

    void LayerStack::PopOverlay(Layer *overlay)
    {
        auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
        if (it != m_Layers.end())
        {
            m_Layers.erase(it);
        }
    }

}