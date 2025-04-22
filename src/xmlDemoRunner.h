#pragma once
#include "xmlParserConfig.h"
#include "xmlParser.h"

class xmlDemoRunner {
    public:
        void run();
        void runDefault();
        bool testMode;
        bool debug;
        ParserConfigs pConfigs;
        std::vector<Xml_Parser::xmlNode> sharedVector;
        std::map<std::string, std::vector<Xml_Parser::xmlNode>> separateVectors;

   
};