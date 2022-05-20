//
//  KeyEvent.h
//  Kreator
//
//  Created by iKan on 20/05/22.
//

#pragma once

#include "Core/Event/Event.h"
#include "Core/KeyCodes.h"

namespace iKan {
    
    /// Key base event class
    class KeyEvent : public Event {
    public:
        virtual ~KeyEvent() = default;
        KeyCode GetKeyCode() const { return m_Code; }
        
        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
        
    protected:
        KeyEvent(KeyCode code)
        : m_Code(code) {}
        
    protected:
        KeyCode m_Code;
    };
    
    /// Key Pressed Event class
    class KeyPressedEvent : public KeyEvent {
    public:
        virtual ~KeyPressedEvent() = default;
        
        KeyPressedEvent(KeyCode keycode, int32_t repeatCount)
        : KeyEvent(keycode), m_RepeatCount(repeatCount) {}
        
        int32_t GetRepeatCount() const { return m_RepeatCount; }
        
        EVENT_CLASS_TYPE(KeyPressed);
        
    private:
        uint32_t m_RepeatCount = 0;
    };
    
    /// Key Released event class
    class KeyReleasedEvent : public KeyEvent {
    public:
        virtual ~KeyReleasedEvent() = default;
        
        KeyReleasedEvent(KeyCode keycode)
        : KeyEvent(keycode) {}
        
        EVENT_CLASS_TYPE(KeyReleased);
    };
    
    /// Key typed Event class
    class KeyTypedEvent : public KeyEvent {
    public:
        virtual ~KeyTypedEvent() = default;
        
        KeyTypedEvent(KeyCode keycode)
        : KeyEvent(keycode) {}
        
        EVENT_CLASS_TYPE(KeyTyped);
    };
    
}
