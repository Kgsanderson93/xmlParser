#include "xmlDemoRunner.h"
#include "xmlUtils.h"
#include <iostream>
#include "xmlParser.h"
#include "userInterface.h"

void xmlDemoRunner::runDefault()
{

    std::string filename = "xmlTestFiles/default.xml";
    std::string filecontents = xmlUtils::readFileToString(filename);
    if (filecontents.empty())
    {
        std::cout << "Empty File or Error reading file " << filename;
    }
    Xml_Parser::xmlParser xmlParser;
    xmlParser.setPrintMode(false);
    std::string pathTofollow = "*/order/amount";
    
    CallbackConfigs config;
    config.pathToFollow= pathTofollow;
    config.greater = true;
    config.trigger= 100;
    config.debug = false;
    config.testmode= false;
    config.attributesToReport.push_back("id");

    xmlParser.addCallBack(pathTofollow, [config](const Xml_Parser::xmlNode &node)
                              {
            
            if (
                xmlUtils::matchPath(config.pathToFollow, node.path) &&
                (
                    (config.greater && xmlUtils::parseIfDouble(node.text) > config.trigger) ||
                    (!config.greater && xmlUtils::parseIfDouble(node.text) < config.trigger)
                )
            ) {
                // Print node info or handle accordingly
                std::cout << "Callback matched: "<<node.parent->name<<" at path: "<< node.path<<" with attributes: \n";
                // Print only specified attributes in config
                
                if(config.debug){
                    std::cout<<"I am looking for "<<config.attributesToReport.size()<<" attributes\n";
                    for(std::string attrname :config.attributesToReport){
                        std::cout<< attrname<<"\n";
                    }
                    std::cout<<"This order has "<<node.parent->attributes.size()<<" attributes\n";
                    for(const auto& attr: node.attributes){
                        std::cout<<"- "<< attr.first <<" = "<<attr.second<<"\n";
                    }
                }
                for (std::string attrName : config.attributesToReport) {
                    auto it = node.parent->attributes.find(attrName);
                    if (it != node.parent->attributes.end()) {
                        std::cout<<"- " << it->first << " = " << it->second << "\n";
                    }
                }
                std::cout<<"With "<<node.name<< " = "<<node.text<<"\n";
                std::string condition=config.greater? "greater":"less";
                std::cout<<"On condition "<<node.name<<" "<<condition<<" than "<< config.trigger<<"\n";
               
            } });

    xmlParser.parse(filecontents);
}

void xmlDemoRunner::setupCallback(Xml_Parser::xmlParser &xmlParser, CallbackConfigs &config, bool store, std::vector<Xml_Parser::xmlNode> &matchedNodes)
{

    std::string path = config.pathToFollow;
    if (store)
    {
        if (config.debug)
        {
            std::cout << "In setupcallback to setup a callback returning a node to vector\n";
        }
        xmlParser.addCallBack(path, [&matchedNodes, config](const Xml_Parser::xmlNode &node)
                              {
                                if(config.debug){
                                    std::cout<<matchedNodes.size()<<"\n";
                                    std::cout<<"In a callback returning a node to vector\n";
                                    std::cout<<"Path is: "<<config.pathToFollow<<"\n";
                                    std::cout<<"Triggval is "<<config.trigger<<"\n";
                                    for(std::string attr: config.attributesToReport){
                                        std::cout<<"attr: "<<attr<<"\n";
                                    }
                                }
            if (
                xmlUtils::matchPath( config.pathToFollow, node.path) &&
                (
                    (config.greater && xmlUtils::parseIfDouble(node.text) > config.trigger) ||
                    (!config.greater && xmlUtils::parseIfDouble(node.text) < config.trigger)
                )
            ){
                if(config.debug){
                    std::cout<<"about to push a node\n";
                    std::cout<<matchedNodes.size()<<"\n";
                }    
                matchedNodes.push_back(node);
                
                
                if(config.debug){
                    std::cout<<"Pushed a node!\n";
                    std::cout<<matchedNodes.size()<<"\n";
                }
                
            } });
    }
    else
    {
        if (config.debug)
        {
            std::cout << "I am in setupCallback setting up a print only callback\n";
        }
        xmlParser.addCallBack(path, [config](const Xml_Parser::xmlNode &node)
                              {
            
            if (
                xmlUtils::matchPath(config.pathToFollow, node.path) &&
                (
                    (config.greater && xmlUtils::parseIfDouble(node.text) > config.trigger) ||
                    (!config.greater && xmlUtils::parseIfDouble(node.text) < config.trigger)
                )
            ) {
                // Print node info or handle accordingly
                std::cout << "Callback matched: "<<node.parent->name<<" at path: "<< node.path<<" with attributes: \n";
                // Print only specified attributes in config
                
                if(config.debug){
                    std::cout<<"I am looking for "<<config.attributesToReport.size()<<" attributes\n";
                    for(std::string attrname :config.attributesToReport){
                        std::cout<< attrname<<"\n";
                    }
                    std::cout<<"This order has "<<node.parent->attributes.size()<<" attributes\n";
                    for(const auto& attr: node.attributes){
                        std::cout<<"- "<< attr.first <<" = "<<attr.second<<"\n";
                    }
                }
                for (std::string attrName : config.attributesToReport) {
                    auto it = node.parent->attributes.find(attrName);
                    if (it != node.parent->attributes.end()) {
                        std::cout<<"- " << it->first << " = " << it->second << "\n";
                    }
                }
                std::cout<<"With "<<node.name<< " = "<<node.text<<"\n";
                std::string condition=config.greater? "greater":"less";
                std::cout<<"On condition "<<node.name<<" "<<condition<<" than "<< config.trigger<<"\n";
               
            } });
    }
}




void xmlDemoRunner::RunCustomDemoWithConfigs(ParserConfigs& configs ){
    configs.fileContents = xmlUtils::readFileToString(configs.filename);
    if (configs.fileContents.empty())
    {
        std::cerr << "Empty File or Error reading file " << configs.filename;
        return;
    }

    Xml_Parser::xmlParser parser;
    parser.printMode=configs.printMode;
    

    if(pConfigs.callbackReportMode == 1){
        for(const auto &cbPair: configs.cbs){
            CallbackConfigs useCB=cbPair.second;
            setupCallback(parser, useCB, false, sharedVector);
        }
    }else if(pConfigs.callbackReportMode == 2){
        for(const auto &cbPair: configs.cbs){
            CallbackConfigs useCB=cbPair.second;
            setupCallback(parser, useCB, true, sharedVector);
        }
    }else{
        for(const auto &cbPair: configs.cbs){
            CallbackConfigs useCB=cbPair.second;
            separateVectors[useCB.pathToFollow] = {};
            setupCallback(parser, useCB, true, separateVectors[useCB.pathToFollow]);
        }
    }
    parser.parse(configs.fileContents);
    return;
    


}
void xmlDemoRunner::run()
{
    // Initialize instance
    ParserConfigs configs;
    configs.testmode=this->testMode;
    configs.debug=this->debug;
    if(!testMode){
        UserInterface::fillParserConfigs(configs);
        while(UserInterface::userWantsToAddCallback()){
            CallbackConfigs cb = UserInterface::collectCallbackConfig();
            cb.debug=configs.debug;
            cb.testmode= configs.testmode;
            configs.cbs[cb.pathToFollow]= cb;
        }
    }
    Xml_Parser::xmlParser parser;
    parser.printMode = configs.printMode;

    
    for(auto& cb: configs.cbs){//register callbacks depending on mode 1 print, 2 one shared vector, 3 seperate vectors per path
        CallbackConfigs currCb = cb.second;
        if(configs.callbackReportMode == 1){
            setupCallback(parser, currCb, false, sharedVector);
        }else if(configs.callbackReportMode ==2){
            setupCallback(parser, currCb, false, sharedVector);
        }else if(configs.callbackReportMode==3){
            separateVectors[currCb.pathToFollow] = {};
            setupCallback(parser, currCb, true, separateVectors[currCb.pathToFollow]);
        }
    }
    parser.parse(configs.fileContents);
    if(!testMode){
    UserInterface::reportResults(sharedVector, separateVectors, configs);
    }
}