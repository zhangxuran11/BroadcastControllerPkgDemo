#include "broadcastmanager.h"
#include"trainstateparser.h"
#include"railwaystateparser.h"
#include"callstateparser.h"
#include"qextserialport.h"
#include<iostream>
#include<stdio.h>
#include<QTimer>
#include<QDebug>

using namespace std;
static void QextSerialPort_init(QextSerialPort *port)
{
    port->setDtr();
    port->setBaudRate(BAUD9600);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);
    port->setTimeout(1000);

}
BroadcastManager::BroadcastManager(const QString &serialName)
{


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

//RailwayStateParser parser;
//parser.startStationEN = "startStationEN";
//parser.endStationEN = "endStationEN";
//parser.startStationThai = "startStationThai";
//parser.endStationThai = "endStationThai";


//    TrainStateParser parser;
//    parser.carID = 8;
//    parser.trainID = 0x2030;
//    parser.speed = 0x33;
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

void BroadcastManager::on_readyRead()
{

//    qDebug("recv data on ttymxc2...\n");
    static QByteArray _bytes;
    QByteArray bytes;
    _bytes.append(broadcastPort->readAll());
//    qDebug()<<"_bytes len :"<<_bytes.length();
    if(_bytes.length() < 8)
        return;
    if(_bytes.length() >= 8)
    {
        bytes.append(_bytes.data(),8);
        _bytes.remove(0,8);
    }

//    qDebug()<<"bytes len :"<<bytes.length();
    int i = 0;
    while(i<bytes.length() &&(quint8)bytes.data()[i++] != 0xf5);
    if(i >= bytes.length())
    {
        qDebug()<<"data  invalid !!";
        return;
    }
    int start = i-1;
    if( callState.FRAME_SIZE == (quint8)bytes.data()[i++] && 0x80 == (quint8)bytes.data()[i++])
    {

        callState.load(QByteArray(bytes.data()+start,callState.FRAME_SIZE));
        qDebug()<<"callState  isValid :"<<callState.isValid;
        if(callState.isValid)
        {
            emit readyRead();

        }
        //printf("valid :%d\nCall1 %d\nCall2 %d\nCall3 %d\nCall4 %d\nBroadcast %d\n",parser.isValid,parser.hasCall1,parser.hasCall2,parser.hasCall3,parser.hasCall4,parser.hasBroadcast);

    }

}
