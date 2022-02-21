#include <iostream>
#include <string>
#include <QApplication>

#include "SolarWindow.h"

int main(int argc, char *argv[])
{
    // create the application
    QApplication app(argc, argv);

    // create a SolarWindow object as the main window.
    SolarWindow *window = new SolarWindow(NULL);

    // Set the size of the main window
    window->setMinimumSize(400, 250);
    window->setGeometry(40, 40, 1280, 720);

    // Set window name
    window->setWindowTitle("Solar Simulation");

    // show the label
    window->show();

    // start it running
    app.exec();

    // clean up, delete the window object
    delete window;

    return 0;
}