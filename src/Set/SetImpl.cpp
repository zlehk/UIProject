#include <vector>
#include <cmath>
#include "ISet.h"

#define MSG_DEFAULT __FUNCTION__
#define SETLOG(logger, msg, rc)\
if (logger != nullptr) {\
    logger->log(msg, rc);\
}

namespace {
    class SetImpl : public ISet {
        public:
            ReturnCode insert(IVector const *vector, IVector::Norm norm, double tolerance) override;
            ReturnCode erase(IVector const *vector, IVector::Norm norm, double tolerance) override;
            ReturnCode erase(size_t ind) override;
            void clear() override;

            ReturnCode find(IVector const *vector, IVector::Norm norm, double tolerance, size_t &ind) const override;
            ReturnCode get(IVector *&dst, size_t ind) const override;
            size_t getDim() const override;
            size_t getSize() const override;
            ISet *clone() const override; //?

            SetImpl();
            ~SetImpl();

        private:
            size_t dim_;
            std::vector<IVector *> data_;
            ILogger *logger_; //needs for IVector::equals method + for ISet::clone()
    };
}

SetImpl::SetImpl() : dim_{0} {
    this->logger_ = ILogger::createLogger(this);
} //OK

SetImpl::~SetImpl() {
    this->clear();
    if (this->logger_ != nullptr)
        this->logger_->releaseLogger(this);
} //OK

ISet *SetImpl::clone() const {
    SetImpl *cloned = new(std::nothrow) SetImpl();
    if (cloned == nullptr) {
        SETLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    cloned->dim_ = this->dim_;
    for (std::vector<IVector *>::const_iterator it = this->data_.begin(); it < this->data_.end(); ++it) {
        cloned->data_.push_back((*it)->clone());
    }
    return cloned;
} //OK

ReturnCode SetImpl::insert(const IVector *vector, IVector::Norm norm, double tolerance) {
    if (vector == nullptr)
        return ReturnCode::RC_NULL_PTR;

    if (std::isnan(tolerance))
        return ReturnCode::RC_NAN;
    if (tolerance < 0)
        return ReturnCode::RC_INVALID_PARAMS;

    if (this->data_.empty()) {
        this->dim_ = vector->getDim();
        this->data_.push_back(vector->clone());
        return ReturnCode::RC_SUCCESS;
    }

    if (vector->getDim() != this->dim_)
        return ReturnCode::RC_WRONG_DIM;


    bool is_in = false;
    ReturnCode rc;
    std::vector<IVector *>::const_iterator it = this->data_.begin();
    do {
        rc = IVector::equals(*it, vector, norm, tolerance, is_in, this->logger_);
    } while (!is_in && ++it != this->data_.end());

    if (rc != ReturnCode::RC_SUCCESS)
        return rc;
    if (!is_in) {
        this->data_.push_back(vector->clone());
    }

    return ReturnCode::RC_SUCCESS;
} //OK

ReturnCode SetImpl::erase(const IVector *vector, IVector::Norm norm, double tolerance) {
    if (vector == nullptr)
        return ReturnCode::RC_NULL_PTR;

    if (this->data_.empty())
        return ReturnCode::RC_ELEM_NOT_FOUND;

    if (vector->getDim() != this->dim_)
        return ReturnCode::RC_WRONG_DIM;

    if (std::isnan(tolerance))
        return ReturnCode::RC_NAN;
    if (tolerance < .0)
        return ReturnCode::RC_INVALID_PARAMS;

    bool is_equal;
    bool is_in = false;
    ReturnCode rc = ReturnCode::RC_SUCCESS;
    for (std::vector<IVector *>::iterator it = this->data_.begin(); rc == ReturnCode::RC_SUCCESS && it < this->data_.end();) {
        rc = IVector::equals(*it, vector, norm, tolerance, is_equal, this->logger_);
        if (is_equal) {
            delete *it;
            this->data_.erase(it);
        } else {
            ++it;
        }
        is_in |= is_equal;
    }

    if (rc != ReturnCode::RC_SUCCESS)
        return rc;
    if (!is_in) {
        return ReturnCode::RC_ELEM_NOT_FOUND;
    }

    if (this->data_.empty()) {
        this->dim_ = 0;
        this->data_.clear();
    }

    return ReturnCode::RC_SUCCESS;
} //OK

ReturnCode SetImpl::erase(size_t ind) {
    if (ind >= this->data_.size())
        return ReturnCode::RC_OUT_OF_BOUNDS;

    std::vector<IVector *>::iterator it = this->data_.begin() + ind;
    delete *it;
    this->data_.erase(it);

    if (this->data_.empty()) {
        this->dim_ = 0;
        this->data_.clear();
    }

    return ReturnCode::RC_SUCCESS;
} //OK

void SetImpl::clear() {
    this->dim_ = 0;
    for (std::vector<IVector *>::iterator it = this->data_.begin(); it < this->data_.end(); ++it)
        delete *it;
    this->data_.clear();
} //OK

ReturnCode SetImpl::find(const IVector *vector, IVector::Norm norm, double tolerance, size_t &ind) const {
    if (vector == nullptr)
        return ReturnCode::RC_NULL_PTR;

    if (this->data_.empty())
        return ReturnCode::RC_ELEM_NOT_FOUND;

    if (vector->getDim() != this->dim_)
        return ReturnCode::RC_WRONG_DIM;

    if (std::isnan(tolerance))
        return ReturnCode::RC_NAN;
    if (tolerance < 0)
        return ReturnCode::RC_INVALID_PARAMS;

    bool is_in = false;
    ReturnCode rc;
    std::vector<IVector *>::const_iterator it = this->data_.begin();
    do {
        rc = IVector::equals(*it, vector, norm, tolerance, is_in, this->logger_);
    } while (!is_in && ++it != this->data_.end());

    if (rc != ReturnCode::RC_SUCCESS)
        return rc;
    if (!is_in)
        return ReturnCode::RC_ELEM_NOT_FOUND;

    ind = it - this->data_.begin();
    return ReturnCode::RC_SUCCESS;
} //OK

ReturnCode SetImpl::get(IVector *&dst, size_t ind) const {
    if (ind >= this->data_.size())
        return ReturnCode::RC_OUT_OF_BOUNDS;
    dst = (*(this->data_.begin() + ind))->clone();
    return ReturnCode::RC_SUCCESS;
} //OK

size_t SetImpl::getDim() const {
    return this->dim_;
} //OK

size_t SetImpl::getSize() const {
    return this->data_.size();
} //OK
