#include "ui/MainWindow.h"
#include "Application.h"
#include "config.h"

// smallest main.cpp file ever!
int main(int argc, char* argv[]) {
    qDebug() << "Version:" << version;
    Application app(argc, argv);
    return app.exec();
}
