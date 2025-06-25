//
// Created by Jesse on 7 okt. 2023.
//

#include "Application.h"

#include <thread>
#include "Config.h"

#include "XmlParser.h"
#include "ui/dialogs/LoadGuide.h"
#include "ui/guide/Guide.h"
#include "themes/GuidePalette.h"

#ifdef WIN32

#include "WindowConsole.h"

#endif

#ifdef Q_OS_ANDROID

#include <private/qandroidextras_p.h>

#endif

void messageHandler(QtMsgType type, const QMessageLogContext&context, const QString&msg) {
    static std::mutex loggerMutex;
    const std::lock_guard<std::mutex> lock(loggerMutex); // synchronized, QFile logFile is not thread-safe

    QString out = qFormatLogMessage(type, context, msg);
    out += QChar::LineFeed;

    QTextStream(stderr) << out.toLocal8Bit();
    APPLICATION->logFile->write(out.toUtf8());
    APPLICATION->logFile->flush();
    fflush(stderr);
}

Application::Application(int&argc, char** argv) : QApplication(argc, argv) {
#ifdef WIN32
    AttachWindowsConsole();
#endif

    setOrganizationName("JesseRobot01");
    setApplicationName("StudyGuide");

    QCommandLineParser commandLineParser;

    commandLineParser.addOptions({
            {{"L", "lang"}, "Sets the language of the application", "lang"}
        }
    );
    commandLineParser.addHelpOption();

    commandLineParser.addPositionalArgument("files", "List of files to open", "[file1, file2, ...]");

    commandLineParser.process(arguments());

    QSettings settings;

    qSetMessagePattern(
        "%{time process}"
        " "
        "%{pid}"
        " "
        "%{if-debug}DEBUG   %{endif}"
        "%{if-info}INFO    %{endif}"
        "%{if-warning}WARNING %{endif}"
        "%{if-critical}CRITICAL%{endif}"
        "%{if-fatal}FATAL   %{endif}"
        " "
        "|"
        " "
        "%{message}");

    QDir logsDir(getLogsDirLocation());
    if (!logsDir.exists())
        logsDir.mkpath(".");

    // get highest number.
    int highestLogNumber = 0;
    for (QFileInfo file: logsDir.entryInfoList(QDir::Files)) {
        QString fileName = file.baseName();
        fileName.replace("Log", "");
        int fileNumber = fileName.toInt();
        if (fileNumber > highestLogNumber)
            highestLogNumber = fileNumber;
    }
    logFile = std::unique_ptr<QFile>(
        new QFile(logsDir.path() + "/Log" + QString::number(highestLogNumber + 1) + ".txt"));
    if (!logFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        qFatal("Can't open log file!");

    qInstallMessageHandler(messageHandler);
    // Logger is installed, so let's put the version in it!
    qDebug() << "Version:" << Config.version;


    // themes
    {
        QString currentTheme = settings.value("Theme", "fusion_dark").toString();

        if (currentTheme == "fusion_dark") {
            GuidePalette palette;
            palette.setFusionDark();
        }
        if (currentTheme == "fusion_light") {
            GuidePalette palette;
            palette.setFusionLight();
        }
        if (currentTheme == "system") {
            // let system decide. So no passing in agruments.
        }
    }

    translator = new QTranslator();


    if (!commandLineParser.value("lang").isEmpty()) {
        settings.setValue("Lang", commandLineParser.value("lang"));
        settings.sync();
    }

    setLanguage(settings.value("Lang", "en").toString());

    qDebug() << "Loading app window.";
    appWindow = new MainWindow;
    appWindow->show();
    qDebug() << "App Window loaded successfully.";

    if (!commandLineParser.positionalArguments().isEmpty()) {
        const QStringList files = commandLineParser.positionalArguments();
        QVector<GuideData::Data> guides = XmlParser::readXml(files);
        for (const GuideData::Data guide: guides) {
            appWindow->processGuide(guide);
        }
    }

    // auto open all files.

        QDir autoOpenDir = getAutoSaveLocation();
        if (autoOpenDir.exists() && !autoOpenDir.isEmpty()) {
            QStringList guideFileNames = autoOpenDir.entryList(QDir::Files);
            QStringList guideFiles;
            LoadGuide* loadGuide = new LoadGuide(nullptr, guideFileNames.count() * 2); // 1 for reading, 1 for opening.
            loadGuide->show();

            for (const QString&GuideFileName: guideFileNames)
                guideFiles.append(autoOpenDir.filePath(GuideFileName));

            QVector<GuideData::Data> guides = XmlParser::readXml(guideFiles);
            loadGuide->increaseProgress(guideFileNames.count());

            for (GuideData::Data guide: guides) {
                appWindow->processGuide(guide, false);
                loadGuide->increaseProgress();
            }
            //manualy update start.
            appWindow->updateStart();
            delete loadGuide; // no need anymore, so bye!
        }

}

Application::~Application() {
}

void Application::setLanguage(const QString&languageCode) {
    qDebug() << "Loading translation for" << languageCode;
    if (translator->load(":/translations/StudyGuide_" + languageCode + ".qm")) {
        installTranslator(translator);
        qDebug() << "Succesfully loaded translations for" << languageCode;
    }
    else {
        qCritical() << "Failed to load translation for" << languageCode;
        qInfo() << "Falling back to default translations.";

        if (translator->load(":/translations/StudyGuide_en.qm")) {
            installTranslator(translator);
        }
        else {
            qCritical() << "Failed to load translations.";
        }
    }
}

void Application::restart() {
#ifndef Q_OS_WASM
    QProcess::startDetached(applicationFilePath());
    qDebug() << "Booted new instance";
    exit(0);
    qDebug() << "Exiting current instance";
#else
    qFatal() << "You can't restart, so get an error!";
#endif
}

#ifdef Q_OS_ANDROID

void Application::requestStoragePermission() {
    bool value = QJniObject::callStaticMethod<jboolean>(
            "android/os/Environment",
            "isExternalStorageManager");
    if (value == false) {
        QJniObject filepermit = QJniObject::getStaticObjectField(
                "android/provider/Settings",
                "ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION",
                "Ljava/lang/String;");
        QJniObject pkgName = QJniObject::fromString("package:io.jesserobot.studyguide");
        QJniObject parsedUri = QJniObject::callStaticObjectMethod(
                "android/net/Uri",
                "parse", "(Ljava/lang/String;)Landroid/net/Uri;",
                pkgName.object<jstring>());
        QJniObject intent("android/content/Intent",
                          "(Ljava/lang/String;Landroid/net/Uri;)V",
                          filepermit.object<jstring>(), parsedUri.object());
        QtAndroidPrivate::startActivity(intent, 0);
    }
}

#endif

QString Application::getLogsDirLocation() {
    QSettings settings;

#ifdef Q_OS_ANDROID
    return settings.value("LogsDir",
                          (QStandardPaths::writableLocation(QStandardPaths::HomeLocation) +
                           "/logs")).toString();
#else
    return settings.value("LogsDir",
                          (QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                           "/logs")).toString();
#endif
}

QString Application::getAutoSaveLocation() {
    QSettings settings;

#ifdef Q_OS_ANDROID
    return settings.value("AutoSaveDir",
                          (QStandardPaths::writableLocation(QStandardPaths::HomeLocation) +
                           "/open guides")).toString();
#else
    return settings.value("AutoSaveDir",
                          (QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                           "/open guides")).toString();
#endif
}

void Application::startAutoSaveTimer() {
    if (!isAutoSaveTimerStarted) {
        int autoSaveInterval = 300000;
        qDebug() << "Starting auto save timer.";
        qInfo() << "Auto saving in" << autoSaveInterval << "Milliseconds.";

        connect(autoSaveTimer, &QTimer::timeout, this, [=]() { this->autoSaveTriggered(); });
        autoSaveTimer->setSingleShot(true);
        autoSaveTimer->start(autoSaveInterval);
        isAutoSaveTimerStarted = true;
    }
}

void Application::autoSaveTriggered() {
    qDebug() << "Auto saving...";
    QVector<GuideData::Data> guideDataToSave;

    //extract the guideData
    for (Guide* guide: guidesToSave) {
        guide->isInAutoSaveList = false;
        guideDataToSave.append(guide->getGuide());
    }
    XmlParser::autoSaveXml(guideDataToSave);

    isAutoSaveTimerStarted = false;

    appWindow->updateStart();
}

QVector<GuideData::Data> Application::getUpToDateGuides() {
    // Extract guides
    QVector<GuideData::Data> result;
    for (Guide* guide: appWindow->guides) {
        result.append(guide->getGuide());
    }
    return result;
}
