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
    port->setBaudRate(BAUD9600);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);
    //port->setTimeout(10);

}
void BroadcastManager::sendTrainState()
{
    qDebug("sendTrainState");
    TrainStateParser parser;
    parser.systemDateTime = parser.systemDateTime = QDateTime::currentDateTime();;
    parser.carID = carId;
    parser.trainID = trainId;
    parser.speed = speed;
    sendBroadcastPkg(parser);
    //parser.print();
}
void BroadcastManager::sendLineInfo()
{
    qDebug("sendLineInfo");
    static int stationType = 1;
    RailwayStateParser parser;
    parser.stationType = stationType;
    if(stationType == 1)
        parser.stationName = startStationEN;
    else if(stationType == 2)
        parser.stationName = endStationEN;
    else if(stationType == 3)
        parser.stationName = startStationThai;
    else if(stationType == 4)
        parser.stationName = endStationThai;
    stationType++;
    if(stationType == 5)
        stationType = 1;
    sendBroadcastPkg(parser);
    //parser.print();
}
BroadcastManager::BroadcastManager(const QString &serialName)
{
    start_f = false;

    trainId = 0;
    carId = 0;
    speed = 0;

    pthread = new QThread;
    pthread->start();
    trainStateTimer = new QTimer;
    lineInfoTimer = new QTimer;

    trainStateTimer->setInterval(1000);
    lineInfoTimer->setInterval(1000);
    trainStateTimer->start();
    lineInfoTimer->start();

    connect(trainStateTimer,SIGNAL(timeout()),this,SLOT(sendTrainState()),Qt::DirectConnection);
    connect(lineInfoTimer,SIGNAL(timeout()),this,SLOT(sendLineInfo()),Qt::DirectConnection);
    trainStateTimer->moveToThread(pthread);
    lineInfoTimer->moveToThread(pthread);

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
void BroadcastManager::start()
{
    start_f = true;
}
void BroadcastManager::sendBroadcastPkg(BasicParser& parser)
{
    parser.generate();
    if(!start_f)
        return;
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

//    qDebug("recv data on ttymxc2...\n");
    static state_flag state = INIT;
    static int start_pos = 0;
    static QByteArray _bytes;
    _bytes.append(broadcastPort->readAll());
label:
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
 //           callState.print();
//            qDebug()<<"callState  isValid :"<<callState.isValid;
            if(callState.isValid)
            {
                emit readyRead();
            }
        }
        goto label;
    }
//    qDebug()<<"------------------------------------------------------in the end state = "<<state;
}
BroadcastManager::~BroadcastManager()
{
    delete trainStateTimer;
    delete lineInfoTimer;
    pthread->exit();
	delete pthread;
    delete broadcastPort;
}
