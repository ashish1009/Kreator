//
//  ApplicationEvent.h
//  Kreator
//
//  Created by iKan on 20/05/22.
//

#pragma once

#include "Core/Event/Event.h"

namespace iKan {
    
    /// Window Resize event
    class WindowResizeEvent : public Event {
    public:
        virtual ~WindowResizeEvent() = default;
        
        /// Window Resize Event constructor
        /// @param width width of new window
        /// @param height height of new window
        WindowResizeEvent(uint32_t width, uint32_t height)
        : m_Width(width), m_Height(height) {}
        
        /// return the width of window
        uint32_t GetWidth() const { return m_Width; }
        /// return the height of window
        uint32_t GetHeight() const { return m_Height; }
        
        EVENT_CLASS_TYPE(WindowResize);
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
        
    private:
        uint32_t m_Width, m_Height;
    };
    
    /// Window Close event
    class WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() = default;
        virtual ~WindowCloseEvent() = default;
        
        EVENT_CLASS_TYPE(WindowClose);
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
    
}
