#ifndef ADQ214_H
#define ADQ214_H

#include <QObject>
#include "ADQAPI.h"
#include <QDebug>
#include <memory.h>
#include <stdio.h>
#include <QFile>

class ADQ214 : public QObject
{
    Q_OBJECT
public:
    explicit ADQ214(QObject *parent = nullptr);
    void connectADQDevice();
    void beginADQ();

signals:
    void collectFinish();
public slots:
private:
    bool isADQ214Connected;
    void *adq_cu;
    FILE* outfileA = NULL;
    FILE* outfileB = NULL;
};

#endif // ADQ214_H
