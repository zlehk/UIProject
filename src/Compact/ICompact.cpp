#include "ICompact.h"
#include "CompactImpl.cpp"
#include <cmath>

ICompact::~ICompact() {}
ICompact::Iterator::~Iterator() {}

enum CompactRelation {
    CR_DISPARATE = 0,
    CR_HAVE_NO_COMMON_POINTS = 1,
    CR_INTERSECTION = 2,
    CR_INCLUSION = 3,
};

static bool passDegeneracyCheck(const IVector *begin, const IVector *end, double tolerance) {
    bool isCompCorrect = true;
    for (size_t i = 0; isCompCorrect && i < begin->getDim(); ++i) {
        isCompCorrect &= (end->getCoord(i) - begin->getCoord(i) > tolerance);
    }
    return isCompCorrect;
} //OK

static CompactRelation exploreCompacts(const ICompact *comp1, const ICompact *comp2, double tolerance, const ICompact *&inclusive, ReturnCode &rc) {
    if (comp1 == nullptr || comp2 == nullptr) {
        rc = ReturnCode::RC_NULL_PTR;
        return CompactRelation::CR_DISPARATE;
    }
    if (comp1->getDim() != comp2->getDim()) {
        rc = ReturnCode::RC_WRONG_DIM;
        return CompactRelation::CR_DISPARATE;
    }

    bool isIntersection;

    rc = comp1->intersects(comp2, isIntersection);
    if (rc != ReturnCode::RC_SUCCESS)
        return CompactRelation::CR_DISPARATE;
    if (!isIntersection)
        return CompactRelation::CR_HAVE_NO_COMMON_POINTS;

    bool isSubset;

    rc = comp1->isSubset(comp2, isSubset);
    if (isSubset) {
        inclusive = comp2;
        return CompactRelation::CR_INCLUSION;
    }

    rc = comp2->isSubset(comp1, isSubset);
    if (isSubset) {
        inclusive = comp1;
        return CompactRelation::CR_INCLUSION;
    }

    if (std::isnan(tolerance)) {
        rc = ReturnCode::RC_NAN;
        return CompactRelation::CR_DISPARATE;
    }
    if (tolerance < .0) {
        rc = ReturnCode::RC_INVALID_PARAMS;
        return CompactRelation::CR_DISPARATE;
    }

    return CompactRelation::CR_INTERSECTION;
} //OK

ICompact *ICompact::createCompact(const IVector *begin, const IVector *end, double tolerance, ILogger *logger) {
    if (begin == nullptr || end == nullptr) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NULL_PTR);
        return nullptr;
    }
    if (begin->getDim() != end->getDim()) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NULL_PTR);
        return nullptr;
    }

    if (std::isnan(tolerance)) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NAN);
        return nullptr;
    }
    if (tolerance < .0) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_INVALID_PARAMS);
        return nullptr;
    }

    if (!passDegeneracyCheck(begin, end, tolerance)) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_INVALID_PARAMS);
        return nullptr;
    }

    IVector *clonedBegin = begin->clone();
    if (clonedBegin == nullptr) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    IVector *clonedEnd = end->clone();
    if (clonedEnd == nullptr) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete clonedBegin;
        return nullptr;
    }

    ICompact *compact = new(std::nothrow)CompactImpl(clonedBegin->getDim(), clonedBegin, clonedEnd);
    if (compact == nullptr) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
    }
    return compact;
} //OK

ICompact *ICompact::_union(const ICompact *comp1, const ICompact *comp2, double tolerance, ILogger *logger) {
    ReturnCode rc;
    const ICompact *inclusive = nullptr;
    ICompact *unionComp;

    CompactRelation cr = exploreCompacts(comp1, comp2, tolerance, inclusive, rc);
    if (rc != ReturnCode::RC_SUCCESS) {
        COMPLOG(logger, MSG_DEFAULT, rc);
        return nullptr;
    }
    switch (cr) {
        case CR_DISPARATE:
            return nullptr;
        case CR_HAVE_NO_COMMON_POINTS:
            return nullptr;
        case CR_INTERSECTION:
            break;
        case CR_INCLUSION:
            unionComp = inclusive == comp1 ? comp1->clone() : comp2->clone();
            if (unionComp == nullptr) {
                COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
            }
            return unionComp;
        default:
            return nullptr;
    }

    size_t unionCompDim = comp1->getDim();
    IVector *unionCompBegin = comp1->getBegin();
    if (unionCompBegin == nullptr) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    IVector *unionCompEnd = comp1->getEnd();
    if (unionCompEnd == nullptr) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete unionCompBegin;
        return nullptr;
    }
    IVector *comp2Begin = comp2->getBegin();
    if (comp2Begin == nullptr) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete unionCompBegin;
        delete unionCompEnd;
        return nullptr;
    }
    IVector *comp2End = comp2->getEnd();
    if (comp2End == nullptr) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete unionCompBegin;
        delete unionCompEnd;
        delete comp2Begin;
        return nullptr;
    }

    size_t identicalCoordsCounter = 0;
    int axisWithDifference = -1;

    for (size_t i = 0; i < unionCompDim; ++i) {
        fabs(unionCompBegin->getCoord(i) - comp2Begin->getCoord(i)) < tolerance ? identicalCoordsCounter++ : axisWithDifference = i;
    }
    if (identicalCoordsCounter < unionCompDim - 1) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_INVALID_PARAMS);
        delete comp2Begin;
        delete comp2End;
        return nullptr;
    }

    bool isUnionPossible = true;

    for (size_t i = 0; i < unionCompDim; ++i) {
        fabs(unionCompEnd->getCoord(i) - comp2End->getCoord(i)) < tolerance ? identicalCoordsCounter-- : isUnionPossible = (i == axisWithDifference
                                                                                                                            ? true
                                                                                                                            : false);
    }
    if (identicalCoordsCounter != 0 || !isUnionPossible) {
        COMPLOG(logger, "Compacts are not suitable for union.", ReturnCode::RC_INVALID_PARAMS);
        delete comp2Begin;
        delete comp2End;
        return nullptr;
    }

    for (size_t i = 0; i < unionCompDim; ++i) {
        unionCompBegin->setCoord(i, std::min(unionCompBegin->getCoord(i), comp2Begin->getCoord(i)));
        unionCompEnd->setCoord(i, std::max(unionCompEnd->getCoord(i), comp2End->getCoord(i)));
    }

    delete comp2Begin;
    delete comp2End;

    if (!passDegeneracyCheck(unionCompBegin, unionCompEnd, tolerance)) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_INVALID_PARAMS);
        return nullptr;
    }

    unionComp = new(std::nothrow)CompactImpl(unionCompDim, unionCompBegin, unionCompEnd);
    if (unionComp == nullptr) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
    }

    return unionComp;
} //OK

ICompact *ICompact::convex(const ICompact *comp1, const ICompact *comp2, double tolerance, ILogger *logger) {
    ReturnCode rc;
    const ICompact *inclusive = nullptr;
    ICompact *convexComp;

    CompactRelation cr = exploreCompacts(comp1, comp2, tolerance, inclusive, rc);
    if (rc != ReturnCode::RC_SUCCESS) {
        COMPLOG(logger, MSG_DEFAULT, rc);
        return nullptr;
    }
    switch (cr) {
        case CR_DISPARATE:
            return nullptr;
        case CR_HAVE_NO_COMMON_POINTS:
            break;
        case CR_INTERSECTION:
            break;
        case CR_INCLUSION:
            convexComp = inclusive == comp1 ? comp1->clone() : comp2->clone();
            if (convexComp == nullptr) {
                COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
            }
            return convexComp;
        default:
            return nullptr;
    }

    size_t convexCompDim = comp1->getDim();
    IVector *convexCompBegin = comp1->getBegin();
    if (convexCompBegin == nullptr) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    IVector *convexCompEnd = comp1->getEnd();
    if (convexCompEnd == nullptr) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete convexCompBegin;
        return nullptr;
    }
    IVector *comp2Begin = comp2->getBegin();
    if (comp2Begin == nullptr) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete convexCompBegin;
        delete convexCompEnd;
        return nullptr;
    }
    IVector *comp2End = comp2->getEnd();
    if (comp2End == nullptr) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete convexCompBegin;
        delete convexCompEnd;
        delete comp2Begin;
        return nullptr;
    }

    for (size_t i = 0; i < convexCompDim; ++i) {
        convexCompBegin->setCoord(i, std::min(convexCompBegin->getCoord(i), comp2Begin->getCoord(i)));
        convexCompEnd->setCoord(i, std::max(convexCompEnd->getCoord(i), comp2End->getCoord(i)));
    }

    delete comp2Begin;
    delete comp2End;

    if (!passDegeneracyCheck(convexCompBegin, convexCompEnd, tolerance)) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_INVALID_PARAMS);
        return nullptr;
    }

    convexComp = new(std::nothrow)CompactImpl(convexCompDim, convexCompBegin, convexCompEnd);
    if (convexComp == nullptr) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
    }

    return convexComp;
} //OK

ICompact *ICompact::intersection(const ICompact *comp1, const ICompact *comp2, double tolerance, ILogger *logger) {
    ReturnCode rc;
    const ICompact *inclusive = nullptr;
    ICompact *intsctComp;

    CompactRelation cr = exploreCompacts(comp1, comp2, tolerance, inclusive, rc);
    if (rc != ReturnCode::RC_SUCCESS) {
        COMPLOG(logger, MSG_DEFAULT, rc);
        return nullptr;
    }
    switch (cr) {
        case CR_DISPARATE:
            return nullptr;
        case CR_HAVE_NO_COMMON_POINTS:
            return nullptr;
        case CR_INTERSECTION:
            break;
        case CR_INCLUSION:
            intsctComp = inclusive == comp1 ? comp2->clone() : comp1->clone();
            if (intsctComp == nullptr) {
                COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
            }
            return intsctComp;
        default:
            return nullptr;
    }

    size_t intsctCompDim = comp1->getDim();
    IVector *intsctCompBegin = comp1->getBegin();
    if (intsctCompBegin == nullptr) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    IVector *intsctCompEnd = comp1->getEnd();
    if (intsctCompEnd == nullptr) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete intsctCompBegin;
        return nullptr;
    }
    IVector *comp2Begin = comp2->getBegin();
    if (comp2Begin == nullptr) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete intsctCompBegin;
        delete intsctCompEnd;
        return nullptr;
    }
    IVector *comp2End = comp2->getEnd();
    if (comp2End == nullptr) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete intsctCompBegin;
        delete intsctCompEnd;
        delete comp2Begin;
        return nullptr;
    }

    for (size_t i = 0; i < intsctCompDim; ++i) {
        intsctCompBegin->setCoord(i, std::max(intsctCompBegin->getCoord(i), comp2Begin->getCoord(i)));
        intsctCompEnd->setCoord(i, std::min(intsctCompEnd->getCoord(i), comp2End->getCoord(i)));
    }

    delete comp2Begin;
    delete comp2End;

    if (!passDegeneracyCheck(intsctCompBegin, intsctCompEnd, tolerance)) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_INVALID_PARAMS);
        return nullptr;
    }

    intsctComp = new(std::nothrow)CompactImpl(intsctCompDim, intsctCompBegin, intsctCompEnd);
    if (intsctComp == nullptr) {
        COMPLOG(logger, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
    }

    return intsctComp;
} //OK


/* UNION
     if (comp1 == nullptr || comp2 == nullptr) {
        //LOG
        return nullptr;
    }
    if (comp1->getDim() != comp2->getDim()) {
        //LOG
        return nullptr;
    }

    ICompact *unionComp;

    bool isSubset;

    comp1->isSubset(comp2, isSubset);
    if (isSubset) {
        unionComp = comp2->clone();
        if (unionComp == nullptr) {
            //LOG
        }
        return unionComp;
    }

    comp2->isSubset(comp1, isSubset);
    if (isSubset) {
        unionComp = comp1->clone();
        if (unionComp == nullptr) {
            //LOG
        }
        return unionComp;
    }

    if (std::isnan(tolerance)) {
        //LOG
        return nullptr;
    }
    if (tolerance < .0) {
        //LOG
        return nullptr;
    }
 */

/* CONVEX
 if (comp1 == nullptr || comp2 == nullptr) {
        //LOG
        return nullptr;
    }
    if (comp1->getDim() != comp2->getDim()) {
        //LOG
        return nullptr;
    }

    ICompact *convexComp;

    bool isSubset;

    comp1->isSubset(comp2, isSubset);
    if (isSubset) {
        convexComp = comp2->clone();
        if (convexComp == nullptr) {
            //LOG
        }
        return convexComp;
    }

    comp2->isSubset(comp1, isSubset);
    if (isSubset) {
        convexComp = comp1->clone();
        if (convexComp == nullptr) {
            //LOG
        }
        return convexComp;
    }

    if (std::isnan(tolerance)) {
        //LOG
        return nullptr;
    }
    if (tolerance < .0) {
        //LOG
        return nullptr;
    }
 */

/* INTERSECTION
    if (comp1 == nullptr || comp2 == nullptr) {
        //LOG
        return nullptr;
    }
    if (comp1->getDim() != comp2->getDim()) {
        //LOG
        return nullptr;
    }

    ICompact *intsctComp;

    bool isIntersect;
    ReturnCode rc;

    rc = comp1->intersects(comp2, isIntersect);
    if (!isIntersect) {
        if (rc != ReturnCode::RC_SUCCESS) {
            //LOG
        }
        return nullptr;
    }

    bool isSubset;

    comp1->isSubset(comp2, isSubset);
    if (isSubset) {
        intsctComp = comp1->clone();
        if (intsctComp == nullptr) {
            //LOG
        }
        return intsctComp;
    }

    comp2->isSubset(comp1, isSubset);
    if (isSubset) {
        intsctComp = comp2->clone();
        if (intsctComp == nullptr) {
            //LOG
        }
        return intsctComp;
    }

    if (std::isnan(tolerance)) {
        //LOG
        return nullptr;
    }
    if (tolerance < .0) {
        //LOG
        return nullptr;
    }
*/