#ifndef TESTSET_H
#define TESTSET_H

#include <new>
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>

#include "../include/ILogger.h"
#include "../include/IVector.h"
#include "../include/ISet.h"

#define EPS 1e-6

static const size_t g_dim0 = 0;
static const size_t g_dim1 = 1;
static const size_t g_dim2 = 2;

static const double  (&g_data1Nan)[g_dim1] = {NAN};
static const double  (&g_data1FarLeft)[g_dim1] = {-3.0};
static const double  (&g_data1Left)[g_dim1] = {0.0};
static const double  (&g_data1Right)[g_dim1] = {7.0};
static const double  (&g_data1FarRight)[g_dim1] = {10.0};

static const double  (&g_data21)[g_dim2] = {-1.0, 2.0};
static const double  (&g_data22)[g_dim2] = {3.0, 0.0};

bool createSet_Ok_ISetPtr(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);

    bool passed = (set != nullptr);
    if (passed) delete set;
    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool getDim_Ok_DimValue(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    bool passed = (set->getDim() == g_dim0);
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool getSize_Ok_SizeValue(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    bool passed = (set->getSize() == g_dim0);
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool insert_NullPtr_NotSuccess(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    ReturnCode rc = set->insert(nullptr, IVector::Norm::NORM_1, EPS);

    bool passed = (rc != ReturnCode::RC_SUCCESS);
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool insert_IntoEmptySet_Success(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim2];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data21, g_dim2 * sizeof(double));
    IVector *vec21 = IVector::createVector(g_dim2, data1, logger);
    assert(vec21 != nullptr);

    ReturnCode rc = set->insert(vec21, IVector::Norm::NORM_1, EPS);

    bool passed = (rc == ReturnCode::RC_SUCCESS && set->getDim() == g_dim2 && set->getSize() == 1);
    delete vec21;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool insert_WrongDim_NotSuccess(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    double *data2 = new(std::nothrow) double[g_dim2];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data21, g_dim2 * sizeof(double));
    IVector *vec21 = IVector::createVector(g_dim2, data2, logger);
    assert(vec21 != nullptr);

    ReturnCode rc = set->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    rc = set->insert(vec21, IVector::Norm::NORM_1, EPS);

    bool passed = (rc != ReturnCode::RC_SUCCESS && set->getDim() == g_dim1 && set->getSize() == 1);
    delete vec1;
    delete vec21;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool insert_NaNTolerance_NotSuccess(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim2];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data21, g_dim2 * sizeof(double));
    IVector *vec21 = IVector::createVector(g_dim2, data1, logger);
    assert(vec21 != nullptr);

    ReturnCode rc = set->insert(vec21, IVector::Norm::NORM_1, NAN);

    bool passed = (rc != ReturnCode::RC_SUCCESS && set->getDim() == g_dim0 && set->getSize() == 0);
    delete vec21;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool insert_NegativeTolerance_NotSuccess(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim2];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data21, g_dim2 * sizeof(double));
    IVector *vec21 = IVector::createVector(g_dim2, data1, logger);
    assert(vec21 != nullptr);

    ReturnCode rc = set->insert(vec21, IVector::Norm::NORM_1, -EPS);

    bool passed = (rc != ReturnCode::RC_SUCCESS && set->getDim() == g_dim0 && set->getSize() == 0);
    delete vec21;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool insert_ExistingElement_Success(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim2];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data21, g_dim2 * sizeof(double));
    IVector *vec21 = IVector::createVector(g_dim2, data1, logger);
    assert(vec21 != nullptr);

    ReturnCode rc = set->insert(vec21, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    rc = set->insert(vec21, IVector::Norm::NORM_1, EPS);

    bool passed = (rc == ReturnCode::RC_SUCCESS && set->getDim() == g_dim2 && set->getSize() == 1);
    delete vec21;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool insert_NewElement_Success(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim2];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data21, g_dim2 * sizeof(double));
    IVector *vec21 = IVector::createVector(g_dim2, data1, logger);
    assert(vec21 != nullptr);

    double *data2 = new(std::nothrow) double[g_dim2];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data22, g_dim2 * sizeof(double));
    IVector *vec22 = IVector::createVector(g_dim2, data2, logger);
    assert(vec22 != nullptr);

    ReturnCode rc = set->insert(vec21, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    rc = set->insert(vec22, IVector::Norm::NORM_1, EPS);

    bool passed = (rc == ReturnCode::RC_SUCCESS && set->getDim() == g_dim2 && set->getSize() == 2);
    delete vec21;
    delete vec22;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool erase_NullPtr_NotSuccess(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    ReturnCode rc = set->erase(nullptr, IVector::Norm::NORM_1, EPS);

    bool passed = (rc != ReturnCode::RC_SUCCESS);
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool erase_FromEmptySet_NotSuccess(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    ReturnCode rc = set->erase(vec1, IVector::Norm::NORM_1, EPS);

    bool passed = (rc != ReturnCode::RC_SUCCESS);
    delete vec1;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool erase_WrongDim_NotSuccess(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    double *data2 = new(std::nothrow) double[g_dim2];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data21, g_dim2 * sizeof(double));
    IVector *vec21 = IVector::createVector(g_dim2, data2, logger);
    assert(vec21 != nullptr);

    ReturnCode rc = set->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    rc = set->erase(vec21, IVector::Norm::NORM_1, EPS);

    bool passed = (rc != ReturnCode::RC_SUCCESS);
    delete vec1;
    delete vec21;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool erase_NaNTolerance_NotSuccess(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    ReturnCode rc;
    rc = set->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    rc = set->erase(vec1, IVector::Norm::NORM_1, NAN);

    bool passed = (rc != ReturnCode::RC_SUCCESS && set->getDim() == g_dim1 && set->getSize() == 1);
    delete vec1;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool erase_NegativeTolerance_NotSuccess(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    ReturnCode rc;
    rc = set->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    rc = set->erase(vec1, IVector::Norm::NORM_1, -EPS);

    bool passed = (rc != ReturnCode::RC_SUCCESS && set->getDim() == g_dim1 && set->getSize() == 1);
    delete vec1;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool erase_NotExistingElement_NotSuccess(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    double *data2 = new(std::nothrow) double[g_dim1];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data1Right, g_dim1 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim1, data2, logger);
    assert(vec2 != nullptr);

    ReturnCode rc;
    rc = set->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    rc = set->erase(vec2, IVector::Norm::NORM_1, EPS);

    bool passed = (rc != ReturnCode::RC_SUCCESS && set->getDim() == g_dim1 && set->getSize() == 1);
    delete vec1;
    delete vec2;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool erase_ExistingElement_Success(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    double *data2 = new(std::nothrow) double[g_dim1];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data1Right, g_dim1 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim1, data2, logger);
    assert(vec2 != nullptr);

    ReturnCode rc;
    rc = set->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    rc = set->insert(vec2, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    rc = set->erase(vec2, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);

    bool passed = (rc == ReturnCode::RC_SUCCESS && set->getDim() == g_dim1 && set->getSize() == 1);
    delete vec1;
    delete vec2;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool erase_LastExistingElement_SuccessAnd0Dim(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    ReturnCode rc;
    rc = set->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    rc = set->erase(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);

    bool passed = (rc == ReturnCode::RC_SUCCESS && set->getDim() == g_dim0 && set->getSize() == 0);
    delete vec1;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool erase_SeveralExistingElements_Success(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    double *data2 = new(std::nothrow) double[g_dim1];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data1Right, g_dim1 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim1, data2, logger);
    assert(vec2 != nullptr);

    ReturnCode rc;
    rc = set->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    rc = set->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    rc = set->insert(vec2, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    rc = set->erase(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);

    bool passed = (rc == ReturnCode::RC_SUCCESS && set->getDim() == g_dim1 && set->getSize() == 1);
    delete vec1;
    delete vec2;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool eraseByIndex_IndexOutOfRange_NotSuccess(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    ReturnCode rc;
    rc = set->erase(g_dim0);

    bool passed = (rc != ReturnCode::RC_SUCCESS);
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool eraseByIndex_LastExistingElement_SuccessAnd0Dim(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    ReturnCode rc;
    rc = set->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    rc = set->erase(0);
    assert(rc == ReturnCode::RC_SUCCESS);

    bool passed = (rc == ReturnCode::RC_SUCCESS && set->getDim() == g_dim0 && set->getSize() == 0);
    delete vec1;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool eraseByIndex_ExistingElement_Success(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    double *data2 = new(std::nothrow) double[g_dim1];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data1Right, g_dim1 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim1, data2, logger);
    assert(vec2 != nullptr);

    ReturnCode rc;
    rc = set->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    rc = set->insert(vec2, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    rc = set->erase(1);
    assert(rc == ReturnCode::RC_SUCCESS);

    bool passed = (rc == ReturnCode::RC_SUCCESS && set->getDim() == g_dim1 && set->getSize() == 1);
    delete vec1;
    delete vec2;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool clear_Ok_Success(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    double *data2 = new(std::nothrow) double[g_dim1];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data1Right, g_dim1 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim1, data2, logger);
    assert(vec2 != nullptr);

    ReturnCode rc;
    rc = set->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    rc = set->insert(vec2, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    set->clear();

    bool passed = (rc == ReturnCode::RC_SUCCESS && set->getDim() == 0 && set->getSize() == 0);
    delete vec1;
    delete vec2;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool find_NullPtr_NotSuccess(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    ReturnCode rc;
    size_t index;
    rc = set->find(nullptr, IVector::Norm::NORM_1, EPS, index);

    bool passed = (rc != ReturnCode::RC_SUCCESS);
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool find_WrongDim_NotSuccess(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    double *data2 = new(std::nothrow) double[g_dim2];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data21, g_dim2 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim2, data2, logger);
    assert(vec2 != nullptr);

    ReturnCode rc;
    rc = set->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    size_t index;
    rc = set->find(vec2, IVector::Norm::NORM_1, EPS, index);

    bool passed = (rc != ReturnCode::RC_SUCCESS);
    delete vec1;
    delete vec2;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool find_NaNTolerance_NotSuccess(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    double *data2 = new(std::nothrow) double[g_dim1];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data1Right, g_dim1 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim1, data2, logger);
    assert(vec2 != nullptr);

    ReturnCode rc;
    rc = set->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    size_t index;
    rc = set->find(vec2, IVector::Norm::NORM_1, NAN, index);

    bool passed = (rc != ReturnCode::RC_SUCCESS);
    delete vec1;
    delete vec2;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool find_NegativeTolerance_NotSuccess(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    double *data2 = new(std::nothrow) double[g_dim1];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data1Right, g_dim1 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim1, data2, logger);
    assert(vec2 != nullptr);

    ReturnCode rc;
    rc = set->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    size_t index;
    rc = set->find(vec2, IVector::Norm::NORM_1, -EPS, index);

    bool passed = (rc != ReturnCode::RC_SUCCESS);
    delete vec1;
    delete vec2;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool find_NotExistingElement_ElemNotFound(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    double *data2 = new(std::nothrow) double[g_dim1];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data1Right, g_dim1 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim1, data2, logger);
    assert(vec2 != nullptr);

    ReturnCode rc;
    rc = set->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    size_t index;
    rc = set->find(vec2, IVector::Norm::NORM_1, EPS, index);

    bool passed = (rc == ReturnCode::RC_ELEM_NOT_FOUND);
    delete vec1;
    delete vec2;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool find_ExistingElement_Success(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    ReturnCode rc;
    rc = set->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    size_t index;
    rc = set->find(vec1, IVector::Norm::NORM_1, EPS, index);

    bool passed = (rc == ReturnCode::RC_SUCCESS && index == 0);
    delete vec1;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool get_IndexOutOfRange_NotSuccess(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    double *data2 = new(std::nothrow) double[g_dim1];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data1Right, g_dim1 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim1, data2, logger);
    assert(vec2 != nullptr);

    ReturnCode rc;
    rc = set->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    rc = set->insert(vec2, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    IVector *foundVec = nullptr;
    rc = set->get(foundVec, 2);

    bool passed = (rc != ReturnCode::RC_SUCCESS && foundVec == nullptr);
    delete vec1;
    delete vec2;
    delete set;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool get_Ok_Success(ILogger *logger, char *&testName) {
    ISet *set = ISet::createSet(logger);
    assert(set != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    double *data2 = new(std::nothrow) double[g_dim1];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data1Right, g_dim1 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim1, data2, logger);
    assert(vec2 != nullptr);

    ReturnCode rc;
    rc = set->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    rc = set->insert(vec2, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);
    IVector *foundVec = nullptr;
    rc = set->get(foundVec, 1);
    assert(rc == ReturnCode::RC_SUCCESS);
    bool isEqual;
    IVector::equals(vec2, foundVec, IVector::Norm::NORM_1, EPS, isEqual, logger);

    bool passed = (rc == ReturnCode::RC_SUCCESS && foundVec != nullptr && isEqual);
    delete vec1;
    delete vec2;
    delete set;
    if (passed) delete foundVec;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool union_NullPtr_NullPtr(ILogger *logger, char *&testName) {
    ISet *unionSet = ISet::_union(nullptr, nullptr, IVector::Norm::NORM_1, EPS, logger);

    bool passed = (unionSet == nullptr);
    if (!passed) delete unionSet;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool union_WrongDim_NullPtr(ILogger *logger, char *&testName) {
    ISet *set1 = ISet::createSet(logger);
    assert(set1 != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    ReturnCode rc;
    rc = set1->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);

    ISet *set2 = ISet::createSet(logger);
    assert(set2 != nullptr);

    double *data2 = new(std::nothrow) double[g_dim2];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data21, g_dim2 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim2, data2, logger);
    assert(vec2 != nullptr);

    rc = set2->insert(vec2, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);

    delete vec1;
    delete vec2;

    ISet *unionSet = ISet::_union(set1, set2, IVector::Norm::NORM_1, EPS, logger);

    delete set1;
    delete set2;

    bool passed = (unionSet == nullptr);
    if (!passed) delete unionSet;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool union_NaNTolerance_NullPtr(ILogger *logger, char *&testName) {
    ISet *set1 = ISet::createSet(logger);
    assert(set1 != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    ReturnCode rc;
    rc = set1->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);

    ISet *set2 = ISet::createSet(logger);
    assert(set2 != nullptr);

    double *data2 = new(std::nothrow) double[g_dim1];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data1Right, g_dim1 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim1, data2, logger);
    assert(vec2 != nullptr);

    rc = set2->insert(vec2, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);

    delete vec1;
    delete vec2;

    ISet *unionSet = ISet::_union(set1, set2, IVector::Norm::NORM_1, NAN, logger);

    delete set1;
    delete set2;

    bool passed = (unionSet == nullptr);
    if (!passed) delete unionSet;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool union_NegativeTolerance_NullPtr(ILogger *logger, char *&testName) {
    ISet *set1 = ISet::createSet(logger);
    assert(set1 != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    ReturnCode rc;
    rc = set1->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);

    ISet *set2 = ISet::createSet(logger);
    assert(set2 != nullptr);

    double *data2 = new(std::nothrow) double[g_dim1];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data1Right, g_dim1 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim1, data2, logger);
    assert(vec2 != nullptr);

    rc = set2->insert(vec2, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);

    delete vec1;
    delete vec2;

    ISet *unionSet = ISet::_union(set1, set2, IVector::Norm::NORM_1, -EPS, logger);

    delete set1;
    delete set2;

    bool passed = (unionSet == nullptr);
    if (!passed) delete unionSet;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool union_UnionIntersecting_ISetPtr(ILogger *logger, char *&testName) {
    ISet *set1 = ISet::createSet(logger);
    assert(set1 != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    ReturnCode rc;
    rc = set1->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);

    ISet *set2 = ISet::createSet(logger);
    assert(set2 != nullptr);

    double *data2 = new(std::nothrow) double[g_dim1];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data1Right, g_dim1 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim1, data2, logger);
    assert(vec2 != nullptr);

    rc = set2->insert(vec2, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);

    double *intsctData = new(std::nothrow) double[g_dim1];
    assert(intsctData != nullptr);
    std::memcpy(intsctData, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *intsctVec = IVector::createVector(g_dim1, intsctData, logger);
    assert(intsctVec != nullptr);

    rc = set1->insert(intsctVec, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);

    rc = set2->insert(intsctVec, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);

    delete vec1;
    delete vec2;
    delete intsctVec;

    ISet *unionSet = ISet::_union(set1, set2, IVector::Norm::NORM_1, EPS, logger);
    assert(unionSet != nullptr);

    delete set1;
    delete set2;

    bool passed = (unionSet != nullptr && unionSet->getSize() == 3);
    delete unionSet;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool union_UnionNotIntersecting_ISetPtr(ILogger *logger, char *&testName) {
    ISet *set1 = ISet::createSet(logger);
    assert(set1 != nullptr);

    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    ReturnCode rc;
    rc = set1->insert(vec1, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);

    ISet *set2 = ISet::createSet(logger);
    assert(set2 != nullptr);

    double *data2 = new(std::nothrow) double[g_dim1];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data1Right, g_dim1 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim1, data2, logger);
    assert(vec2 != nullptr);

    rc = set2->insert(vec2, IVector::Norm::NORM_1, EPS);
    assert(rc == ReturnCode::RC_SUCCESS);

    delete vec1;
    delete vec2;

    ISet *unionSet = ISet::_union(set1, set2, IVector::Norm::NORM_1, EPS, logger);
    assert(unionSet != nullptr);

    delete set1;
    delete set2;

    bool passed = (unionSet != nullptr && unionSet->getSize() == 2);
    delete unionSet;

    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

//
//bool difference_NullPtr_NullPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool difference_WrongDim_NullPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool difference_NaNTolerance_NullPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool difference_NegativeTolerance_NullPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool difference_DiffIntersecting_ISetPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool difference_DiffNotIntersecting_ISetPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool difference_DiffEqual_ISetPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool symmDifference_NullPtr_NullPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool symmDifference_WrongDim_NullPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool symmDifference_NaNTolerance_NullPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool symmDifference_NegativeTolerance_NullPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool symmDifference_SymmDiffIntersecting_ISetPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool symmDifference_SymmDiffNotIntersecting_ISetPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool symmDifference_SymmDiffEqual_ISetPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool intersection_NullPtr_NullPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool intersection_WrongDim_NullPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool intersection_NaNTolerance_NullPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool intersection_NegativeTolerance_NullPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool intersection_DiffIntersecting_ISetPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool intersection_DiffNotIntersecting_ISetPtr(ILogger *logger, char *&testName) {
//
//}
//
//bool intersection_DiffEqual_ISetPtr(ILogger *logger, char *&testName) {
//
//}

#endif //TESTSET_H
