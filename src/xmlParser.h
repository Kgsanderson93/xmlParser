#pragma once
#include <string>
#include <stack>
#include <map>
#include <functional>
#include <memory>

namespace xmlParser
{

    struct xmlNode
    {
        std::string name;
        size_t startIndx;
        size_t endIdx;
        std::string path;
        std::string text;
        std::map<std::string, std::string> attributes;
        xmlNode *parent = nullptr;
        std::vector<std::unique_ptr<xmlNode>> children;
    };

    using NodeCallBack = std::function<void(const xmlNode &)>;

    class xmlParser
    {
    public:
        bool printMode = true;
        void setPrintMode(bool onOff);
        int parse(const std::string &xml);
        void addCallBack(std::string& Path, NodeCallBack cb);
        void setCallBacks(std::map<std::string, NodeCallBack>);

    private:
        std::unique_ptr<xmlNode> root;
        xmlNode *current = nullptr;
        std::map<std::string, NodeCallBack> callbacks;
    };
}