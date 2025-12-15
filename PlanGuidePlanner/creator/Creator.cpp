//
// Created by Jesse on 16-06-2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_CreatorMainWindow.h" resolved

#include "Creator.h"

#include <QFileDialog>
#include <JlCompress.h>

#include "ui_Creator.h"
#include "guide/GuideData.h"
#include <ui/guide/Guide.h>

#include "Application.h"
#include "XmlParser.h"
#include "themes/GuidePalette.h"


Creator::Creator(QWidget* parent) : QMainWindow(parent), ui(new Ui::Creator) {
    ui->setupUi(this);

    addTask = new QListWidgetItem(ui->addList);
    addTask->setText(tr("Add Goal Task"));

    addReportTest = new QListWidgetItem(ui->addList);
    addReportTest->setText(tr("Add Test To Report"));

    listSpacer = new QListWidgetItem(ui->addList);

    addGoal = new QListWidgetItem(ui->addList);
    addGoal->setText(tr("Add Goal"));

    addTest = new QListWidgetItem(ui->addList);
    addTest->setText(tr("Add Test"));

    addReport = new QListWidgetItem(ui->addList);
    addReport->setText(tr("Add Report"));

    addBreak = new QListWidgetItem(ui->addList);
    addBreak->setText(tr("Add Break"));


    //Restore to initial configuration
    hideAllOptions();

    showShortEdit("", tr("Subject Name"));
    showExtraEdit("", tr("Short Name"));

    setActionButtons(ui->mainDisplay->currentItem());
}

Creator::~Creator() {
    delete ui;
}

void Creator::hideAllOptions() {
    addTask->setHidden(true);
    listSpacer->setHidden(true);
    addReportTest->setHidden(true);
    listSpacer->setHidden(true);

    ui->typeSelector->setEnabled(false);
    ui->typeSelector->hide();
    ui->typeSelectorLabel->hide();

    ui->shortEdit->setEnabled(false);
    ui->shortEdit->hide();
    ui->shortEditLabel->hide();

    ui->extraEdit->setEnabled(false);
    ui->extraEdit->hide();
    ui->extraEditLabel->hide();

    ui->longEdit->setEnabled(false);
    ui->longEdit->hide();
    ui->longEditLabel->hide();

    ui->progressSlider->setEnabled(false);
    ui->progressSlider->hide();
    ui->progressLabel->hide();

    ui->dateEdit->setEnabled(false);
    ui->dateEdit->hide();
    ui->dateEditLabel->hide();

    ui->timeEdit->setEnabled(false);
    ui->timeEdit->hide();
    ui->timeEditLabel->hide();
}

void Creator::showAddTask() {
    addTask->setHidden(false);
    listSpacer->setHidden(false);
}


void Creator::showAddReportTest() {
    addReportTest->setHidden(false);
    listSpacer->setHidden(false);
}


void Creator::showTypeSelector(QString text) {
    ui->typeSelector->setCurrentText(text);
    ui->typeSelector->setEnabled(true);
    ui->typeSelector->show();
    ui->typeSelectorLabel->show();
}


void Creator::showShortEdit(QString text, QString labelName) {
    ui->shortEdit->setText(text);
    ui->shortEdit->setEnabled(true);
    ui->shortEditLabel->setText(labelName);
    ui->shortEdit->show();
    ui->shortEditLabel->show();
}


void Creator::showExtraEdit(QString text, QString labelName) {
    ui->extraEdit->setText(text);
    ui->extraEdit->setEnabled(true);
    ui->extraEditLabel->setText(labelName);
    ui->extraEdit->show();
    ui->extraEditLabel->show();
}


void Creator::showLongEdit(QString text, QString labelName) {
    ui->longEdit->setPlainText(text);
    ui->longEdit->setEnabled(true);
    ui->longEditLabel->setText(labelName);
    ui->longEdit->show();
    ui->longEditLabel->show();
}


void Creator::showProgressSlider(int value) {
    ui->progressSlider->setValue(value);
    ui->progressSlider->setEnabled(true);
    setProgressSliderColour(value);
    ui->progressSlider->show();
    ui->progressLabel->show();
}


void Creator::showDateEdit(QString date) {
    ui->dateEdit->setDate(QDate::fromString(date, Qt::RFC2822Date));
    ui->dateEdit->setEnabled(true);
    ui->dateEdit->show();
    ui->dateEditLabel->show();
}


void Creator::showTimeEdit(QString value) {
    ui->timeEdit->setValue(value.toInt());
    ui->timeEdit->setEnabled(true);
    ui->timeEdit->show();
    ui->timeEditLabel->show();
}

void Creator::on_mainDisplay_itemClicked(QTreeWidgetItem* item, int column) {
    const QString type = item->text(0);
    const QString mainText = item->text(1);
    const QString extraText = item->text(2);

    hideAllOptions();

    // Enable / Disable actions
    setActionButtons(item);

    //goals
    if (type == tr("Goal")) {
        //goal
        showShortEdit(mainText, tr("Name"));
        showExtraEdit(extraText, tr("Number"));
        showAddTask();

        return;
    }
    // For the goal prefixes
    if (type == tr("Work") ||
        type == tr("Watch/ Listen") ||
        type == tr("Read") ||
        type == tr("Process") ||
        type == tr("Information") &&
        item->parent()->text(0) == tr("Goal")) // big if :)

    {
        showTypeSelector(type);
        showShortEdit(mainText, tr("Goal"));
        showExtraEdit(extraText, tr("Link"));
        showAddTask();

        return;
    }
    if (type == tr("Date & Duration")) {
        //goal time

        showDateEdit(mainText);
        showTimeEdit(extraText);
        showAddTask();
        return;
    }
    if (type == tr("Date")) {
        // this comment used to be usefull, but lost his purpose
        showDateEdit(mainText);
        return;
    }
    if (type == tr("Progress")) {
        //goal progress
        showAddTask();
        showProgressSlider(mainText.toInt());
        return;
    } // Test
    if (type == tr("Test")) {
        //Main Test (also works with report test)
        showShortEdit(mainText, tr("Name"));
        showExtraEdit(extraText, tr("Number"));

        if (item->parent() != nullptr) {
            showAddReportTest();
        }
        return;
    }
    // report
    if (type == tr("Report")) {
        showAddReportTest();
        return;
    }
    if (type == tr("Information") && item->parent()->text(0) != tr("Goal")) {
        // Also used for the main info
        showLongEdit(mainText, tr("Info"));
        return;
    }
    //Main Guide
    if (type == tr("Guide")) {
        showShortEdit(mainText, tr("Subject Name"));
        showExtraEdit(extraText, tr("Short Name"));
        return;
    }
    if (type == tr("Period")) {
        showShortEdit(mainText, tr("Period"));
        return;
    }
}

void Creator::setActionButtons(QTreeWidgetItem* item) {
    if (!item) {
        ui->upButton->setEnabled(false);
        ui->downButton->setEnabled(false);
        ui->deleteButton->setEnabled(false);
        return;
    }

    bool manipulatable = canBeManipulated(item);
    bool canGoUp = false;
    bool canGoDown = false;

    // Check per button;
    if (manipulatable) {
        QTreeWidgetItem* parent = item->parent();

        if (parent) {
            // It's a child item
            int index = parent->indexOfChild(item);
            if (index > 0) {
                QTreeWidgetItem* above = parent->child(index - 1);
                if (above && canBeManipulated(above)) {
                    canGoUp = true;
                }
            }
            QTreeWidgetItem* below = parent->child(index + 1);
            if (below && canBeManipulated(below)) {
                canGoDown = true;
            }
        }
        else {
            // It's a top-level item
            int index = ui->mainDisplay->indexOfTopLevelItem(item);
            if (index > 0) {
                QTreeWidgetItem* above = ui->mainDisplay->topLevelItem(index - 1);
                if (above && canBeManipulated(above)) {
                    canGoUp = true;
                }
            }
            QTreeWidgetItem* below = ui->mainDisplay->topLevelItem(index + 1);
            if (below && canBeManipulated(below)) {
                canGoDown = true;
            }
        }
    }

    ui->upButton->setEnabled(canGoUp);
    ui->downButton->setEnabled(canGoDown);
    ui->deleteButton->setEnabled(manipulatable || !isDefaultObject(item));
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
    /*QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    QScrollArea* scrollArea = new QScrollArea(widget);


    scrollArea->setWidget(guide);
    layout->addWidget(scrollArea);
    widget->setLayout(layout);
    widget->show();*/

    GuideData::Data data = getCurrentGuide();
    Guide* guide = new Guide(nullptr, &data);
    guide->show();
}

GuideData::Data Creator::getCurrentGuide() {
    GuideData::Data guidedata;

    for (int i = 0; i < ui->mainDisplay->topLevelItemCount(); ++i) {
        QTreeWidgetItem* item = ui->mainDisplay->topLevelItem(i);

        if (item->text(0) == tr("Goal")) {
            GuideData::Object goal;
            goal.type = GuideData::Goal;

            goal.name = item->text(1);
            goal.number = item->text(2);
            for (int j = 0; j < item->childCount(); ++j) {
                QTreeWidgetItem* child = item->child(j);

                if (child->text(0) == tr("Work"))
                    goal.addTask(child->text(1), GuideData::Work, child->text(2));

                else if (child->text(0) == tr("Read"))
                    goal.addTask(child->text(1), GuideData::Read, child->text(2));

                else if (child->text(0) == tr("Watch/ Listen"))
                    goal.addTask(child->text(1), GuideData::Watch, child->text(2));

                else if (child->text(0) == tr("Process"))
                    goal.addTask(child->text(1), GuideData::Process, child->text(2));

                else if (child->text(0) == tr("Information"))
                    goal.addTask(child->text(1), GuideData::Info, child->text(2));

                else if (child->text(0) == tr("Week"))
                    goal.setDateFromWeek(child->text(1));

                else if (child->text(0) == tr("Progress"))
                    goal.setProgressFromInt(child->text(1).toInt());

                else if (child->text(0) == tr("Date & Duration")) {
                    goal.date = QDate::fromString(child->text(1), Qt::RFC2822Date);
                    goal.time = child->text(2).toInt();
                }
            }

            guidedata.objects.append(goal);
        }

        if (item->text(0) == tr("Test")) {
            GuideData::Object test;
            test.type = GuideData::Test;
            test.name = (item->text(1));
            test.number = (item->text(2));

            for (int j = 0; j < item->childCount(); ++j) {
                QTreeWidgetItem* child = item->child(j);

                if (child->text(0) == tr("Date"))
                    test.date = QDate::fromString(child->text(1), Qt::RFC2822Date);

                else if (child->text(0) == tr("Information"))
                    test.info = child->text(1);
            }

            guidedata.objects.append(test);
            continue;
        }

        if (item->text(0) == tr("Report")) {
            GuideData::Object report;
            report.type = GuideData::Report;

            for (int j = 0; j < item->childCount(); ++j) {
                QTreeWidgetItem* child = item->child(j);
                if (child->text(0) == tr("Test")) {
                    GuideData::ReportTest reportTest;
                    reportTest.name = child->text(1);
                    reportTest.weight = child->text(2).toInt();
                    reportTest.weightType = child->text(3);

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
        if (item->text(0) == tr("Break")) {
            GuideData::Object breakObj;
            breakObj.type = GuideData::Break;

            guidedata.objects.append(breakObj);
            continue;
        }
    }
    return guidedata;
}

void Creator::on_addList_itemDoubleClicked(QListWidgetItem* item) {
    if (item->text() == tr("Add Goal")) {
        QTreeWidgetItem* topItem = new QTreeWidgetItem(ui->mainDisplay);
        topItem->setText(0, tr("Goal"));

        QTreeWidgetItem* timeItem = new QTreeWidgetItem(topItem);
        timeItem->setText(0, tr("Date & Duration"));
        timeItem->setText(1, QDate(2024, 01, 01).toString(Qt::RFC2822Date));
        timeItem->setText(2, "0");

        QTreeWidgetItem* progressItem = new QTreeWidgetItem(topItem);
        progressItem->setText(0, tr("Progress"));
        progressItem->setText(1, "0");

        ui->mainDisplay->setCurrentItem(topItem);
        on_mainDisplay_itemClicked(topItem, 0);
        return;
    }

    if (item->text() == tr("Add Test")) {
        QTreeWidgetItem* topItem = new QTreeWidgetItem(ui->mainDisplay);
        topItem->setText(0, tr("Test"));

        QTreeWidgetItem* week = new QTreeWidgetItem(topItem);
        week->setText(0, tr("Date"));
        week->setText(1, QDate(2024, 01, 01).toString(Qt::RFC2822Date));

        QTreeWidgetItem* information = new QTreeWidgetItem(topItem);
        information->setText(0, tr("Information"));

        ui->mainDisplay->setCurrentItem(topItem);
        on_mainDisplay_itemClicked(topItem, 0);
        return;
    }

    if (item->text() == tr("Add Report")) {
        QTreeWidgetItem* topItem = new QTreeWidgetItem(ui->mainDisplay);
        topItem->setText(0, tr("Report"));

        QTreeWidgetItem* testItem = new QTreeWidgetItem(topItem);
        testItem->setText(0, tr("Test"));

        ui->mainDisplay->setCurrentItem(topItem);
        on_mainDisplay_itemClicked(topItem, 0);
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

        ui->mainDisplay->setCurrentItem(goalTask);
        on_mainDisplay_itemClicked(goalTask, 0);
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

        ui->mainDisplay->setCurrentItem(reportTest);
        on_mainDisplay_itemClicked(reportTest, 0);
        return;
    }

    if (item->text() == tr("Add Break")) {
        QTreeWidgetItem* topItem = new QTreeWidgetItem(ui->mainDisplay);
        topItem->setText(0, tr("Break"));

        ui->mainDisplay->setCurrentItem(topItem);
        on_mainDisplay_itemClicked(topItem, 0);

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

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open PlanGuidePlanner"),
                                                    settings.value("LastOpenedDir", ".").toString(),
                                                    tr(
                                                        "All Supported Files (*.pgd *.pgx *.xml);;Plan Guide Planner Document (*.pgd);;Plan Guide Planner XML-based file (*.pgx);;*.Xml Files (*.xml);;All Files (*)"));

    if (fileName.isEmpty()) return;
    QFileInfo file(fileName);
    settings.setValue("LastOpenedDir", file.path());


    if (APPLICATION->isZipFile(file.fileName())) {
        qDebug() << "Found zip-based file:" << file << ". Extracting...";
        QDir tempDir(QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/planguideplanner");

        QStringList extractedFiles = JlCompress::extractDir(file.filePath(), tempDir.absolutePath());

        for (const QString&extractedFile: extractedFiles) {
            qDebug() << "Extracted:" << file;
            if (APPLICATION->isXmlFile(extractedFile)) {
                open(XmlParser::readXml(extractedFile));
                // We're done!
                if (tempDir.exists()) {
                    tempDir.removeRecursively();
                }
                return;
            }
        }
    }
    else if (APPLICATION->isXmlFile(file.fileName()))
        open(XmlParser::readXml(fileName));
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

    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save PlanGuidePlanner"),
                                                        baseFileName,
                                                        tr(
                                                            "Plan Guide Planner Document (*.pgd);;Plan Guide Planner XML-based file (*.pgx);;*.Xml Files (*.xml);;All Files (*)"));

    if (saveFileName.isEmpty()) {
        qWarning() << "No save file given. Can't save";
        return;
    }
    currentGuide = QFileInfo(saveFileName);
    save(guide);
}

void Creator::save(GuideData::Data guide) {
    QFile fileToSave(currentGuide.absoluteFilePath());

    if (fileToSave.fileName().endsWith("pgd")) {
        QDir tempLocation = (QStandardPaths::writableLocation(QStandardPaths::TempLocation));

        // saving
        QFileInfo fileInfoToSave(tempLocation.absoluteFilePath(guide.shortName + "_0" + ".pgx"));

        QString baseName = guide.shortName;
        QString candidateName = baseName + ".pgx";
        QString fullPath = tempLocation.absoluteFilePath(candidateName);
        fileInfoToSave = QFileInfo(fullPath);

        QFile xmlFileToSave(fileInfoToSave.absoluteFilePath());
        XmlParser::saveXml(guide, xmlFileToSave, false, false);


        // Zip them!
        if (JlCompress::compressFiles(fileToSave.fileName(), QStringList(xmlFileToSave.fileName()))) {
            qDebug() << "File zipped successfully!";
        }
        else {
            qCritical() << "Failed to zop files!";
        }

        // Cleanup!
        if (tempLocation.exists()) {
            tempLocation.removeRecursively();
        }
        return;
    }
    else if (fileToSave.fileName().endsWith("pgx"))
        XmlParser::saveXml(guide, fileToSave, false, false);
    else
        XmlParser::saveXml(guide, fileToSave);
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
        if (object.type == GuideData::Goal) {
            QTreeWidgetItem* goalItem = new QTreeWidgetItem(mainDisplay);
            goalItem->setText(0, tr("Goal"));
            goalItem->setText(1, object.name);
            goalItem->setText(2, object.number);


            QTreeWidgetItem* timeItem = new QTreeWidgetItem(goalItem);
            timeItem->setText(0, tr("Date & Duration"));
            timeItem->setText(1, object.date.toString(Qt::RFC2822Date));
            timeItem->setText(2, QString::number(object.time));

            QTreeWidgetItem* progressItem = new QTreeWidgetItem(goalItem);
            progressItem->setText(0, tr("Progress"));
            progressItem->setText(1, QString::number(object.progress));

            for (auto task: object.tasks) {
                QTreeWidgetItem* taskItem = new QTreeWidgetItem(goalItem);

                switch (task.task) {
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
                taskItem->setText(1, task.text);
            }
            continue;
        }
        if (object.type == GuideData::Test) {
            QTreeWidgetItem* testItem = new QTreeWidgetItem(mainDisplay);
            testItem->setText(0, tr("Test"));
            testItem->setText(1, object.name);
            testItem->setText(2, object.number);

            QTreeWidgetItem* week = new QTreeWidgetItem(testItem);
            week->setText(0, tr("Date"));
            week->setText(1, object.date.toString(Qt::RFC2822Date));

            QTreeWidgetItem* information = new QTreeWidgetItem(testItem);
            information->setText(0, tr("Information"));
            information->setText(1, object.info);

            continue;
        }
        if (object.type == GuideData::Report) {
            QTreeWidgetItem* reportItem = new QTreeWidgetItem(mainDisplay);
            reportItem->setText(0, tr("Report"));
            for (auto test: object.tests) {
                QTreeWidgetItem* testItem = new QTreeWidgetItem(reportItem);
                testItem->setText(0, tr("Test"));
                testItem->setText(1, test.name);
                testItem->setText(2, QString::number(test.weight));
                testItem->setText(3, test.weightType);
            }
        }
        if (object.type == GuideData::Break) {
            QTreeWidgetItem* reportItem = new QTreeWidgetItem(mainDisplay);
            reportItem->setText(0, tr("Break"));
        }
    }

    showShortEdit(guide.name, tr("Subject Name"));
    showExtraEdit(guide.shortName, tr("Short Name"));
}

void Creator::on_upButton_clicked() {
    QTreeWidgetItem* current = ui->mainDisplay->currentItem();
    if (!current || !canBeManipulated(current))
        return;

    QTreeWidgetItem* parent = current->parent();

    if (parent) {
        // It's a child item
        int index = parent->indexOfChild(current);
        if (index > 0) {
            QTreeWidgetItem* above = parent->child(index - 1);
            if (above && canBeManipulated(above)) {
                parent->takeChild(index);
                parent->insertChild(index - 1, current);
                ui->mainDisplay->setCurrentItem(current);
            }
        }
    }
    else {
        // It's a top-level item
        int index = ui->mainDisplay->indexOfTopLevelItem(current);
        if (index > 0) {
            QTreeWidgetItem* above = ui->mainDisplay->topLevelItem(index - 1);
            if (above && canBeManipulated(above)) {
                ui->mainDisplay->takeTopLevelItem(index);
                ui->mainDisplay->insertTopLevelItem(index - 1, current);
                ui->mainDisplay->setCurrentItem(current);
            }
        }
    }

    // Reset Buttons
    setActionButtons(current);
}

void Creator::on_downButton_clicked() {
    QTreeWidgetItem* current = ui->mainDisplay->currentItem();
    if (!current || !canBeManipulated(current))
        return;

    QTreeWidgetItem* parent = current->parent();

    if (parent) {
        // It's a child item
        int index = parent->indexOfChild(current);
        QTreeWidgetItem* below = parent->child(index + 1);
        if (below && canBeManipulated(below)) {
            parent->takeChild(index);
            parent->insertChild(index + 1, current);
            ui->mainDisplay->setCurrentItem(current);
        }
    }
    else {
        // It's a top-level item
        int index = ui->mainDisplay->indexOfTopLevelItem(current);
        QTreeWidgetItem* below = ui->mainDisplay->topLevelItem(index + 1);
        if (below && canBeManipulated(below)) {
            ui->mainDisplay->takeTopLevelItem(index);
            ui->mainDisplay->insertTopLevelItem(index + 1, current);
            ui->mainDisplay->setCurrentItem(current);
        }
    }

    // Reset Buttons
    setActionButtons(current);
}

bool Creator::canBeManipulated(QTreeWidgetItem* item) {
    QString name = item->text(0);
    return name == tr("Goal") ||
           name == tr("Work") ||
           name == tr("Watch/ Listen") ||
           name == tr("Read") ||
           name == tr("Process") ||
           // There are multiple instances of information, but only 1 can be manipulated.
           (name == tr("Information") && item->parent()->text(0) == tr("Goal")) ||
           name == tr("Test") || // Also for report tests
           name == tr("Report")||
           name == tr("Break");
}

bool Creator::isDefaultObject(QTreeWidgetItem* item) {
    QString name = item->text(0);
    return name == tr("Guide") ||
           name == tr("Period") ||
           (name == tr("Information") && item->parent()->text(0) == tr("Guide"));
}

void Creator::on_deleteButton_clicked() {
    QTreeWidgetItem* currentItem = ui->mainDisplay->currentItem();
    if (!currentItem)
        return;

    if (canBeManipulated(currentItem))
        delete currentItem; // Aju!
    else if (!isDefaultObject(currentItem))
        delete currentItem->parent(); // Aju parent!

    // Reset Buttons
    setActionButtons(ui->mainDisplay->currentItem());
}

void Creator::on_progressSlider_sliderMoved(int newValue) {
    // Set value to element
    ui->mainDisplay->currentItem()->setText(1, QString::number(newValue));
    setProgressSliderColour(newValue);
}

void Creator::setProgressSliderColour(int progress) {
    QString colour;
    GuidePalette palette;
    GuidePalette::GuideElements element;

    switch (progress) {
        case 2:
            element = GuidePalette::Progress_Finished;
            break;
        case 1:
            element = GuidePalette::Progress_Working;
            break;
        case 0:
            element = GuidePalette::Progress_NotStarted;
            break;
        default:
            element = GuidePalette::HeaderBackground;
            break;
    }
    colour = QString::fromLatin1("background-color:%1;")
            .arg(palette.getColor(element).name());

    ui->progressSlider->setStyleSheet(colour);
}

void Creator::on_dateEdit_dateChanged(QDate date) {
    ui->mainDisplay->currentItem()->setText(1, date.toString(Qt::RFC2822Date));
}

void Creator::on_timeEdit_valueChanged(int newTime) {
    ui->mainDisplay->currentItem()->setText(2, QString::number(newTime));
}
