#include <QApplication>
#include "mainwindow.h"


CREATION_COORDS creation_coords;
LAYERS layers;
CONFIG config;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
