// tests/test_xmlDemoRunner.cpp
#include "xmlDemoRunner.h"
#include "xmlParserConfig.h"
#include "testUtils.h"
#include "xmlUtils.h"
#include <sstream>

int testsPassed = 0;
int testsFailed = 0;

void runTest(bool result) {
    if (result) testsPassed++;
    else testsFailed++;
}

void test_BasicPathMatch() {
    ParserConfigs pConfig;
    CallbackConfigs cConfig;

    // Configure to test a known path match
    pConfig.filename = "xmlTestFiles/default.xml";
    pConfig.testmode = true;
    pConfig.debug = false;
    pConfig.printMode =false;
    pConfig.callbackReportMode = 1;

    cConfig.pathToFollow= "*/order/amount";
    cConfig.attributesToReport.push_back("id");
    cConfig.testmode = true;
    cConfig.debug = false;
    cConfig.greater = true;
    cConfig.trigger = 100;

    pConfig.cbs[cConfig.pathToFollow] = cConfig;
    xmlDemoRunner runner;
    runner.pConfigs = pConfig;
 
    std::string fileToRead = "/home/kgsande/xmlparse/xmlTestFiles/test_xmlDemoRunnerOutputGoal1.txt";
    std::string expected = xmlUtils::readFileToString(fileToRead); // example expected
    

    std::string output = captureOutput([&]() {
        runner.RunCustomDemoWithConfigs(pConfig);
    });
    
    runTest(assertEqual(output, expected, "test_BasicPathMatch"));
}

void test_BasicPathMatchLesser() {
    ParserConfigs pConfig;
    CallbackConfigs cConfig;

    // Configure to test a known path match
    pConfig.filename = "xmlTestFiles/default.xml";
    pConfig.testmode = true;
    pConfig.debug = false;
    pConfig.printMode =false;
    pConfig.callbackReportMode = 1;

    cConfig.pathToFollow= "*/order/amount";
    cConfig.attributesToReport.push_back("id");
    cConfig.testmode = true;
    cConfig.debug = false;
    cConfig.greater = false;
    cConfig.trigger = 100;

    pConfig.cbs[cConfig.pathToFollow] = cConfig;
    xmlDemoRunner runner;
    runner.pConfigs = pConfig;
 
    std::string fileToRead = "/home/kgsande/xmlparse/xmlTestFiles/test_xmlDemoRunnerOutputGoal2.txt";
    std::string expected = xmlUtils::readFileToString(fileToRead); // example expected
    

    std::string output = captureOutput([&]() {
        runner.RunCustomDemoWithConfigs(pConfig);
    });

    
    
    runTest(assertEqual(output, expected, "test_BasicPathMatchLesser"));
}

void test_BasicPathMatchComment() {
    ParserConfigs pConfig;
    CallbackConfigs cConfig;

    // Configure to test a known path match
    pConfig.filename = "xmlTestFiles/commentandescape.xml";
    pConfig.testmode = true;
    pConfig.debug = false;
    pConfig.printMode =false;
    pConfig.callbackReportMode = 1;

    cConfig.pathToFollow= "*/book/price";
    cConfig.attributesToReport.push_back("id");
    cConfig.testmode = true;
    cConfig.debug = false;
    cConfig.greater = true;
    cConfig.trigger = 2;

    pConfig.cbs[cConfig.pathToFollow] = cConfig;
    xmlDemoRunner runner;
    runner.pConfigs = pConfig;
 
    std::string fileToRead = "/home/kgsande/xmlparse/xmlTestFiles/test_xmlDemoRunnerOutputGoal3.txt";
    std::string expected = xmlUtils::readFileToString(fileToRead); // example expected
    

    std::string output = captureOutput([&]() {
        runner.RunCustomDemoWithConfigs(pConfig);
    });

    
    
    runTest(assertEqual(output, expected, "test_BasicPathMatchComment"));
}

void test_BasicPathMatchPrint() {
    ParserConfigs pConfig;
    CallbackConfigs cConfig;

    // Configure to test a known path match
    pConfig.filename = "xmlTestFiles/books.xml";
    pConfig.testmode = true;
    pConfig.debug = false;
    pConfig.printMode =true;
    pConfig.callbackReportMode = 1;

    cConfig.pathToFollow= "*/book/price";
    cConfig.attributesToReport.push_back("id");
    cConfig.testmode = true;
    cConfig.debug = false;
    cConfig.greater = true;
    cConfig.trigger = 10;

    pConfig.cbs[cConfig.pathToFollow] = cConfig;
    xmlDemoRunner runner;
    runner.pConfigs = pConfig;
 
    std::string fileToRead = "/home/kgsande/xmlparse/xmlTestFiles/test_xmlDemoRunnerOutputGoal4.txt";
    std::string expected = xmlUtils::readFileToString(fileToRead); // example expected
    

    std::string output = captureOutput([&]() {
        runner.RunCustomDemoWithConfigs(pConfig);
    });

    
    
    runTest(assertEqual(output, expected, "test_BasicPathMatchPrint"));
}


int main() {
    std::cout << "Running xmlDemoRunner tests...\n\n";

    test_BasicPathMatch();
    test_BasicPathMatchLesser();
    test_BasicPathMatchComment();
    test_BasicPathMatchPrint();
    




    std::cout << "\nTests passed: " << testsPassed << "\nTests failed: " << testsFailed << "\n";
    return testsFailed == 0 ? 0 : 1;
}
