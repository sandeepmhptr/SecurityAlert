#include "mainwindow.h"
#include <QApplication>
#include "tabdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    TabDialog td(0,0);
    td.show();
    return a.exec();
}
