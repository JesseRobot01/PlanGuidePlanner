//
// Created by Jesse on 7 okt. 2023.
//

#include "Application.h"

#include <iostream>
#include <thread>
#include <QProcess>
#include <QCommandLineParser>

#include "XmlParser.h"
#include "ui/dialogs/LoadGuide.h"
#include "ui/guide/Guide.h"

#ifdef WIN32

#include "WindowConsole.h"

#endif

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    static std::mutex loggerMutex;
    const std::lock_guard<std::mutex> lock(loggerMutex); // synchronized, QFile logFile is not thread-safe

    QString out = qFormatLogMessage(type, context, msg);
    out += QChar::LineFeed;

    QTextStream(stderr) << out.toLocal8Bit();
    APPLICATION->logFile->write(out.toUtf8());
    APPLICATION->logFile->flush();
    fflush(stderr);
}

Application::Application(int &argc, char **argv) : QApplication(argc, argv) {
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
            "%{if-debug}DEBUG   %{endif}"
            "%{if-info}INFO    %{endif}"
            "%{if-warning}WARNING %{endif}"
            "%{if-critical}CRITICAL%{endif}"
            "%{if-fatal}FATAL   %{endif}"
            " "
            "|"
            " "
            "%{message}");

    QDir logsDir(settings.value("logDir", QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                                          "/logs").toString());
    if (!logsDir.exists())
        logsDir.mkpath(".");

    // get highest number.
    int latestLog = logsDir.entryList(QDir::Files).count();

    logFile = std::unique_ptr<QFile>(new QFile(logsDir.path() + "/Log" + QString::number(latestLog + 1) + ".txt"));
    if (!logFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        qFatal() << "Can't open log file!";

    qInstallMessageHandler(messageHandler);

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
            LoadGuide *loadGuide = new LoadGuide(nullptr, guideFileNames.count() * 2); // 1 for reading, 1 for opening.
            loadGuide->show();

            for (const QString &GuideFileName: guideFileNames)
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

void Application::setLanguage(const QString &languageCode) {
    if (translator->load(":/translations/StudyGuide_" + languageCode + ".qm")) {
        installTranslator(translator);
        qDebug() << "Succesfully loaded translations for" << languageCode;
    } else {
        qCritical() << "Failed to load translation for" << languageCode;
        qInfo() << "Falling back to default translations.";

        if (translator->load(":/translations/StudyGuide_en.qm")) {
            installTranslator(translator);
        } else {
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
