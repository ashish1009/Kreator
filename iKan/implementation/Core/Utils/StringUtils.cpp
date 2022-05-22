//
//  StringUtils.cpp
//  iKan
//
//  Created by iKan on 22/05/22.
//

#include "StringUtils.hpp"

using namespace iKan;

/// Read the file and store in a string
/// @param path file path to be read
std::string StringUtils::ReadFromFile(const std::string& path) {
    std::string result;
    std::ifstream in(path, std::ios::in | std::ios::binary);
    if (in) {
        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        if (-1 != size) {
            result.resize(size);
            in.seekg(0, std::ios::beg);
            in.read(&result[0], size);
            in.close();
        }
        else {
            IK_CORE_ERROR("Could not read from file '{0}'", path);
        }
    }
    else {
        IK_CORE_ERROR("Could not open file '{0}'", path);
    }
    return result;
}

/// Extract the file name from file path
/// @param path file path for which file name need to be extracted
std::string StringUtils::GetNameFromFilePath(const std::string& path) {
    auto lastSlash = path.find_last_of("/\\");
    lastSlash      = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    
    auto lastDot = path.find_last_of('.');
    auto count   = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
    
    return path.substr(lastSlash, count);
}

/// Extract the file extension from file path
/// @param path file path for which file Extenstion need to be extracted
std::string StringUtils::GetExtensionFromFilePath(const std::string& path) {
    auto lastDot = path.find_last_of('.');
    lastDot      = lastDot == std::string::npos ? 0 : lastDot + 1;
    auto count   = path.size() - lastDot;
    return path.substr(lastDot, count);
}

/// Extract the file directory from file path
/// @param path file path for which file Extenstion need to be extracted
std::string StringUtils::GetDirectoryFromFilePath(const std::string& path) {
    return path.substr(0, path.find_last_of('/'));
}

/// Return the pointer of first occurance of token, token should be alone (space or nothing on left and right side)
/// NOTE: it is assumed that struct and uniform in the shader code should be alone eg. struct { } or uniform ...
/// @param str base string from which we need to search the token
/// @param token token to be searched from str
const char* StringUtils::FindToken(const char* str, const std::string& token) {
    const char* t = str;
    while ((t = strstr(t, token.c_str()))) {
        // Check if left character of token is empty
        // TODO: '/' might chnage if wont work for some scenario, using '/' as indicator of first character of line
        bool left = str == t || isspace(t[-1]) || t[-1] == '/';
        
        // Check if right character of token is empty
        bool right = !t[token.size()] || isspace(t[token.size()]);
        
        if (left && right)
            return t;
        
        t += token.size();
    }
    return nullptr;
}

/// Extract the Code statement before "}"
/// Also Update the string (outposition) just after the block
/// @param str token to be extracted from outPosition
/// @param outPosition string pointer of the file, that will move to some offset offset is the "}" of the strcut
std::string StringUtils::GetBlock(const char* str, const char** outPosition) {
    const char* end = strstr(str, "}");
    if (!end)
        return str;
    
    // offset the shader string to the end of current strcut
    if (outPosition)
        *outPosition = end;
    
    uint32_t length = (uint32_t)(end - str + 1);
    return std::string(str, length);
}

/// Extract the Code statement before ";"
/// @param str token to be extracted from outPosition
/// @param outPosition string pointer of the shader code, that will move to some offset offset is the ";" of the strcut
std::string StringUtils::GetStatement(const char* str, const char** outPosition) {
    const char* end = strstr(str, ";");
    if (!end)
        return str;
    
    if (outPosition)
        *outPosition = end;
    uint32_t length = (uint32_t)(end - str + 1);
    return std::string(str, length);
}

/// Break the string. Break points are stored in delimiter, any character in the delimeter will be cosnider as break points
/// @param string broken strings vector
/// @param delimiters (break point of string). From here string will be split
std::vector<std::string> StringUtils::SplitString(const std::string& string, const std::string& delimiters) {
    size_t start = 0;
    size_t end = string.find_first_of(delimiters);
    
    std::vector<std::string> result;
    
    while (end <= std::string::npos) {
        std::string token = string.substr(start, end - start);
        if (!token.empty())
            result.push_back(token);
        
        if (end == std::string::npos)
            break;
        
        start = end + 1;
        end = string.find_first_of(delimiters, start);
    }
    
    return result;
}

std::vector<std::string> StringUtils::SplitString(const std::string& string, const char delimiter) {
    return SplitString(string, std::string(1, delimiter));
}

std::vector<std::string> StringUtils::Tokenize(const std::string& string) {
    return SplitString(string, " \t\n");
}

std::vector<std::string> StringUtils::GetLines(const std::string& string) {
    return SplitString(string, "\n");
}
