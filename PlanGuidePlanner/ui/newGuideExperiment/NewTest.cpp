//
// Created by Jesse on 08-10-2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_NewTest.h" resolved

#include "NewTest.h"
#include "ui_NewTest.h"
#include "guide/NewGuideData.h"


NewTest::NewTest(QWidget* parent, const NewGuideData::Object* testObject) : QWidget(parent), ui(new Ui::NewTest) {
    ui->setupUi(this);

    ui->name->setText(testObject->name);
    ui->shortName->setText(testObject->shortName);
    ui->week->setText(QString::number(testObject->date.weekNumber()));
    ui->testInfo->setText(testObject->info);

}

NewTest::~NewTest() {
    delete ui;
}