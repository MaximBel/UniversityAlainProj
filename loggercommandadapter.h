#ifndef LOGGERCOMMANDADAPTER_H
#define LOGGERCOMMANDADAPTER_H

#include "datacommand.h"
#include "memory"
#include "logger.h"

class LoggerCommandAdapter : public DataCommand<float>
{
public:
    LoggerCommandAdapter(std::shared_ptr<Logger> logger);
    ~LoggerCommandAdapter() = default;
    float runCommand(float data) override;
private:
    std::shared_ptr<Logger> loggerPtr;
};

#endif // LOGGERCOMMANDADAPTER_H
