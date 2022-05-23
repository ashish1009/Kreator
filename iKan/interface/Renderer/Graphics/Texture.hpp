//
//  Texture.hpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#pragma once

#include "Renderer/Utils/Renderer.hpp"

namespace iKan {
    
    /// Interface class for Storing Renderer Texture data. Implementation is depending on the
    /// Supported Renerer API.
    class Texture {
    public:
        virtual ~Texture() = default;
        
        /// Return the Width of Texture
        virtual uint32_t GetWidth() const = 0;
        /// Return the Height of Texture
        virtual uint32_t GetHeight() const = 0;
        /// Return the Renderer ID of Texture
        virtual RendererID GetRendererID() const = 0;
        /// Return the File Path of Texture
        /// NOTE: Return "" for white texture
        virtual std::string GetfilePath() const = 0;
        
        /// Bind the Current Texture to a slot of shader
        /// @param slot Slot of shader
        virtual void Bind(uint32_t slot = 0) const = 0;
        /// Unbind the Current Texture from shader slot
        virtual void Unbind() const = 0;
        
        /// Create Emptry Textyre with user Defined Data of size height and Width
        /// @param width Width of Empty Texture
        /// @param height Height of Empty Texture
        /// @param data Data to be stored in Empty Texture
        /// @param size Size of type of data stored in Texture
        static std::shared_ptr<Texture> Create(uint32_t width, uint32_t height, void* data, uint32_t size);
    };
    
}
