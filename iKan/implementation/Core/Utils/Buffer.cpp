//
//  Buffer.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "Buffer.hpp"
#include "Core/Utils/Asserts.h"

using namespace iKan;

uint32_t Buffer::s_AllocatedBytes = 0;

/// Buffer constructor to assign already allocated memory
/// @param data Allocated Memory pointer as byte
/// @param size size of memory to be stored in Buffer
Buffer::Buffer(std::byte* data, uint32_t size) : Data(data), Size(size) {}

/// Copy Constructot
/// @param other other buffer that need to be copy inside the buffer
Buffer::Buffer(const Buffer& other) : Size(other.Size) {
    Data = new std::byte[Size];
    memcpy(Data, other.Data, Size);
    
    s_AllocatedBytes += Size;
}

/// Move Constructot
/// @param other other buffer that need to be copy inside the buffer
Buffer::Buffer(Buffer&& other) : Data(other.Data),  Size(other.Size) {
    other.Size = 0;
    other.Data = nullptr;
}

/// Buffer Destructor
Buffer::~Buffer() {
    Clear();
}

/// Clear the Buffer
void Buffer::Clear() {
    if (Data)
        Dealocate();
    
    Size = 0;
    Data = nullptr;
}

/// Allocate new memory to buffer
/// @param size size of Allocatiom
void Buffer::Allocate(uint32_t size) {
    if (size == 0)
        return;
    
    Clear();
    Data = new std::byte[size];
    Size = size;
    
    s_AllocatedBytes += size;
}

/// De-Allocate the memory. Delete
void Buffer::Dealocate() {
    s_AllocatedBytes -= Size;
    delete[] Data;
}

/// Initialize the buffer with 0
void Buffer::ZeroInitialize() {
    if (Data)
        memset(Data, 0, Size);
}

/// Write the data in the buffer
/// @param data Data to be written in the buffer
/// @param size Size of data to be copied
/// @param offset offset from where data to be copied
void Buffer::Write(void* data, uint32_t size, uint32_t offset) {
    if (size == 0)
        return;
    
    IK_CORE_ASSERT(offset + size <= Size || offset > Size, "Buffer overflow!");
    memcpy(Data + offset, data, size);
}

/// Get the Size of Buffer
constexpr uint32_t Buffer::GetSize() const {
    return Size;
}

/// bool Operator overloading
Buffer::operator bool() const { return Data; }

/// [] Operator Overload for both value and Reference of Data at index
/// @param index index of data to be returned
std::byte& Buffer::operator[](uint32_t index) { return Data[index]; }
std::byte Buffer::operator[](uint32_t index) const { return Data[index]; }

/// Copy Operator =
/// @param other Buffer instacne that need to be copied
Buffer& Buffer::operator =(const Buffer& other) {
    Clear();
    Allocate(other.Size);
    Write((void*)other.Data, other.Size);
    return *this;
}

/// Move Operator =
/// @param other Buffer instacne that need to be moved
Buffer& Buffer::operator =(Buffer&& other) {
    Clear();

    Size = other.Size;
    Data = other.Data;
    
    other.Size = 0;
    other.Data = nullptr;
    
    return *this;
}

/// Create new instance of Buffer. Copy the data and return the buffer
/// @param data Data to be copied
/// @param size size of data
Buffer Buffer::Copy(void* data, uint32_t size) {
    Buffer buffer;
    
    if (size == 0)
        return buffer;
    
    buffer.Allocate(size);
    memcpy(buffer.Data, data, size);
    return buffer;
}

/// get The total allocated size till call
uint32_t Buffer::GetTotalAllocations() {
    return s_AllocatedBytes;
}
