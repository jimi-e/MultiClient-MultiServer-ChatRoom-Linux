
#include "mainwin.h"
#include "qdebug.h"
#include <QApplication>


int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

    mainwin m;
    m.show();

    return a.exec();
}

