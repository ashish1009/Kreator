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
        /// Enable the docking in Imgui
        static void StartDcocking();
        /// Disable the docking
        static void EndDcocking();
        
        // Window to show stats and data
        /// Show the frame rate of Renderer
        static void FrameRate();
        
        // Theme
        /// Set the Grey theme of Imgui
        static void SetGreyThemeColors();
        /// Set the Light theme of Imgui
        static void SetLightThemeColors();
        /// Set the Dark theme of Imgui
        static void SetDarkThemeColors();
        
    private:
        ImguiAPI() = default;
        MAKE_SINGLETON(ImguiAPI);
    };
    
}
