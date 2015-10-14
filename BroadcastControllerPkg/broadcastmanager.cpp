#include "broadcastmanager.h"
#include"trainstateparser.h"
#include"railwaystateparser.h"
#include"callstateparser.h"
#include"qextserialport.h"
#include<QThread>
#include<iostream>
#include<stdio.h>
#include<QTimer>
#include<QDebug>

using namespace std;
static void QextSerialPort_init(QextSerialPort *port)
{
    //port->setDtr();
    port->setBaudRate(BAUD19200);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_ODD);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);
    port->setTimeout(500);

}
void BroadcastManager::sendTrainState()
{
    TrainStateParser parser;
    parser.carID = carId;
    parser.trainID = trainId;
    parser.speed = speed;
    sendBroadcastPkg(parser);
}
void BroadcastManager::sendLineInfo()
{
    RailwayStateParser parser;
    parser.startStationEN = startStationEN;
    parser.startStationThai = startStationThai;
    parser.endStationEN = endStationEN;
    parser.endStationThai = endStationThai;
    sendBroadcastPkg(parser);
}
BroadcastManager::BroadcastManager(const QString &serialName)
{
    trainId = 0;
    carId = 0;
    speed = 0;

    pthread = new QThread;
    pthread->start();
    trainStateTimer = new QTimer;
    lineInfoTimer = new QTimer;
    trainStateTimer->moveToThread(pthread);
    lineInfoTimer->moveToThread(pthread);
    trainStateTimer->setInterval(1000);
    lineInfoTimer->setInterval(5000);
    connect(trainStateTimer,SIGNAL(timeout()),this,SLOT(sendTrainState()),Qt::DirectConnection);
    connect(lineInfoTimer,SIGNAL(timeout()),this,SLOT(sendLineInfo()),Qt::DirectConnection);

    broadcastPort = new QextSerialPort(serialName,QextSerialPort::EventDriven);
    QextSerialPort_init(broadcastPort);
    connect(broadcastPort,SIGNAL(readyRead()),this,SLOT(on_readyRead()),Qt::DirectConnection);


    if (broadcastPort->open(QIODevice::ReadWrite) == true) {
        cout<<"listening for data on "<<broadcastPort->portName().toStdString()<<endl;
    }
    else {
        cout<<"device failed to open: "<<broadcastPort->errorString().toStdString() <<endl;
        return;
    }


}

void BroadcastManager::sendBroadcastPkg(BasicParser& parser)
{

    parser.generate();
    qint64  writeCount= 0,writeLen = parser.FRAME_SIZE;
    do
    {
        writeCount = broadcastPort->write(parser.getData() + parser.FRAME_SIZE - writeLen, writeLen);
//        qDebug("Car send %lldbytes\n",writeCount);
         if(writeCount == -1)
             break;
         writeLen -= writeCount;
    }while(writeLen > 0);
}
enum state_flag{INIT,WAIT_HEAD,WAIT_LEN,COMPLETE};
void BroadcastManager::on_readyRead()
{

    qDebug("recv data on ttymxc2...\n");
    static state_flag state = INIT;
    static int start_pos = 0;
    static QByteArray _bytes;
    _bytes.append(broadcastPort->readAll());
    if(state == INIT)
    {
//        qDebug("INIT");
        if(_bytes.length() >= 8)
        {
            state = WAIT_HEAD;
        }
    }
    if(state == WAIT_HEAD)
    {
//        qDebug("WAIT_HEAD");
        while(start_pos<_bytes.length() &&(quint8)_bytes.data()[start_pos] != 0xf5)
        {
            start_pos++;
        }
        if(start_pos < _bytes.length())
        {
            state = WAIT_LEN;
        }
    }
    if(state == WAIT_LEN)
    {
//        qDebug("WAIT_LEN");
        if(_bytes.length() - start_pos >= 8)
        {
            state = COMPLETE;
        }
    }
    if(state == COMPLETE)
    {
//        qDebug("COMPLETE");
        QByteArray bytes(QByteArray(_bytes.mid(start_pos,8)));
        _bytes.remove(0,start_pos+8);
        state = INIT;
        start_pos = 0;
        if(8 == (quint8)bytes.data()[1] && 0x80 == (quint8)bytes.data()[2])
        {
            callState.load(QByteArray(bytes.data(),8));
            callState.print();
//            qDebug()<<"callState  isValid :"<<callState.isValid;
            if(callState.isValid)
            {
                emit readyRead();
            }
        }
    }
//    qDebug()<<"------------------------------------------------------in the end state = "<<state;
}
BroadcastManager::~BroadcastManager()
{
    delete trainStateTimer;
    delete lineInfoTimer;
    delete pthread;
    delete broadcastPort;
}
