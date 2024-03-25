#include "chatwin.h"
#include "ui_chatwin.h"

chatwin::chatwin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chatwin)
{
    ui->setupUi(this);
}

chatwin::~chatwin()
{
    Sub_Socket->disconnectFromHost();
    if (!Sub_Socket->waitForConnected(2000))
        Sub_Socket->close();
    delete Sub_Socket;
    delete ui;
}

void chatwin::CreateChat(QString LU,QString CU,QString CT)
{
    LocalUser = LU;
    ChatUser = CU;
    ChatType = CT;

    if(ChatType == "P2P")
    ui->label->setText(LocalUser+"，您正在与："+ChatUser+"私人聊天");
    else if(ChatType == "MulP") //
    {ui->label->setText(LocalUser+"，您正在群组："+ChatUser+"中聊天");

     ui->pushButton_2->setVisible(false);
    }
    ui->progressBar->setVisible(false);


    Sub_Socket = new QTcpSocket();
    connect(Sub_Socket, SIGNAL(connected()), this, SLOT(Connected()));
    connect(Sub_Socket, SIGNAL(disconnected()), this, SLOT(Disconnected()));
    connect(Sub_Socket, SIGNAL(readyRead()), this, SLOT(ReadyRead()));

    Sub_Socket->connectToHost("127.0.0.1", 8888);// ip , 端口

    if (!Sub_Socket->waitForConnected(2000))
        ui->label->setText("与服务器连接失败！请关闭重试！");


}
void chatwin::Connected()
{
    maininfo Con_Info;
    //此用户 对方/群组 P2P/MulP
    Con_Info.ChatConnectREQ(LocalUser,ChatUser,ChatType);

    Sub_Socket->write(Con_Info.SendBuf);

    qDebug() <<"发送建立聊天连接";
    ConnectTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
}

void chatwin::Disconnected()
{
    ui->label->setText("与服务器连接失败！请关闭重试！");


}

void chatwin::ReadyRead()
{
    QString msg="更新时间：";
    msg.append(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));
    ui->label_2->setText(msg);

    if(FileF)
        FileRev(Sub_Socket);

    if (Sub_Socket->bytesAvailable() < (qint64)sizeof(qint64))
        return;

    QDataStream dataStream(Sub_Socket->read(sizeof (qint64)));

    qint64 BufSize = 0;
    dataStream >> BufSize;


    while (Sub_Socket->bytesAvailable() < BufSize)
        QThread::msleep(10);

    maininfo RcvMsg_Info;
    RcvMsg_Info.RcvBuf = Sub_Socket->read(BufSize);

    RcvMsg_Info.Trans_RcvBuf();



    if(RcvMsg_Info.InfoType == "P2PMsg"||RcvMsg_Info.InfoType == "MulPMsg")
    {   QString msg2;
        msg2.append("[" + RcvMsg_Info.REQTime + "] ");
        this->ui->textBrowser->append(msg2);
        msg2.clear();
        msg2.append("<"+RcvMsg_Info.UserName+">  :  " + RcvMsg_Info.Content);
        this->ui->textBrowser->append(msg2);
        qDebug() <<"————接收聊天消息————";
        qDebug() <<"大小:" << BufSize;
    }
    if (RcvMsg_Info.InfoType == "SendFile")
    {

        QMessageBox::information(this, "提醒", "收到文件，请选择保存地址！");

        QString DirName = QFileDialog::getExistingDirectory(this, tr("选择接收文件夹"), "/home");

        DirName.append("/"+RcvMsg_Info.Content);
        qDebug()<<"文件保存至："<< DirName;

        FileS = new QFile;
        if(FileS->exists(DirName))
        {
            qDebug()<<"文件存在！";
            QMessageBox::information(this, "错误", "文件存在！保存失败！");
            return ;
        }
        FileS->setFileName(DirName);
        if(!FileS->open(QIODevice::WriteOnly))
         {
             qDebug()<<"文件打开失败！";
             QMessageBox::information(this, "错误", "文件打开失败！保存失败！");
             return ;
         }

        FileH = 0;
        FileRSize = RcvMsg_Info.FileSize;    // 文件大小
        FileF = true;

        QString msg4;
        msg4.append("[" + RcvMsg_Info.REQTime + "] ");
        this->ui->textBrowser->append(msg4);
        msg4.clear();
        msg4.append("<div style=\"color:red;\">");
        msg4.append("你收到了("+RcvMsg_Info.UserName+")发送的文件" + "\""+RcvMsg_Info.Content+"\"");
        msg4.append("</div>");
        this->ui->textBrowser->append(msg4);

        FileRev(Sub_Socket);

    }


    if((qint64)Sub_Socket->bytesAvailable() >= (qint64)sizeof(qint64))
        ReadyRead();
}

void chatwin::FileRev(QTcpSocket* source)
{
    FileB += source->readAll();
    FileH += FileB.size();
    if (FileH >= FileRSize)
    {
        FileF = false;
        FileS->write(FileB);
        //文件保存成功

    }
}

void chatwin::on_pushButton_clicked()
{
    QString Text = ui->textEdit->toPlainText();
    if(Text!=NULL)
    {
        maininfo SendMsg_Info;
        SendMsg_Info.SendMsg(LocalUser,ChatUser,ChatType,Text);

        Sub_Socket->write(SendMsg_Info.SendBuf);
        qDebug() << "发送聊天消息";
    }
        ui->textEdit->clear();

}

void chatwin::on_pushButton_2_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("选择发送文件"), "/home");
    qDebug()<<"文件地址："<< fileName;

    QFile *f = new QFile(fileName);
    if(!f->open(QFile::ReadOnly))
    {
        qDebug() << "文件打开失败！";
        QMessageBox::information(this, "错误", "文件打开失败！");
        return;
    }

    maininfo SendF_Info;
    SendF_Info.SendFile(f->size(), fileName.right(fileName.size()- fileName.lastIndexOf('/')-1),LocalUser,ChatUser);

    Sub_Socket->write(SendF_Info.SendBuf);

    // 下面开始传输文件
    quint64 bufSize = 4*1024;   // 每次发送的buf大小
    quint64 haveSent = 0;   // 已经发送大小
    quint64 fileSize = f->size();    // 文件大小
    QByteArray sendBuf;

    ui->progressBar->setVisible(true);
    ui->progressBar->setMaximum(fileSize);
    ui->progressBar->setValue(haveSent);

    qDebug() << "开始发送文件";
    while (haveSent != fileSize) {
        sendBuf = f->read(qMin(bufSize, fileSize-haveSent));
        Sub_Socket->write(sendBuf);
        haveSent += qMin(bufSize, fileSize-haveSent);
        ui->progressBar->setMaximum(fileSize);
        ui->progressBar->setValue(haveSent);


    }
    QString msg3;
    msg3.append("[" + QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") + "] ");
    this->ui->textBrowser->append(msg3);
    msg3.clear();
    msg3.append("<div style=\"color:red;\">");
    msg3.append("你发送了文件\""+ fileName.right(fileName.size()- fileName.lastIndexOf('/')-1)+ "\"");
    msg3.append("</div>");
    this->ui->textBrowser->append(msg3);

    QMessageBox::information(this, "提醒", "文件发送成功！");
    ui->progressBar->setVisible(false);


    qDebug() << "文件发送成功，大小为：";
    qDebug() << f->size();

}
