//
// Created by Jesse on 3 okt. 2023.
//

#include <QFile>

#include "guide/GuideData.h"

#include "Guide.h"
#include "Index.h"
#include "Test.h"
#include "Report.h"

#include "ui_Guide.h"
#include "themes/GuidePalette.h"


Guide::Guide(QWidget* parent) : QWidget(parent), ui(new Ui::Guide) {
    ui->setupUi(this);
    updateStyle();
    ui->period->hide();
    ui->period_number->hide();
    ui->subject_name->setGeometry(0, 0, ui->headerFrame->width() - 5, 68);
}

void Guide::updateStyle() {
    GuidePalette palette;

    QString objectTextStyle = QString::fromLatin1("color: %1;").arg(palette.getColor(GuidePalette::ObjectText).name());

    QString frameStyle = QString::fromLatin1("background-color: %1;").arg(palette.color(QPalette::Base).name());

    QString borderColourString = QString::fromLatin1("border-width:3px;border-style:solid;border-color: %1;")
            .arg(palette.color(QPalette::Base).name());

    QString headerStyle = QString::fromLatin1("background-color: %1; color: %2")
            .arg(palette.getColor(GuidePalette::HeaderBackground).name())
            .arg(palette.getColor(GuidePalette::HeaderText).name());

    QString periodStyle = QString::fromLatin1("background-color: %1; color:%2")
            .arg(palette.getColor(GuidePalette::TestBackground).name())
            .arg(palette.getColor(GuidePalette::HeaderText).name());


    QString workIndicatorStyle = QString::fromLatin1("background-color: %1;color:%2;")
                                 .arg(palette.getColor(GuidePalette::WorkIndicatorBackground).name())
                                 .arg(palette.getColor(GuidePalette::WorkIndicatorText).name())
                                 + borderColourString;

    QString workIndicatorTextStyle = QString::fromLatin1("background-color: %1;color:%2;")
                                     .arg(palette.getColor(GuidePalette::WorkIndicatorExample).name())
                                     .arg(palette.getColor(GuidePalette::WorkIndicatorText).name())
                                     + borderColourString;


    QString watchIndicatorStyle = QString::fromLatin1("background-color: %1;color:%2;")
                                  .arg(palette.getColor(GuidePalette::WatchIndicatorBackground).name())
                                  .arg(palette.getColor(GuidePalette::WatchIndicatorText).name())
                                  + borderColourString;

    QString watchIndicatorTextStyle = QString::fromLatin1("background-color: %1;color:%2;")
                                      .arg(palette.getColor(GuidePalette::WatchIndicatorExample).name())
                                      .arg(palette.getColor(GuidePalette::WatchIndicatorText).name())
                                      + borderColourString;


    QString readIndicatorStyle = QString::fromLatin1("background-color: %1;color:%2;")
                                 .arg(palette.getColor(GuidePalette::ReadIndicatorBackground).name())
                                 .arg(palette.getColor(GuidePalette::ReadIndicatorText).name())
                                 + borderColourString;

    QString readIndicatorTextStyle = QString::fromLatin1("background-color: %1;color:%2;")
                                     .arg(palette.getColor(GuidePalette::ReadIndicatorExample).name())
                                     .arg(palette.getColor(GuidePalette::ReadIndicatorText).name())
                                     + borderColourString;


    QString processIndicatorStyle = QString::fromLatin1("background-color: %1;color:%2;")
                                    .arg(palette.getColor(GuidePalette::ProcessIndicatorBackground).name())
                                    .arg(palette.getColor(GuidePalette::ProcessIndicatorText).name())
                                    + borderColourString;

    QString processIndicatorTextStyle = QString::fromLatin1("background-color: %1;color:%2;")
                                        .arg(palette.getColor(GuidePalette::ProcessIndicatorExample).name())
                                        .arg(palette.getColor(GuidePalette::ProcessIndicatorText).name())
                                        + borderColourString;

    QString infoIndicatorStyle = QString::fromLatin1("background-color: %1;color:%2;")
                                 .arg(palette.getColor(GuidePalette::InfoIndicatorBackground).name())
                                 .arg(palette.getColor(GuidePalette::InfoIndicatorText).name())
                                 + borderColourString;

    QString infoIndicatorTextStyle = QString::fromLatin1("background-color: %1;color:%2;")
                                     .arg(palette.getColor(GuidePalette::InfoIndicatorExample).name())
                                     .arg(palette.getColor(GuidePalette::InfoIndicatorText).name())
                                     + borderColourString;


    ui->infoFrame->setStyleSheet(frameStyle);
    ui->headerFrame->setStyleSheet(headerStyle);
    ui->period_number->setStyleSheet(periodStyle);
    ui->mainInfoLabel->setStyleSheet(objectTextStyle);
    ui->period->setStyleSheet(headerStyle);

    ui->work_indicator_name->setStyleSheet(workIndicatorTextStyle);
    ui->work_indicator_example->setStyleSheet(workIndicatorStyle);

    ui->wl_indicator_name->setStyleSheet(watchIndicatorTextStyle);
    ui->wl_indicator_example->setStyleSheet(watchIndicatorStyle);

    ui->read_indicator_name->setStyleSheet(readIndicatorTextStyle);
    ui->read_indicator_example->setStyleSheet(readIndicatorStyle);

    ui->process_indicator_name->setStyleSheet(processIndicatorTextStyle);
    ui->process_indicator_example->setStyleSheet(processIndicatorStyle);

    ui->info_indicator_name->setStyleSheet(infoIndicatorTextStyle);
    ui->info_indicator_example->setStyleSheet(infoIndicatorStyle);
}

Guide::~Guide() {
    delete ui;
    for (int i = 0; i < indexes.size(); i++) {
        delete indexes.at(i);
    }
    for (int i = 0; i < tests.size(); i++) {
        delete tests.at(i);
    }
    for (int i = 0; i < reports.size(); i++) {
        delete reports.at(i);
    }
}


void Guide::addIndex(Index* index) {
    indexes.append(index);
    objectOrder.append(GuideData::Index);
    index->setGeometry(30, size, 1240, index->size + 5);
    index->show();
    size += (index->size + 35);
    resize(1285, size);
}


void Guide::addTest(Test* test) {
    tests.append(test);
    objectOrder.append(GuideData::Test);
    test->setGeometry(50, size, 1260, test->size);
    test->show();
    size += test->size + 30;
    resize(1285, size);
}

void Guide::addReport(Report* report) {
    reports.append(report);
    objectOrder.append(GuideData::Report);
    report->setGeometry(30, size, 1240, report->size + 5);
    report->show();
    size += report->size + 35;
    resize(1285, size);
}

void Guide::setName(const QString&nameE) {
    name = nameE;
    ui->subject_name->setText(name);
}

void Guide::setInfo(const QString&info) {
    ui->info->setText(info);
}

void Guide::setPeriod(const QString&period) {
    if (period.isEmpty()) return;
    ui->period->show();
    ui->period_number->show();
    ui->subject_name->setGeometry(0, 0, 950, 68);
    ui->period_number->setText(period);
}

void Guide::setShortName(const QString&shortNameE) {
    shortName = shortNameE;
}

GuideData::Data Guide::getGuide() {
    GuideData::Data finalGuide;
    int currentIndex = 0;
    int currentTest = 0;
    int currentReport = 0;

    finalGuide.name = ui->subject_name->text();
    finalGuide.shortName = shortName;
    finalGuide.period = ui->period_number->text();
    finalGuide.info = ui->info->text();
    finalGuide.originalFile = originalFile;
    finalGuide.autoSaveFile = autoSaveFile;

    for (GuideData::ObjectTypes type: objectOrder) {
        if (type == GuideData::Index) {
            Index&index = *indexes.at(currentIndex);

            finalGuide.objects.append(index.getGuideObject());
            currentIndex++;
        }
        if (type == GuideData::Test) {
            Test&test = *tests.at(currentTest);

            finalGuide.objects.append(test.getGuideobject());
            currentTest++;
        }
        if (type == GuideData::Report) {
            Report&report = *reports.at(currentReport);

            finalGuide.objects.append(report.getGuideobject());
            currentReport++;
        }
    }

    return finalGuide;
}

void Guide::setGuide(GuideData::Data guide) {
    // Default things
    QString name = guide.name;
    QString shortName = guide.shortName;
    setName(name);
    setShortName(shortName);
    setInfo(guide.info);
    setPeriod(guide.period);
    originalFile = guide.originalFile;
    autoSaveFile = guide.autoSaveFile;


    //Processing the guide objects
    for (GuideData::GuideObject guideObject: guide.objects) {
        if (guideObject.objectType == GuideData::Index) {
            Index* index = new Index(this);
            for (GuideData::GuideGoals goal: guideObject.goals) {
                Goal* finalGoal = new Goal(index);
                finalGoal->setName(goal.name);
                finalGoal->setProgress(goal.progress, false);
                finalGoal->setTime(goal.time);
                finalGoal->setWeek(goal.week);
                finalGoal->setGoalNumber(goal.goalNumber);
                finalGoal->parentGuide = this;

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
            addIndex(index);
        }
        if (guideObject.objectType == GuideData::Test) {
            Test* test = new Test(this);
            test->setName(guideObject.name);
            test->setWeek(guideObject.week);
            test->setInfo(guideObject.info);
            test->setShortName(guideObject.shortName);
            test->finalise();
            addTest(test);
        }
        if (guideObject.objectType == GuideData::Report) {
            Report* report = new Report(this);
            for (GuideData::ReportTests test: guideObject.tests)
                report->addTest(test.name, test.weight);
            report->finalise();
            addReport(report);
        }
    }
}

void Guide::emptyGuide() {
    // Deleting...
    for (auto index: indexes) {
        delete index;
    }
    for (auto test: tests) {
        delete test;
    }
    for (auto report: reports) {
        delete report;
    }
    // Clearing
    indexes.clear();
    tests.clear();
    reports.clear();
    objectOrder.clear();

    // Resetting
    size = defaultSize;
    name = "";
    shortName = "";

    originalFile = QFileInfo("");
    autoSaveFile = QFileInfo("");

    ui->period->hide();
    ui->period_number->hide();
    ui->subject_name->setGeometry(0, 0, ui->headerFrame->width() - 5, 68);
    ui->subject_name->setText(name);
    ui->info->setText("");
    ui->period_number->setText("");
}
