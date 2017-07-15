#include "mainwindow.h"
#include <QApplication>
#include "../connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (!createConnection())//this control is necessary before creation of w in order to create headerconnection
        return 1;

    MainWindow w;
    w.setGeometry(25, 40, 800, 700);
    w.show();

    return a.exec();
}
