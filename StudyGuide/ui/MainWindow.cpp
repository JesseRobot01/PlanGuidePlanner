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

#include "guide/Guide.h"
#include "ui/dialogs/LoadGuide.h"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionPreference_triggered() {
    preferenceWindow = new PreferenceWindow();
    preferenceWindow->show();
}

void MainWindow::on_actionOpen_File_triggered() {
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
        processGuide(guide);
        loadGuide->increaseProgress();
    }

    // Copy them over to auto open dir
    if (settings.value("AutoOpen", "1").toBool() && settings.value("AutoCopyGuide", "1").toBool()) {
        QDir copyToDestination = settings.value("AutoOpenDir",
                                                (QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                                                 "/open guides")).toString();

        if (copyToDestination.mkpath(".")) {
            for (QFile fileToCopy: files) {
                QFileInfo fileInfo(fileToCopy);
                if (!copyToDestination.exists(fileInfo.fileName())) {
                    if (fileToCopy.copy(copyToDestination.filePath(fileInfo.fileName()))) {
                        qDebug() << "Succesfully copied over file" << fileInfo.fileName() << "to auto open directory.";
                    }
                    else {
                        qCritical() << "Failed to copy over file" << fileInfo.fileName();
                    }
                }
                else
                    qWarning() << "File already copied over.";
                loadGuide->increaseProgress();
            }
        }
        else {
            qCritical() << "Failed to create auto open dir.";
        }
    }
    delete loadGuide;
}

void MainWindow::processGuide(GuideData::Data guide) {
    // default things
    Guide* finalGuide = new Guide(this);
    QString name = guide.name;
    QString shortName = guide.shortName;
    finalGuide->setName(name);
    finalGuide->setShortName(shortName);
    finalGuide->setInfo(guide.info);
    finalGuide->setPeriod(guide.period);


    //Processing the guide objects
    for (GuideData::GuideObject guideObject: guide.objects) {
        if (guideObject.objectType == GuideData::Index) {
            Index* index = new Index(finalGuide);
            for (GuideData::GuideGoals goal: guideObject.goals) {
                Goal* finalGoal = new Goal(index);
                finalGoal->setName(goal.name);
                finalGoal->setProgress(goal.progress);
                finalGoal->setTime(goal.time);
                finalGoal->setWeek(goal.week);
                finalGoal->setGoalNumber(goal.goalNumber);


                for (GuideData::GuideGoalPrefixes goalPrefix: goal.prefixes) {
                    switch (goalPrefix.prefix) {
                        case GuideData::Work:
                            finalGoal->addWork(goalPrefix.prefixText,goalPrefix.link);
                            break;
                        case GuideData::Read:
                            finalGoal->addRead(goalPrefix.prefixText,goalPrefix.link);
                            break;
                        case GuideData::Watch:
                            finalGoal->addWatch(goalPrefix.prefixText,goalPrefix.link);
                            break;
                        case GuideData::Process:
                            finalGoal->addProcess(goalPrefix.prefixText,goalPrefix.link);
                            break;

                        case GuideData::Info:
                            finalGoal->addInfo(goalPrefix.prefixText,goalPrefix.link);
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
}

void MainWindow::addGuide(Guide* guide, const QString&name) {
    guides.append(guide);
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidget(guide);
    ui->tabWidget->addTab(scrollArea, name);
}

void MainWindow::on_actionSave_Guide_As_triggered() {
    QSettings settings;
    Guide* guideToSave = guides.at(ui->tabWidget->currentIndex());

    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Open StudyGuide"),
                                                        settings.value("LastOpenedDir", ".").toString() + "/" +
                                                        guideToSave->
                                                        name, tr("XML Files (*.xml);;All Files (*)"));

    if (saveFileName.isEmpty()) {
        qWarning() << "No save file given. Can't save";
        return;
    }

    QFile fileToSave(saveFileName);
    //get the guide;
    GuideData::Data guide = guideToSave->getGuide();
    XmlParser::saveXml(guide, fileToSave);
}

void MainWindow::on_actionAbout_triggered() {
    AboutWindow* aboutWindow = new AboutWindow();
    aboutWindow->show();
}
