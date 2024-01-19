//
// Created by Jesse on 7 okt. 2023.
//

#if defined(APPLICATION)
#undef APPLICATION
#endif
#define APPLICATION (static_cast<Application*>(QCoreApplication::instance()))


#include <QFile>

#include "guide/GuideData.h"
#include "ui/MainWindow.h"

class Application : public QApplication {
public:
    Application(int &argc, char **argv);

    virtual ~Application();

    static void restart();

    std::unique_ptr<QFile> logFile;

public slots:

    static void addGuide(GuideData::Data data);

private:
    void setLanguage(const QString &languageCode);

    MainWindow *appWindow;
    QTranslator *translator;
};
