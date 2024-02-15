#pragma once
#include "Layer.h"

#include <vector>

namespace EngineX
{
   /* VERY important reminder:
    * We want to update rendering from first index -> last index!!
    * This is because  we want our UI / Gizmos / Debug graphics etc to render on top of everything else.
    * Basically like the naming is: Render the layers first -> then render the overlays
    * We want to update events from last index -> first index!!
    * This is because if we our overlays such as UI's which exists in the last half of the vector and we click on left mouse click on a UI Button
    * We want to trigger the event for said button and then tell the event system that the event has been handled
    * Its important that we tell the system that the event has been handled otherwise it will chain down all the layers
    * and potentially trigger another event that is based on left mouse click
    */
 
    /* The LayerStack decides the rendering order and event order etc. 
     * A vector is a dynamic array-like data structure. Allowing for dynamic resizing and efficient random access 
     * Not using an actual 'Stack' because we to be able to push layers into whatever order we so wish
     * For example: We need layer y to go in front of layer x
     * NOTE: A layer can never go in back of an overlay and an overlay can never go before a layer!
     * A layer should always be in the first half of the array
     * Layers primary use is for 3d rendering etc.  
     * An overlay should always be in the last half of the array
     * Overlays primary usage is for UI / gizmos / debug graphics etc.
     */ 
    class LayerStack
    {
    public:
        LayerStack();
        ~ LayerStack();

        
        /**
         * \brief This inserts the given layer into the first half of the vector m_Layers
         * \param layer The layer that should be inserted
         */
        void InsertLayer(Layer* layer);
        
        /**
         * \brief Removes the first occurrence of the layer from the vector of layers if found
         * \param layer the layer that should be removed
         */
        void RemoveLayer(Layer* layer);
        
        /**
         * \brief This inserts the given overlay into the last half of the vector m_Layers
         * \param overlay The overlay that should be inserted
         */
        void InsertOverlay(Layer* overlay);
     
        /**
         * \brief Removes the first occurrence of the overlay from the vector of layers if found
         * \param overlay the overlay that should be removed
         */
        void RemoveOverlay(Layer* overlay);
        
        // Returns an iterator pointing to the first element of the vector.
        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }

        // Returns an iterator pointing to the position just beyond the last element of the vector.
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }

    private:
        std::vector<Layer*> m_Layers;
        unsigned int m_LayerInsertIndex = 0;
    };
}
