#include "TestSet.h"
#include "../include/Tester.h"
#include <vector>

int main() {
    void *client = (void *) new(std::nothrow) int;
    assert(client != nullptr);
    ILogger *logger = ILogger::createLogger(client);
    assert(logger != nullptr);
    logger->setLogFile("TestSet.log");

    std::cout << "\nISet Testing...\n" << '\n';
    int i = 0;
    std::cout << std::setw(gaps[i++]) << "" << " :: ";

    for (int j = 0; j < 3; ++j) {
        std::cout << std::setw(gaps[j + i]) << columnNames[j] << " ";
    }
    std::cout << "\n\n";

    std::vector<Test_t> tests;
    tests.push_back(createSet_Ok_ISetPtr);
    tests.push_back(getDim_Ok_DimValue);
    tests.push_back(getSize_Ok_SizeValue);
    tests.push_back(insert_NullPtr_NotSuccess);
    tests.push_back(insert_IntoEmptySet_Success);
    tests.push_back(insert_WrongDim_NotSuccess);
    tests.push_back(insert_NaNTolerance_NotSuccess);
    tests.push_back(insert_NegativeTolerance_NotSuccess);
    tests.push_back(insert_ExistingElement_Success);
    tests.push_back(insert_NewElement_Success);
    tests.push_back(erase_NullPtr_NotSuccess);
    tests.push_back(erase_FromEmptySet_NotSuccess);
    tests.push_back(erase_WrongDim_NotSuccess);
    tests.push_back(erase_NaNTolerance_NotSuccess);
    tests.push_back(erase_NegativeTolerance_NotSuccess);
    tests.push_back(erase_NotExistingElement_NotSuccess);
    tests.push_back(erase_ExistingElement_Success);
    tests.push_back(erase_LastExistingElement_SuccessAnd0Dim);
    tests.push_back(erase_SeveralExistingElements_Success);
    tests.push_back(eraseByIndex_IndexOutOfRange_NotSuccess);
    tests.push_back(eraseByIndex_LastExistingElement_SuccessAnd0Dim);
    tests.push_back(eraseByIndex_ExistingElement_Success);
    tests.push_back(clear_Ok_Success);
    tests.push_back(find_NullPtr_NotSuccess);
    tests.push_back(find_WrongDim_NotSuccess);
    tests.push_back(find_NaNTolerance_NotSuccess);
    tests.push_back(find_NegativeTolerance_NotSuccess);
    tests.push_back(find_NotExistingElement_ElemNotFound);
    tests.push_back(find_ExistingElement_Success);
    tests.push_back(get_IndexOutOfRange_NotSuccess);
    tests.push_back(get_Ok_Success);
    tests.push_back(union_NullPtr_NullPtr);
    tests.push_back(union_WrongDim_NullPtr);
    tests.push_back(union_NaNTolerance_NullPtr);
    tests.push_back(union_NegativeTolerance_NullPtr);
    tests.push_back(union_UnionIntersecting_ISetPtr);
    tests.push_back(union_UnionNotIntersecting_ISetPtr);

    int testCounter = 0;
    int passedTestConter = 0;
    for (int j = 0, testsLen = tests.size(); j < testsLen; ++j) {
        if (passTest(tests[j], "ISet", testCounter, logger))
            passedTestConter++;
    }

    std::cout << "\nPASSED: " << passedTestConter << "/" << testCounter << ".\n";

    logger->releaseLogger(client);
    delete (int*)client;

    return 0;
}


