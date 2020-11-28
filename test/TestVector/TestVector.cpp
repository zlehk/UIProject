#include "TestVector.h"
#include "../include/Tester.h"
#include <vector>

int main() {
    void *client = (void *) new(std::nothrow) int;
    assert(client != nullptr);
    ILogger *logger = ILogger::createLogger(client);
    assert(logger != nullptr);
    logger->setLogFile("TestVector.log");

    std::cout << "\nIVector Testing...\n" << '\n';
    int i = 0;
    std::cout << std::setw(gaps[i++]) << "" << " :: ";

    for (int j = 0; j < 3; ++j) {
        std::cout << std::setw(gaps[j + i]) << columnNames[j] << " ";
    }
    std::cout << "\n\n";

    std::vector<Test_t> tests;
    tests.push_back(createVector_WrongDim_NullPtr);
    tests.push_back(createVector_NullPtr_NullPtr);
    tests.push_back(createVector_NaNValue_NullPtr);
    tests.push_back(createVector_Ok_IVectorPtr);
    tests.push_back(getCoord_IndexOutOfRange_NaN);
    tests.push_back(getCoord_Ok_CoordByIndex);
    tests.push_back(setCoord_IndexOutOfRange_NotSuccess);
    tests.push_back(setCoord_NaNValue_NotSuccess);
    tests.push_back(setCoord_Ok_Success);
    tests.push_back(norm_InvalidNorm_NaN);
    tests.push_back(norm_Ok_Norm1Value);
    tests.push_back(norm_Ok_Norm2Value);
    tests.push_back(norm_Ok_NormInfValue);
    tests.push_back(equal_NullPtr_NullPtr);
    tests.push_back(equal_WrongDim_NullPtr);
    tests.push_back(equal_InvalidNorm_NullPtr);
    tests.push_back(equal_NaNTolerance_NullPtr);
    tests.push_back(equal_Ok_NotSuccess);
    tests.push_back(equal_Ok_Success);
    tests.push_back(add_NullPtr_NullPtr);
    tests.push_back(add_WrongDim_NullPtr);
    tests.push_back(add_Ok_IVectorPtr);
    tests.push_back(sub_NullPtr_NullPtr);
    tests.push_back(sub_WrongDim_NullPtr);
    tests.push_back(sub_Ok_IVectorPtr);
    tests.push_back(mulByNumber_NullPtr_NullPtr);
    tests.push_back(mulByNumber_NaNValue_NullPtr);
    tests.push_back(mulByNumber_Ok_IVectorPtr);
    tests.push_back(mul_NullPtr_NullPtr);
    tests.push_back(mul_WrongDim_NullPtr);
    tests.push_back(mul_Ok_IVectorPtr);

    int testCounter = 0;
    int passedTestConter = 0;
    for (int j = 0, testsLen = tests.size(); j < testsLen; ++j) {
        if (passTest(tests[j], "IVector", testCounter, logger))
            passedTestConter++;
    }

    std::cout << "\nPASSED: " << passedTestConter << "/" << testCounter << ".\n";

    logger->releaseLogger(client);
    delete (int*)client;

    return 0;
}
