//
//  UUID.hpp
//  iKan
//
//  Created by iKan on 24/05/22.
//

#pragma once

namespace iKan {
    
    /// "UUID" (universally unique identifier) or GUID is (usually) a 128-bit integer used to "uniquely" identify information. In Kreator
    /// Engine, even though we use the term GUID and UUID, at the moment we're simply using a randomly generated 64-bit integer, as the
    /// possibility of a clash is low enough for now.
    class UUID {
    public:
        UUID();
        UUID(uint64_t uuid);
        UUID(const UUID& other);
        
        /// return the UUID as uint64_t
        operator uint64_t () { return m_UUID; }
        /// return the UUID as const uint64_t
        operator const uint64_t () const { return m_UUID; }

    private:
        uint64_t m_UUID;
    };
    
}

namespace std {
    /// Userdefined Hash ID
    template <> struct hash<iKan::UUID> {
        std::size_t operator()(const iKan::UUID& uuid) const { return hash<uint64_t>()((uint64_t)uuid); }
    };
}
