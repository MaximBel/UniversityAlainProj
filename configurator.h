#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <string>
#include "configurationunit.h"
#include "list"
#include "memory"

class Configurator
{
public:
    Configurator(std::string configFilePath);
    virtual ~Configurator() = default;

    template<class T>
    void registerDataUnit(std::string key, T& dataVar);
    void readConfig();

private:
    std::string configFIle;
    std::list<std::unique_ptr<ConfigurationUnitBase>> unitList;
};

#endif // CONFIGURATOR_H
