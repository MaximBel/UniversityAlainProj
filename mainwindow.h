#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <serial.h>
#include "stdint.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setup_plot();

private slots:
    void on_toolButton_toggled(bool checked);

    void on_plot_button_toggled(bool checked);

    void on_pushButton_4_toggled(bool checked);

    void on_pushButton_WriteToFile_clicked();

private:
    Ui::MainWindow *ui;
    Serial *serial;
    QTimer *plot_timer1;

    QVector<float> DataIn_Buffer;
    QVector<float> DataOut_Buffer;

    int32_t verticalRangeMax_Input = 260;
    int32_t verticalRangeMin_Input = 0;

    int32_t verticalRangeMax_Output = 260;
    int32_t verticalRangeMin_Output = 0;

    char control_byte = 0;

    const QString DataInFile = "SignalIn.txt";

    const QString DataOutFile = "SignalOut.txt";

    void WriteDataToFile(QString FileName, QVector<float> *DataVector);

    void ReadConfigFromFile(void);

    void ParseConfig(int32_t *var, const QString *line);

    void UpdatePortList(void);

    void UpdateBaudList(void);

public slots:
    void realtimeDataSlot_DataIn();
    void realtimeDataSlot_DataOut();
};

#endif // MAINWINDOW_H
