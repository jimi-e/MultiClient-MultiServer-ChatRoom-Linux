#ifndef MAININFO_H
#define MAININFO_H

#include "qdebug.h"
#include <QDateTime>
#include <QDataStream>
#include <QVector>

class maininfo
{
public:
    QString UserName;
    QString REVerName;
    QString InfoType; //MainWin UserList
    QString REQTime;
    QString Content;
    qint64  FileSize;

    QByteArray  SendBuf;
    QByteArray  RcvBuf;

    QVector<QString> RevVec;



    void Trans_SendBuf();
    void Trans_RcvBuf();

    void mainwinREQ(QString);
    void GroupCreate(QString,QString);
    void ChatConnectREQ(QString,QString,QString);
    void SendMsg(QString,QString,QString,QString);
    void SendFile(qint64, QString, QString, QString);

private:


};

#endif // MAININFO_H
