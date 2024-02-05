#pragma once

#include <memory>

#define BIT(x) (1 << x)

// Macro to create a lambda function that binds a member function (fn) to the current object's context
#define ENX_BIND_EVENT_FN(fn) [this](auto&& ... args) -> decltype(auto)\
    { return this->fn(std::forward<decltype(args)>(args)...); }

namespace EngineX
{
    // Define a shorthand "Scope" for unique pointers
    template <typename T>
    using Scope = std::unique_ptr<T>;

    // Create a unique pointer to T and return it
    template <typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args&&... args)
    {
        // Use std::make_unique to construct T and return it as a unique pointer
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    // Define a shorthand "Ref" for shared pointers
    template<typename T>
    using Ref = std::shared_ptr<T>;

    // Create a shared pointer to T and return it
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        // Use std::make_shared to construct T and return it as a shared pointer
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

}

#include "EngineX/Core/Log.h"