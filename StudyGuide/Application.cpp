//
// Created by Jesse on 7 okt. 2023.
//

#include "Application.h"

#include <iostream>
#include <thread>
#include "Config.h"

#include "XmlParser.h"
#include "ui/dialogs/LoadGuide.h"
#include "ui/guide/Guide.h"

#ifdef WIN32

#include "WindowConsole.h"

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
        "%{threadid}"
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

    QDir logsDir(settings.value("logsDir", QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                                           "/logs").toString());
    if (!logsDir.exists())
        logsDir.mkpath(".");

    // get highest number.
    int highestLogNumber = 0;
   for(QFileInfo file : logsDir.entryInfoList( QDir::Files)){
      QString fileName =  file.baseName();
       fileName.replace("Log", "");
       int fileNumber = fileName.toInt();
       if(fileNumber > highestLogNumber)
           highestLogNumber = fileNumber;
   }
    logFile = std::unique_ptr<QFile>(new QFile(logsDir.path() + "/Log" + QString::number(highestLogNumber + 1) + ".txt"));
    if (!logFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        qFatal("Can't open log file!");

    qInstallMessageHandler(messageHandler);
    // Logger is installed, so let's put the version in it!
    qDebug() << "Version:" << Config.version;


    // themes
    {
        QString currentTheme = settings.value("Theme", "fusion_dark").toString();

        if (currentTheme == "fusion_dark") {
            setStyle("fusion");
            QPalette darkPalette;
            darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
            darkPalette.setColor(QPalette::WindowText, Qt::white);
            darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
            darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
            darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
            darkPalette.setColor(QPalette::ToolTipText, Qt::white);
            darkPalette.setColor(QPalette::Text, Qt::white);
            darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
            darkPalette.setColor(QPalette::ButtonText, Qt::white);
            darkPalette.setColor(QPalette::BrightText, Qt::red);
            darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
            darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
            darkPalette.setColor(QPalette::HighlightedText, Qt::black);
            setPalette(darkPalette);
        }
        if (currentTheme == "fusion_light") {
            setStyle("fusion");
            QPalette lightPalette;
            lightPalette.setColor(QPalette::Window, Qt::white);
            lightPalette.setColor(QPalette::WindowText, Qt::black);
            lightPalette.setColor(QPalette::Base, Qt::white);
            lightPalette.setColor(QPalette::AlternateBase, QColor(240, 240, 240));
            lightPalette.setColor(QPalette::ToolTipBase, Qt::white);
            lightPalette.setColor(QPalette::ToolTipText, Qt::black);
            lightPalette.setColor(QPalette::Text, Qt::black);
            lightPalette.setColor(QPalette::Button, QColor(240, 240, 240));
            lightPalette.setColor(QPalette::ButtonText, Qt::black);
            lightPalette.setColor(QPalette::BrightText, Qt::red);
            lightPalette.setColor(QPalette::Link, QColor(42, 130, 218));
            lightPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
            lightPalette.setColor(QPalette::HighlightedText, Qt::white);
            setPalette(lightPalette);
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

    appWindow = new MainWindow;
    appWindow->show();

    if (!commandLineParser.positionalArguments().isEmpty()) {
        const QStringList files = commandLineParser.positionalArguments();
        QVector<GuideData::Data> guides = XmlParser::readXml(files);
        for (const GuideData::Data guide: guides) {
            appWindow->processGuide(guide);
        }
    }

    // auto open all files.
    if (settings.value("AutoOpen", "1").toBool()) {
        QDir autoOpenDir = settings.value("AutoOpenDir",
                                          (QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                                           "/open guides")).toString();
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
                appWindow->processGuide(guide);
                loadGuide->increaseProgress();
            }
            delete loadGuide; // no need anymore, so bye!
        }
    }
}

Application::~Application() {
}

void Application::setLanguage(const QString&languageCode) {
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
    QProcess::startDetached(applicationFilePath());
    qDebug() << "Booted new instance";
    exit(0);
    qDebug() << "Exiting current instance";
}
