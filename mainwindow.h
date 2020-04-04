#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stdint.h"
#include "functional"
#include "serial.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

typedef enum {
    PlotType_Input,
    PlotType_Output
} PlotType_t;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setAccessToPortCallback(std::function<bool(bool, QString, int)> clb);
    void setPlotCallback(std::function<void(bool)> clb);
    void setPausePlotCallback(std::function<void(bool)> clb);
    void setSaveDataCallback(std::function<void(bool)> clb);

    void setSerialPortList(QList<QSerialPortInfo> list);

    QCustomPlot* getCustomPlot(PlotType_t type);

private slots:
    void on_tbAccessToPort_toggled(bool checked);
    void on_tbPlot_toggled(bool checked);
    void on_pbPause_toggled(bool checked);
    void on_pbSaveData_clicked();

private:
    Ui::MainWindow *ui;
    std::function<bool(bool, QString, int)> clbAccesToPort;
    std::function<void(bool)> clbPlot;
    std::function<void(bool)> clbPausePlot;
    std::function<void(bool)> clbSaveData;

    void updateSerialPortList(void);
    void updateSerialBaudList(void);
};

#endif // MAINWINDOW_H
