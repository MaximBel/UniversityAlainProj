#ifndef LOGGER_H
#define LOGGER_H

#include "QVector"
#include "QString"
#include "memory"

class LogStrategyBase {
public:
    LogStrategyBase() = default;
    virtual ~LogStrategyBase() = default;
    virtual void saveData(QString filePath, QVector<float>& dataVector) = 0;
};

class Logger
{
public:
    Logger(QString filePath);

    void putNewData(float dataUnit);
    void registerNewStrategy(std::unique_ptr<LogStrategyBase> strategy);

    void clearData();
    void saveDataToFile();
private:
    QString outputFilePath;
    QVector<float> dataVector;
    std::list<std::unique_ptr<LogStrategyBase>> strategyList;
};

#endif // LOGGER_H
