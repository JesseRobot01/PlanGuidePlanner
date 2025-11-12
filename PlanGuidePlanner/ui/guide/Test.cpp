//
// Created by Jesse on 08-10-2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_NewTest.h" resolved

#include "Test.h"

#include "Guide.h"
#include "ui_Test.h"
#include "guide/GuideData.h"
#include "themes/GuidePalette.h"


Test::Test(Guide* parent, const GuideData::Object* testObject) : QWidget(parent), ui(new Ui::Test) {
    ui->setupUi(this);

    if (testObject)
        setTest(*testObject);
    else
        qCritical() << "Test made without object!";
}

void Test::setTest(GuideData::Object testObject) {
    if (testObject.type != GuideData::Test)
        qWarning() << "Object inside Test is not a test!";

    ui->name->setText(testObject.name);
    ui->shortName->setText(testObject.number);
    ui->week->setText(QString::number(testObject.date.weekNumber()));

    if (!testObject.info.isEmpty())
        ui->testInfo->setText(testObject.info);
    else
        ui->testInfo->hide();
}

Test::~Test() {
    delete ui;
}

void Test::updateStyle() {
    GuidePalette palette;
    QString style =
            QString::fromLatin1(
                "background-color: %1; border-width: 3px; border-style: solid; border-color: %2;color:%3;")
            .arg(palette.getColor(GuidePalette::TestBackground).name())
            .arg(palette.color(QPalette::Base).name())
            .arg(palette.getColor(GuidePalette::HeaderText).name());

    ui->shortName->setStyleSheet(style);
    ui->name->setStyleSheet(style);
    ui->week->setStyleSheet(style);
    ui->testInfo->setStyleSheet(style);
}
