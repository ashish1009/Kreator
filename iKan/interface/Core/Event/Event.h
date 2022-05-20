//
//  Event.h
//  Kreator
//
//  Created by iKan on 20/05/22.
//

#pragma once

namespace iKan {
    
#define EVENT_CLASS_TYPE(type) static Event::Type GetStaticType() { return Event::Type::type; }     \
virtual Event::Type GetEventType() const override { return GetStaticType(); }                     \
virtual const char* GetName() const override { return #type; }
    
#define EVENT_CLASS_CATEGORY(category) virtual int32_t GetCategoryFlags() const override { return category; }
    
// API to bind class member function to std function pointer
#define IK_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
    
    /// Wrapper Base class to handle all the events
    class Event {
    public:
        /// Type of Event supported by Kreator
        enum Type : uint8_t {
            NoEventType,
            WindowClose, WindowResize, WindowFocused, WindowHovered,
            KeyPressed, KeyReleased, KeyTyped,
            MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
        };
        
        /// Event Categorisation
        enum Category : uint8_t {
            NoEventCategory = 0,
            EventCategoryApplication    = BIT(0),
            EventCategoryInput          = BIT(1),
            EventCategoryKeyboard       = BIT(2),
            EventCategoryMouse          = BIT(3),
            EventCategoryMouseButton    = BIT(4)
        };

        // Member functions
        virtual ~Event() = default;
        
        // Getters
        virtual Type GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int32_t GetCategoryFlags() const = 0;
        
        bool IsInCategory(Category category) const { return GetCategoryFlags() & category; }
        
    public:
        bool Handled = false;
    };
    
    /// Class to dispatch event dynamically. Event type can be checked with current event and perform the functionality
    class EventDispatcher {
    public:
        EventDispatcher(Event& event)
        : m_Event(event) {}
        
        template<typename T, typename F>
        /// Dispatch the event. Once an event callback triggered, it dispatch the event function with template T
        /// NOTE : Template - <T> - Type of Event Triggered
        ///                 - <F> - Function type. This is dynmically executed. It stores the type of function passed as pointer
        /// @param func Function pointer that need to dispace on event occurance
        bool Dispatch(const F& func) {
            if (m_Event.GetEventType() == T::GetStaticType()) {
                m_Event.Handled |= func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }
        
    private:
        Event& m_Event;
    };
    
}
