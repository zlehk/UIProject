#include "IVector.h"
#include "VectorImpl.cpp"

IVector::~IVector() {}

IVector *IVector::createVector(size_t dim, double *data, ILogger *logger) {
    if (dim == 0) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_ZERO_DIM);
        return nullptr;
    }
    if (data == nullptr) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_NULL_PTR);
        return nullptr;
    }
    for (size_t i = 0; i < dim; ++i) {
        if (std::isnan(data[i])) {
            VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_NAN);
            return nullptr;
        }
    }

    double *clonedData = new(std::nothrow)double[dim];
    if (clonedData == nullptr) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    for (size_t i = 0; i < dim; i++)
        clonedData[i] = data[i];

    IVector *vec = new(std::nothrow)VectorImpl(dim, clonedData);
    if (vec == nullptr) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete[]clonedData;
    }
    return vec;
} //OK

IVector *IVector::add(IVector const *addend1, IVector const *addend2, ILogger *logger) {
    if (addend1 == nullptr || addend2 == nullptr) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_NULL_PTR);
        return nullptr;
    }
    if (addend1->getDim() != addend2->getDim()) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_WRONG_DIM);
        return nullptr;
    }

    double *coords = new(std::nothrow)double[addend1->getDim()];
    if (coords == nullptr) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    for (size_t i = 0; i < addend1->getDim(); ++i) {
        coords[i] = addend1->getCoord(i) + addend2->getCoord(i);
    }

    IVector *sum = new(std::nothrow)VectorImpl(addend1->getDim(), coords);
    if (sum == nullptr) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete[]coords;
    }
    return sum;
} //OK

IVector *IVector::sub(IVector const *minuend, IVector const *subtrahend, ILogger *logger) {
    if (minuend == nullptr || subtrahend == nullptr) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_NULL_PTR);
        return nullptr;
    }
    if (minuend->getDim() != subtrahend->getDim()) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_WRONG_DIM);
        return nullptr;
    }

    double *coords = new(std::nothrow)double[minuend->getDim()];
    if (coords == nullptr) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    for (size_t i = 0; i < minuend->getDim(); ++i) {
        coords[i] = minuend->getCoord(i) - subtrahend->getCoord(i);
    }

    IVector *diff = new(std::nothrow)VectorImpl(minuend->getDim(), coords);
    if (diff == nullptr) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete[]coords;
    }
    return diff;
} //OK

IVector *IVector::mul(IVector const *multiplier, double scale, ILogger *logger) {
    if (multiplier == nullptr) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_NULL_PTR);
        return nullptr;
    }
    if (std::isnan(scale)) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_NAN);
        return nullptr;
    }

    double *coords = new(std::nothrow)double[multiplier->getDim()];
    if (coords == nullptr) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    for (size_t i = 0; i < multiplier->getDim(); ++i) {
        coords[i] = multiplier->getCoord(i) * scale;
    }

    IVector *prod = new(std::nothrow)VectorImpl(multiplier->getDim(), coords);
    if (prod == nullptr) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete[]coords;
    }
    return prod;
} //OK

double IVector::mul(IVector const *multiplier1, IVector const *multiplier2, ILogger *logger) {
    if (multiplier1 == nullptr || multiplier2 == nullptr) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_NULL_PTR);
        return std::nan("1");
    }
    if (multiplier1->getDim() != multiplier2->getDim()) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_WRONG_DIM);
        return std::nan("1");
    }

    double prod = 0;
    for (size_t i = 0; i < multiplier1->getDim(); ++i) {
        prod += multiplier1->getCoord(i) * multiplier2->getCoord(i);
    }
    return prod;
} //OK

ReturnCode IVector::equals(IVector const *v1, IVector const *v2, IVector::Norm norm, double tolerance, bool &result, ILogger *logger) {
    result = false;
    if (v1 == nullptr || v2 == nullptr) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_NULL_PTR);
        return ReturnCode::RC_NULL_PTR;
    }
    if (v1->getDim() != v2->getDim()) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_WRONG_DIM);
        return ReturnCode::RC_WRONG_DIM;
    }
    if (std::isnan(tolerance)) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_NAN);
        return ReturnCode::RC_NAN;
    }
    if (tolerance < 0) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_INVALID_PARAMS);
        return ReturnCode::RC_INVALID_PARAMS;
    }

    IVector *diff = sub(v1, v2, logger);
    if (diff == nullptr) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_INVALID_PARAMS);
        return ReturnCode::RC_INVALID_PARAMS;
    }

    double diff_norm = diff->norm(norm);
    if (std::isnan(diff_norm)) {
        VECLOG(logger, MSG_DEFAULT, ReturnCode::RC_NAN);
        return ReturnCode::RC_NAN;
    }

    result = diff_norm < tolerance;
    delete diff;

    return ReturnCode::RC_SUCCESS;
} //OK
