//
//  LoggerCoreAPI.h
//  iKan
//
//  Created by iKan on 19/05/22.
//

#pragma once

// Includes API for Logging only for Core Engine not visible to client

#include "Core/Debug/Logger.hpp"

#ifdef IK_ENABLE_LOG

// Core log macros
#define IK_CORE_TRACE(...) ::iKan::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define IK_CORE_DEBUG(...) ::iKan::Logger::GetCoreLogger()->debug(__VA_ARGS__)
#define IK_CORE_INFO(...) ::iKan::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define IK_CORE_WARN(...) ::iKan::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define IK_CORE_ERROR(...) ::iKan::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define IK_CORE_CRITICAL(...) ::iKan::Logger::GetCoreLogger()->critical(__VA_ARGS__)

#define IK_LOG_SEPARATOR() IK_CORE_TRACE("-------------------------------------------------");

#else

// Core log macros
#define IK_CORE_TRACE(...)
#define IK_CORE_DEBUG(...)
#define IK_CORE_INFO(...)
#define IK_CORE_WARN(...)
#define IK_CORE_ERROR(...)
#define IK_CORE_CRITICAL(...)

#endif
