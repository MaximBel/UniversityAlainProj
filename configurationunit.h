#ifndef CONFIGURATIONUNITBASE_H
#define CONFIGURATIONUNITBASE_H

#include <string>
#include <memory>
#include "QString"

class ConfigurationUnitBase
{
public:
    ConfigurationUnitBase() = default;
    virtual ~ConfigurationUnitBase() = default;
    virtual bool readUnit(QString configFileLine) = 0;
};

template <typename T>
class ConfigurationUnit: public ConfigurationUnitBase {
public:
    ConfigurationUnit(std::string key, std::unique_ptr<T> paramPtr);
    std::string getKey();
    bool readUnit(QString configFileLine) override;
private:
    std::string key;
    std::unique_ptr<T> paramPtr;
};

#endif // CONFIGURATIONUNITBASE_H
