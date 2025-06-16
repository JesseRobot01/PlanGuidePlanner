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
    preferenceWindow = new PreferenceWindow();
    preferenceWindow->show();
}

#ifdef Q_OS_WASM

// for web, not as fancy as the original
void MainWindow::on_actionOpen_File_triggered() {
    auto uploadedGuide = [this](const QString &fileName, const QByteArray &fileContent) {
        if (fileName.isEmpty()) {
            qWarning() << "No file was given, returning";
            return;
        } else {
            QFile file("/tmp/guide.xml");

            if (file.open(QIODevice::WriteOnly)) {
                file.write(fileContent);
                file.close();
                GuideData::Data guide = XmlParser::readXml(&file);
                processGuide(guide);

            } else {
                qFatal() << "Cannot open file!";
                return;
            }

        }
    };

    QFileDialog::getOpenFileContent(tr("XML Files (*.xml);;All Files (*)"), uploadedGuide);
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
                                                      tr("XML Files (*.xml);;All Files (*)"));

    if (files.empty()) return;


    QFileInfo lastOpenedPath(files.at(0));

    settings.setValue("LastOpenedDir", lastOpenedPath.path());

    LoadGuide* loadGuide = new LoadGuide(
        nullptr,
        files.count() * (settings.value("AutoOpen", "1").toBool() && settings.value("AutoCopyGuide", "1").toBool()
                             ? 3
                             : 2));
    // the * 2 (or * 3) is for reading, Opening and copying over the guides

    loadGuide->show();

    QVector<GuideData::Data> guides;

    guides = XmlParser::readXml(files);
    loadGuide->increaseProgress(files.count());


    for (GuideData::Data guide: guides) {
        processGuide(guide, true);
        loadGuide->increaseProgress();
        if (settings.value("AutoOpen", "1").toBool() && settings.value("AutoCopyGuide", "1").toBool()) {
            QDir copyToDestination(APPLICATION->getAutoOpenLocation());

            if (copyToDestination.mkpath(".")) {
                QFile autoSaveFile(copyToDestination.filePath(guide.name + ".xml"));
                XmlParser::saveXml(guide, autoSaveFile, true, false);
                loadGuide->increaseProgress();
            }
            else {
                qCritical() << "Failed to create auto open dir.";
            }
        }
    }


    // Copy them over to auto open dir

    delete loadGuide;
}

#endif


void MainWindow::processGuide(GuideData::Data guide, bool updateStart) {
    // default things
    Guide* finalGuide = new Guide(this);
    QString name = guide.name;
    QString shortName = guide.shortName;
    finalGuide->setName(name);
    finalGuide->setShortName(shortName);
    finalGuide->setInfo(guide.info);
    finalGuide->setPeriod(guide.period);
    finalGuide->originalFile = guide.originalFile;
    finalGuide->autoSaveFile = guide.autoSaveFile;


    //Processing the guide objects
    for (GuideData::GuideObject guideObject: guide.objects) {
        if (guideObject.objectType == GuideData::Index) {
            Index* index = new Index(finalGuide);
            for (GuideData::GuideGoals goal: guideObject.goals) {
                Goal* finalGoal = new Goal(index);
                finalGoal->setName(goal.name);
                finalGoal->setProgress(goal.progress, false);
                finalGoal->setTime(goal.time);
                finalGoal->setWeek(goal.week);
                finalGoal->setGoalNumber(goal.goalNumber);
                finalGoal->parentGuide = finalGuide;

                for (GuideData::GuideGoalPrefixes goalPrefix: goal.prefixes) {
                    switch (goalPrefix.prefix) {
                        case GuideData::Work:
                            finalGoal->addWork(goalPrefix.prefixText, goalPrefix.link);
                            break;
                        case GuideData::Read:
                            finalGoal->addRead(goalPrefix.prefixText, goalPrefix.link);
                            break;
                        case GuideData::Watch:
                            finalGoal->addWatch(goalPrefix.prefixText, goalPrefix.link);
                            break;
                        case GuideData::Process:
                            finalGoal->addProcess(goalPrefix.prefixText, goalPrefix.link);
                            break;

                        case GuideData::Info:
                            finalGoal->addInfo(goalPrefix.prefixText, goalPrefix.link);
                            break;
                    }
                }
                finalGoal->finalise();
                index->addGoal(finalGoal);
            }
            index->finalise();
            finalGuide->addIndex(index);
        }
        if (guideObject.objectType == GuideData::Test) {
            Test* test = new Test(finalGuide);
            test->setName(guideObject.name);
            test->setWeek(guideObject.week);
            test->setInfo(guideObject.info);
            test->setShortName(guideObject.shortName);
            test->finalise();
            finalGuide->addTest(test);
        }
        if (guideObject.objectType == GuideData::Report) {
            Report* report = new Report(finalGuide);
            for (GuideData::ReportTests test: guideObject.tests)
                report->addTest(test.name, test.weight);
            report->finalise();
            finalGuide->addReport(report);
        }
    }


    addGuide(finalGuide, shortName.isEmpty() ? name : shortName);

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
    Guide *guideToSave = guides.at(ui->guideSwitcher->currentIndex());
    GuideData::Data guide = guideToSave->getGuide();

    QFile tmpFile("/tmp/savingGuide.xml");

    XmlParser::saveXml(guide, tmpFile);
    if (tmpFile.open(QIODevice::ReadOnly)) {
        QByteArray fileContent(tmpFile.readAll());

        QFileDialog::saveFileContent(fileContent, QString("%1.xml").arg(guide.name));
    } else {
        qFatal() << "Error while saving. Can't open file.";
    }
}

#else

void MainWindow::on_actionSave_Guide_As_triggered() {
    Guide* guideToSave = guides.at(ui->guideSwitcher->currentIndex());
    GuideData::Data guide = guideToSave->getGuide();

    saveGuideAs(guide);
}

#endif

void MainWindow::on_actionSave_triggered() {
    Guide* guideToSave = guides.at(ui->guideSwitcher->currentIndex());
    GuideData::Data guide = guideToSave->getGuide();
}

void MainWindow::saveGuide(GuideData::Data guide) {
    QString saveFileName = guide.originalFile.filePath();

    if (saveFileName.isEmpty()) {
        saveGuideAs(guide); //Trigger the save as.
        return;
    }

    QFile fileToSave(saveFileName);
    XmlParser::saveXml(guide, fileToSave);
}

void MainWindow::saveGuideAs(GuideData::Data guide) {
    QSettings settings;
    QString baseFileName;

    if (!guide.originalFile.exists())
        baseFileName = guide.originalFile.filePath();
    else
        baseFileName = settings.value("LastOpenedDir", ".").toString() + "/" + guide.name + ".xml";

    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Open StudyGuide"),
                                                        baseFileName, tr("XML Files (*.xml);;All Files (*)"));

    if (saveFileName.isEmpty()) {
        qWarning() << "No save file given. Can't save";
        return;
    }

    QFile fileToSave(saveFileName);
    XmlParser::saveXml(guide, fileToSave);
}

void MainWindow::on_actionAbout_triggered() {
    AboutWindow* aboutWindow = new AboutWindow();
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

void MainWindow::closeGuide(int guideIndex) {
    Guide* guideToClose = guides.at(guideIndex);
    qDebug() << "Closing Guide " << guideToClose->name;
    GuideData::Data guide = guideToClose->getGuide();

    // first of all, save it.
    saveGuide(guide);

    // next, delete the auto save file.
    QFile autoSaveFile(guide.autoSaveFile.filePath());
    autoSaveFile.remove();

    // and now, delete it from the program.
    ui->guideSwitcher->removeTab(guideIndex + 1);
    guides.removeAt(guideIndex);
    qDebug() << "Guide closed.";

    //update start
    updateStart();
}

void MainWindow::updateStart() {
    startScreen->updateStart();
}

void MainWindow::on_guideSwitcher_currentChanged(int tab) {
    if (tab != 0) {
        ui->actionClose_guide->setEnabled(true);
        return;
    }

    if (tab == 0) {
        ui->actionClose_guide->setEnabled(false);
    }
    if (APPLICATION->isAutoSaveTimerStarted) //todo: deticated boolean for a change
    {
        updateStart();
    }
}

void MainWindow::on_actionClose_guide_triggered() {
    int currentTab = ui->guideSwitcher->currentIndex();

    if (currentTab == 0) {
        qWarning() << "Can't close start screen, returning.";
        return;
    }

    closeGuide(currentTab - 1);
}

void MainWindow::on_actionClose_all_guides_triggered() {
    // get guide count
    int guideCount = guides.size();
    for (int i = 0; i < guideCount; i++) {
        // each time the index shifts, so always close 0
        closeGuide(0);
    }
}
