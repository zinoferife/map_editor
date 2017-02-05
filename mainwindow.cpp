#include "mainwindow.h"

void MainWindow::putMessage(const char *mess)
{
    QString text = QString::fromUtf8(mess);
    setWindowTitle(text);
    qDebug() << text;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    map = new Map;
    setCentralWidget(map);
    memset (&creation_coords, 0, sizeof(creation_coords));
    memset (&layers, 1, sizeof(layers));
    memset (&config, 0, sizeof(config));

    config.draw_mode = DRAW_MODE_WIREFRAME;
    creation_coords.type = OBJECTTYPE_FLOOR;
    connect(map,&Map::putMessage,this,&MainWindow::putMessage);
}

MainWindow::~MainWindow()
{

}
