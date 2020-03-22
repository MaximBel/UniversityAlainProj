#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <serial.h>
#include "stdint.h"
#include "plotter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tbAccessToPort_toggled(bool checked);
    void on_tbPlot_toggled(bool checked);
    void on_pbPause_toggled(bool checked);
    void on_pbSaveData_clicked();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<Serial> serial;
    std::unique_ptr<Plotter> inputDataProtter;
    std::unique_ptr<Plotter> outputDataProtter;

    QVector<float> DataIn_Buffer;
    QVector<float> DataOut_Buffer;

    //const QString DataInFile = "SignalIn.txt";
    //const QString DataOutFile = "SignalOut.txt";

    void WriteDataToFile(QString FileName, QVector<float> *DataVector);
    void ReadConfigFromFile(void);
    void ParseConfig(int32_t *var, const QString *line);

    void updateSerialPortList(void);
    void updateSerialBaudList(void);
};

#endif // MAINWINDOW_H
