#pragma once
#include "xmlParserConfig.h"
#include "xmlParser.h"

class xmlDemoRunner {
    public:
        void run();
        void runDefault();
        void RunCustomDemoWithConfigs(ParserConfigs& configs);
        bool testMode;
        bool debug;
        ParserConfigs pConfigs;
        std::vector<Xml_Parser::xmlNode> sharedVector;
        std::map<std::string, std::vector<Xml_Parser::xmlNode>> separateVectors;
        void setupCallback(Xml_Parser::xmlParser &xmlParser, CallbackConfigs &config, bool store, std::vector<Xml_Parser::xmlNode> &matchedNodes);

   
};