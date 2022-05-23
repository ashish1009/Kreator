//
//  FrameBuffer.cpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#include "FrameBuffer.hpp"
#include "Platform/OpenGL/OpenGLFrameBuffer.hpp"

using namespace iKan;

/// Framebuffer Attachment copy Constructor
/// @param other other Attchment
FrameBuffer::Attachment::Attachment(const FrameBuffer::Attachment& other) : TextureFormats(other.TextureFormats) {
    IK_CORE_INFO("Framebuffer Attachment Copied");
}

/// Copy Operator = for Framebuffer Attachment
/// @param other Specificaion reference
FrameBuffer::Attachment& FrameBuffer::Attachment::operator=(const FrameBuffer::Attachment& other) {
    IK_CORE_TRACE("Copying (= operator) FrameBuffer::Attachment  ");
    TextureFormats = other.TextureFormats;
    // NOTE: For now this copy constructor is only to Avoid Redundant copying the data
    return *this;
}

/// Framebuffer specification Copy Constructor
/// @param other other Specification
FrameBuffer::Specification::Specification(const FrameBuffer::Specification& other) : Width(other.Width), Height(other.Height), Color(other.Color), Attachments(other.Attachments) {
    IK_CORE_INFO("Framebuffer Specification Copied");
}

/// Copy Operator = for Framebuffer Specification
/// @param other Specificaion reference
FrameBuffer::Specification& FrameBuffer::Specification::operator=(const FrameBuffer::Specification& other) {
    IK_CORE_TRACE("Copying (= operator) FrameBuffer::Attachment  ");
    Width = other.Width;
    Height = other.Height;
    Color = other.Color;
    Attachments = other.Attachments;
    // NOTE: For now this copy constructor is only to Avoid Redundant copying the data
    return *this;
}

/// Create instance of renderer Framebuffer based on Current Rendere API
/// @param spec Frame buffer specification
std::shared_ptr<FrameBuffer> FrameBuffer::Create(const Specification& spec) {
    switch (Renderer::GetAPI()) {
        case Renderer::API::OpenGL: return std::make_shared<OpenGLFrameBuffer>(spec);
        case Renderer::API::None:
        default: IK_CORE_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
}
