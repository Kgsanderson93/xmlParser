#pragma once
#include "xmlParserConfig.h"
#include "xmlParser.h"

class UserInterface
{
public:
    static void fillParserConfigs(ParserConfigs &configs);
    static CallbackConfigs collectCallbackConfig();
    static bool userWantsToAddCallback();
    static void reportResults(
        const std::vector<Xml_Parser::xmlNode> &sharedVector,
        const std::map<std::string, std::vector<Xml_Parser::xmlNode>> &separateVectors,
        const ParserConfigs &configs);

private:
    static std::string promptFileSelection();
    static bool promptPrintMode();
    static int promptCallbackMode();
    static std::string promptPathToFollow();
    static std::vector<std::string> promptAttributes();
    static std::pair<bool, int> promptTrigger(const std::string &lastPathComponent);
    static void printCallBackNode(const Xml_Parser::xmlNode& node, const CallbackConfigs& configs);
};