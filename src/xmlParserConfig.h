
#pragma once
#include <string>
#include <vector>
#include <map>


class CallbackConfigs
{
    public:
        std::string pathToFollow;
        //This demo has us reporting an attribute, but we could test an attribute and print a value. Ex order # greater than 20 print their associated amounts. The callback being under the user's control means that this config should also be under their control. 
        std::vector<std::string> attributesToReport;
        bool greater;
        //Currently only using an int trigger but in theory could also be string but would then need an enum or something to say I want to see if a string matches or part of a string or i want a value ><= to some value. For this demo we're sticking with ints but really it could be anything
        double trigger;
        bool debug;
        bool testmode;
};

class ParserConfigs
{
    public:
        //may not need to store filename and contents but doesn't currently hurt.
        std::string filename;
        std::string fileContents;
        /*If tags should be printed as they are encountered.*/
        bool printMode;
        /*List of callbacks and their paths to be registered with parser*/
        std::map<std::string, CallbackConfigs> cbs;
        //There is likely a better way to do this but since this was all refactored out of main where debug had been a global var this was easier.
        bool debug = false;
        bool testmode = false;
        /*1 = print node if callback succeeds, 2= send node to shared vector 3=send to seperate vector */ //This may be nice to make an enum
        int callbackReportMode;
};