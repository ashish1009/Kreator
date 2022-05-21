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
    /// Base class of all type of events
    class Event {
    public:
        /// Type of Event supported by iKan
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

        // Member Methods
        virtual ~Event() = default;
        
        // Getters
        /// Return the type of triggered Event. e.g. EindowClosed...
        virtual Type GetEventType() const = 0;
        /// Return the Name of Event in char (Convert the type of Event in string)
        virtual const char* GetName() const = 0;
        /// Return the Caregory Flag of Event as Bit poisition defined in enu, Category
        virtual int32_t GetCategoryFlags() const = 0;
        
        /// Check if Event is aa same category as argument 'category'
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
        /// Checks the type of triggered Event (m_Event.GetEventType()) is same as Dispatched Event T::GetStaticType())
        /// If same then call the Function F (Function pointer passed to Dispatcher)
        /// NOTE : Template - <T> - Type of Dispatched event
        ///                 - <F> - Function type. This is dynmically executed. It stores the type of function passed as pointer
        /// @param func Function pointer that need to dispatch on event occurance
        /// -------------------------------------------------------------------------------------
        /// Event Should be Dispached using this function only. All Event handler takes only base
        /// Instance of Event. This Dispatcher Dispatch the function binded to a specific Event.
        /// So checked all the triggered events at same time and return the calling on Function
        /// Binded as Funciton pointer <F>
        /// -------------------------------------------------------------------------------------
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
