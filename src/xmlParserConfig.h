
#pragma once
#include <string>
#include <vector>
#include <map>


class CallbackConfigs
{
    public:
        std::string pathToFollow;
        std::vector<std::string> attributesToReport;
        bool greater;
        int trigger;
        bool debug;
        bool testmode;
};

class ParserConfigs
{
    public:
        std::string filename;
        std::string fileContents;
        bool printMode;
        std::map<std::string, CallbackConfigs> cbs;
        bool debug = false;
        bool testmode = false;
        int callbackReportMode;
};