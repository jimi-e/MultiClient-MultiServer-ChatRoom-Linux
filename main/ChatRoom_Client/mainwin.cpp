#include "mainwin.h"
#include "ui_mainwin.h"

mainwin::mainwin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mainwin)
{
    ui->setupUi(this);
    ui->lineEdit_2->setVisible(false);
    ui->pushButton_4->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->pushButton->setVisible(false);


}

mainwin::~mainwin()
{
    delete ui;
}


void mainwin::Start(QString g)
{
    this->UserName=g ;

    //初始化用户表群聊表
    UserListView = new QStringListModel(this);
    ui->listView->setModel(UserListView);
    ui->listView->setEditTriggers(QAbstractItemView:: NoEditTriggers);

    //初始化群聊表
    GroupListView = new QStringListModel(this);
    ui->listView_2->setModel(GroupListView);
    ui->listView_2->setEditTriggers(QAbstractItemView:: NoEditTriggers);

    //初始化按键
    ui->pushButton_4->setVisible(true);
    ui->pushButton_3->setVisible(true);
    ui->pushButton->setVisible(true);

    //初始化子聊天页面
    sub = new chatwin();
    Qt::WindowFlags flags = Qt::Dialog;
    sub->setWindowFlags(flags);
    sub->setWindowModality(Qt::ApplicationModal);//chat页面出现时禁用主页面

    //flagConnected = false;
    // 这个socket用于交换用户表群聊表信息
    Main_Socket = new QTcpSocket();
    connect(Main_Socket, SIGNAL(connected()), this, SLOT(Connected()));
    connect(Main_Socket, SIGNAL(disconnected()), this, SLOT(Disconnected()));
    connect(Main_Socket, SIGNAL(readyRead()), this, SLOT(ReadyRead()));

    Main_Socket->connectToHost("127.0.0.1", 8888);


    if (!Main_Socket->waitForConnected(2000))
    {
        ui->label_2->setText("Server not online! Please try again.");
    }
    else
    {
        // 连接到server，向server发送一条请求
        maininfo Start_Info;
        Start_Info.mainwinREQ(UserName);
        Main_Socket->write(Start_Info.SendBuf);


        ui->label->setText("User:"+g);
        ui->pushButton_2->setEnabled(false);
        ui->lineEdit->setEnabled(false);
        //ui->label_2->setText("Connected to server.");

    }

}

void mainwin::Connected()
{
    ui->label_2->setText("服务器连接成功！");
}
void mainwin::Disconnected()
{

    ui->label->setText("");
    ui->label_2->setText("服务器下线！");
    ui->label_3->setText("");
    ui->pushButton_2->setEnabled(true);
    ui->lineEdit->setEnabled(true);
    UserListView->removeRows(0,UserListView->rowCount());//清空


}

void mainwin::ReadyRead()
{

    QString msg="更新时间：";
    msg.append(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));
    ui->label_3->setText(msg);

    if (Main_Socket->bytesAvailable() < (qint64)sizeof(qint64))
        return;

    QDataStream dataStream(Main_Socket->read(sizeof (qint64)));

    qint64 nextBufSize = 0;
    dataStream >> nextBufSize;
    qDebug() <<"R接收：缓冲大小"<< nextBufSize;

    while (Main_Socket->bytesAvailable() < nextBufSize)
        QThread::msleep(10);

    maininfo RR_Info;
    RR_Info.RcvBuf = Main_Socket->read(nextBufSize);

    RR_Info.Trans_RcvBuf();


    qDebug() <<RR_Info.InfoType;

    if (RR_Info.InfoType == "UserList")
    {
        QStringList qsl;

        for (int i = 0; i < RR_Info.RevVec.size(); i++)
        {
                qsl += RR_Info.RevVec.at(i);
        }

        UserListView->setStringList(qsl);

    }


    else if (RR_Info.InfoType == "GroupList")
    {
        QStringList qsl;

        for (int i = 0; i < RR_Info.RevVec.size(); i++) {
                qsl += RR_Info.RevVec.at(i);

        }
        GroupListView->setStringList(qsl);


    }

    if(Main_Socket->bytesAvailable() >= (qint64)sizeof(qint64))
        ReadyRead();

}


void mainwin::on_pushButton_clicked()//刷新
{
    ReadyRead();

}

void mainwin::on_pushButton_2_clicked()//登陆
{
    QString tmp = ui->lineEdit->text();

    if(tmp==NULL)
    {
        QMessageBox::information(this, "错误", "ID不能为空！");
    }
    else
    {
        //QMessageBox::information(this, "错误", GetUserName());
        //UserNameG == tmp;
        UserName = tmp ;
        Start(UserName);

    }

}

void mainwin::on_listView_doubleClicked(const QModelIndex &index)
{
    //row从0开始
    //QMessageBox::information(this, "ID", QString::asprintf("当前项:row=%d, column=%d", index.row(), index.column()));

    //创建chat子窗口
    sub = new chatwin();
    Qt::WindowFlags flags = Qt::Dialog;
    sub->setWindowFlags(flags);
    sub->setWindowModality(Qt::ApplicationModal);//chat页面出现时禁用主页面


    QStringList tmp;
    tmp = UserListView->stringList();
    QString ChatName = tmp.at(index.row());


    sub->CreateChat(UserName,ChatName,"P2P");

    sub->show();
    qDebug() << "打开私聊子窗口!!";
    if(sub->exec() == QDialog::Rejected)
    {
        delete sub;
    }
    qDebug() << "关闭私聊子窗口!!";


}

void mainwin::on_listView_2_doubleClicked(const QModelIndex &index)
{
    sub = new chatwin();
    Qt::WindowFlags flags = Qt::Dialog;
    sub->setWindowFlags(flags);
    sub->setWindowModality(Qt::ApplicationModal);//chat页面出现时禁用主页面


    QStringList tmp;
    tmp = GroupListView->stringList();
    QString GroupName = tmp.at(index.row());

    sub->CreateChat(UserName,GroupName,"MulP");

    sub->show();
    qDebug() << "打开群聊子窗口!!";
    qDebug() << GroupName;

    if(sub->exec() == QDialog::Rejected)
    {
        delete sub;
    }
    qDebug() << "关闭群聊子窗口!!";


}

void mainwin::on_pushButton_3_clicked()
{
    sub = new chatwin();
    Qt::WindowFlags flags = Qt::Dialog;
    sub->setWindowFlags(flags);
    sub->setWindowModality(Qt::ApplicationModal);//chat页面出现时禁用主页面

    QString ChatName = "世界频道";


    sub->CreateChat(UserName,ChatName,"MulP");

    sub->show();

    if(sub->exec() == QDialog::Rejected)
    {
        delete sub;
    }

}

void mainwin::on_pushButton_4_clicked()
{
    if(ui->pushButton_4->text() == "创建群聊")
    {
        ui->lineEdit_2->setVisible(true);
        ui->pushButton_4->setText("确定");}
    else
    {


        QStringList tmp;
        tmp = GroupListView->stringList();
        if(tmp.indexOf(ui->lineEdit_2->text())==-1)//未创建
        {
            tmp += ui->lineEdit_2->text();
            GroupListView->setStringList(tmp);
            maininfo GC_Info;
            GC_Info.GroupCreate(UserName,ui->lineEdit_2->text());
            Main_Socket->write(GC_Info.SendBuf);

        }
        else
        {
            QMessageBox::information(this, "错误", "该群组已存在！");
        }
        ui->lineEdit_2->setText("");
        ui->pushButton_4->setText("创建群聊");
        ui->lineEdit_2->setVisible(false);

    }


}


