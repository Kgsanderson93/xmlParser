// tests/testUtils.h
#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <iostream>
#include <string>

inline bool assertEqual(const std::string& actual, const std::string& expected, const std::string& testName) {
    if (actual != expected) {
        std::cerr << "❌ FAIL: " << testName << "\n  Expected: \"" << expected << "\"\n  Got:      \"" << actual << "\"\n";
        return false;
    }
    std::cout << "✅ PASS: " << testName << "\n";
    return true;
}

inline bool assertEqual(int actual, int expected, const std::string& testName) {
    if (actual != expected) {
        std::cerr << "❌ FAIL: " << testName << "\n  Expected: " << expected << "\n  Got:      " << actual << "\n";
        return false;
    }
    std::cout << "✅ PASS: " << testName << "\n";
    return true;
}

inline bool assertTrue(bool condition, const std::string& testName) {
    if (!condition) {
        std::cerr << "❌ FAIL: " << testName << "\n  Expected condition to be true.\n";
        return false;
    }
    std::cout << "✅ PASS: " << testName << "\n";
    return true;
}

#endif
