#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H
#include"callstateparser.h"
#include"railwaystateparser.h"
#include"trainstateparser.h"

////////////////////////////////广播控制器通讯串口//////////////////////////
class QTimer;
class QextSerialPort;
class BroadcastManager : public QObject
{
    Q_OBJECT


    QTimer* trainStateTimer;
    QTimer* lineInfoTimer;
    QThread* pthread;
    QextSerialPort *broadcastPort;

    CallStateParser callState;
signals:
    void readyRead();

private slots:
    void on_readyRead();
    void sendTrainState();
    void sendLineInfo();

public:
    int trainId;
    int carId;
    float speed;

    QString startStationEN;
    QString startStationThai;
    QString endStationEN;
    QString endStationThai;
    BroadcastManager(const QString &serialName);
    ~BroadcastManager();
    const CallStateParser& getOneCallState(){return callState;}
    void sendBroadcastPkg(BasicParser& parser);

};

#endif // SERIALTHREAD_H
