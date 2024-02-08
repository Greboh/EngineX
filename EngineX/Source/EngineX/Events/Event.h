﻿#pragma once
#include "enxpch.h"

namespace EngineX
{
    // Events are currently blocking. Meaning that it will immediately get dispatched and must be dealt with.  
    //TODO: Make a sort of buffer for events in a bus and process them during update stage accordingly
    enum class EventType
    {
        None = 0,
        // Window Events
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        // Application Events
        AppTick,
        AppUpdate,
        AppRender,
        // K&B Events
        KeyPressed,
        KeyReleased,
        KeyTyped,
        // Mouse Events
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    enum EventCategory
    {
        None = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type;}\
                                virtual EventType GetEventType() const override { return GetStaticType(); }\
                                virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

    class Event
    {
    public:
        virtual ~Event() = default;

        bool Handled = false;

        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }
    };

    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event) : m_Event(event)
        {
        }

        // F Will be deduced by the compiler!
        template <typename T, typename F>
        bool Dispatch(const F& func)
        {
            if (m_Event.GetEventType() == T::GetStaticType())
            {
                // Combining Handling Results: After calling the event handler (func),
                // we update the Handled flag to reflect whether the event was processed.
                // This flag indicates if any event handler has dealt with the event.

                // Bitwise OR Assignment: The |= operator merges the current Handled flag
                // with the result of the event handler. This allows multiple handlers to
                // contribute to the Handled flag. If any handler sets it, the overall result is true..

                m_Event.Handled |= func(static_cast<T&>(m_Event));
                return true;
            }

            return false;
        }

    private:
        Event& m_Event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }
}
