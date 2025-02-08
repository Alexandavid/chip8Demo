#include <chrono>
#include <fstream>
#include <iostream>
#include <QApplication>

#include "Qt/MainWindow.h"


uint8_t screenHeight, screenWidth;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
