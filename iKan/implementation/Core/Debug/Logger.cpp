//
//  Logger.cpp
//  iKan
//
//  Created by iKan on 19/05/22.
//

#include "Logger.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

using namespace iKan;

// Static globals
static const std::string g_LogFileExtension = ".iKanLog"; // Log  file extention.

// Static Member variable Declaration and Initialisation
std::shared_ptr<spdlog::logger> Logger::s_CoreLogger = nullptr;
std::shared_ptr<spdlog::logger> Logger::s_ClientLogger = nullptr;

// Log Utils API
namespace LoggerUtils {
    
    /// Convert SPD Log Level type to string for Printing
    /// @param level SPD Log level
    static const char* GetLogLevelStringFromSpdLevel(spdlog::level::level_enum level) {
        switch (level) {
            case spdlog::level::level_enum::trace : return ToString(Trace);
            case spdlog::level::level_enum::debug : return ToString(Debug);
            case spdlog::level::level_enum::info : return ToString(Info);
            case spdlog::level::level_enum::warn : return ToString(Warning);
            case spdlog::level::level_enum::err : return ToString(Error);
            case spdlog::level::level_enum::critical : return ToString(Critical);
            case spdlog::level::level_enum::off : return ToString(Off);
            case spdlog::level::level_enum::n_levels : return ToString(No Level);
            default: break;
        }
        return "No Level";
    }
    
    /// Convert IK Log level to SPD Log Level
    /// @param level IK Log Level
    static spdlog::level::level_enum GetSpdLevelFromKreatorLevel(Logger::Level level) {
        switch (level) {
            case Logger::Level::Trace: return spdlog::level::level_enum::trace;
            case Logger::Level::Debug: return spdlog::level::level_enum::debug;
            case Logger::Level::Info: return spdlog::level::level_enum::info;
            case Logger::Level::Warning: return spdlog::level::level_enum::warn;
            case Logger::Level::Error: return spdlog::level::level_enum::err;
            case Logger::Level::Critical: return spdlog::level::level_enum::critical;
            default:
                IK_CORE_ASSERT(false, "Invalid level");
        }
    }
}

/// initialize spd logger. Create instance for both core and client
/// @param coreLevel Core log Level
/// @param clientLevel Client Log Level
/// @param logFilePath Path relative to executable binary, where our log file to be saved.
///                    NOTE: Add '/' in the end otherwise last directory will be appended in file name
/// @param logFileName Log file name. NOTE: Without any extention
void Logger::Init(Level coreLevel, Level clientLevel, const std::string& logFilePath, const std::string& logFileName) {
    std::cout << "Initialising SPD Logger ..." << std::endl;
    
    std::vector<spdlog::sink_ptr> logSinks;
    logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    
    std::string logFileFullPath = logFilePath + "/" + logFileName + g_LogFileExtension;
    logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFileFullPath.c_str(), true));
    std::cout << "    Saving Logs at path '" << logFileFullPath.c_str() << "'" << std::endl;
    
    logSinks[0]->set_pattern("[%T:%e:%f] [%-8l] [%-4n] : %v");
    logSinks[1]->set_pattern("[%T:%e:%f] [%-8l] [%-4n] : %v");
    
    s_CoreLogger = std::make_shared<spdlog::logger>("IKAN ", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_CoreLogger);
    
    auto spdCoreLevel = LoggerUtils::GetSpdLevelFromKreatorLevel(coreLevel);
    std::cout << "    Intiializing SPD Logger for Core with log level : '" << LoggerUtils::GetLogLevelStringFromSpdLevel(spdCoreLevel) << "'" << std::endl;
    
    s_CoreLogger->set_level(spdCoreLevel);
    s_CoreLogger->flush_on(spdCoreLevel);
    
    s_ClientLogger = std::make_shared<spdlog::logger>("APP  ", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_ClientLogger);
    
    auto spdClientLevel = LoggerUtils::GetSpdLevelFromKreatorLevel(clientLevel);
    std::cout << "    Intiializing SPD Logger for Client with log level : '" << LoggerUtils::GetLogLevelStringFromSpdLevel(spdClientLevel) << "'" << std::endl;
    
    s_ClientLogger->set_level(spdClientLevel);
    s_ClientLogger->flush_on(spdClientLevel);
    
    std::cout << " ------------------------------------------------------------- " << std::endl;
}

// ---------------- SPD Instance Getters --------------------
std::shared_ptr<spdlog::logger>& Logger::GetCoreLogger() { return s_CoreLogger; }
std::shared_ptr<spdlog::logger>& Logger::GetClientLogger() { return s_ClientLogger; }
// ----------------------------------------------------------
