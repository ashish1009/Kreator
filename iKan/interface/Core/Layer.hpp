//
//  Layer.hpp
//  iKan
//
//  Created by iKan on 19/05/22.
//

#pragma once

// Interface to create a Layer at Core or Client.
// All the layer that need to be stored in Application should be derived from this Layer
namespace iKan {

    // Forward declaration
    class Event;
    
    /// Layer Interface class
    /// Core Application manages Bellow API As per functionality defined
    class Layer {
    public:
        Layer(const std::string& name) : m_Name(name) {}
        virtual ~Layer() = default;

        // NOTE: Override these virtual Methods in Client Layer
        /// Calls when we push layer in application. Initilaise the Layer.
        virtual void Attach() = 0;
        
        /// Calls when we pop layer in application.
        virtual void Detach() = 0;
        
        /// Render GUI Window for this layer. To be called each frame from application
        virtual void RenderGui() = 0;
        
        /// Update layer each frame. To be called each frame from application
        virtual void Update() = 0;

        /// Handle events of Layer. Trigger when any Even interrupted layer in each frame.
        /// calls specific even handler from here internally
        /// @param event Event (Base class) intance. Dispatch event from Event Dispatcher
        virtual void EventHandler(Event& event) = 0;

        /// Return the name of layer
        virtual const std::string& GetName() const = 0;

    protected:
        std::string m_Name = "Unknown";
    };

}
