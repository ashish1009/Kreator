//
//  OpenGLRendererPipeline.cpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#include "OpenGLRendererPipeline.hpp"
#include <glad/glad.h>

using namespace iKan;


/// Get the Open GL Data type from user defined Shader data type
/// @param type Shader data type
static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::NoType:   return 0;
        case ShaderDataType::Float:    return GL_FLOAT;
        case ShaderDataType::Float2:   return GL_FLOAT;
        case ShaderDataType::Float3:   return GL_FLOAT;
        case ShaderDataType::Float4:   return GL_FLOAT;
        case ShaderDataType::Mat3:     return GL_FLOAT;
        case ShaderDataType::Mat4:     return GL_FLOAT;
        case ShaderDataType::Int:      return GL_INT;
        case ShaderDataType::Int2:     return GL_INT;
        case ShaderDataType::Int3:     return GL_INT;
        case ShaderDataType::Int4:     return GL_INT;
        case ShaderDataType::Bool:     return GL_BOOL;
    }
    
    IK_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

#ifdef IK_DEBUG_FEATURE

/// Get the Open GL Data type from user defined Shader data type
/// @param type Shader data type
static std::string ShaderDataTypeToString(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::NoType:   return "NoType";
        case ShaderDataType::Float:    return "Float ";
        case ShaderDataType::Float2:   return "Float2";
        case ShaderDataType::Float3:   return "Float3";
        case ShaderDataType::Float4:   return "Float4";
        case ShaderDataType::Mat3:     return "Mat3  ";
        case ShaderDataType::Mat4:     return "Mat4  ";
        case ShaderDataType::Int:      return "Int   ";
        case ShaderDataType::Int2:     return "Int2  ";
        case ShaderDataType::Int3:     return "Int3  ";
        case ShaderDataType::Int4:     return "Int4  ";
        case ShaderDataType::Bool:     return "Bool  ";
    }
    
    IK_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

#endif

/// Open GL Pipeline Constructor
OpenGLRendererPipeline::OpenGLRendererPipeline() {
    PROFILE();
    glGenVertexArrays(1, &m_RendererID);
    glBindVertexArray(m_RendererID);

    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Creating Open GL Piperline ...");
    IK_CORE_INFO("    Renderer ID : {0}", m_RendererID);
    IK_LOG_SEPARATOR();
}

/// Open GL Pipeline Destructor
OpenGLRendererPipeline::~OpenGLRendererPipeline() {
    PROFILE();
    IK_CORE_WARN("Destroying Open GL Piperline with Renderer ID : {0} !!!", m_RendererID);
    glDeleteVertexArrays(1, &m_RendererID);
}

/// Bind Open GL Pipeline
void OpenGLRendererPipeline::Bind() const {
    glBindVertexArray(m_RendererID);
    for (auto vb : m_VertexBuffers)
        vb->Bind();
    
    if (m_IndexBuffer)
        m_IndexBuffer->Bind();
}

/// Unbind Open GL Pipeline
void OpenGLRendererPipeline::Unbind() const {
    glBindVertexArray(0);
}

/// Add the Pipeline in the Open GL Pipeline.
/// @param vertexBuffer Vertex Buffer adding to the Pipeline
void OpenGLRendererPipeline::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
    PROFILE();
    IK_CORE_INFO("Storing the Vertex Buffer (ID: {0}) into Pipeline (ID: {1})", vertexBuffer->GetRendererID(), m_RendererID);
    IK_CORE_INFO("    Number of Vertex Buffer stored in Pipeline (ID {0}) : {1}", m_RendererID, m_VertexBuffers.size());
    
    glBindVertexArray(m_RendererID);
    m_VertexBuffers.push_back(vertexBuffer);
    
    IK_LOG_SEPARATOR();

    uint32_t index = 0;
    const auto& layout = vertexBuffer->GetLayout();
    IK_CORE_INFO("    Vertex Attrib with Stride : {0} ", layout.GetStride());
    for (const auto& element : layout.GetElements()) {
        IK_CORE_INFO("        -----------------------------------------");
        IK_CORE_INFO("        Name      : {0} ", element.Name);
        IK_CORE_INFO("        Type      : {0} ", ShaderDataTypeToString(element.Type));
        IK_CORE_INFO("        Size      : {0} ", element.Size);
        IK_CORE_INFO("        Count     : {0} ", element.Count);
        IK_CORE_INFO("        Offset    : {0} ", element.Offset);
        IK_CORE_INFO("        Normalize : {0} ", element.Normalized);

        switch (element.Type) {
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::Bool: {
                glEnableVertexAttribArray(index);
                glVertexAttribIPointer(index,
                                       element.Count,
                                       ShaderDataTypeToOpenGLBaseType(element.Type),
                                       layout.GetStride(),
                                       (const void*)element.Offset);
                index++;
                break;
            }
                
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4: {
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index,
                                      element.Count,
                                      ShaderDataTypeToOpenGLBaseType(element.Type),
                                      element.Normalized ? GL_TRUE : GL_FALSE,
                                      layout.GetStride(),
                                      (const void*)element.Offset);
                index++;
                break;
            }
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4: {
                uint8_t count = element.Count;
                for (uint8_t i = 0; i < count; i++) {
                    glEnableVertexAttribArray(index);
                    glVertexAttribPointer(index,
                                          count,
                                          ShaderDataTypeToOpenGLBaseType(element.Type),
                                          element.Normalized ? GL_TRUE : GL_FALSE,
                                          layout.GetStride(),
                                          (const void*)(sizeof(float) * count * i));
                    glVertexAttribDivisor(index, 1);
                    index++;
                }
                break;
            }
            default: {
                IK_CORE_ASSERT(false, "Unknown ShaderDataType!");
            }
        } // switch (element.Type)
    } // for (const auto& element : layout.GetElements())
    IK_LOG_SEPARATOR();
}

/// Setting Open GL Pipeline Constructor
/// @param indexBuffer Current Index buffer for Pipeline
void OpenGLRendererPipeline::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
    PROFILE();
    IK_CORE_INFO("Setting up the Index Buffer (ID: {0}) into Pipeline (ID: {1})", indexBuffer->GetRendererID(), m_RendererID);
    m_IndexBuffer = indexBuffer;
    glBindVertexArray(m_RendererID);
    m_IndexBuffer->Bind();
}
