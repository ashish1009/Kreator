//
//  Buffer.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

namespace iKan {
    
    /// A Containor to store the data in buffer as bytes.
    /// Allocate the memory in heap on cosntruction and de-allocate the memory on destruction
    struct Buffer {
        // Member variables
        std::byte* Data = nullptr;
        uint32_t Size = 0;

        // Constructors
        constexpr Buffer() = default;
        Buffer(std::byte* data, uint32_t size);
        Buffer(const Buffer& other);
        Buffer(Buffer&& other);
        
        // Destructor
        ~Buffer();
        
        // Member Functions
        /// Delete the Buffer and make size 0
        void Clear();
        /// Set the memory of buffer to 0
        void ZeroInitialize();
        /// Write the data in the buffer
        /// @param data Data to be written in the buffer
        /// @param size Size of data to be copied
        /// @param offset offset from where data to be copied
        void Write(void* data, uint32_t size, uint32_t offset = 0);
        /// Allocate the memory to buffer of size
        /// @param size size of new buffer
        void Allocate(uint32_t size);
        
        // Getters
        /// Return the size of buffer
        constexpr uint32_t GetSize() const;
         
        // Operators
        /// Return true if Data is not nullptr
        operator bool() const;
        /// return the byte reference at position index
        /// @param index index of position
        std::byte& operator[](uint32_t index);
        /// return the byte value at position index
        /// @param index index of position
        std::byte operator[](uint32_t index) const;
        
        /// equal to operator overload
        /// @param other assign Buffer other to this Buffer
        /// NOTE: new memory allocation is done (Deep Copy)
        Buffer& operator =(const Buffer& other);
        /// equal to operator overload
        /// @param other move Buffer other to this Buffer
        /// NOTE: No new memory allocation is done. All data moved as it is
        Buffer& operator =(Buffer&& other);
        
        // Static Member Functions
        /// Create new instance of Buffer. Copy the 'data' and return the newly created buffer
        /// @param data Data to be copied
        /// @param size size of data
        static Buffer Copy(void* data, uint32_t size);
        /// Get the total allocated size in entire program
        static uint32_t GetTotalAllocations();
        
        // Templated Functions : Implementation
        /// Read the buffer at specific offset as T
        /// @param offset offset from where buffer to be read
        template<typename T> T& Read(uint32_t offset) { return *(T*)(Data + offset); }
        /// Read thr buffer from begining as T
        template<typename T> constexpr T* As() { return (T*)Data; }
        
    private:
        void Dealocate();

        // Stores the Total allocation made in program using Buffer class
        static uint32_t s_AllocatedBytes;
    };
    
}
