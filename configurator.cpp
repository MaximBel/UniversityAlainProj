#include "configurator.h"
#include "QFile"
#include "QDebug"

Configurator::Configurator(QString configFilePath): configFIle(configFilePath) {}

void Configurator::readConfig() {
    qDebug() << "Configurator. readConfgi()";

    QFile file(configFIle);
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
