//
// Created by Jesse on 30 aug. 2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <QFileDialog>
#include <guide/GuideData.h>

#include "MainWindow.h"

#include "AboutWindow.h"
#include "ui_MainWindow.h"
#include "XmlParser.h"
#include "guide/Index.h"
#include "guide/Test.h"
#include "guide/Report.h"
#include "guide/Guide.h"
#include "ui/dialogs/LoadGuide.h"
#include "Application.h"
#include "ui/guide/Goal.h"
#include <QCloseEvent>
#include <JlCompress.h>
#include <QScrollArea>

#include "creator/Creator.h"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // poke in start screen
    startScreen = new StartScreen(this);
    startScreen->show();
    ui->guideSwitcher->addTab(startScreen, tr("Start"));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionPreference_triggered() {
    preferenceWindow = new PreferenceWindow(this);
    preferenceWindow->show();
}

#ifdef Q_OS_WASM
// for web, not as fancy as the original
void MainWindow::on_actionOpen_File_triggered() {
    auto uploadedGuide = [this](const QString&fileName, const QByteArray&fileContent) {
        if (fileName.isEmpty()) {
            qWarning() << "No file was given, returning";
            return;
        }
        else {
            QDir tempDir(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
            QFile file("/tmp/guide");

            if (file.open(QIODevice::ReadWrite)) {
                file.write(fileContent);

                if (fileName.endsWith("zip")) {
                    QDir tempDir(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
                    tempDir.mkpath(".");

                    QStringList extractedFiles = JlCompress::extractDir(file.fileName(), tempDir.absolutePath());

                    for (const QString&extractedFile: extractedFiles) {
                        qDebug() << "Extracted:" << extractedFile;
                        if (extractedFile.endsWith("xml")) {
                            GuideData::Data guide = XmlParser::readXml(extractedFile);
                            processGuide(guide);
                        }
                    }
                }
                else if (fileName.endsWith(".xml")) {
                    file.close(); // Parser does not like open files.
                    GuideData::Data guide = XmlParser::readXml(&file);
                    processGuide(guide);
                }
                file.close();
            }
        }
    };

    QFileDialog::getOpenFileContent(
        tr("All Supported Files (*.xml *.zip);;*.Xml Files (*.xml);;Zip Files (*.zip);;All Files (*)"),
        uploadedGuide);
}
#else

void MainWindow::on_actionOpen_File_triggered() {
#ifdef Q_OS_ANDROID
    // make sure to have permission to storage
    APPLICATION->requestStoragePermission();
#endif
    QSettings settings;

    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open StudyGuide"),
                                                      settings.value("LastOpenedDir", ".").toString(),
                                                      tr(
                                                          "All Supported Files (*.xml *.zip);;*.Xml Files (*.xml);;Zip Files (*.zip);;All Files (*)"));

    if (files.empty()) return;


    QFileInfo lastOpenedPath(files.at(0));

    settings.setValue("LastOpenedDir", lastOpenedPath.path());

    QStringList guideFiles;
    QDir tempDir(QStandardPaths::writableLocation(QStandardPaths::TempLocation));

    // Determine if there are any zip files
    for (auto file: files) {
        if (file.endsWith("zip")) {
            qDebug() << "Found zip file:" << file << ". Extracting...";

            QStringList extractedFiles = JlCompress::extractDir(file, tempDir.absolutePath());

            for (const QString&extractedFile: extractedFiles) {
                qDebug() << "Extracted:" << file;
                if (extractedFile.endsWith("xml"))
                    guideFiles.append(extractedFile);
            }
        }
        else if (file.endsWith("xml"))
            guideFiles.append(file);
    }

    LoadGuide* loadGuide = new LoadGuide(
        this,
        guideFiles.count() * 3);
    // the * 3 is for reading, Opening and copying over the guides

    loadGuide->show();

    QVector<GuideData::Data> guides;

    guides = XmlParser::readXml(guideFiles);
    loadGuide->increaseProgress(guideFiles.count());


    for (GuideData::Data guide: guides) {
        QDir copyToDestination(APPLICATION->getAutoSaveLocation());

        if (copyToDestination.mkpath(".")) {
            QFileInfo autoSaveFile(copyToDestination.filePath(guide.shortName + "_0.xml"));


            QString baseName = guide.shortName;
            QString candidateName = baseName + "_0.xml";
            QString fullPath = copyToDestination.absoluteFilePath(candidateName);

            int number = 1; // Incase there are duplicates

            // Check if the file already exists and increment the counter
            while (QFile::exists(fullPath)) {
                candidateName = baseName + "_" + QString::number(number++) + ".xml";
                fullPath = copyToDestination.absoluteFilePath(candidateName); // Recalculate path with new number
            }

            autoSaveFile = QFileInfo(fullPath);

            QFile fileToSave(autoSaveFile.absoluteFilePath());

            XmlParser::saveXml(guide, fileToSave, true, false);

            loadGuide->increaseProgress();

            guide.autoSaveFile = autoSaveFile;
            processGuide(guide, false);
            loadGuide->increaseProgress();
        }
        else {
            qCritical() << "Failed to create auto open dir.";
        }
    }

    // Empty tmp dir
    if (tempDir.exists()) {
        tempDir.removeRecursively();
    }

    //update start screen
    updateStart();
    delete loadGuide;
}

#endif


void MainWindow::processGuide(GuideData::Data guide, bool updateStart) {
    Guide* finalGuide = new Guide(this);

    finalGuide->setGuide(guide);
    addGuide(finalGuide, guide.shortName.isEmpty() ? guide.name : guide.shortName);

    //update start
    if (updateStart)
        startScreen->updateStart();
}

void MainWindow::addGuide(Guide* guide, const QString&name) {
    guides.append(guide);
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidget(guide);
    ui->guideSwitcher->addTab(scrollArea, name);
}

#ifdef Q_OS_WASM
void MainWindow::on_actionSave_Guide_As_triggered() {
    int currentTab = (ui->guideSwitcher->currentIndex() - 1);
    Guide* guideToSave = guides.at(currentTab);
    GuideData::Data guide = guideToSave->getGuide();

    QFile tmpFile("/tmp/savingGuide.xml");

    XmlParser::saveXml(guide, tmpFile);
    if (tmpFile.open(QIODevice::ReadOnly)) {
        QByteArray fileContent(tmpFile.readAll());

        QFileDialog::saveFileContent(fileContent, QString("%1.xml").arg(guide.name));

        closeGuide(currentTab);
    }
    else {
        qFatal() << "Error while saving. Can't open file.";
    }
}

#else

void MainWindow::on_actionSave_Guide_As_triggered() {
    int currentTab(ui->guideSwitcher->currentIndex() - 1);
    Guide* guideToSave = guides.at(currentTab);
    GuideData::Data guide = guideToSave->getGuide();

    saveGuideAs(guide);

    //close it!
    closeGuide(currentTab);
}

#endif

void MainWindow::saveGuideAs(GuideData::Data guide) {
    QSettings settings;
    QString baseFileName;

    if (guide.originalFile.exists())
        baseFileName = guide.originalFile.baseName();
    else
        baseFileName = settings.value("LastOpenedDir", ".").toString() + "/" + guide.name + ".xml";

    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save StudyGuide"),
                                                        baseFileName, tr("XML Files (*.xml);;All Files (*)"));

    if (saveFileName.isEmpty()) {
        qWarning() << "No save file given. Can't save";
        return;
    }

    QFile fileToSave(saveFileName);
    XmlParser::saveXml(guide, fileToSave);
}

#ifdef Q_OS_WASM
void MainWindow::on_actionSave_All_Guides_triggered() {
    //Saving everything to a temp location
    QDir tempLocation = (QStandardPaths::writableLocation(QStandardPaths::TempLocation));
    QStringList GuidesToSave;

    for (auto* guide: guides) {
        GuideData::Data guideDat = guide->getGuide();

        QFileInfo fileInfoToSave(tempLocation.absoluteFilePath(guideDat.shortName + "_0.xml"));

        QString baseName = guideDat.shortName;
        QString candidateName = baseName + ".xml";
        QString fullPath = tempLocation.absoluteFilePath(candidateName);

        int number = 1; // Incase there are duplicates

        // Check if the file already exists and increment the counter
        while (QFile::exists(fullPath)) {
            candidateName = baseName + "_" + QString::number(number++) + ".xml";
            fullPath = tempLocation.absoluteFilePath(candidateName); // Recalculate path with new number
        }

        fileInfoToSave = QFileInfo(fullPath);

        QFile fileToSave(fileInfoToSave.absoluteFilePath());
        XmlParser::saveXml(guideDat, fileToSave);

        GuidesToSave.append(fileToSave.fileName());
    }

    // Zip them!
    QFileInfo exportFileInfo(tempLocation.filePath("export.zip"));
    bool success = JlCompress::compressFiles(exportFileInfo.absoluteFilePath(), GuidesToSave);
    if (success) {
        qDebug() << "Files zipped successfully!";
    }
    else {
        qCritical() << "Failed to zop files!";
    }

    // Download
    QFile exportFile(exportFileInfo.absoluteFilePath());
    exportFile.open(QIODevice::ReadOnly);
    QByteArray fileContent(exportFile.readAll());
    exportFile.close();
    QDateTime time;
    QFileDialog::saveFileContent(fileContent, QString("%1.zip").arg(time.currentDateTime().toString()));

    //close all
    int guideCount = guides.size();
    for (int i = 0; i < guideCount; i++) {
        // each time the index shifts, so always close 0
        closeGuide(0, false);
    }

    //update start
    updateStart();
}
#else
void MainWindow::on_actionSave_All_Guides_triggered() {
    QSettings settings;
    QDateTime time;
    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save All Guides"),
                                                        time.currentDateTime().toString() + ".zip",
                                                        tr("ZIP Files (*.zip);;All Files (*)"));

    if (saveFileName.isEmpty()) {
        qWarning() << "No save file given. Can't save";
        return;
    }

    QStringList GuidesToSave;

    QDir tempLocation = (QStandardPaths::writableLocation(QStandardPaths::TempLocation));

    for (auto* guide: guides) {
        GuideData::Data guideDat = guide->getGuide();

        // saving
        QFileInfo fileInfoToSave(tempLocation.absoluteFilePath(guideDat.shortName + "_0.xml"));

        QString baseName = guideDat.shortName;
        QString candidateName = baseName + ".xml";
        QString fullPath = tempLocation.absoluteFilePath(candidateName);

        int number = 1; // Incase there are duplicates

        // Check if the file already exists and increment the counter
        while (QFile::exists(fullPath)) {
            candidateName = baseName + "_" + QString::number(number++) + ".xml";
            fullPath = tempLocation.absoluteFilePath(candidateName); // Recalculate path with new number
        }

        fileInfoToSave = QFileInfo(fullPath);

        QFile fileToSave(fileInfoToSave.absoluteFilePath());
        XmlParser::saveXml(guideDat, fileToSave);

        GuidesToSave.append(fileToSave.fileName());
    }

    // Zip them!
    bool success = JlCompress::compressFiles(saveFileName, GuidesToSave);
    if (success) {
        qDebug() << "Files zipped successfully!";
    }
    else {
        qCritical() << "Failed to zop files!";
    }


    // Cleanup!
    if (tempLocation.exists()) {
        tempLocation.removeRecursively();
    }

    // Now close them!
    // get guide count
    int guideCount = guides.size();
    for (int i = 0; i < guideCount; i++) {
        // each time the index shifts, so always close 0
        closeGuide(0, false);
    }

    //update start
    updateStart();
}
#endif
void MainWindow::on_actionAbout_triggered() {
    AboutWindow* aboutWindow = new AboutWindow(this);
    aboutWindow->show();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (APPLICATION->isAutoSaveTimerStarted) {
        // force auto save
        APPLICATION->autoSaveTriggered();
    }
    event->accept();
}

void MainWindow::on_guideSwitcher_tabCloseRequested(int tab) {
    qDebug() << "Closing guide...";
    if (tab == 0) {
        qWarning() << "Can't close start screen, returning.";
        return;
    }
    closeGuide(tab - 1);
}

void MainWindow::closeGuide(int guideIndex, bool updateStartBool) {
    Guide* guideToClose = guides.at(guideIndex);
    qDebug() << "Closing Guide " << guideToClose->name;
    GuideData::Data guide = guideToClose->getGuide();

    // next, delete the auto save file.
    QFile autoSaveFile(guide.autoSaveFile.filePath());
    autoSaveFile.remove();

    // and now, delete it from the program.
    ui->guideSwitcher->removeTab(guideIndex + 1);
    guides.removeAt(guideIndex);
    qDebug() << "Guide closed.";

    //update start
    if (updateStartBool)
        updateStart();
}

void MainWindow::setTabName(int tab, QString name) {
    ui->guideSwitcher->setTabText(tab, name);
}

void MainWindow::updateStart() {
    startScreen->updateStart();
}

void MainWindow::on_guideSwitcher_currentChanged(int tab) {
    if (tab != 0) {
        ui->actionSave_Guide_As->setEnabled(true);
        ui->actionOpen_In_Creator->setEnabled(true);
        return;
    }

    if (tab == 0) {
        ui->actionSave_Guide_As->setEnabled(false);
        ui->actionOpen_In_Creator->setEnabled(false);
        if (APPLICATION->isFileChanged) {
            updateStart();
        }
    }
}

void MainWindow::on_actionOpen_Creator_triggered() {
    Creator* creator = new Creator();
    creator
            ->show();
                     // dafoque
}

void MainWindow::on_actionOpen_In_Creator_triggered() {
    // First of all, save everything
    APPLICATION->autoSaveTriggered();

    // Now get the guide to open.
    int currentGuideIndex(ui->guideSwitcher->currentIndex() - 1);
    GuideData::Data currentGuide = guides.at(currentGuideIndex)->getGuide();

    // Open in creator
    Creator* creator = new Creator();
    creator->open(currentGuide);
    creator->currentGuide = currentGuide.originalFile;
    creator->appAutoSaveLocation = currentGuide.autoSaveFile;
    creator->applicationGuideIndex = currentGuideIndex;
    creator->show();
}
