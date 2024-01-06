//
// Created by Jesse on 3 okt. 2023.
//

#include "Test.h"
#include "ui_Test.h"

Test::Test(QWidget* parent) : QWidget(parent), ui(new Ui::Test) {
    ui->setupUi(this);
}

Test::~Test() {
    delete ui;
}

void Test::setName(const QString&name) {
    ui->testName->setText(name);
}

void Test::setShortName(const QString&shortName) {
    ui->shortTestName->setText(shortName);
}

void Test::setInfo(const QString&info) {
    ui->testInfo->setText(info);
}

void Test::setWeek(const QString&week) {
    ui->testWeek->setText(week);
}

void Test::finalise() {
    // first, get the size of the info string.
    QString* info = new QString(ui->testInfo->text());
    int newLineCount = (int)info->count("\n");

    if (!info->isEmpty()) {
        ui->testInfo->resize(1200, newLineCount * 17 + 40);
        size = newLineCount * 17 + 40;
        if (newLineCount > 0)
            ui->testInfo->setAlignment(Qt::AlignHCenter);


        return;
    }
    else {
        size = 40;
        ui->testInfo->resize(0, 0);
    }
}

GuideData::GuideObject Test::getGuideobject() {
    GuideData::GuideObject object;
    object.objectType = GuideData::Test;
    object.setTestName(ui->testName->text());
    object.setShortTestName(ui->shortTestName->text());
    object.setTestInfo(ui->testInfo->text());
    object.setTestWeek(ui->testWeek->text());

    return object;
}
