#include <iostream>
#include <fstream>
#include <stack>
#include <sstream>
#include "xmlParser.h"
#include "xmlUtils.h"
#include <climits> // For INT_MIN

#ifdef _MSC_VER
    #define FOLD_REGION(name) __pragma(region name)
    #define END_FOLD_REGION   __pragma(endregion)
#else
    #define FOLD_REGION(name)
    #define END_FOLD_REGION
#endif



std::string readFileToString(std::string &filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        printf("File failed to open");
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
void rundefaultdemo()
{
    std::string filename = "xmlTestFiles/default.xml";
    std::string filecontents = readFileToString(filename);
    if (filecontents.empty())
    {
        std::cout << "Empty File or Error reading file " << filename;
    }
    Xml_Parser::xmlParser xmlParser;
    xmlParser.setPrintMode(true);
    std::string pathTofollow = "*/order/amount";
    /// This should be refactored to be..idk easier. And preferably all of the stuff above is passed to the xmlParser file or the util file. IDK definitely tomorrows prob.
    xmlParser.addCallBack(pathTofollow, [](const Xml_Parser::xmlNode &node)
                          {
        if(xmlUtils::pathEndsIn(node.path, "order/amount") && xmlUtils::parseIfInteger(node.text)>100){
            auto it = node.parent->attributes.find("id");
            if(it != node.parent->attributes.end()){
                std::cout<< "\n\nOrder "<<it->first<<":" << it->second << " has amount "<<node.text<<" which is greater than 100.\n\n";
            }
        } });

    xmlParser.parse(filecontents);
}

int main(int argc, char *argv[])
{
FOLD_REGION("getXMLfile")
    std::string filename;
    
    std::string command;
    if (argc < 2) //<2 Args should prompt user for an xml file
    {
        printf("Please select xml file to run \n- 1 default.xml\n- 2 books.xml\n- 3 provide your own xml file\n- 4 run default demo with no further args.\n");
        getline(std::cin, command);
        if (command == "4")
        {
            rundefaultdemo();
            return 0; // this runs the default demo then exits the program. Made to quickly test if changes broke anything.
        }
        else if (command == "1")
        {
            filename = "xmlTestFiles/default.xml";
        }
        else if (command == "2")
        {
            filename = "xmlTestFiles/books.xml";
        }
        else if (command == "3")
        {
            printf("Enter path/filename now\n");
            getline(std::cin, filename);
        } // this is not else if'ed on purpose since command 3 allows users to enter their own file name, if they select 3 and then fail to provide a filename we will default back to the default file
        if (filename.empty())
        {
            printf("No file provided, using default file\n");
            filename = "xmlTestFiles/default.xml";
        }
    }
    else if (argc == 2) // theoretically you can simply provide the file name as a command line arg. This may prove beneficial if I set up a testing suite.
    {
        printf("Parsing XML file: %s\n", argv[1]);
        // Open the XML file
        filename = argv[1];
    }
    else // >2 Args is too many arguments for now may edit this to be a file that reads in a callback?
    {
        printf("Too many arguments provided. Please try again \n");
        return 1;
    }

    std::string filecontents = readFileToString(filename);
    if (filecontents.empty())
    {
        std::cout << "Empty File or Error reading file " << filename;
    }
END_FOLD_REGION
FOLD_REGION( "CallbackSetUp")
    Xml_Parser::xmlParser xmlParser;

    printf("Would you like print mode enabled during parsing? \n- 1 yes\n- 2 no\n");
    getline(std::cin, command);
    if (command.empty() || command == "1")
    {
        xmlParser.setPrintMode(true);
    }
    else if (command == "2")
    {
        xmlParser.setPrintMode(false);
    }

    printf("Would you like to set up a callback? Only simple callbacks can be configured in demo(following the predefined callbackmodel), please edit file for more in depth callbacks. \n- 1 yes \n- 2 no\n");
    getline(std::cin, command);
    std::string pathTofollow;
    std::string attributeToReport;
    if (command == "1")
    {
        printf("Please enter a path to the node you'd like a callback associated with.\n NOTE: Currently only front wildcards and total wildcard are supported ex: '*/order/amount' vs. 'root/order/amount' or '*'\n");
        getline(std::cin, pathTofollow);
        pathTofollow= xmlUtils::trim(pathTofollow);
        std::cout<<"I read: "<<pathTofollow<<"\n";
        while (pathTofollow.empty())
        { // You must enter a path or exit program. (I could maybe check if you are still using default and if so go back to default cb but honestly, you got this far. Just put in a * if you're lost.)
            printf("Path cannot be empty, if you would like to check every node please enter a wildcard or path \n");
            getline(std::cin, pathTofollow);
        }
        printf("Please enter the attribute you would like reported (attribute must be associated with parent node currently following the example of order id for amounts greater than 100)\n");
        getline(std::cin, attributeToReport);
        attributeToReport=xmlUtils::trim(attributeToReport);
        std::cout<<"I read: "<<attributeToReport<<"\n";
        
        if (attributeToReport.empty())
        {
            printf("All attributes will be reported\n");
        }
        printf("Please enter if trigger value should be greater or less than \n- 1 greater \n- 2 less than\n");
        bool greater = false;
        getline(std::cin, command);
        if (command == "1")
        {
            greater = true;
        } // all other responses trigger false. I don't see the benefit of looping here.
        std::cout<<"Greater is currently "<<std::boolalpha<<greater<<"\n";
        printf("Please enter a trigger value\n");
        getline(std::cin, command);
        int triggerVal;
        triggerVal = xmlUtils::parseIfInteger(xmlUtils::trim(command));
        std::cout<<"I read: "<<triggerVal<<"\n";
        while (triggerVal == INT_MIN)
        {
            printf("Only integers may be entered currently. Please try again\n");
            getline(std::cin, command);
            triggerVal = xmlUtils::parseIfInteger(xmlUtils::trim(command));
        }
        
        pathTofollow= xmlUtils::normalizePath(pathTofollow);
       


        xmlParser.addCallBack(pathTofollow, [pathTofollow, triggerVal, greater, attributeToReport](const Xml_Parser::xmlNode &node)
                              {
            if (
                xmlUtils::matchPath( pathTofollow, node.path) &&
                (
                    (greater && xmlUtils::parseIfInteger(node.text) > triggerVal) ||
                    (!greater && xmlUtils::parseIfInteger(node.text) < triggerVal)
                )
            )
            {
                std::string gOrL = greater ? "greater" : "less";
        
                if (!attributeToReport.empty())
                {
                    auto it = node.parent->attributes.find(attributeToReport);
                    if (it != node.parent->attributes.end())
                    {
                        std::cout << "\n\n" << node.parent->name << " " << it->first << ": " << it->second
                                  << " has a/an " << node.name << " = " << node.text
                                  << " which is " << gOrL << " than " << triggerVal << ".\n\n";
                    }
                }
                else
                {
                    std::cout << "\n\nA/an " << node.parent->name << " found with " << node.name << " = "
                              << node.text << " which is " << gOrL << " than " << triggerVal << "\n";
                    std::cout << "Attributes of " << node.parent->name << " are:\n";
                    for (const auto &attr : node.parent->attributes)
                    {
                        std::cout << "  " << attr.first << " = " << attr.second << "\n";
                    }
                    std::cout << "\n";
                }
            } });

            xmlParser.parse(filecontents);
            return 0;
    }
    else if (command == "2")
    {
        printf("Not setting a callback results in default callback which is only applicable to default.xml, if a different xml file was provided this will likely result in nothing reported.\n");
    
    pathTofollow = "*/order/amount";
    pathTofollow= xmlUtils::normalizePath(pathTofollow);

    /// This should be refactored to be..idk easier. And preferably all of the stuff above is passed to the xmlParser file or the util file. IDK definitely tomorrows prob.
    xmlParser.addCallBack(pathTofollow, [pathTofollow](const Xml_Parser::xmlNode &node)
                          {
                            std::cout<<node.path;
        if(xmlUtils::matchPath( pathTofollow, node.path) && xmlUtils::parseIfInteger(node.text)>100){
            auto it = node.parent->attributes.find("id");
            if(it != node.parent->attributes.end()){
                std::cout<< "\n\nOrder "<<it->first<<":" << it->second << " has amount "<<node.text<<" which is greater than 100.\n\n";
            }
        } });
    }
END_FOLD_REGION
    xmlParser.parse(filecontents);
}