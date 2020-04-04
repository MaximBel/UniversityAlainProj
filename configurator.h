#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <string>
#include "configurationunit.h"
#include "list"
#include "memory"

class Configurator
{
public:
    Configurator(QString configFilePath);
    virtual ~Configurator() = default;

    void readConfig();

private:
    QString configFIle;
    std::list<std::unique_ptr<ConfigurationUnitBase>> unitList;

public:
    template<typename T>
    void registerDataUnit(QString key, T& dataVar) {
        unitList.emplace_back(std::unique_ptr<ConfigurationUnitBase>(new ConfigurationUnit<T>(key, dataVar)));
    }
};

#endif // CONFIGURATOR_H
