#include <QApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server* s;
    s = new Server();

    return a.exec();
}
