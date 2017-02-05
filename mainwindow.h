#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QAction>
#include <QDebug>
#include "map.h"


extern CREATION_COORDS creation_coords;
extern LAYERS layers;
extern CONFIG config;


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    QAction *floor;
    QAction  *wall;
    QAction  *item;
    QAction * entity;
    QAction *sound;
    QAction *light;
    Map * map;

private slots:
    void putMessage(const char *mess);

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
