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
        /// Read a file and return the file data as string
        /// @param path path of file
        static std::string ReadFromFile(const std::string& path);
        /// Return the file name from path
        /// @param path path of file
        static std::string GetNameFromFilePath(const std::string& path);
        /// return the Extention of File from path
        /// @param path path of file
        static std::string GetExtensionFromFilePath(const std::string& path);
        /// get the directory from file path. It exclude the file name with extension from path
        /// @param path path of file
        static std::string GetDirectoryFromFilePath(const std::string& path);
        
        /// Find a word/token from string and return the char pointer
        /// @param str base tring from which token need to be searched
        /// @param token toke to be searched
        static const char*FindToken(const char* str, const std::string& token);
        /// Find a block of string from string and return the char pointer
        /// @param str base tring from which block need to be searched
        /// @param outPosition position of start of block as char pointer
        static std::string GetBlock(const char* str, const char** outPosition);
        /// Find a statement of string from string and return the char pointer
        /// @param str base tring from which block need to be searched
        /// @param outPosition position of start of statement as char pointer
        static std::string GetStatement(const char* str, const char** outPosition);
        
        /// Split the string when it finds the delimiter
        /// @param string string which need to be split
        /// @param delimiters deliminater from which string need to be split
        static std::vector<std::string> SplitString(const std::string& string, const std::string& delimiters);
        /// Split the string when it finds the delimiter
        /// @param string string which need to be split
        /// @param delimiter deliminater from which string need to be split
        static std::vector<std::string> SplitString(const std::string& string, const char delimiter);
        /// Split the string when it finds either '\t' or '\n'
        /// @param string string which need to be split
        static std::vector<std::string> Tokenize(const std::string& string);
        /// Split the string when it finds either '\n'
        /// @param string string which need to be split
        static std::vector<std::string> GetLines(const std::string& string);

    private:
        StringUtils() = default;
    };
    
}
