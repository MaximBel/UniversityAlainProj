#include "plotter.h"

std::unique_ptr<QTimer> Plotter::plotTimer = nullptr;

PlotterBuilder::PlotterBuilder(QCustomPlot* plot): verticalMin(0), verticalMax(260), horResolution(0.2) {
    uiPlot = plot;
}
PlotterBuilder* PlotterBuilder::setVerticalRange(int32_t min, int32_t max) {
    verticalMin = min;
    verticalMax = max;

    return this;
}
PlotterBuilder* PlotterBuilder::setHorizontalResolution(float res) {
    horResolution = res;

    return this;
}

std::unique_ptr<Plotter> createPlotter() {
    auto plot = std::unique_ptr<Plotter>();

    return plot;
}

Plotter::Plotter()
{
    if(!plotTimer) {
        plotTimer = std::unique_ptr<QTimer>();
    }
}


