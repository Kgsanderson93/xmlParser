#include <iostream>
#include <stack>
#include <string>
#include <climits>
#include "xmlUtils.h"
#include "testUtils.h"

using namespace xmlUtils;
int testsPassed = 0;
int testsFailed = 0;

// Test functions for each utility function

void testNormalizePath()
{
    assertEqual("order/amount", normalizePath("order/amount"), "Normalize no slashes") ? testsPassed++ : testsFailed++;
    assertEqual("order/amount", normalizePath("/order/amount"), "Normalize with leading slash") ? testsPassed++ : testsFailed++;
    assertEqual("order/amount", normalizePath("order/amount/"), "Normalize with trailing slash") ? testsPassed++ : testsFailed++;
    assertEqual("order/amount", normalizePath("order//amount"), "Normalize with double slashes") ? testsPassed++ : testsFailed++;
    assertEqual("order/amount", normalizePath("/order//amount/"), "Normalize both ends + extra") ? testsPassed++ : testsFailed++;
}

void testMatchPath()
{
    assertTrue(matchPath("order/amount", "order/amount"), "Exact match") ? testsPassed++ : testsFailed++;
    assertTrue(matchPath("*/order/amount", "root/order/amount"), "Wildcard match") ? testsPassed++ : testsFailed++;
    assertTrue(!matchPath("*/price", "root/order/amount"), "Wildcard no match") ? testsPassed++ : testsFailed++;
}

void testParseIfInteger()
{
    assertEqual(42, parseIfInteger("42"), "Valid integer") ? testsPassed++ : testsFailed++;
    assertEqual(INT_MIN, parseIfInteger("notAnInt"), "Invalid string") ? testsPassed++ : testsFailed++;
    assertEqual(INT_MIN, parseIfInteger("42abc"), "Mixed content") ? testsPassed++ : testsFailed++;
    assertEqual(INT_MIN, parseIfInteger(""), "Empty string") ? testsPassed++ : testsFailed++;
}

void testPathEndsIn()
{
    assertTrue(pathEndsIn("root/order/amount", "order/amount"), "Ends in match") ? testsPassed++ : testsFailed++;
    assertTrue(!pathEndsIn("root/order/amount", "order/total"), "Ends in no match") ? testsPassed++ : testsFailed++;
}

void testIsWhiteSpaceOnly()
{
    assertTrue(isWhiteSpaceOnly("   \t\n"), "All spaces") ? testsPassed++ : testsFailed++;
    assertTrue(!isWhiteSpaceOnly("  test "), "Not all spaces") ? testsPassed++ : testsFailed++;
}

void testTrim()
{
    assertEqual("test", trim("  test "), "Trim leading/trailing") ? testsPassed++ : testsFailed++;
    assertEqual("", trim("   \t\n"), "Trim all whitespace") ? testsPassed++ : testsFailed++;
}

void testBuildPath()
{
    std::stack<std::string> tagstack;
    tagstack.push("order");
    tagstack.push("amount");
    assertEqual("/order/amount", buildPath(tagstack), "Build path from stack") ? testsPassed++ : testsFailed++;
}

void testDecodeEnt()
{
    assertEqual("\"&\\<>", decodeEnt("&quot;&amp;&apos;&lt;&gt;"), "Decode all entities") ? testsPassed++ : testsFailed++;
    assertEqual("&unknown;", decodeEnt("&unknown;"), "Leave unknown entity") ? testsPassed++ : testsFailed++;
}

int main()
{
    std::cout << "Starting xmlUtils Test: \n";
    testNormalizePath();
    testMatchPath();
    testParseIfInteger();
    testPathEndsIn();
    testIsWhiteSpaceOnly();
    testTrim();
    testBuildPath();
    testDecodeEnt();

    std::cout << "\n--- Test Summary ---" << "\n";
    std::cout << "Passed: " << testsPassed << "\n";
    std::cout << "Failed: " << testsFailed << "\n";

    return testsFailed == 0 ? 0 : 1;
}
