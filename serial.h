#ifndef SERIAL_H
#define SERIAL_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include "stdint.h"

typedef enum {
    dataInput,
    dataOutput
} dataType;

class Serial : public QWidget
{
    Q_OBJECT
protected:
    bool enabled=false;

private:
    uint8_t controlByte = 0;

public:
    explicit Serial(QWidget *parent = 0);
    void Init_Serial(QString Serial_name,int Baud);
    void Close_Serial();
    bool isOpened();
    void setControl(bool run);

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

    float getData(dataType type);

signals:

public slots:
    void read_port();
    void WriteToPort();
};

#endif // SERIAL_H
