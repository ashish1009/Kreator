//
//  MouseEvent.h
//  Kreator
//
//  Created by iKan on 09/04/22.
//

#pragma once

#include "Core/Event/Event.h"
#include "Core/MouseCodes.h"

namespace iKan {
    
    /// Mouse base Event class
    class MouseMovedEvent : public Event {
    public:
        virtual ~MouseMovedEvent() = default;
        
        MouseMovedEvent(float x, float y)
        : m_MouseX(x), m_MouseY(y) {}
        
        float GetX() const { return m_MouseX; }
        float GetY() const { return m_MouseY; }
        
        EVENT_CLASS_TYPE(MouseMoved);
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
        
    private:
        MOUSE_POS_X_TYPE m_MouseX;
        MOUSE_POS_Y_TYPE m_MouseY;
    };
    
    /// Mouse scrol event
    class MouseScrolledEvent : public Event {
    public:
        virtual ~MouseScrolledEvent() = default;
        
        MouseScrolledEvent(float xOffset, float yOffset)
        : m_XOffset(xOffset), m_YOffset(yOffset) {}
        
        float GetXOffset() const { return m_XOffset; }
        float GetYOffset() const { return m_YOffset; }
        
        EVENT_CLASS_TYPE(MouseScrolled);
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
        
    private:
        MOUSE_POS_X_TYPE m_XOffset;
        MOUSE_POS_Y_TYPE m_YOffset;
    };
    
    /// Mouse button base calss
    class MouseButtonEvent : public Event {
    public:
        virtual ~MouseButtonEvent() = default;
        MouseButton GetMouseButton() const { return m_Button; }
        
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
        
    protected:
        MouseButtonEvent(MouseButton button)
        : m_Button(button) {}
        
        MouseButton m_Button;
    };
    
    /// Mouse button pressed
    class MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        virtual ~MouseButtonPressedEvent() = default;
        
        MouseButtonPressedEvent(MouseButton button)
        : MouseButtonEvent(button) {}
        
        EVENT_CLASS_TYPE(MouseButtonPressed);
    };
    
    /// Mouse button released
    class MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        virtual ~MouseButtonReleasedEvent() = default;
        
        MouseButtonReleasedEvent(MouseButton button)
        : MouseButtonEvent(button) {}
        
        EVENT_CLASS_TYPE(MouseButtonReleased);
    };
    
}
