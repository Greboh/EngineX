#include "enxpch.h"
#include "LayerStack.h"

namespace EngineX
{
    LayerStack::LayerStack()
    {
        // Start it at the beginning of the vector.
        m_LayerInsert = m_Layers.begin();
    }

    LayerStack::~LayerStack()
    {
        // Remove all layers
        for (Layer* layer : m_Layers)
        {
            delete layer;
        }
    }

    void LayerStack::InsertLayer(Layer* layer)
    {
        // Insert layer at the position based on m_LayerInsert (begin | end)
        m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
    }

    void LayerStack::RemoveLayer(Layer* layer)
    {
        // Find the first layer in the vector that satisfies the search query (our layer)
        // If the iteration is not at the end of our layers remove that iteration and shrink the layer
        if( const auto iteration= std::ranges::find(m_Layers, layer)
            ; iteration != m_Layers.end())
        {
            m_Layers.erase(iteration);
            --m_LayerInsert;
        }
    }

    void LayerStack::InsertOverlay(Layer* overlay)
    {
        // Insert layer at the end of array .. Always
        m_Layers.emplace_back(overlay);
    }

    void LayerStack::RemoveOverlay(Layer* overlay)
    {
        if( const auto iteration= std::ranges::find(m_Layers, overlay)
           ; iteration != m_Layers.end())
        {
            m_Layers.erase(iteration);
        }
    }
}
