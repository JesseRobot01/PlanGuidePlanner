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


Guide::Guide(QWidget* parent) : QWidget(parent), ui(new Ui::Guide) {
    ui->setupUi(this);
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
    size += (index->size + 35);
    resize(1285, size);
}

void Guide::addTest(Test* test) {
    tests.append(test);
    objectOrder.append(GuideData::Test);
    test->setGeometry(50, size, 1260, test->size);
    size += test->size + 30;
    resize(1285, size);
}

void Guide::addReport(Report* report) {
    reports.append(report);
    objectOrder.append(GuideData::Report);
    report->setGeometry(30, size, 1240, report->size + 5);
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

