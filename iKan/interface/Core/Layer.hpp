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

    /// Layer Interface class
    class Layer {
    public:
        Layer(const std::string& name) : m_Name(name) {}
        virtual ~Layer() = default;

        // NOTE: Override these virtual Methods in Client Layer
        virtual void Attach() = 0;
        virtual void Update() = 0;
        virtual void RenderGui() = 0;
        virtual void Detach() = 0;
        virtual void EventHandler() = 0;

        virtual const std::string& GetName() const = 0;

    protected:
        std::string m_Name = "Unknown";
    };

}
