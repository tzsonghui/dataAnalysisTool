#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("数据解析辅助工具");
    packet.configOpt.load_cardReader_list("cardReaderProtocol.xml");
    this->initStyle();
    this->addQcombobox();
    this->createActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}



//style
//----------------------------------------------------------------------------
void MainWindow::initStyle()
{

}



//menuBar
//----------------------------------------------------------------------------

void MainWindow::createActions()
{
    connect(ui->action_5, &QAction::triggered, this, &MainWindow::displaySpec);
    connect(ui->action_7, &QAction::triggered, this, &MainWindow::displayInfo);
    connect(ui->action_2, &QAction::triggered, this, &MainWindow::displayList);
    connect(ui->actionReload, &QAction::triggered, this, &MainWindow::reloadList);

}


void MainWindow::displayInfo()
{
    QMessageBox::about(NULL, "About", "数据解析辅助工具v0.2\nSH\n@copyleft...GPL");
}


void MainWindow::displaySpec()
{
    QMessageBox::about(NULL, "Illustration", "1.网络数据暂未改动,延续上版从位图开始粘贴数据\n\
2.控制盒数据协议在同级目录下:\n\
      \文件‘cardReaderProtocol.xml’\n\
\t1)协议暂时需要手动添加 参照即可\n\
\t2)各指令以城市区域划分\n\
\t3)指令协议(cmd)分为:\n\
\t      工控到墩位(--send)墩位到工控(--recv)\n\
\t4)field表示一字段\n\
\t      .)数据顺序项 正(normal)/逆(inverted)\n\
\t      .)数据类型项 hex/bcd(参照协议)\n\
\t      .)数据长度项 参照协议\n\
\t      .)类似交易记录区等可拆分为具体字段\n\
3.其他项 \n\
\t1)crc为ModBus CRC16 杭州等地...\n\
\t2)unix时间戳转换到本地(北京)时间\n\
4.其他待添加...");
}



void MainWindow::reloadList()
{
    for(int i=0;i<packet.configOpt.protocolList.size();i++)
    {
        for(int j=0;j<packet.configOpt.protocolList[i].cmdList.size();j++)
        {
            packet.configOpt.protocolList[i].cmdList[j].fieldList.clear();
            qDebug()<< "after release field"<<packet.configOpt.protocolList[i].cmdList[j].fieldList.size();
        }
        packet.configOpt.protocolList[i].cmdList.clear();
        qDebug()<< "after release cmd"<<packet.configOpt.protocolList[i].cmdList.size();
    }
    packet.configOpt.protocolList.clear();
    qDebug()<< "after release List"<<packet.configOpt.protocolList.size();

#if 1
    if( true == packet.configOpt.load_cardReader_list("cardReaderProtocol.xml"))
    {
        QMessageBox::information(NULL, "Success", "重新加载控制盒协议成功！", QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(NULL, "Fail", "重新加载控制盒协议失败！\n请确认文件是否存在或文件名是否正确", QMessageBox::Ok);
    }
#endif

}


void MainWindow::displayList()
{
    int i = 0,j = 0,k = 0;
    QString field;
    QTreeView *tree = new QTreeView();
    QStandardItemModel* model = new QStandardItemModel(tree);
    model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("协议")<<QStringLiteral("说明"));

    for(i=0;i<packet.configOpt.protocolList.size();i++)
    {
        QStandardItem* itemCity = new QStandardItem(packet.configOpt.protocolList[i].get_cityName());
        model->appendRow(itemCity);
        model->setItem(model->indexFromItem(itemCity).row(),1,new QStandardItem(QStringLiteral("区域/城市")));

        for(j=0;j<packet.configOpt.protocolList[i].cmdList.size();j++)
        {
            QStandardItem* itemCmd = new QStandardItem(packet.configOpt.protocolList[i].cmdList[j].get_cmdName());
            itemCity->appendRow(itemCmd);
            itemCity->setChild(itemCmd->index().row(),1,new QStandardItem(QStringLiteral("指令")));

            for(k=packet.configOpt.protocolList[i].cmdList[j].fieldList.size();k>0;k--)
            {
                field.clear();
                field +=" Len : ";
                field +=QString::number(packet.configOpt.protocolList[i].cmdList[j].fieldList[k-1].get_dateLength());
                if(packet.configOpt.protocolList[i].cmdList[j].fieldList[k-1].get_dateLength() <10)
                {
                    field +="  ";
                }
                field +="       Type : ";
                field +=packet.configOpt.protocolList[i].cmdList[j].fieldList[k-1].get_dateType();
                field +="         Sequence : ";
                field +=packet.configOpt.protocolList[i].cmdList[j].fieldList[k-1].get_dateSequence();
                QStandardItem* itemField = new QStandardItem(packet.configOpt.protocolList[i].cmdList[j].fieldList[k-1].get_fieldExplain());
                itemCmd->appendRow(itemField);
                itemCmd->setChild(itemField->index().row(),1,new QStandardItem(field));
            }
        }
    }
    tree->setModel(model);
    tree->show();
}

//----------------------------------------------------------------------------





void MainWindow::addQcombobox()
{
    int i = 0;
    for(i=0;i<packet.configOpt.protocolList.size();i++)
    {
        ui->comboBox->addItem(packet.configOpt.protocolList[i].get_cityName());
    }
}



void MainWindow::on_pushButton_clicked()
{
    int ret=0;
    QString buff;
    QString buff_show;

    packet.netProtocol.clear();
    packet.netProtocol = ui->comboBox_3->currentText();
    qDebug()<<"netPro"<<packet.netProtocol;

    buff = ui->plainTextEdit->toPlainText();
    QTextStream cout(stdout,  QIODevice::WriteOnly);
    cout << buff << endl;
    ret=packet.interfaceOnline(buff,buff_show);
    if(ret==1)
    {
        ui->textBrowser->append(buff_show);
        ui->textBrowser->append("解包完毕");
    }
    else if(ret == -2)
    {
        ui->textBrowser->append(buff_show);
        ui->textBrowser->append("数据不完整");
    }
    else if(ret == -1)
    {
        ui->textBrowser->append("bitmap不完整");
    }
    else
    {
        ui->textBrowser->append(buff);

        ui->textBrowser->append("解包出错/未知");
    }
    return;}

void MainWindow::on_pushButton_2_clicked()
{
    ui->plainTextEdit->clear();
    ui->textBrowser->clear();
    return;
}

void MainWindow::on_pushButton_3_clicked()
{

    int ret=0;
    QString buff;
    QString buff_show;
    QString cityName;

    cityName.clear();
    cityName = ui->comboBox->currentText();

    buff = ui->plainTextEdit_2->toPlainText();
    ret=packet.interfaceCardReader(buff,buff_show,cityName);
    if(ret == 1)
    {
        ui->textBrowser_2->append(buff_show);
        ui->textBrowser_2->append("unpack success");
    }
    else if( ret == 0 )
    {
        ui->textBrowser_2->append("fd date imcomplete len<6");
    }
    else if(ret == -1)
    {
        ui->textBrowser_2->append("without normative date or dont copy");
    }
    else if(ret == -2)
    {
        ui->textBrowser_2->append(buff_show);
        ui->textBrowser_2->append("normative date imcomplete");
    }
    else if( ret == -3)
    {
        ui->textBrowser_2->append(buff_show);
        ui->textBrowser_2->append("cmd protocol wrong or date wrong");
    }
    else if(ret == -4)
    {
        ui->textBrowser_2->append("no protocol of the cmd");
    }
    else if(ret == -5)
    {
        ui->textBrowser_2->append("no protocol of the city");
    }
    else
    {
        ui->textBrowser_2->append("unknown error");
    }

    //xml.readFile("cardReaderConfig.xml");
    //configopt.load_cardReader_list("cardReaderConfig.xml");
    //QString str2 = "中文";
    //qDebug() << str2.toStdString().c_str();
    //ui->textBrowser_2->append(str2);

}

void MainWindow::on_pushButton_4_clicked()
{
    ui->plainTextEdit_2->clear();
    ui->textBrowser_2->clear();
    return;
}

void MainWindow::on_pushButton_5_clicked()
{
    QString len;
    QString xorResult;
    QString crcResult;
    QString recv;

    recv.clear();
    recv = ui->plainTextEdit_3->toPlainText();
    packet.interfaceOther(recv,len,xorResult,crcResult);


    ui->lineEdit->setText(len);
    ui->lineEdit_2->setText(xorResult);
    ui->lineEdit_3->setText(crcResult);

    return ;
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->plainTextEdit_3->clear();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    return ;
}




void MainWindow::on_pushButton_7_clicked()
{
    QDateTime time;
    QString type;
    QString recv;
    QString result;
    int date = 0;

    type.clear();
    recv.clear();

    type = ui->comboBox_2->currentText();
    recv = ui->lineEdit_4->text();
    if( type == "十进制输入" )
    {
        date = recv.toInt(0,10);
    }
    else
    {
        date = recv.toInt(0,16);
    }
    qDebug()<< date ;

    time= QDateTime::fromTime_t(date);

    result =time.toString("yyyy-MM-dd hh:mm:ss");
    ui->lineEdit_5->setText(result);

}
