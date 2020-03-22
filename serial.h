#ifndef SERIAL_H
#define SERIAL_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include "stdint.h"
#include <QSerialPortInfo>

typedef enum {
    dataInput,
    dataOutput
} DataType;

class Serial : public QWidget
{
    Q_OBJECT

public:
    explicit Serial(QWidget *parent = 0);
    void open(QString Serial_name,int Baud);
    void close();
    bool isOpened();
    void setControl(bool run);
    float getData(DataType type);

    static QList<QSerialPortInfo> getPortList();

public slots:
    void readFromPort();
    void writeToPort();

private:
    bool ready_to_write=true;
    QSerialPort *Serial_port;
    union{
        short d;
        unsigned char b[2];
    } temp_in;

    int pitch_os=0;
    int pitch_angle=0;
    int roll_os=0;
    int roll_angle=0;
    int yaw_os=0;
    int yaw_angle=0;

    float inputValue = 0;
    float outputValue = 0;

    bool accident_flag=false;
    uint8_t controlByte = 0;
    bool enabled=false;
};

#endif // SERIAL_H
