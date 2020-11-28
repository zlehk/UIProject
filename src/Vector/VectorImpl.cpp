#include "IVector.h"
#include <cmath>
#include <new>

#define MSG_DEFAULT __FUNCTION__
#define VECLOG(logger, msg, rc)\
if (logger != nullptr) {\
    logger->log(msg, rc);\
}

namespace {
    class VectorImpl : public IVector {
        public:
            IVector *clone() const override;
            ReturnCode setCoord(size_t index, double value) const override;
            double getCoord(size_t index) const override;
            double norm(Norm norm) const override;
            size_t getDim() const override;

            VectorImpl(size_t dim, double *data);
            ~VectorImpl();

        private:
            size_t dim_;
            double *data_;
            ILogger *logger_;
    };
}

VectorImpl::VectorImpl(size_t dim, double *data) : dim_{dim}, data_{data} {
    this->logger_ = ILogger::createLogger(this);
} //OK

VectorImpl::~VectorImpl() {
    delete[]this->data_;
    this->data_ = nullptr;
    if (this->logger_ != nullptr)
        this->logger_->releaseLogger(this);
} //OK

IVector *VectorImpl::clone() const {
    double *clonedCoords = new(std::nothrow)double[this->dim_];
    if (clonedCoords == nullptr) {
        VECLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    for (size_t i = 0; i < this->dim_; i++)
        clonedCoords[i] = this->data_[i];

    VectorImpl *cloned = new(std::nothrow)VectorImpl(this->dim_, clonedCoords);
    if (cloned == nullptr) {
        VECLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NO_MEM);
        delete[]clonedCoords;
    }
    return cloned;
} //OK

ReturnCode VectorImpl::setCoord(size_t index, double value) const {
    if (index >= this->dim_) {
        VECLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_OUT_OF_BOUNDS);
        return ReturnCode::RC_OUT_OF_BOUNDS;
    }
    if (std::isnan(value)) {
        VECLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_NAN);
        return ReturnCode::RC_NAN;
    }
    this->data_[index] = value;
    return ReturnCode::RC_SUCCESS;
} //OK

double VectorImpl::getCoord(size_t index) const {
    if (index >= this->dim_) {
        VECLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_OUT_OF_BOUNDS);
        return NAN;
        //return std::nan("1");
    }
    return this->data_[index];
} //OK


double VectorImpl::norm(IVector::Norm norm) const {
    double vec_norm = 0;
    switch (norm) {
        case IVector::Norm::NORM_1:
            for (int i = 0; i < this->dim_; ++i) {
                vec_norm += fabs(this->data_[i]);
            }
            break;
        case IVector::Norm::NORM_2:
            for (int i = 0; i < this->dim_; ++i) {
                vec_norm += this->data_[i] * this->data_[i];
            }
            vec_norm = sqrt(vec_norm);
            break;
        case IVector::Norm::NORM_INF:
            vec_norm = fabs(this->data_[0]);
            for (int i = 1; i < this->dim_; ++i) {
                if (fabs(this->data_[i]) > vec_norm)
                    vec_norm = fabs(this->data_[i]);
            }
            break;
        default:
            VECLOG(this->logger_, MSG_DEFAULT, ReturnCode::RC_INVALID_PARAMS);
            vec_norm = std::nan("1");
            break;
    }
    return vec_norm;
} //OK

size_t VectorImpl::getDim() const {
    return this->dim_;
} //OK


