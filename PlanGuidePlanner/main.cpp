#include "ui/MainWindow.h"
#include "Application.h"

// smallest main.cpp file ever!
int main(int argc, char* argv[]) {
    Application app(argc, argv);
    return app.exec();
}
