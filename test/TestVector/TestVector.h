#ifndef TESTVECTOR_H
#define TESTVECTOR_H

#include <new>
#include <cassert>
#include <cmath>
#include <cstring>

#include "../include/ILogger.h"
#include "../include/IVector.h"

#define EPS 1e-6

static const size_t g_dim0 = 0;
static const size_t g_dim1 = 1;
static const size_t g_dim2 = 2;

static const double  (&g_data1)[g_dim1] = {-1.0};
static const double  (&g_data1Nan)[g_dim1] = {NAN};
static const double  (&g_data2)[g_dim2] = {-1.0, 2.0};


bool createVector_WrongDim_NullPtr(ILogger *logger, char *&testName) {
    IVector *vecNull = IVector::createVector(g_dim0, nullptr, logger);

    bool passed = (vecNull == nullptr);
    if (!passed) delete vecNull;
    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool createVector_NullPtr_NullPtr(ILogger *logger, char *&testName) {
    IVector *vecNull = IVector::createVector(g_dim1, nullptr, logger);

    bool passed = (vecNull == nullptr);
    if (!passed) delete vecNull;
    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool createVector_NaNValue_NullPtr(ILogger *logger, char *&testName) {
    double *data = new(std::nothrow) double[g_dim1];
    assert(data != nullptr);
    std::memcpy(data, g_data1Nan, g_dim1 * sizeof(double));
    IVector *vecNull = IVector::createVector(g_dim1, data, logger);

    bool passed = (vecNull == nullptr);
    if (!passed) delete vecNull;
    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool createVector_Ok_IVectorPtr(ILogger *logger, char *&testName) {
    double *data = new(std::nothrow) double[g_dim1];
    assert(data != nullptr);
    std::memcpy(data, g_data1, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data, logger);

    bool passed = (vec1 != nullptr);
    if (passed) delete vec1;
    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool getCoord_IndexOutOfRange_NaN(ILogger *logger, char *&testName) {
    double *data = new(std::nothrow) double[g_dim1];
    assert(data != nullptr);
    std::memcpy(data, g_data1, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data, logger);
    assert(vec1 != nullptr);

    double coord = vec1->getCoord(g_dim1);
    delete vec1;
    testName = const_cast<char *>(__FUNCTION__);
    return std::isnan(coord);
}

bool getCoord_Ok_CoordByIndex(ILogger *logger, char *&testName) {
    double *data = new(std::nothrow) double[g_dim1];
    assert(data != nullptr);
    std::memcpy(data, g_data1, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data, logger);
    assert(vec1 != nullptr);

    double coord = vec1->getCoord(g_dim1 - 1);
    delete vec1;
    testName = const_cast<char *>(__FUNCTION__);
    return coord == g_data1[g_dim1 - 1];
}

bool setCoord_IndexOutOfRange_NotSuccess(ILogger *logger, char *&testName) {
    double *data = new(std::nothrow) double[g_dim1];
    assert(data != nullptr);
    std::memcpy(data, g_data1, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data, logger);
    assert(vec1 != nullptr);

    ReturnCode rc = vec1->setCoord(g_dim1, 4.0);
    delete vec1;
    testName = const_cast<char *>(__FUNCTION__);
    return rc != ReturnCode::RC_SUCCESS;
}

bool setCoord_NaNValue_NotSuccess(ILogger *logger, char *&testName) {
    double *data = new(std::nothrow) double[g_dim1];
    assert(data != nullptr);
    std::memcpy(data, g_data1, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data, logger);
    assert(vec1 != nullptr);

    ReturnCode rc = vec1->setCoord(g_dim1 - 1, NAN);
    delete vec1;
    testName = const_cast<char *>(__FUNCTION__);
    return rc != ReturnCode::RC_SUCCESS;
}

bool setCoord_Ok_Success(ILogger *logger, char *&testName) {
    double *data = new(std::nothrow) double[g_dim1];
    assert(data != nullptr);
    std::memcpy(data, g_data1, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data, logger);
    assert(vec1 != nullptr);

    ReturnCode rc = vec1->setCoord(g_dim1 - 1, 4.0);
    double newCoord = vec1->getCoord(g_dim1 - 1);
    delete vec1;
    testName = const_cast<char *>(__FUNCTION__);
    return rc == ReturnCode::RC_SUCCESS && newCoord == 4.0;
}

bool norm_InvalidNorm_NaN(ILogger *logger, char *&testName) {
    double *data = new(std::nothrow) double[g_dim2];
    assert(data != nullptr);
    std::memcpy(data, g_data2, g_dim2 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim2, data, logger);
    assert(vec2 != nullptr);

    double normValue = vec2->norm((IVector::Norm) 10);
    delete vec2;
    testName = const_cast<char *>(__FUNCTION__);
    return std::isnan(normValue);
}

bool norm_Ok_Norm1Value(ILogger *logger, char *&testName) {
    double *data = new(std::nothrow) double[g_dim2];
    assert(data != nullptr);
    std::memcpy(data, g_data2, g_dim2 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim2, data, logger);
    assert(vec2 != nullptr);

    double normValue = vec2->norm(IVector::Norm::NORM_1);
    delete vec2;
    testName = const_cast<char *>(__FUNCTION__);
    return normValue == (std::fabs(g_data2[0]) + std::fabs(g_data2[1]));
}

bool norm_Ok_Norm2Value(ILogger *logger, char *&testName) {
    double *data = new(std::nothrow) double[g_dim2];
    assert(data != nullptr);
    std::memcpy(data, g_data2, g_dim2 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim2, data, logger);
    assert(vec2 != nullptr);

    double normValue = vec2->norm(IVector::Norm::NORM_2);
    delete vec2;
    testName = const_cast<char *>(__FUNCTION__);
    return normValue == std::sqrt(g_data2[0] * g_data2[0] + g_data2[1] * g_data2[1]);
}

bool norm_Ok_NormInfValue(ILogger *logger, char *&testName) {
    double *data = new(std::nothrow) double[g_dim2];
    assert(data != nullptr);
    std::memcpy(data, g_data2, g_dim2 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim2, data, logger);
    assert(vec2 != nullptr);

    double normValue = vec2->norm(IVector::Norm::NORM_INF);
    delete vec2;
    testName = const_cast<char *>(__FUNCTION__);
    return normValue == (std::fabs(g_data2[0]) > std::fabs(g_data2[1]) ? std::fabs(g_data2[0]) : std::fabs(g_data2[1]));
}

bool equal_NullPtr_NullPtr(ILogger *logger, char *&testName) {
    bool isEqual;
    ReturnCode rc = IVector::equals(nullptr, nullptr, IVector::Norm::NORM_1, EPS, isEqual, logger);
    testName = const_cast<char *>(__FUNCTION__);
    return rc != ReturnCode::RC_SUCCESS && !isEqual;
}

bool equal_WrongDim_NullPtr(ILogger *logger, char *&testName) {
    double *data2 = new(std::nothrow) double[g_dim2];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data2, g_dim2 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim2, data2, logger);
    assert(vec2 != nullptr);
    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    bool isEqual;
    ReturnCode rc = IVector::equals(vec2, vec1, IVector::Norm::NORM_1, EPS, isEqual, logger);
    delete vec2;
    delete vec1;
    testName = const_cast<char *>(__FUNCTION__);
    return rc != ReturnCode::RC_SUCCESS && !isEqual;
}

bool equal_InvalidNorm_NullPtr(ILogger *logger, char *&testName) {
    double *data = new(std::nothrow) double[g_dim2];
    assert(data != nullptr);
    std::memcpy(data, g_data2, g_dim2 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim2, data, logger);
    assert(vec2 != nullptr);
    IVector *clonedVec2 = vec2->clone();
    assert(clonedVec2 != nullptr);

    bool isEqual;
    ReturnCode rc = IVector::equals(vec2, clonedVec2, (IVector::Norm) 10, EPS, isEqual, logger);
    delete vec2;
    delete clonedVec2;
    testName = const_cast<char *>(__FUNCTION__);
    return rc != ReturnCode::RC_SUCCESS && !isEqual;
}

bool equal_NaNTolerance_NullPtr(ILogger *logger, char *&testName) {
    double *data = new(std::nothrow) double[g_dim2];
    assert(data != nullptr);
    std::memcpy(data, g_data2, g_dim2 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim2, data, logger);
    assert(vec2 != nullptr);
    IVector *clonedVec2 = vec2->clone();
    assert(clonedVec2 != nullptr);

    bool isEqual;
    ReturnCode rc = IVector::equals(vec2, clonedVec2, IVector::Norm::NORM_1, NAN, isEqual, logger);
    delete vec2;
    delete clonedVec2;
    testName = const_cast<char *>(__FUNCTION__);
    return rc != ReturnCode::RC_SUCCESS && !isEqual;
}

bool equal_Ok_NotSuccess(ILogger *logger, char *&testName) {
    double *data = new(std::nothrow) double[g_dim2];
    assert(data != nullptr);
    std::memcpy(data, g_data2, g_dim2 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim2, data, logger);
    assert(vec2 != nullptr);
    IVector *clonedVec2 = vec2->clone();
    assert(clonedVec2 != nullptr);
    clonedVec2->setCoord(0, -g_data2[0]);

    bool isEqual;
    ReturnCode rc = IVector::equals(vec2, clonedVec2, IVector::Norm::NORM_1, EPS, isEqual, logger);
    delete vec2;
    delete clonedVec2;
    testName = const_cast<char *>(__FUNCTION__);
    return rc == ReturnCode::RC_SUCCESS && !isEqual;
}

bool equal_Ok_Success(ILogger *logger, char *&testName) {
    double *data = new(std::nothrow) double[g_dim2];
    assert(data != nullptr);
    std::memcpy(data, g_data2, g_dim2 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim2, data, logger);
    assert(vec2 != nullptr);
    IVector *clonedVec2 = vec2->clone();
    assert(clonedVec2 != nullptr);

    bool isEqual;
    ReturnCode rc = IVector::equals(vec2, clonedVec2, IVector::Norm::NORM_1, EPS, isEqual, logger);
    delete vec2;
    delete clonedVec2;
    testName = const_cast<char *>(__FUNCTION__);
    return rc == ReturnCode::RC_SUCCESS && isEqual;
}

bool add_NullPtr_NullPtr(ILogger *logger, char *&testName) {
    IVector *addVec = IVector::add(nullptr, nullptr, logger);

    bool passed = (addVec == nullptr);
    if (!passed) delete addVec;
    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool add_WrongDim_NullPtr(ILogger *logger, char *&testName) {
    double *data2 = new(std::nothrow) double[g_dim2];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data2, g_dim2 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim2, data2, logger);
    assert(vec2 != nullptr);
    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    IVector *addVec = IVector::add(vec2, vec1, logger);
    delete vec2;
    delete vec1;
    bool passed = (addVec == nullptr);
    if (!passed) delete addVec;
    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool add_Ok_IVectorPtr(ILogger *logger, char *&testName) {
    double *data = new(std::nothrow) double[g_dim2];
    assert(data != nullptr);
    std::memcpy(data, g_data2, g_dim2 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim2, data, logger);
    assert(vec2 != nullptr);
    IVector *clonedVec2 = vec2->clone();
    assert(clonedVec2 != nullptr);

    IVector *addVec = IVector::add(vec2, clonedVec2, logger);
    assert(addVec != nullptr);
    delete vec2;
    delete clonedVec2;
    bool passed = (std::fabs(addVec->getCoord(0) - 2 * g_data2[0]) < EPS && std::fabs(addVec->getCoord(1) - 2 * g_data2[1]) < EPS);
    delete addVec;
    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool sub_NullPtr_NullPtr(ILogger *logger, char *&testName) {
    IVector *subVec = IVector::sub(nullptr, nullptr, logger);
    bool passed = (subVec == nullptr);
    if (!passed) delete subVec;
    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool sub_WrongDim_NullPtr(ILogger *logger, char *&testName) {
    double *data2 = new(std::nothrow) double[g_dim2];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data2, g_dim2 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim2, data2, logger);
    assert(vec2 != nullptr);
    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    IVector *subVec = IVector::sub(vec2, vec1, logger);
    delete vec2;
    delete vec1;
    bool passed = (subVec == nullptr);
    if (!passed) delete subVec;
    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool sub_Ok_IVectorPtr(ILogger *logger, char *&testName) {
    double *data = new(std::nothrow) double[g_dim2];
    assert(data != nullptr);
    std::memcpy(data, g_data2, g_dim2 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim2, data, logger);
    assert(vec2 != nullptr);
    IVector *clonedVec2 = vec2->clone();
    assert(clonedVec2 != nullptr);

    IVector *subVec = IVector::sub(vec2, clonedVec2, logger);
    assert(subVec != nullptr);
    delete vec2;
    delete clonedVec2;
    bool passed = (std::fabs(subVec->getCoord(0)) < EPS && std::fabs(subVec->getCoord(1)) < EPS);
    delete subVec;
    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool mulByNumber_NullPtr_NullPtr(ILogger *logger, char *&testName) {
    IVector *mulVec = IVector::mul(nullptr, 2.0, logger);

    bool passed = (mulVec == nullptr);
    if (!passed) delete mulVec;
    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool mulByNumber_NaNValue_NullPtr(ILogger *logger, char *&testName) {
    double *data2 = new(std::nothrow) double[g_dim2];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data2, g_dim2 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim2, data2, logger);
    assert(vec2 != nullptr);

    IVector *mulVec = IVector::mul(vec2, NAN, logger);
    delete vec2;
    bool passed = (mulVec == nullptr);
    if (!passed) delete mulVec;
    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool mulByNumber_Ok_IVectorPtr(ILogger *logger, char *&testName) {
    double *data = new(std::nothrow) double[g_dim2];
    assert(data != nullptr);
    std::memcpy(data, g_data2, g_dim2 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim2, data, logger);
    assert(vec2 != nullptr);

    IVector *mulVec = IVector::mul(vec2, 2.0, logger);
    assert(mulVec != nullptr);
    delete vec2;

    bool passed = (std::fabs(mulVec->getCoord(0) - 2 * g_data2[0]) < EPS && std::fabs(mulVec->getCoord(1) - 2 * g_data2[1]) < EPS);
    delete mulVec;
    testName = const_cast<char *>(__FUNCTION__);
    return passed;
}

bool mul_NullPtr_NullPtr(ILogger *logger, char *&testName) {
    double prod = IVector::mul(nullptr, nullptr, logger);
    testName = const_cast<char *>(__FUNCTION__);
    return std::isnan(prod);
}

bool mul_WrongDim_NullPtr(ILogger *logger, char *&testName) {
    double *data2 = new(std::nothrow) double[g_dim2];
    assert(data2 != nullptr);
    std::memcpy(data2, g_data2, g_dim2 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim2, data2, logger);
    assert(vec2 != nullptr);
    double *data1 = new(std::nothrow) double[g_dim1];
    assert(data1 != nullptr);
    std::memcpy(data1, g_data1, g_dim1 * sizeof(double));
    IVector *vec1 = IVector::createVector(g_dim1, data1, logger);
    assert(vec1 != nullptr);

    double prod = IVector::mul(vec2, vec1, logger);
    delete vec2;
    delete vec1;
    testName = const_cast<char *>(__FUNCTION__);
    return std::isnan(prod);
}

bool mul_Ok_IVectorPtr(ILogger *logger, char *&testName) {
    double *data = new(std::nothrow) double[g_dim2];
    assert(data != nullptr);
    std::memcpy(data, g_data2, g_dim2 * sizeof(double));
    IVector *vec2 = IVector::createVector(g_dim2, data, logger);
    assert(vec2 != nullptr);
    IVector *clonedVec2 = vec2->clone();
    assert(clonedVec2 != nullptr);

    double prod = IVector::mul(vec2, clonedVec2, logger);
    assert(!std::isnan(prod));
    delete vec2;
    delete clonedVec2;
    testName = const_cast<char *>(__FUNCTION__);
    return (std::fabs(prod - (g_data2[0] * g_data2[0] + g_data2[1] * g_data2[1])) < EPS);
}


#endif //TESTVECTOR_H
