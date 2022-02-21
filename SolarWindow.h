#ifndef SOLARWINDOW_H
#define SOLARWINDOW_H
#ifndef __GL_POLYGON_WINDOW_H__
#define __GL_POLYGON_WINDOW_H__ 1

#include <QGLWidget>
#include <QSlider>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QTimer>
#include "SolarWidget.h"

class SolarWindow: public QWidget {

public:
    SolarWindow(QWidget *parent);
    ~SolarWindow();

private:
    void init_Widgets();
    void add_connections();
    SolarWidget *solarWidget;

    QGridLayout *windowLayout;
    QGridLayout *sliderLayout;
    QHBoxLayout *buttonLayout;

    QSlider *leftSlider;
    QSlider *rightSlider;
    QSlider *bottomSlider;
    QSlider *zoomSlider;
    QSlider *speedSlider;
    QLabel  *zoomLabel;
    QLabel  *speedLabel;
    QLabel  *voidLabel;
    QPushButton *enableEnv;
    QPushButton *enableSun;

    QTimer *stimer;
};

#endif
#endif
