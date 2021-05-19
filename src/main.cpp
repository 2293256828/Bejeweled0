#include "src/headers/ui/mainwindow.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
//    MainWindow w2;
    w.show();
//    w2.show();
    a.exec();

}
