#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtGui>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QPersistentModelIndex>
#include <QModelIndex>
#include <QTreeView>
#include "unpack.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();




private slots:

    void displayInfo();
    void displaySpec();
    void displayList();
    void reloadList();

    void on_pushButton_clicked();               //网络解包

    void on_pushButton_2_clicked();             //网络清空

    void on_pushButton_3_clicked();             //蹲位解包

    void on_pushButton_4_clicked();             //蹲位清空

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();



private:
    Ui::MainWindow *ui;
    unpack packet;


    void addQcombobox();
    void createActions();
    void initStyle();
};

#endif // MAINWINDOW_H
