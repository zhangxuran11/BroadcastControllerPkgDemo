#ifndef BASICPARSER_H
#define BASICPARSER_H
#include<QtGlobal>
#include<QByteArray>
#include"basicparser.h"
//呼叫状态解析器
class BasicParser
{

    friend class BroadcastManager;
protected:
    quint8 FRAME_SIZE;
    QByteArray dataFram;
    virtual quint8 genarateCheckSum() = 0;
    virtual void generate() = 0;
public:
    explicit BasicParser(quint8 _FRAME_SIZE):FRAME_SIZE(_FRAME_SIZE){}
    const char* getData()const{return dataFram.data();}

    virtual void print() = 0;
};
#endif // BASICPARSER_H