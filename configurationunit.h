#ifndef CONFIGURATIONUNITBASE_H
#define CONFIGURATIONUNITBASE_H

#include <string>
#include <memory>
#include "QString"
#include "QStringList"
#include "QDebug"

class ConfigurationUnitBase
{
public:
    ConfigurationUnitBase() = default;
    virtual ~ConfigurationUnitBase() = default;
    virtual bool readUnit(QString configFileLine) = 0;
};

template <typename T>
class ConfigurationUnit: public ConfigurationUnitBase {
private:
    QString key;
    T& paramRef;
public:
    ConfigurationUnit(QString key, T& paramRef): key(key), paramRef(paramRef) {}

    std::string getKey() {
        return key;
    }

    bool readUnit(QString configFileLine) override{
        if(configFileLine.contains(key)) {
            QStringList list = configFileLine.split("=");

            if(list.size() == 2) {
                paramRef = static_cast<T>(list[1].toFloat());
            } else {
               qDebug() << "Could not read config for var:" + list[0];
            }
            return true;
        } else {
            return false;
        }
    }
};

#endif // CONFIGURATIONUNITBASE_H
