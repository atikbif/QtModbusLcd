#include "mainwindow.h"
#include <QApplication>
#include "lcdwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedWidth(w.display->width()+20);
    w.show();

    return a.exec();
}
