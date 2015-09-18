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


    QextSerialPort *broadcastPort;

    CallStateParser callState;
signals:
    void readyRead();
public:
    BroadcastManager(const QString &serialName);
    const CallStateParser& getOneCallState(){return callState;}
    void sendBroadcastPkg(BasicParser& parser);
private slots:
    void on_readyRead();

};

#endif // SERIALTHREAD_H
