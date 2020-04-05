#include "mainwindow.h"
#include <QApplication>
#include <serial.h>
#include "stdint.h"
#include "plotter.h"
#include "configurator.h"
#include "serialcommandadapter.h"
#include "logger.h"
#include "loggercommandadapter.h"
#include "logstrategysimple.h"

static QString configFileName = "/config.ini";

//TODO: move this to the config file
static const QString DataInFile = "/SignalIn.txt";
static const QString DataOutFile = "/SignalOut.txt";

// default params
static int32_t vRangeInputMax = 260;
static int32_t vRangeOutputMax = 260;
static int32_t vRangeInputMin = 0;
static int32_t vRangeOutputMin = 0;
static float hRangeInput = 8;
static float hRangeOutput = 8;
static int32_t servTimerPeriod = 30;
static float hStepInput = 2;
static float hStepOutput = 2;

static std::shared_ptr<Serial> serial;
static std::shared_ptr<Logger> loggerInput;
static std::shared_ptr<Logger> loggerOutput;
static std::unique_ptr<Plotter> inputDataProtter;
static std::unique_ptr<Plotter> outputDataProtter;

static void setupAll(MainWindow& w);
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
    loggerInput = std::make_shared<Logger>(qApp->applicationDirPath() + DataInFile);
    loggerOutput = std::make_shared<Logger>(qApp->applicationDirPath() + DataOutFile);

    loggerInput->registerNewStrategy(std::unique_ptr<LogStrategyBase>(new LogStrategySimple()));
    loggerOutput->registerNewStrategy(std::unique_ptr<LogStrategyBase>(new LogStrategySimple()));

    auto configurator = std::unique_ptr<Configurator>(new Configurator(qApp->applicationDirPath() + configFileName));

    configurator->registerDataUnit("VRANGE_MAX_INPUT", vRangeInputMax);
    configurator->registerDataUnit("VRANGE_MIN_INPUT", vRangeInputMin);
    configurator->registerDataUnit("VRANGE_MAX_OUTPUT", vRangeOutputMax);
    configurator->registerDataUnit("VRANGE_MIN_OUTPUT", vRangeOutputMin);
    configurator->registerDataUnit("HRANGE_INPUT", hRangeInput);
    configurator->registerDataUnit("HRANGE_OUTPUT", hRangeOutput);
    configurator->registerDataUnit("HSTEP_INPUT", hStepInput);
    configurator->registerDataUnit("HSTEP_OUTPUT", hStepOutput);
    configurator->registerDataUnit("SERV_TIMER_PERIOD", servTimerPeriod);

    configurator->readConfig();

    inputDataProtter = std::unique_ptr<PlotterBuilder>(new PlotterBuilder(w.getCustomPlot(PlotType_Input), servTimerPeriod))
            ->setVerticalRange(vRangeInputMin, vRangeInputMax)
            .setHorizontalResolution(hRangeInput, hStepInput)
            .setInputDataCommand(std::make_shared<SerialCommandAdapter>(serial, dataInput))
            .setOutputDataCommand(std::make_shared<LoggerCommandAdapter>(loggerInput))
            .createPlotter();
    outputDataProtter = std::unique_ptr<PlotterBuilder>(new PlotterBuilder(w.getCustomPlot(PlotType_Output), servTimerPeriod))
            ->setVerticalRange(vRangeOutputMin, vRangeOutputMax)
            .setHorizontalResolution(hRangeOutput, hStepOutput)
            .setInputDataCommand(std::make_shared<SerialCommandAdapter>(serial, dataOutput))
            .setOutputDataCommand(std::make_shared<LoggerCommandAdapter>(loggerOutput))
            .createPlotter();

    w.setPlotCallback(callbackPlot);
    w.setSaveDataCallback(callbackSaveData);
    w.setPausePlotCallback(callbackPausePlot);
    w.setAccessToPortCallback(callbackAccessToPort);

    w.setSerialPortList(serial->getPortList());
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
        loggerInput->clearData();
        loggerOutput->clearData();

        inputDataProtter->start();
        outputDataProtter->start();
    }
}
static void callbackSaveData(bool state) {
    (void)state;
    loggerInput->saveDataToFile();
    loggerOutput->saveDataToFile();
    loggerInput->clearData();
    loggerOutput->clearData();
}
