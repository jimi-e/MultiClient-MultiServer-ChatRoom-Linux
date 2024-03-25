#include "maininfo.h"



void maininfo::mainwinREQ(QString UserName_)
{
    UserName   = UserName_;
    REVerName  = "server";
    InfoType   = "MainWin";
    REQTime    = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    Trans_SendBuf();

}
void maininfo::GroupCreate(QString UN,QString GN)
{
    UserName   = UN;
    REVerName  = "server";
    InfoType   = "GroupCreate";
    REQTime    = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    Content    = GN;
    Trans_SendBuf();

}

void maininfo::ChatConnectREQ(QString LU,QString CU,QString CT)
{
    UserName    = LU;
    REVerName   = CU;
    if(CT=="P2P")
        InfoType = "P2PConnect";
    else
        InfoType = "MulPConnect";
    REQTime     = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    Trans_SendBuf();
}

void maininfo::SendMsg(QString LU,QString CU,QString CT,QString T)
{
    UserName    = LU;
    REVerName   = CU;
    if(CT=="P2P")
        InfoType = "P2PMsg";
    else
        InfoType = "MulPMsg";
    REQTime     = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    Content     = T;
    Trans_SendBuf();

}

void maininfo::SendFile(qint64 FL, QString FN, QString LU, QString CU)
{
    UserName    = LU;
    REVerName   = CU;
    InfoType    = "SendFile";
    REQTime     = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    Content     = FN;
    FileSize    = FL;
    Trans_SendBuf();

}

void maininfo::Trans_SendBuf()
{
    SendBuf.clear();
    QDataStream Data(&SendBuf, QIODevice::ReadWrite);
    Data << (qint64) 0 << UserName << REVerName << InfoType  << REQTime;

    if(InfoType == "MainWin"||InfoType == "P2PConnect"||InfoType == "MulPConnect")
    { }
    else if(InfoType == "P2PMsg"||InfoType == "MulPMsg")
    {
        Data << Content;
    }
    else if(InfoType == "SendFile")
    {
        Data << Content << FileSize;
    }
    else if(InfoType == "GroupCreate")
    {
         Data << Content;
    }

    Data.device()->seek(0);
    Data << (qint64) (SendBuf.size() - sizeof (qint64));


}

void maininfo::Trans_RcvBuf()
{
    QDataStream Data(&RcvBuf, QIODevice::ReadWrite);
    Data >> UserName >> REVerName >> InfoType >> REQTime;

    if(InfoType == "UserList" || InfoType == "GroupList" )
    {
        Data >> RevVec;
    }
    else if (InfoType == "SendFile")
    {
        Data >> Content >> FileSize;
    }
    else if(InfoType == "P2PMsg"||InfoType == "MulPMsg")
    {
        Data >> Content;
    }

}

