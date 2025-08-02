//
// Created by Jesse on 16-06-2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_CreatorMainWindow.h" resolved

#include "Creator.h"

#include <QFileDialog>

#include "ui_Creator.h"
#include "guide/GuideData.h"
#include <ui/guide/Guide.h>

#include "Application.h"
#include "XmlParser.h"


Creator::Creator(QWidget* parent) : QMainWindow(parent), ui(new Ui::Creator) {
    ui->setupUi(this);

    addTask = new QListWidgetItem(ui->addList);
    addTask->setText("Add Goal Task");

    addReportTest = new QListWidgetItem(ui->addList);
    addReportTest->setText("Add Test To Report");

    listSpacer = new QListWidgetItem(ui->addList);

    addGoal = new QListWidgetItem(ui->addList);
    addGoal->setText("Add Goal");

    addTest = new QListWidgetItem(ui->addList);
    addTest->setText("Add Test");

    addReport = new QListWidgetItem(ui->addList);
    addReport->setText("Add Report");


    //Restore to initial configuration
    ui->typeSelector->setEnabled(false);
    ui->shortEdit->setEnabled(false);
    ui->extraEdit->setEnabled(false);
    ui->longEdit->setEnabled(false);

    ui->typeSelector->hide();
    ui->shortEdit->hide();
    ui->extraEdit->hide();
    ui->longEdit->hide();

    ui->typeSelectorLabel->hide();
    ui->shortEditLabel->hide();
    ui->extraEditLabel->hide();
    ui->longEditLabel->hide();

    addTask->setHidden(true);
    addReportTest->setHidden(true);
    listSpacer->setHidden(true);
}

Creator::~Creator() {
    delete ui;
}


void Creator::hideAddTask() {
    addTask->setHidden(true);
    listSpacer->setHidden(true);
}

void Creator::showAddTask() {
    addTask->setHidden(false);
    listSpacer->setHidden(false);
}

void Creator::hideAddReportTest() {
    addReportTest->setHidden(true);
    listSpacer->setHidden(true);
}

void Creator::showAddReportTest() {
    addReportTest->setHidden(false);
    listSpacer->setHidden(false);
}

void Creator::hideTypeSelector() {
    ui->typeSelector->setEnabled(false);
    ui->typeSelector->hide();
    ui->typeSelectorLabel->hide();
}

void Creator::showTypeSelector(QString text) {
    ui->typeSelector->setCurrentText(text);
    ui->typeSelector->setEnabled(true);
    ui->typeSelector->show();
    ui->typeSelectorLabel->show();
}

void Creator::hideShortEdit() {
    ui->shortEdit->setEnabled(false);
    ui->shortEdit->hide();
    ui->shortEditLabel->hide();
}

void Creator::showShortEdit(QString text) {
    ui->shortEdit->setText(text);
    ui->shortEdit->setEnabled(true);
    ui->shortEdit->show();
    ui->shortEditLabel->show();
}

void Creator::hideExtraEdit() {
    ui->extraEdit->setEnabled(false);
    ui->extraEdit->hide();
    ui->extraEditLabel->hide();
}

void Creator::showExtraEdit(QString text) {
    ui->extraEdit->setText(text);
    ui->extraEdit->setEnabled(true);
    ui->extraEdit->show();
    ui->extraEditLabel->show();
}

void Creator::hideLongEdit() {
    ui->longEdit->setEnabled(false);
    ui->longEdit->hide();
    ui->longEditLabel->hide();
}

void Creator::showLongEdit(QString text) {
    ui->longEdit->setPlainText(text);
    ui->longEdit->setEnabled(true);
    ui->longEdit->show();
    ui->longEditLabel->show();
}

void Creator::on_mainDisplay_itemClicked(QTreeWidgetItem* item, int column) {
    //goals
    if (item->text(0) == tr("Goal")) {
        hideAddReportTest();
        hideTypeSelector();
        hideLongEdit();

        //goal
        showShortEdit(item->text(1));
        showExtraEdit(item->text(2));
        showAddTask();
    }
    // For the goal prefixes
    else if (item->text(0) == tr("Work") ||
             item->text(0) == tr("Watch/ Listen") ||
             item->text(0) == tr("Read") ||
             item->text(0) == tr("Process") ||
             item->text(0) == tr("Information") &&
             item->parent()->text(0) == tr("Goal")) // big if :)

    {
        hideAddReportTest();
        hideLongEdit();

        showTypeSelector(item->text(0));
        showShortEdit(item->text(1));
        showExtraEdit(item->text(2));
        showAddTask();
    }
    else if (item->text(0) == tr("Time")) {
        //goal time
        hideAddReportTest();
        hideTypeSelector();
        hideExtraEdit();
        hideLongEdit();

        showShortEdit(item->text(1));
        showAddTask();
    }
    else if (item->text(0) == tr("Week")) {
        //goal week (also works with test)
        hideAddReportTest();
        hideTypeSelector();
        hideExtraEdit();
        hideLongEdit();

        showShortEdit(item->text(1));

        if (item->parent()->text(0) == tr("Goal")) {
            showAddTask();
        }
        else {
            hideAddTask();
        }
    }
    else if (item->text(0) == tr("Progress")) {
        //goal progress
        hideAddReportTest();
        hideTypeSelector();
        hideExtraEdit();
        hideLongEdit();

        showShortEdit(item->text(1));
        showAddTask();
    } // Test
    else if (item->text(0) == tr("Test")) {
        //Main Test (also works with report test)
        hideAddTask();
        hideTypeSelector();
        hideLongEdit();

        showShortEdit(item->text(1));
        showExtraEdit(item->text(2));

        if (item->parent() != nullptr) {
            showAddReportTest();
        }
        else {
            hideAddReportTest();
        }
    }
    // report
    else if (item->text(0) == tr("Report")) {
        hideAddTask();
        hideTypeSelector();
        hideShortEdit();
        hideExtraEdit();
        hideLongEdit();

        showAddReportTest();
    }
    else if (item->text(0) == tr("Information") && item->parent()->text(0) != tr("Goal")) {
        // Also used for the main info
        hideAddTask();
        hideAddReportTest();
        hideTypeSelector();
        hideShortEdit();
        hideExtraEdit();

        showLongEdit(item->text(1));
    }
    //Main Guide
    else if (item->text(0) == tr("Guide")) {
        hideAddTask();
        hideAddReportTest();
        hideTypeSelector();
        hideLongEdit();

        showShortEdit(item->text(1));
        showExtraEdit(item->text(2));
    }
    else if (item->text(0) == tr("Period")) {
        hideAddReportTest();
        hideTypeSelector();
        hideAddTask();
        hideExtraEdit();
        hideLongEdit();

        showShortEdit(item->text(1));
    }
}

void Creator::on_shortEdit_editingFinished() {
    QTreeWidgetItem* currentItem = ui->mainDisplay->currentItem();
    currentItem->setText(1, ui->shortEdit->text());
}

void Creator::on_typeSelector_currentTextChanged(QString string) {
    QTreeWidgetItem* currentItem = ui->mainDisplay->currentItem();

    currentItem->setText(0, string);
}

void Creator::on_displayButton_clicked() {
    Guide* guide = new Guide();

    guide->setGuide(getCurrentGuide());
    guide->show();
}

GuideData::Data Creator::getCurrentGuide() {
    GuideData::Data guidedata;
    GuideData::GuideObject* index = nullptr;

    for (int i = 0; i < ui->mainDisplay->topLevelItemCount(); ++i) {
        QTreeWidgetItem* item = ui->mainDisplay->topLevelItem(i);

        if (item->text(0) == tr("Goal")) {
            if (index == nullptr) {
                index = new GuideData::GuideObject();
                index->objectType = GuideData::Index;
            }

            GuideData::GuideGoals goal;

            goal.name = item->text(1);
            goal.goalNumber = item->text(2);
            for (int j = 0; j < item->childCount(); ++j) {
                QTreeWidgetItem* child = item->child(j);

                if (child->text(0) == tr("Work"))
                    goal.addWork(child->text(1), child->text(2));

                else if (child->text(0) == tr("Read"))
                    goal.addRead(child->text(1), child->text(2));

                else if (child->text(0) == tr("Watch/ Listen"))
                    goal.addWatch(child->text(1), child->text(2));

                else if (child->text(0) == tr("Process"))
                    goal.addProcess(child->text(1), child->text(2));

                else if (child->text(0) == tr("Information"))
                    goal.addInfo(child->text(1), child->text(2));

                else if (child->text(0) == tr("Week"))
                    goal.week = child->text(1);

                else if (child->text(0) == tr("Progress"))
                    goal.progress = child->text(1);

                else if (child->text(0) == tr("Time"))
                    goal.time = child->text(1);
            }

            index->goals.append(goal);
            continue;
        }
        else {
            if (index != nullptr) {
                guidedata.objects.append(*index);
                index = nullptr;
            }
        }

        if (item->text(0) == tr("Test")) {
            GuideData::GuideObject test;
            test.objectType = GuideData::Test;
            test.setTestName(item->text(1));
            test.shortName = (item->text(2));

            for (int j = 0; j < item->childCount(); ++j) {
                QTreeWidgetItem* child = item->child(j);

                if (child->text(0) == tr("Week"))
                    test.week = child->text(1);

                else if (child->text(0) == tr("Information"))
                    test.info = child->text(1);
            }

            guidedata.objects.append(test);
            continue;
        }

        if (item->text(0) == tr("Report")) {
            GuideData::GuideObject report;
            report.objectType = GuideData::Report;

            for (int j = 0; j < item->childCount(); ++j) {
                QTreeWidgetItem* child = item->child(j);
                if (child->text(0) == tr("Test")) {
                    GuideData::ReportTests reportTest;
                    reportTest.name = child->text(1);
                    reportTest.weight = child->text(2);

                    report.tests.append(reportTest);
                }
            }

            guidedata.objects.append(report);
        }

        if (item->text(0) == tr("Guide")) {
            guidedata.name = item->text(1);
            guidedata.shortName = item->text(2);

            for (int j = 0; j < item->childCount(); ++j) {
                QTreeWidgetItem* child = item->child(j);

                if (child->text(0) == tr("Period"))
                    guidedata.period = child->text(1);

                else if (child->text(0) == tr("Information"))
                    guidedata.info = child->text(1);
            }
            continue;
        }
    }

    if (index != nullptr) {
        guidedata.objects.append(*index);
        index = nullptr;
    }

    return guidedata;
}

void Creator::on_addList_itemDoubleClicked(QListWidgetItem* item) {
    if (item->text() == tr("Add Goal")) {
        QTreeWidgetItem* topItem = new QTreeWidgetItem(ui->mainDisplay);
        topItem->setText(0, tr("Goal"));

        QTreeWidgetItem* timeItem = new QTreeWidgetItem(topItem);
        timeItem->setText(0, tr("Time"));

        QTreeWidgetItem* weekItem = new QTreeWidgetItem(topItem);
        weekItem->setText(0, tr("Week"));

        QTreeWidgetItem* progressItem = new QTreeWidgetItem(topItem);
        progressItem->setText(0, tr("Progress"));
        progressItem->setText(1, "0");

        return;
    }

    if (item->text() == tr("Add Test")) {
        QTreeWidgetItem* topItem = new QTreeWidgetItem(ui->mainDisplay);
        topItem->setText(0, tr("Test"));

        QTreeWidgetItem* week = new QTreeWidgetItem(topItem);
        week->setText(0, tr("Week"));

        QTreeWidgetItem* information = new QTreeWidgetItem(topItem);
        information->setText(0, tr("Information"));

        return;
    }

    if (item->text() == tr("Add Report")) {
        QTreeWidgetItem* topItem = new QTreeWidgetItem(ui->mainDisplay);
        topItem->setText(0, tr("Report"));

        QTreeWidgetItem* testItem = new QTreeWidgetItem(topItem);
        testItem->setText(0, tr("Test"));

        return;
    }

    if (item->text() == tr("Add Goal Task")) {
        QTreeWidgetItem* topItem;
        //search for parent goal
        if (ui->mainDisplay->currentItem()->text(0) == tr("Goal")) {
            topItem = ui->mainDisplay->currentItem();
        }
        else topItem = ui->mainDisplay->currentItem()->parent();


        QTreeWidgetItem* goalTask = new QTreeWidgetItem(topItem);
        goalTask->setText(0, tr("Work"));

        return;
    }

    if (item->text() == tr("Add Test To Report")) {
        QTreeWidgetItem* topItem;
        //search for parent report
        if (ui->mainDisplay->currentItem()->text(0) == tr("Report")) {
            topItem = ui->mainDisplay->currentItem();
        }
        else topItem = ui->mainDisplay->currentItem()->parent();


        QTreeWidgetItem* reportTest = new QTreeWidgetItem(topItem);
        reportTest->setText(0, tr("Test"));

        return;
    }
}

void Creator::on_extraEdit_editingFinished() {
    QTreeWidgetItem* currentItem = ui->mainDisplay->currentItem();
    currentItem->setText(2, ui->extraEdit->text());
}

void Creator::on_longEdit_textChanged() {
    QTreeWidgetItem* currentItem = ui->mainDisplay->currentItem();
    currentItem->setText(1, ui->longEdit->toPlainText());
}

void Creator::on_actionOpen_Guide_triggered() {
    QSettings settings;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open StudyGuide"),
                                                    settings.value("LastOpenedDir", ".").toString(),
                                                    tr(
                                                        "All Supported Files (*.sgd *.sga *.xml);;StudyGuide Document (*.sgd);;StudyGuide Auto Save File (*.sga);;*.Xml Files (*.xml);;All Files (*)"));

    if (fileName.isEmpty()) return;
    QFileInfo file(fileName);
    settings.setValue("LastOpenedDir", file.path());

    // read guide
    QFile actualFile(file.absoluteFilePath());
    GuideData::Data guide = XmlParser::readXml(&actualFile);
    currentGuide = file;

    open(guide);
}

void Creator::on_actionSave_Guide_triggered() {
    // First of all, check if the guide is actually from the program itself.
    if (applicationGuideIndex != -1) {
        // First save to auto save
        GuideData::Data guide = getCurrentGuide();
        QFile currentAutoSaveFile(appAutoSaveLocation.absoluteFilePath());
        XmlParser::saveXml(guide, currentAutoSaveFile, true, false);

        if (currentGuide.exists()) {
            //If the original still exists, save to it!
            save(getCurrentGuide());
        }

        APPLICATION->updateGuide(applicationGuideIndex, guide);
        return;
    }
    else if (!currentGuide.exists()) {
        // Save as
        on_actionSave_Guide_As_triggered();
        return;
    }

    save(getCurrentGuide());
}

void Creator::on_actionSave_Guide_As_triggered() {
    QSettings settings;
    QString baseFileName;
    GuideData::Data guide = getCurrentGuide();

    if (currentGuide.exists())
        baseFileName = currentGuide.baseName();
    else
        baseFileName = settings.value("LastOpenedDir", ".").toString() + "/" + guide.name + ".xml";

    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save StudyGuide"),
                                                        baseFileName, tr("StudyGuide Document (*.sgd);;XML Files (*.xml);;All Files (*)"));

    if (saveFileName.isEmpty()) {
        qWarning() << "No save file given. Can't save";
        return;
    }
    currentGuide = QFileInfo(saveFileName);
    save(guide);
}

void Creator::save(GuideData::Data guide) {
    QFile currentGuideFile(currentGuide.absoluteFilePath());
    if (currentGuideFile.fileName().endsWith("sgd"))
    XmlParser::saveXml(guide, currentGuideFile, false, false);
}

void Creator::open(GuideData::Data guide) {
    auto* mainDisplay = ui->mainDisplay;

    // First of all, clear the current one
    mainDisplay->clear();

    //main config
    auto* baseData = new QTreeWidgetItem(mainDisplay);
    auto* period = new QTreeWidgetItem(baseData);
    auto* info = new QTreeWidgetItem(baseData);

    baseData->setText(0, tr("Guide"));
    period->setText(0, tr("Period"));
    info->setText(0, tr("Information"));

    baseData->setText(1, guide.name);
    baseData->setText(2, guide.shortName);
    period->setText(1, guide.period);
    info->setText(1, guide.info);

    for (auto object: guide.objects) {
        if (object.objectType == GuideData::Index) {
            for (auto goal: object.goals) {
                QTreeWidgetItem* goalItem = new QTreeWidgetItem(mainDisplay);
                goalItem->setText(0, tr("Goal"));
                goalItem->setText(1, goal.name);
                goalItem->setText(2, goal.goalNumber);


                QTreeWidgetItem* timeItem = new QTreeWidgetItem(goalItem);
                timeItem->setText(0, tr("Time"));
                timeItem->setText(1, goal.time);


                QTreeWidgetItem* weekItem = new QTreeWidgetItem(goalItem);
                weekItem->setText(0, tr("Week"));
                weekItem->setText(1, goal.week);


                QTreeWidgetItem* progressItem = new QTreeWidgetItem(goalItem);
                progressItem->setText(0, tr("Progress"));
                progressItem->setText(1, goal.progress);

                for (auto task: goal.prefixes) {
                    QTreeWidgetItem* taskItem = new QTreeWidgetItem(goalItem);

                    switch (task.prefix) {
                        case GuideData::Work:
                            taskItem->setText(0, tr("Work"));
                            break;
                        case GuideData::Read:
                            taskItem->setText(0, tr("Read"));
                            break;
                        case GuideData::Watch:
                            taskItem->setText(0, tr("Watch/ Listen"));
                            break;
                        case GuideData::Process:
                            taskItem->setText(0, tr("Process"));
                            break;
                        case GuideData::Info:
                            taskItem->setText(0, tr("Information"));
                            break;
                    }
                    taskItem->setText(1, task.prefixText);
                    taskItem->setText(2, task.link);
                }
            }
            continue;
        }
        if (object.objectType == GuideData::Test) {
            QTreeWidgetItem* testItem = new QTreeWidgetItem(mainDisplay);
            testItem->setText(0, tr("Test"));
            testItem->setText(1, object.name);
            testItem->setText(2, object.shortName);

            QTreeWidgetItem* week = new QTreeWidgetItem(testItem);
            week->setText(0, tr("Week"));
            week->setText(1, object.week);

            QTreeWidgetItem* information = new QTreeWidgetItem(testItem);
            information->setText(0, tr("Information"));
            information->setText(1, object.info);

            continue;
        }
        if (object.objectType == GuideData::Report) {
            QTreeWidgetItem* reportItem = new QTreeWidgetItem(mainDisplay);
            reportItem->setText(0, tr("Report"));
            for (auto test: object.tests) {
                QTreeWidgetItem* testItem = new QTreeWidgetItem(reportItem);
                testItem->setText(0, tr("Test"));
                testItem->setText(1, test.name);
                testItem->setText(2, test.weight);
            }
        }
    }
}
