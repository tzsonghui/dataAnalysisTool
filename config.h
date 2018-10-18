#ifndef CONFIG_H
#define CONFIG_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QtGui>
#include <qxmlstream.h>
#include <iostream>
#include <QDebug>
#include <QList>


class cardReader_field_packet
{
public:
    cardReader_field_packet()
    {
    }
    QString get_fieldName(void)
    {
        return fieldName;
    }

    void save_fieldName(const QString name)
    {
       fieldName = name;
       return ;
    }

    int get_dateLength(void)
    {
        return dateLength;
    }

    void save_dateLength(const int len)
    {
       dateLength = len;
       return ;
    }

    QString get_dateType(void)
    {
        return dateType;
    }

    void save_dateType(const QString type)
    {
       dateType = type;
       return ;
    }

    QString get_fieldExplain(void)
    {
        return fieldExplain;
    }

    void save_fieldExplain(const QString explain)
    {
       fieldExplain = explain;
       return ;
    }

    QString get_dateSequence(void)
    {
        return dateSequence;
    }

    void save_dateSequence(const QString seq)
    {
       dateSequence = seq;
       return ;
    }
private:
    QString         fieldName;
    int             dateLength;
    QString         dateType;
    QString         fieldExplain;
    QString         dateSequence;
};

class cardReader_cmd_packet
{

public:
    QList<cardReader_field_packet> fieldList;

    cardReader_cmd_packet()
    {
    }
    QString get_cmdName(void)
    {
        return cmdName;
    }

    void save_cmdName(const QString name)
    {
       cmdName = name;
       return ;
    }

    int get_fieldAmount(void)
    {
        return fieldAmount;
    }

    void save_fieldAmount(const int len)
    {
       fieldAmount = len;
       return ;
    }

private:
    QString     cmdName;
    int         fieldAmount;

};

class cardReader_city_packet
{

public:
    cardReader_city_packet()
    {
    }
    QList<cardReader_cmd_packet> cmdList;

    QString get_cityName(void)
    {
        return cityName;
    }

    void save_cityName(const QString name)
    {
       cityName = name;
       return ;
    }

    int get_cmdAmount(void)
    {
        return cmdAmount;
    }

    void save_cmdAmount(const int len)
    {
       cmdAmount = len;
       return ;
    }

private:
    QString     cityName;
    int         cmdAmount;

};



class config
{
public:
    config();
    QList<cardReader_city_packet> protocolList;

    int load_cardReader_list(const QString &fileName);

    int getFieldList(QList<cardReader_field_packet>& fieldList,const QString fieldName);
    int getCmdList(QList<cardReader_cmd_packet>& cmdList,const QString cmdName);
    int getCityList(QList<cardReader_city_packet>& cityList,const QString cityName);

private:
    QXmlStreamReader    reader;


    int addFieldList(QList<cardReader_field_packet>& fieldList);
    int addCmdList(QList<cardReader_cmd_packet>& cmdList);
    int addCityList(QList<cardReader_city_packet>& cityList);

    void loadFieldElement(QList<cardReader_field_packet>& fieldList, const QString fieldName);
    void loadCmdElement(QList<cardReader_cmd_packet>& cmdList, const QString cmdName);
    void loadCityElement(QList<cardReader_city_packet>& cityList, const QString cityName);
    void loadProtocol(QList<cardReader_city_packet>& cityList);


    void skipUnknowElement();       //跳过不认识的元素


};

#endif // CONFIG_H
