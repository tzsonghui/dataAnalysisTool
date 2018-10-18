#include "config.h"

config::config()
{

}



void config::skipUnknowElement()
{
    reader.readNext();
    while (!reader.atEnd())
    {
        if (reader.isEndElement())
        {
            reader.readNext();
            break;
        }
        if (reader.isStartElement())
        {
            skipUnknowElement();
        } else
        {
            reader.readNext();
        }
    }
}


int config::addFieldList(QList<cardReader_field_packet>& fieldList)
{

    cardReader_field_packet *field = new cardReader_field_packet;
    fieldList.prepend(*field);
    return 0;

}


int config::getFieldList(QList<cardReader_field_packet>& fieldList,const QString fieldName)
{
    int i = -1;

    if( fieldList.size() <= 0)
    {
        return -1;
    }
    else
    {
        for(i=0;i<fieldList.size();i++)
        {
            if( fieldList[i].get_fieldName() == fieldName )
                return i;
        }
        return -1;
    }

}

int config::addCmdList(QList<cardReader_cmd_packet>& cmdList)
{

    cardReader_cmd_packet *cmd = new cardReader_cmd_packet;
    cmdList.prepend(*cmd);
    return 0;
}

int config::getCmdList(QList<cardReader_cmd_packet>& cmdList,const QString cmdName)
{
    int i = -1;

    if( cmdList.size() <= 0)
    {
        return -1;
    }
    else
    {
        for(i=0;i<cmdList.size();i++)
        {
            if( cmdList[i].get_cmdName() == cmdName )
                return i;
        }
        return -1;
    }

}


int config::addCityList(QList<cardReader_city_packet>& cityList)
{
    cardReader_city_packet *city = new cardReader_city_packet;
    cityList.prepend( *city );
    //qDebug() << "getCityList" <<cityList.size();
    return 0;
}

int config::getCityList(QList<cardReader_city_packet>& cityList,const QString cityName)
{
    int i = -1;

    if( cityList.size() <= 0)
    {
        return -1;
    }
    else
    {
        for(i=0;i<cityList.size();i++)
        {

            if( cityList[i].get_cityName() == cityName )
                return i;
        }
        return -1;
    }
}

void config::loadFieldElement(QList<cardReader_field_packet>& fieldList, const QString fieldName)
{
    int i = 0;
    QString str;

    i = getFieldList(fieldList,fieldName);
    if( i == -1)
    {
        i = addFieldList(fieldList);
    }

    fieldList[i].save_fieldName(reader.attributes().value("name").toString());

    reader.readNext();
    while (!reader.atEnd())
    {
        if (reader.isEndElement())
        {
            reader.readNext();
            break;
        }
        if (reader.isStartElement())
        {
            if (reader.name() == "len")
            {
                str.clear();
                str = reader.readElementText();
                fieldList[i].save_dateLength(str.toInt());
                 qDebug() << "        len: " << str.toInt();
                if( reader.isEndElement() )
                    reader.readNext();
            }
            else if(reader.name() == "type")
            {
                str.clear();
                str = reader.readElementText();
                fieldList[i].save_dateType(str);
                qDebug() << "        type: " << str;
                if( reader.isEndElement() )
                    reader.readNext();
            }
            else if(reader.name() == "explain")
            {
                str.clear();
                str = reader.readElementText();
                fieldList[i].save_fieldExplain(str);
                qDebug() << "        explain: " << str;
                if( reader.isEndElement() )
                    reader.readNext();
            }
            else if( reader.name() == "sequence" )
            {
                str.clear();
                str = reader.readElementText();
                fieldList[i].save_dateSequence(str);
                qDebug() << "        sequence: " << str;
                if( reader.isEndElement() )
                    reader.readNext();            }
            else
            {
                skipUnknowElement();
            }
        }
        else
        {
            reader.readNext();
        }
    }
    return;
}


void config::loadCmdElement(QList<cardReader_cmd_packet>& cmdList, const QString cmdName)
{
    int i = 0;

    i = getCmdList(cmdList,cmdName);
    if( i == -1)
    {
        i = addCmdList(cmdList);
    }

    cmdList[i].save_cmdName(reader.attributes().value("name").toString());

    reader.readNext();
    while( !reader.atEnd() )
    {
        if(reader.isEndElement())
        {
            reader.readNext();
            break;
        }

        if(reader.isStartElement())
        {
            if( reader.name() == "field" )
            {
                 qDebug() << "      [load]:now is field - "<<reader.attributes().value("name").toString();
                loadFieldElement(cmdList[i].fieldList,reader.attributes().value("name").toString());
            }
            else
                skipUnknowElement();
        }
        else
            reader.readNext();
    }
    return;

}


void config::loadCityElement( QList<cardReader_city_packet>& cityList,const QString cityName)
{
    int i = 0;

    i = getCityList(cityList,cityName);
    if( i == -1)
    {
        i = addCityList(cityList);
    }

    cityList[i].save_cityName(reader.attributes().value("name").toString());

    reader.readNext();
    while( !reader.atEnd() )
    {
        if(reader.isEndElement())
        {
            reader.readNext();
            break;
        }
        if(reader.isStartElement())
        {
            if( reader.name() == "cmd" )
            {
                 qDebug() << "    [load]:now is cmd - "<<reader.attributes().value("name").toString();
                loadCmdElement(cityList[i].cmdList,reader.attributes().value("name").toString());
            }
            else
                skipUnknowElement();
        }
        else
            reader.readNext();
    }

    return ;
}

void config::loadProtocol(QList<cardReader_city_packet>& cityList)
{
    reader.readNext();
    while (!reader.atEnd())
    {
        if (reader.isEndElement())
        {
            reader.readNext();
            break;
        }
        if (reader.isStartElement())
        {
            if(reader.name() == "city")
            {
                qDebug() << "  [load]:now is city - "<<reader.attributes().value("name").toString();

                loadCityElement(cityList,reader.attributes().value("name").toString());

            }
            else
            {
                skipUnknowElement();
            }
        }
        else
        {
            reader.readNext();
        }
    }
}


int config::load_cardReader_list(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        return false;
    }

    qDebug()<<QString("read success");
    reader.setDevice(&file);

    while (!reader.atEnd())
    {
        if (reader.isStartElement())
        {
            if(reader.name() == "protocol")
            {
                qDebug() << "[load]:now is protocol";
                loadProtocol(protocolList);
            }
            else
            {
                reader.raiseError(QObject::tr("Not a protocol file"));
            }
        }
        else
        {
            reader.readNext();
        }
    }
    file.close();

    return true;

}
