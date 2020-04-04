#include "configurationunit.h"
#include "QString"
#include "QStringList"
#include "QDebug"

template <typename T>
ConfigurationUnit<T>::ConfigurationUnit(std::string key, std::unique_ptr<T> paramPtr) {
    this->key = key;
    this->paramPtr = paramPtr;
}

template <typename T>
std::string ConfigurationUnit<T>::getKey() {
    return key;
}

template <typename T>
bool ConfigurationUnit<T>::readUnit(QString configFileLine) {
    if(configFileLine.contains(QString::fromStdString(key))) {
        QStringList list = configFileLine.split("=");

        if(list.size() == 2) {
            *paramPtr = dynamic_cast<T>(list[1].toFloat());
        } else {
           qDebug() << "Could not read config for var:" + list[0];
        }

        return true;
    } else {
        return false;
    }
}
