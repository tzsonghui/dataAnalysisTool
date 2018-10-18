#ifndef UNPACK_H
#define UNPACK_H
#include <QMainWindow>
#include <stdio.h>
#include <qdebug.h>
#include <QTextStream>
#include <iostream>
#include "config.h"

//QTextStream cin(stdin, QIODevice::ReadOnly);
//QTextStream cout(stdout, QIODevice::WriteOnly);
//QTextStream cerr(stderr, QIODevice::WriteOnly);

const int PTPACK_OFFSET [][2]=
{
    {0,8},                      //0域偏移量
    {8,6},                      //1域偏移量
    {14,8},                     //2域偏移量
    {22,16},					//3域偏移量
    {38,20},					//4域偏移量
    {58,20},					//5域偏移量
    {78,5},                     //6域偏移量
    {83,1},                     //7域偏移量
    {84,19},					//8域偏移量
    {103,8},				  	//9域偏移量
    {111,12},					//10域偏移量
    {123,12},					//11域偏移量
    {135,12},					//12域偏移量
    {147,1},					//13域偏移量
    {148,24},					//14域偏移量
    {172,1},					//15域偏移量
    {173,12},					//16域偏移量
    {185,20},					//17域偏移量
    {205,19},					//18域偏移量
    {224,8},					//19域偏移量
    {232,19},					//20域偏移量
    {251,4},					//21域偏移量
    {255,12},					//22域偏移量
    {267,16},					//23域偏移量
    {283,16},					//24域偏移量
    {299,20},					//25域偏移量
    {319,16},					//26域偏移量
    {335,16},					//27域偏移量
    {351,37},					//28域偏移量
    {388,104},					//29域偏移量
    {492,3},					//30域偏移量
    {495,10},					//31域偏移量
    {505,12},					//32域偏移量
    {517,1},					//33域偏移量
    {518,1},					//34域偏移量
    {519,1},					//35域偏移量
    {520,16},					//36域偏移量
    {536,16},					//37域偏移量
    {552,16},					//38域偏移量
    {568,32},					//39域偏移量
    {600,32},					//40域偏移量`
    {632,32},					//41域偏移量
    {664,64},					//42域偏移量
    {728,64},					//43域偏移量
    {792,64},					//44域偏移量
    {856,127},					//45域偏移量
    {983,255},					//46域偏移量
    {1238,511},					//47域偏移量
    {1749,256},					//48域偏移量
    {2005,1024},				//49域偏移量
    {3029,64},					//50域偏移量
    {3093,64},					//51域偏移量
    {3157,64},					//52域偏移量
    {3221,64},					//53域偏移量
    {3285,64},					//54域偏移量
    {3349,640}					//55域偏移量

};


const int PTPACK_OFFSET2 [][2]=
{
    {0,8},                      //0域偏移量
    {8,6},                      //1域偏移量
    {14,8},                     //2域偏移量
    {22,16},					//3域偏移量
    {38,4},                     //4域偏移量
    {42,20},					//5域偏移量
    {62,5},                     //6域偏移量
    {67,1},                     //7域偏移量
    {68,19},					//8域偏移量
    {87,8},                     //9域偏移量
    {95,12},					//10域偏移量
    {107,12},					//11域偏移量
    {119,12},					//12域偏移量
    {131,1},					//13域偏移量
    {132,8},					//14域偏移量
    {140,1},					//15域偏移量
    {141,12},					//16域偏移量
    {153,12},					//17域偏移量
    {165,8},					//18域偏移量
    {173,8},					//19域偏移量
    {181,8},					//20域偏移量
    {189,4},					//21域偏移量
    {193,12},					//22域偏移量
    {205,16},					//23域偏移量
    {221,16},					//24域偏移量
    {237,20},					//25域偏移量
    {257,16},					//26域偏移量
    {273,16},					//27域偏移量
    {289,37},					//28域偏移量
    {326,104},					//29域偏移量
    {430,3},					//30域偏移量
    {433,10},					//31域偏移量
    {443,12},					//32域偏移量
    {455,1},					//33域偏移量
    {456,1},					//34域偏移量
    {457,1},					//35域偏移量
    {458,16},					//36域偏移量
    {474,16},					//37域偏移量
    {490,16},					//38域偏移量
    {506,32},					//39域偏移量
    {538,32},					//40域偏移量
    {570,32},					//41域偏移量
    {602,64},					//42域偏移量
    {666,64},					//43域偏移量
    {730,64},					//44域偏移量
    {794,127},					//45域偏移量
    {921,255},					//46域偏移量
    {1176,511},                 //47域偏移量
    {1687,1023}                 //48域偏移量

};


class unpack
{
public:
    unpack();
    config configOpt;
    QString netProtocol;
    int interfaceOnline(const QString &packet,QString &display);
    int interfaceCardReader(const QString &packet,QString &display,const QString cityName);
    int interfaceOther(const QString &packet,QString &displayLen,QString &displayXOR,QString &displayCRC);

private:
    unsigned char bitmap[8];
    QString sendDate;
    QString sendDate2;



    int writeDate(QString &destDate,const QString &sourDate,int len);

    // online
    unsigned char getverify(unsigned char * data,int count);
    int unpackDate(const QString &recv);
    int getBitMap(const QString &recvDate);
    int qstringToUnChar(const QString &changeDate,unsigned char *bits);
    int copyDate(const QString &origenDate,int len);
    void writeLevel(int level);

    //cardreader
    int isLength(QString &date);
    int cardReaderDateHandler(QString &date,const QString cityName);
    int findProtocolHead(QString &date);
    int cardReaderUnpack(QList<cardReader_cmd_packet>& cmdList, QString &date, int type);
    int cardReaderUnpack_field(QList<cardReader_field_packet>& fieldList,int i,QString &date);

    //other
    int xorValue(const QString &Date,QString &result);
    int crcValue(const QString &Date,QString &result);

};

#endif // UNPACK_H


