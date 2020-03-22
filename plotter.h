#ifndef PLOTTER_H
#define PLOTTER_H

#include "qcustomplot.h"

class Plotter
{
public:
    Plotter();

protected:
    QCustomPlot* uiPlot;
    int32_t verticalMin;
    int32_t verticalMax;
    float horResolution;

private:
    static std::unique_ptr<QTimer> plotTimer;
};

class PlotterBuilder: private Plotter {
public:
    PlotterBuilder(QCustomPlot* plot);
    ~PlotterBuilder() = default;
    PlotterBuilder* setVerticalRange(int32_t min, int32_t max);
    PlotterBuilder* setHorizontalResolution(float res);
    std::unique_ptr<Plotter> createPlotter();
private:
    QCustomPlot* uiPlot;
    int32_t verticalMin;
    int32_t verticalMax;
    float horResolution;
};



#endif // PLOTTER_H
