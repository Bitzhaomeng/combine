#ifndef MOTOR_H
#define MOTOR_H

#include <QObject>
#include"motorthread.h"

class motor : public QObject
{
    Q_OBJECT
public:
    explicit motor(QObject *parent = nullptr);
    void prepare();
    void moveAbsolute(const int &a);
    void moveRelative(const int &a);
    void position();
signals:
  void motorAngle(const double &s);
   void motorPrepareOk();
public slots:
private slots:
    void portError_OR_timeout();
     void receive_response(const QString &s);
private:
    QString portname,Order_str,AC,DC,PR,PA,SP,PX_Origin,PX_Begin,ret;
    //       串口名   发送命令 加减速度 相对/绝对移动 零点原始位置  电机转动初始位置 接收数据
    motorthread thread_port;
    bool defaultset,direction,handle_PX,check_PX,PR_move,first_move;
    //                     默认设置    转向      等待获取位置 等待检查位置  确定PR、PA转动  判断初次转动
};

#endif // MOTOR_H



