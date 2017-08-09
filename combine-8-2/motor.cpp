#include "motor.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

motor::motor(QObject *parent) : QObject(parent)
{
    connect(&thread_port,SIGNAL(response2(QString)),this,SLOT(receive_response(QString)));
    connect(&thread_port,SIGNAL(S_PortNotOpen()),this,SLOT(portError_OR_timeout()));
    connect(&thread_port,SIGNAL(timeout2()),this,SLOT(portError_OR_timeout()));
}

void motor::prepare()
{
    /*portname.clear();
    QSerialPort my_serial;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        my_serial.close();
        my_serial.setPortName(info.portName());
        if(!my_serial.open(QIODevice::ReadWrite))
            return;
        my_serial.setBaudRate(QSerialPort::Baud19200);
        my_serial.setDataBits(QSerialPort::Data8);
        my_serial.setStopBits(QSerialPort::OneStop);
        my_serial.setFlowControl(QSerialPort::NoFlowControl);

        QString test("VR;");
        QByteArray testData = test.toLocal8Bit();
        my_serial.write(testData);
        if(my_serial.waitForBytesWritten(15))
        {
            if(my_serial.waitForReadyRead(30))
            {
                QByteArray testResponse = my_serial.readAll();
                while(my_serial.waitForReadyRead(10))
                    testResponse += my_serial.readAll();
                QString response(testResponse);
                if(response.left(10) == "VR;Whistle")
                {
                    portname = info.portName();
                    break;
                }
            }
        }
    }
    my_serial.close();

    if(portname.left(3) == "COM")
    {
        Order_str = "MO=1;";
        thread_port.transaction(portname,Order_str);

    }*/
    portname="COM8";
    Order_str = "MO=1;";
    thread_port.transaction(portname,Order_str);

}

void motor::moveAbsolute(const int &a)
{   QString anglePA=QString::number(a*524000/360);
    qDebug()<<"a="<<anglePA;
    Order_str = "PA="+anglePA+";";
    thread_port.transaction(portname,Order_str);
}

void motor::moveRelative(const int &a)
{
    QString anglePR=QString::number(a*524000/360);
    Order_str = "PR="+anglePR+";";
    thread_port.transaction(portname,Order_str);
}

void motor::position()
{
    Order_str = "PX;";
    thread_port.transaction(portname,Order_str);
}

void motor::portError_OR_timeout()
{
    qDebug()<<"error";
}

void motor::receive_response(const QString &s)
{
    if(s.left(2) == "MO")                       //电机是否关闭
    {
        if(s.left(4) == "MO=1")
        {
            Order_str = "AC=1800;";
            thread_port.transaction(portname,Order_str);
             qDebug()<<"电机打开";
        }
        else
            qDebug()<<"电机关闭";
    }
    if(s.left(2) == "AC")
    {
        Order_str = "DC=1800;";
        thread_port.transaction(portname,Order_str);
    }
    if(s.left(2) == "DC")
    {
        Order_str = "SP=1310000;";
        thread_port.transaction(portname,Order_str);
    }
    if(s.left(2) == "MS")
    {
        qDebug()<<"MS3";
    }
    if(s.left(2) == "PA")
    {
        Order_str = "BG;";
        thread_port.transaction(portname,Order_str);
    }
    if(s.left(2) == "PR")
    {
        Order_str = "BG;";
        thread_port.transaction(portname,Order_str);
    }
    if(s.left(2) == "PX")
    {
      QString a=s.split(";").at(1).toLocal8Bit().data();
      double angle=a.toInt()*360/524000;
      qDebug()<<"PX"<<angle;
      emit this->motorAngle(angle);
    }
    if(s.left(2) == "SP")
    {
        emit this->motorPrepareOk();
    }
//    if(s.left(10) == "VR;Whistle")
//    {
//        Order_str = "MO=1;";
//        thread_port.transaction(portname,Order_str);
//    }
}





