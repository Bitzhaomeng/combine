#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "compass.h"
#include "motor.h"

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

    void on_portOpenButton_clicked();

    void on_motorPrepareButton_clicked();

    void on_motorMoveButton_clicked();

    void on_motorReadButton_clicked();

    void on_RelativeMoveButton_clicked();

private:
    Ui::MainWindow *ui;
    compass Compass;
    void showCompassAngle(const double &s);
    void showMotorAngle(const double &s);
    motor Motor;
    void show2();
};

#endif // MAINWINDOW_H
