#pragma once
#include <memory>

#define BIT(x) (1 << x)

// Macro to create a lambda function that binds a member function (fn) to the current object's context
#define ENX_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace EngineX
{
    // This makes it easier to create unique pointers by being able to declare them using Scope
    template <typename T>
    using Scope = std::unique_ptr<T>;

    // This makes it possible to have arguments associated with the pointer
    template <typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    // Same as above but for shared pointers
    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

