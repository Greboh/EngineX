#pragma once

#include <memory>

namespace EngineX
{
    class Texture
    {
    public:
        virtual void Bind(uint32_t slot = 0) const = 0;
    };

    class Texture2D : public Texture
    {
    public:
        static Ref<Texture2D> Create(const std::string& filePath);
    };
    
}
