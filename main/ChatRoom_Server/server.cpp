#include "server.h"


Server::Server(QWidget *parent) : QWidget(parent)
{
     Serv_Init();
     Server_Socket->setMaxPendingConnections(10);
     connect(Server_Socket, SIGNAL(newConnection()), this, SLOT(NewConnection()));

    if (Server_Socket->listen(QHostAddress::Any, 8888))
        qDebug() << "服务器运行成功！端口号：8888";
    else
    {
        qDebug() << "错误：服务器无法启动！";
        qDebug() << Server_Socket->errorString();
    }

}

void Server::NewConnection()
{
    QTcpSocket* client = Server_Socket->nextPendingConnection();

    connect(client, &QTcpSocket::disconnected, this, &Server::Disconnected);
    connect(client, &QTcpSocket::readyRead, this, &Server::ReadyRead);
    connect(client, &QTcpSocket::stateChanged, this, &Server::StateChanged);

    qDebug() << "Socket连接(IP：" + client->peerAddress().toString() + " 端口:" + QString::number(client->peerPort())+ ")" ;
}

void Server::Disconnected()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());

    qDebug() << "Socket断开连接(IP：" + client->peerAddress().toString() +" 端口:" + QString::number(client->peerPort()) + ")" ;

    if (UserN_S->contains(client))
    {
        qDebug() << "——————用户退出——————";
        qDebug() << "用户名：" << UserN_S->value(client);

        UserNameList.removeOne(UserN_S->value(client));
        UserN_S->remove(client);

        foreach (QTcpSocket* s, UserN_S->keys())
        {
            SendFList(s);
        }
        return;
    }

    else if (P2PMap->values().contains(client))
    {


        QVector<QString> k = P2PMap->key(client);
        P2PMap->remove(k);
        return;
    }

    else if (GroupS_N->contains(client))
    {

        QString clientName = UserN_S->value(client);
        UserN_S->remove(client);
        foreach (QVector<QTcpSocket*> v, GroupMemSList->values())
        {
            if (v.contains(client))
            {
                v.removeOne(client);

                break;
            }
        }
        foreach (QVector<QString> v, GroupMemNList.values())
        {
            if (v.contains(clientName))
            {
                v.removeOne(clientName);
                break;
            }
        }
        return;
    }
}

void Server::ReadyRead()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());

    if (FileF)
    {
        FileSend(client, File_Socket);
    }

    qint64 nextBufSize = 0;

    if (client->bytesAvailable() < (qint64)sizeof(qint64))
    {
        return;
    }
    QDataStream dataStream(client->read(sizeof (qint64)));
    dataStream >> nextBufSize;



    serverinfo Serv_Info;
    Serv_Info.RcvBuf = client->read(nextBufSize);
    Serv_Info.Trans_RcvBuf();
    Serv_Info.Trans_SendBuf();

    // 更新用户表和群组
    if (Serv_Info.InfoType == "MainWin")
    {

        if(!UserN_S->contains(client))
        {
            qDebug() << "——————新用户登陆——————";
            qDebug() << "用户名：" << Serv_Info.UserName;

            UserN_S->insert(client, Serv_Info.UserName);
            UserNameList.push_back(Serv_Info.UserName);

            SendGList(client);
            // 广播
            foreach (QTcpSocket* s, UserN_S->keys())
            {

                SendFList(s);
            }
        }
        // 如果在列表中，只向其发送列表就可以
        else
        {
            qDebug() << "——————用户刷新请求——————";
            qDebug() << "用户名：" << Serv_Info.UserName;

            SendFList(client);
        }

    }
    //创建群组
    else if(Serv_Info.InfoType == "GroupCreate")
    {
        qDebug() << "——————创建群组——————";
        qDebug() << "群名：" << Serv_Info.Content;

        GroupNameList.push_back(Serv_Info.Content);
        QVector<QTcpSocket*> vt;
        GroupMemSList->insert(Serv_Info.Content, vt);
        QVector<QString> vs;
        GroupMemNList.insert(Serv_Info.Content, vs);

        foreach (QTcpSocket* s, UserN_S->keys()) {
           SendGList(s);
         }
    }
    // 私人聊天建立
    else if (Serv_Info.InfoType == "P2PConnect")
    {
        qDebug() << "——————私人聊天创建——————";
        qDebug() << "请求用户：" << Serv_Info.UserName;
        qDebug() << "接收用户：" << Serv_Info.REVerName;

        QVector<QString> a2b;
        a2b.push_back(Serv_Info.UserName);
        a2b.push_back(Serv_Info.REVerName);
        if (!P2PMap->contains(a2b))
        {

            P2PMap->insert(a2b, client);
            HistoryInfo(a2b);
        }
    }
    // 群聊建立
    else if (Serv_Info.InfoType == "MulPConnect")
    {
        qDebug() << "——————用户加入群聊——————";
        qDebug() << "请求用户：" << Serv_Info.UserName;
        qDebug() << "加入群组：" << Serv_Info.REVerName;


        QString groupName = Serv_Info.REVerName;

        GroupMemSList->find(groupName)->push_back(client);

        GroupMemNList.find(groupName)->push_back(Serv_Info.UserName);
        GroupS_N->insert(client, Serv_Info.UserName);


    }

    // 私聊消息
    else if (Serv_Info.InfoType == "P2PMsg")
    {
        qDebug() << "——————私聊消息——————";
        qDebug() << "发送用户：" << Serv_Info.UserName;
        qDebug() << "接收用户：" << Serv_Info.REVerName;
        qDebug() << "消息内容：" << Serv_Info.Content;



        QVector<QString> a2b, b2a;
        a2b.push_back(Serv_Info.UserName);
        a2b.push_back(Serv_Info.REVerName);
        b2a.push_back(Serv_Info.REVerName);
        b2a.push_back(Serv_Info.UserName);

        // 存入历史信息
        if (!P2PMsgBuf->contains(a2b))
        {
            QVector<serverinfo> infoVector;
            infoVector.push_back(Serv_Info);
            P2PMsgBuf->insert(a2b, infoVector);
            P2PMsgBuf->insert(b2a, infoVector);
        }
        else {
            QMap<QVector<QString>, QVector<serverinfo>>::Iterator it;
            it = P2PMsgBuf->find(a2b);
            it.value().push_back(Serv_Info);
            it = P2PMsgBuf->find(b2a);
            it.value().push_back(Serv_Info);
        }

        // 发送
        if (client->isOpen() && client->isWritable())
        {
            client->write(Serv_Info.SendBuf);
        }

        if (P2PMap->contains(b2a))
        {
            QTcpSocket* target = P2PMap->value(b2a);
            if (target->isOpen() && target->isWritable())
            {
                target->write(Serv_Info.SendBuf);
            }
        }

    }

    else if (Serv_Info.InfoType == "MulPMsg")
    {

        qDebug() << "——————私聊消息——————";
        qDebug() << "发送用户：" << Serv_Info.UserName;
        qDebug() << "群组：" << Serv_Info.REVerName;
        qDebug() << "消息内容：" << Serv_Info.Content;

        QString groupName = Serv_Info.REVerName;

        // 存入历史信息
        if (!MulPMsgBuf->contains(groupName))
        {
            QVector<serverinfo> infoVector;
            infoVector.push_back(Serv_Info);
            MulPMsgBuf->insert(groupName, infoVector);
            MulPMsgBuf->insert(groupName, infoVector);
        }
        else {
            MulPMsgBuf->find(groupName).value().push_back(Serv_Info);
        }

        //组播给群成员
        foreach (QTcpSocket* socket, GroupMemSList->value(Serv_Info.REVerName))
        {
            if (socket->isOpen() && socket->isWritable())
            {
                socket->write(Serv_Info.SendBuf);
            }
        }

    }
    else if (Serv_Info.InfoType == "SendFile")
    {

        qDebug() << "——————发送文件——————";
        qDebug() << "发送用户：" << Serv_Info.UserName;
        qDebug() << "接收用户：" << Serv_Info.REVerName;
        qDebug() << "文件名：" << Serv_Info.Content;
        qDebug() << "文件大小：" << Serv_Info.FileSize;

        QVector<QString> b2a;
        b2a.push_back(Serv_Info.REVerName);
        b2a.push_back(Serv_Info.UserName);
        if (P2PMap->contains(b2a))
        {
            FileF = true;
            FileRSize = Serv_Info.FileSize;
            FileH = 0;
            File_Socket = P2PMap->value(b2a);
            File_Socket->write(Serv_Info.SendBuf);
            FileSend(client, File_Socket);
        }
    }
}

void Server::StateChanged(QAbstractSocket::SocketState state)
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString desc;

    if (state == QAbstractSocket::UnconnectedState)
        desc = "未连接";
    else if (state == QAbstractSocket::HostLookupState)
        desc = "正在进行主机名查找";
    else if (state == QAbstractSocket::ConnectingState)
        desc = "正在建立连接";
    else if (state == QAbstractSocket::ConnectedState)
        desc = "连接已建立";
    else if (state == QAbstractSocket::BoundState)
        desc = "Socket绑定IP和端口";
    else if (state == QAbstractSocket::ClosingState)
        desc = "Socket即将关闭(数据可能未写入)";
    qDebug() << "Socket状态变化(IP：" + client->peerAddress().toString() + " 端口:" + QString::number(client->peerPort()) + "): " + desc;
}

void Server::Serv_Init()
{
    Server_Socket      = new QTcpServer(this);

    UserN_S  = new QMap<QTcpSocket*, QString>;
    P2PMap  = new QMap<QVector<QString>, QTcpSocket*>;
    P2PMsgBuf   = new QMap<QVector<QString>, QVector<serverinfo>>;

    GroupMemSList       = new QMap<QString, QVector<QTcpSocket*>>;
    GroupS_N  = new QMap<QTcpSocket*, QString>;
    MulPMsgBuf        = new QMap<QString, QVector<serverinfo>>;

    GroupNameList.push_back("公共聊天室");
    QVector<QTcpSocket*> S1;
    GroupMemSList->insert("公共聊天室", S1);
    QVector<QString> QS1;
    GroupMemNList.insert("公共聊天室", QS1);

}
void Server::SendFList(QTcpSocket* client)
{
    serverinfo Serv_Info;
    Serv_Info.SendVec("UserList", UserNameList);
    if (client->isOpen() && client->isWritable())
    {
        client->write(Serv_Info.SendBuf);

    }
}

void Server::SendGList(QTcpSocket* client)
{
    serverinfo Serv_Info;
    Serv_Info.SendVec("GroupList", GroupNameList);
    if (client->isOpen() && client->isWritable())
    {
        client->write(Serv_Info.SendBuf);
    }
}

void Server::HistoryInfo(QVector<QString> a2b)
{
    if (P2PMsgBuf->contains(a2b) && P2PMsgBuf->value(a2b).size())
    {

        QTcpSocket *client = P2PMap->value(a2b);
        for (int i = 0; i < P2PMsgBuf->value(a2b).size(); i++) {
            while(true)
            {
                if (client->isOpen() && client->isWritable())
                {
                    client->write(P2PMsgBuf->value(a2b).at(i).SendBuf);
                    break;
                }
                QThread::msleep(10);
            }

        }
    }
}

void Server::FileSend(QTcpSocket* source, QTcpSocket* target)
{
    QByteArray fileBuf;

    fileBuf = source->readAll();
    FileH += fileBuf.size();
    target->write(fileBuf);

    if (FileH >= FileRSize)
    {
        FileF = false;

    }
}

