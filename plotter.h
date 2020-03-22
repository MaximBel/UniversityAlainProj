#ifndef PLOTTER_H
#define PLOTTER_H

#include "qcustomplot.h"
#include "datacommand.h"

class Plotter: public QObject
{
    Q_OBJECT

public:
    Plotter();
    virtual ~Plotter();
    void initialize();
    void start();
    void stop();
    void clear();

    friend class PlotterBuilder; // builder class can access private fields

protected:
    QCustomPlot* uiPlot;
    int32_t verticalMin;
    int32_t verticalMax;
    float horResolution;
    float horStepResolution;
    std::shared_ptr<DataCommand<float>> inputDataCommand;
    std::shared_ptr<DataCommand<float>> outputDataCommand;

    static uint32_t servTimerInterval;

private:
    static std::unique_ptr<QTimer> plotTimer;
    float lastPointKey;
    float startPointKey;

    static void initializeStatic();

private slots:
    void slotPlotUpdate();
};

class PlotterBuilder: public Plotter {
public:
    PlotterBuilder(QCustomPlot* plot, uint32_t servTimerMsec = 0);
    ~PlotterBuilder() = default;
    PlotterBuilder& setVerticalRange(int32_t min, int32_t max);
    PlotterBuilder& setHorizontalResolution(float res, float stepResolution);
    PlotterBuilder& setInputDataCommand(std::shared_ptr<DataCommand<float>> command);
    PlotterBuilder& setOutputDataCommand(std::shared_ptr<DataCommand<float>> command);
    std::unique_ptr<Plotter> createPlotter();
private:
    QCustomPlot* uiPlot;
    int32_t verticalMin;
    int32_t verticalMax;
    float horResolution;
    float horStepResolution;
    uint32_t servTimerInterval;
    std::shared_ptr<DataCommand<float>> inputDataCommand;
    std::shared_ptr<DataCommand<float>> outputDataCommand;
};



#endif // PLOTTER_H
