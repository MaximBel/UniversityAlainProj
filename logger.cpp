#include "logger.h"

Logger::Logger(QString filePath): outputFilePath(filePath) {}

void Logger::putNewData(float dataUnit) {
    dataVector.append(dataUnit);
}

void Logger::registerNewStrategy(std::unique_ptr<LogStrategyBase> strategy) {
    strategyList.emplace_back(std::move(strategy));
}

void Logger::clearData() {
    dataVector.resize(0);
}

void Logger::saveDataToFile() {
    for (auto it=strategyList.begin(); it != strategyList.end(); ++it) {
        it->get()->saveData(outputFilePath, dataVector);
    }
}
