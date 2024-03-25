#ifndef CHATWIN_H
#define CHATWIN_H

#include <QDialog>
#include <QTcpSocket>
#include "maininfo.h"
#include "QDebug"
#include <QThread>
#include "QFileDialog"
#include "QMessageBox"
namespace Ui {
class chatwin;
}

class chatwin : public QDialog
{
    Q_OBJECT

public:
    explicit chatwin(QWidget *parent = 0);
    ~chatwin();
    void CreateChat(QString,QString,QString);
    void FileRev(QTcpSocket*);


private slots:
    void on_pushButton_clicked();

    void Connected();
    void Disconnected();
    void ReadyRead();

    void on_pushButton_2_clicked();

private:
    Ui::chatwin *ui;
    QString    LocalUser;
    QString    ChatUser;
    QString    ChatType; //P2P MulP

    QTcpSocket *Sub_Socket;
    QString     ConnectTime;
    bool        FileF;
    QByteArray  FileB;//缓存
    QFile*      FileS;//文件
    qint64      FileH=0;//进度
    quint64     FileRSize=0;//长度

};

#endif // CHATWIN_H
