//
//  ImguiAPI.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

namespace iKan {
    
    /// Imgui Wrapper class
    class ImguiAPI {
    public:
        // Docking
        static void StartDcocking();
        static void EndDcocking();
        
        // Window to show stats and data
        static void FrameRate();
        
    private:
        ImguiAPI() = default;
        MAKE_SINGLETON(ImguiAPI);
    };
    
}
