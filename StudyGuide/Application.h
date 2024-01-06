//
// Created by Jesse on 7 okt. 2023.
//

#if defined(APPLICATION)
#undef APPLICATION
#endif
#define APPLICATION (static_cast<Application*>(QCoreApplication::instance()))


#include "guide/GuideData.h"
#include "ui/MainWindow.h"

class Application : public QApplication
{
public:
    Application(int& argc, char** argv);

    virtual ~Application();

    static void restart();


public slots:
    static void addGuide(GuideData::Data data);

private:
    void setLanguage(const QString& languageCode);
     MainWindow* appWindow;
     QTranslator* translator;
};
