#include "logstrategysimple.h"
#include "QFile"
#include "QTextStream"

void LogStrategySimple::saveData(QString filePath, QVector<float>& dataVector) {
    QFile OutputFile(filePath);

    if(OutputFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream OutputStream(&OutputFile);
        for(int counter = 0; counter < dataVector.size(); counter++) {
            OutputStream << dataVector.data()[counter];
            OutputStream << ' ';
        }
        OutputFile.close();
    }
}
