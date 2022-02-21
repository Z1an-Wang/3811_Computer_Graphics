#include "SolarWindow.h"
#include <iostream>

SolarWindow::SolarWindow(QWidget *parent) : QWidget(parent){

    // create the window layout
    windowLayout = new QGridLayout(this);
    sliderLayout = new QGridLayout(this);
    buttonLayout = new QHBoxLayout(this);

    init_Widgets();

    solarWidget = new SolarWidget(this);

    windowLayout->addWidget(solarWidget, 1, 1, 1, 10);
    windowLayout->addWidget(leftSlider, 1, 0, 1, 1);
    windowLayout->addWidget(rightSlider, 1, 11, 1, 1);

    windowLayout->addWidget(bottomSlider, 2, 1, 1, 10);
    windowLayout->addWidget(voidLabel, 3, 0, 1, 12);

    windowLayout->addWidget(enableEnv, 4, 1, 2, 2);
    windowLayout->addWidget(enableSun, 6, 1, 2, 2);

    windowLayout->addWidget(zoomLabel, 4, 4, 1, 1);
    windowLayout->addWidget(zoomSlider, 5, 4, 1, 7);

    windowLayout->addWidget(speedLabel, 6, 4, 1, 1);
    windowLayout->addWidget(speedSlider, 7, 4, 1, 7);

    add_connections();

    stimer = new QTimer(this);
    connect(stimer, SIGNAL(timeout()), solarWidget, SLOT(solarRotate()));
    stimer->start(10);
}

void SolarWindow::init_Widgets(){
    this->leftSlider = new QSlider(Qt::Vertical);
    leftSlider->setFixedWidth(25);

    this->rightSlider = new QSlider(Qt::Vertical);
    rightSlider->setFixedWidth(25);

    this->bottomSlider = new QSlider(Qt::Horizontal);
    bottomSlider->setFixedHeight(25);

    this->zoomSlider = new QSlider(Qt::Horizontal);
    zoomSlider->setFixedHeight(25);

    this->speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setFixedHeight(25);

    leftSlider->setRange(-180, 180);
    leftSlider->setTickPosition(QSlider::TicksBothSides);
    leftSlider->setTickInterval(24);
    rightSlider->setRange(-90, 90);
    rightSlider->setTickPosition(QSlider::TicksBothSides);
    rightSlider->setTickInterval(12);
    bottomSlider->setRange(-360, 360);
    bottomSlider->setTickPosition(QSlider::TicksBelow);
    bottomSlider->setTickInterval(24);
    zoomSlider->setRange(30, 260);
    zoomSlider->setTickPosition(QSlider::TicksBelow);
    zoomSlider->setTickInterval(23);
    speedSlider->setRange(0, 100);
    speedSlider->setTickPosition(QSlider::TicksBelow);
    speedSlider->setTickInterval(10);

    leftSlider->setValue(0);
    rightSlider->setValue(25);
    bottomSlider->setValue(0);
    zoomSlider->setValue(160);
    speedSlider->setValue(10);

    this->enableEnv = new QPushButton();
    enableEnv -> setText("Environment Light");
    this->enableSun = new QPushButton();
    enableSun->setText("Sun Light");

    this->zoomLabel = new QLabel("Zoom: ");
    this->speedLabel = new QLabel("Speed: ");
    this->voidLabel = new QLabel();
    voidLabel->setFixedHeight(10);
}

void SolarWindow::add_connections(){
    connect(rightSlider, SIGNAL(valueChanged(int)), solarWidget, SLOT(updateAngleX(int)));
    connect(bottomSlider, SIGNAL(valueChanged(int)), solarWidget, SLOT(updateAngleY(int)));
    connect(leftSlider, SIGNAL(valueChanged(int)), solarWidget, SLOT(updateAngleZ(int)));
    connect(zoomSlider, SIGNAL(valueChanged(int)), solarWidget, SLOT(updateZoom(int)));
    connect(speedSlider, SIGNAL(valueChanged(int)), solarWidget, SLOT(updateSpeed(int)));
    connect(enableSun, SIGNAL(clicked()), solarWidget, SLOT(enableSunLight()));
    connect(enableEnv, SIGNAL(clicked()), solarWidget, SLOT(enableEnvLight()));
}

SolarWindow::~SolarWindow()
{
    // delete stimer;
    delete speedSlider;
    delete zoomSlider;
    delete bottomSlider;
    delete rightSlider;
    delete leftSlider;
    delete solarWidget;
    delete sliderLayout;
    delete buttonLayout;
    delete windowLayout;
}