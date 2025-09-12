//
// Created by Jesse on 3 okt. 2023.
//

#include "Test.h"
#include "ui_Test.h"
#include "themes/GuidePalette.h"

Test::Test(QWidget *parent) : QWidget(parent), ui(new Ui::Test) {
    ui->setupUi(this);
    updateStyle();
}

void Test::updateStyle() {
    GuidePalette palette;
    QString style =
            QString::fromLatin1(
                    "background-color: %1; border-width: 3px; border-style: solid; border-color: %2;color:%3;")
                    .arg(palette.getColor(GuidePalette::TestBackground).name())
                    .arg(palette.color(QPalette::Base).name())
                    .arg(palette.getColor(GuidePalette::HeaderText).name());

    ui->shortTestName->setStyleSheet(style);
    ui->testName->setStyleSheet(style);
    ui->testWeek->setStyleSheet(style);
    ui->testInfo->setStyleSheet(style);
}

Test::~Test() {
    delete ui;
}

void Test::setName(const QString &name) {
    ui->testName->setText(name);
}

void Test::setShortName(const QString &shortName) {
    ui->shortTestName->setText(shortName);
}

void Test::setInfo(const QString &info) {
    ui->testInfo->setText(info);
}

void Test::setWeek(const QString &week) {
    ui->testWeek->setText(week);
}

void Test::finalise() {
    // first, get the size of the info string.
    QString *info = new QString(ui->testInfo->text());
    int newLineCount = (int) info->count("\n");

    if (!info->isEmpty()) {
        ui->testInfo->resize(1200, newLineCount * 20 + 40);
        size = newLineCount * 20 + 80;
        if (newLineCount > 0)
            ui->testInfo->setAlignment(Qt::AlignCenter);

        return;
    } else {
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
