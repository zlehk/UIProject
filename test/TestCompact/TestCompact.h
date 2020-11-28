#ifndef TESTCOMPACT_H
#define TESTCOMPACT_H

#include <new>
#include <cassert>
#include <cmath>
#include <cstring>

#include "../include/ILogger.h"
#include "../include/IVector.h"
#include "../include/ICompact.h"

#define EPS 1e-6

static const size_t g_dim0 = 0;
static const size_t g_dim1 = 1;
static const size_t g_dim2 = 2;

static const double  (&g_data1Nan)[g_dim1] = {NAN};
static const double  (&g_data1FarLeft)[g_dim1] = {-3.0};
static const double  (&g_data1Left)[g_dim1] = {0.0};
static const double  (&g_data1Right)[g_dim1] = {7.0};
static const double  (&g_data1FarRight)[g_dim1] = {10.0};

static const double  (&g_data2Nan)[g_dim2] = {NAN, 1.0};
static const double  (&g_data2BottomLeft)[g_dim2] = {-3.0, -1.0};
static const double  (&g_data2BottomzdjhfLeft)[g_dim2] = {-3.0 + EPS / 2.0, -1.0};
static const double  (&g_data2CenterLeft)[g_dim2] = {-1.0, 0.0};
static const double  (&g_data2CenterRight)[g_dim2] = {0.0, 1.0};
static const double  (&g_data2TopRight)[g_dim2] = {2.0, 1.5};


bool createCompact_NullPtr_NullPtr(ILogger *logger, char *& testName) {
    ICompact *compNull = ICompact::createCompact(nullptr, nullptr, EPS, logger);

    bool passed = compNull == nullptr;
    if (!passed) delete compNull;
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool createCompact_WrongDim_NullPtr(ILogger *logger, char *& testName) {
    double *beginData2 = new(std::nothrow) double[g_dim2];
    assert(beginData2 != nullptr);
    std::memcpy(beginData2, g_data2BottomLeft, g_dim2 * sizeof(double));
    IVector *begin = IVector::createVector(g_dim2, beginData2, logger);
    assert(begin != nullptr);

    double *endData1 = new(std::nothrow) double[g_dim1];
    assert(endData1 != nullptr);
    std::memcpy(endData1, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *end = IVector::createVector(g_dim1, endData1, logger);
    assert(end != nullptr);

    ICompact *compNull = ICompact::createCompact(begin, end, EPS, logger);

    delete begin;
    delete end;

    bool passed = compNull == nullptr;
    if (!passed) delete compNull;
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool createCompact_DegeneracyCheck_NullPtr(ILogger *logger, char *& testName) {
    double *beginData = new(std::nothrow) double[g_dim1];
    assert(beginData != nullptr);
    std::memcpy(beginData, g_data1Right, g_dim1 * sizeof(double));
    IVector *begin = IVector::createVector(g_dim1, beginData, logger);
    assert(begin != nullptr);

    double *endData = new(std::nothrow) double[g_dim1];
    assert(endData != nullptr);
    std::memcpy(endData, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *end = IVector::createVector(g_dim1, endData, logger);
    assert(end != nullptr);

    ICompact *compNull = ICompact::createCompact(begin, end, EPS, logger);

    delete begin;
    delete end;

    bool passed = compNull == nullptr;
    if (!passed) delete compNull;
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool createCompact_NaNTolerance_NullPtr(ILogger *logger, char *& testName) {
    double *beginData = new(std::nothrow) double[g_dim1];
    assert(beginData != nullptr);
    std::memcpy(beginData, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin = IVector::createVector(g_dim1, beginData, logger);
    assert(begin != nullptr);

    double *endData = new(std::nothrow) double[g_dim1];
    assert(endData != nullptr);
    std::memcpy(endData, g_data1Right, g_dim1 * sizeof(double));
    IVector *end = IVector::createVector(g_dim1, endData, logger);
    assert(end != nullptr);

    ICompact *compNull = ICompact::createCompact(begin, end, NAN, logger);

    delete begin;
    delete end;

    bool passed = compNull == nullptr;
    if (!passed) delete compNull;
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool createCompact_NegativeTolerance_NullPtr(ILogger *logger, char *& testName) {
    double *beginData = new(std::nothrow) double[g_dim1];
    assert(beginData != nullptr);
    std::memcpy(beginData, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin = IVector::createVector(g_dim1, beginData, logger);
    assert(begin != nullptr);

    double *endData = new(std::nothrow) double[g_dim1];
    assert(endData != nullptr);
    std::memcpy(endData, g_data1Right, g_dim1 * sizeof(double));
    IVector *end = IVector::createVector(g_dim1, endData, logger);
    assert(end != nullptr);

    ICompact *compNull = ICompact::createCompact(begin, end, -EPS, logger);

    delete begin;
    delete end;

    bool passed = compNull == nullptr;
    if (!passed) delete compNull;
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool createCompact_Ok_ICompactPtr(ILogger *logger, char *& testName) {
    double *beginData = new(std::nothrow) double[g_dim1];
    assert(beginData != nullptr);
    std::memcpy(beginData, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin = IVector::createVector(g_dim1, beginData, logger);
    assert(begin != nullptr);

    double *endData = new(std::nothrow) double[g_dim1];
    assert(endData != nullptr);
    std::memcpy(endData, g_data1Right, g_dim1 * sizeof(double));
    IVector *end = IVector::createVector(g_dim1, endData, logger);
    assert(end != nullptr);

    ICompact *comp = ICompact::createCompact(begin, end, EPS, logger);

    delete begin;
    delete end;

    bool passed = comp != nullptr;
    if (passed) delete comp;
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool getDim_Ok_DimValue(ILogger *logger, char *& testName) {
    double *beginData = new(std::nothrow) double[g_dim1];
    assert(beginData != nullptr);
    std::memcpy(beginData, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin = IVector::createVector(g_dim1, beginData, logger);
    assert(begin != nullptr);

    double *endData = new(std::nothrow) double[g_dim1];
    assert(endData != nullptr);
    std::memcpy(endData, g_data1Right, g_dim1 * sizeof(double));
    IVector *end = IVector::createVector(g_dim1, endData, logger);
    assert(end != nullptr);

    ICompact *comp = ICompact::createCompact(begin, end, EPS, logger);
    assert(comp != nullptr);

    delete begin;
    delete end;

    bool passed = (comp->getDim() == g_dim1);
    delete comp;
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool getBegin_Ok_BeginIVector(ILogger *logger, char *& testName) {
    double *beginData = new(std::nothrow) double[g_dim1];
    assert(beginData != nullptr);
    std::memcpy(beginData, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin = IVector::createVector(g_dim1, beginData, logger);
    assert(begin != nullptr);

    double *endData = new(std::nothrow) double[g_dim1];
    assert(endData != nullptr);
    std::memcpy(endData, g_data1Right, g_dim1 * sizeof(double));
    IVector *end = IVector::createVector(g_dim1, endData, logger);
    assert(end != nullptr);

    ICompact *comp = ICompact::createCompact(begin, end, EPS, logger);
    assert(comp != nullptr);

    delete begin;
    delete end;

    IVector *compBegin = comp->getBegin();
    delete comp;
    assert(compBegin != nullptr);

    bool passed = (compBegin->getCoord(0) == g_data1FarLeft[0]);
    delete compBegin;
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool getEnd_Ok_EndIVector(ILogger *logger, char *& testName) {
    double *beginData = new(std::nothrow) double[g_dim1];
    assert(beginData != nullptr);
    std::memcpy(beginData, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin = IVector::createVector(g_dim1, beginData, logger);
    assert(begin != nullptr);

    double *endData = new(std::nothrow) double[g_dim1];
    assert(endData != nullptr);
    std::memcpy(endData, g_data1Right, g_dim1 * sizeof(double));
    IVector *end = IVector::createVector(g_dim1, endData, logger);
    assert(end != nullptr);

    ICompact *comp = ICompact::createCompact(begin, end, EPS, logger);
    assert(comp != nullptr);

    delete begin;
    delete end;

    IVector *compEnd = comp->getEnd();
    delete comp;
    assert(compEnd != nullptr);

    bool passed = (compEnd->getCoord(0) == g_data1Right[0]);
    delete compEnd;
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool contains_NullPtr_NotSuccess(ILogger *logger, char *& testName) {
    double *beginData = new(std::nothrow) double[g_dim1];
    assert(beginData != nullptr);
    std::memcpy(beginData, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin = IVector::createVector(g_dim1, beginData, logger);
    assert(begin != nullptr);

    double *endData = new(std::nothrow) double[g_dim1];
    assert(endData != nullptr);
    std::memcpy(endData, g_data1Right, g_dim1 * sizeof(double));
    IVector *end = IVector::createVector(g_dim1, endData, logger);
    assert(end != nullptr);

    ICompact *comp = ICompact::createCompact(begin, end, EPS, logger);
    assert(comp != nullptr);

    delete begin;
    delete end;

    bool isInside;
    ReturnCode rc = comp->contains(nullptr, isInside);
    delete comp;
    testName = const_cast<char *>(__FUNCTION__); return (rc != ReturnCode::RC_SUCCESS && !isInside);;
}

bool contains_WrongDim_NotSuccess(ILogger *logger, char *& testName) {
    double *beginData = new(std::nothrow) double[g_dim1];
    assert(beginData != nullptr);
    std::memcpy(beginData, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin = IVector::createVector(g_dim1, beginData, logger);
    assert(begin != nullptr);

    double *endData = new(std::nothrow) double[g_dim1];
    assert(endData != nullptr);
    std::memcpy(endData, g_data1Right, g_dim1 * sizeof(double));
    IVector *end = IVector::createVector(g_dim1, endData, logger);
    assert(end != nullptr);

    ICompact *comp = ICompact::createCompact(begin, end, EPS, logger);
    assert(comp != nullptr);

    delete begin;
    delete end;

    double *vecData = new(std::nothrow) double[g_dim2];
    assert(vecData != nullptr);
    std::memcpy(vecData, g_data2BottomLeft, g_dim2 * sizeof(double));
    IVector *vec = IVector::createVector(g_dim2, vecData, logger);
    assert(vec != nullptr);

    bool isInside;
    ReturnCode rc = comp->contains(vec, isInside);
    delete comp;
    delete vec;
    testName = const_cast<char *>(__FUNCTION__); return (rc != ReturnCode::RC_SUCCESS && !isInside);
}

bool contains_NotExistingElement_Success(ILogger *logger, char *& testName) {
    double *beginData = new(std::nothrow) double[g_dim1];
    assert(beginData != nullptr);
    std::memcpy(beginData, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin = IVector::createVector(g_dim1, beginData, logger);
    assert(begin != nullptr);

    double *endData = new(std::nothrow) double[g_dim1];
    assert(endData != nullptr);
    std::memcpy(endData, g_data1Right, g_dim1 * sizeof(double));
    IVector *end = IVector::createVector(g_dim1, endData, logger);
    assert(end != nullptr);

    ICompact *comp = ICompact::createCompact(begin, end, EPS, logger);
    assert(comp != nullptr);

    delete begin;
    delete end;

    double *vecData = new(std::nothrow) double[g_dim1];
    assert(vecData != nullptr);
    std::memcpy(vecData, g_data1FarRight, g_dim1 * sizeof(double));
    IVector *vec = IVector::createVector(g_dim1, vecData, logger);
    assert(vec != nullptr);

    bool isInside;
    ReturnCode rc = comp->contains(vec, isInside);
    delete comp;
    delete vec;
    testName = const_cast<char *>(__FUNCTION__); return (rc == ReturnCode::RC_SUCCESS && !isInside);
}

bool contains_ExistingElement_Success(ILogger *logger, char *& testName) {
    double *beginData = new(std::nothrow) double[g_dim1];
    assert(beginData != nullptr);
    std::memcpy(beginData, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin = IVector::createVector(g_dim1, beginData, logger);
    assert(begin != nullptr);

    double *endData = new(std::nothrow) double[g_dim1];
    assert(endData != nullptr);
    std::memcpy(endData, g_data1Right, g_dim1 * sizeof(double));
    IVector *end = IVector::createVector(g_dim1, endData, logger);
    assert(end != nullptr);

    ICompact *comp = ICompact::createCompact(begin, end, EPS, logger);
    assert(comp != nullptr);

    delete begin;
    delete end;

    double *vecData = new(std::nothrow) double[g_dim1];
    assert(vecData != nullptr);
    std::memcpy(vecData, g_data1Left, g_dim1 * sizeof(double));
    IVector *vec = IVector::createVector(g_dim1, vecData, logger);
    assert(vec != nullptr);

    bool isInside;
    ReturnCode rc = comp->contains(vec, isInside);
    delete comp;
    delete vec;
    testName = const_cast<char *>(__FUNCTION__); return (rc == ReturnCode::RC_SUCCESS && isInside);
}

bool isSubset_NullPtr_NotSuccess(ILogger *logger, char *& testName) {
    double *beginData = new(std::nothrow) double[g_dim1];
    assert(beginData != nullptr);
    std::memcpy(beginData, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin = IVector::createVector(g_dim1, beginData, logger);
    assert(begin != nullptr);

    double *endData = new(std::nothrow) double[g_dim1];
    assert(endData != nullptr);
    std::memcpy(endData, g_data1Right, g_dim1 * sizeof(double));
    IVector *end = IVector::createVector(g_dim1, endData, logger);
    assert(end != nullptr);

    ICompact *comp = ICompact::createCompact(begin, end, EPS, logger);
    assert(comp != nullptr);

    delete begin;
    delete end;

    bool isSubset;
    ReturnCode rc = comp->isSubset(nullptr, isSubset);
    delete comp;

    testName = const_cast<char *>(__FUNCTION__); return (rc != ReturnCode::RC_SUCCESS && !isSubset);
}

bool isSubset_WrongDim_NotSuccess(ILogger *logger, char *& testName) {
    double *beginData1 = new(std::nothrow) double[g_dim1];
    assert(beginData1 != nullptr);
    std::memcpy(beginData1, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin1 = IVector::createVector(g_dim1, beginData1, logger);
    assert(begin1 != nullptr);

    double *endData1 = new(std::nothrow) double[g_dim1];
    assert(endData1 != nullptr);
    std::memcpy(endData1, g_data1Right, g_dim1 * sizeof(double));
    IVector *end1 = IVector::createVector(g_dim1, endData1, logger);
    assert(end1 != nullptr);

    ICompact *comp1 = ICompact::createCompact(begin1, end1, EPS, logger);
    assert(comp1 != nullptr);

    delete begin1;
    delete end1;

    double *beginData2 = new(std::nothrow) double[g_dim2];
    assert(beginData2 != nullptr);
    std::memcpy(beginData2, g_data2BottomLeft, g_dim2 * sizeof(double));
    IVector *begin2 = IVector::createVector(g_dim2, beginData2, logger);
    assert(begin2 != nullptr);

    double *endData2 = new(std::nothrow) double[g_dim2];
    assert(endData2 != nullptr);
    std::memcpy(endData2, g_data2TopRight, g_dim2 * sizeof(double));
    IVector *end2 = IVector::createVector(g_dim2, endData2, logger);
    assert(end2 != nullptr);

    ICompact *comp2 = ICompact::createCompact(begin2, end2, EPS, logger);
    assert(comp2 != nullptr);

    delete begin2;
    delete end2;

    bool isSubset;
    ReturnCode rc = comp1->isSubset(comp2, isSubset);
    delete comp1;
    delete comp2;

    testName = const_cast<char *>(__FUNCTION__); return (rc != ReturnCode::RC_SUCCESS && !isSubset);
}

bool isSubset_CheckIncluding_Success(ILogger *logger, char *& testName) {
    double *beginData1 = new(std::nothrow) double[g_dim1];
    assert(beginData1 != nullptr);
    std::memcpy(beginData1, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin1 = IVector::createVector(g_dim1, beginData1, logger);
    assert(begin1 != nullptr);

    double *endData1 = new(std::nothrow) double[g_dim1];
    assert(endData1 != nullptr);
    std::memcpy(endData1, g_data1FarRight, g_dim1 * sizeof(double));
    IVector *end1 = IVector::createVector(g_dim1, endData1, logger);
    assert(end1 != nullptr);

    ICompact *comp1 = ICompact::createCompact(begin1, end1, EPS, logger);
    assert(comp1 != nullptr);

    delete begin1;
    delete end1;

    double *beginData2 = new(std::nothrow) double[g_dim1];
    assert(beginData2 != nullptr);
    std::memcpy(beginData2, g_data1Left, g_dim1 * sizeof(double));
    IVector *begin2 = IVector::createVector(g_dim1, beginData2, logger);
    assert(begin2 != nullptr);

    double *endData2 = new(std::nothrow) double[g_dim1];
    assert(endData2 != nullptr);
    std::memcpy(endData2, g_data1Right, g_dim1 * sizeof(double));
    IVector *end2 = IVector::createVector(g_dim1, endData2, logger);
    assert(end2 != nullptr);

    ICompact *comp2 = ICompact::createCompact(begin2, end2, EPS, logger);
    assert(comp2 != nullptr);

    delete begin2;
    delete end2;

    bool isSubset;
    ReturnCode rc = comp2->isSubset(comp1, isSubset);
    delete comp1;
    delete comp2;

    testName = const_cast<char *>(__FUNCTION__); return (rc == ReturnCode::RC_SUCCESS && isSubset);
}

bool isSubset_CheckNotIntersecting_Success(ILogger *logger, char *& testName) {
    double *beginData1 = new(std::nothrow) double[g_dim1];
    assert(beginData1 != nullptr);
    std::memcpy(beginData1, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin1 = IVector::createVector(g_dim1, beginData1, logger);
    assert(begin1 != nullptr);

    double *endData1 = new(std::nothrow) double[g_dim1];
    assert(endData1 != nullptr);
    std::memcpy(endData1, g_data1Left, g_dim1 * sizeof(double));
    IVector *end1 = IVector::createVector(g_dim1, endData1, logger);
    assert(end1 != nullptr);

    ICompact *comp1 = ICompact::createCompact(begin1, end1, EPS, logger);
    assert(comp1 != nullptr);

    delete begin1;
    delete end1;

    double *beginData2 = new(std::nothrow) double[g_dim1];
    assert(beginData2 != nullptr);
    std::memcpy(beginData2, g_data1Right, g_dim1 * sizeof(double));
    IVector *begin2 = IVector::createVector(g_dim1, beginData2, logger);
    assert(begin2 != nullptr);

    double *endData2 = new(std::nothrow) double[g_dim1];
    assert(endData2 != nullptr);
    std::memcpy(endData2, g_data1FarRight, g_dim1 * sizeof(double));
    IVector *end2 = IVector::createVector(g_dim1, endData2, logger);
    assert(end2 != nullptr);

    ICompact *comp2 = ICompact::createCompact(begin2, end2, EPS, logger);
    assert(comp2 != nullptr);

    delete begin2;
    delete end2;

    bool isSubset;
    ReturnCode rc = comp2->isSubset(comp1, isSubset);
    delete comp1;
    delete comp2;

    testName = const_cast<char *>(__FUNCTION__); return (rc == ReturnCode::RC_SUCCESS && !isSubset);
}

bool intersects_NullPtr_NotSuccess(ILogger *logger, char *& testName) {
    double *beginData = new(std::nothrow) double[g_dim1];
    assert(beginData != nullptr);
    std::memcpy(beginData, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin = IVector::createVector(g_dim1, beginData, logger);
    assert(begin != nullptr);

    double *endData = new(std::nothrow) double[g_dim1];
    assert(endData != nullptr);
    std::memcpy(endData, g_data1Right, g_dim1 * sizeof(double));
    IVector *end = IVector::createVector(g_dim1, endData, logger);
    assert(end != nullptr);

    ICompact *comp = ICompact::createCompact(begin, end, EPS, logger);
    assert(comp != nullptr);

    delete begin;
    delete end;

    bool isIntersects;
    ReturnCode rc = comp->intersects(nullptr, isIntersects);
    delete comp;

    testName = const_cast<char *>(__FUNCTION__); return (rc != ReturnCode::RC_SUCCESS && !isIntersects);
}

bool intersects_WrongDim_NotSuccess(ILogger *logger, char *& testName) {
    double *beginData1 = new(std::nothrow) double[g_dim1];
    assert(beginData1 != nullptr);
    std::memcpy(beginData1, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin1 = IVector::createVector(g_dim1, beginData1, logger);
    assert(begin1 != nullptr);

    double *endData1 = new(std::nothrow) double[g_dim1];
    assert(endData1 != nullptr);
    std::memcpy(endData1, g_data1Right, g_dim1 * sizeof(double));
    IVector *end1 = IVector::createVector(g_dim1, endData1, logger);
    assert(end1 != nullptr);

    ICompact *comp1 = ICompact::createCompact(begin1, end1, EPS, logger);
    assert(comp1 != nullptr);

    delete begin1;
    delete end1;

    double *beginData2 = new(std::nothrow) double[g_dim2];
    assert(beginData2 != nullptr);
    std::memcpy(beginData2, g_data2BottomLeft, g_dim2 * sizeof(double));
    IVector *begin2 = IVector::createVector(g_dim2, beginData2, logger);
    assert(begin2 != nullptr);

    double *endData2 = new(std::nothrow) double[g_dim2];
    assert(endData2 != nullptr);
    std::memcpy(endData2, g_data2TopRight, g_dim2 * sizeof(double));
    IVector *end2 = IVector::createVector(g_dim2, endData2, logger);
    assert(end2 != nullptr);

    ICompact *comp2 = ICompact::createCompact(begin2, end2, EPS, logger);
    assert(comp2 != nullptr);

    delete begin2;
    delete end2;

    bool isIntersets;
    ReturnCode rc = comp1->intersects(comp2, isIntersets);
    delete comp1;
    delete comp2;

    testName = const_cast<char *>(__FUNCTION__); return (rc != ReturnCode::RC_SUCCESS && !isIntersets);
}

bool intersects_CheckIntersecting_Success(ILogger *logger, char *& testName) {
    double *beginData1 = new(std::nothrow) double[g_dim1];
    assert(beginData1 != nullptr);
    std::memcpy(beginData1, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin1 = IVector::createVector(g_dim1, beginData1, logger);
    assert(begin1 != nullptr);

    double *endData1 = new(std::nothrow) double[g_dim1];
    assert(endData1 != nullptr);
    std::memcpy(endData1, g_data1Right, g_dim1 * sizeof(double));
    IVector *end1 = IVector::createVector(g_dim1, endData1, logger);
    assert(end1 != nullptr);

    ICompact *comp1 = ICompact::createCompact(begin1, end1, EPS, logger);
    assert(comp1 != nullptr);

    delete begin1;
    delete end1;

    double *beginData2 = new(std::nothrow) double[g_dim1];
    assert(beginData2 != nullptr);
    std::memcpy(beginData2, g_data1Left, g_dim1 * sizeof(double));
    IVector *begin2 = IVector::createVector(g_dim1, beginData2, logger);
    assert(begin2 != nullptr);

    double *endData2 = new(std::nothrow) double[g_dim1];
    assert(endData2 != nullptr);
    std::memcpy(endData2, g_data1FarRight, g_dim1 * sizeof(double));
    IVector *end2 = IVector::createVector(g_dim1, endData2, logger);
    assert(end2 != nullptr);

    ICompact *comp2 = ICompact::createCompact(begin2, end2, EPS, logger);
    assert(comp2 != nullptr);

    delete begin2;
    delete end2;

    bool isIntersects;
    ReturnCode rc = comp2->intersects(comp1, isIntersects);
    delete comp1;
    delete comp2;

    testName = const_cast<char *>(__FUNCTION__); return (rc == ReturnCode::RC_SUCCESS && isIntersects);
}

bool intersects_CheckNotIntersecting_Success(ILogger *logger, char *& testName) {
    double *beginData1 = new(std::nothrow) double[g_dim1];
    assert(beginData1 != nullptr);
    std::memcpy(beginData1, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin1 = IVector::createVector(g_dim1, beginData1, logger);
    assert(begin1 != nullptr);

    double *endData1 = new(std::nothrow) double[g_dim1];
    assert(endData1 != nullptr);
    std::memcpy(endData1, g_data1Left, g_dim1 * sizeof(double));
    IVector *end1 = IVector::createVector(g_dim1, endData1, logger);
    assert(end1 != nullptr);

    ICompact *comp1 = ICompact::createCompact(begin1, end1, EPS, logger);
    assert(comp1 != nullptr);

    delete begin1;
    delete end1;

    double *beginData2 = new(std::nothrow) double[g_dim1];
    assert(beginData2 != nullptr);
    std::memcpy(beginData2, g_data1Right, g_dim1 * sizeof(double));
    IVector *begin2 = IVector::createVector(g_dim1, beginData2, logger);
    assert(begin2 != nullptr);

    double *endData2 = new(std::nothrow) double[g_dim1];
    assert(endData2 != nullptr);
    std::memcpy(endData2, g_data1FarRight, g_dim1 * sizeof(double));
    IVector *end2 = IVector::createVector(g_dim1, endData2, logger);
    assert(end2 != nullptr);

    ICompact *comp2 = ICompact::createCompact(begin2, end2, EPS, logger);
    assert(comp2 != nullptr);

    delete begin2;
    delete end2;

    bool isIntersects;
    ReturnCode rc = comp2->intersects(comp1, isIntersects);
    delete comp1;
    delete comp2;

    testName = const_cast<char *>(__FUNCTION__); return (rc == ReturnCode::RC_SUCCESS && !isIntersects);
}

bool union_NullPtr_NullPtr(ILogger *logger, char *& testName) {
    ICompact *_unionComp = ICompact::_union(nullptr, nullptr, EPS, logger);

    bool passed = (_unionComp == nullptr);
    if (!passed) delete _unionComp;
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool union_WrongDim_NullPtr(ILogger *logger, char *& testName) {
    double *beginData1 = new(std::nothrow) double[g_dim1];
    assert(beginData1 != nullptr);
    std::memcpy(beginData1, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin1 = IVector::createVector(g_dim1, beginData1, logger);
    assert(begin1 != nullptr);

    double *endData1 = new(std::nothrow) double[g_dim1];
    assert(endData1 != nullptr);
    std::memcpy(endData1, g_data1Right, g_dim1 * sizeof(double));
    IVector *end1 = IVector::createVector(g_dim1, endData1, logger);
    assert(end1 != nullptr);

    ICompact *comp1 = ICompact::createCompact(begin1, end1, EPS, logger);
    assert(comp1 != nullptr);

    delete begin1;
    delete end1;

    double *beginData2 = new(std::nothrow) double[g_dim2];
    assert(beginData2 != nullptr);
    std::memcpy(beginData2, g_data2BottomLeft, g_dim2 * sizeof(double));
    IVector *begin2 = IVector::createVector(g_dim2, beginData2, logger);
    assert(begin2 != nullptr);

    double *endData2 = new(std::nothrow) double[g_dim2];
    assert(endData2 != nullptr);
    std::memcpy(endData2, g_data2TopRight, g_dim2 * sizeof(double));
    IVector *end2 = IVector::createVector(g_dim2, endData2, logger);
    assert(end2 != nullptr);

    ICompact *comp2 = ICompact::createCompact(begin2, end2, EPS, logger);
    assert(comp2 != nullptr);

    delete begin2;
    delete end2;

    ICompact *_unionComp = ICompact::_union(comp1, comp2, EPS, logger);
    delete comp1;
    delete comp2;

    bool passed = (_unionComp == nullptr);
    if (!passed) delete _unionComp;
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool union_NaNTolerance_NullPtr(ILogger *logger, char *& testName) {
    double *beginData1 = new(std::nothrow) double[g_dim1];
    assert(beginData1 != nullptr);
    std::memcpy(beginData1, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin1 = IVector::createVector(g_dim1, beginData1, logger);
    assert(begin1 != nullptr);

    double *endData1 = new(std::nothrow) double[g_dim1];
    assert(endData1 != nullptr);
    std::memcpy(endData1, g_data1Right, g_dim1 * sizeof(double));
    IVector *end1 = IVector::createVector(g_dim1, endData1, logger);
    assert(end1 != nullptr);

    ICompact *comp1 = ICompact::createCompact(begin1, end1, EPS, logger);
    assert(comp1 != nullptr);

    delete begin1;
    delete end1;

    double *beginData2 = new(std::nothrow) double[g_dim1];
    assert(beginData2 != nullptr);
    std::memcpy(beginData2, g_data1Left, g_dim1 * sizeof(double));
    IVector *begin2 = IVector::createVector(g_dim1, beginData2, logger);
    assert(begin2 != nullptr);

    double *endData2 = new(std::nothrow) double[g_dim1];
    assert(endData2 != nullptr);
    std::memcpy(endData2, g_data1FarRight, g_dim1 * sizeof(double));
    IVector *end2 = IVector::createVector(g_dim1, endData2, logger);
    assert(end2 != nullptr);

    ICompact *comp2 = ICompact::createCompact(begin2, end2, EPS, logger);
    assert(comp2 != nullptr);

    delete begin2;
    delete end2;

    ICompact *_unionComp = ICompact::_union(comp1, comp2, NAN, logger);
    delete comp1;
    delete comp2;

    bool passed = (_unionComp == nullptr);
    if (!passed) delete _unionComp;
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool union_NegativeTolerance_NullPtr(ILogger *logger, char *& testName) {
    double *beginData1 = new(std::nothrow) double[g_dim1];
    assert(beginData1 != nullptr);
    std::memcpy(beginData1, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin1 = IVector::createVector(g_dim1, beginData1, logger);
    assert(begin1 != nullptr);

    double *endData1 = new(std::nothrow) double[g_dim1];
    assert(endData1 != nullptr);
    std::memcpy(endData1, g_data1Right, g_dim1 * sizeof(double));
    IVector *end1 = IVector::createVector(g_dim1, endData1, logger);
    assert(end1 != nullptr);

    ICompact *comp1 = ICompact::createCompact(begin1, end1, EPS, logger);
    assert(comp1 != nullptr);

    delete begin1;
    delete end1;

    double *beginData2 = new(std::nothrow) double[g_dim1];
    assert(beginData2 != nullptr);
    std::memcpy(beginData2, g_data1Left, g_dim1 * sizeof(double));
    IVector *begin2 = IVector::createVector(g_dim1, beginData2, logger);
    assert(begin2 != nullptr);

    double *endData2 = new(std::nothrow) double[g_dim1];
    assert(endData2 != nullptr);
    std::memcpy(endData2, g_data1FarRight, g_dim1 * sizeof(double));
    IVector *end2 = IVector::createVector(g_dim1, endData2, logger);
    assert(end2 != nullptr);

    ICompact *comp2 = ICompact::createCompact(begin2, end2, EPS, logger);
    assert(comp2 != nullptr);

    delete begin2;
    delete end2;

    ICompact *_unionComp = ICompact::_union(comp1, comp2, -EPS, logger);
    delete comp1;
    delete comp2;

    bool passed = (_unionComp == nullptr);
    if (!passed) delete _unionComp;
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool union_UnionNotIntersecting_ICompactPtr(ILogger *logger, char *& testName) {
    double *beginData1 = new(std::nothrow) double[g_dim1];
    assert(beginData1 != nullptr);
    std::memcpy(beginData1, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin1 = IVector::createVector(g_dim1, beginData1, logger);
    assert(begin1 != nullptr);

    double *endData1 = new(std::nothrow) double[g_dim1];
    assert(endData1 != nullptr);
    std::memcpy(endData1, g_data1Left, g_dim1 * sizeof(double));
    IVector *end1 = IVector::createVector(g_dim1, endData1, logger);
    assert(end1 != nullptr);

    ICompact *comp1 = ICompact::createCompact(begin1, end1, EPS, logger);
    assert(comp1 != nullptr);

    delete begin1;
    delete end1;

    double *beginData2 = new(std::nothrow) double[g_dim1];
    assert(beginData2 != nullptr);
    std::memcpy(beginData2, g_data1Right, g_dim1 * sizeof(double));
    IVector *begin2 = IVector::createVector(g_dim1, beginData2, logger);
    assert(begin2 != nullptr);

    double *endData2 = new(std::nothrow) double[g_dim1];
    assert(endData2 != nullptr);
    std::memcpy(endData2, g_data1FarRight, g_dim1 * sizeof(double));
    IVector *end2 = IVector::createVector(g_dim1, endData2, logger);
    assert(end2 != nullptr);

    ICompact *comp2 = ICompact::createCompact(begin2, end2, EPS, logger);
    assert(comp2 != nullptr);

    delete begin2;
    delete end2;

    ICompact *_unionComp = ICompact::_union(comp1, comp2, EPS, logger);
    delete comp1;
    delete comp2;

    bool passed = (_unionComp != nullptr);
    if (passed) {
        IVector *begin = _unionComp->getBegin();
        IVector *end = _unionComp->getEnd();
        assert(begin != nullptr);
        assert(end != nullptr);
        passed &= (begin->getCoord(0) == g_data1FarLeft[0] && end->getCoord(0) == g_data1FarRight[0]);
        delete _unionComp;
        delete begin;
        delete end;
    }
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool union_UnionIntersectingOn1Axis_ICompactPtr(ILogger *logger, char *& testName) {
    testName = const_cast<char *>(__FUNCTION__); return false;
}

bool union_UnionIntersectingOnSeveralAxes_ICompactPtr(ILogger *logger, char *& testName) {
    double *beginData1 = new(std::nothrow) double[g_dim2];
    assert(beginData1 != nullptr);
    std::memcpy(beginData1, g_data2BottomLeft, g_dim2 * sizeof(double));
    IVector *begin1 = IVector::createVector(g_dim2, beginData1, logger);
    assert(begin1 != nullptr);

    double *endData1 = new(std::nothrow) double[g_dim2];
    assert(endData1 != nullptr);
    std::memcpy(endData1, g_data2CenterRight, g_dim2 * sizeof(double));
    IVector *end1 = IVector::createVector(g_dim2, endData1, logger);
    assert(end1 != nullptr);

    ICompact *comp1 = ICompact::createCompact(begin1, end1, EPS, logger);
    assert(comp1 != nullptr);

    delete begin1;
    delete end1;

    double *beginData2 = new(std::nothrow) double[g_dim2];
    assert(beginData2 != nullptr);
    std::memcpy(beginData2, g_data2CenterLeft, g_dim2 * sizeof(double));
    IVector *begin2 = IVector::createVector(g_dim2, beginData2, logger);
    assert(begin2 != nullptr);

    double *endData2 = new(std::nothrow) double[g_dim2];
    assert(endData2 != nullptr);
    std::memcpy(endData2, g_data2TopRight, g_dim2 * sizeof(double));
    IVector *end2 = IVector::createVector(g_dim2, endData2, logger);
    assert(end2 != nullptr);

    ICompact *comp2 = ICompact::createCompact(begin2, end2, EPS, logger);
    assert(comp2 != nullptr);

    delete begin2;
    delete end2;

    ICompact *_unionComp = ICompact::_union(comp1, comp2, EPS, logger);
    delete comp1;
    delete comp2;

    bool passed = (_unionComp == nullptr);
    if (!passed) delete _unionComp;
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool union_UnionIncluding_ICompactPtr(ILogger *logger, char *& testName) {
    double *beginData1 = new(std::nothrow) double[g_dim2];
    assert(beginData1 != nullptr);
    std::memcpy(beginData1, g_data2BottomLeft, g_dim2 * sizeof(double));
    IVector *begin1 = IVector::createVector(g_dim2, beginData1, logger);
    assert(begin1 != nullptr);

    double *endData1 = new(std::nothrow) double[g_dim2];
    assert(endData1 != nullptr);
    std::memcpy(endData1, g_data2TopRight, g_dim2 * sizeof(double));
    IVector *end1 = IVector::createVector(g_dim2, endData1, logger);
    assert(end1 != nullptr);

    ICompact *comp1 = ICompact::createCompact(begin1, end1, EPS, logger);
    assert(comp1 != nullptr);

    double *beginData2 = new(std::nothrow) double[g_dim2];
    assert(beginData2 != nullptr);
    std::memcpy(beginData2, g_data2CenterLeft, g_dim2 * sizeof(double));
    IVector *begin2 = IVector::createVector(g_dim2, beginData2, logger);
    assert(begin2 != nullptr);

    double *endData2 = new(std::nothrow) double[g_dim2];
    assert(endData2 != nullptr);
    std::memcpy(endData2, g_data2CenterRight, g_dim2 * sizeof(double));
    IVector *end2 = IVector::createVector(g_dim2, endData2, logger);
    assert(end2 != nullptr);

    ICompact *comp2 = ICompact::createCompact(begin2, end2, EPS, logger);
    assert(comp2 != nullptr);

    delete begin2;
    delete end2;

    ICompact *_unionComp = ICompact::_union(comp1, comp2, EPS, logger);
    delete comp1;
    delete comp2;

    bool passed = (_unionComp != nullptr);
    if (passed) {
        IVector *begin = _unionComp->getBegin();
        IVector *end = _unionComp->getEnd();
        assert(begin != nullptr);
        assert(end != nullptr);
        bool beginEquality, endEquality;
        IVector::equals(begin, begin1, IVector::Norm::NORM_1, EPS, beginEquality);
        IVector::equals(end, end1, IVector::Norm::NORM_1, EPS, endEquality);
        passed &= (beginEquality && endEquality);
        delete _unionComp;
        delete begin;
        delete end;
    }
    delete begin1;
    delete end1;
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool convex_NullPtr_NullPtr(ILogger *logger, char *& testName) {
    ICompact *convexComp = ICompact::convex(nullptr, nullptr, EPS, logger);

    bool passed = (convexComp == nullptr);
    if (!passed) delete convexComp;
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool convex_WrongDim_NullPtr(ILogger *logger, char *& testName) {
    double *beginData1 = new(std::nothrow) double[g_dim1];
    assert(beginData1 != nullptr);
    std::memcpy(beginData1, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin1 = IVector::createVector(g_dim1, beginData1, logger);
    assert(begin1 != nullptr);

    double *endData1 = new(std::nothrow) double[g_dim1];
    assert(endData1 != nullptr);
    std::memcpy(endData1, g_data1Right, g_dim1 * sizeof(double));
    IVector *end1 = IVector::createVector(g_dim1, endData1, logger);
    assert(end1 != nullptr);

    ICompact *comp1 = ICompact::createCompact(begin1, end1, EPS, logger);
    assert(comp1 != nullptr);

    delete begin1;
    delete end1;

    double *beginData2 = new(std::nothrow) double[g_dim2];
    assert(beginData2 != nullptr);
    std::memcpy(beginData2, g_data2BottomLeft, g_dim2 * sizeof(double));
    IVector *begin2 = IVector::createVector(g_dim2, beginData2, logger);
    assert(begin2 != nullptr);

    double *endData2 = new(std::nothrow) double[g_dim2];
    assert(endData2 != nullptr);
    std::memcpy(endData2, g_data2TopRight, g_dim2 * sizeof(double));
    IVector *end2 = IVector::createVector(g_dim2, endData2, logger);
    assert(end2 != nullptr);

    ICompact *comp2 = ICompact::createCompact(begin2, end2, EPS, logger);
    assert(comp2 != nullptr);

    delete begin2;
    delete end2;

    ICompact *convexComp = ICompact::convex(comp1, comp2, EPS, logger);
    delete comp1;
    delete comp2;

    bool passed = (convexComp == nullptr);
    if (!passed) delete convexComp;
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool convex_NaNTolerance_NullPtr(ILogger *logger, char *& testName) {
    testName = const_cast<char *>(__FUNCTION__); return false;
}

bool convex_NegativeTolerance_NullPtr(ILogger *logger, char *& testName) {
    testName = const_cast<char *>(__FUNCTION__); return false;
}

bool convex_UnionNotIntersecting_ICompactPtr(ILogger *logger, char *& testName) {
    testName = const_cast<char *>(__FUNCTION__); return false;
}

bool convex_UnionIntersecting_ICompactPtr(ILogger *logger, char *& testName) {
    testName = const_cast<char *>(__FUNCTION__); return false;
}

bool convex_UnionIncluding_ICompactPtr(ILogger *logger, char *& testName) {
    testName = const_cast<char *>(__FUNCTION__); return false;
}

bool intersection_NullPtr_NullPtr(ILogger *logger, char *& testName) {
    ICompact *intsctComp = ICompact::intersection(nullptr, nullptr, EPS, logger);

    bool passed = (intsctComp == nullptr);
    if (!passed) delete intsctComp;
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool intersection_WrongDim_NullPtr(ILogger *logger, char *& testName) {
    double *beginData1 = new(std::nothrow) double[g_dim1];
    assert(beginData1 != nullptr);
    std::memcpy(beginData1, g_data1FarLeft, g_dim1 * sizeof(double));
    IVector *begin1 = IVector::createVector(g_dim1, beginData1, logger);
    assert(begin1 != nullptr);

    double *endData1 = new(std::nothrow) double[g_dim1];
    assert(endData1 != nullptr);
    std::memcpy(endData1, g_data1Right, g_dim1 * sizeof(double));
    IVector *end1 = IVector::createVector(g_dim1, endData1, logger);
    assert(end1 != nullptr);

    ICompact *comp1 = ICompact::createCompact(begin1, end1, EPS, logger);
    assert(comp1 != nullptr);

    delete begin1;
    delete end1;

    double *beginData2 = new(std::nothrow) double[g_dim2];
    assert(beginData2 != nullptr);
    std::memcpy(beginData2, g_data2BottomLeft, g_dim2 * sizeof(double));
    IVector *begin2 = IVector::createVector(g_dim2, beginData2, logger);
    assert(begin2 != nullptr);

    double *endData2 = new(std::nothrow) double[g_dim2];
    assert(endData2 != nullptr);
    std::memcpy(endData2, g_data2TopRight, g_dim2 * sizeof(double));
    IVector *end2 = IVector::createVector(g_dim2, endData2, logger);
    assert(end2 != nullptr);

    ICompact *comp2 = ICompact::createCompact(begin2, end2, EPS, logger);
    assert(comp2 != nullptr);

    delete begin2;
    delete end2;

    ICompact *intsctComp = ICompact::intersection(comp1, comp2, EPS, logger);
    delete comp1;
    delete comp2;

    bool passed = (intsctComp == nullptr);
    if (!passed) delete intsctComp;
    testName = const_cast<char *>(__FUNCTION__); return passed;
}

bool intersection_NaNTolerance_NullPtr(ILogger *logger, char *& testName) {
    testName = const_cast<char *>(__FUNCTION__); return false;
}

bool intersection_NegativeTolerance_NullPtr(ILogger *logger, char *& testName) {
    testName = const_cast<char *>(__FUNCTION__); return false;
}

bool intersection_UnionNotIntersecting_ICompactPtr(ILogger *logger, char *& testName) {
    testName = const_cast<char *>(__FUNCTION__); return false;
}

bool intersection_UnionIntersecting_ICompactPtr(ILogger *logger, char *& testName) {
    testName = const_cast<char *>(__FUNCTION__); return false;
}

bool intersection_UnionIncluding_ICompactPtr(ILogger *logger, char *& testName) {
    testName = const_cast<char *>(__FUNCTION__); return false;
}

bool intersection_DegeneracyCheck_NullPtr(ILogger *logger, char *& testName) {
    testName = const_cast<char *>(__FUNCTION__); return false;
}


#endif //TESTCOMPACT_H
