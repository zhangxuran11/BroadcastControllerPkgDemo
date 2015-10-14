#include "railwaystateparser.h"
#include<QDebug>
#include"ztools.h"
//线路状态解析器

//    quint8 msgHead; //包起始符
//    quint8 msgLen;  //包长度  固定长度132byte
//    quint8 msgType; //包类型  固定为0x82
//    QString startStationEN;  //始发站英文版
//    QString startStationThai;  //始发站泰文版
//    QString endStationEN;  //终点站英文版
//    QString endStationThai;  //终点站泰文版
//    quint8 fcs;//累加和
RailwayStateParser::RailwayStateParser():
    BasicParser(132)
{
    dataFram.resize(FRAME_SIZE);
    msgHead = 0xf5;
    msgLen = 132;
    msgType = 0x82;
}

void RailwayStateParser::generate()
{
    quint16 tmp = 0;
    dataFram.resize(FRAME_SIZE);
    dataFram.fill(0,FRAME_SIZE);
    memcpy(dataFram.data(),&msgHead,1);
    memcpy(dataFram.data()+1,&msgLen,1);
    memcpy(dataFram.data()+2,&msgType,1);
    tmp = startStationEN.toAscii().length();
    memcpy(dataFram.data()+3,&tmp,2);
    memcpy(dataFram.data()+5,startStationEN.toUtf8().data(),tmp);
    tmp = ZTools::str2unicode(startStationThai).length();
    memcpy(dataFram.data()+21,&tmp,2);
    memcpy(dataFram.data()+23,ZTools::str2unicode(startStationThai).data(),tmp);
    tmp = endStationEN.toAscii().length();
    memcpy(dataFram.data()+67,&tmp,2);
    memcpy(dataFram.data()+69,endStationEN.toUtf8().data(),tmp);
    tmp = ZTools::str2unicode(endStationThai).length();
    memcpy(dataFram.data()+85,&tmp,2);
    memcpy(dataFram.data()+87,ZTools::str2unicode(endStationThai).data(),tmp);
    fcs = genarateCheckSum();
    memcpy(dataFram.data()+131,&fcs,1);
//    for(int i= 0 ;i < dataFram.length();i++)
//        printf("%d--%x\n",i,dataFram.data()[i]);

}

void RailwayStateParser::print()
{
    qDebug()<<"startStationEN   : "<<startStationEN;
    qDebug()<<"startStationThai : "<<startStationThai;
    qDebug()<<"endStationEN     : "<<endStationEN;
    qDebug()<<"endStationThai   : "<<endStationThai;
    printRaw();
}
