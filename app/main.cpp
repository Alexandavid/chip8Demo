#include <chrono>
#include <QApplication>
#include <QFile>
#include <qstyle.h>
#include <QTextStream>

#include "Qt/MainWindow.h"


void loadStyleSheet(QApplication &app) {
    QFile file("../app/Qt/widgets.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        QString styleSheet = stream.readAll();
        app.setStyleSheet(styleSheet);
    }
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    loadStyleSheet(app);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
