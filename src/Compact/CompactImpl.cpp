#include "ICompact.h"
#include <algorithm>

#define MSG_DEFAULT __FUNCTION__
#define COMPLOG(logger, msg, rc)\
if (logger != nullptr) {\
    logger->log(msg, rc);\
}

namespace {
    class CompactImpl : public ICompact {
        public:
            class IteratorImpl : public ICompact::Iterator {
                public:
                    IVector *getPoint() const override;
                    ReturnCode setDirection(std::vector<size_t> const &direction) override;
                    ReturnCode doStep() override;

                    IteratorImpl(std::vector<size_t> const &dir, IVector *const step, IVector *const begin, IVector *const end, IVector *current);
                    ~IteratorImpl();

                private:
                    std::vector<size_t> direction_;
                    IVector *current_;
                    IVector *const begin_;
                    IVector *const end_;
                    IVector *const step_;
                    ILogger *logger_;
            };

            Iterator *begin(IVector const *step) override;
            Iterator *end(IVector const *step) override;

            ICompact *clone() const override;
            IVector *getBegin() const override;
            IVector *getEnd() const override;
            ReturnCode contains(IVector const *vec, bool &result) const override;
            ReturnCode isSubset(ICompact const *comp, bool &result) const override;
            ReturnCode intersects(ICompact const *comp, bool &result) const override;
            size_t getDim() const override;

            CompactImpl(size_t dim, IVector *begin, IVector *end);
            ~CompactImpl();

        private:
            size_t dim_;
            IVector *begin_;
            IVector *end_;
            ILogger *logger_;
    };
}

CompactImpl::CompactImpl(size_t dim, IVector *begin, IVector *end) : dim_{dim}, begin_{begin}, end_{end} {
    this->logger_ = ILogger::createLogger(this);
} //OK

CompactImpl::~CompactImpl() {
    this->dim_ = 0;
    delete this->begin_;
    this->begin_ = nullptr;
    delete this->end_;
    this->end_ = nullptr;

    if (this->logger_ != nullptr)
        this->logger_->releaseLogger(this);
} //OK

ICompact *CompactImpl::clone() const {
    IVector *clonedBegin = this->begin_->clone();
    if (clonedBegin == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    IVector *clonedEnd = this->end_->clone();
    if (clonedEnd == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete clonedBegin;
        return nullptr;
    }

    ICompact *compact = new(std::nothrow)CompactImpl(clonedBegin->getDim(), clonedBegin, clonedEnd);
    if (compact == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
    }
    return compact;
} //OK

ICompact::Iterator *CompactImpl::begin(const IVector *step) {
    if (step == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NULL_PTR);
        return nullptr;
    }
    if (step->getDim() != this->dim_) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_WRONG_DIM);
        return nullptr;
    }
    for (size_t i = 0; i < step->getDim(); ++i) {
        if (step->getCoord(i) < .0) {
            COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_INVALID_PARAMS);
            return nullptr;
        }
    }

    IVector *clonedStep = step->clone();
    if (clonedStep == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    IVector *begin = this->begin_->clone();
    if (begin == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete clonedStep;
        return nullptr;
    }
    IVector *end = this->end_->clone();
    if (end == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete clonedStep;
        delete begin;
        return nullptr;
    }
    IVector *current = this->begin_->clone();
    if (current == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete clonedStep;
        delete begin;
        delete end;
        return nullptr;
    }

    std::vector<size_t> dir;
    for (size_t i = 0; i < this->dim_; ++i)
        dir.push_back(i);

    ICompact::Iterator *iterator = new(std::nothrow) CompactImpl::IteratorImpl(dir, clonedStep, begin, end, current);
    if (iterator == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete clonedStep;
        delete begin;
        delete end;
        delete current;
    }
    return iterator;
} //OK

ICompact::Iterator *CompactImpl::end(const IVector *step) {
    if (step == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NULL_PTR);
        return nullptr;
    }
    if (step->getDim() != this->dim_) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_WRONG_DIM);
        return nullptr;
    }
    for (size_t i = 0; i < step->getDim(); ++i) {
        if (step->getCoord(i) < .0) {
            COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_INVALID_PARAMS);
            return nullptr;
        }
    }

    IVector *clonedStep = step->clone();
    if (clonedStep == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    for (size_t i = 0; i < clonedStep->getDim(); ++i) {
        clonedStep->setCoord(i, -step->getCoord(i));
    }

    IVector *begin = this->end_->clone();
    if (begin == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete clonedStep;
        return nullptr;
    }
    IVector *end = this->begin_->clone();
    if (end == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete clonedStep;
        delete begin;
        return nullptr;
    }
    IVector *current = this->end_->clone();
    if (current == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete clonedStep;
        delete begin;
        delete end;
        return nullptr;
    }

    std::vector<size_t> dir;
    for (size_t i = 0; i < this->dim_; ++i)
        dir.push_back(i);

    ICompact::Iterator *iterator = new(std::nothrow) CompactImpl::IteratorImpl(dir, clonedStep, begin, end, current);
    if (iterator == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete clonedStep;
        delete begin;
        delete end;
        delete current;
    }
    return iterator;
} //OK

IVector *CompactImpl::getBegin() const {
    return this->begin_->clone();
} //OK

IVector *CompactImpl::getEnd() const {
    return this->end_->clone();
} //OK

size_t CompactImpl::getDim() const {
    return this->dim_;
} //OK

ReturnCode CompactImpl::contains(const IVector *vec, bool &result) const {
    result = false;
    if (vec == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NULL_PTR);
        return ReturnCode::RC_NULL_PTR;
    }
    if (vec->getDim() != this->dim_) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_WRONG_DIM);
        return ReturnCode::RC_WRONG_DIM;
    }

    result = true;
    for (size_t i = 0; result && i < this->dim_; ++i)
        result &= (this->begin_->getCoord(i) <= vec->getCoord(i) && vec->getCoord(i) <= this->end_->getCoord(i));

    return ReturnCode::RC_SUCCESS;
} //OK

ReturnCode CompactImpl::isSubset(const ICompact *comp, bool &result) const {
    result = false;
    if (comp == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NULL_PTR);
        return ReturnCode::RC_NULL_PTR;
    }
    if (comp->getDim() != this->dim_) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_WRONG_DIM);
        return ReturnCode::RC_WRONG_DIM;
    }

    bool isInside;
    ReturnCode rc;

    rc = comp->contains(this->begin_, isInside);
    if (rc != ReturnCode::RC_SUCCESS || !isInside)
        return rc;
    rc = comp->contains(this->end_, isInside);
    if (rc != ReturnCode::RC_SUCCESS || !isInside)
        return rc;

    result = true;
    return ReturnCode::RC_SUCCESS;
} //OK

ReturnCode CompactImpl::intersects(const ICompact *comp, bool &result) const {
    result = false;
    if (comp == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NULL_PTR);
        return ReturnCode::RC_NULL_PTR;
    }
    if (comp->getDim() != this->dim_) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_WRONG_DIM);
        return ReturnCode::RC_WRONG_DIM;
    }

    IVector *compBegin = comp->getBegin();
    if (compBegin == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        return ReturnCode::RC_NULL_PTR;
    }
    IVector *compEnd = comp->getEnd();
    if (compEnd == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete compBegin;
        return ReturnCode::RC_NULL_PTR;
    }

    result = true;
    for (size_t i = 0; i < this->dim_; ++i) {
        result &= (std::max(this->begin_->getCoord(i), compBegin->getCoord(i)) <= std::min(this->end_->getCoord(i), compEnd->getCoord(i)));
    }

    delete compBegin;
    delete compEnd;

    return ReturnCode::RC_SUCCESS;
} //OK


CompactImpl::IteratorImpl::IteratorImpl(std::vector<size_t> const &dir, IVector *const step, IVector *const begin, IVector *const end,
                                        IVector *current) : direction_{dir}, step_{step}, begin_{begin}, end_{end}, current_{current} {
    this->logger_ = ILogger::createLogger(this);
} //OK

CompactImpl::IteratorImpl::~IteratorImpl() {
    delete this->current_;
    delete this->step_;
    delete this->begin_;
    delete this->end_;
    if (this->logger_ != nullptr)
        this->logger_->releaseLogger(this);
} //OK

IVector *CompactImpl::IteratorImpl::getPoint() const {
    return this->current_->clone();
} //OK

ReturnCode CompactImpl::IteratorImpl::setDirection(const std::vector<size_t> &direction) {
    if (this->end_->getDim() != direction.size()) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_WRONG_DIM);
        return ReturnCode::RC_WRONG_DIM;
    }

    bool *indicators = new(std::nothrow) bool[this->end_->getDim()]{false};
    if (indicators == nullptr) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        return ReturnCode::RC_NO_MEM;
    }
    for (size_t i = 0; i < this->end_->getDim(); ++i) {
        if (direction[i] < direction.size())
            indicators[direction[i]] |= true;
    }

    for (size_t i = 1; i < this->end_->getDim(); ++i) {
        indicators[0] &= indicators[i];
    }

    if (!indicators[0]) {
        COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_INVALID_PARAMS);
        delete indicators;
        return ReturnCode::RC_INVALID_PARAMS;
    }

    delete indicators;
    this->direction_ = direction;
    return ReturnCode::RC_SUCCESS;
} //OK

ReturnCode CompactImpl::IteratorImpl::doStep() {
    bool inside = false;
    for (size_t i = 0; i < this->direction_.size(); ++i) {
        size_t axis = this->direction_.at(i);
        double coord = this->current_->getCoord(axis) + this->step_->getCoord(axis);
        inside = (this->step_->getCoord(axis) > 0 ? coord <= this->end_->getCoord(axis) : coord >= this->end_->getCoord(axis));
        if (inside) {
            this->current_->setCoord(axis, coord);
            break;
        } else {
            this->current_->setCoord(axis, this->begin_->getCoord(axis));
        }
    }

    if (inside) {
        return ReturnCode::RC_SUCCESS;
    }

    for (size_t i = 0; i < this->end_->getDim(); ++i) {
        this->current_->setCoord(i, this->end_->getCoord(i));
    }

    COMPLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_OUT_OF_BOUNDS);
    return ReturnCode::RC_OUT_OF_BOUNDS;
} //OK

