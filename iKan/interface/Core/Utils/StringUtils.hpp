//
//  StringUtils.hpp
//  iKan
//
//  Created by iKan on 22/05/22.
//

#pragma once

namespace iKan {
    
    class StringUtils {
    public:
        static std::string ReadFromFile(const std::string& path);
        static std::string GetNameFromFilePath(const std::string& path);
        static std::string GetExtensionFromFilePath(const std::string& path);
        static std::string GetDirectoryFromFilePath(const std::string& path);
        
        static const char*FindToken(const char* str, const std::string& token);
        static std::string GetBlock(const char* str, const char** outPosition);
        static std::string GetStatement(const char* str, const char** outPosition);
        
        static std::vector<std::string> SplitString(const std::string& string, const std::string& delimiters);
        static std::vector<std::string> SplitString(const std::string& string, const char delimiter);
        static std::vector<std::string> Tokenize(const std::string& string);
        static std::vector<std::string> GetLines(const std::string& string);

    private:
        StringUtils() = default;
    };
    
}
