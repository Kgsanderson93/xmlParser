#include "xmlParser.h"
#include <iostream>
#include <vector>
#include "xmlUtils.h"

namespace Xml_Parser
{
    const std::unique_ptr<xmlNode>& xmlParser::getRoot() const {
        return this->root;
    }
    /**
     * This is mostly used for debugging. If this is on the structural parts of the xml file will be printed during parsing.
     */
    void xmlParser::setPrintMode(bool onOff)
    {
        this->printMode = onOff;
    }

    void xmlParser::addCallBack(std::string& path, NodeCallBack cb)
    {
        if(!path.empty()){
        callbacks[path] = cb;
        }else{
            callbacks["*"]= cb;
        }
    }

    /**
     * This function is the root of the xmlParser class. It takes in an string which represents an entire xml file and creates a tree structure to reprsent the xml file tags.
     */
    int xmlParser::parse(const std::string &xml)
    {
        std::stack<std::string> tagStack;
        size_t i = 0;

        root = std::make_unique<xmlNode>();
        root->name = "EmptyRoot";
        root->path = "";
        current = root.get();

        while (i < xml.size())
        {
            if (xml[i] == '<')
            {
                if (xml.compare(i, 4, "<!--") == 0)
                {
                    // This is a comment, must now find end of comment and skip everything between
                    size_t end = xml.find("-->", i);
                    if (end == std::string::npos)
                    {
                        break;
                    }
                    else
                    {
                        if (this->printMode)
                        {
                            std::string comment = xml.substr(i, end + 3 - i);
                            std::cout << "Comment: " << comment << "\n";
                        }
                        i = end + 3;
                        continue;
                    }
                }
                if (xml[i + 1] == '/')
                { // this is likely an end tag. Lets check.
                    i += 2;
                    size_t end = xml.find('>', i);
                    std::string name = xml.substr(i, end - i);
                    // std::cout << "End tag: </" << name << ">\n";
                    if (tagStack.empty()) // if our stack is empty and we find an end tag this is likely not ok. Alert.
                    {
                        std::cerr << "Error: closing tag </" << name << "> without a matching opening tag.\n";
                        return -1;
                    }
                    else if (tagStack.top() != name) // if the top of our stack does not match the end tag we found this is likely an error. Alert.
                    {
                        std::cerr << "Error: closing tag </" << name << "< does not match last opening tag <" << tagStack.top() << ">  \n";
                        return -1;
                    }
                    else // No issues found tag now closed.
                    {
                        i = end + 1;
                        if (this->printMode)
                        {
                            std::cout << "Closing tag :</" << name << ">\n"; //" successful current index is "<<i << "Char at index is "<< xml[i];
                        }
                        current->endIdx = end;
                        if(!callbacks.empty()){
                            for(const auto& [pattern, cb]: callbacks){
                                if(xmlUtils::matchPath(pattern, current->path)){
                                    cb(*current);
                                }
                            }
                        }
                        if (current->name != "EmptyRoot" && current->parent != nullptr)
                        {

                            current = current->parent; // move back to parent
                        }
                        tagStack.pop();
                    }
                }
                else
                {
                    i++;
                    size_t start = i;
                    while (i < xml.size() && xml[i] != '>' && xml[i] != ' ')
                    {
                        i++;
                    }
                    std::string name = xml.substr(start, i - start);
                    if (this->printMode)
                    {
                        std::cout << "Starting tag <" << name << "> \n";
                    }
                    auto newNode = std::make_unique<xmlNode>();
                    newNode->name = name;
                    newNode->parent = current;
                    if(current->name=="EmptyRoot"){
                        newNode->path= name;
                    }else{
                    newNode->path = current->path + "/" + name;
                    }
                    newNode->startIdx = start;
                    current->children.push_back(std::move(newNode));
                    current = current->children.back().get();

                    tagStack.push(name);

                    while (i < xml.size() && xml[i] != '>')
                    {
                        while (i < xml.size() && isspace(xml[i]))
                            i++;
                        size_t attStart = i;
                        while (i < xml.size() && xml[i] != '=' && xml[i] != '>')
                            i++;
                        if (i > xml.size() || xml[i] == '>')
                            break;

                        std::string attrName = xml.substr(attStart, i - attStart);
                        i++;
                        if (i < xml.size() && xml[i] == '"')
                        {
                            i++;
                            size_t valStart = i;
                            while (i < xml.size() && xml[i] != '"')
                                i++;
                            std::string attrVal = xml.substr(valStart, i - valStart);
                            i++;
                            attrVal=xmlUtils::decodeEnt(xmlUtils::trim(attrVal));
                            current->attributes[attrName]=attrVal;
                            if (this->printMode)
                            {

                                std::cout << "    Attribute: " << attrName << " = " << attrVal << "\n";
                            }
                        }
                    }
                    if (i < xml.size() && xml[i] == '>')
                        i++;
                }
            }
            else
            {
                size_t start = i;
                while (i < xml.size() && xml[i] != '<')
                    i++;
                std::string text = xml.substr(start, i - start);
                // if(!text.empty() &&!isWhiteSpaceOnly(text)){
                // std::cout<< "\nTESTING :: "<<text<<"\n\n";}
                if (!text.empty() && !xmlUtils::isWhiteSpaceOnly(text) && !tagStack.empty())
                {
                    std::string path = xmlUtils::buildPath(tagStack);
                    text = xmlUtils::decodeEnt(xmlUtils::trim(text));
                    current->text = text;
                    if (this->printMode)
                    {

                        std::cout << "Text at " << path << ": " << text  << "\n"; //" TESTING DECODE: "<<decodeEnt(text)<<" TESTING TRIM "<< trim(text)<<"\n";
                    }
                }
            }
        }
        return 0;
    }
}
