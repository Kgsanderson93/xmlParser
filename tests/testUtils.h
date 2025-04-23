// tests/testUtils.h
#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <iostream>
#include <string>
#include <sstream>
#include <functional>

void stringDif(std::string s1, std::string s2)
{
    size_t minLength = std::min(s1.size(), s2.size());
    for (size_t i = 0; i < minLength; ++i)
    {
        if (s1[i] != s2[i])
        {
            std::cout << "Difference at index " << i << ": "
                      << s1[i] << " vs " << s2[i] << std::endl;
        }
    }
    if (s1.size() != s2.size())
    {
        std::cout << "Strings have different lengths." << std::endl;
    }
}

inline bool assertEqual(const std::string &actual, const std::string &expected, const std::string &testName)
{
    if (actual != expected)
    {
        std::cerr << "❌ FAIL: " << testName << "\n  Expected: \"" << expected << "\"\n  Got: \"" << actual << "\"\n";

        stringDif(actual, expected);
        return false;
    }
    std::cout << "✅ PASS: " << testName << "\n";
    return true;
}

inline bool assertEqual(int actual, int expected, const std::string &testName)
{
    if (actual != expected)
    {
        std::cerr << "❌ FAIL: " << testName << "\n  Expected: " << expected << "\n  Got:      " << actual << "\n";
        return false;
    }
    std::cout << "✅ PASS: " << testName << "\n";
    return true;
}

inline bool assertTrue(bool condition, const std::string &testName)
{
    if (!condition)
    {
        std::cerr << "❌ FAIL: " << testName << "\n  Expected condition to be true.\n";
        return false;
    }
    std::cout << "✅ PASS: " << testName << "\n";
    return true;
}

// Capture std::cout output for testing functions that print
inline std::string captureOutput(std::function<void()> func)
{
    std::ostringstream captured;
    std::streambuf *oldCout = std::cout.rdbuf(captured.rdbuf());

    func();

    std::cout.rdbuf(oldCout);
    return captured.str();
}

#endif
