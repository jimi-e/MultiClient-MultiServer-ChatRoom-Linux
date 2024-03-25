#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QDebug>

#include "serverinfo.h"
#include <QThread>

class Server : public QWidget
{
    Q_OBJECT
public:
    explicit Server(QWidget *parent = nullptr);
    void Serv_Init();
    void SendFList(QTcpSocket *);
    void SendGList(QTcpSocket *);
    void HistoryInfo(QVector<QString>);
    void FileSend(QTcpSocket *, QTcpSocket *);

public slots:
    void NewConnection();
    void Disconnected();
    void ReadyRead();
    void StateChanged(QAbstractSocket::SocketState);

private:
    QTcpServer *Server_Socket;      //服务器Socket

    QVector<QString> UserNameList;  //所有用户名
    QMap<QTcpSocket*, QString> *UserN_S;                    //<用户Socket，用户名>
    //私聊
    QMap<QVector<QString>, QTcpSocket*>         *P2PMap;    //<[发送方，接收方]，私聊页面Socket>

    QMap<QVector<QString>, QVector<serverinfo>> *P2PMsgBuf; //<[发送方，接收方]，<历史消息>>

    //群聊
    QVector<QString> GroupNameList; //所有组名

    QMap<QString, QVector<QTcpSocket*>>     *GroupMemSList; //<组名，<组内成员Socket>>
    QMap<QString, QVector<QString>>         GroupMemNList;  //<组名，<组内成员名>>
    QMap<QTcpSocket*, QString>              *GroupS_N;      //<用户端对应的群窗口Socket,群名>

    QMap<QString, QVector<serverinfo>>      *MulPMsgBuf;    //<组名，<历史消息>>

    //文件传输
    QTcpSocket *File_Socket;
    bool        FileF = false;
    qint64      FileH=0;//进度
    quint64     FileRSize=0;//长度

};

#endif // SERVER_H
