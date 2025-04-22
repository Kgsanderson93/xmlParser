#pragma once
#include <string>
#include <stack>
#define MAXENTITIES 5

namespace xmlUtils{
    //Utility Functions
    std::string readFileToString(std::string &filepath);
    std::string getLastPathComponent(const std::string& path);
    std::string normalizePath(const std::string& rawPath);
    bool matchPath(const std::string& pattern, const std::string& path);
    int parseIfInteger(const std::string& str);
    bool pathEndsIn(std::string nodePath, std::string endsin);
    bool isWhiteSpaceOnly(const std::string &str);
    std::string trim(const std::string &str);
    std::string buildPath(std::stack<std::string>& tagstack);
    std::string decodeEnt(const std::string& input);
    

}