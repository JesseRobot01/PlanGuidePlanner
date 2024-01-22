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
}

void Guide::updateStyle() {
    GuidePalette palette;
    QString frameStyle = QString::fromLatin1("background-color: %1;").arg(palette.color(QPalette::Base).name());

    QString borderColourString = QString::fromLatin1("border-width:3px;border-style:solid;border-color: %1;")
            .arg(palette.color(QPalette::Base).name());

    QString HeaderStyle = QString::fromLatin1("background-color: %1;").arg(
        palette.getColor(GuidePalette::HeaderBackground).name());

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


    ui->infoFrame->setStyleSheet(frameStyle);
    ui->headerFrame->setStyleSheet(HeaderStyle);
    ui->period_number->setStyleSheet(periodStyle);

    ui->work_indicator_name->setStyleSheet(workIndicatorTextStyle);
    ui->work_indicator_example->setStyleSheet(workIndicatorStyle);

    ui->wl_indicator_name->setStyleSheet(watchIndicatorTextStyle);
    ui->wl_indicator_example->setStyleSheet(watchIndicatorStyle);

    ui->read_indicator_name->setStyleSheet(readIndicatorTextStyle);
    ui->read_indicator_example->setStyleSheet(readIndicatorStyle);

    ui->process_indicator_name->setStyleSheet(processIndicatorTextStyle);
    ui->process_indicator_example->setStyleSheet(processIndicatorStyle);
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


void Guide::addIndex(Index *index) {
    indexes.append(index);
    objectOrder.append(GuideData::Index);
    index->setGeometry(30, size, 1240, index->size + 5);
    size += (index->size + 35);
    resize(1285, size);
}

void Guide::addTest(Test *test) {
    tests.append(test);
    objectOrder.append(GuideData::Test);
    test->setGeometry(50, size, 1260, test->size);
    size += test->size + 30;
    resize(1285, size);
}

void Guide::addReport(Report *report) {
    reports.append(report);
    objectOrder.append(GuideData::Report);
    report->setGeometry(30, size, 1240, report->size + 5);
    size += report->size + 35;
    resize(1285, size);
}

void Guide::setName(const QString &nameE) {
    name = nameE;
    ui->subject_name->setText(name);
}

void Guide::setInfo(const QString&info) {
    ui->info->setText(info);
}

void Guide::setPeriod(const QString &period) {
    ui->period_number->setText(period);
}

void Guide::setShortName(const QString &shortNameE) {
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

    for (GuideData::ObjectTypes type: objectOrder) {
        if (type == GuideData::Index) {
            Index &index = *indexes.at(currentIndex);

            finalGuide.objects.append(index.getGuideObject());
            currentIndex++;
        }
        if (type == GuideData::Test) {
            Test &test = *tests.at(currentTest);

            finalGuide.objects.append(test.getGuideobject());
            currentTest++;
        }
        if (type == GuideData::Report) {
            Report &report = *reports.at(currentReport);

            finalGuide.objects.append(report.getGuideobject());
            currentReport++;
        }
    }

    return finalGuide;
}

