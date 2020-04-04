#ifndef LOGSTRATEGYSIMPLE_H
#define LOGSTRATEGYSIMPLE_H

#include "logger.h"

class LogStrategySimple: public LogStrategyBase
{
public:
    LogStrategySimple() = default;
    virtual ~LogStrategySimple() = default;
    void saveData(QString filePath, QVector<float>& dataVector) override;
};

#endif // LOGSTRATEGYSIMPLE_H
