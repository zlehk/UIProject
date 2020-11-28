#include "TestCompact.h"
#include "../include/Tester.h"
#include <vector>

int main() {
    void *client = (void *) new(std::nothrow) int;
    assert(client != nullptr);
    ILogger *logger = ILogger::createLogger(client);
    assert(logger != nullptr);
    logger->setLogFile("TestCompact.log");

    std::cout << "\nICompact Testing...\n" << '\n';
    int i = 0;
    std::cout << std::setw(gaps[i++]) << "" << " :: ";

    for (int j = 0; j < 3; ++j) {
        std::cout << std::setw(gaps[j + i]) << columnNames[j] << " ";
    }
    std::cout << "\n\n";

    std::vector<Test_t> tests;
    tests.push_back(createCompact_NullPtr_NullPtr);
    tests.push_back(createCompact_WrongDim_NullPtr);
    tests.push_back(createCompact_DegeneracyCheck_NullPtr);
    tests.push_back(createCompact_NaNTolerance_NullPtr);
    tests.push_back(createCompact_NegativeTolerance_NullPtr);
    tests.push_back(createCompact_Ok_ICompactPtr);
    tests.push_back(createCompact_Ok_ICompactPtr);
    tests.push_back(getDim_Ok_DimValue);
    tests.push_back(getBegin_Ok_BeginIVector);
    tests.push_back(getEnd_Ok_EndIVector);
    tests.push_back(contains_NullPtr_NotSuccess);
    tests.push_back(contains_WrongDim_NotSuccess);
    tests.push_back(contains_NotExistingElement_Success);
    tests.push_back(contains_ExistingElement_Success);
    tests.push_back(isSubset_NullPtr_NotSuccess);
    tests.push_back(isSubset_WrongDim_NotSuccess);
    tests.push_back(isSubset_CheckIncluding_Success);
    tests.push_back(isSubset_CheckNotIntersecting_Success);
    tests.push_back(intersects_NullPtr_NotSuccess);
    tests.push_back(intersects_WrongDim_NotSuccess);
    tests.push_back(intersects_CheckIntersecting_Success);
    tests.push_back(intersects_CheckNotIntersecting_Success);
    tests.push_back(union_NullPtr_NullPtr);
    tests.push_back(union_WrongDim_NullPtr);
    tests.push_back(union_NaNTolerance_NullPtr);
    tests.push_back(union_NegativeTolerance_NullPtr);
    tests.push_back(union_UnionNotIntersecting_ICompactPtr);
//    tests.push_back(union_UnionIntersectingOn1Axis_ICompactPtr);
    tests.push_back(union_UnionIntersectingOnSeveralAxes_ICompactPtr);
    tests.push_back(union_UnionIncluding_ICompactPtr);
    tests.push_back(convex_NullPtr_NullPtr);
    tests.push_back(convex_WrongDim_NullPtr);
    tests.push_back(intersection_NullPtr_NullPtr);
    tests.push_back(intersection_WrongDim_NullPtr);

    int testCounter = 0;
    int passedTestConter = 0;
    for (int j = 0, testsLen = tests.size(); j < testsLen; ++j) {
        if (passTest(tests[j], "ICompact", testCounter, logger))
            passedTestConter++;
    }

    std::cout << "\nPASSED: " << passedTestConter << "/" << testCounter << ".\n";

    logger->releaseLogger(client);
    delete (int *) client;

    return 0;
}

