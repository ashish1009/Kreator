//
//  Logger.hpp
//  iKan
//
//  Created by iKan on 19/05/22.
//

#pragma once

#include <spdlog/spdlog.h>

namespace iKan {
    
    /// Wrapper class for using spd logger. Initialize and create instance for both Core and Client Logger
    class Logger {
    public:
        /// IK Log Level enum
        enum Level { Trace, Debug, Info, Warning, Error, Critical };

        // Static member functions

        /// initialize spd logger. Create instance for both core and client
        /// @param coreLevel Core log Level
        /// @param clientLevel Client Log Level
        /// @param logFilePath Path relative to executable binary, where our log file to be saved.
        ///                    NOTE: Add '\' in the end.
        ///                    TODO: Change default argument file path when Asset manager will get introduced
        /// @param logFileName Log file name. NOTE: Without any extention.
        static void Init(Level coreLevel = Level::Trace, Level clientLevel = Level::Trace, const std::string& logFilePath = "../../Logs/", const std::string& logFileName = "iKan");
        
        /// Return the shared pointer of Core log instance
        static std::shared_ptr<spdlog::logger>& GetCoreLogger();
        
        /// Return the shared pointer of Client log instance
        static std::shared_ptr<spdlog::logger>& GetClientLogger();

    private:
        Logger() = default;
        MAKE_SINGLETON(Logger);
        static std::shared_ptr<spdlog::logger> s_CoreLogger, s_ClientLogger;
    };
    
}

#ifdef IK_ENABLE_LOG

// Client log macros
#define IK_TRACE(...) ::iKan::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define IK_DEBUG(...) ::iKan::Logger::GetClientLogger()->debug(__VA_ARGS__)
#define IK_INFO(...) ::iKan::Logger::GetClientLogger()->info(__VA_ARGS__)
#define IK_WARN(...) ::iKan::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define IK_ERROR(...) ::iKan::Logger::GetClientLogger()->error(__VA_ARGS__)
#define IK_CRITICAL(...) ::iKan::Logger::GetClientLogger()->critical(__VA_ARGS__)

#define IK_LOG_SEPARATOR() IK_CORE_TRACE("-------------------------------------------------");

#else

// Client log macros
#define IK_TRACE(...)
#define IK_DEBUG(...)
#define IK_INFO(...)
#define IK_WARN(...)
#define IK_ERROR(...)
#define IK_CRITICAL(...)

#define IK_LOG_SEPARATOR()

#endif

// Example for LOG API
// use {i} for printing any variable at ith position in arguament
// IK_INFO(" ... string ... {0}, {1} .... ", Arg0, Arg1 ...);
