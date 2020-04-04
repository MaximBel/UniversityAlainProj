#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plotter.h"
#include "serialcommandadapter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateSerialBaudList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateSerialBaudList(void) {
    ui->baudComboBox->clear();

    ui->baudComboBox->addItem("9600");
    ui->baudComboBox->addItem("19200");
    ui->baudComboBox->addItem("38400");
    ui->baudComboBox->addItem("57600");
    ui->baudComboBox->addItem("74880");
    ui->baudComboBox->addItem("115200");
    ui->baudComboBox->addItem("230400");
    ui->baudComboBox->addItem("250000");
}

void MainWindow::on_tbAccessToPort_toggled(bool checked) {
    if(checked){
        if(!ui->portComboBox->currentText().length()) {
            ui->tbAccessToPort->setChecked(false);
            return;
        }
        if(clbAccesToPort(true, ui->portComboBox->currentText(), ui->baudComboBox->currentText().toInt())){
            ui->portComboBox->setEnabled(false);
            ui->baudComboBox->setEnabled(false);
            ui->tbPlot->setEnabled(true);
        } else {
            clbAccesToPort(false, ui->portComboBox->currentText(), ui->baudComboBox->currentText().toInt());

            ui->tbPlot->setChecked(false);
        }
    } else{
        clbAccesToPort(false, ui->portComboBox->currentText(), ui->baudComboBox->currentText().toInt());

        ui->portComboBox->setEnabled(true);
        ui->baudComboBox->setEnabled(true);
        ui->tbPlot->setEnabled(false);
        ui->pbPause->setEnabled(false);
    }
}

void MainWindow::on_tbPlot_toggled(bool checked) {
    if(checked){
        clbPlot(true);

        ui->tbPlot->setEnabled(false);
        ui->pbPause->setEnabled(true);
    }else{
        clbPlot(false);

        ui->tbPlot->setEnabled(true);
        ui->pbPause->setEnabled(false);
    }
}

void MainWindow::on_pbPause_toggled(bool checked) {
    if(checked){
        ui->pbSaveData->setEnabled(true);

        clbPausePlot(true);
    }else{
        ui->pbSaveData->setEnabled(false);

        clbPausePlot(false);
    }
}

void MainWindow::on_pbSaveData_clicked() {
    clbSaveData(true);
}

void MainWindow::setAccessToPortCallback(std::function<bool(bool, QString, int)> clb) {
    clbAccesToPort = clb;
}
void MainWindow::setPlotCallback(std::function<void(bool)> clb) {
    clbPlot = clb;
}
void MainWindow::setPausePlotCallback(std::function<void(bool)> clb) {
    clbPausePlot = clb;
}
void MainWindow::setSaveDataCallback(std::function<void(bool)> clb) {
    clbSaveData = clb;
}

void MainWindow::setSerialPortList(QList<QSerialPortInfo> list) {
    ui->portComboBox->clear();

    for(int i = 0; i < list.size(); i++) {
        ui->portComboBox->addItem(list[i].portName());
    }
}

QCustomPlot* MainWindow::getCustomPlot(PlotType_t type) {
    if(type == PlotType_Input) {
        return ui->Plot_DataIn;
    } else {
        return ui->Plot_DataOut;
    }
}
