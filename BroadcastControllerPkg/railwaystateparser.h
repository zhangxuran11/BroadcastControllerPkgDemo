#ifndef RAILWAYSTATEPARSER_H
#define RAILWAYSTATEPARSER_H
#include"basicparser.h"
#include<QString>
//线路状态解析器
class RailwayStateParser :public BasicParser
{
private:
    quint8 msgHead; //包起始符
    quint8 msgLen;  //包长度  固定长度132byte
    quint8 msgType; //包类型  固定为0x82

    quint8 fcs;//累加和
    void generate();

//    friend class BroadcastManager;
public:
    RailwayStateParser();
    QString startStationEN;  //始发站英文版
    QString startStationThai;  //始发站泰文版
    QString endStationEN;  //终点站英文版
    QString endStationThai;  //终点站泰文版

    void print();


};

#endif // RAILWAYSTATEPARSER_H
