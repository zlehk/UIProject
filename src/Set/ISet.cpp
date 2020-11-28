#include "ISet.h"
#include "SetImpl.cpp"

ISet::~ISet() {}

ISet *ISet::createSet(ILogger *logger) {
    ISet *set = new(std::nothrow) SetImpl();
    if (set == nullptr) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
    }
    return set;
} //OK

ISet *ISet::_union(const ISet *set1, const ISet *set2, IVector::Norm norm, double tolerance, ILogger *logger) {
    if (set1 == nullptr || set2 == nullptr) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_NULL_PTR);
        return nullptr;
    }

    if (set1->getDim() == 0 && set2->getDim() == 0)
        return set1->clone();
    else if (set1->getDim() == 0)
        return set2->clone();
    else if (set2->getDim() == 0)
        return set1->clone();

    if (set1->getDim() != set2->getDim()) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_WRONG_DIM);
        return nullptr;
    }

    if (std::isnan(tolerance)) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_NAN);
        return nullptr;
    }
    if (tolerance < 0) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_INVALID_PARAMS);
        return nullptr;
    }

    ISet *uni = set1->clone();
    if (uni == nullptr) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        return nullptr;
    }

    IVector *vec = nullptr;
    ReturnCode rc;
    for (size_t i = 0; i < set2->getSize(); ++i) {
        rc = set2->get(vec, i);
        if (vec != nullptr) {
            rc = uni->insert(vec, norm, tolerance);
            delete vec;
            vec = nullptr;
        }
        else
            SETLOG(logger, MSG_DEFAULT, rc);
    }

    return uni;

} //OK

ISet *ISet::difference(const ISet *minuend, const ISet *subtrahend, IVector::Norm norm, double tolerance, ILogger *logger) {
    if (minuend == nullptr || subtrahend == nullptr) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_NULL_PTR);
        return nullptr;
    }

    if (subtrahend->getDim() == 0)
        return minuend->clone();

    if (minuend->getDim() != subtrahend->getDim()) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_WRONG_DIM);
        return nullptr;
    }

    if (std::isnan(tolerance)) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_NAN);
        return nullptr;
    }
    if (tolerance < 0) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_INVALID_PARAMS);
        return nullptr;
    }

    ISet *diff = minuend->clone();
    if (diff == nullptr) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    IVector *vec = nullptr;
    ReturnCode rc;
    for (size_t i = 0; i < subtrahend->getSize(); ++i) {
        rc = subtrahend->get(vec, i);
        if (vec != nullptr) {
            diff->erase(vec, norm, tolerance);
            delete vec;
            vec = nullptr;
        }
        else
            SETLOG(logger, MSG_DEFAULT, rc);
    }
    return diff;
} //OK

ISet *ISet::symmetricDifference(const ISet *set1, const ISet *set2, IVector::Norm norm, double tolerance, ILogger *logger) {
    if (set1 == nullptr || set2 == nullptr) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_NULL_PTR);
        return nullptr;
    }

    if (set1->getDim() == 0 && set2->getDim() == 0)
        return set1->clone();
    else if (set1->getDim() == 0)
        return set2->clone();
    else if (set2->getDim() == 0)
        return set1->clone();

    if (set1->getDim() != set2->getDim()) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_WRONG_DIM);
        return nullptr;
    }

    if (std::isnan(tolerance)) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_NAN);
        return nullptr;
    }
    if (tolerance < 0) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_INVALID_PARAMS);
        return nullptr;
    }

    ISet *unique_set1 = set1->clone();
    if (unique_set1 == nullptr) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    ISet *unique_set2 = set2->clone();
    if (unique_set2 == nullptr) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete unique_set1;
        return nullptr;
    }
    IVector *vec = nullptr;
    size_t index;
    ReturnCode rc;
    for (size_t i = 0; i < set1->getSize(); ++i) {
        rc = set1->get(vec, i);
        if (vec != nullptr && set2->find(vec, norm, tolerance, index) == ReturnCode::RC_SUCCESS) {
            unique_set1->erase(vec, norm, tolerance);
            unique_set2->erase(vec, norm, tolerance);
            delete vec;
            vec = nullptr;
        }
        else
            SETLOG(logger, MSG_DEFAULT, rc);
    }

    ISet* symmdiff = _union(unique_set1, unique_set2, norm, tolerance, logger);
    delete unique_set1;
    delete unique_set2;
    return symmdiff;

} //OK

ISet *ISet::intersection(const ISet *set1, const ISet *set2, IVector::Norm norm, double tolerance, ILogger *logger) {
    if (set1 == nullptr || set2 == nullptr) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_NULL_PTR);
        return nullptr;
    }

    if (set1->getDim() == 0 || set2->getDim() == 0)
        return set1->getDim() == 0 ? set1->clone() : set2->clone();

    if (set1->getDim() != set2->getDim()) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_WRONG_DIM);
        return nullptr;
    }

    if (std::isnan(tolerance)) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_NAN);
        return nullptr;
    }
    if (tolerance < 0) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_INVALID_PARAMS);
        return nullptr;
    }

    ISet *intsct = set1->clone();
    if (intsct == nullptr) {
        SETLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    IVector *vec = nullptr;
    size_t index;
    ReturnCode rc;
    for (size_t i = 0; i < set1->getSize(); ++i) {
        rc = set1->get(vec, i);
        if (vec != nullptr && set2->find(vec, norm, tolerance, index) == ReturnCode::RC_ELEM_NOT_FOUND) {
            intsct->erase(vec, norm, tolerance);
            delete vec;
            vec = nullptr;
        }
        else
            SETLOG(logger, MSG_DEFAULT, rc);
    }
    return intsct;
} //OK

