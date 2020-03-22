#include "plotter.h"

std::unique_ptr<QTimer> Plotter::plotTimer = nullptr;
uint32_t Plotter::servTimerInterval = 30;

//////////////////
// PLOTTER BUILDER
//////////////////

PlotterBuilder::PlotterBuilder(QCustomPlot* plot, uint32_t servTimerMsec):
    uiPlot(plot), verticalMin(0), verticalMax(260),
    horResolution(8), horStepResolution(0.2), servTimerInterval(servTimerMsec),
    inputDataCommand(nullptr), outputDataCommand(nullptr) {}

PlotterBuilder& PlotterBuilder::setVerticalRange(int32_t min, int32_t max) {
    verticalMin = min;
    verticalMax = max;
    return *this;
}
PlotterBuilder& PlotterBuilder::setHorizontalResolution(float res, float stepResolution) {
    horResolution = res;
    horStepResolution = stepResolution;
    return *this;
}

PlotterBuilder& PlotterBuilder::setInputDataCommand(std::shared_ptr<DataCommand<float>> command) {
    inputDataCommand = command;
    return *this;
}
PlotterBuilder& PlotterBuilder::setOutputDataCommand(std::shared_ptr<DataCommand<float>> command) {
    outputDataCommand = command;
    return *this;
}

std::unique_ptr<Plotter> PlotterBuilder::createPlotter() {
    auto plot = std::unique_ptr<Plotter>(new Plotter);

    if(servTimerInterval) {
        plot->servTimerInterval = servTimerInterval;
    }

    plot->uiPlot = uiPlot;
    plot->verticalMin = verticalMin;
    plot->verticalMax = verticalMax;
    plot->horResolution = horResolution;
    plot->horStepResolution = horStepResolution;
    plot->inputDataCommand = inputDataCommand;
    plot->outputDataCommand = inputDataCommand;

    return plot;
}

//////////
// PLOTTER
//////////

Plotter::Plotter(): lastPointKey(0), startPointKey(0) {
    if(!plotTimer) {
        plotTimer = std::unique_ptr<QTimer>(new QTimer());
    }
}

Plotter::~Plotter() {
    if(plotTimer->isActive()) {
        plotTimer->stop();
    }
}

void Plotter::initialize() {
    initializeStatic();

    uiPlot->addGraph(); //добавляем новый график
    uiPlot->graph(0)->setPen(QPen(Qt::blue));//цвет линии
    uiPlot->graph(0)->setAntialiasedFill(false);

    uiPlot->addGraph(); //добавляем новый график
    uiPlot->addGraph(); //точка в конце графика
    uiPlot->graph(1)->setPen(QPen(Qt::blue));//цвет точка
    uiPlot->graph(1)->setLineStyle(QCPGraph::lsNone);//без линии
    uiPlot->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);//тип фигуры(диск)

    uiPlot->xAxis->setTickLabelType(QCPAxis::ltNumber);//ось х-число
    //uiPlot->xAxis->setDateTimeFormat("hh:mm:ss");//тип времени(часы:минуты:секунды)
    uiPlot->xAxis->setAutoTickStep(false);
    uiPlot->xAxis->setTickStep(horStepResolution);//шаг по оси Х
    uiPlot->axisRect()->setupFullAxesBox();

    uiPlot->yAxis->setRangeLower(verticalMin);//диапазон по Y
    uiPlot->yAxis->setRangeUpper(verticalMax);

    // изменение диапазонов по х
    connect(uiPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), uiPlot->xAxis2, SLOT(setRange(QCPRange)));
}
void Plotter::start() {
    startPointKey = QDateTime::currentDateTime().toMSecsSinceEpoch();
    lastPointKey = 0;

    connect(plotTimer.get(), SIGNAL(timeout()), this, SLOT(slotPlotUpdate()));//перестройка графика по сигналу от таймера
}
void Plotter::stop() {
    disconnect(plotTimer.get(), SIGNAL(timeout()), this, SLOT(slotPlotUpdate()));//отключаем метод перестройки графика от таймера
}
void Plotter::clear() {
    uiPlot->clearGraphs();/////////////////очистка графика
    uiPlot->replot();
}

void Plotter::initializeStatic() {
    if( !plotTimer->isActive() ) {
        plotTimer->start(servTimerInterval);
    }
}

void Plotter::slotPlotUpdate() {
    float key = QDateTime::currentDateTime().toMSecsSinceEpoch() - startPointKey;
    float value=inputDataCommand->runCommand(0);

    //ui->input_line->setText(QString::number(serial->getData(dataInput)));

    // добавление точки на графике addData(x,y):
    uiPlot->graph(0)->addData(key, value);

    // set data of dots:
    uiPlot->graph(1)->clearData();//чистим предидущую точку
    uiPlot->graph(1)->addData(key, value);//устанавливаем новую точку addData(x,y)
    // удалаем график,находящийся за пределами видимости в окне:
    uiPlot->graph(0)->removeDataBefore(key-horResolution);
    // обновление диапазона по х с учетом точки на графике (at a constant range size of 8):
    uiPlot->xAxis->setRange(key+0.25, horResolution, Qt::AlignRight);
    uiPlot->replot();

    if(outputDataCommand) {
        (void)outputDataCommand->runCommand(value);
    }

    lastPointKey = key;
}


