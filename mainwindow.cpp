#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plotter.h"
#include "serialcommandadapter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    serial(std::make_shared<Serial>(this)),
    inputDataProtter(nullptr),
    outputDataProtter(nullptr)
{
    ui->setupUi(this);

    ReadConfigFromFile();

    updateSerialPortList();
    updateSerialBaudList();

    inputDataProtter = std::unique_ptr<PlotterBuilder>(new PlotterBuilder(ui->Plot_DataIn, 30))
            ->setVerticalRange(0, 260)
            .setHorizontalResolution(8, 0.02)
            .setInputDataCommand(std::make_shared<SerialCommandAdapter>(serial, dataInput))
            .createPlotter();
    outputDataProtter = std::unique_ptr<PlotterBuilder>(new PlotterBuilder(ui->Plot_DataOut, 30))
            ->setVerticalRange(0, 260)
            .setHorizontalResolution(8, 0.02)
            .setInputDataCommand(std::make_shared<SerialCommandAdapter>(serial, dataOutput))
            .createPlotter();

    DataIn_Buffer.resize(0);
    DataOut_Buffer.resize(0);
}

MainWindow::~MainWindow()
{
    if(serial->isOpened()){
        serial->close();
    }
    this->destroy();
    //delete ui;
}

void MainWindow::WriteDataToFile(QString FileName, QVector<float> *DataVector) {
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

void MainWindow::ReadConfigFromFile(void) {
//    QFile file(qApp->applicationDirPath() + "/config.txt");
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        qDebug() << "Cant open config file";
//        return;
//    }


//    while (!file.atEnd()) {
//         QString line = file.readLine();
//qDebug() << "line:" + line;
//         if(line.contains("RANGE_MAX_INPUT=")) {
//             ParseConfig(&verticalRangeMax_Input, &line);
//         } else if(line.contains("RANGE_MIN_INPUT=")) {
//             ParseConfig(&verticalRangeMin_Input, &line);
//         } else if(line.contains("RANGE_MAX_OUTPUT=")) {
//             ParseConfig(&verticalRangeMax_Output, &line);
//         } else if(line.contains("RANGE_MIN_OUTPUT=")) {
//             ParseConfig(&verticalRangeMin_Output, &line);
//         }

//    }
}

void MainWindow::ParseConfig(int32_t *var, const QString *line) {
//    QStringList list = line->split("=");

//    if(list.size() == 2) {
//        *var = list[1].toInt();
//        qDebug() << "Parse config for var:" + list[0];
//    }
}

void MainWindow::updateSerialPortList(void) {
    auto list = serial->getPortList();

    ui->portComboBox->clear();

    for(int i = 0; i < list.size(); i++) {
        ui->portComboBox->addItem(list[i].portName());
    }
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

        serial->open(ui->portComboBox->currentText(),ui->baudComboBox->currentText().toInt());

        if(serial->isOpened()){
            ui->portComboBox->setEnabled(false);
            ui->baudComboBox->setEnabled(false);
            ui->tbPlot->setEnabled(true);
        } else {
            serial->close();

            ui->tbPlot->setChecked(false);
        }
    } else{
        if(serial->isOpened()){
            ui->tbPlot->setChecked(false);
            serial->close();
        }
        ui->portComboBox->setEnabled(true);
        ui->baudComboBox->setEnabled(true);
        ui->tbPlot->setEnabled(false);
        ui->pbPause->setEnabled(false);
    }
}

void MainWindow::on_tbPlot_toggled(bool checked) {
    if(checked){
        serial->setControl(true); // запускаем работу ардуины

        inputDataProtter->initialize();
        outputDataProtter->initialize();
        inputDataProtter->start();
        outputDataProtter->start();

        ui->tbPlot->setEnabled(false);
        ui->pbPause->setEnabled(true);
    }else{
        serial->setControl(false); // останавливаем работу ардуины

        inputDataProtter->stop();
        outputDataProtter->stop();
        inputDataProtter->clear();
        outputDataProtter->clear();

        ui->tbPlot->setEnabled(true);
        ui->pbPause->setEnabled(false);
    }
}

void MainWindow::on_pbPause_toggled(bool checked) {
    if(checked){
        ui->pbSaveData->setEnabled(true);

        inputDataProtter->stop();
        outputDataProtter->stop();
    }else{
        ui->pbSaveData->setEnabled(false);

        //DataIn_Buffer.resize(0);
        //DataOut_Buffer.resize(0);

        inputDataProtter->start();
        outputDataProtter->start();
    }
}

void MainWindow::on_pbSaveData_clicked() {
//    if(DataIn_Buffer.size() != 0 && DataOut_Buffer.size() != 0) {
//        WriteDataToFile(DataInFile, &DataIn_Buffer);
//        WriteDataToFile(DataOutFile, &DataOut_Buffer);
//        DataIn_Buffer.resize(0);
//        DataOut_Buffer.resize(0);
//    }
}
