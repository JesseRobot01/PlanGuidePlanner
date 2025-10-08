//
// Created by Jesse on 08-10-2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_NewTest.h" resolved

#include "NewTest.h"
#include "ui_NewTest.h"
#include "guide/NewGuideData.h"
#include "themes/GuidePalette.h"


NewTest::NewTest(QWidget* parent, const NewGuideData::Object* testObject) : QWidget(parent), ui(new Ui::NewTest) {
    ui->setupUi(this);

    ui->name->setText(testObject->name);
    ui->shortName->setText(testObject->shortName);
    ui->week->setText(QString::number(testObject->date.weekNumber()));
    ui->testInfo->setText(testObject->info);

    updateStyle();

}

NewTest::~NewTest() {
    delete ui;
}

void NewTest::updateStyle() {
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