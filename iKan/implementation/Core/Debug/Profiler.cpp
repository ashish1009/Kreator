//
//  Profiler.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "Profiler.hpp"
#include "LoggerCoreAPI.h"

using namespace iKan;

/// Construct Profiler and store time stamp of Scope starting
/// @param functionName function name
Profiler::Profiler(const char* functionName) : m_FuncName(functionName) {
    m_StartTimepoint = std::chrono::high_resolution_clock::now();
}

/// Destructor of profiler. get the time taken by scope operation
Profiler::~Profiler() {
    m_EndTImepoint = std::chrono::high_resolution_clock::now();;
    m_Duration = m_EndTImepoint - m_StartTimepoint;
    
    IK_CORE_TRACE(" '{0}': {1} sec, {2} ms, {3} us", m_FuncName, m_Duration.count(), m_Duration.count() * 1000, m_Duration.count() * 1000000);
}
