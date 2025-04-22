#pragma once
#include <string>
#include <stack>
#include <map>
#include <functional>
#include <memory>
#include <vector>

namespace Xml_Parser
{

    struct xmlNode : public std::enable_shared_from_this<xmlNode>
    {
        std::string name;
        size_t startIdx;
        size_t endIdx;
        std::string path;
        std::string text;
        std::map<std::string, std::string> attributes;
        xmlNode* parent =nullptr;         
        std::vector<std::shared_ptr<xmlNode>> children;

        
    // Default constructor
    xmlNode() : name(""), startIdx(0), endIdx(0), path(""), text("") {}
    
    // Constructor with node name
    xmlNode(const std::string& nodeName) : name(nodeName), startIdx(0), endIdx(0), path(""), text("") {}

    
    };

    using NodeCallBack = std::function<void(const xmlNode &)>;

    class xmlParser
    {
    public:
        bool printMode = true;
        void setPrintMode(bool onOff);
        int parse(const std::string &xml);
        void addCallBack(std::string& path, NodeCallBack cb);
        void setCallBacks(std::map<std::string, NodeCallBack>);
        const std::shared_ptr<xmlNode>& getRoot() const;
        

    private:
        std::shared_ptr<xmlNode> root;
        xmlNode *current = nullptr;
        std::map<std::string, NodeCallBack> callbacks;
    };
}