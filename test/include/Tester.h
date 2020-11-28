#ifndef TESTER_H
#define TESTER_H

#include <iostream>
#include <iomanip>
#include "ILogger.h"
#include <string>

typedef bool (*Test_t)(ILogger *, char *&testName);

static const std::string delimiter = "_";
static const size_t  (&gaps)[4] = {9, 15, 30, 15};
static const char * const (&columnNames)[3] = {"method", "state under test", "expected"};

static bool checkLibrary(const char *libraryName);

static bool passTest(Test_t test, const char *testGroupName, int &testCounter, ILogger *logger) {
    char *testName;
    bool testResult = test(logger, testName);

    std::string s(testName);
    s += delimiter;

    int i = 0;
    std::cout << std::setw(gaps[i++]) << testGroupName << " :: ";

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        std::cout << std::setw(gaps[i++]) << token << " ";
        s.erase(0, pos + delimiter.length());
    }

    std::cout << std::setw(2) << ++testCounter << (testResult ? " PASSED " : " FAILED ") << '\n';
    return testResult;
}

#endif //TESTER_H
