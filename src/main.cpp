#include <iostream>
#include <fstream>
#include <stack>
#include <sstream>
#include "xmlParser.h"
#include "xmlUtils.h"

std:: string readFileToString( std::string& filepath){
    std::ifstream file(filepath);
    if(!file.is_open()){
        printf("File failed to open");
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
int main(int argc, char *argv[])
{
    std::string filename;
    if (argc < 2)//<2 Args should prompt user for an xml file
    {
        printf("Please provide the XML file to parse or leave blank to use default file\n");
        getline(std::cin, filename);
        if (filename.empty())
        {
            printf("No file provided, using default file\n");
            filename = "xmlTestFiles/default.xml";
        }
    }
    else if (argc == 2)
    {
        printf("Parsing XML file: %s\n", argv[1]);
        // Open the XML file
        filename = argv[1];
    }
    else // >2 Args is too many arguments for now
    {
        printf("Too many arguments provided. Please try again \n");
        return 1;
    }
    std::string filecontents = readFileToString(filename);
    if(filecontents.empty()){
        std::cout<< "Empty File or Error reading file "<<filename;
    }
    //else std::cout<<filecontents<<"\n";
    xmlParser::xmlParser xmlParser;
    xmlParser.setPrintMode(false);
    std::string pathTofollow = "*/order/amount";


    ///This should be refactored to be..idk easier. And preferably all of the stuff above is passed to the xmlParser file or the util file. IDK definitely tomorrows prob. 
    xmlParser.addCallBack(pathTofollow,[](const xmlParser::xmlNode& node){
        if(xmlUtils::pathEndsIn(node.path, "order/amount") && xmlUtils::parseIfInteger(node.text)>100){
            auto it = node.parent->attributes.find("id");
            if(it != node.parent->attributes.end()){
                std::cout<< "Order "<<it->first<<":" << it->second << " has amount "<<node.text<<" which is greater than 100.\n";
            }
        }
    });

    xmlParser.parse(filecontents);
    

   
}