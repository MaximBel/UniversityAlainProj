#include "loggercommandadapter.h"

LoggerCommandAdapter::LoggerCommandAdapter(std::shared_ptr<Logger> logger): loggerPtr(logger) {}

float LoggerCommandAdapter::runCommand(float data) {
    loggerPtr->putNewData(data);
    return data;
}
