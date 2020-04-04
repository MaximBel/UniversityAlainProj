#include "configurator.h"
#include "QFile"
#include "QDebug"

Configurator::Configurator(std::string configFilePath): configFIle(configFilePath) {}

template<class T>
void Configurator::registerDataUnit(std::string key, T& dataVar) {
    unitList.emplace_back(std::unique_ptr<ConfigurationUnitBase>(new ConfigurationUnit<T>(key, std::unique_ptr<T>(&dataVar))));
}

void Configurator::readConfig() {
    qDebug() << "Configurator. readConfgi()";

    QFile file(QString::fromStdString(configFIle));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cant open config file";
        return;
    }

    while (!file.atEnd()) {
        QString line = file.readLine();
        qDebug() << "parse line:" + line;

        for (auto it=unitList.begin(); it != unitList.end(); ++it) {
            if(it->get()->readUnit(line)) {
                break;
            }
        }
    }
}
