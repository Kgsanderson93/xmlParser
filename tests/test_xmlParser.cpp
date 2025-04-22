// tests/test_parser.cpp
#include "xmlParser.h"
#include "xmlUtils.h"
#include "testUtils.h"
#include <string>
#include <iostream>

using namespace Xml_Parser;
int testsPassed = 0;
int testsFailed = 0;

void test_basic_structure() {
    xmlParser parser;
    std::string xml = "<root><child>hello</child></root>";
    int result = parser.parse(xml);
    assertEqual(result, 0, "Basic XML structure parses")? testsPassed++ : testsFailed++;
    assertEqual(parser.getRoot()->children[0]->name, "root", "Root tag name")? testsPassed++ : testsFailed++;
    assertEqual(parser.getRoot()->children[0]->children[0]->text, "hello", "Text in child node")? testsPassed++ : testsFailed++;
}

void test_attribute_parsing() {
    xmlParser parser;
    std::string xml = "<root attr=\"value\"></root>";
    parser.parse(xml);
    auto root = parser.getRoot()->children[0].get();
    assertEqual(root->attributes["attr"], "value", "Attribute parsing")? testsPassed++ : testsFailed++;
}

void test_comment_ignoring() {
    xmlParser parser;
    std::string xml = "<root><!-- ignored --><child>hi</child></root>";
    parser.parse(xml);
    assertEqual(parser.getRoot()->children[0]->children[0]->name, "child", "Ignore XML comment")? testsPassed++ : testsFailed++;
}

void test_callback_triggering() {
    xmlParser parser;
    bool called = false;
    std::string path = "root/child";
    parser.addCallBack(path, [&](const xmlNode& node) {
        called = true;
        assertEqual(node.text, "data", "Callback triggered with correct text")? testsPassed++ : testsFailed++;
    });

    std::string xml = "<root><child>data</child></root>";
    parser.parse(xml);
    assertTrue(called, "Callback was triggered")? testsPassed++ : testsFailed++;
}

void test_mismatched_tag_error() {
    xmlParser parser;
    std::string xml = "<root><child></child2></root>";
    int result = parser.parse(xml);
    assertEqual(result, -1, "Detect mismatched closing tag")? testsPassed++ : testsFailed++;
}

void test_unclosed_tag_error() {
    xmlParser parser;
    std::string xml = "<root><child></root>";
    int result = parser.parse(xml);
    assertEqual(result, -1, "Detect unclosed inner tag")? testsPassed++ : testsFailed++;
}


int main() {
    std::cout<<"Starting xmlParser Tests\n";
    test_basic_structure();
    test_attribute_parsing();
    test_comment_ignoring();
    test_callback_triggering();
    test_mismatched_tag_error();
    test_unclosed_tag_error();

    std::cout << "\n--- Test Summary ---" << "\n";
    std::cout << "Passed: " << testsPassed << "\n";
    std::cout << "Failed: " << testsFailed << "\n";

    return testsFailed == 0 ? 0 : 1;
    
}
