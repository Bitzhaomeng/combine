#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include<QTimer>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&Compass, &compass::compassAngle, this, &MainWindow::showCompassAngle);
    connect(&Motor, &motor::motorAngle, this, &MainWindow::showMotorAngle);
    connect(&Motor, &motor::motorPrepareOk, this, &MainWindow::show2);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_readCompassButton_clicked()
{
    Compass.read();           //读取罗盘数据
}

void MainWindow::on_portOpenButton_clicked()
{
//    Compass.stop();
    Motor.prepare();
    Compass.read();

}

void MainWindow::on_motorPrepareButton_clicked()
{
   Motor.prepare();        //电机上电
}

void MainWindow::on_motorMoveButton_clicked()
{
    Motor.moveAbsolute(90);   //绝对转动
}

void MainWindow::on_motorReadButton_clicked()
{
    Motor.position();        //读取电机位置
}

void MainWindow::showCompassAngle(const double &s)
{
    ui->compassText->setText(QString::number(s,'f2',2));
}

void MainWindow::showMotorAngle(const double &s)
{
    ui->motorText->setText(QString::number(s,'f2',2));
}

void MainWindow::show2()
{
    Motor.position();
    Motor.moveAbsolute(90);
}

void MainWindow::on_RelativeMoveButton_clicked()
{
     Motor.moveRelative(-90);   //相对转动
}
