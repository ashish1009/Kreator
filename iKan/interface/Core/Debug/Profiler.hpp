//
//  Profiler.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

// Call 'Profile()' in begining of any scope. It will compute the time and print the Profiler result at the end of Scope
#ifdef IK_DEBUG_FEATURE
    #define PROFILE() iKan::Profiler _profiler(__PRETTY_FUNCTION__);
#else
    #define PROFILE()
#endif

namespace iKan {
    
    /// Class to get the Profile time of the execution of a scope.
    /// Intantiate the Profiler with function name in any scope. It will print the executrion time of the function.
    /// Or just call the Macro defiled PROFILE().
    class Profiler
    {
    public:
        Profiler(const char* functionName);
        ~Profiler();
        
    private:
        std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint, m_EndTImepoint;
        std::chrono::duration<float> m_Duration;
        const char* m_FuncName;
    };
}
