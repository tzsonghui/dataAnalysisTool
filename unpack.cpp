#include "unpack.h"

unpack::unpack()
{

}


unsigned char unpack::getverify(unsigned char * data,int count)
{
    unsigned char verify=0x00;
    int i;
    for(i=0;i<count;i++)
    {
        verify=verify^data[i];
    }
    return verify;
}

int unpack::writeDate(QString &destDate,const QString &sourDate,int len)
{
    int bitLen = 0;
    bitLen = sourDate.size();
    //QTextStream cout(stdout,  QIODevice::WriteOnly);
    //cout << bitLen << endl;
    if( bitLen < len)
    {
        return -1;
    }
    destDate.append(sourDate.mid(0,len));
    destDate.append("\n");
    return 1;
}


void unpack::writeLevel(int level)
{
    sendDate.append("\\\\");
    sendDate.append(QString::number(level));
    sendDate.append("\n");

    return;
}

int unpack::qstringToUnChar(const QString &changeDate,unsigned char *bits)
{
    unsigned char i;
    bool ok;
    i = (unsigned char)changeDate.toInt(&ok,16);
    qDebug()<<ok;
    if(ok != true)
    {
        return -4;
    }
    QTextStream cout(stdout,  QIODevice::WriteOnly);
    cout << i << endl;
    *bits = i;
    return 1;
}


int unpack::getBitMap(const QString &recvDate)
{

    int i,len =0,ret=0;
    QString changeDate,oneBit;

    len =recvDate.size();
    QTextStream cout(stdout,  QIODevice::WriteOnly);
    cout << len << endl;
    if( len < 23)
    {
        return -1;
    }
    changeDate =recvDate.mid(0,24);
    qDebug()<<changeDate;
    for(i=0;i<8;i++)
    {
        oneBit = changeDate.mid(i*3,2);
        ret=qstringToUnChar(oneBit,&bitmap[i]);
        if(ret != 1)
        {
            qDebug()<<"位图转换出错";
            return ret;
        }
    }

    return 1;
}

int unpack::copyDate(const QString &origenDate,int len)
{
    int bitLen=0;
    bitLen =origenDate.size();
    QTextStream cout(stdout,  QIODevice::WriteOnly);
    //cout << bitLen << endl;
    if( bitLen < len)
    {
        return -2;
    }
    sendDate.append(origenDate.mid(0,len));
    sendDate.append("\n");
    return 1;
}

int unpack::unpackDate(const QString &recv)
{
    int ret=0;
    int nChar,nBit,i=0,j=0,sta=24;
    QString date;

    ret = getBitMap(recv);
    if(ret != 1)
    {
       return ret;
    }

    for(i = 0; i < 64; i++)
    {
        nChar = i / 8;
        nBit = i % 8;
        if ((0x01 << (7-nBit))&( bitmap[nChar]))
        {
            if(i==0)                               //兼容有的位图中包含位图位
            {
                sta=0;
            }
            date.clear();
            writeLevel(i);
            date=recv.mid(sta+j);
            if( netProtocol =="SmartBike新平台通信协议" )
            {
                ret=copyDate(date,PTPACK_OFFSET[i][1]*3);
                j += PTPACK_OFFSET[i][1]*3;
            }
            else
            {
                ret=copyDate(date,PTPACK_OFFSET2[i][1]*3);
                j += PTPACK_OFFSET2[i][1]*3;
            }
            if(ret != 1)
            {
               return ret;
            }
        }
    }
    return ret;
}

int unpack:: interfaceOnline(const QString &packet, QString &display)
{
    int ret=0;
    memset(bitmap,0x00,8);
    sendDate.clear();

    ret = unpackDate(packet);

    if(ret == 1)
    {
        qDebug() << "unpack success";
        display = sendDate;
        return ret;
    }
    else if(ret == -1)
    {
        qDebug() << "bitmap lose";
        return ret;
    }
    else if(ret == -2)
    {
        qDebug() << "date lose";
        display = sendDate;
        return ret;
    }
    else
    {
        qDebug() << "other error";
        return ret;
    }

    return ret;
}




int unpack::isLength(QString &date)
{
    if( date.length() < 12 )
    {
        return 0;
    }
    if( date.length() == 12 )
    {
        return -1;
    }
    if( date.length() <= 18)
    {
        return -2;
    }
    return 1;

}

int unpack::findProtocolHead(QString &date)
{
    int ret = 0;
    int i   = 0;
    QString tmp;

    //if ((ret = isLength(date)) != 1)
    //    return ret;

    ret = 0;
    for(i=0;i< date.length();i++)
    {
        if( (date.mid(i*2,2) == "bb" || date.mid(i*2,2) == "BB") && (date.mid((i+3)*2,2) == "cc" || date.mid((i+3)*2,2) == "CC")  )
        {
            if( i == 2)
            {
                //qDebug()<<"1";
                return 1;
            }
            else
            {
                 //qDebug()<<"2";
                tmp.clear();
                tmp = date.mid((i-2)*2);
               // qDebug()<<tmp;
                date.clear();
                date = tmp;
                return 1;
            }

        }
        else
        {
             //qDebug()<<"3";
            ret = 0;
        }
    }
    for(i=0;i< date.length();i++)
    {
        if( (date.mid(i*2,2) == "00") && (date.mid((i+2)*2,2) == "aa" || date.mid((i+2)*2,2) == "AA")  )
        {
            if( i == 2)
            {
                //qDebug()<<"1";
                return 2;
            }
            else
            {
                 //qDebug()<<"2";
                tmp.clear();
                tmp = date.mid((i-2)*2);
               // qDebug()<<tmp;
                date.clear();
                date = tmp;
                return 2;
            }
        }
        else
        {
             //qDebug()<<"3";
            ret = 0;
        }
    }
    return ret;
}



#if 1
int unpack::cardReaderUnpack_field(QList<cardReader_field_packet>& fieldList,int i,QString &date)
{
    QString wr;
    QString inverted;
    QString tmpDate;
    int tmp = 0,j = 0,k = 0;

    wr.clear();
    wr.append(fieldList[i].get_fieldExplain());
    wr.append(" : ");

    if( fieldList[i].get_dateType() == "hex")
    {
        if( fieldList[i].get_dateSequence() == "normal")
        {
            tmp = date.toInt(0,16);
            wr.append( QString::number(tmp));
        }
        else
        {
            inverted.clear();
            for(j =0 ; j< date.size() ; j+=2 )
            {
                inverted.append(date.mid(date.size()-j-2,2));
            }
            tmp = inverted.toInt(0,16);
            wr.append( QString::number(tmp));

        }
    }
    else
    {
        if( fieldList[i].get_dateSequence() == "normal")
        {
            wr.append(date.mid(0));
        }
        else
        {
            inverted.clear();
            for(j =0 ; j< date.size() ; j+=2 )
            {
                inverted.append(date.mid(date.size()-j-2,2));
            }
             wr.append(date.mid(0));
        }
    }

    tmpDate.clear();
    tmpDate.append("                            //");
    for(k=0;k<date.size();k+=2)
    {
        tmpDate.append(date.mid(k,2));
        tmpDate.append(" ");
    }
    wr.append(tmpDate);
    writeDate(sendDate2,wr,wr.size());
    //writeDate(sendDate2,tmpDate,tmpDate.size());
    return 1;
}
#endif



int unpack::cardReaderUnpack(QList<cardReader_cmd_packet>& cmdList, QString &date,int type)
{
    int num = -1,i = 0;
    int ret = 0;
    QString buff;
    QString cmd;
    QString field;

    if( 1 == type)
    {
        if((ret = isLength(date)) != 1)
            return ret;
    }
    else if( 2 == type )
    {
        if(date.length() <= 10 )
            return -2;
    }

    ret = 0;

    buff.clear();
    if( 1 == type)
    {
        buff = date.mid(16,2);
        buff += "--recv";
    }
    else if( 2 == type )
    {
        buff = date.mid(8,2);
        buff += "--send";
    }
    num = configOpt.getCmdList(cmdList,buff);
    if(num == -1)
    {
        return -4;
    }

    cmd.clear();
    cmd.append("=^=^======   0x");
    cmd.append(buff.mid(0));
    cmd.append("   ======^=^=");
    writeDate(sendDate2,cmd,cmd.size());

    buff.clear();
    buff = date.mid(18);
    if( 1 == type)
    {
        buff = date.mid(18);
    }
    else if( 2 == type )
    {
        buff = date.mid(10);
    }
    qDebug() << cmdList[num].fieldList.size() ;

    for(i=0;i<cmdList[num].fieldList.size();i++ )
    {
        if( buff.size() >= (cmdList[num].fieldList[cmdList[num].fieldList.size()-1-i].get_dateLength())*2  )
        {
            qDebug() << i << cmdList[num].fieldList[cmdList[num].fieldList.size()-1-i].get_fieldName();

            field.clear();
            field = buff.mid(0,(cmdList[num].fieldList[cmdList[num].fieldList.size()-1-i].get_dateLength())*2);
            cardReaderUnpack_field(cmdList[num].fieldList,cmdList[num].fieldList.size()-1-i,field);

            field.clear();
            field = buff.mid((cmdList[num].fieldList[cmdList[num].fieldList.size()-1-i].get_dateLength())*2);
            buff.clear();
            buff = field;

        }
        else
        {
            return -2;
        }

    }

    return 1;

}


int unpack::cardReaderDateHandler(QString &date,const QString cityName)
{
    int ret = 0;
    int num = -1;
    QString buff;

    num = configOpt.getCityList(configOpt.protocolList,cityName);
    if(num == -1)
    {
        return -5;
    }

    buff.clear();
    buff = date;

    ret = findProtocolHead(buff);
    if(ret == 1)
    {
        qDebug()<<buff;
        ret = cardReaderUnpack(configOpt.protocolList[num].cmdList,buff,1);
    }
    else if(ret == 2)
    {
        qDebug()<<buff;
        ret = cardReaderUnpack(configOpt.protocolList[num].cmdList,buff,2);
    }

    return ret;
}


int unpack::interfaceCardReader(const QString &packet,QString &display,const QString cityName)
{
    // ret: 1/ok 0/数据协议头不完整 长度<6  -1/无标准数据或没有拷贝标准数据 -2/标准数据不够
    //      -3/此指令协议不对或数据不对 -4/没有此指令协议 -5/没有城市协议 -6/工控发送的数据不完整
    int ret = 0;
    QString packDate ;

    sendDate2.clear();
    packDate.clear();
    packDate = packet;
    packDate.remove(QRegExp("\\s"));
    qDebug()<<packDate.length();

    ret = cardReaderDateHandler(packDate,cityName);
    if(ret == 1)
    {
        qDebug() << "unpack success";
        display = sendDate2;
        return ret;
    }
    else if( ret == 0 )
    {
        qDebug() << "date imcomplete";
        return ret;
    }
    else if(ret == -1)
    {
        qDebug() << "without normative date or dont copy";
        return ret;
    }
    else if(ret == -2)
    {
        qDebug() << "normative date imcomplete";
        display = sendDate2;
        return ret;
    }
    else if( ret == -3)
    {
        qDebug() << "cmd protocol wrong or date wrong";
        display = sendDate2;
        return ret;
    }
    else if(ret == -4)
    {
        qDebug() << "no protocol of the cmd";
        return ret;
    }
    else if(ret == -5)
    {
        qDebug() << "no protocol of the city";
        return ret;
    }

    return ret;
}


int unpack::xorValue(const QString &Date,QString &result)
{
    int i = 0;
    char s[3]={0};
    QString packDate;
    QString resultDate;

    unsigned char verify=0x00;

    packDate.clear();
    packDate = Date;

    for(i=0;i<packDate.length();i+=2)
    {
        verify=verify^((unsigned char)(packDate.mid(i,2).toInt(0,16)));
    }
    qDebug()<<"xor"<<verify;

    sprintf(s,"%.2x",verify);
    resultDate.clear();
    resultDate = QString(QLatin1String(s));
    qDebug()<<"xor"<<resultDate;
    result = resultDate;

    return 1;
}



int unpack::crcValue(const QString &Date,QString &result)
{
    unsigned int  IX, IY, CRC;
    char s[5]={0};
    unsigned char high = 0,low = 0;
    unsigned int Len;
    QString packDate;
    QString resultDate;

    packDate.clear();
    packDate = Date;
    Len = packDate.length()/2;

    CRC = 0xFFFF;     //set all 1
    if ( Len <= 0 )
    {
        CRC = 0;
    }
    else
    {
        Len--;
        for ( IX = 0; IX <= Len; IX++ )
        {
            CRC = CRC ^ ( unsigned int )( packDate.mid(IX*2,2).toInt(0,16));
            for( IY = 0; IY <= 7; IY++ )
            {
                if ( ( CRC & 1 ) != 0 )
                {
                    CRC = ( CRC >> 1 ) ^ 0xA001;
                }
                else
                {
                    CRC = CRC >> 1;
                }
            }
        }
    }

    high = ( CRC & 0xff00 ) >> 8; //高位置
    low = ( CRC & 0x00ff ); //低位置
    qDebug()<<"crc"<<high<<low;

    sprintf(s,"%.2x %.2x",high,low);

    resultDate.clear();
    resultDate = QString(QLatin1String(s));
    qDebug()<<"crc"<<resultDate;
    result = resultDate;

    return 1;
}



int unpack::interfaceOther(const QString &packet,QString &displayLen,QString &displayXOR,QString &displayCRC)
{
    int len = 0;
    QString packDate ;

    packDate.clear();
    packDate = packet;
    packDate.remove(QRegExp("\\s"));
    len = packDate.length()/2;
    qDebug()<<packDate.length()/2;

    displayLen = QString::number(len);
    xorValue(packDate,displayXOR);
    crcValue(packDate,displayCRC);

    return 1;
}



