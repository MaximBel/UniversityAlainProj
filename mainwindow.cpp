#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include "plotter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ReadConfigFromFile();

    UpdatePortList();
    UpdateBaudList();

    ui->Plot_DataIn->yAxis->setRangeLower(verticalRangeMin_Input);//диапазон по у
    ui->Plot_DataIn->yAxis->setRangeUpper(verticalRangeMax_Input);

    ui->Plot_DataOut->yAxis->setRangeLower(verticalRangeMin_Output);//диапазон по у
    ui->Plot_DataOut->yAxis->setRangeUpper(verticalRangeMax_Output);

    DataIn_Buffer.resize(0);

    DataOut_Buffer.resize(0);
}

MainWindow::~MainWindow()
{
    if(serial->isOpened()){
        serial->Close_Serial();
    }
    this->destroy();
    //delete ui;
}

void MainWindow::on_toolButton_toggled(bool checked)
{
    if(checked){
        serial=new Serial(this);
        serial->Init_Serial(ui->portComboBox->currentText(),ui->baudComboBox->currentText().toInt());
        if(serial->isOpened()){
            ui->portComboBox->setEnabled(false);
            ui->baudComboBox->setEnabled(false);
            ui->plot_button->setEnabled(true);

        } else {
            serial->Close_Serial();
            ui->toolButton->setChecked(false);
        }
    } else{
        if(serial->isOpened()){

            ui->plot_button->setChecked(false);
            serial->Close_Serial();

        }
        ui->portComboBox->setEnabled(true);
        ui->baudComboBox->setEnabled(true);
        ui->plot_button->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
    }

}

void MainWindow::setup_plot(){//////////////////setup graphic

    DataIn_Buffer.resize(0);
    DataOut_Buffer.resize(0);

    ui->Plot_DataIn->addGraph(); //добавляем новый график
    ui->Plot_DataIn->graph(0)->setPen(QPen(Qt::blue));//цвет линии
    //ui->customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->Plot_DataIn->graph(0)->setAntialiasedFill(false);
    ui->Plot_DataIn->addGraph(); //добавляем новый график

    ui->Plot_DataIn->addGraph(); //точка в конце графика
    ui->Plot_DataIn->graph(1)->setPen(QPen(Qt::blue));//цвет точка
    ui->Plot_DataIn->graph(1)->setLineStyle(QCPGraph::lsNone);//без линии
    ui->Plot_DataIn->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);//тип фигуры(диск)

    ui->Plot_DataIn->xAxis->setTickLabelType(QCPAxis::ltDateTime);//ось х-время
    ui->Plot_DataIn->xAxis->setDateTimeFormat("hh:mm:ss");//тип времени(часы:минуты:секунды)
    ui->Plot_DataIn->xAxis->setAutoTickStep(false);
    ui->Plot_DataIn->xAxis->setTickStep(2);//шаг 2с
    ui->Plot_DataIn->axisRect()->setupFullAxesBox();
    ui->Plot_DataIn->yAxis->setRangeLower(verticalRangeMin_Input);//диапазон по у
    ui->Plot_DataIn->yAxis->setRangeUpper(verticalRangeMax_Input);

    ///////////////////////////
    ui->Plot_DataOut->addGraph(); //добавляем новый график
    ui->Plot_DataOut->graph(0)->setPen(QPen(Qt::blue));//цвет линии
    //ui->customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->Plot_DataOut->graph(0)->setAntialiasedFill(false);
    ui->Plot_DataOut->addGraph(); //добавляем новый график

    ui->Plot_DataOut->addGraph(); //точка в конце графика
    ui->Plot_DataOut->graph(1)->setPen(QPen(Qt::blue));//цвет точка
    ui->Plot_DataOut->graph(1)->setLineStyle(QCPGraph::lsNone);//без линии
    ui->Plot_DataOut->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);//тип фигуры(диск)

    ui->Plot_DataOut->xAxis->setTickLabelType(QCPAxis::ltDateTime);//ось х-время
    ui->Plot_DataOut->xAxis->setDateTimeFormat("hh:mm:ss");//тип времени(часы:минуты:секунды)
    ui->Plot_DataOut->xAxis->setAutoTickStep(false);
    ui->Plot_DataOut->xAxis->setTickStep(2);//шаг 2с
    ui->Plot_DataOut->axisRect()->setupFullAxesBox();
    ui->Plot_DataOut->yAxis->setRangeLower(verticalRangeMin_Output);//диапазон по у
    ui->Plot_DataOut->yAxis->setRangeUpper(verticalRangeMax_Output);

    // изменение диапазонов по х
    connect(ui->Plot_DataIn->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->Plot_DataIn->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->Plot_DataOut->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->Plot_DataOut->xAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
}

void MainWindow::realtimeDataSlot_DataIn(){///////////////////rebuild graphic
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.01) //добавление новой точки графика каждые 10мс
    {
        float value=serial->getData(dataInput);

        DataIn_Buffer.append(value);

        ui->input_line->setText(QString::number(serial->getData(dataInput)));

        // добавление точки на графике addData(x,y):
        ui->Plot_DataIn->graph(0)->addData(key, value);

        // set data of dots:
        ui->Plot_DataIn->graph(1)->clearData();//чистим предидущую точку
        ui->Plot_DataIn->graph(1)->addData(key, value);//устанавливаем новую точку addData(x,y)
        // удалаем график,находящийся за пределами видимости в окне:
        ui->Plot_DataIn->graph(0)->removeDataBefore(key-8);
        lastPointKey = key;
    }
    // обновление диапазона по х с учетом точки на графике (at a constant range size of 8):
    ui->Plot_DataIn->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
    ui->Plot_DataIn->replot();
}

void MainWindow::realtimeDataSlot_DataOut(){///////////////////rebuild graphic
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.01) //добавление новой точки графика каждые 10мс
    {
        float value=serial->getData(dataOutput);

        DataOut_Buffer.append(value);

        ui->output_line->setText(QString::number(serial->getData(dataOutput)));

        // добавление точки на графике addData(x,y):
        ui->Plot_DataOut->graph(0)->addData(key, value);

        // set data of dots:
        ui->Plot_DataOut->graph(1)->clearData();//чистим предидущую точку
        ui->Plot_DataOut->graph(1)->addData(key, value);//устанавливаем новую точку addData(x,y)
        // удалаем график,находящийся за пределами видимости в окне:
        ui->Plot_DataOut->graph(0)->removeDataBefore(key-8);
        lastPointKey = key;
    }
    // обновление диапазона по х с учетом точки на графике (at a constant range size of 8):
    ui->Plot_DataOut->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
    ui->Plot_DataOut->replot();
}

void MainWindow::on_plot_button_toggled(bool checked)
{
    if(checked){
        serial->setControl(true); // запускаем работу ардуины
        setup_plot();//инициализация графика
        plot_timer1=new QTimer(this);//таймер для работы графика

        connect(plot_timer1, SIGNAL(timeout()), this, SLOT(realtimeDataSlot_DataIn()));//перестройка графика по сигналу от таймера
        connect(plot_timer1, SIGNAL(timeout()), this, SLOT(realtimeDataSlot_DataOut()));   

        plot_timer1->start(30); // максимальная скорость работы таймера

        ui->toolButton->setEnabled(false);
        ui->pushButton_4->setEnabled(true);
    }else{
        serial->setControl(false); // останавливаем работу ардуины
        disconnect(plot_timer1, SIGNAL(timeout()), this, SLOT(realtimeDataSlot_DataIn()));//отключаем метод перестройки графика от таймера
        disconnect(plot_timer1, SIGNAL(timeout()), this, SLOT(realtimeDataSlot_DataOut()));
        plot_timer1->stop();//останавливаем таймер построения

        ui->Plot_DataIn->clearGraphs();/////////////////очистка графика
        ui->Plot_DataIn->replot();
        ui->Plot_DataOut->clearGraphs();/////////////////очистка графика
        ui->Plot_DataOut->replot();
        ui->toolButton->setEnabled(true);
        ui->pushButton_4->setEnabled(false);
    }
}

void MainWindow::on_pushButton_4_toggled(bool checked)
{
    if(checked){

        ui->pushButton_WriteToFile->setEnabled(true);

        disconnect(plot_timer1, SIGNAL(timeout()), this, SLOT(realtimeDataSlot_DataIn()));//отключаем метод перестройки графика от таймера
        disconnect(plot_timer1, SIGNAL(timeout()), this, SLOT(realtimeDataSlot_DataOut()));
    }else{

        ui->pushButton_WriteToFile->setEnabled(false);

        DataIn_Buffer.resize(0);

        DataOut_Buffer.resize(0);

        connect(plot_timer1, SIGNAL(timeout()), this, SLOT(realtimeDataSlot_DataIn()));//перестройка графика по сигналу от таймера
        connect(plot_timer1, SIGNAL(timeout()), this, SLOT(realtimeDataSlot_DataOut()));
    }
}

void MainWindow::on_pushButton_WriteToFile_clicked()
{

    if(DataIn_Buffer.size() != 0 && DataOut_Buffer.size() != 0) {

        WriteDataToFile(DataInFile, &DataIn_Buffer);

        WriteDataToFile(DataOutFile, &DataOut_Buffer);

        DataIn_Buffer.resize(0);

        DataOut_Buffer.resize(0);

    }

}


void MainWindow::WriteDataToFile(QString FileName, QVector<float> *DataVector) {
    QFile OutputFile(qApp->applicationDirPath() + "/"+ FileName);

    if(OutputFile.open(QIODevice::WriteOnly | QIODevice::Truncate) == true) {

        QTextStream OutputStream(&OutputFile);

        for(int counter = 0; counter < DataVector->size(); counter++) {

            OutputStream << DataVector->data()[counter];

            OutputStream << ' ';

        }

        OutputFile.close();

    }

}

void MainWindow::ReadConfigFromFile(void) {
    QFile file(qApp->applicationDirPath() + "/config.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cant open config file";
        return;
    }


    while (!file.atEnd()) {
         QString line = file.readLine();
qDebug() << "line:" + line;
         if(line.contains("RANGE_MAX_INPUT=")) {
             ParseConfig(&verticalRangeMax_Input, &line);
         } else if(line.contains("RANGE_MIN_INPUT=")) {
             ParseConfig(&verticalRangeMin_Input, &line);
         } else if(line.contains("RANGE_MAX_OUTPUT=")) {
             ParseConfig(&verticalRangeMax_Output, &line);
         } else if(line.contains("RANGE_MIN_OUTPUT=")) {
             ParseConfig(&verticalRangeMin_Output, &line);
         }

    }

}

void MainWindow::ParseConfig(int32_t *var, const QString *line) {

    QStringList list = line->split("=");

    if(list.size() == 2) {

        *var = list[1].toInt();

        qDebug() << "Parse config for var:" + list[0];

    }
}

void MainWindow::UpdatePortList(void) {

    ui->portComboBox->clear();

    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();

    for(int i = 0; i < list.size(); i++) {

        ui->portComboBox->addItem(list[i].portName());

    }

}

void MainWindow::UpdateBaudList(void) {

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
