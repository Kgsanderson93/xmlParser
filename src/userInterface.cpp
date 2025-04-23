#include "userInterface.h"
#include "xmlUtils.h"
#include <iostream>
#include <sstream>
#include <climits>

void UserInterface::fillParserConfigs(ParserConfigs &configs)
{
    configs.filename = promptFileSelection();
    configs.fileContents = xmlUtils::readFileToString(configs.filename);
    configs.printMode = promptPrintMode();
    configs.callbackReportMode = promptCallbackMode();
}

CallbackConfigs UserInterface::collectCallbackConfig()
{
    CallbackConfigs cb;
    cb.pathToFollow = promptPathToFollow();
    cb.attributesToReport = promptAttributes();

    auto [greater, trigger] = promptTrigger(xmlUtils::getLastPathComponent(cb.pathToFollow));
    cb.greater = greater;
    cb.trigger = trigger;
    return cb;
}

bool UserInterface::userWantsToAddCallback()
{
    std::cout << "Would you like to add a new callback configuration?\n- 1 yes\n- 2 no\n";
    std::string input;
    std::getline(std::cin, input);
    return input == "1" || input == "yes" || input == "y";
}

void UserInterface::reportResults(
    const std::vector<Xml_Parser::xmlNode> &sharedVector,
    const std::map<std::string, std::vector<Xml_Parser::xmlNode>> &separateVectors,
    const ParserConfigs &configs
) {
    if (configs.callbackReportMode == 2) {
        std::cout << "Total nodes matched across all callbacks: " << sharedVector.size() << "\n";
        for (const auto &node : sharedVector) {
            bool foundCB = false;
            CallbackConfigs useCB;
            for (const auto &[_, cb] : configs.cbs) {
                if (xmlUtils::matchPath(cb.pathToFollow, node.path)) {
                    useCB = cb;
                    foundCB = true;
                    break;  // stop once we find the first matching CB
                }
            }
            if (foundCB) {
                printCallBackNode(node, useCB);
            } else {
                std::cerr << "Failed to find correct callback config\n";
            }
        }
    }
    else if (configs.callbackReportMode  == 3) {
        for (const auto &[path, vec] : separateVectors) {
            std::cout << "Path '" << path << "' matched " << vec.size() << " nodes.\n";
            for (const auto &node : vec) {
                auto cbIt = configs.cbs.find(path);
                if (cbIt != configs.cbs.end()) {
                    printCallBackNode(node, cbIt->second);
                } else {
                    std::cerr << "No config found for path: " << path << "\n";
                }
            }
        }
    }
}


// PRIVATE METHODS

std::string UserInterface::promptFileSelection()
{
    std::cout << "Please select XML file to run \n- 1 default.xml\n- 2 books.xml\n- 3 provide your own XML file\n";
    std::string command;
    std::getline(std::cin, command);
    if (command == "1")
        return "xmlTestFiles/default.xml";
    if (command == "2")
        return "xmlTestFiles/books.xml";

    std::cout << "Enter path/filename now\n";
    std::getline(std::cin, command);
    return command.empty() ? "xmlTestFiles/default.xml" : command;
}

bool UserInterface::promptPrintMode()
{
    std::cout << "Would you like print mode enabled during parsing?\n- 1 yes\n- 2 no\n";
    std::string response;
    std::getline(std::cin, response);
    return response.empty() || response == "1";
}

int UserInterface::promptCallbackMode()
{
    std::cout << "Would you like your callbacks to print or return a node to a vector?\n"
              << "- 1 print\n- 2 vector (shared)\n- 3 vector (separate for each)\n";
    std::string command;
    std::getline(std::cin, command);
    int mode = xmlUtils::parseIfDouble(command);
    if (mode >= 1 && mode <= 3){
        return mode;
    }else{
    std::cerr << "Invalid callback report mode entered. Defaulting to print mode.\n";
    return 1;
    }
}

std::string UserInterface::promptPathToFollow()
{
    std::cout << "Please enter a path to the node you'd like a callback for (e.g., */order/amount or *)\n";
    std::string path;
    std::getline(std::cin, path);
    path = xmlUtils::trim(path);
    return path.empty() ? "*" : path;
}

std::vector<std::string> UserInterface::promptAttributes()
{
    std::cout << "Enter attributes to report (comma-separated): \n";
    std::string attr;
    std::getline(std::cin, attr);

    std::stringstream ss(attr);
    std::string token;
    std::vector<std::string> attrs;
    while (std::getline(ss, token, ','))
    {
        attrs.push_back(xmlUtils::trim(token));
    }
    return attrs;
}

std::pair<bool, int> UserInterface::promptTrigger(const std::string &nodeName)
{
    std::cout << "Should " << nodeName << " be greater or less than a trigger value?\n- 1 greater\n- 2 less than\n";
    std::string input;
    std::getline(std::cin, input);
    bool greater = (input == "1");

    int triggerVal = INT_MIN;
    while (triggerVal == INT_MIN)
    {
        std::cout << "Enter a valid double trigger value: \n";
        std::getline(std::cin, input);
        triggerVal = xmlUtils::parseIfDouble(xmlUtils::trim(input));
        if (triggerVal == INT_MIN)
        {
            std::cerr << "Invalid input. Please enter an double.\n";
        }
    }
    return {greater, triggerVal};
}

void UserInterface::printCallBackNode(const Xml_Parser::xmlNode& node,const CallbackConfigs& config)
{
    std::cout << "Callback matched: " << node.parent->name<<" at path "<<node.path << " with attributes: \n";
    // Print only specified attributes in config

    if (config.debug)
    {
        std::cout << "I am looking for " << config.attributesToReport.size() << " attributes\n";
        for (std::string attrname : config.attributesToReport)
        {
            std::cout << attrname << "\n";
        }
        std::cout << "This order has " << node.parent->attributes.size() << " attributes\n";
        for (const auto &attr : node.attributes)
        {
            std::cout << "- " << attr.first << " = " << attr.second << "\n";
        }
    }
    for (std::string attrName : config.attributesToReport)
    {
        auto it = node.parent->attributes.find(attrName);
        if (it != node.parent->attributes.end())
        {
            std::cout << "- " << it->first << " = " << it->second << "\n";
        }
    }
    std::cout << "With " << node.name << " = " << node.text << "\n";
    std::string condition = config.greater ? "greater" : "less";
    std::cout << "On condition " << node.name << " " << condition << " than " << config.trigger << "\n";
}

