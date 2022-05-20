//
//  Profiler.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

#ifdef IK_DEBUG_FEATURE
    #define PROFILE() iKan::Profiler _profiler(__PRETTY_FUNCTION__);
#else
    #define PROFILE()
#endif

namespace iKan {

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
