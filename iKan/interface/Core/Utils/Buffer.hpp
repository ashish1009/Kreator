//
//  Buffer.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

namespace iKan {
    
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
        
        // Getters
        constexpr uint32_t GetSize() const;
         
        // Operators
        operator bool() const;
        std::byte& operator[](uint32_t size);
        std::byte operator[](uint32_t size) const;
        
        Buffer& operator =(const Buffer& other);
        Buffer& operator =(Buffer&& other);
        
        // Static Member Functions
        /// Create new instance of Buffer. Copy the data and return the buffer
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
        void Allocate(uint32_t size);

        // Stores the Total allocation made in program using Buffer class
        static uint32_t s_AllocatedBytes;
    };
    
}
