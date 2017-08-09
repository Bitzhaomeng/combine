#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "compass.h"
#include "motor.h"
#include "motorthread.h"
#include "adq214.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_readCompassButton_clicked();

    void on_startButton_clicked();

    void checkMove();

private:
    Ui::MainWindow *ui;
    compass Compass;
    void showCompassAngle(const double &s);
    void checkMotorAngle(const double &s);
    void timeStart();
    void getPosition();
    void checkMotor();

    motor Motor;
    void readyToMove();
    bool moveNorth,checkReady;
    double headAngle,motorPX0;
    QTimer *timeOclock;
//    motorthread Motorthread;
//    compassThread CompassThread;

    ADQ214 capture;
    int perTime=60;     //每次转动的角度，先假设为每60°采一组数
};

#endif // MAINWINDOW_H
