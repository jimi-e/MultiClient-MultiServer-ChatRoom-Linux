#ifndef SERVERINFO_H
#define SERVERINFO_H

#include "qdebug.h"
#include <QDateTime>
#include <QDataStream>
#include <QVector>


class serverinfo
{
public:

    QString UserName;
    QString REVerName;
    QString InfoType;
    QString REQTime;
    QString Content;
    qint64  FileSize;

    QByteArray SendBuf;
    QByteArray RcvBuf;

    QVector<QString> SendVector;



    void Trans_RcvBuf();
    void Trans_SendBuf();

    void SendVec(QString, QVector<QString>);

private:


};

#endif // SERVERINFO_H
