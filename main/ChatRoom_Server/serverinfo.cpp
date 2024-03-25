#include "serverinfo.h"

void serverinfo::SendVec(QString T, QVector<QString> V)
{
    UserName    = "SERVER";
    REVerName   = "ALLUser";
    InfoType    = T; //"GroupList"
    REQTime     = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    SendVector  = V;
    Trans_SendBuf();
}

void serverinfo::Trans_SendBuf()
{
    SendBuf.clear();
    QDataStream Data(&SendBuf, QIODevice::ReadWrite);
    Data << (qint64) 0 << UserName << REVerName << InfoType << REQTime;

    if (InfoType == "SendFile")
    {
        Data << Content << FileSize;
    }
    else if(InfoType == "UserList" || InfoType == "GroupList")
    {
        Data << SendVector;
    }
    else if(InfoType == "P2PMsg"||InfoType == "MulPMsg")
    {
        Data << Content;
    }
    else
    {
        Data << Content;
    }

    Data.device()->seek(0);
    Data << (qint64) (SendBuf.size() - sizeof (qint64));

}


void serverinfo::Trans_RcvBuf()
{
    QDataStream Data(&RcvBuf, QIODevice::ReadWrite);
    Data >> UserName >> REVerName >> InfoType >> REQTime;

    if (InfoType == "MainWin"||InfoType == "P2PConnect"||InfoType == "MulPConnect")
    {}
   else if(InfoType == "P2PMsg"||InfoType == "MulPMsg")
    {
        Data >> Content;
    }
    else if(InfoType == "SendFile")
    {
        Data >> Content >> FileSize;
    }
    else if(InfoType == "GroupCreate")
    {
         Data >> Content;
    }
    else
    {
        Data >> Content;
    }

}


