//
// Created by Jesse on 7 okt. 2023.
//


#include <QTimer>
#include "guide/GuideData.h"
#include "ui/MainWindow.h"
#include <QApplication>
#include "creator/Creator.h"

#if defined(APPLICATION)
#undef APPLICATION
#endif
#define APPLICATION (static_cast<Application*>(QCoreApplication::instance()))

class Application : public QApplication {
public:
    Application(int&argc, char** argv);

    virtual ~Application();

    static void restart();

    std::unique_ptr<QFile> logFile;

    QString getLogsDirLocation();

    QString getAutoSaveLocation();

    QVector<Guide *> guidesToSave;

    void startAutoSaveTimer();

    bool isAutoSaveTimerStarted = false;

    QVector<GuideData::Data> getUpToDateGuides();

    void updateGuide(int guideIndex, GuideData::Data updatedGuide);

    bool isFileChanged = false;

    bool isXmlFile(const QString&file);

    bool isZipFile(const QString&file);

    void autoSave(Guide* guide);

    void setLanguage(const QString&languageCode);

    void retranslateUi();

    void updateStyle();

    QString defaultStyle;
#ifdef Q_OS_ANDROID

    void requestStoragePermission();

#endif

public slots:
    void autoSaveTriggered();

private:
    MainWindow* appWindow;
    QTranslator* translator;
    QTimer* autoSaveTimer = new QTimer(this);
    Creator* creator;
};
