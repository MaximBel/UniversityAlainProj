#include "serial.h"

Serial::Serial(QWidget *parent) : QWidget(parent)
{
    Serial_port = new QSerialPort (this);
}

void Serial::open(QString Serial_name,int Baud){
    Serial_port->setPortName(Serial_name);
    Serial_port->setBaudRate(Baud);
    Serial_port->setDataBits(QSerialPort::Data8);
    Serial_port->setParity(QSerialPort::NoParity);
    Serial_port->setStopBits(QSerialPort::OneStop);
    Serial_port->setFlowControl(QSerialPort::NoFlowControl);
    Serial_port->clear();
    if (Serial_port->open(QIODevice::ReadWrite))
    {
        Serial_port->setBaudRate(Baud); // короче, без этого получаем какую-то кашу. похоже что без этого неправильно инициализируется порт
        Serial_port->setDataBits(QSerialPort::Data8);
        Serial_port->setParity(QSerialPort::NoParity);
        Serial_port->setStopBits(QSerialPort::OneStop);
        Serial_port->setFlowControl(QSerialPort::NoFlowControl);
        Serial_port->clear();

        enabled=true;
        qDebug() << "+++++++++++++++++++++++++++++++++++";
        qDebug() << "COMPORT Opened";
        qDebug() << "+++++++++++++++++++++++++++++++++++";
        connect(Serial_port,SIGNAL(readyRead()),this,SLOT(readFromPort()));
    }
    else
    {
        enabled=false;
        qDebug() << "===================================";
        qDebug() << "COMPORT Closed";
        qDebug() << "===================================";
    }

}

void Serial::readFromPort(){
    int start=-1;
    //qDebug() << available;
    //if(available==11){
    //QByteArray buffer1;
    //buffer1.clear();



    uint8_t buffer[200];
    for(int i=0;i<sizeof(buffer);i++) buffer[i]=0;
    int available=(int)Serial_port->bytesAvailable();
    if(available>=11 and available<50){
        //buffer1 = Serial_port->readLine(65535);
        int read=Serial_port->read((char *)buffer,available);
        //qDebug()<< "Buffer ==========";
        for(int i=0;i<available;i++){
            //qDebug()<< (uint8_t)buffer[i];
            if((unsigned char)buffer[i]==253){
                start=i;
                //break;
            }
        }
        //qDebug()<< "==========";
    }else if(available>50){
        char clear_buf[2000];
        int clear=Serial_port->read(clear_buf,Serial_port->bytesAvailable());
    }

    if(start!=-1){
        if((int)(unsigned char)buffer[start+10]==254){

            union {
                float fl;
                uint8_t bytes[4];
            } bytes_to_float;

            bytes_to_float.bytes[0] = (uint8_t)buffer[start+1];
            bytes_to_float.bytes[1] = (uint8_t)buffer[start+2];
            bytes_to_float.bytes[2] = (uint8_t)buffer[start+3];
            bytes_to_float.bytes[3] = (uint8_t)buffer[start+4];

            inputValue = bytes_to_float.fl;

            bytes_to_float.bytes[0] = (uint8_t)buffer[start+5];
            bytes_to_float.bytes[1] = (uint8_t)buffer[start+6];
            bytes_to_float.bytes[2] = (uint8_t)buffer[start+7];
            bytes_to_float.bytes[3] = (uint8_t)buffer[start+8];

            outputValue = bytes_to_float.fl;

        }
    }
}

void Serial :: writeToPort(){//Запись данных в порт
    QByteArray ctrl;
    if(enabled) {

        ///ctrl.resize(0);
        //ctrl.append(static_cast<char>(controlByte));
        //Serial_port->write(ctrl);

    }

}

bool Serial::isOpened(){
    if (enabled){
        return true;
    } else return false;
}

void Serial::setControl(bool run) {
    if(run) {
        controlByte = 240;
    } else {
        controlByte = 0;
    }
}

void Serial::close(){

    enabled=false;
    disconnect(Serial_port,SIGNAL(readyRead()),this,SLOT(readFromPort()));
    Serial_port->close();

}

float Serial::getData(DataType type){
    switch(type){
    case dataInput:
        return inputValue;
        break;
    case dataOutput:
        return outputValue;
        break;
    default:
        return 0;
    }
}

QList<QSerialPortInfo> Serial::getPortList() {
    return QSerialPortInfo::availablePorts();
}



