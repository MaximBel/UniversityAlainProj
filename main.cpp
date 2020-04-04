#include "mainwindow.h"
#include <QApplication>
#include <serial.h>
#include "stdint.h"
#include "plotter.h"
#include "configurator.h"
#include "serialcommandadapter.h"

std::shared_ptr<Serial> serial;
std::unique_ptr<Plotter> inputDataProtter;
std::unique_ptr<Plotter> outputDataProtter;

QVector<float> DataIn_Buffer;
QVector<float> DataOut_Buffer;

//const QString DataInFile = "SignalIn.txt";
//const QString DataOutFile = "SignalOut.txt";
static void setupAll(MainWindow& w);
static void WriteDataToFile(QString FileName, QVector<float> *DataVector);
static bool callbackAccessToPort(bool state, QString port, int baud);
static void callbackPlot(bool state);
static void callbackPausePlot(bool state);
static void callbackSaveData(bool state);

int main(int argc, char *argv[])
{
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("imageformats");
    paths.append("platforms");
    paths.append("sqldrivers");
    QCoreApplication::setLibraryPaths(paths);
    QApplication a(argc, argv);
    MainWindow w;

    setupAll(w);

    w.show();

    return a.exec();
}

static void setupAll(MainWindow& w) {
    serial = std::make_shared<Serial>();
    //qApp->applicationDirPath() + "/config.txt"

    w.setPlotCallback(callbackPlot);
    w.setSaveDataCallback(callbackSaveData);
    w.setPausePlotCallback(callbackPausePlot);
    w.setAccessToPortCallback(callbackAccessToPort);

    w.setSerialPortList(serial->getPortList());

    inputDataProtter = std::unique_ptr<PlotterBuilder>(new PlotterBuilder(w.getCustomPlot(PlotType_Input), 30))
            ->setVerticalRange(0, 260)
            .setHorizontalResolution(8, 0.02)
            .setInputDataCommand(std::make_shared<SerialCommandAdapter>(serial, dataInput))
            .createPlotter();
    outputDataProtter = std::unique_ptr<PlotterBuilder>(new PlotterBuilder(w.getCustomPlot(PlotType_Output), 30))
            ->setVerticalRange(0, 260)
            .setHorizontalResolution(8, 0.02)
            .setInputDataCommand(std::make_shared<SerialCommandAdapter>(serial, dataOutput))
            .createPlotter();

    DataIn_Buffer.resize(0);
    DataOut_Buffer.resize(0);
}

static void WriteDataToFile(QString FileName, QVector<float> *DataVector) {
//    QFile OutputFile(qApp->applicationDirPath() + "/"+ FileName);

//    if(OutputFile.open(QIODevice::WriteOnly | QIODevice::Truncate) == true) {

//        QTextStream OutputStream(&OutputFile);

//        for(int counter = 0; counter < DataVector->size(); counter++) {

//            OutputStream << DataVector->data()[counter];

//            OutputStream << ' ';

//        }

//        OutputFile.close();

//    }
}
static bool callbackAccessToPort(bool state, QString port, int baud) {
    if(state) {
        serial->open(port,baud);
        if(serial->isOpened()) {
            return true;
        } else {
            return false;
        }
    } else {
        if(serial->isOpened()) {
            serial->close();
        }
        return true;
    }
}
static void callbackPlot(bool state) {
    if(state) {
        serial->setControl(true); // запускаем работу ардуины

        inputDataProtter->initialize();
        outputDataProtter->initialize();
        inputDataProtter->start();
        outputDataProtter->start();
    } else {
        serial->setControl(false); // останавливаем работу ардуины

        inputDataProtter->stop();
        outputDataProtter->stop();
        inputDataProtter->clear();
        outputDataProtter->clear();
    }
}
static void callbackPausePlot(bool state) {
    if(state) {
        inputDataProtter->stop();
        outputDataProtter->stop();
    } else {
        //DataIn_Buffer.resize(0);
        //DataOut_Buffer.resize(0);

        inputDataProtter->start();
        outputDataProtter->start();
    }
}
static void callbackSaveData(bool state) {
    (void)state;
    //    if(DataIn_Buffer.size() != 0 && DataOut_Buffer.size() != 0) {
    //        WriteDataToFile(DataInFile, &DataIn_Buffer);
    //        WriteDataToFile(DataOutFile, &DataOut_Buffer);
    //        DataIn_Buffer.resize(0);
    //        DataOut_Buffer.resize(0);
    //    }
}
