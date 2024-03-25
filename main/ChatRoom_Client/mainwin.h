#ifndef MAINWIN_H
#define MAINWIN_H

#include <QDialog>
#include "qdebug.h"
#include <QTcpSocket>
#include "maininfo.h"
#include "QMessageBox"
#include <QStringListModel>
#include <QThread>
#include <QMainWindow>
#include "chatwin.h"

namespace Ui {
class mainwin;
}

class mainwin : public QDialog
{
    Q_OBJECT

public:
    explicit mainwin(QWidget *parent = 0);
    ~mainwin();
    void Start(QString);//从main传来的名字 并显示在界面上 并初始化socket





private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void Connected();
    void Disconnected();
    void ReadyRead();

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_listView_2_doubleClicked(const QModelIndex &index);

private:
    Ui::mainwin *ui;
    QString UserName;
    QStringListModel *UserListView;
    QStringListModel *GroupListView;


    QTcpSocket *Main_Socket;
    chatwin *sub;

};

#endif // MAINWIN_H
