#include "include/ILogger.h"
#include <vector>
#include <cstdio>

namespace {
    class LoggerImpl : public ILogger {
        public:
            static ILogger *addClient(void *client);

            void releaseLogger(void *client) override;
            void log(char const *message, ReturnCode rc) override;
            ReturnCode setLogFile(char const *logFileName) override;

            LoggerImpl();
            ~LoggerImpl();

        private:
            static const char *msgMask_;
            static size_t msgCounter_;

            static LoggerImpl *instance_;

            std::vector<void *> clients_;
            FILE *logFile_;
    };
    LoggerImpl *LoggerImpl::instance_ = nullptr;
    const char *LoggerImpl::msgMask_ = "%d.--Function:[%s]--ReturnCode:[%d]--Message:[%s]";
    size_t LoggerImpl::msgCounter_ = 0;
}

static char const* msgDefaults[(size_t)ReturnCode::RC_UNKNOWN + 1] = {
        "It's OK",
        "Can not allocate memory",
        "Null pointer passed as argument",
        "Degenerate mathematical object",
        "Mismatch of dimensions of mathematical objects",
        "NAN value passed as argument",
        "Index exceeds the number of elements in container",
        "Can not open file",
        "This element not found",
        "Invalid arguments passed",
        "Object requires initialization",
        "Uknown return"
};


void LoggerImpl::releaseLogger(void *client) {
    if (client == nullptr) {
        //METALOG
        return;
    }

    std::vector<void *>::iterator it = this->clients_.begin();
    while (*it != client && ++it != this->clients_.end());

    this->clients_.erase(it); //this or instance_?

    if (this->clients_.empty()) {
        delete LoggerImpl::instance_;
        LoggerImpl::instance_ = nullptr;
    }
} //OK, but LOG?

void LoggerImpl::log(const char *message, ReturnCode rc) {
    LoggerImpl::msgCounter_++;

    if (message == NULL)
        fprintf(this->logFile_, LoggerImpl::msgMask_, LoggerImpl::msgCounter_, __FUNCTION__, rc, msgDefaults[(size_t)rc]);
    else
        fprintf(this->logFile_, LoggerImpl::msgMask_, LoggerImpl::msgCounter_, message, rc, msgDefaults[(size_t)rc]);
    fprintf(this->logFile_, "\n");
} //OK, but LOG

ReturnCode LoggerImpl::setLogFile(const char *logFileName) {
    if (logFileName == nullptr) {
        //METALOG
        return ReturnCode::RC_NULL_PTR;
    }

    if (this->logFile_ != nullptr && this->logFile_ != stdout) {
        fflush(this->logFile_);
        fclose(this->logFile_);
        this->logFile_ = nullptr;
    }

    this->logFile_ = fopen(logFileName, "w");
    if (this->logFile_ == nullptr) {
        //METALOG
        this->logFile_ = stdout;
        return ReturnCode::RC_OPEN_FILE;
    }
    return ReturnCode::RC_SUCCESS;
} //OK, but LOG?

ILogger *LoggerImpl::addClient(void *client) {
    if (client == nullptr) {
        //METALOG
        return nullptr;
    }

    if (LoggerImpl::instance_ == nullptr) {
        LoggerImpl::instance_ = new(std::nothrow) LoggerImpl();
        if (LoggerImpl::instance_ == nullptr) {
            //METALOG
            return nullptr;
        }
        LoggerImpl::instance_->logFile_ = stdout;
    }

    LoggerImpl::instance_->clients_.push_back(client);
    return LoggerImpl::instance_;
} //OK, but LOG?

LoggerImpl::LoggerImpl() : logFile_{nullptr} {

} //OK

LoggerImpl::~LoggerImpl() {
    if (this->logFile_ == nullptr)
        return;

    fflush(this->logFile_);
    if (this->logFile_ != stdout)
        fclose(this->logFile_);
    this->logFile_ = nullptr;
} //OK
